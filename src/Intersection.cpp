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

bool Intersection::newTurnIsPossible(Road::RoadDirection endRoadDir, int numNewLanes, bool* roadIsExpected){
    bool turnIsPossible;
    Road::isValidRoadDirection(endRoadDir);

    if(numNewLanes <= 0){
        turnIsPossible = true;
        *roadIsExpected = false;
    }
    else if(! roadExists(endRoadDir)){
        /// For now we will say the turn is possible but we expect the end road to be populated later
        turnIsPossible = true;
        *roadIsExpected = true;
    }
    else{
        turnIsPossible = (numNewLanes <= roads[endRoadDir]->getNumLanes(Road::straight));
        *roadIsExpected = false;
    }

    return turnIsPossible;
}

bool Intersection::newRoadIsPossible(Road::RoadDirection dir, int numLeftLanes, int numRightLanes){
    bool leftTurnPossible, rightTurnPossible, roadIsPossible;
    bool expectLeftRoad;
    bool expectRightRoad;

    leftTurnPossible = newTurnIsPossible(Road::roadLeftOf(dir), numLeftLanes, &expectLeftRoad);
    rightTurnPossible = newTurnIsPossible(Road::roadRightOf(dir), numRightLanes, &expectRightRoad);
    roadIsPossible = leftTurnPossible && rightTurnPossible;

    if(roadIsPossible){
        /// We only want to set expectedRoads if both turns are currently possible
        expectedRoads[Road::roadLeftOf(dir)] = expectedRoads[Road::left] || expectLeftRoad;
        expectedRoads[Road::roadRightOf(dir)] = expectedRoads[Road::right] || expectRightRoad;
    }

    return roadIsPossible;
}

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr){
    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newRoadIsPossible(dir, numLanesArr[Road::left], numLanesArr[Road::right])){
        return turnNotPossible;
    }

    roads[dir] = new Road(dir, numLanesArr);
    expectedRoads[dir] = false;     /// If we were expecting this road before, we now no longer are.

    return success;
}

int Intersection::setAllLightDurations(int onDur, int yellowDir){    
    int numRoadsSet = 0;
    int redDur = -1;
    
    for(Road* rd : roads){
        if(rd != NULL){
            rd->setAllLightDurations(onDur, redDur, yellowDir);
        }
        numRoadsSet++;
    }
    
    return numRoadsSet;
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

Road::RoadDirection Road::roadRightOf(RoadDirection dir){
    int rightRoadDir = dir + 1;

    if(rightRoadDir == numRoadDirections){
        rightRoadDir = 0;
    }

    isValidRoadDirection((Road::RoadDirection&)rightRoadDir);
    return (Road::RoadDirection)rightRoadDir;
}

Road::RoadDirection Road::roadLeftOf(RoadDirection dir){
    int leftRoadDir = dir - 1;

    if(leftRoadDir < 0){
        leftRoadDir = numRoadDirections - 1;
    }

    isValidRoadDirection((Road::RoadDirection&)leftRoadDir);
    return (Road::RoadDirection)leftRoadDir;
}

Road::RoadDirection Road::roadOppositeOf(RoadDirection dir){
    int halfRoads = numRoadDirections / 2;  /// This assumes numRoadDirections is even
    RoadDirection tempRd = dir;

    if(numRoadDirections % 2 != 0){
        throw std::domain_error("Road::roadOppositeOf() is called with an odd-length Road::numRoadDirections");
        /// Functionality is undefined when numRoadDirections is odd.
    }

    for(int i=0; i<halfRoads; i++){
        tempRd = roadLeftOf(tempRd);
    }

    return tempRd;
}

int Road::setAllLightDurations(int onDur, int redDur, int yellowDur){
    int numLightsSet = 0;

    if(onDur < -1 || redDur < -1 || yellowDur < -1){
        throw std::out_of_range("Road::setAllLightDurations has a duration < -1");
        return 0;
    }
    
    for(TrafficLight* tl : lights){
        if(tl != NULL){
            tl->setDuration(tl->getOnColor(), onDur);
            tl->setDuration(TrafficLight::red, redDur);
            if(yellowDur != -1){
                tl->setDuration(TrafficLight::yellow, yellowDur);
            }

            numLightsSet++;
        }
    }

    return numLightsSet;
}

int Road::getNumLanes(TurnOption opt){
    isValidTurnOption(opt);

    return lanesPerTurnOption[opt];
}

TrafficLight* Road::getLight(TurnOption opt){
    isValidTurnOption(opt);

    return lights[opt];
}

bool Road::isThroughRoad(){
    return throughRoad;
}