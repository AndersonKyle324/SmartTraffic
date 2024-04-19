#include <vector>

#include "Road.h"

Road::Road(RoadDirection dir, std::array<int, TurnOption::numTurnOptions> numLanesArr, double onDuration, double yellowDuration){
    isValidRoadDirection(dir);
    direction = dir;

    /// Initializes all turnOptions pointers
    for(int opt=0; opt < TurnOption::numTurnOptions; opt++){
        if(numLanesArr[opt] > 0){
            turnOptions[opt] = new TurnOption((TurnOption::Type)opt, numLanesArr[opt], DEFAULT_MAX_LANE_VEHICLES, DEFAULT_TIME_TO_CROSS, onDuration);
        }
        else{
            /// Default constructor
            turnOptions[opt] = new TurnOption();
        }
    }
}

Road::~Road(){
    for(TurnOption* opt : turnOptions){
        delete opt;
    }
}

Road::RoadDirection Road::exitRoadDirection(RoadDirection startDir, TurnOption::Type turnOpt){
    RoadDirection exitDir;

    isValidRoadDirection(startDir);
    TurnOption::isValidTurnOption(turnOpt);
    
    switch(turnOpt){
        case(TurnOption::left):
            exitDir = roadLeftOf(startDir);
            break;

        case(TurnOption::straight):
            exitDir = roadOppositeOf(startDir);
            break;

        case(TurnOption::right):
            exitDir = roadRightOf(startDir);
            break;

        default:
            throw std::domain_error("Road::exitRoadDirection() called with unhandled TurnOption::Type\n");
            exitDir = numRoadDirections;
            break;
    }

    return exitDir;
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

bool Road::startLight(TurnOption::Type turnOpt, double onDuration, double yellowDuration){
    if(getLight(turnOpt) != NULL){
        getLight(turnOpt)->setOnDuration(onDuration);
        if(yellowDuration != DONT_SET){
            getLight(turnOpt)->setDuration(TrafficLight::yellow, yellowDuration);
        }
        getLight(turnOpt)->start();
        return true;
    }

    return false;
}

int Road::setGreen(double onDuration, double yellowDuration){
    int numLightsSet = 0;

    numLightsSet += setGreenRight(onDuration, yellowDuration);

    if(startLight(TurnOption::straight, onDuration, yellowDuration)){
        numLightsSet++;
    }

    return numLightsSet;
}

int Road::setGreenLeft(double onDuration, double yellowDuration){
    int numLightsSet = 0;

    if(startLight(TurnOption::left, onDuration, yellowDuration)){
        numLightsSet++;
    }

    return numLightsSet;
}

int Road::setGreenRight(double onDuration, double yellowDuration){
    int numLightsSet = 0;

    if(startLight(TurnOption::right, onDuration, yellowDuration)){
        numLightsSet++;
    }

    return numLightsSet;
}

int Road::setAllLightDurations(double onDur, double redDur, double yellowDur){
    int numLightsSet = 0;

    if(onDur < -1 || redDur < -1 || yellowDur < -1){
        throw std::out_of_range("Road::setAllLightDurations has a duration < -1");
        return 0;
    }
    
    for(TurnOption* opt : turnOptions){
        if(opt == NULL){
            throw std::logic_error("Road constructor did not initialize all TurnOption pointers\n");
        }

        TrafficLight* tl = opt->getLight();

        if(tl != NULL){
            tl->setDuration(tl->getOnColor(), onDur);
            tl->setDuration(TrafficLight::red, redDur);
            if(yellowDur != DONT_SET){
                tl->setDuration(TrafficLight::yellow, yellowDur);
            }

            numLightsSet++;
        }
    }

    return numLightsSet;
}

int Road::getTotalNumLanes(){
    int totalNumLanes = 0;

    for(int opt=0; opt < TurnOption::numTurnOptions; opt++){
        if(turnOptions[opt] == NULL){
            throw std::logic_error("Road constructor did not initialize all TurnOption pointers\n");
            return -1;
        }

        totalNumLanes += turnOptions[opt]->getNumLanes();
    }

    return totalNumLanes;
}

int Road::getNumLanes(TurnOption::Type opt){
    TurnOption::isValidTurnOption(opt);

    if(turnOptions[opt] == NULL){
        throw std::logic_error("Road constructor did not initialize all TurnOption pointers\n");
    }

    return turnOptions[opt]->getNumLanes();
}

TurnOption* Road::getTurnOption(TurnOption::Type opt){
    TurnOption::isValidTurnOption(opt);

    if(turnOptions[opt] == NULL){
        throw std::logic_error("Road constructor did not initialize all TurnOption pointers\n");
    }

    return turnOptions[opt];

}

TrafficLight* Road::getLight(TurnOption::Type opt){
    TurnOption::isValidTurnOption(opt);

    if(turnOptions[opt] == NULL){
        throw std::logic_error("Road constructor did not initialize all TurnOption pointers\n");
    }

    return turnOptions[opt]->getLight();
}

std::vector<TrafficLight*> Road::getLights(){
    std::vector<TrafficLight*> lights;
    TrafficLight *tl;

    for(int turnOpt=0; turnOpt < TurnOption::numTurnOptions; turnOpt++){
        tl = getLight((TurnOption::Type)turnOpt);

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
