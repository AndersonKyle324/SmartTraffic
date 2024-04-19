#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <array>
#include <iostream>

extern int refreshRateHzGlobal;

#define DEFAULT_ON_DURATION (1)
#define DEFAULT_YELLOW_DURATION (1)

/**
 * @class TrafficLight
 * @brief Represents a traffic light.
 * 
 * This class models a traffic light with various colors and durations
 */
class TrafficLight{

public:
    /**
     * @brief Enumeration of available colors for the traffic light.
     */
    enum AvailableColors {green, greenLeft, greenRight, yellow, red, numColors};

    double yellowDuration = DEFAULT_YELLOW_DURATION; ///< The duration of the yellow light in seconds.

protected:
    AvailableColors onColor; ///< The color direction of the traffic light.
    AvailableColors color; ///< The current color of the traffic light.
    int ticksRemaining; ///< The remaining duration for the current color in ticks.
    std::array<double, numColors> colorDuration; ///< Array of durations for each color in seconds.
    
    /// Variables associated with the lanes directed by this light.
    unsigned long numVehiclesDirected;   ///< The total number of vehicles directed by this light that have crossed through the intersection.

public:
    /**
     * @brief Default constructor for TrafficLight.
     */
    TrafficLight(): color(red), 
                    ticksRemaining(-1), 
                    colorDuration{0.0, 0.0, 0.0, yellowDuration, -1.0}, 
                    numVehiclesDirected(0)
                    {};

    /**
     * @brief Construct a new Traffic Light object specifying color direction and durations
     * 
     * @param aOnColor   color direction of onColor
     * @param onColorDur duration of the onColor in seconds
     * @param redDur     duration of the red light in seconds
     * @param lanes      number of lanes directed by this light
     */
    TrafficLight(AvailableColors aOnColor, double onColorDur, double redDur);

    friend class Intersection; ///< Friend class Intersection.

    /**
    * @brief Starts the TrafficLight by setting its color to the onColor.
    */
    void start();

    /**
     * @brief Decreases the ticksRemaining and updates the state of the TrafficLight.
     * 
     * If the ticksRemaining is > 0, decrement ticksRemaining. Then call nextState()
     *
     * @return The updated ticksRemaining value.
     *
     * @warning if ticksRemaining is negative, it will remain in the same state
     */
    int tick();

    /**
     * @brief Determines and sets the next state of the TrafficLight based on the current color and duration.
     * 
     * If the ticksRemaining is 0, procede to the next state. 
     *
     * @return The new color of the TrafficLight.
     * 
     * @throws std::out_of_range if TrafficLight reaches an unexpected color state.
     * @warning if ticksRemaining is negative, it will remain in the same state
     */
    AvailableColors nextState();

    /**
     * @brief Get the time remaining until next state in ticks
     * 
     * @return ticks remaining until next state
     */
    int getTicksRemaining(){ return ticksRemaining; };

    /**
     * @brief Gets the duration for a specific color in seconds.
     *
     * @param durColor The color for which to get the duration.
     * @return The duration for the specified color.
     */
    double getColorDuration(AvailableColors durColor){ return colorDuration[durColor]; };

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
     * @brief Checks if light is yellow
     * 
     * @return true light is yellow
     * @return false light is any green or red
     */
    bool isYellow(){ return (getColor() == yellow); }

    /**
     * @brief Checks if light is red
     * 
     * @return true light is red
     * @return false light is any green or yellow
     */
    bool isRed(){ return (getColor() == red); }

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
     * @brief Sets the ticks remaining to the duration for the current color.
     */
    void resetTicksRemaining();

    /**
     * @brief Sets the ticks remaining to newDuration * refresh rate.
     *
     * @param newDuration The new duration value in seconds.
     */
    void setTicksRemaining(double newDuration){ ticksRemaining = static_cast<int>(newDuration * refreshRateHzGlobal); }

    /**
     * @brief Sets the ticksRemaining to the duration of a specific color.
     *
     * @param durColor The color of the duration to be set to ticksRemaining.
     */
    void setTicksRemainingColor(AvailableColors durColor){ ticksRemaining = static_cast<int>(colorDuration[durColor] * refreshRateHzGlobal); }

    /**
     * @brief Sets the duration for a specific color.
     *
     * @param durColor The color for which to set the duration.
     * @param duration The duration value. The number of seconds to stay on "durColor" color.
     */
    void setDuration(AvailableColors durColor, double duration){ colorDuration[durColor] = duration; }

    /**
     * @brief Sets the duration for the onColor.
     *
     * @param duration The duration value. The number of seconds to stay on onColor color.
     */
    void setOnDuration(double duration){ setDuration(onColor, duration); }

    /**
     * @brief Sets the current color of the traffic light.
     *
     * @param newColor The new color.
     */
    void setColor(AvailableColors newColor){ 
        isValidColor(newColor);

        color = newColor;
        setTicksRemainingColor(newColor);
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