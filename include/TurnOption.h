#ifndef TURN_OPTION_H
#define TURN_OPTION_H

#include "TrafficLight.h"

#define DEFAULT_MAX_LANE_VEHICLES (5)
#define DEFAULT_NUM_LANES (1)
#define DEFAULT_TIME_TO_CROSS (2)   

/**
 * @class TurnOption
 * @brief Represents a set of lanes that is directed by a single TrafficLight.
 * 
 * Also contains data related to vehicles in these lanes and their progress through the Intersection.
 * e.g. The left hand TurnOption has 3 lanes that are all directed by the same light.
 * 
 */
class TurnOption{
public:
    /**
     * @brief The available types of TurnOption for each Road. Defines where these lanes are located for this Road.
     */
    enum Type {left, straight, right, numTurnOptions};

    /**
     * @brief Checks "opt" is within the allowable range of TurnOption::Type values
     * 
     * @param opt   The TurnOption::Type to be checked
     * @return true if "opt" is between 0 and (TurnOption::numTurnOptions - 1)
     */
    static bool isValidTurnOption(Type& opt){
        if(opt >= numTurnOptions || opt < 0){
            throw std::out_of_range("TurnOption out of range");
            return false;
        }

        return true;
    }

protected:
    Type type;                                      ///< The relative location of these lanes for this Road.
    TrafficLight* light;                            ///< Pointer to the TrafficLight that directs these lanes.
    unsigned int numLanes;                          ///< The number of lanes
    unsigned int maxVehiclesPerLane;                ///< The max number of vehicles allowed per lane
    unsigned int timeToCross;                       ///< The number of ticks it takes for a vehicle to cross through the intersection

    unsigned int queuedVehicles;                    ///< The number of vehicles currently waiting
    unsigned int currentVehicleProgress;            ///< The number of ticks remaining for the vehicle(s) currently in the intersection to cross.
    unsigned int numVehiclesCurrentlyCrossing;      ///< The number of vehicles currently crossing the intersection.

public:
    /**
     * @brief Default constructor for TurnOption. Sets all values to 0, type is set to an invalid value.
     */
    TurnOption():   type(numTurnOptions),
                    light(NULL),
                    numLanes(0), 
                    maxVehiclesPerLane(0),
                    timeToCross(0),
                    queuedVehicles(0), 
                    currentVehicleProgress(0), 
                    numVehiclesCurrentlyCrossing(0)
                    {}
    
    TurnOption(Type aType, unsigned int lanes, unsigned int maxNumVehiclesPerLane, unsigned int crossTime, double lightDuration, double lightRedDuration=-1.0);   
    ~TurnOption(){ delete light; }
    
    /**
     * @brief Determines if this TurnOption object is valid
     * 
     * @return true  this TurnOption type is within the allowable range for TurnOption::Type
     * @return false 
     */
    bool isValid();

    /**
     * @brief Determines if the vehicle queue is full
     * 
     * @return true     the number of vehicles in the queue is >= the max queue size
     */
    bool queueIsFull();

    /**
     * @brief Determines if there are no vehicles in the queue
     * 
     * @return true     the number of vehicesl in the queue is 0
     */
    bool queueIsEmpty();

    /**
     * @brief Operations necessary to move vehicles currently in the intersection one tick
     *          closer to their destination. (Decrements currentVehicleProgress)
     */
    void progressVehicles();

    /**
     * @brief Determines if any vehicles from this TurnOption are currently crossing the intersection
     * 
     * @return true     there are vehicles presently crossing the intersection
     */
    bool vehiclesAreCrossing();

    /**
     * @brief The operations that take place when the next set of vehicles begin traveling throught the intersection.
     * Increments exit queue by the number of vehicles that have just finished crossing.
     * Adds vehicles into Intersection crossing only if light is green, not yellow/red.
     * 
     * @param exitTurnOpt   The TurnOption being exited onto, the exiting vehicles will be added to this queue.
     */
    void nextVehiclesBeginCrossing(TurnOption *exitTurnOpt);

    /**
     * @brief Operations to be taken when vehicles are still in the Intersection when the TrafficLight has turned red.
     * Sets the currentVehicleProgress and numVehiclesCurrentlyCrossing to 0 and prints Traffic Jam indication.
     * 
     * @return true     There are indeed vehicles left in the Intersection 
     * @return false    There are not vehicles left in the Intersection
     */
    bool vehiclesLeftInIntersection();

    /**
     * @brief Add "numVehiclesToAdd" vehicles to the queue. The queue is limited to 
     *  the size returned by getMaxNumVehicles().
     * 
     * @param numVehicles 
     * @return true all "numVehiclesToAdd" vehicles were added without exceeding the max queue size
     * @return false "numVehiclesToAdd" <= 0 or adding "numVehiclesToAdd" exceeds the max queue size
     */
    bool addVehicles(int numVehiclesToAdd);

    Type getType(){ return type; }
    TrafficLight* getLight(){ return light; }
    unsigned int getNumLanes(){ return numLanes; }
    unsigned int getMaxVehiclesPerLane(){ return maxVehiclesPerLane; }
    unsigned int getMaxNumVehicles(){ return getMaxVehiclesPerLane() * getNumLanes(); }
    unsigned int getTimeToCross(){ return timeToCross * refreshRateHzGlobal; }
    unsigned int getQueuedVehicles(){ return queuedVehicles; }
    unsigned int getCurrentVehicleProgress(){ return currentVehicleProgress; }
    unsigned int getNumVehiclesCurrentlyCrossing(){ return numVehiclesCurrentlyCrossing; }

};

#endif