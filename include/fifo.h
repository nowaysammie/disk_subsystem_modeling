#ifndef FIFO_H
#define FIFO_H

#include <memreq.h>
#include <sim_info.h>

SimulationInfo simulate_for_fifo(Request *mem_requests, unsigned int total_requests);

#endif