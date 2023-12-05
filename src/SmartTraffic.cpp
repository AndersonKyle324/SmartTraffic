#include <cstdlib>
#include <iostream>

#include "TrafficLight.h"
#include "Intersection.h"

int main(int argc, char *argv[]){
    Intersection inter = Intersection();
    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});
    
    inter.print();

    return 0;
}