#include <cstdlib>
#include <iostream>

#include "Intersection.h"
#include "TrafficLight.h"

int main(int argc, char *argv[]){
    TrafficLightLeft tl = TrafficLightLeft();
    Intersection inter = Intersection();
    
    // std::cout << tl.getColor();

    tl.setDuration(TrafficLight::greenLeft, 2);
    tl.setDuration(TrafficLight::red, 1);

    tl.start();
    std::cout << tl;
    tl.nextState();
    tl.tick();
    std::cout << tl;
    tl.tick();
    std::cout << tl;
    tl.tick();

    std::cout << tl;
    tl.tick();

    std::cout << tl;

    return 0;
}