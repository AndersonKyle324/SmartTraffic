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
        exitRoads[i] = NULL;
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

    for(int dir=0; dir<Road::numRoadDirections; dir++){
        Road* rd = getRoad((Road::RoadDirection)dir);
        if(rd != NULL){
            for(int turnType=0; turnType < TurnOption::numTurnOptions; turnType++){
                if(rd->getTurnOption((TurnOption::Type)turnType)->isValid() && getExitRoad((Road::RoadDirection)dir) == NULL){
                    std::cout << "Expecting an exit road facing " << (Road::RoadDirection)dir << std::endl;
                    return false;
                }
            }
        }

        if(expectedRoads[dir]){
            std::cout << "Expecting a road facing " << (Road::RoadDirection)dir << std::endl;
            return false;
        }
    }

    std::cout << "Intersection is valid\n";

    return true;
}

int Intersection::tick(){
    TrafficLight *roadLight;

    for(Road *rd : roads){
        /// Skip Road if its NULL
        if(rd == NULL){
            continue;
        }

        for(int opt=0; opt < TurnOption::numTurnOptions; opt++){
            TurnOption* turnOpt;

            turnOpt = rd->getTurnOption((TurnOption::Type)opt);
            
            /// Skip this TurnOption if is not valid
            if( ! turnOpt->isValid()){
                continue;
            }

            roadLight = turnOpt->getLight();
            
            handleVehicles(rd, (TurnOption::Type)opt);
            handleLightTick(roadLight);
        }
    }

    ticksSinceStart++;

    return numUnfinishedLights;
}

void Intersection::handleLightTick(TrafficLight* light){
    
    /// If light is already red, it doesnt need to be ticked
    if(light->getColor() != TrafficLight::red){
        light->tick();

        /// If light transitions to red, decrement numUnfinishedLights in Intersection
        if(light->getColor() == TrafficLight::red){
            if(numUnfinishedLights <= 0){
                throw std::out_of_range("Intersection reached numUnfinishedLights of 0 before finishing all lights in Intersection::tick()");
            }

            numUnfinishedLights--;
        }
    }
}

void Intersection::handleVehicles(Road* rd, TurnOption::Type opt){
    TurnOption* turnOpt;
    TurnOption* exitTurnOpt;

    turnOpt = rd->getTurnOption((TurnOption::Type)opt);

    if(turnOpt->queueIsEmpty()){
        /// No vehicles in queue, nothing to do.
        return;
    }

    /// Move along vehicles currently in intersection
    if(turnOpt->vehiclesAreCrossing()){
        turnOpt->progressVehicles();
    }

    exitTurnOpt = getExitRoad(rd->getDirection(), turnOpt->getType())->getTurnOption(opt);
    if(exitTurnOpt == NULL){
        throw std::logic_error("Exit Road in Intersection::handleVehicles() is NULL, Intersection is invalid\n");
        return;
    }
    if( ! exitTurnOpt->isValid()){
        /// This turnOpt does not exist in the exit Road, use the straight TurnOpt by default
        exitTurnOpt = getExitRoad(rd->getDirection(), turnOpt->getType())->getTurnOption(TurnOption::straight);
    }

    if( ! turnOpt->getLight()->isRed() && 
        ! turnOpt->vehiclesAreCrossing() && 
        ! exitTurnOpt->queueIsFull())
    {
        turnOpt->nextVehiclesBeginCrossing(exitTurnOpt);
    }
    else if(turnOpt->getLight()->isRed() && 
            turnOpt->vehiclesAreCrossing())
    {
        turnOpt->vehiclesLeftInIntersection();
    }
}

bool Intersection::schedule(LightConfig::Option configOpt, Road::RoadDirection direction, double duration, double yellowDuration){
    LightConfig *interConfig = new LightConfig(configOpt, direction, duration, yellowDuration);

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
    return schedule(config.getConfigOption(), config.getDirection(), config.getDuration(), config.getYellowDuration());
}

void Intersection::clearSchedule(){
    configSchedule.clear();
}

bool Intersection::start(){
    bool configSuccess = setLightConfig(0);
    
    if( ! configSuccess){
        throw std::runtime_error("Intersection::start() error, invalid LightConfig\n");
    }

    return configSuccess;
}

bool Intersection::setLightConfig(int idx){
    bool configSuccess = false;
    LightConfig *config;
    
    config = configSchedule.at(idx);

    switch(config->getConfigOption()){
        case LightConfig::doubleGreen:
            configSuccess = doubleGreen(config->getDirection(), config->getDuration(), config->getYellowDuration());
            break;

        case LightConfig::singleGreen:
            configSuccess = singleGreen(config->getDirection(), config->getDuration(), config->getYellowDuration());
            break;

        case LightConfig::doubleGreenLeft:
            configSuccess = doubleGreenLeft(config->getDirection(), config->getDuration(), config->getYellowDuration());
            break;

        default:
            throw std::out_of_range("Intersection::nextLightConfig() encountered an unhandled LightConfig::Option");
            break;
    }

    return configSuccess;
}

bool Intersection::nextLightConfig(){
    bool configSuccess;
    configScheduleIdx++;

    if(configScheduleIdx >= configSchedule.size()){
        /// Loop the LightConfigs in configSchedule
        configScheduleIdx = 0;
    }

    configSuccess = setLightConfig(configScheduleIdx);
    if( ! configSuccess){
        throw std::runtime_error("Intersection::nextLightConfig() error, invalid LightConfig\n");
    }

    return configSuccess;
}

bool Intersection::doubleGreen(Road::RoadDirection dir, double onDuration, double yellowDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    /// If light is set properly, add an unfinished light
    numUnfinishedLights += rd->setGreen(onDuration, yellowDuration);
    numUnfinishedLights += oppRd->setGreen(onDuration, yellowDuration);
    
    return true;
}

bool Intersection::singleGreen(Road::RoadDirection dir, double onDuration, double yellowDuration){
    Road *rd = roads[dir];

    if(rd == NULL){
        return false;
    }

    numUnfinishedLights += rd->setGreen(onDuration, yellowDuration);
    numUnfinishedLights += rd->setGreenLeft(onDuration, yellowDuration);
    
    return true;
}

bool Intersection::doubleGreenLeft(Road::RoadDirection dir, double onDuration, double yellowDuration){
    Road *rd = roads[dir];
    Road *oppRd = roads[Road::roadOppositeOf(dir)];

    if(rd == NULL || oppRd == NULL){
        return false;
    }

    numUnfinishedLights += rd->setGreenLeft(onDuration, yellowDuration);
    numUnfinishedLights += oppRd->setGreenLeft(onDuration, yellowDuration);
    
    return true;
}

bool Intersection::roadExists(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);

    return roads[dir] != NULL;
}
    
Road* Intersection::getExitRoad(Road::RoadDirection startDir, TurnOption::Type turnOpt){
    Road::RoadDirection exitRoadDir = Road::exitRoadDirection(startDir, turnOpt);

    return exitRoads[exitRoadDir];
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
        turnIsPossible = (numNewLanes <= roads[endRoadDir]->getTotalNumLanes());
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

int Intersection::addRoad(Road::RoadDirection dir, std::array<int, TurnOption::numTurnOptions> numLanesArr, double onDuration, double yellowDuration){
    Road::isValidRoadDirection(dir);

    if(roads[dir] != NULL){
        return alreadyExists;
    }
    
    if( ! newRoadIsPossible(dir, numLanesArr[TurnOption::left], numLanesArr[TurnOption::right])){
        return turnNotPossible;
    }

    roads[dir] = new Road(dir, numLanesArr, onDuration, yellowDuration);
    expectedRoads[dir] = false;     /// If we were expecting this road before, we now no longer are.
    numRoads++;

    return success;
}

long Intersection::addMaxVehicles(){
    long totalVehiclesAdded = 0;
    int numVehiclesToMax;

    for(Road* rd : roads){
        if(rd == NULL){
            continue;
        }

        for(int turnType=0; turnType < TurnOption::numTurnOptions; turnType++){
            TurnOption* turn = rd->getTurnOption((TurnOption::Type)turnType);

            if(turn->isValid()){
                numVehiclesToMax = turn->getMaxNumVehicles() - turn->getQueuedVehicles();

                if(turn->addVehicles(numVehiclesToMax)){
                    totalVehiclesAdded += numVehiclesToMax;
                }
            }
        }
    }

    return totalVehiclesAdded;
}

bool Intersection::addVehicles(Road::RoadDirection dir, TurnOption::Type turn, int numVehiclesToAdd){
    Road* rd = getRoad(dir);

    if(rd != NULL){
        TurnOption* turnOpt = rd->getTurnOption(turn);
        return turnOpt->addVehicles(numVehiclesToAdd);
    }

    return false;
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

void Intersection::setExitRoad(Road::RoadDirection dir, Road* exitRd){
    Road::isValidRoadDirection(dir);
    exitRoads[dir] = exitRd;
}

Road* Intersection::getRoad(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);
    return roads[dir];
}

Road* Intersection::getExitRoad(Road::RoadDirection dir){
    Road::isValidRoadDirection(dir);
    return exitRoads[dir];
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
    Road* tempRoad;
    TurnOption* tempTurnOpt;
    std::string vehicleString;
    int slotLeft, slotStr, slotRight, vehicleLeft, vehicleStr, vehicleRight;
    Road::RoadDirection vehicleDir;
    std::string templateStr =    "           |                             |                             |           \n";
    std::string templateStrMid = "           |                                                           |           \n";
    

    switch(dir){
        case (Road::north):
            outStr = templateStr;
            
            slotLeft = SLOT_NORTH_LEFT;
            slotStr = SLOT_NORTH_STR;
            slotRight = SLOT_NORTH_RIGHT;

            vehicleDir = Road::south;
            vehicleLeft = SLOT_SOUTH_LEFT;
            vehicleStr = SLOT_SOUTH_STR;
            vehicleRight = SLOT_SOUTH_RIGHT;
            break;

        case (Road::south):
            outStr = templateStr;

            slotLeft = SLOT_SOUTH_LEFT;
            slotStr = SLOT_SOUTH_STR;
            slotRight = SLOT_SOUTH_RIGHT;

            vehicleDir = Road::north;
            vehicleLeft = SLOT_NORTH_LEFT;
            vehicleStr = SLOT_NORTH_STR;
            vehicleRight = SLOT_NORTH_RIGHT;
            break;

        case (Road::east):
            outStr = templateStrMid;
            outStr.append(templateStrMid);
            outStr.append(templateStrMid);

            slotLeft = SLOT_EAST;
            slotStr = SLOT_EAST + LEN_LINE;         /// Same position one line down
            slotRight = SLOT_EAST + (LEN_LINE * 2); /// Same position two lines down

            vehicleDir = Road::east;
            vehicleLeft = SLOT_WEST;
            vehicleStr = SLOT_WEST + LEN_LINE;
            vehicleRight = SLOT_WEST + (LEN_LINE * 2);
            break;

        case (Road::west):
            outStr = templateStrMid;
            outStr.append(templateStrMid);
            outStr.append(templateStrMid);

            slotLeft = SLOT_WEST + (LEN_LINE * 2);  /// Bottom position
            slotStr = SLOT_WEST + LEN_LINE;         /// Middle position
            slotRight = SLOT_WEST;                  /// Top position

            vehicleDir = Road::west;
            vehicleLeft = SLOT_EAST + (LEN_LINE * 2);
            vehicleStr = SLOT_EAST + LEN_LINE;
            vehicleRight = SLOT_EAST;
            break;

        default:
            return;
    }

    if( ! roadExists(dir)){
        return;
    }

    /// TrafficLights / Exit Road Queues
    tempLight = getLight(dir, TurnOption::left);
    tempTurnOpt = getExitRoad(dir)->getTurnOption(TurnOption::left);
    if(tempLight != NULL){
        vehicleString = tempLight->toString().append(" " + std::to_string(tempTurnOpt->getQueuedVehicles()) + "/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
        outStr.replace(slotLeft, vehicleString.size(), vehicleString);
    }
    tempLight = getLight(dir, TurnOption::straight);
    tempTurnOpt = getExitRoad(dir)->getTurnOption(TurnOption::straight);
    if(tempLight != NULL){
        vehicleString = tempLight->toString().append(" " + std::to_string(tempTurnOpt->getQueuedVehicles()) + "/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
        outStr.replace(slotStr, vehicleString.size(), vehicleString);
    }
    tempLight = getLight(dir, TurnOption::right);
    tempTurnOpt = getExitRoad(dir)->getTurnOption(TurnOption::right);
    if(tempLight != NULL){
        vehicleString = tempLight->toString().append(" " + std::to_string(tempTurnOpt->getQueuedVehicles()) + "/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
        outStr.replace(slotRight, vehicleString.size(), vehicleString);
    }

    /// Vehicle Queues
    tempRoad = getRoad(vehicleDir);
    if(tempRoad != NULL){
        tempTurnOpt = tempRoad->getTurnOption(TurnOption::left);
        if(tempTurnOpt->isValid()){
            vehicleString = std::to_string(tempTurnOpt->getQueuedVehicles());
            vehicleString.append("/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
            outStr.replace(vehicleLeft, vehicleString.size(), vehicleString);
        }

        tempTurnOpt = tempRoad->getTurnOption(TurnOption::straight);
        if(tempTurnOpt->isValid()){
            vehicleString = std::to_string(tempTurnOpt->getQueuedVehicles());
            vehicleString.append("/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
            outStr.replace(vehicleStr, vehicleString.size(), vehicleString);
        }

        tempTurnOpt = tempRoad->getTurnOption(TurnOption::right);
        if(tempTurnOpt->isValid()){
            vehicleString = std::to_string(tempTurnOpt->getQueuedVehicles());
            vehicleString.append("/" + std::to_string(tempTurnOpt->getMaxNumVehicles()));
            outStr.replace(vehicleRight, vehicleString.size(), vehicleString);
        }
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

    std::cout << "Time: " << time() / refreshRateHzGlobal << "s (" << time() << " ticks)" << std::endl;
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
