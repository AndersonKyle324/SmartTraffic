#include <stdexcept>
#include <iostream>
#include <sstream>

#include "TrafficLight.h"

TrafficLight::TrafficLight(AvailableColors aOnColor, double onColorDur, double redDur) : TrafficLight(){
    onColor = aOnColor;
    setDuration(aOnColor, onColorDur);
    setDuration(yellow, yellowDuration);
    setDuration(red, redDur);
}

TrafficLightLeft::TrafficLightLeft(){
    onColor = greenLeft;
}

TrafficLightLeft::TrafficLightLeft(int leftDur, int redDur) : TrafficLight(greenLeft, leftDur, redDur){}

void TrafficLight::start(){
    setColor(onColor);
}

int TrafficLight::tick(){
    if(ticksRemaining > 0){
        ticksRemaining--;
    }

    nextState();

    return ticksRemaining;
}

int TrafficLight::addVehiclesDirected(int numVehicles){
    numVehiclesDirected += numVehicles;

    return numVehiclesDirected;
}

void TrafficLight::resetTicksRemaining(){
    setTicksRemainingColor(color);
}

TrafficLight::AvailableColors TrafficLight::nextState(){
    if(ticksRemaining == 0){
        switch(color){
            case green:
            case greenLeft:
            case greenRight:
                color = yellow;
                setTicksRemainingColor(yellow);
                break;

            case yellow:
                color = red;
                setTicksRemainingColor(red);
                break;

            case red:
                color = onColor;
                setTicksRemainingColor(onColor);
                break;

            default:
                throw std::out_of_range("TrafficLight reached unexpected color in nextState()");
                break;
        }
    }

    return color;
}

std::ostream& operator<<(std::ostream &out, TrafficLight::AvailableColors const& data){
    switch(data){
        case TrafficLight::green:
            out << "green";
            break;
        case TrafficLight::greenLeft:
            out << "greenLeft";
            break;
        case TrafficLight::greenRight:
            out << "greenRight";
            break;
        case TrafficLight::yellow:
            out << "yellow";
            break;
        case TrafficLight::red:
            out << "red";
            break;
        default:
            out << "Color NaN";
            break;
    }

    return out;
}

std::string TrafficLight::toString(){
    std::stringstream ss;
    std::string outStr;
    /* This is for debugging
    out << "<c:'";
    out << data.color;
    out << "' dr:";
    out << data.ticksRemaining;
    out << " ";
    out << data.colorDuration[TrafficLight::green];
    out << "-";
    out << data.colorDuration[TrafficLight::greenLeft];
    out << "-";
    out << data.colorDuration[TrafficLight::greenRight];
    out << "-";
    out << data.colorDuration[TrafficLight::yellow];
    out << "-";
    out << data.colorDuration[TrafficLight::red];
    out << ">";
    */
    ss << getColor();
    ss >> outStr;

    return outStr;
}
