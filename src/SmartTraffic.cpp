#include <cstdlib>
#include <iostream>
#include <chrono>
#include <unistd.h>

#include "TrafficLight.h"
#include "Intersection.h"

#define TWENTIETH_SEC (50000)

int refreshRateHzGlobal;

auto currentTime(){
    return std::chrono::steady_clock::now();
}

bool oneSecondElapsed(auto startTime){
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

bool commenceTraffic(Intersection& inter, int refreshRateHz){
    long long secondsElapsed = 0;

    refreshRateHzGlobal = refreshRateHz;

    if( ! inter.validate()){
        return false;
    }

    inter.start();

    while(secondsElapsed < 20){
        auto startTime = currentTime();

        ///tick() "refreshRateHz" times
        for(int i=0; i < refreshRateHz; i++){
            inter.tick();

            if(oneSecondElapsed(startTime)){
                throw std::runtime_error("commenceTraffic: inter.tick() did not run refreshRateHz times in 1 second\n");
            }
        }

        ///while timer has not finished, wait
        while( ! oneSecondElapsed(startTime)){
            waitSleep();
        }

        inter.print();
        secondsElapsed++;
    }

    return true;
}

int main(int argc, char *argv[]){
    ///int numConfigs;
    int onDuration = 3;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    inter.schedule(LightConfig::doubleGreen, Road::north, onDuration);
    inter.schedule(LightConfig::doubleGreenLeft, Road::north, onDuration);
    inter.schedule(LightConfig::doubleGreen, Road::east, onDuration);
    inter.schedule(LightConfig::singleGreen, Road::west, onDuration);

    /*
    inter.start();
    inter.print();

    for(numConfigs=0; numConfigs<6; numConfigs++){
        for(int i=0; i<onDuration; i++){
            inter.tick();
            inter.print();
        }

        for(int j=0; j<DEFAULT_YELLOW_DURATION; j++){
            inter.tick();
            inter.print();
        }

        inter.nextLightConfig();
        inter.print();
    }
    */

    commenceTraffic(inter, 50);

    return 0;
}
