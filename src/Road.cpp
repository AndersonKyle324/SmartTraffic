#include <vector>

#include "Road.h"

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

bool Road::startLight(Road::TurnOption turnOpt){
    if(getLight(turnOpt) != NULL){
        getLight(turnOpt)->start();
        return true;
    }

    return false;
}

bool Road::setGreen(){
    setGreenRight();

    return startLight(straight);
}

bool Road::setGreenLeft(){
    return startLight(left);
}

bool Road::setGreenRight(){
    return startLight(right);
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

std::vector<TrafficLight*> Road::getLights(){
    std::vector<TrafficLight*> lights;
    TrafficLight *tl;

    for(int turnOpt=0; turnOpt < numTurnOptions; turnOpt++){
        tl = getLight((Road::TurnOption)turnOpt);

        if(tl != NULL){
            lights.push_back(tl);
        }
    }

    return lights;
}

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
