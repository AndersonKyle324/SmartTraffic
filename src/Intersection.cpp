#include "Intersection.h"


Road::Road(RoadDirection dir, int numStraightLanes, int numLeftLanes, int numRightLanes){

}

int Road::getNumLanes(TurnOption opt){
    isValidTurnOption(opt);

    return lanesPerTurnOption[opt];
}

TrafficLight* Road::getLight(TurnOption opt){
    isValidTurnOption(opt);

    if(lanesPerTurnOption[opt] > 0){
        return &lights[opt];
    }

    return NULL;
}