#pragma once

#include "api.h"
#include "lemlib/api.hpp"

#include "intake.hpp"

class Arm;

struct LoadInInfo {
    Intake* intake;
    Arm* arm;
};

#include "arm.hpp"

struct FetchInfo {
    lemlib::PID* pid;
    Arm* arm;
    float* target;
    pros::Rotation* encoder;
};

struct SetInfo {
    lemlib::PID* pid;
    float error;
    std::int32_t curr_angle;
    float* target;
};

bool within(double num, double expected_val, double range);

/**
 * for autons
 */
// default timeout for when im lazy to specify
#define TO 1200

#define TUNING_TO 2000
#define waitd chassis.waitUntilDone()
#define MAX_SPEED 127.0

struct tamtpParams {
    int turnTO = TO;

    /** what direction to turn in, defaults to AUTO */
    lemlib::AngularDirection turnDir = lemlib::AngularDirection::AUTO;
    /* whether to TURN opposite the point */
    // bool turnOpp = false;

    int moveTO = TO;

    /** remember, forwards is the mogo side! */
    bool forwards = true;

    /** if true, MAKE SURE to run `waitd;` after whatever call you make,
    such as a `chassis.waitUntil()`, otherwise all the actions will
    snowball on each other and the robot won't do much of anything ;-; */
    bool async = false;

    float mvMaxSpeed = MAX_SPEED;

    /** runs `chassis.moveToPose()` instead of `chassis.moveToPoint()`
    after turning to the point. it was more of a test on my side to see
    why the robot kept turning arbitrarily right before it reached the point  */
    bool move_to_pose = false; 

    // adds in early exit range for move
    float mvEarlyExitRange = 0.0;

    // optional waituntil -- REQUIRES `async` to be true
    float waitUntil = 0.0;

    // use pose x after turn
    bool usePoseX = false;
    // use pose y after turn
    bool usePoseY = false;
};

void turnAndMoveToPoint(
    float x
    , float y
    , tamtpParams params = {}
);