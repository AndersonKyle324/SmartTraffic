#include "LightConfig.h"

bool DoubleGreen::start(Intersection *inter){
    return inter->doubleGreen(getDirection(), getDuration(), getYellowDuration());
}

bool SingleGreen::start(Intersection *inter){
    return inter->singleGreen(getDirection(), getDuration(), getYellowDuration());
}

bool DoubleGreenLeft::start(Intersection *inter){
    return inter->doubleGreenLeft(getDirection(), getDuration(), getYellowDuration());
}