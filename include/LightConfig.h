#ifndef LIGHT_CONFIG_H
#define LIGHT_CONFIG_H

#include "Road.h"
#include "Intersection.h"

/**
 * @class LightConfig
 * @brief Defines which lights in an Intersection are on as well as for how long
            the Intersection should remain in this configuration.
*/
class LightConfig{
public:

    /**
     * @brief The possible configurations for a Road or Roads in an Intersection.
     *
     * @warning if a new option is added, a new switch case must be added to Intersection::setLightConfig()
    */
    enum Option {doubleGreen, singleGreen, doubleGreenLeft, numConfigOptions};

protected:
    Option configOpt;               ///< The desired road configuration option
    Road::RoadDirection direction;  ///< The direction of the configOpt
    double duration;                ///< The duration to remain in this config in seconds
    double yellowDuration;          ///< The duration to remain in yellow once the duration has been exceeded

public:
    LightConfig(Option interConfigOpt, Road::RoadDirection dir, double newDuration, double newYellowDuration) : configOpt(interConfigOpt), direction(dir), duration(newDuration), yellowDuration(newYellowDuration) {}
    
    ~LightConfig() {}

    Option getConfigOption() { return configOpt; }
    Road::RoadDirection getDirection(){ return direction; }
    double getDuration(){ return duration; }
    double getYellowDuration(){ return yellowDuration; }
    double getTotalDuration(){ return duration + yellowDuration; }

};

#endif