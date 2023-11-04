#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <array>
#include "TrafficLight.h"

class Road{
public:
    enum RoadDirection {north, east, south, west, numRoadDirections};
    enum TurnOption {straight, left, right, numTurnOptions};

    static bool isValidTurnOption(TurnOption& opt){
        if(opt >= Road::numTurnOptions || opt < 0){
            throw std::out_of_range("TurnOption out of range");
            return false;
        }

        return true;
    }

protected:
    RoadDirection direction;                                /// The compass direction in which the road is pointing
    std::array<int, numTurnOptions> lanesPerTurnOption;     /// Number of lanes in each turn option
    std::array<TrafficLight, numTurnOptions> lights;        /// TrafficLight associated with each TurnOption

public:
    Road(RoadDirection dir, int numStraightLanes, int numLeftLanes, int numRightLanes);

    RoadDirection getDirection(){ return direction; };
    int getNumLanes(TurnOption opt);
    TrafficLight* getLight(TurnOption opt);
};

class Intersection{

protected:
    std::array<Road, Road::numRoadDirections> roads;

public:
    Intersection();
};

#endif