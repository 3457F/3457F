#include "main.h"
#include "mogo.hpp"
#include "util.hpp"
#include "consts.hpp"

#include <functional>
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

Timer::Timer() {}

void Timer::start() {
    start_time = pros::millis();
}

uint32_t Timer::getElapsedTime() {
    return pros::millis() - start_time;
}

uint32_t Timer::getElapsedTimeSecs() {
    return getElapsedTime() / 1000;
}

// ---

uint32_t timer(std::function<void()> func) {
    Timer timer;
    timer.start();

    func();

    return timer.getElapsedTime();
}

/**
 * A pros::Task that DISABLES color sort one minute into driver control!
 */
void disable_color_sort(void* color_sorting) {
    Timer timer;
    timer.start();

    // waits for one minute of driver control
    while (timer.getElapsedTimeSecs() < (DISABLE_COLOR_SORT_AFTER)) {
        pros::delay(20); 
    }

    // THEN disables color sorting!
    *static_cast<bool*>(color_sorting) = false;
    printf("DISABLED color sort one minute into driver control!\n");
}

/**
 * A pros::Task that UNCLAMPS the mogo at the end of the match (1:45 into
 * driver control)
 */
void unclamp_mogo(void* mogoVoid) {
    MogoMech* mogo = static_cast<MogoMech*>(mogoVoid);

    Timer timer;
    timer.start();

    // 60 + 45 represents the 1:45 duration of driver control!
    while (timer.getElapsedTimeSecs() < (UNCLAMP_MOGO_AFTER)) {
        pros::delay(20);
    }

    mogo->release();
    printf("RELEASED mogo!\n");
}

/**
 * for autons
 */

void turnAndMoveToPoint(
    float x
    , float y
    , tamtpParams params
) {
    waitd;

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
                , .maxSpeed = params.mvMaxSpeed, .minSpeed = params.mvMinSpeed
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

/**
 * DON'T USE -- FOR SOME REASON IT DISCONNECTS CODE WHENEVER I RUN WITH THIS TASK ENABLED */
void opcontrolTask(void* chassisVoid) {
	lemlib::Chassis* chassis = static_cast<lemlib::Chassis*>(chassisVoid);

	while (true) {
		// debug
		lemlib::Pose curr_pose = chassis->getPose();
		printf("chassis x: %f chassis y: %f, chassis theta: %f\n", curr_pose.x, curr_pose.y, curr_pose.theta);

		pros::delay(1000);
	}
}