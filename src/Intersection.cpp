#include "Intersection.h"

Intersection::Intersection(){
    for(int i=0; i<Road::numRoadDirections; i++){
        roads[i] = NULL;
        expectedRoads[i] = false;
    }
}

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

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr){
    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newTurnIsPossible(numLanesArr[Road::left], Road::roadClkwiseOf(dir)) || ! newTurnIsPossible(numLanesArr[Road::right], Road::roadCounterClkwiseOf(dir))){
        return turnNotPossible;
    }

    roads[dir] = new Road(dir, numLanesArr);
    expectedRoads[dir] = false;     /// If we were expecting this road before, we now no longer are.

    return success;
}

Road* Intersection::getRoad(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);
    return roads[dir];
}

bool Intersection::roadIsExpected(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);
    return expectedRoads[dir];
}

Road::Road(RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr){
    isValidRoadDirection(dir);
    direction = dir;

    for(int i=0; i<numTurnOptions; i++){
        lanesPerTurnOption[i] = 0;
        lights[i] = NULL;
    }

    if(numLanesArr[left] > 0){
        lanesPerTurnOption[left] = numLanesArr[left];
        lights[left] = new TrafficLightLeft(1, 1);
    }
    if(numLanesArr[straight] > 0){
        lanesPerTurnOption[straight] = numLanesArr[straight];
        lights[straight] = new TrafficLight(TrafficLight::green, 1, 1);
    }
    if(numLanesArr[right] > 0){
        lanesPerTurnOption[right] = numLanesArr[right];
        lights[right] = new TrafficLight(TrafficLight::greenRight, 1, 1);
    }
}

Road::~Road(){
    for(TrafficLight* light : lights){
        delete light;
    }
}

Road::RoadDirection Road::roadClkwiseOf(RoadDirection dir){
    int clkwiseDir = dir + 1;

    if(clkwiseDir == numRoadDirections){
        clkwiseDir = 0;
    }

    isValidRoadDirection((Road::RoadDirection&)clkwiseDir);
    return (Road::RoadDirection)clkwiseDir;
}

Road::RoadDirection Road::roadCounterClkwiseOf(RoadDirection dir){
    int counterClkwiseDir = dir - 1;

    if(counterClkwiseDir < 0){
        counterClkwiseDir = numRoadDirections - 1;
    }

    isValidRoadDirection((Road::RoadDirection&)counterClkwiseDir);
    return (Road::RoadDirection)counterClkwiseDir;
}

int Road::getNumLanes(TurnOption opt){
    isValidTurnOption(opt);

    return lanesPerTurnOption[opt];
}

TrafficLight* Road::getLight(TurnOption opt){
    isValidTurnOption(opt);

    return lights[opt];
}