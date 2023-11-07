#include "Intersection.h"


Road::Road(RoadDirection dir, int numStraightLanes, int numLeftLanes, int numRightLanes){
    direction = dir;

    for(int i=0; i<numTurnOptions; i++){
        lanesPerTurnOption[i] = 0;
        lights[i] = NULL;
    }

    if(numStraightLanes > 0){
        lanesPerTurnOption[straight] = numStraightLanes;
        lights[straight] = new TrafficLight(TrafficLight::green, 1, 1);
    }
    if(numLeftLanes > 0){
        lanesPerTurnOption[left] = numLeftLanes;
        lights[left] = new TrafficLightLeft(1, 1);
    }
    if(numRightLanes > 0){
        lanesPerTurnOption[right] = numRightLanes;
        lights[right] = new TrafficLight(TrafficLight::greenRight, 1, 1);
    }
}

Road::~Road(){
    for(TrafficLight* light : lights){
        delete light;
    }
}

int Road::getNumLanes(TurnOption opt){
    isValidTurnOption(opt);

    return lanesPerTurnOption[opt];
}

TrafficLight* Road::getLight(TurnOption opt){
    isValidTurnOption(opt);

    return lights[opt];
}