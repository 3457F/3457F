#include "main.h"
#include "util.hpp"
#include <cmath>


bool within(double num, double expected_val, double range) {
    double diff = std::abs(num - expected_val);

    return diff < range;
}

bool within_range(double num, double min, double max) {
    return num >= min && num <= max;
}

/**
 * driver control
 */

// driving functions
void tank() {
	left_motors.move((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0) * DRIVE_SPEED);
	right_motors.move((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0) * DRIVE_SPEED);
}

void arcade() {
	int move = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

	left_motors.move(((move + turn) / 127.0) * DRIVE_SPEED);
	right_motors.move(((move - turn) / 127.0) * DRIVE_SPEED);
}

/**
 * for autons
 */

void turnAndMoveToPoint(
    float x
    , float y
    , tamtpParams params
) {
    pros::delay(1000);

    chassis.turnToPoint(
        x
        , y
        , params.turnTO
        , { 
            // .forwards = params.turnOpp ? !params.forwards : params.forwards
            .forwards = params.forwards
            , .direction = params.turnDir
        }
        , params.async
    );
    waitd;

    lemlib::Pose pose = chassis.getPose();

    if (params.move_to_pose) {
        chassis.moveToPose(
            x
            , y
            , pose.theta
            , params.moveTO
            , {
                .forwards = params.forwards
                , .maxSpeed = params.mvMaxSpeed
            }
            , params.async
        );
    } else {
        float moveX = params.usePoseX ? pose.x : x;
        float moveY = params.usePoseY ? pose.y : y;

        chassis.moveToPoint(
            x
            , y
            , params.moveTO
            , {
                .forwards = params.forwards
                , .maxSpeed = params.mvMaxSpeed
                , .earlyExitRange = params.mvEarlyExitRange
            }
            , params.async
        );

        if (params.waitUntil != 0.0) {
            chassis.waitUntil(params.waitUntil);
        }
    }

    if (!params.async) {
        waitd;
    }
}