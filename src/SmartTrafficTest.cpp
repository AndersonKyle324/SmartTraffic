#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest/doctest.h"

#include "TrafficLight.h"
#include "Intersection.h"
#include "LightConfig.h"

TEST_CASE("TC_1-1_TF_start"){
    TrafficLightLeft tf = TrafficLightLeft();

    tf.start();

    CHECK(tf.getColor() == TrafficLight::greenLeft);
}

TEST_CASE("TC_1-2_TF_setDuration"){
    TrafficLight tl = TrafficLight(TrafficLight::green, 3, -1);

    CHECK(tl.getColorDuration(TrafficLight::green) == 3);
    CHECK(tl.getColorDuration(TrafficLight::red) == -1);

    tl.start();

    CHECK(tl.getDurationRemaining() == 3); 

    tl.setDuration(TrafficLight::green, 4);
    tl.start();
    CHECK(tl.getDurationRemaining() == 4); 

    tl.setOnDuration(5);
    tl.start();
    CHECK(tl.getDurationRemaining() == 5); 
}

TEST_CASE("TC_2-1_TF_tick"){
    TrafficLightLeft tf = TrafficLightLeft();
    tf.start();
}

TEST_CASE("TC_3-1_TF_nextState"){
    TrafficLightLeft tf = TrafficLightLeft(1, 2);
    TrafficLight::AvailableColors color;
    tf.start();

    color = tf.nextState();
    CHECK(color == TrafficLight::greenLeft);
    CHECK(tf.getDurationRemaining() == tf.getColorDuration(color));

    tf.setDurationRemaining(0);
    color = tf.nextState();
    CHECK(color == TrafficLight::yellow);
    CHECK(tf.getDurationRemaining() == tf.getColorDuration(color));

    tf.setDurationRemaining(0);
    color = tf.nextState();
    CHECK(color == TrafficLight::red);
    CHECK(tf.getDurationRemaining() == tf.getColorDuration(color));

    tf.setDurationRemaining(0);
    color = tf.nextState();
    CHECK(color == TrafficLight::greenLeft);
    CHECK(tf.getDurationRemaining() == tf.getColorDuration(color));
}

TEST_CASE("TC_4-1_TF_TrafficLightLeft"){
    int onColorDur = 4;
    int redDur = 5;
    TrafficLight tf = TrafficLight(TrafficLight::greenLeft, onColorDur, redDur);

    CHECK(tf.getColor() == TrafficLight::red);
    CHECK(tf.getOnColor() == TrafficLight::greenLeft);
    CHECK(tf.getColorDuration(TrafficLight::greenLeft) == onColorDur);
    CHECK(tf.getColorDuration(TrafficLight::red) == redDur);
    CHECK(tf.getColorDuration(TrafficLight::green) == 0);
    CHECK(tf.getColorDuration(TrafficLight::greenRight) == 0);
    CHECK(tf.getColorDuration(TrafficLight::yellow) == tf.yellowDuration);
}

TEST_CASE("TC_5-1_TF_isValidColor"){
    TrafficLight::AvailableColors color = TrafficLight::red;

    CHECK(TrafficLight::isValidColor(color));
    
    color = TrafficLight::green;
    CHECK(TrafficLight::isValidColor(color));

    color = TrafficLight::greenRight;
    CHECK(TrafficLight::isValidColor(color));

    color = TrafficLight::greenLeft;
    CHECK(TrafficLight::isValidColor(color));

    color = TrafficLight::yellow;
    CHECK(TrafficLight::isValidColor(color));

    color = TrafficLight::numColors;
    CHECK_THROWS_AS( ! TrafficLight::isValidColor(color), std::out_of_range);

    color = (TrafficLight::AvailableColors)-1;
    CHECK_THROWS_AS( ! TrafficLight::isValidColor(color), std::out_of_range);

    color = (TrafficLight::AvailableColors)6;
    CHECK_THROWS_AS( ! TrafficLight::isValidColor(color), std::out_of_range);
}

TEST_CASE("TC_6-1_TF_tick"){
    int leftDur = 1;
    int redDur = 2;
    TrafficLightLeft tf = TrafficLightLeft(leftDur, redDur);
    int durRem;
    tf.start();

    // Start Left
    durRem = tf.tick();
    // Go Yellow
    CHECK(durRem == tf.yellowDuration);
    CHECK(tf.getDurationRemaining() == tf.yellowDuration);
    CHECK(tf.getColor() == TrafficLight::yellow);

    for(int i=(tf.yellowDuration - 1); i>0; i--){
        // Make sure still yellow during whole dur
        durRem = tf.tick();
        CHECK(durRem == i);
        CHECK(tf.getDurationRemaining() == i);
        CHECK(tf.getColor() == TrafficLight::yellow);
    }

    durRem = tf.tick();
    // Go Red
    CHECK(durRem == redDur);
    CHECK(tf.getDurationRemaining() == redDur);
    CHECK(tf.getColor() == TrafficLight::red);
    for(int i=(redDur - 1); i>0; i--){
        // Make sure still red during whole dur
        durRem = tf.tick();
        CHECK(durRem == i);
        CHECK(tf.getDurationRemaining() == i);
        CHECK(tf.getColor() == TrafficLight::red);
    }

    durRem = tf.tick();
    // Go Left
    CHECK(durRem == leftDur);
    CHECK(tf.getDurationRemaining() == leftDur);
    CHECK(tf.getColor() == TrafficLight::greenLeft);

    durRem = tf.tick();
    // Go back to yellow again
    CHECK(durRem == tf.yellowDuration);
    CHECK(tf.getDurationRemaining() == tf.yellowDuration);
    CHECK(tf.getColor() == TrafficLight::yellow);
}

TEST_CASE("TC_6-2_TF_tick_no_change"){
    int leftDur = -1;
    int redDur = 2;
    TrafficLightLeft tf = TrafficLightLeft(leftDur, redDur);
    int durRem;
    tf.start();
    CHECK(tf.getDurationRemaining() == -1);

    // Start Left
    durRem = tf.tick();
    CHECK(durRem == -1);
    CHECK(tf.getDurationRemaining() == -1);
    CHECK(tf.getColor() == TrafficLight::greenLeft);

    durRem = tf.tick();
    // Still Left
    CHECK(durRem == -1);
    CHECK(tf.getDurationRemaining() == -1);
    CHECK(tf.getColor() == TrafficLight::greenLeft);
}

TEST_CASE("TC_7-1_INT_addRoad"){
    Intersection inter = Intersection();
    int retVal;

    /// Good first road
    retVal = inter.addRoad(Road::north, {3, 4, 5});
    CHECK(retVal == Intersection::success);
    CHECK(inter.roadExists(Road::north) == true);
    
    CHECK(inter.roadIsExpected(Road::north) == false);
    CHECK(inter.roadIsExpected(Road::east) == true);
    CHECK(inter.roadIsExpected(Road::west) == true);
    CHECK(inter.roadIsExpected(Road::south) == false);
    
    CHECK(inter.getRoad(Road::north) != NULL);
}

TEST_CASE("TC_7-2_INT_addRoad_alreadyExists"){
    Intersection inter = Intersection();
    int retVal;

    /// Good first road
    retVal = inter.addRoad(Road::north, {3, 4, 5});
    CHECK(retVal == Intersection::success);

    /// Try the same road, should error
    retVal = inter.addRoad(Road::north, {3, 4, 5});
    CHECK(retVal == Intersection::alreadyExists);
}

TEST_CASE("TC_7-3_INT_addRoad_turnNotPossible"){
    Intersection inter = Intersection();
    int retVal;

    /// Good north road
    retVal = inter.addRoad(Road::north, {0, 3, 0});
    CHECK(retVal == Intersection::success);
    
    /// Good south road
    retVal = inter.addRoad(Road::south, {0, 4, 0});
    CHECK(retVal == Intersection::success);

    /// Try a road with too many right turns to North
    retVal = inter.addRoad(Road::west, {0, 0, 4});
    CHECK(retVal == Intersection::turnNotPossible);
    CHECK(inter.roadIsExpected(Road::west) == false);
    
    /// Try a road with too many left turns to North
    retVal = inter.addRoad(Road::east, {4, 0, 0});
    CHECK(retVal == Intersection::turnNotPossible);
    CHECK(inter.roadIsExpected(Road::east) == false);
    
    /// Try a road with too many left turns to South
    retVal = inter.addRoad(Road::west, {5, 0, 0});
    CHECK(retVal == Intersection::turnNotPossible);
    CHECK(inter.roadIsExpected(Road::west) == false);
    
    /// Try a road with too many left turns to North
    retVal = inter.addRoad(Road::east, {0, 0, 5});
    CHECK(retVal == Intersection::turnNotPossible);
    CHECK(inter.roadIsExpected(Road::east) == false);
}

TEST_CASE("TC_7-4_INT_addRoad_4way"){
    Intersection inter = Intersection();
    int retVal;

    /// Good first road
    retVal = inter.addRoad(Road::north, {3, 4, 5});
    CHECK(retVal == Intersection::success);
    CHECK(inter.roadExists(Road::north) == true);
    CHECK(inter.getRoad(Road::north) != NULL);
    
    CHECK(inter.roadIsExpected(Road::north) == false);
    CHECK(inter.roadIsExpected(Road::east) == true);
    CHECK(inter.roadIsExpected(Road::west) == true);
    CHECK(inter.roadIsExpected(Road::south) == false);
    
    /// Good second road
    retVal = inter.addRoad(Road::east, {0, 1, 0});
    CHECK(retVal == Intersection::success);
    CHECK(inter.roadExists(Road::east) == true);
    CHECK(inter.getRoad(Road::east) != NULL);
    
    CHECK(inter.roadIsExpected(Road::north) == false);
    CHECK(inter.roadIsExpected(Road::east) == false);
    CHECK(inter.roadIsExpected(Road::west) == true);
    CHECK(inter.roadIsExpected(Road::south) == false);
    
    /// Good third road
    retVal = inter.addRoad(Road::west, {2, 3, 1});
    CHECK(retVal == Intersection::success);
    CHECK(inter.roadExists(Road::west) == true);
    CHECK(inter.getRoad(Road::west) != NULL);
    
    CHECK(inter.roadIsExpected(Road::north) == false);
    CHECK(inter.roadIsExpected(Road::east) == false);
    CHECK(inter.roadIsExpected(Road::west) == false);
    CHECK(inter.roadIsExpected(Road::south) == true);

    /// Good fourth road
    retVal = inter.addRoad(Road::south, {1, 2, 3});
    CHECK(retVal == Intersection::success);
    CHECK(inter.roadExists(Road::south) == true);
    CHECK(inter.getRoad(Road::south) != NULL);
    
    CHECK(inter.roadIsExpected(Road::north) == false);
    CHECK(inter.roadIsExpected(Road::east) == false);
    CHECK(inter.roadIsExpected(Road::west) == false);
    CHECK(inter.roadIsExpected(Road::south) == false);
}

TEST_CASE("TC_8-1_RD"){
    int retVal;
    Road rd = Road(Road::north, {1,2,3}, DEFAULT_ON_DURATION);

    retVal = rd.getNumLanes(TurnOption::left);
    CHECK(retVal == 1);
    retVal = rd.getNumLanes(TurnOption::straight);
    CHECK(retVal == 2);
    retVal = rd.getNumLanes(TurnOption::right);
    CHECK(retVal == 3);

    CHECK(rd.getLight(TurnOption::left) != NULL);
    CHECK(rd.getLight(TurnOption::straight) != NULL);
    CHECK(rd.getLight(TurnOption::right) != NULL);
}

TEST_CASE("TC_9-1_RD_setGreen"){
    int retVal;
    Road rd = Road(Road::north, {1,2,3}, DEFAULT_ON_DURATION);

    TrafficLight *left = rd.getLight(TurnOption::left);
    TrafficLight *straight = rd.getLight(TurnOption::straight);
    TrafficLight *right = rd.getLight(TurnOption::right);

    CHECK(left->getColor() == TrafficLight::red);
    CHECK(straight->getColor() == TrafficLight::red);
    CHECK(right->getColor() == TrafficLight::red);

    retVal = rd.setGreen(1);
    CHECK(retVal == 2);
    
    CHECK(left->getColor() == TrafficLight::red);
    CHECK(straight->getColor() == TrafficLight::green);
    CHECK(right->getColor() == TrafficLight::greenRight);
}

TEST_CASE("TC_9-2_RD_setGreen_false"){
    bool retVal;
    Road rd = Road(Road::north, {0,0,0}, DEFAULT_ON_DURATION);

    retVal = rd.setGreen(1);
    CHECK(retVal == false);
}

TEST_CASE("TC_10-1_INT_singleGreen"){
    bool retVal;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    //CHECK(inter.validate(std::cout) == true);

    CHECK(inter.getLight(Road::north, TurnOption::left)->getColor() == TrafficLight::red);
    CHECK(inter.getLight(Road::north, TurnOption::straight)->getColor() == TrafficLight::red);
    CHECK(inter.getLight(Road::north, TurnOption::right)->getColor() == TrafficLight::red);

    retVal = inter.singleGreen(Road::north, 4);
    CHECK(retVal == true);

    CHECK(inter.getLight(Road::north, TurnOption::left)->getColor() == TrafficLight::greenLeft);
    CHECK(inter.getLight(Road::north, TurnOption::straight)->getColor() == TrafficLight::green);
    CHECK(inter.getLight(Road::north, TurnOption::right)->getColor() == TrafficLight::greenRight);

    CHECK(inter.getLight(Road::north, TurnOption::left)->getDurationRemaining() == 4);
    CHECK(inter.getLight(Road::north, TurnOption::straight)->getDurationRemaining() == 4);
    CHECK(inter.getLight(Road::north, TurnOption::right)->getDurationRemaining() == 4);
}

TEST_CASE("TC_10-2_INT_singleGreen_missingTurns"){
    bool retVal;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    //CHECK(inter.validate(std::cout) == true);

    CHECK(inter.getLight(Road::east, TurnOption::left) == NULL);
    CHECK(inter.getLight(Road::east, TurnOption::straight)->getColor() == TrafficLight::red);
    CHECK(inter.getLight(Road::east, TurnOption::right) == NULL);

    retVal = inter.singleGreen(Road::east, 1);
    CHECK(retVal == true);

    CHECK(inter.getLight(Road::east, TurnOption::straight)->getColor() == TrafficLight::green);
}

TEST_CASE("TC_10-3_INT_singleGreen_missingRoad"){
    bool retVal;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    // No East Road.
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    //CHECK(inter.validate(std::cout) == true);

    retVal = inter.singleGreen(Road::east, 1);
    CHECK(retVal == false);
}

TEST_CASE("TC_11-1_INT_doubleGreen"){
    bool retVal;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // North
    TrafficLight *left1 = inter.getLight(Road::north, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::north, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::north, TurnOption::right);
    
    // South
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);
    TrafficLight *straight2 = inter.getLight(Road::south, TurnOption::straight);
    TrafficLight *right2 = inter.getLight(Road::south, TurnOption::right);

    //CHECK(inter.validate(std::cout) == true);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::red);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);

    retVal = inter.doubleGreen(Road::north, 1);
    CHECK(retVal == true);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::green);
    CHECK(right1->getColor() == TrafficLight::greenRight);
    CHECK(left2->getColor() == TrafficLight::red);
    CHECK(straight2->getColor() == TrafficLight::green);
    CHECK(right2->getColor() == TrafficLight::greenRight);
}

TEST_CASE("TC_11-2_INT_doubleGreen_missingOppositeRoad"){
    bool retVal;
    Intersection inter = Intersection();

    // Missing North
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // North
    TrafficLight *left1 = inter.getLight(Road::south, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::south, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::south, TurnOption::right);
    
    retVal = inter.doubleGreen(Road::south, 1);
    CHECK(retVal == false);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
}

TEST_CASE("TC_12-1_INT_doubleGreenLeft"){
    bool retVal;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // North
    TrafficLight *left1 = inter.getLight(Road::north, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::north, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::north, TurnOption::right);
    
    // South
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);
    TrafficLight *straight2 = inter.getLight(Road::south, TurnOption::straight);
    TrafficLight *right2 = inter.getLight(Road::south, TurnOption::right);

    //CHECK(inter.validate(std::cout) == true);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::red);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);

    retVal = inter.doubleGreenLeft(Road::north, 1);
    CHECK(retVal == true);

    CHECK(left1->getColor() == TrafficLight::greenLeft);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::greenLeft);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);
}

TEST_CASE("TC_13-1_INT_tick"){
    bool retVal;
    int numUnfinishedLights;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // North
    TrafficLight *left1 = inter.getLight(Road::north, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::north, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::north, TurnOption::right);
    
    // South
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);
    TrafficLight *straight2 = inter.getLight(Road::south, TurnOption::straight);
    TrafficLight *right2 = inter.getLight(Road::south, TurnOption::right);

    //CHECK(inter.validate(std::cout) == true);

    retVal = inter.doubleGreenLeft(Road::north, 2);
    CHECK(retVal == true);

    CHECK(left1->getColor() == TrafficLight::greenLeft);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::greenLeft);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);

    CHECK(left1->getDurationRemaining() == 2);
    CHECK(straight1->getDurationRemaining() == -1);
    CHECK(right1->getDurationRemaining() == -1);
    CHECK(left2->getDurationRemaining() == 2);
    CHECK(straight2->getDurationRemaining() == -1);
    CHECK(right2->getDurationRemaining() == -1);

    numUnfinishedLights = inter.tick();

    CHECK(numUnfinishedLights == 2);

    CHECK(left1->getDurationRemaining() == 1);
    CHECK(straight1->getDurationRemaining() == -1);
    CHECK(right1->getDurationRemaining() == -1);
    CHECK(left2->getDurationRemaining() == 1);
    CHECK(straight2->getDurationRemaining() == -1);
    CHECK(right2->getDurationRemaining() == -1);

    numUnfinishedLights = inter.tick();

    CHECK(numUnfinishedLights == 2);

    // durationRemaining hits 0, should set to yellow
    CHECK(left1->getColor() == TrafficLight::yellow);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::yellow);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);

    CHECK(left1->getDurationRemaining() == left1->yellowDuration);
    CHECK(straight1->getDurationRemaining() == -1);
    CHECK(right1->getDurationRemaining() == -1);
    CHECK(left2->getDurationRemaining() == left2->yellowDuration);
    CHECK(straight2->getDurationRemaining() == -1);
    CHECK(right2->getDurationRemaining() == -1);

    for(int i=0; i<left1->yellowDuration; i++){
        numUnfinishedLights = inter.tick();
    }

    CHECK(numUnfinishedLights == 0);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);
    CHECK(left2->getColor() == TrafficLight::red);
    CHECK(straight2->getColor() == TrafficLight::red);
    CHECK(right2->getColor() == TrafficLight::red);

    CHECK(left1->getDurationRemaining() == -1);
    CHECK(straight1->getDurationRemaining() == -1);
    CHECK(right1->getDurationRemaining() == -1);
    CHECK(left2->getDurationRemaining() == -1);
    CHECK(straight2->getDurationRemaining() == -1);
    CHECK(right2->getDurationRemaining() == -1);
}

TEST_CASE("TC_13-2_INT_tick_longerDuration"){
    bool retVal;
    int numUnfinishedLights;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // West
    TrafficLight *left1 = inter.getLight(Road::west, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::west, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::west, TurnOption::right);
    
    int onDuration = 10;
    int yellowDuration = 4;

    left1->setDuration(TrafficLight::yellow, yellowDuration);
    straight1->setDuration(TrafficLight::yellow, yellowDuration);
    right1->setDuration(TrafficLight::yellow, yellowDuration);

    retVal = inter.singleGreen(Road::west, onDuration);
    CHECK(retVal == true);

    CHECK(left1->getColor() == TrafficLight::greenLeft);
    CHECK(straight1->getColor() == TrafficLight::green);
    CHECK(right1->getColor() == TrafficLight::greenRight);

    CHECK(left1->getDurationRemaining() == onDuration);
    CHECK(straight1->getDurationRemaining() == onDuration);
    CHECK(right1->getDurationRemaining() == onDuration);

    numUnfinishedLights = inter.getNumUnfinishedLights();
    CHECK(numUnfinishedLights == 3);

    for(int i=0; i<onDuration; i++){
        numUnfinishedLights = inter.tick();
    }

    CHECK(numUnfinishedLights == 3);

    // durationRemaining hits 0, should set to yellow
    CHECK(left1->getColor() == TrafficLight::yellow);
    CHECK(straight1->getColor() == TrafficLight::yellow);
    CHECK(right1->getColor() == TrafficLight::yellow);

    CHECK(left1->getDurationRemaining() == yellowDuration);
    CHECK(straight1->getDurationRemaining() == yellowDuration);
    CHECK(right1->getDurationRemaining() == yellowDuration);

    for(int i=0; i<yellowDuration; i++){
        CHECK(left1->getColor() == TrafficLight::yellow);
        CHECK(straight1->getColor() == TrafficLight::yellow);
        CHECK(right1->getColor() == TrafficLight::yellow);

        numUnfinishedLights = inter.tick();
    }

    CHECK(numUnfinishedLights == 0);

    CHECK(left1->getColor() == TrafficLight::red);
    CHECK(straight1->getColor() == TrafficLight::red);
    CHECK(right1->getColor() == TrafficLight::red);

    CHECK(left1->getDurationRemaining() == -1);
    CHECK(straight1->getDurationRemaining() == -1);
    CHECK(right1->getDurationRemaining() == -1);
}

TEST_CASE("TC_13-3_INT_tick_addVehicles"){
    int numUnfinishedLights;
    TurnOption* turnOptNorth;
    TurnOption* turnOptSouth;
    Road* tempRd;
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    tempRd = inter.getRoad(Road::north);
    turnOptNorth = tempRd->getTurnOption(TurnOption::left);
    tempRd = inter.getRoad(Road::south);
    turnOptSouth = tempRd->getTurnOption(TurnOption::left);

    /// Add vehicles
    inter.addVehicles(Road::north, TurnOption::left, 8);
    inter.addVehicles(Road::south, TurnOption::left, 2);

    inter.doubleGreenLeft(Road::north, 2);

    numUnfinishedLights = inter.tick();
    CHECK(numUnfinishedLights == 2);
    CHECK(turnOptNorth->getCurrentVehicleProgress() == DEFAULT_TIME_TO_CROSS);
    CHECK(turnOptNorth->getQueuedVehicles() == 5);
    CHECK(turnOptSouth->getCurrentVehicleProgress() == DEFAULT_TIME_TO_CROSS);
    CHECK(turnOptSouth->getQueuedVehicles() == 1);

    numUnfinishedLights = inter.tick();
    CHECK(numUnfinishedLights == 2);
    CHECK(turnOptNorth->getCurrentVehicleProgress() == DEFAULT_TIME_TO_CROSS - 1);
    CHECK(turnOptNorth->getQueuedVehicles() == 5);
    CHECK(turnOptSouth->getCurrentVehicleProgress() == DEFAULT_TIME_TO_CROSS - 1);
    CHECK(turnOptSouth->getQueuedVehicles() == 1);

    // durationRemaining hits 0, should set to yellow
    for(int i=0; i<DEFAULT_YELLOW_DURATION; i++){
        numUnfinishedLights = inter.tick();
    }

    CHECK(turnOptNorth->getCurrentVehicleProgress() == 0);
    CHECK(turnOptNorth->getQueuedVehicles() == 5);
    CHECK(turnOptSouth->getCurrentVehicleProgress() == 0);
    CHECK(turnOptSouth->getQueuedVehicles() == 1);

    CHECK(inter.getLight(Road::north, TurnOption::left)->getNumVehiclesDirected() == 3);
    CHECK(inter.getLight(Road::south, TurnOption::left)->getNumVehiclesDirected() == 1);
}

TEST_CASE("TC_14-1_INT_schedule_LightConfig"){
    int onDuration = 10;
    int yellowDuration = 4;

    LightConfig config = LightConfig(LightConfig::doubleGreen, Road::north, onDuration);
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // North (Road 1)
    TrafficLight *left1 = inter.getLight(Road::north, TurnOption::left);
    TrafficLight *straight1 = inter.getLight(Road::north, TurnOption::straight);
    TrafficLight *right1 = inter.getLight(Road::north, TurnOption::right);
    
    // South (Road 2)
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);

    // Set the yellowDuration to be different for road1
    left1->setDuration(TrafficLight::yellow, yellowDuration);
    straight1->setDuration(TrafficLight::yellow, yellowDuration);
    right1->setDuration(TrafficLight::yellow, yellowDuration);

    inter.schedule(config);
    inter.start();

    CHECK(inter.getNumUnfinishedLights() == 4);

    for(int i=0; i<onDuration; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 4);

    // The yellowDuration for road2 is shorter than road1. So road2 lights will finish before road1.
    for(int i=0; i<left2->yellowDuration; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<(left1->getColorDuration(TrafficLight::yellow) - left2->yellowDuration); i++){
        inter.tick();
    }
    
    CHECK(inter.getNumUnfinishedLights() == 0);
}

TEST_CASE("TC_14-2_INT_schedule_doubleGreenLeft"){
    int onDuration = 10;
    int yellowDuration = 4;

    LightConfig config = LightConfig(LightConfig::doubleGreenLeft, Road::east, onDuration);
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {1, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // East (Road 1)
    TrafficLight *left1 = inter.getLight(Road::east, TurnOption::left);
    
    // West (Road 2)
    TrafficLight *left2 = inter.getLight(Road::west, TurnOption::left);

    // Set the yellowDuration to be different for road1
    left1->setDuration(TrafficLight::yellow, yellowDuration);

    inter.schedule(config);
    inter.start();

    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<onDuration; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 2);

    // The yellowDuration for road2 is shorter than road1. So road2 lights will finish before road1.
    for(int i=0; i<left2->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 2);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 1);

    for(int i=0; i<(left1->getColorDuration(TrafficLight::yellow) - left2->yellowDuration); i++){
        CHECK(inter.getNumUnfinishedLights() == 1);
        inter.tick();
    }
    
    CHECK(inter.getNumUnfinishedLights() == 0);
}

TEST_CASE("TC_14-3_INT_schedule_nextLightConfig"){
    int onDuration1 = 10;
    int onDuration2 = 5;

    LightConfig config = LightConfig(LightConfig::doubleGreenLeft, Road::east, onDuration1);
    LightConfig config2 = LightConfig(LightConfig::singleGreen, Road::south, onDuration2);
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {1, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // East (Road 1)
    TrafficLight *left1 = inter.getLight(Road::east, TurnOption::left);
    
    // South (Road 2)
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);

    inter.schedule(config);
    inter.schedule(config2);
    inter.start();

    // The first LightConfig: East left and West left are on
    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<onDuration1; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<left1->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 2);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 0);

    // Go to next light config
    inter.nextLightConfig();

    // The second LightConfig: South left, straight, and right are on.
    CHECK(inter.getNumUnfinishedLights() == 3);

    for(int i=0; i<onDuration2; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 3);

    for(int i=0; i<left2->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 3);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 0);
}

TEST_CASE("TC_14-3_INT_schedule_nextLightConfig_loop"){
    int onDuration1 = 10;
    int onDuration2 = 5;

    LightConfig config = LightConfig(LightConfig::doubleGreenLeft, Road::east, onDuration1);
    LightConfig config2 = LightConfig(LightConfig::singleGreen, Road::south, onDuration2);
    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {1, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    // East (Road 1)
    TrafficLight *left1 = inter.getLight(Road::east, TurnOption::left);
    
    // South (Road 2)
    TrafficLight *left2 = inter.getLight(Road::south, TurnOption::left);

    inter.schedule(config);
    inter.schedule(config2);
    inter.start();

    // The first LightConfig: East left and West left are on
    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<onDuration1; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<left1->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 2);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 0);

    // Go to next light config
    inter.nextLightConfig();

    // The second LightConfig: South left, straight, and right are on.
    CHECK(inter.getNumUnfinishedLights() == 3);

    for(int i=0; i<onDuration2; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 3);

    for(int i=0; i<left2->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 3);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 0);
    
    // Loop back to first light config
    inter.nextLightConfig();
    
    // Loop back to the first LightConfig: East left and West left are on
    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<onDuration1; i++){
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 2);

    for(int i=0; i<left1->yellowDuration; i++){
        CHECK(inter.getNumUnfinishedLights() == 2);
        inter.tick();
    }

    CHECK(inter.getNumUnfinishedLights() == 0);
}

TEST_CASE("TC_15-1_INT_addVehicles"){
    bool retVal;
    TurnOption* turnOpt;

    Intersection inter = Intersection();

    inter.addRoad(Road::north, {3, 4, 5});
    inter.addRoad(Road::east, {0, 1, 0});
    inter.addRoad(Road::west, {2, 3, 1});
    inter.addRoad(Road::south, {1, 2, 3});

    /// This lane should support 20 vehicles
    turnOpt = inter.getRoad(Road::north)->getTurnOption(TurnOption::straight);

    retVal = inter.addVehicles(Road::north, TurnOption::straight, 5);
    CHECK(retVal == true);
    CHECK(turnOpt->getQueuedVehicles() == 5);

    retVal = inter.addVehicles(Road::north, TurnOption::straight, 15);
    CHECK(retVal == true);
    CHECK(turnOpt->getQueuedVehicles() == 20);

    /// Max exceeeded
    retVal = inter.addVehicles(Road::north, TurnOption::straight, 1);
    CHECK(retVal == false);
    CHECK(turnOpt->getQueuedVehicles() == 20);

    /// Road DNE
    retVal = inter.addVehicles(Road::east, TurnOption::right, 1);
    CHECK(retVal == false);

    /// numVehiclesToAdd <= 0
    retVal = inter.addVehicles(Road::south, TurnOption::straight, -1);
    CHECK(retVal == false);
}
