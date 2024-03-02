#include <iostream>

#include "Timer_Linux.h"
#include "SmartTraffic.h"

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

    commenceTraffic(inter, 50, 20, true);

    return 0;
}
