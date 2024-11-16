#ifndef MEMREQ_H
#define MEMREQ_H

/* Описание запроса */
typedef struct Request {
    unsigned int creation_time; /* Время создания запроса в мс */
    unsigned int track;         /* Номер дорожки */
    unsigned int surface;       /* Номер поверхности */
    unsigned int sector_count;  /* Количество последовательных секторов */
    unsigned int first_sector;  /* Первый сектор для обработки */
    char operation;             /* Тип операции */
    struct Request *next;       /* Указатель на следующий запрос */
} Request;

Request *generate_memory_requests(unsigned int max_interval_time, unsigned int *req_count);

Request *copy_mem_request(Request *mem_request);

void push_req_to_queue(Request **queue, Request *request);

void sort_queue(Request **queue, unsigned int direction);

void free_queue(Request **queue);

#endif