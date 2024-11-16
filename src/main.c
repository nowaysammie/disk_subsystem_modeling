#include <fifo.h>
#include <fscan.h>
#include <input_data.h>
#include <memreq.h>
#include <sim_info.h>
#include <stdio.h>

/* Функция отображения данных моделирования */
static void display_sim_info(const char *method, unsigned int total_requests, SimulationInfo sim_info)
{
    printf(
        "_____________________\n\n"
        "[Simulation info]\nMethod %s.\n"
        "---------------------\n"
        "reqeust number: %u\n"
        "handled requests: %u\n"
        "min request service time: %f\n"
        "max request service time: %f\n"
        "avg request service time: %f\n"
        "deviation: %f\n"
        "max reqiest queue len: %u\n"
        "total downtime: %f\n"
        "_____________________\n\n",
        method, total_requests, sim_info.handled_requests,
        sim_info.min_request_service_time,
        sim_info.max_request_service_time,
        sim_info.avg_request_service_time, sim_info.deviation,
        sim_info.max_request_queue_len, sim_info.total_downtime);
}

int main()
{
    unsigned int total_requests;
    /* Симуляция от Tmax */
    unsigned int max_interval_time = MAX_INTERVAL_TIME;
    Request *head = generate_memory_requests(max_interval_time, &total_requests);
    /* Моделируем работу для FIFO к сгенерированным запросам от Tmax */
    SimulationInfo sim_info_fifo = simulate_for_fifo(head, total_requests);
    display_sim_info("FIFO (Tmax)", total_requests, sim_info_fifo);
    /* Моделируем работу для FSCAN к сгенерированным запросам от Tmax */
    SimulationInfo sim_info_fscan = simulate_for_fscan(head, total_requests);
    display_sim_info("FSCAN (Tmax)", total_requests, sim_info_fscan);
    free_queue(&head);

    /* Симуляция от (Tmax / 10) */
    max_interval_time = MAX_INTERVAL_TIME / 10;
    head = generate_memory_requests(max_interval_time, &total_requests);
    /* Моделируем работу для FIFO к сгенерированным запросам от (Tmax / 10) */
    sim_info_fifo = simulate_for_fifo(head, total_requests);
    display_sim_info("FIFO (Tmax/10)", total_requests, sim_info_fifo);
    /* Моделируем работу для FSCAN к сгенерированным запросам от (Tmax / 10) */
    sim_info_fscan = simulate_for_fscan(head, total_requests);
    display_sim_info("FSCAN (Tmax/10)", total_requests, sim_info_fscan);
    free_queue(&head);

    /* Симуляция от (Tmax / 100) */
    max_interval_time = MAX_INTERVAL_TIME / 100;
    head = generate_memory_requests(max_interval_time, &total_requests);
    /* Моделируем работу для FIFO к сгенерированным запросам от (Tmax / 100) */
    sim_info_fifo = simulate_for_fifo(head, total_requests);
    display_sim_info("FIFO (Tmax/100)", total_requests, sim_info_fifo);
    /* Моделируем работу для FSCAN к сгенерированным запросам от (Tmax / 100) */
    sim_info_fscan = simulate_for_fscan(head, total_requests);
    display_sim_info("FSCAN (Tmax/100)", total_requests, sim_info_fscan);
    free_queue(&head);
    return 0;
}