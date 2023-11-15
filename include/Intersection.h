#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <array>
#include "TrafficLight.h"

#define MIN_NUM_ROADS (3)

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

    int setAllLightDurations(int onDur, int redDur, int yellowDur=-1);

    RoadDirection getDirection(){ return direction; };
    int getNumLanes(TurnOption opt);
    TrafficLight* getLight(TurnOption opt);
};

class RoadExit : public Road{

};

class Intersection{
public:
    enum IntersectionError {success, unknown, alreadyExists, turnNotPossible};

protected:
    int numRoads;
    std::array<Road*, Road::numRoadDirections> roads;
    std::array<bool, Road::numRoadDirections> expectedRoads;

    /**
     * @brief 
     * 
     * @param endRoadDir        Direction of road to turn onto
     * @param numNewLanes       Number of proposed new lanes
     * @param roadIsExpected    A boolean set true by this function when there is an expectedRoad
     * @return true The proposed turn does not exceed to number of available lanes in "endRoadDir"
     *          or the end road is missing and a new expectedRoad is set.
     * @return false 
     */
    bool newTurnIsPossible(Road::RoadDirection endRoadDir, int numNewLanes, bool* roadIsExpected);
    bool newRoadIsPossible(Road::RoadDirection dir, int numLeftLanes, int numRightLanes);

public:
    Intersection();

    ~Intersection();

    /**
     * @brief Checks there are no missing roads in the intersection
     * 
     * @param out the stream to print success or failure messages to
     * 
     * @return true 
     * @return false There is an unsatisfied expectedRoad, prints a message.
     */
    bool validate(std::ostream out);

    /**
     * @brief Checks if road at "dir" is currently present in this Intersection
     * 
     * @param dir direction of the road to check for
     * @return true 
     * @return false 
     */
    bool roadExists(Road::RoadDirection dir);

    /**
     * @brief Creates new Road while checking to make sure it conforms with other Roads already present.
     * 
     * @param dir           The direction the road is facing when cars are at a stop
     * @param numLanesArr   The number of lanes for each TurnOption. Should be Road::numTurnOptions long.
     * @return Success if road is added, else returns an IntersectionError value representing the specific error. 
     */
    int addRoad(Road::RoadDirection dir, std::array<int, Road::numTurnOptions> numLanesArr);

    int setAllLightDurations(int onDur, int yellowDur=-1);

    int getNumRoads(){ return numRoads; }
    Road* getRoad(Road::RoadDirection dir);
    bool roadIsExpected(Road::RoadDirection dir);
};

#endif