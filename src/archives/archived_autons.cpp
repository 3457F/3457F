#include "main.h"
#include "util.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "pros/rtos.hpp"

#include <chrono>

/**
 * ------------------------------------------------------------
 * RED NEGATIVE
 * ------------------------------------------------------------
 */

void red_negative() {
    // starts at the center of the intersection of the top two tiles, the mogo mech facing directly rightward
    chassis.setPose(
        -58.6
        , 47
        , 90
    );

    // turns and moves towards the first mogo, clamping it
    turnAndMoveToPoint(
        -24.5
        , 21.25
        , {
            .turnTO = 500
            , .moveTO = 1500
            , .async = true
        }
    );
    chassis.waitUntil(29.5);
    mogo.toggle();
    waitd;

    // intakes the preload ring
    intake.intake();

    // immediately starts moving 
    turnAndMoveToPoint(
        -12
        , 34.75
        , {
            .turnTO = 600
            , .moveTO = 750
            , .forwards = false
            , .mvMaxSpeed = 75
        }
    );

    pros::delay(350);

    chassis.moveToPoint(
        -22.65
        , 22
        , TO
    );
    waitd;
    turnAndMoveToPoint(
        -22.65
        , 44
        , {
            .turnTO = 650
            , .forwards = false
        }
    );
    waitd;

    pros::delay(400);    
    intake.brake();
    turnAndMoveToPoint(
        -11.55
        , 49.25
        , {
            .turnTO = 500
            , .moveTO = 800
            , .forwards = false
        }
    );
    waitd;

    pros::delay(250);

    chassis.moveToPoint(
        -40
        , 49.25
        , TO
    );
}

void red_negative_5_ring() {
    red_negative();
    waitd;

    pros::delay(25);
    chassis.turnToHeading(
        135
        , TO
    );
    waitd;

    chassis.moveToPose(
        -70
        , 70
        , 135
        , 1350
        , {
            .forwards = false
            , .maxSpeed = 70
        }
    );
    pros::delay(50);
    intake.brake();
    chassis.waitUntil(7.5);
    intake.outtake();
    waitd;

    pros::delay(250);

    intake.intake();

    pros::delay(500);

    chassis.turnToHeading(
        90
        , TO
    );
    waitd;

    chassis.moveToPoint(
        -65
        , 57.5
        , TO
        , {
            .forwards = false
        }
    );
    waitd;

    chassis.moveToPoint(
        -33.968
        , 23.456
        , TO
    );
    waitd;
}

/**
 * NOT USING: SEE RED NEG AWP REDO */
void red_negative_sawp() {
    red_negative();

    chassis.waitUntil(15);
    intake.brake();
    waitd;

    chassis.setPose(
        -40
        , 49.25
        , chassis.getPose().theta
    );

    chassis.turnToPoint(
        -50.25
        , 11.648
        , 650
    );
    waitd;
    chassis.moveToPose(
        -50.25
        , -30
        , 180
        , 2650
        , {
            .minSpeed = 75
        }
    );

    chassis.waitUntil(30);
    mogo.toggle();
    waitd;

    double offset = -11.5;
    chassis.moveToPoint(
        -50
        , offset
        , 1000
        , {
            .forwards = false
        }
    );
    waitd;

    lemlib::Pose pos = chassis.getPose();
    double a = 1;
    chassis.turnToPoint(
        -10000000000
        , pos.y
        , 650
    );
    waitd;
    pos = chassis.getPose();
    chassis.moveToPoint(
        pos.x-a
        , pos.y
        , TO
        , {
            .maxSpeed = 85
        }
    );
    waitd;

    intake.intake();

    pros::delay(750);

    intake.brake();
}

/**
 * ------------------------------------------------------------
 * Blue Negative
 * ------------------------------------------------------------
 */


void blue_negative() {
    // starts at the center of the intersection of the top two tiles, the mogo mech facing directly rightward
    chassis.setPose(
        58.6
        , 47
        , -90
    );

    // turns and moves towards the first mogo, clamping it
    turnAndMoveToPoint(
        21
        , 17
        , {
            .turnTO = 500
            , .moveTO = 1500
            , .async = true
            , .mvMaxSpeed = 80
        }
    );
    chassis.waitUntil(28.5);
    mogo.toggle();
    waitd;

    // intakes the preload ring

    // immediately starts moving 
    chassis.turnToPoint(
        17.5
        , 34
        , 600
        , {
            .forwards = false
        }
    );
    waitd;
    intake.intake();
    chassis.moveToPoint(
        17.5
        , 34
        , 750
        , {
            .forwards = false
            , .maxSpeed = 80
        }
    );
    waitd;

    pros::delay(350);

    chassis.moveToPoint(
        30
        , 27
        , TO
    );
    waitd;
    turnAndMoveToPoint(
        36
        , 44
        , {
            .turnTO = 650
            , .forwards = false
            , .mvMaxSpeed = 75
        }
    );
    waitd;

    pros::delay(400);    
    intake.brake();

    chassis.turnToPoint(
        28.75
        , 51.5
        , 750
        , { 
            .forwards = false
        }
    );
    waitd;
    intake.intake();
    chassis.moveToPoint(
        28.75
        , 51.5
        , 800
        , {
            .forwards = false
        }
    );
    waitd;

    pros::delay(500);

    chassis.moveToPoint(
        40
        , 49.25
        , TO
    );
}


void blue_negative_safe() {
    blue_negative();
    waitd;

    turnAndMoveToPoint(
        27
        , 10
    );
    arm.set_pos(arm.LOADIN_POS);
    intake.brake();
}

void blue_negative_5_ring() {
    blue_negative();

    pros::delay(25);
    chassis.turnToHeading(
        -135
        , TO
    );
    waitd;

    chassis.moveToPose(
        90
        , 70
        , -135
        , 1350
        , {
            .forwards = false
            , .maxSpeed = 80
        }
    );
    waitd;
    chassis.moveToPoint(
        110
        , 85
        , TO
        , {
            .forwards = false
            , .maxSpeed = 95
        }
    );
    intake.intake();
    waitd;
    chassis.swingToHeading(
        -90
        , DriveSide::LEFT
        , TO
    );
    waitd;
    chassis.swingToHeading(
        45
        , DriveSide::LEFT
        , 800
    );
    waitd;

    pros::delay(500);

    chassis.moveToPoint(
        65
        , 57.5
        , TO
        , {
            .forwards = false
        }
    );
    waitd;

    chassis.moveToPoint(
        33.968
        , 23.456
        , TO
    );
    waitd;
}

void blue_negative_sawp() {
    blue_negative();

    intake.brake();
    waitd;

    chassis.turnToPoint(
        54.211
        , 11.648
        , 650
    );
    waitd;
    chassis.moveToPose(
        54.211
        , 11.648
        , -180
        , TO
    );
    waitd;

    chassis.moveToPoint(
        54.211
        , -5
        , 750
        , {
            .maxSpeed = 75
        }
    );
    waitd;

    mogo.toggle();

    chassis.moveToPoint(
        54.211
        , 0
        , 250
    );
    waitd;

    chassis.turnToHeading(
        270
        , 650
    );
    waitd;

    lemlib::Pose pos = chassis.getPose();
    int a = 10;
    chassis.moveToPoint(
        pos.x + a
        , pos.y
        , TO
        , {
            .maxSpeed = 85
        }
    );
    waitd;

    intake.intake();

    pros::delay(750);

    intake.brake();

    chassis.moveToPoint(
        54.471
        , pos.y
        , TO
        , {
            .maxSpeed = 50
        }
    );
    waitd;
}

/**
 * Other Autons
 */

void red_cross_sawp() {
    // ASSUME PUSHING HAS ALR HAPPENED!
    chassis.setPose(
        -58.6
        , 47
        , 90
    );

    auto start = std::chrono::high_resolution_clock::now();

    // turns and moves towards mogo -> 42.6867
    turnAndMoveToPoint(
        -24
        , 22
        , {
            .turnTO = 1000
            , .moveTO = 1500
            , .async = true
        }
    );

    // clamps pre-emptively
    chassis.waitUntil(29.5);
    mogo.toggle();

    // waits for rest of movement to finish
    waitd;

    // intakes preload ring + waits
    intake.intake();
    // 500ms -> 400
    pros::delay(400);

    // comes back so can rotate 
    // TODO: verify this is correct...?
    turnAndMoveToPoint(
        -24
        , 48
        , {
            .turnTO = 1000
            , .moveTO = 1000
            , .forwards = false
        }
    );
    waitd;
    pros::delay(1000);

    // drops mogo + goes for second one
    // TODO: might need to stop intake here, in case accidentally intakes blue ring..?
    turnAndMoveToPoint(
        -45
        , -9.5
        , {
            .turnTO = 1500
            , .moveTO = 2000
            , .forwards = false
            , .async = true
        }
    );
    // drops mogo midway so we don't have to care about it
    chassis.waitUntil(40);
    // "drops" old mogo
    mogo.toggle();
    // waits for rest of movement
    waitd;

    turnAndMoveToPoint(
        -23.5
        , -18.5
        , {
            .turnTO = 2000
            , .moveTO = 2000
            , .async = true
        }
    );
    chassis.waitUntil(25);
    // "gets" new mogo + waits
    mogo.toggle();

    // waits for rest of movement to finish
    waitd;

    // turns + moves towards third ring on field
    turnAndMoveToPoint(
        -25.5
        , -47.25
        , {
            .turnTO = 1000
            , .moveTO = 1000
            , .forwards = false
        }
    );
    // waits for third ring to be intaked
    pros::delay(500);

    // going to ladder!
    chassis.moveToPoint(
        -19.5
        , -4
        , 2000
    );

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time elapsed: " 
              << elapsed.count()
              << "seconds." << std::endl;
}