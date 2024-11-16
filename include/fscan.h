#ifndef FSCAN_H
#define FSCAN_H

#include <memreq.h>
#include <sim_info.h>

/* Коды направления движения головки */
enum {
    DIRECTION_TO_CENTER = 0, /* К центру */
    DIRECTION_TO_EDGE        /* К краю */
};

SimulationInfo simulate_for_fscan(Request *mem_requests, unsigned int total_requests);

#endif