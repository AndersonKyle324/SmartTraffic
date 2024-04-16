#include <iostream>

#include "Timer_Linux.h"
#include "SmartTraffic.h"

int main(int argc, char *argv[]){
    int onDuration = 3;
    /// THIS RIGHT HERE IS WHY THE YELLOW DURATION WASNT WORKING, refreshRateHzGloabl wasn't being set until later and since yellow is never manually set it never updated with the proper rate!!
    refreshRateHzGlobal = 50;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // In a full implementation these would be objects from another Intersection
    inter.setExitRoad(Road::north, new Road(Road::north, {3,4,5}, DEFAULT_ON_DURATION));
    inter.setExitRoad(Road::east, new Road(Road::east, {0,1,0}, DEFAULT_ON_DURATION));
    inter.setExitRoad(Road::west, new Road(Road::west, {2,3,1}, DEFAULT_ON_DURATION));
    inter.setExitRoad(Road::south, new Road(Road::south, {1,2,3}, DEFAULT_ON_DURATION));

    inter.schedule(LightConfig::doubleGreen, Road::north, onDuration);
    inter.schedule(LightConfig::doubleGreenLeft, Road::north, onDuration);
    inter.schedule(LightConfig::doubleGreen, Road::east, onDuration);
    inter.schedule(LightConfig::singleGreen, Road::west, onDuration);

    inter.addMaxVehicles();

    commenceTraffic(inter, 50, 20, true);

    return 0;
}
