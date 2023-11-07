#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <array>
#include "TrafficLight.h"

class Road{
public:
    enum RoadDirection {north, east, south, west, numRoadDirections};
    enum TurnOption {straight, left, right, numTurnOptions};

    /**
     * @brief Checks "opt" is within the allowable range of TurnOption values
     * 
     * @param opt   The TurnOption to be checked
     * @return true if "opt" is between 0 and (numTurnOptions - 1)
     */
    static bool isValidTurnOption(TurnOption& opt){
        if(opt >= Road::numTurnOptions || opt < 0){
            throw std::out_of_range("TurnOption out of range");
            return false;
        }

        return true;
    }

protected:
    RoadDirection direction;                                /// The compass direction in which the road is facing when cars are at a stop
    std::array<int, numTurnOptions> lanesPerTurnOption;     /// Number of lanes in each turn option
    std::array<TrafficLight*, numTurnOptions> lights;       /// TrafficLight associated with each TurnOption

public:
    /**
     * @brief Construct a new Road object. Allocates memory for a TrafficLight for each TurnOption
     *  with a number of lanes greater than 0.
     * 
     * @param dir               Road direction
     * @param numStraightLanes  Num straight lanes
     * @param numLeftLanes      Num left turn lanes
     * @param numRightLanes     Num right rutn lanes
     */
    Road(RoadDirection dir, int numStraightLanes, int numLeftLanes, int numRightLanes);
    
    /**
     * @brief Destroy the Road object. Deletes all TrafficLights in lights.
     */
    ~Road();

    RoadDirection getDirection(){ return direction; };
    int getNumLanes(TurnOption opt);
    TrafficLight* getLight(TurnOption opt);
};

class Intersection{

protected:
    std::array<Road, Road::numRoadDirections> roads;

public:
    Intersection();

    /**
     * @brief Creates new Road while checking to make sure it conforms with other Roads already present.
     * 
     * @param dir           The direction the road is facing when cars are at a stop
     * @param numLanesArr   The number of lanes for each TurnOption. Should be Road::numTurnOptions long.
     * @return An integer representing success or a value representing a specific Intersection non-compliance error.
     *      -0 : Success
     *      -1 : Unknown error
     */
    int addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions>& numLanesArr);

};

#endif