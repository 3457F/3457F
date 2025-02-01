#include "main.h"
#include "util.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "pros/rtos.hpp"

#include <chrono>
#include <set>

// import pure pursuit files
ASSET(mecha_red_pos_awp_corner_txt);

void print_robot_pos(void* chassisVoid) {
    lemlib::Chassis* chassis
        = static_cast<
            lemlib::Chassis*
        >(chassisVoid);

    while (true) {
        lemlib::Pose pos = chassis->getPose();

        std::cout << "x: " << pos.x
				  << " | y: " << pos.y
				  << " | theta: " << pos.theta
				  << std::endl;

        pros::delay(20);
    } 
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * NEW AUTONSSSSS
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

void red_positive_awp_nostack() {
    chassis.setPose(
        -58.386
        , -24
        , 90
    );

    // TODO: make a pure pursuit path
    // that slows down just at the end, to
    // save time
    // goes to mogo
    chassis.moveToPoint(
        // -30.323
        -28.69
        // , -23.647
        , -24
        , 1500
        , {
            .maxSpeed = 70
        }
    );
    chassis.waitUntil(25.5);
    mogo.clamp();
    waitd;

    // turns intake towards the first stack
    chassis.turnToPoint(
        -21.849
        , -48.6
        , 1000
        , {
            .forwards = false
            , .direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
        }
    );
    waitd;

    // to intake preload + ring stack!
    intake.intake();

    // goes to ring stack
    chassis.moveToPoint(
        -24.395
        , -48.771
        , 1500
        , {
            .forwards = false
        }
    );
    waitd;
    // waits for ring to be scored
    pros::delay(500);

    // brake so that next blue ring don't get in
    intake.brake();

    intake.lift(1);

    // TODO: make pure pursuit path for this
    // go to corner stack
    chassis.follow(
        mecha_red_pos_awp_corner_txt
        , 5
        , 2000
        , false
    );
     arm.set_pos(arm.LOADIN_POS);
     intake.intake();
    
}

void red_negative_five_ring(){

    chassis.setPose(-54.319, 31.127, 270);

    turnAndMoveToPoint(-10.177, 42.041, { .moveTO = 1300, .forwards = false});
    intake.intake(); 
    waitd;
    intake.is_ring_on_top(); 
    intake.brake();

    chassis.moveToPose(-17.875, 27.132, 56.5, 1950, {.forwards = true});
    waitd;
    mogo.toggle();

    chassis.moveToPose(-17.875, 27.132, 56.5, 1200, {.forwards = false}); 
    pros::delay(650);
    intake.intake(); 
    waitd;

    chassis.moveToPose(-48.959, 24.111, 54, 1500, {.forwards = false});
    waitd; 
    
    chassis.moveToPose(-47.595, 6.961, 355, 1500, {.forwards = false});
    intake.lift(true);
    waitd;

    intake.lift(false);
    intake.is_ring_on_top(); 
    pros::delay(800);
    intake.brake();

}

void move_forward() {
    chassis.setPose(
        0
        , 0
        , 90
    );

    chassis.moveToPoint(
        24
        , 0
        , 1000
    );
}