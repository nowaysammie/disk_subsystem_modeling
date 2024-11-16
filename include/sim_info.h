#ifndef SIM_INFO_H
#define SIM_INFO_H

/* Данные о моделировании */
typedef struct SimulationInfo {
    unsigned int handled_requests;      /* Количество обработанных запросов */
    double min_request_service_time;    /* Минимальное время обслуживания запроса */
    double max_request_service_time;    /* Максимальное время обслуживания запроса*/
    double avg_request_service_time;    /* Среднее время обслуживания запроса */
    double deviation;                   /* Среднеквадратичное отклонение от среднего */
    unsigned int max_request_queue_len; /* Максимальная длина очереди запросов */
    double total_downtime;              /* Суммарное время простоя */
} SimulationInfo;

#endif