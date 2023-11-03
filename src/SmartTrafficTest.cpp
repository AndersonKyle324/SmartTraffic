#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest/doctest.h"

#include "TrafficLight.h"

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
