#ifndef ROAD_H
#define ROAD_H

#include "TrafficLight.h"
#include "TurnOption.h"

class Road{
public:
    enum RoadDirection {north, east, south, west, numRoadDirections};

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

protected:
    RoadDirection direction;                                            /// The compass direction in which the road is facing when cars are at a stop
    std::array<TurnOption*, TurnOption::numTurnOptions> turnOptions;            /// Array of TurnOptions. Contains lane, TrafficLight, and vehicle info.
    bool exitRoad;                                                      /// Road is an intersection exit only, can only 

    /**
     * @brief Calls start() on the "turnOpt" TrafficLight.
     *
     * @param turnOpt       the desired light to start
     * @param onDuration    The onDuration to be set for the "turnOpt" light in seconds
     * @param yellowDuration    The duration of the yellow light in seconds
     * 
     * @return false if the light is unavailable (NULL)
    */
    bool startLight(TurnOption::Type turnOpt, double onDuration, double yellowDuration=DONT_SET);

public:
    /**
     * @brief Construct a new Road object. Allocates memory for a TrafficLight for each TurnOption
     *  with a number of lanes greater than 0. All TurnOptions will be initialized but not all will
     *  necessarily be valid if a TurnOption is unavailable.
     * 
     * @param dir               Road direction
     * @param numLanesArr       Number of lanes for each TurnOption
     * @param onDuration        The duration for the onColor for all lights in this Road in seconds
     * @param yellowDuration    The duration of the yellow light for all lights in this Road in seconds
     */
    Road(RoadDirection dir, std::array<int, TurnOption::numTurnOptions> numLanesArr, double onDuration, double yellowDuration);
    
    /**
     * @brief Destroy the Road object. Deletes all TrafficLights in lights array.
     */
    ~Road();

    /**
     * @brief Gets the finishing road direction when starting in "startDir" and taking a "turnOpt" turn.
     * 
     * @param startDir  The starting direction
     * @param turnOpt   The turn being made.
     * @return RoadDirection the direction of the exiting road
     */
    static RoadDirection exitRoadDirection(RoadDirection startDir, TurnOption::Type turnOpt);

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
     * @param onDuration    The onDuration to be set for the light(s)
     * @param yellowDuration    (optional) The duration of the yellow light in seconds
     *
     * @return the number of lights set
    */
    int setGreen(double onDuration, double yellowDuration=DONT_SET);

    /**
     * @brief Sets this Road's greenLeft light. Allows left turns.
     *
     * @param onDuration    The onDuration to be set for the light(s)
     * @param yellowDuration    (optional) The duration of the yellow light in seconds
     *
     * @return the number of lights set
    */
    int setGreenLeft(double onDuration, double yellowDuration=DONT_SET);

    /**
     * @brief Sets this Road's greenRight light. Allows right turns.
     *
     * @param onDuration    The onDuration to be set for the light(s)
     * @param yellowDuration    (optional) The duration of the yellow light in seconds
     *
     * @return the number of lights set
    */
    int setGreenRight(double onDuration, double yellowDuration=DONT_SET);

    /**
     * @brief Sets the on duration, red duration, and yellow duration for all lights in the road.
     * 
     * @param onDur     the value to be set to all lights on duration in seconds
     * @param redDur    the value to be set to all lights red duration in seconds
     * @param yellowDur (optional) the value to be set to all lights yellow duration in seconds
     * 
     * @note A duration of -1 means infinite time.
     *
     * @return the number of lights that were set
    */
    int setAllLightDurations(double onDur, double redDur, double yellowDur=DONT_SET);

    RoadDirection getDirection(){ return direction; };
    int getTotalNumLanes();
    int getNumLanes(TurnOption::Type opt);
    TurnOption* getTurnOption(TurnOption::Type opt);
    TrafficLight* getLight(TurnOption::Type opt);

    /**
     * @brief Gets a std::vector of all non-NULL TrafficLights in this Road.
     *
     * @return Vector of active TrafficLight pointers
    */
    std::vector<TrafficLight*> getLights();
};

#endif