#include <fifo.h>
#include <input_data.h>
#include <math.h>
#include <memreq.h>
#include <sim_info.h>
#include <stdio.h>
#include <stdlib.h>

/* Функция вычисления времени ожидания сектора */
static double waiting_for_sector(unsigned int cur_sector, unsigned int target_sector, double time_for_pass_section)
{
    unsigned int needed_sector;
    if (target_sector == 0 && cur_sector != 0)
        needed_sector = 16;
    else
        needed_sector = target_sector;
    return (abs(needed_sector - cur_sector) * time_for_pass_section);
}

/* Запись данных о времени обслуживания в файл */
static void write_data_to_file(const char *filename, double *request_service_time_arr, unsigned int len)
{
    static unsigned int postfix_id = 0;
    const char *postfix[3] = {"tmax", "tmax_div_10", "tmax_div_100"};
    char filename_new[100];
    sprintf(filename_new, "%s_%s.txt", filename, postfix[postfix_id]);
    FILE *file = fopen(filename_new, "w");
    if (file != NULL) {
        for (int i = 0; i < len; i++)
            fprintf(file, "%f\n", request_service_time_arr[i]);
        fclose(file);
    }
    postfix_id++;
}

/* Функция симуляции обработки запросов с помощью метода FIFO */
SimulationInfo simulate_for_fifo(Request *mem_requests, unsigned int total_requests)
{
    Request *cur_mem_request = mem_requests; /* Текущий запрос */
    Request *mem_request_ptr;                /* Временный указатель на запрос */
    unsigned int cur_track = 0;              /* Текущая дорожка */
    unsigned int cur_sector = 0;             /* Текущий сектор */
    double current_time = 0;                 /* Текущее время */
    /* Структура сбора статистики моделирования */
    SimulationInfo simulation_info;
    simulation_info.max_request_queue_len = 0;
    simulation_info.max_request_service_time = 0;
    simulation_info.handled_requests = 0;
    /* Массив времени обработки запросов */
    double request_service_time_arr[total_requests];
    double downtime = 0;            /* Суммарное время простоя дисковой подсистемы */
    double handle_time = 0;         /* Суммарное время обработки запросов */
    unsigned int request_queue_len; /* Счётчик длины очереди */
    double sectors_pass_time;       /* Время прохождения головки над секторами */
    double request_interval;        /* Интервал между временем создания запроса и текущим временем */
    double request_service_time;    /* Время обслуживания запроса */
    /* Время прохождения головкой одного сектора в мс */
    double time_for_pass_section = (1 / (double)SECTORS_ON_HEAD_COUNT) / ((double)DISK_ROTATION_SPEED / MSEC_IN_MIN);
    /* Выполнение запросов */
    while (current_time <= SIMULATE_TIME && cur_mem_request != NULL) {
        request_interval = (double)cur_mem_request->creation_time - current_time;
        /* Учитывание времени простоя */
        if (request_interval > 0) {
            current_time += request_interval;
            downtime += request_interval;
            continue;
        }
        request_service_time = 0;
        /* Переход на нужную дорожку */
        request_service_time += abs(cur_mem_request->track - cur_track) * (double)HEAD_TRANSITION_TIME;
        cur_track = cur_mem_request->track;
        /* Время ожидания сектора */
        request_service_time += waiting_for_sector(cur_sector, cur_mem_request->first_sector, time_for_pass_section);
        /* Проход над нужными секторами (чтение или запись) */
        sectors_pass_time = cur_mem_request->sector_count * time_for_pass_section;
        request_service_time += sectors_pass_time;
        cur_sector = (cur_mem_request->first_sector + cur_mem_request->sector_count) % SECTORS_ON_HEAD_COUNT;
        /* Верификация в случае записи */
        if (cur_mem_request->operation == 'w') {
            request_service_time += waiting_for_sector(cur_sector, cur_mem_request->first_sector, time_for_pass_section);
            request_service_time += sectors_pass_time;
        }
        /* Выставление максимального и минимального времени обслуживания запроса */
        if (request_service_time > simulation_info.max_request_service_time)
            simulation_info.max_request_service_time = request_service_time;
        if (cur_mem_request->creation_time == 0 || request_service_time < simulation_info.min_request_service_time)
            simulation_info.min_request_service_time = request_service_time;
        /* Учёт времени обслуживания запросов и переход к следующему */
        handle_time += request_service_time;
        request_service_time_arr[simulation_info.handled_requests] = request_service_time;
        current_time += request_service_time;
        /* Подсчёт размера возникшей очереди */
        request_queue_len = 0;
        mem_request_ptr = cur_mem_request->next;
        while (mem_request_ptr != NULL) {
            if (mem_request_ptr->creation_time < current_time) {
                request_queue_len++;
                mem_request_ptr = mem_request_ptr->next;
            }
            else {
                break;
            }
        }
        /* Выставление значения максимальной очереди */
        if (request_queue_len > simulation_info.max_request_queue_len)
            simulation_info.max_request_queue_len = request_queue_len;
        simulation_info.handled_requests++;
        /* Переход к следующему запросу */
        cur_mem_request = cur_mem_request->next;
    }
    /* Запись данных и статистические подсчёты */
    write_data_to_file("output/fifo", request_service_time_arr, simulation_info.handled_requests);
    simulation_info.avg_request_service_time = handle_time / simulation_info.handled_requests;
    double sq_diff_req_and_req_avg = 0.0;
    for (int i = 0; i < simulation_info.handled_requests; i++)
        sq_diff_req_and_req_avg += pow((request_service_time_arr[i] - simulation_info.avg_request_service_time), 2);
    simulation_info.deviation = sqrt((double)sq_diff_req_and_req_avg / (simulation_info.handled_requests - 1));
    simulation_info.total_downtime = downtime;
    return simulation_info;
}