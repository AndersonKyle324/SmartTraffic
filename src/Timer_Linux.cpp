#include <chrono>
#include <unistd.h>

#include "Timer_Linux.h"

std::chrono::_V2::steady_clock::time_point currentTime(){
    return std::chrono::steady_clock::now();
}

bool oneSecondElapsed(std::chrono::_V2::steady_clock::time_point startTime){
    const std::chrono::duration<double> oneSecond = std::chrono::seconds(1);
    std::chrono::time_point<std::chrono::steady_clock> intervalTime;
    std::chrono::duration<double> elapsedSeconds;

    intervalTime = currentTime();
    elapsedSeconds = intervalTime - startTime;
    
    return elapsedSeconds > oneSecond;
}

void waitSleep(){
    usleep(TWENTIETH_SEC);
}
