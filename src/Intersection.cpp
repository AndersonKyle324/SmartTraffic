#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Intersection.h"

Intersection::Intersection(){
    numRoads = 0;

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

bool Intersection::validate(std::ostream out){
    if(numRoads < MIN_NUM_ROADS){
        out << "Must have at least " << MIN_NUM_ROADS << " roads\n";
        return false;
    }

    for(int i=0; i<Road::numRoadDirections; i++){
        if(expectedRoads[i]){
            out << "Expecting a road facing " << expectedRoads[i] << '\n';
            return false;
        }
    }

    out << "Intersection is valid\n";

    return true;
}

void Intersection::tick(){
    TrafficLight *roadLight;

    for(Road *rd : roads){
        if(rd != NULL){
            for(int turnOpt=0; turnOpt < Road::numTurnOptions; turnOpt++){
                roadLight = rd->getLight((Road::TurnOption)turnOpt);
                
                if(roadLight != NULL){
                    roadLight->tick();
                }
            }
        }
    }
}

bool Intersection::doubleGreen(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    rd->setGreen(onDuration);
    oppRd->setGreen(onDuration);
    
    return true;
}

bool Intersection::singleGreen(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];

    if(rd == NULL){
        return false;
    }

    rd->setGreen(onDuration);
    rd->setGreenLeft(onDuration);
    
    return true;
}

bool Intersection::doubleGreenLeft(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    rd->setGreenLeft(onDuration);
    oppRd->setGreenLeft(onDuration);
    
    return true;
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

    /// We only want to set expectedRoads if both turns are currently possible
    if(roadIsPossible){
        /// If expectedRoad is already true, dont set it false.
        expectedRoads[Road::roadLeftOf(dir)] = expectedRoads[Road::roadLeftOf(dir)] || expectLeftRoad;
        expectedRoads[Road::roadRightOf(dir)] = expectedRoads[Road::roadRightOf(dir)] || expectRightRoad;
    }

    return roadIsPossible;
}

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr, int onDuration){
    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newRoadIsPossible(dir, numLanesArr[Road::left], numLanesArr[Road::right])){
        return turnNotPossible;
    }

    roads[dir] = new Road(dir, numLanesArr, onDuration);
    expectedRoads[dir] = false;     /// If we were expecting this road before, we now no longer are.
    numRoads++;

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

TrafficLight* Intersection::getLight(Road::RoadDirection dir, Road::TurnOption turn){
    if(getRoad(dir) != NULL){
        return getRoad(dir)->getLight(turn);
    }

    return NULL;
}

std::vector<TrafficLight*> Intersection::getLights(){
    std::vector<TrafficLight*> allLights;

    for(Road *rd : roads){
        if(rd != NULL){
            std::vector<TrafficLight*> roadLights = rd->getLights();
            allLights.insert(allLights.end(), roadLights.begin(), roadLights.end());
        }
    }

    return allLights;
}

void Intersection::printHelper(Road::RoadDirection dir, std::string& outStr){
    TrafficLight *tempLight;
    int slotLeft, slotStr, slotRight;
    std::string templateStr =    "           |                             |                             |           \n";
    std::string templateStrMid = "           |                                                           |           \n";
    

    switch(dir){
        case (Road::north):
            outStr = templateStr;
            
            slotLeft = SLOT_NORTH_LEFT;
            slotStr = SLOT_NORTH_STR;
            slotRight = SLOT_NORTH_RIGHT;
            break;

        case (Road::south):
            outStr = templateStr;

            slotLeft = SLOT_SOUTH_LEFT;
            slotStr = SLOT_SOUTH_STR;
            slotRight = SLOT_SOUTH_RIGHT;
            break;

        case (Road::east):
            outStr = templateStrMid;
            outStr.append(templateStrMid);
            outStr.append(templateStrMid);

            slotLeft = SLOT_EAST;
            slotStr = SLOT_EAST + LEN_LINE;         /// Same position one line down
            slotRight = SLOT_EAST + (LEN_LINE * 2); /// Same position two lines down
            break;

        case (Road::west):
            outStr = templateStrMid;
            outStr.append(templateStrMid);
            outStr.append(templateStrMid);

            slotLeft = SLOT_WEST;
            slotStr = SLOT_WEST + LEN_LINE;         /// Same position one line down
            slotRight = SLOT_WEST + (LEN_LINE * 2); /// Same position two lines down
            break;

        default:
            return;
    }

    if( ! roadExists(dir)){
        return;
    }

    tempLight = getLight(dir, Road::left);
    if(tempLight != NULL){
        outStr.replace(slotLeft, tempLight->toString().size(), tempLight->toString());
    }
    tempLight = getLight(dir, Road::straight);
    if(tempLight != NULL){
        outStr.replace(slotStr, tempLight->toString().size(), tempLight->toString());
    }
    tempLight = getLight(dir, Road::right);
    if(tempLight != NULL){
        outStr.replace(slotRight, tempLight->toString().size(), tempLight->toString());
    }
}

void Intersection::print(){
    std::string northStr, westStr, eastStr, southStr;
    std::string crosswalkStr =    "-----------|===========================================================|-----------\n";
    std::string dividerStr =      "===========|                             O                             |===========\n";

    printHelper(Road::north, northStr);
    printHelper(Road::west, westStr);
    printHelper(Road::east, eastStr);
    printHelper(Road::south, southStr);

    std::cout << northStr;
    std::cout << crosswalkStr;
    std::cout << westStr;
    std::cout << dividerStr;
    std::cout << eastStr;
    std::cout << crosswalkStr;
    std::cout << southStr;
}
