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

protected:
    Road::RoadDirection direction;  ///< The direction of the configOpt
    double duration;                ///< The duration to remain in this config in seconds
    double yellowDuration;          ///< The duration to remain in yellow once the duration has been exceeded

public:
    LightConfig(Road::RoadDirection dir, double newDuration, double newYellowDuration) : direction(dir), duration(newDuration), yellowDuration(newYellowDuration) {}
    
    ~LightConfig() {}

    virtual bool start(Intersection *inter) = 0;

    Road::RoadDirection getDirection(){ return direction; }
    double getDuration(){ return duration; }
    double getYellowDuration(){ return yellowDuration; }
    double getTotalDuration(){ return duration + yellowDuration; }

};

class DoubleGreen : public LightConfig{
public:
    DoubleGreen(Road::RoadDirection dir, double newDuration, double newYellowDuration) : LightConfig{dir, newDuration, newYellowDuration} {}

    bool start(Intersection *inter);
};

class SingleGreen : public LightConfig{
public:
    SingleGreen(Road::RoadDirection dir, double newDuration, double newYellowDuration) : LightConfig{dir, newDuration, newYellowDuration} {}

    bool start(Intersection *inter);
};

class DoubleGreenLeft : public LightConfig{
public:
    DoubleGreenLeft(Road::RoadDirection dir, double newDuration, double newYellowDuration) : LightConfig{dir, newDuration, newYellowDuration} {}

    bool start(Intersection *inter);
};

#endif