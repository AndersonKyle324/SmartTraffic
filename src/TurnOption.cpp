#include "TurnOption.h"
    
TurnOption::TurnOption(TurnOption::Type aType, unsigned int lanes, unsigned int maxNumVehiclesPerLane, unsigned int crossTime, int lightDuration, int lightRedDuration): TurnOption(){
    TrafficLight::AvailableColors lightAvailColor;
    
    type = aType;

    switch(aType){
        case left:
            lightAvailColor = TrafficLight::greenLeft;
            break;

        case straight:
            lightAvailColor = TrafficLight::green;
            break;

        case right:
            lightAvailColor = TrafficLight::greenRight;
            break;
        
        default:
            throw std::domain_error("TurnOption() constructuor called with unhandled TurnOption::Type\n");

    }
    light = new TrafficLight(lightAvailColor, lightDuration, lightRedDuration);

    numLanes = lanes;
    maxVehiclesPerLane = maxNumVehiclesPerLane;
    timeToCross = crossTime;

}

bool TurnOption::isValidTurnOption(){
    return isValidTurnOption(type);
}

bool TurnOption::queueIsFull(){
    return getQueuedVehicles() >= getMaxNumVehicles();
}

void TurnOption::progressVehicles(){
    currentVehicleProgress--;
}

bool TurnOption::vehiclesAreCrossing(){
    return getCurrentVehicleProgress() != 0;
}

void TurnOption::vehiclesEnterIntersection(){
    if(getQueuedVehicles() > getNumLanes()){
        numVehiclesCurrentlyCrossing = getNumLanes();
    }
    else{
        numVehiclesCurrentlyCrossing = getQueuedVehicles();
    }

    queuedVehicles -= numVehiclesCurrentlyCrossing;
    getLight()->addVehiclesDirected(numVehiclesCurrentlyCrossing);

    currentVehicleProgress = getTimeToCross();
}