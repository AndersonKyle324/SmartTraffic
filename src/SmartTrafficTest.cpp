#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest/doctest.h"

#include "TrafficLight.h"
#include "Intersection.h"

TEST_CASE("TC_1_TF_start"){
    TrafficLightLeft tf = TrafficLightLeft();

    tf.start();

    CHECK(tf.getColor() == TrafficLight::greenLeft);
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
    Road rd = Road(Road::north, {1,2,3});

    retVal = rd.getNumLanes(Road::left);
    CHECK(retVal == 1);
    retVal = rd.getNumLanes(Road::straight);
    CHECK(retVal == 2);
    retVal = rd.getNumLanes(Road::right);
    CHECK(retVal == 3);

    CHECK(rd.getLight(Road::left) != NULL);
    CHECK(rd.getLight(Road::straight) != NULL);
    CHECK(rd.getLight(Road::right) != NULL);
}

TEST_CASE("TC_8-1_RD"){
    int retVal;
    Road rd = Road(Road::north, {1,2,3});

    retVal = rd.getNumLanes(Road::left);
    CHECK(retVal == 1);
    retVal = rd.getNumLanes(Road::straight);
    CHECK(retVal == 2);
    retVal = rd.getNumLanes(Road::right);
    CHECK(retVal == 3);

    CHECK(rd.getLight(Road::left) != NULL);
    CHECK(rd.getLight(Road::straight) != NULL);
    CHECK(rd.getLight(Road::right) != NULL);
}