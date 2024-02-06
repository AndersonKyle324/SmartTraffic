#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Intersection.h"

Intersection::Intersection(){
    numRoads = 0;
    configScheduleIdx = 0;
    numUnfinishedLights = 0;
    ticksSinceStart = 0;

    for(int i=0; i<Road::numRoadDirections; i++){
        roads[i] = NULL;
        expectedRoads[i] = false;
    }
}

Intersection::~Intersection(){
    for(Road* rd : roads){
        delete rd;
    }

    for(LightConfig *cfg : configSchedule){
        delete cfg;
    }
}

bool Intersection::validate(){
    if(numRoads < MIN_NUM_ROADS){
        std::cout << "Must have at least " << MIN_NUM_ROADS << " roads\n";
        return false;
    }

    for(int i=0; i<Road::numRoadDirections; i++){
        if(expectedRoads[i]){
            std::cout << "Expecting a road facing " << expectedRoads[i] << '\n';
            return false;
        }
    }

    std::cout << "Intersection is valid\n";

    return true;
}

int Intersection::tick(){
    TrafficLight *roadLight;
    TurnOption* exitRoadStraight;

    for(Road *rd : roads){
        /// Skip Road if its NULL
        if(rd == NULL){
            continue;
        }

        for(int opt=0; opt < TurnOption::numTurnOptions; opt++){
            TurnOption* turnOpt;
            roadLight = rd->getLight((TurnOption::Type)opt);
            
            /// Skip light if its NULL
            if(roadLight == NULL){
                continue;
            }
            
            /// If light is already red, it doesnt need to be ticked
            if(roadLight->getColor() != TrafficLight::red){
                roadLight->tick();

                turnOpt = rd->getTurnOption((TurnOption::Type)opt);

                if(turnOpt->getCurrentVehicleProgress() > 0){
                    turnOpt->progressVehicles();
                }

                exitRoadStraight = getExitRoad(rd->getDirection(), (TurnOption::Type)opt)->getTurnOption(TurnOption::straight);

                /// Assumes a car will not enter the intersection if the light is yellow.
                if( ! turnOpt->queueIsFull() && 
                    roadLight->isGreen() && 
                    ! turnOpt->vehiclesAreCrossing() && 
                    /// *********** WARNING this only checks if the exitRoad's straight TurnOption queue is full. Should I assume a vehicle turning left will end in the left turnOption, a car turning straight will end in the straight turnOption etc??? Should I check if the whole road is full???
                    ! exitRoadStraight->queueIsFull())
                {
                    turnOpt->vehiclesEnterIntersection();
                }

                /// If light transitions to red, decrement numUnfinishedLights in Intersection
                if(roadLight->getColor() == TrafficLight::red){
                    if(numUnfinishedLights <= 0){
                        throw std::out_of_range("Intersection reached numUnfinishedLights of 0 before finishing all lights in Intersection::tick()");
                    }

                    numUnfinishedLights--;
                }
            }
        }
    }

    ticksSinceStart++;

    return numUnfinishedLights;
}

bool Intersection::schedule(LightConfig::Option configOpt, Road::RoadDirection direction, int duration){
    LightConfig *interConfig = new LightConfig(configOpt, direction, duration);

    try{
        configSchedule.push_back(interConfig);
    }
    catch(...){
        throw std::runtime_error("Intersection::schedule() error adding new LightConfig to schedule");
        return false;
    }

    return true;
}

bool Intersection::schedule(LightConfig& config){
    return schedule(config.getConfigOption(), config.getDirection(), config.getDuration());
}


bool Intersection::start(){
    return setLightConfig(0);
}

bool Intersection::setLightConfig(int idx){
    bool configSuccess = false;
    LightConfig *config;
    
    config = configSchedule.at(idx);

    switch(config->getConfigOption()){
        case LightConfig::doubleGreen:
            configSuccess = doubleGreen(config->getDirection(), config->getDuration());
            break;

        case LightConfig::singleGreen:
            configSuccess = singleGreen(config->getDirection(), config->getDuration());
            break;

        case LightConfig::doubleGreenLeft:
            configSuccess = doubleGreenLeft(config->getDirection(), config->getDuration());
            break;

        default:
            throw std::out_of_range("Intersection::nextLightConfig() encountered an unhandled LightConfig::Option");
            break;
    }

    return configSuccess;
}

bool Intersection::nextLightConfig(){
    configScheduleIdx++;

    if(configScheduleIdx >= configSchedule.size()){
        /// Loop the LightConfigs in configSchedule
        configScheduleIdx = 0;
    }

    return setLightConfig(configScheduleIdx);
}

bool Intersection::doubleGreen(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    /// If light is set properly, add an unfinished light
    numUnfinishedLights += rd->setGreen(onDuration);
    numUnfinishedLights += oppRd->setGreen(onDuration);
    
    return true;
}

bool Intersection::singleGreen(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];

    if(rd == NULL){
        return false;
    }

    numUnfinishedLights += rd->setGreen(onDuration);
    numUnfinishedLights += rd->setGreenLeft(onDuration);
    
    return true;
}

bool Intersection::doubleGreenLeft(Road::RoadDirection dir, int onDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    numUnfinishedLights += rd->setGreenLeft(onDuration);
    numUnfinishedLights += oppRd->setGreenLeft(onDuration);
    
    return true;
}

bool Intersection::roadExists(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);

    return roads[dir] != NULL;
}
    
Road* Intersection::getExitRoad(Road::RoadDirection startDir, TurnOption::Type turnOpt){
    Road::RoadDirection exitRoadDir = Road::exitRoadDirection(startDir, turnOpt);

    return roads[exitRoadDir];
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
        turnIsPossible = (numNewLanes <= roads[endRoadDir]->getNumLanes(TurnOption::straight));
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

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, TurnOption::numTurnOptions> numLanesArr, int onDuration){
    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newRoadIsPossible(dir, numLanesArr[TurnOption::left], numLanesArr[TurnOption::right])){
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

TrafficLight* Intersection::getLight(Road::RoadDirection dir, TurnOption::Type turn){
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

            slotLeft = SLOT_WEST + (LEN_LINE * 2);  /// Bottom position
            slotStr = SLOT_WEST + LEN_LINE;         /// Middle position
            slotRight = SLOT_WEST;                  /// Top position
            break;

        default:
            return;
    }

    if( ! roadExists(dir)){
        return;
    }

    tempLight = getLight(dir, TurnOption::left);
    if(tempLight != NULL){
        outStr.replace(slotLeft, tempLight->toString().size(), tempLight->toString());
    }
    tempLight = getLight(dir, TurnOption::straight);
    if(tempLight != NULL){
        outStr.replace(slotStr, tempLight->toString().size(), tempLight->toString());
    }
    tempLight = getLight(dir, TurnOption::right);
    if(tempLight != NULL){
        outStr.replace(slotRight, tempLight->toString().size(), tempLight->toString());
    }
}

void Intersection::print(){
    std::string northStr, westStr, eastStr, southStr;
    std::string crosswalkStr =    "-----------|===========================================================|-----------\n";
    std::string dividerStr =      "===========|                             O                             |===========\n";
    std::string endSeparator =    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";

    printHelper(Road::north, northStr);
    printHelper(Road::west, westStr);
    printHelper(Road::east, eastStr);
    printHelper(Road::south, southStr);

    std::cout << "Time: " << time() << std::endl;
    std::cout << northStr;
    std::cout << crosswalkStr;
    std::cout << westStr;
    std::cout << dividerStr;
    std::cout << eastStr;
    std::cout << crosswalkStr;
    std::cout << southStr;
    std::cout << endSeparator;
    std::cout << std::endl;
}
