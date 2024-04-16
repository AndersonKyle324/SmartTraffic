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

bool TurnOption::isValid(){
    return ! (getType() == numTurnOptions || getLight() == NULL || getNumLanes() == 0);
}

bool TurnOption::queueIsFull(){
    return getQueuedVehicles() >= getMaxNumVehicles();
}

bool TurnOption::queueIsEmpty(){
    return getQueuedVehicles() == 0;
}

void TurnOption::progressVehicles(){
    currentVehicleProgress--;
}

bool TurnOption::vehiclesAreCrossing(){
    return getCurrentVehicleProgress() > 0;
}
    
void TurnOption::nextVehiclesBeginCrossing(TurnOption *exitTurnOpt){
    /// If this is the first call to handleVehicles() for this green/yellow light, numVehiclesCurrentlyCrossing should be 0.
    queuedVehicles -= getNumVehiclesCurrentlyCrossing();
    getLight()->addVehiclesDirected(getNumVehiclesCurrentlyCrossing());
    
    /// Vehicles have finished crossing so they should be added to the exit TurnOption queue.
    if( ! exitTurnOpt->addVehicles(getNumVehiclesCurrentlyCrossing())){
        std::cout << "Traffic Jam! Exit TurnOption queue is full : TurnOption::nextVehiclesBeginCrossing()" << std::endl;
    }

    /// New vehicles should only enter the Intersection if light is green, not yellow/red.
    if(getLight()->isGreen()){
        if(getQueuedVehicles() > getNumLanes()){
            numVehiclesCurrentlyCrossing = getNumLanes();
        }
        else{
            numVehiclesCurrentlyCrossing = getQueuedVehicles();
        }

        currentVehicleProgress = getTimeToCross();
    }
}

bool TurnOption::vehiclesLeftInIntersection(){
    if( ! getLight()->isRed() || ! vehiclesAreCrossing()){
        throw std::logic_error("TurnOption::vehiclesLeftInIntersection: Called when there are not vehicles in Intersection\n");
        return false;
    }

    currentVehicleProgress = 0;
    numVehiclesCurrentlyCrossing = 0;

    std::cout << "Traffic Jam! There are vehicles left in the Intersection : TurnOption::vehiclesLeftInIntersection()" << std::endl;
    return true;
}

bool TurnOption::addVehicles(int numVehiclesToAdd){
    unsigned int newQueuedVehiclesTotal = queuedVehicles + numVehiclesToAdd;

    if(numVehiclesToAdd < 0){
        return false;
    }

    if(newQueuedVehiclesTotal <= getMaxNumVehicles()){
        queuedVehicles = newQueuedVehiclesTotal;
        return true;
    }
    else{
        queuedVehicles = getMaxNumVehicles();
        return false;
    }
}