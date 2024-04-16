#include <stdexcept>
#include <iostream>
#include <sstream>

#include "TrafficLight.h"

TrafficLight::TrafficLight(AvailableColors aOnColor, int onColorDur, int redDur) : TrafficLight(){
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
    if(durationRemaining > 0){
        durationRemaining--;
    }

    nextState();

    return durationRemaining;
}

int TrafficLight::addVehiclesDirected(int numVehicles){
    numVehiclesDirected += numVehicles;

    return numVehiclesDirected;
}

void TrafficLight::resetDurationRemaining(){
    setDurationRemainingColor(color);
}

TrafficLight::AvailableColors TrafficLight::nextState(){
    if(durationRemaining == 0){
        switch(color){
            case green:
            case greenLeft:
            case greenRight:
                color = yellow;
                setDurationRemainingColor(yellow);
                break;

            case yellow:
                color = red;
                setDurationRemainingColor(red);
                break;

            case red:
                color = onColor;
                setDurationRemainingColor(onColor);
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
    out << data.durationRemaining;
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

/*std::ostream& operator<<(std::ostream &out, const TrafficLight& data){
     This is for debugging
    out << "<c:'";
    out << data.color;
    out << "' dr:";
    out << data.durationRemaining;
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
    out << data.toString();

    return out;
}
*/

/*std::ostream& operator<<(std::ostream &out, const TrafficLightLeft& data){
    out << "{Left ";
    out << data.toString();
    out << "}";

    return out;
}
*/