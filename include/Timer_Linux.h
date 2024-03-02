#ifndef TIMER_LINUX_H
#define TIMER_LINUX_H

#include <chrono>

#define TWENTIETH_SEC (50000)

/**
 * @brief Gets current time object
 * 
 * @return auto time object
 */
std::chrono::_V2::steady_clock::time_point currentTime();

/**
 * @brief Determines if a second has passed since "startTime"
 * @note Written without types so that it can be overloaded for other operating systems that
 *      use different system calls.
 * 
 * @param startTime time object from which to determine if a second has passed
 * @return true more than one second has passed since "startTime"
 */
bool oneSecondElapsed(std::chrono::_V2::steady_clock::time_point startTime);

/**
 * @brief Sleep for 1/20th of a second
 * 
 * @note Written without types so that it can be overloaded for other operating systems that
 *      use different system calls.
 */
void waitSleep();

#endif