#ifndef SMART_TRAFFIC_H
#define SMART_TRAFFIC_H

#include "Intersection.h"

#define FOREVER (-1)
#define DEFAULT_REFRESH_RATE (1)

/**
 * @brief The number of ticks per second.
 */
extern int refreshRateHzGlobal;

/**
 * @brief Starts the main control loop for this Intersection.
 * 
 * @param inter             The Intersection to begin operation. Must be fully defined.
 * @param refreshRateHz     The number of ticks per second
 * @param runTime           The number of seconds this function will run for. A value of FOREVER runs until an interrupt is received.
 * @param printToConsole    Prints Intersection status to console when true
 * 
 * @return true     The function exited normally
 * @return false    "inter" is invalid or there was a clock overrun
 */
bool commenceTraffic(Intersection& inter, int refreshRateHz, int runTime, bool printToConsole);

#endif