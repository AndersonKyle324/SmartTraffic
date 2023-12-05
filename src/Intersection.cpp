#include <string>
#include <iostream>
#include <sstream>
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
    std::string dividerStr =       "===========|                             O                             |===========\n";

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

/*std::ostream& operator<<(std::ostream &out, Intersection const& data){
}
*/

std::ostream& operator<<(std::ostream &out, Road::RoadDirection const& data){
    switch(data){
        case Road::north:
            out << "North";
            break;
        
        case Road::east:
            out << "East";
            break;
        
        case Road::south:
            out << "South";
            break;
        
        case Road::west:
            out << "West";
            break;
        
        default:
            out << "RoadDirection NaN";
            break;
    }

    return out;
}
