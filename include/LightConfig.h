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
    */
    enum Option {doubleRoad, singleRoad, doubleLeftRoad, numConfigOptions};

protected:
    Option configOpt;               ///< The desired road configuration option
    Road::RoadDirection direction;  ///< The direction of the configOpt
    int duration;                   ///< The duration to remain in this config

public:
    LightConfig(Option interConfigOpt, Road::RoadDirection dir, int newDuration) : configOpt(interConfigOpt), direction(dir), duration(newDuration) {}

    Option getConfigOption() { return configOpt; }
    Road::RoadDirection getDirection(){ return direction; }
    int getDuration(){ return duration; }

};

#endif