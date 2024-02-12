#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <array>
#include <iostream>

#define DEFAULT_ON_DURATION (1)
#define DEFAULT_YELLOW_DURATION (1)

/**
 * @class TrafficLight
 * @brief Represents a traffic light.
 * 
 * This class models a traffic light with various colors and durations as well as the lanes
 * that are directed by this light, including the vehicles queued in these lanes.
 */
class TrafficLight{

public:
    /**
     * @brief Enumeration of available colors for the traffic light.
     */
    enum AvailableColors {green, greenLeft, greenRight, yellow, red, numColors};

    int yellowDuration = DEFAULT_YELLOW_DURATION; ///< The duration of the yellow light.

protected:
    AvailableColors onColor; ///< The color direction of the traffic light.
    AvailableColors color; ///< The current color of the traffic light.
    int durationRemaining; ///< The remaining duration for the current color.
    std::array<int, numColors> colorDuration; ///< Array of durations for each color.
    
    /// Variables associated with the lanes directed by this light.
    unsigned long numVehiclesDirected;   ///< The total number of vehicles directed by this light that have crossed through the intersection.

public:
    /**
     * @brief Default constructor for TrafficLight.
     */
    TrafficLight(): color(red), 
                    durationRemaining(-1), 
                    colorDuration{0, 0, 0, yellowDuration, -1}, 
                    numVehiclesDirected(0)
                    {};

    /**
     * @brief Construct a new Traffic Light object specifying color direction and durations
     * 
     * @param aOnColor   color direction of onColor
     * @param onColorDur duration of the onColor
     * @param redDur     duration of the red light
     * @param lanes      number of lanes directed by this light
     */
    TrafficLight(AvailableColors aOnColor, int onColorDur, int redDur);

    friend class Intersection; ///< Friend class Intersection.

    /**
    * @brief Starts the TrafficLight by setting its color to the onColor.
    */
    void start();

    /**
     * @brief Decreases the durationRemaining and updates the state of the TrafficLight.
     * 
     * If the durationRemaining is > 0, decrement durationRemaining. Then call nextState()
     *
     * @return The updated durationRemaining value.
     *
     * @warning if durationRemaining is negative, it will remain in the same state
     */
    int tick();

    /**
     * @brief Determines and sets the next state of the TrafficLight based on the current color and duration.
     * 
     * If the durationRemaining is 0, procede to the next state. 
     *
     * @return The new color of the TrafficLight.
     * 
     * @throws std::out_of_range if TrafficLight reaches an unexpected color state.
     * @warning if durationRemaining is negative, it will remain in the same state
     */
    AvailableColors nextState();

    /**
     * @brief Get the Duration Remaining
     * 
     * @return durationRemaining
     */
    int getDurationRemaining(){ return durationRemaining; };

    /**
     * @brief Gets the duration for a specific color.
     *
     * @param durColor The color for which to get the duration.
     * @return The duration for the specified color.
     */
    int getColorDuration(AvailableColors durColor){ return colorDuration[durColor]; };

    /**
     * @brief Gets the current color of the traffic light.
     *
     * @return The current color.
     */
    AvailableColors getColor(){ return color; };

    /**
     * @brief Checks if light is any form of green (green, greenleft, or greenRight)
     * 
     * @return true light is green, greenLeft, or greenRight
     * @return false light is yellow or red
     */
    bool isGreen(){ return (getColor() == green || getColor() == greenLeft || getColor() == greenRight); }

    /**
     * @brief Gets the onColor of the traffic light.
     *
     * @return The onColor.
     */
    AvailableColors getOnColor(){ return onColor; };

    unsigned long getNumVehiclesDirected(){ return numVehiclesDirected; }

    /**
     * @brief Adds "numVehicles" to the total number of vehicles directed by this light
     * 
     * @param numVehicles the amount to be added
     * @return int the new total directed
     */
    int addVehiclesDirected(int numVehicles);

    /**
     * @brief Sets the duration remaining to the duration for the current color.
     */
    void resetDurationRemaining();

    /**
     * @brief Sets the duration remaining to newDuration.
     *
     * @param newDuration The new duration value.
     */
    void setDurationRemaining(int newDuration){ durationRemaining = newDuration; }

    /**
     * @brief Sets the durationRemaining to the duration of a specific color.
     *
     * @param durColor The color of the duration to be set to durationRemaining.
     */
    void setDurationRemainingColor(AvailableColors durColor){ durationRemaining = colorDuration[durColor]; }

    /**
     * @brief Sets the duration for a specific color.
     *
     * @param durColor The color for which to set the duration.
     * @param duration The duration value. The number of ticks to stay on "durColor" color.
     */
    void setDuration(AvailableColors durColor, int duration){ colorDuration[durColor] = duration; }

    /**
     * @brief Sets the duration for a specific color.
     *
     * @param duration The duration value. The number of ticks to stay on onColor color.
     */
    void setOnDuration(int duration){ setDuration(onColor, duration); }

    /**
     * @brief Sets the color of the traffic light.
     *
     * @param newColor The new color.
     */
    void setColor(AvailableColors newColor){ 
        isValidColor(newColor);

        color = newColor;
        setDurationRemainingColor(newColor);
    }

    static bool isValidColor(const AvailableColors& aColor){
        if(aColor < 0 || aColor >= numColors){
            throw std::out_of_range("Unexpected color");
            return false;
        }

        return true;
    }

    std::string toString();

    /**
    * @brief Overloaded << operator to output TrafficLight::AvailableColors to a stream.
    *
    * @param out The output stream.
    * @param data The TrafficLight::AvailableColors to be output.
    * @return The modified output stream.
    */
    friend std::ostream& operator<<(std::ostream &out, TrafficLight::AvailableColors const& data);

    /**
     * @brief Overloaded << operator to output a TrafficLight object to a stream.
     *
     * Outputs the color and duration information of the TrafficLight object.
     * Format e.g. : <c:'green' dr:10 15-0-0-3-20>
     *
     * @param out The output stream.
     * @param data The TrafficLight object to be output.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream &out, const TrafficLight& data);
};

/**
 * @class TrafficLightLeft
 * @brief Represents a left-turn traffic light.
 *
 * Inherits from TrafficLight and provides left-turn specific behavior.
 */
class TrafficLightLeft : public TrafficLight{
    /**
    * @brief Overloaded << operator to output a TrafficLightLeft object to a stream.
    *
    * Outputs the left-specific information of the TrafficLightLeft object.
    * Format e.g. : {Left <c:'green' dr:10 15-0-0-3-20>}
    *
    * @param out The output stream.
    * @param data The TrafficLightLeft object to be output.
    * @return The modified output stream.
    */
    friend std::ostream& operator<<(std::ostream &out, TrafficLightLeft& data);

public:
    /**
     * @brief Default constructor for TrafficLightLeft.
     *
     * Calls parent consructor for TrafficLight and sets the onColor to greenLeft.
     */
    TrafficLightLeft();

    /**
     * @brief Construct a new Traffic Light Left object. Setting onColor to greenLeft and
     * greenLefts duration to "leftDur".
     * 
     * @param onDur duration to be set to greenLeft
     */
    TrafficLightLeft(int leftDur, int redDur);

};

#endif