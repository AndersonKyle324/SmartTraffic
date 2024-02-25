#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <array>
#include <vector>
#include "TrafficLight.h"
#include "Road.h"
#include "LightConfig.h"

#define MIN_NUM_ROADS    (3)

/// #defines used for the print() function
#define MAX_LEN_RIGHT    (10)
#define MAX_LEN_LEFT     (9)
#define MAX_LEN_STR      (6)
#define SPACE            (1)
#define GAP              (3)
#define SLOT_WEST        (0)
#define SLOT_SOUTH_RIGHT (SLOT_WEST + MAX_LEN_RIGHT + GAP)
#define SLOT_SOUTH_STR   (SLOT_SOUTH_RIGHT + MAX_LEN_RIGHT + SPACE)
#define SLOT_SOUTH_LEFT  (SLOT_SOUTH_STR + MAX_LEN_STR + SPACE)
#define SLOT_NORTH_LEFT  (SLOT_SOUTH_LEFT + MAX_LEN_LEFT + GAP)
#define SLOT_NORTH_STR   (SLOT_NORTH_LEFT + MAX_LEN_LEFT + SPACE)
#define SLOT_NORTH_RIGHT (SLOT_NORTH_STR + MAX_LEN_STR + SPACE)
#define SLOT_EAST        (SLOT_NORTH_RIGHT + MAX_LEN_RIGHT + GAP)
#define LEN_LINE         (SLOT_EAST + MAX_LEN_RIGHT + SPACE)

class Intersection{
public:
    enum IntersectionError {success, unknown, alreadyExists, turnNotPossible};

protected:
    int numRoads;                                               ///< Number of Roads in the Intersection
    std::array<Road*, Road::numRoadDirections> roads;           ///< Array of the Intersection Road pointers
    std::array<bool, Road::numRoadDirections> expectedRoads;    ///< Array indicating what roads still need to added to the intersection for it to be positively validated.
    std::vector<LightConfig*> configSchedule;                   ///< The vector containing the LightConfig schedule in order.
    unsigned long configScheduleIdx;                            ///< The index in configSchedule indicating the LightConfig the Intersecion is currently on.
    int numUnfinishedLights;                                    ///< The number of lights for the current config that have not yet turned red     
    unsigned long ticksSinceStart;                              ///< Total number of times tick() has been called on this Intersection

    /**
     * @brief Checks to see if "light" should be ticked and updates the Intersections
     *          unfinished lights count if the light transitions to red.
     * 
     * @param light the TrafficLight object to be checked
     */
    void handleLightTick(TrafficLight* light);
    
    /**
     * @brief Advances vehicles currenly crossing intersection and adds new vehicles to cross
     *          if the right conditions are met.
     * 
     * @param rd        The desired Road
     * @param turnOpt   The desired TurnOption in "rd"
     */
    void handleVehicles(Road* rd, TurnOption::Type opt);

    /**
     * @brief Checks to see if this intended new turn for a new road is compatible with the current state of the
     *          intersection. This is based on whether the exit road already exists and that is has enough
     *          lanes to satisfy the other roads leading to it.
     * 
     * @param endRoadDir        Direction of road to turn onto
     * @param numNewLanes       Number of proposed new lanes
     * @param roadIsExpected    A boolean set true by this function when there is an expectedRoad
     * @return true The proposed turn does not exceed to number of available lanes in "endRoadDir"
     *          or the end road is missing and a new expectedRoad is set.
     * @return false 
     */
    bool newTurnIsPossible(Road::RoadDirection endRoadDir, int numNewLanes, bool* roadIsExpected);
    
    /**
     * @brief Checks to see if this intended new road is compatible with the current state of the
     *          intersection. This is based on whether the intended turns are compatible with the
     *          roads that already exist. If the roads are needed and they do not already exist, 
     *          an expectedRoad will be set.
     * 
     * @param dir           The direction of the new road
     * @param numLeftLanes  The number of left lanes for the new road
     * @param numRightLanes The number of right lanes for the new road
     *
     * @return true when the road is currently possible.
    */
    bool newRoadIsPossible(Road::RoadDirection dir, int numLeftLanes, int numRightLanes);
    
    /**
    * @brief Get the current light state for the desired road as an ascii art string
    *
    * @note This is a helper function for Intersection::print()
    *
    * @param dir    The road to write a string for
    * @param outStr The string to be filled
    */
    void printHelper(Road::RoadDirection dir, std::string& outStr);

    /**
     * @brief Sets the Intersection to the "idx" LightConfig in the schedule vector.
     *
     * @param idx       The idx of the desired LightConfig in configSchedule vector
     *
     * @return false if any of the specified Roads are NULL of if an unhandled LightConfig::Option is requested.
    */
    bool setLightConfig(int idx);

public:
    Intersection();

    ~Intersection();

    /**
     * @brief Checks there are no missing roads in the intersection
     * 
     * @param out the stream to print success or failure messages to
     * 
     * @return true 
     * @return false There is an unsatisfied expectedRoad, prints a message or there are less than
     *          #MIN_NUM_ROADS.
     */
    bool validate();

    /**
     * @brief Call tick() for all TrafficLights in this Intersection and advance
     *  vehicles waiting at each light accordingly.
     *
     * @return the number of unfinished lights in the Intersection.
    */
    int tick();

    /**
     * @brief Add a LightConfig to the end of the Intersections current configSchedule vector
     *
     * @param configOpt     The LightConfig::Option specifying which type of configuration will be set
     * @param direction     The RoadDirection of "configOpt". The road in which the config option is being applied to
     * @param duration      The duration the Intersection will remain in this configuration
     *
     * @return true upon success
    */
    bool schedule(LightConfig::Option configOpt, Road::RoadDirection direction, int duration);
    bool schedule(LightConfig& config);

    /**
     * @brief Begins light operation for the intersection, looping through all LightConfig's scheduled indefinetly.
     *
     * @return false if there is an error
    */
    bool start();

    /**
     * @brief Sets the Intersection to the next scheduled LightConfig in configSchedule. If the last
     *          LightConfig is reached, loop back to the first LightConfig.
     * 
     * @return false if the scheduled LightConfig fails
    */
    bool nextLightConfig();

    /**
     * @brief Gets pointer to the LightConfig the intersection is currently on
     * 
     * @return LightConfig* the current LightConfig
     */
    LightConfig* currentLightConfig(){ return configSchedule.at(configScheduleIdx); }

    /**
     * @brief Sets two opposite roads green. Allowing both straight and right Road::turnOptions for both Roads.
     *
     * @param dir           The direction to be set green, the direction opposite of this will also be set green.
     * @param onDuration    The duration for this light config
     *
     * @warning if this road is missing the afformentioned lights, nothing will happen
     *
     * @return false if road in "dir" is missing or road opposite of "dir" is missing
    */
    bool doubleGreen(Road::RoadDirection dir, int onDuration);

    /**
     * @brief Sets one roads green, greenLeft, and greenRight. Allowing both straight, left, and right Road::turnOptions for this Road alone.
     *
     * @param dir The direction to be set green
     * @param onDuration    The duration for this light config
     *
     * @warning if this road is missing the afformentioned lights, nothing will happen
     *
     * @return false if road "dir" is missing
    */
    bool singleGreen(Road::RoadDirection dir, int onDuration);

    /**
     * @brief Sets two opposite roads greenLeft. Allowing left Road::turnOptions for both Roads.
     *
     * @param dir The direction to be set greenLeft, the direction opposite of this will also be set greenLeft.
     * @param onDuration    The duration for this light config
     *
     * @warning if this road is missing the afformentioned lights, nothing will happen
     *
     * @return false if road "dir" is missing or if road opposite of "dir" is missing
    */
    bool doubleGreenLeft(Road::RoadDirection dir, int onDuration);

    /**
     * @brief Checks if road at "dir" is currently present in this Intersection
     * 
     * @param dir direction of the road to check for
     * @return true 
     * @return false 
     */
    bool roadExists(Road::RoadDirection dir);

    /**
     * @brief Get the Exit Road object
     * 
     * @param startDir  the starting road direction
     * @param turnOpt   The turn being made
     * @return Road* pointer to the Road being exited onto
     */
    Road* getExitRoad(Road::RoadDirection startDir, TurnOption::Type turnOpt);

    /**
     * @brief Creates new Road while checking to make sure it conforms with other Roads already present.
     * 
     * @param dir           The direction the road is facing when cars are at a stop
     * @param numLanesArr   The number of lanes for each TurnOption. Should be Road::numTurnOptions long.
     * @param onDuration    (Optional)The duration of the onColor for all lights in this Road
     *
     * @return Success if road is added, else returns an IntersectionError value representing the specific error. 
     */
    int addRoad(Road::RoadDirection dir, std::array<int, TurnOption::numTurnOptions> numLanesArr, int onDuration=DEFAULT_ON_DURATION);

    /**
     * @brief Add "numVehiclesToAdd" vehicles to the vehicle queue on road "dir" for TurnOption::Type "turn". The queue is limited to 
     *  the size returned by turnOpt->getMaxNumVehicles().
     * 
     * @param dir           the desired Road
     * @param turn          the desired TurnOption for Road at "dir"
     * @param numVehicles   the max number of vehicles to add to the vehicle queue
     * @return true all "numVehiclesToAdd" vehicles were added without exceeding the max queue size
     * @return false "numVehiclesToAdd" <= 0 or adding "numVehiclesToAdd" exceeds the max queue size or this Road DNE.
     */
    bool addVehicles(Road::RoadDirection dir, TurnOption::Type turn, int numVehiclesToAdd);

    /**
     * @brief Sets the on duration and yellow duration for all lights in the intersection
     * 
     * @param onDur     the value to be set to all lights on duration
     * @param yellowDur (optional) the value to be set to all lights yellow duration
     * 
     * @return the number of roads that were set
    */
    int setAllLightDurations(int onDur, int yellowDur=-1);

    int getNumRoads(){ return numRoads; }
    Road* getRoad(Road::RoadDirection dir);
    bool roadIsExpected(Road::RoadDirection dir);
    
    /**
    * @brief Gets the TrafficLight pointer for the desired RoadDirection and turnOption
    * 
    * @param dir        The direction of the desired road
    * @param turn       The desired light on road "dir"
    *
    * @return pointer to the desired TrafficLight
    */
    TrafficLight* getLight(Road::RoadDirection dir, TurnOption::Type turn);

    /**
     * @brief Gets a std::vector of all non-NULL TrafficLights in this intersection.
     *
     * @return Vector of active TrafficLight pointers
    */
    std::vector<TrafficLight*> getLights();

    int getNumUnfinishedLights(){ return numUnfinishedLights; }

    /**
     * @brief Gets ticksSinceStart i.e. the current time in ticks
     * 
     * @return ticksSinceStart unsigned long
    */
    unsigned long time(){ return ticksSinceStart; }
    
    /**
    * @brief Prints the current state of all the lights and vehicles queues in the intersection in an
    *            ascii art fashion to look like a birds eye view of an intersection.
    */
    void print();
};

#endif