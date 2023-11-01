#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest/doctest.h"

#include "TrafficLight.h"

TEST_CASE("TC_1_TF_Start"){
    TrafficLightLeft tf = TrafficLightLeft();

    tf.start();

    CHECK(tf.getColor() == TrafficLight::greenLeft);
}

TEST_CASE("TC_4_TF_isValidColor"){
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
    int onColorDur = 1;
    int redDur = 2;
    TrafficLightLeft tf = TrafficLightLeft(onColorDur, redDur);

    CHECK(tf.getColor() == TrafficLight::red);
    CHECK(tf.getOnColor() == TrafficLight::greenLeft);
    CHECK(tf.getColorDuration(TrafficLight::greenLeft) == onColorDur);
    CHECK(tf.getColorDuration(TrafficLight::red) == redDur);
}