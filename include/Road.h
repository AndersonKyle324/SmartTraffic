
#ifndef ROAD_H
#define ROAD_H

#include "TrafficLight.h"

class Road{
public:
    enum RoadDirection {north, east, south, west, numRoadDirections};
    enum TurnOption {left, straight, right, numTurnOptions};

    /**
    * @brief Overloaded << operator to output Road::RoadDirection to a stream.
    *
    * @param out The output stream.
    * @param data The Road::RoadDirection to be output.
    * @return The modified output stream.
    */
    friend std::ostream& operator<<(std::ostream &out, RoadDirection const& data);

    /**
     * @brief Checks "dir" is within the allowable range of RoadDirection values
     * 
     * @param dir   The RoadDirection to be checked
     * @return true if "dir" is between 0 and (Road::numRoadDirections - 1)
     */
    static bool isValidRoadDirection(RoadDirection& dir){
        if(dir >= Road::numRoadDirections || dir < 0){
            throw std::out_of_range("RoadDirection out of range");
            return false;
        }

        return true;
    }

    /**
     * @brief Checks "opt" is within the allowable range of TurnOption values
     * 
     * @param opt   The TurnOption to be checked
     * @return true if "opt" is between 0 and (Road::numTurnOptions - 1)
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
    bool exitRoad;                                          /// Road is an intersection exit only, can only 

    /**
     * @brief Calls start() on the "turnOpt" TrafficLight.
     *
     * @param turnOpt the desired light to start
     * 
     * @return false if the light is unavailable (NULL)
    */
    bool startLight(Road::TurnOption turnOpt);

public:
    /**
     * @brief Construct a new Road object. Allocates memory for a TrafficLight for each TurnOption
     *  with a number of lanes greater than 0.
     * 
     * @param dir               Road direction
     * @param numLanesArr       Number of lanes for each TurnOption
     */
    Road(RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr);
    
    /**
     * @brief Destroy the Road object. Deletes all TrafficLights in lights array.
     */
    ~Road();

    /**
     * @brief Gets the road direction to the right of "dir"
     * 
     * @param dir The starting direction
     * @return The RoadDirection to the right of "dir"
     */
    static RoadDirection roadRightOf(RoadDirection dir);
    
    /**
     * @brief Gets the road direction to the left of "dir"
     * 
     * @param dir The starting direction
     * @return The RoadDirection to the left of "dir"
     */
    static RoadDirection roadLeftOf(RoadDirection dir);

    /**
     * @brief Gets the road direction across from "dir"
     * 
     * @param dir The starting direction
     * @return The RoadDirection across from "dir"
     */
    static RoadDirection roadOppositeOf(RoadDirection dir);

    /**
     * @brief Sets this Road's green light. Allows straight travel and right turns if available.
     *
     * @return false if the straight TurnOption is unavailable.
    */
    bool setGreen();

    /**
     * @brief Sets this Road's greenLeft light. Allows left turns.
     *
     * @return false if this TurnOption is unavailable.
    */
    bool setGreenLeft();

    /**
     * @brief Sets this Road's greenRight light. Allows right turns.
     *
     * @return false if this TurnOption is unavailable.
    */
    bool setGreenRight();

    /**
     * @brief Sets the on duration, red duration, and yellow duration for all lights in the road.
     * 
     * @param onDur     the value to be set to all lights on duration
     * @param redDur    the value to be set to all lights red duration
     * @param yellowDur (optional) the value to be set to all lights yellow duration
     * 
     * @note A duration of -1 means infinite time.
     *
     * @return the number of lights that were set
    */
    int setAllLightDurations(int onDur, int redDur, int yellowDur=-1);

    RoadDirection getDirection(){ return direction; };
    int getNumLanes(TurnOption opt);
    TrafficLight* getLight(TurnOption opt);

    /**
     * @brief Gets a std::vector of all non-NULL TrafficLights in this Road.
     *
     * @return Vector of active TrafficLight pointers
    */
    std::vector<TrafficLight*> getLights();
};

#endif