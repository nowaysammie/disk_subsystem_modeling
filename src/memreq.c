#include <fscan.h>
#include <input_data.h>
#include <memreq.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Функция генерации очереди запросов */
Request *generate_memory_requests(unsigned int max_interval_time, unsigned int *req_count)
{
    /* Устанавливаем seed для генератора рандомных чисел */
    srand((unsigned int)time(NULL));
    unsigned int total_requests = 0; /* Количество сгенерированных запросов */
    unsigned int time_counter = 0;   /* Время генерации запроса */
    Request *mem_request;            /* Текущий запрос */
    Request *first_request = NULL;   /* Первый запрос */
    Request *prev_request = NULL;    /* Предыдущий запрос */
    while (time_counter < SIMULATE_TIME) {
        mem_request = (Request *)malloc(sizeof(Request));
        if (first_request != NULL)
            time_counter += rand() % (max_interval_time + 1);
        mem_request->creation_time = time_counter;
        mem_request->track = rand() % TRACK_COUNT;
        mem_request->surface = rand() % SURFACE_COUNT;
        mem_request->operation = (rand() % 2) ? 'r' : 'w';
        mem_request->sector_count = 1 + rand() % MAX_CONSECUTIVE_SECTORS;
        mem_request->first_sector = rand() % SECTORS_ON_HEAD_COUNT;
        if (first_request == NULL) {
            first_request = mem_request;
        }
        else {
            mem_request->next = NULL;
            prev_request->next = mem_request;
        }
        prev_request = mem_request;
        total_requests++;
    }
    *req_count = total_requests;
    return first_request;
}

/* Функция копирования структуры Request */
Request *copy_mem_request(Request *mem_request)
{
    Request *copied_mem_request = (Request *)malloc(sizeof(Request));
    memcpy(copied_mem_request, mem_request, sizeof(Request));
    copied_mem_request->next = NULL;
    return copied_mem_request;
}

/* Функция добавления запроса в конец очереди */
void push_req_to_queue(Request **queue, Request *request)
{
    if (*queue == NULL) {
        *queue = copy_mem_request(request);
    }
    else {
        Request *last_req = *queue;
        while (last_req->next != NULL)
            last_req = last_req->next;
        last_req->next = copy_mem_request(request);
    }
}

/* Функция сравнения запросов в направлении к центру */
static int cmp_queue_up(const void *a, const void *b)
{
    const Request *aqueue = a, *bqueue = b;
    return (bqueue->track - aqueue->track);
}

/* Функция сравнения запросов в направлении к краю */
static int cmp_queue_down(const void *a, const void *b)
{
    const Request *aqueue = a, *bqueue = b;
    return (aqueue->track - bqueue->track);
}

/* Функция сортировки очереди */
void sort_queue(Request **queue, unsigned int direction)
{
    if (*queue != NULL) {
        /* Подсчёт размера очереди */
        unsigned int size = 0;
        Request *ptr = *queue;
        while (ptr != NULL) {
            ptr = ptr->next;
            size++;
        }
        /* Заполнение массива с заявками */
        Request sorted_queue[size];
        ptr = *queue;
        for (int i = 0; i < size; i++) {
            memcpy(sorted_queue + i, ptr, sizeof(Request));
            ptr = ptr->next;
        }
        /* Сортировка */
        switch (direction) {
            case DIRECTION_TO_CENTER:
                qsort(sorted_queue, size, sizeof(Request), cmp_queue_up);
                break;

            case DIRECTION_TO_EDGE:
                qsort(sorted_queue, size, sizeof(Request), cmp_queue_down);
                break;
        }
        free_queue(queue);
        /* Заполнение очереди отсортированными заявками */
        for (int i = 0; i < size; i++)
            push_req_to_queue(queue, &sorted_queue[i]);
    }
}

/* Функция очистки очереди */
void free_queue(Request **queue)
{
    if (*queue != NULL) {
        Request *ptr = *queue;
        while (*queue != NULL) {
            ptr = (*queue)->next;
            free(*queue);
            *queue = ptr;
        }
    }
}