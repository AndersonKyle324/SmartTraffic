#include "Timer_Linux.h"
#include "SmartTraffic.h"

int refreshRateHzGlobal = DEFAULT_REFRESH_RATE;

bool commenceTraffic(Intersection& inter, int refreshRateHz, int runTime, bool printToConsole){
    long long totalSecondsElapsed = 0;
    int secondsSinceLightConfigStart = 0;

    refreshRateHzGlobal = refreshRateHz;

    if( ! inter.validate()){
        return false;
    }

    inter.start();

    while(runTime == FOREVER || totalSecondsElapsed < runTime){
        auto startTime = currentTime();

        if(printToConsole){
            inter.print();
        }

        ///tick() "refreshRateHz" times
        for(int i=0; i < refreshRateHz; i++){
            inter.tick();

            if(oneSecondElapsed(startTime)){
                throw std::runtime_error("commenceTraffic: inter.tick() did not run refreshRateHz times in 1 second\n");
                return false;
            }
        }

        ///while timer has not finished, wait
        while( ! oneSecondElapsed(startTime)){
            waitSleep();
        }

        totalSecondsElapsed++;
        secondsSinceLightConfigStart++;

        if(secondsSinceLightConfigStart > inter.currentLightConfig()->getTotalDuration()){
            inter.nextLightConfig();
            secondsSinceLightConfigStart = 0;
        }
    }

    if(printToConsole){
        inter.print();
    }

    return true;
}
