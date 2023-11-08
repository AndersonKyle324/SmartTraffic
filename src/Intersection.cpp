#include "Intersection.h"

Intersection::~Intersection(){
    for(Road* rd : roads){
        delete rd;
    }
}

bool Intersection::roadExists(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);

    return roads[dir] != NULL;
}

bool Intersection::newTurnIsPossible(int numNewLanes, Road::RoadDirection endRoadDir){
    bool turnIsPossible;
    Road::isValidRoadDirection(endRoadDir);

    if(! roadExists(endRoadDir)){
        /// For now we will say the turn is possible but we expect the end road to be populated later
        expectedRoads[endRoadDir] = true;
        turnIsPossible = true;
    }
    else{
        turnIsPossible = (numNewLanes <= roads[endRoadDir]->getNumLanes(Road::straight));
    }

    return turnIsPossible;
}

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions>& numLanesArr){
    Road * newRoad;

    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newTurnIsPossible(numLanesArr[Road::left], Road::west) || ! newTurnIsPossible(numLanesArr[Road::right], Road::east)){
        return turnNotPossible;
    }

    roads[dir] = new Road(dir, numLanesArr[Road::straight], numLanesArr[Road::left], numLanesArr[Road::right]);
}

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