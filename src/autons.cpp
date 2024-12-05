#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "mogo.hpp"
#include "pros/imu.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <chrono>

// default timeout for when im lazy to specify
#define TO 1200

#define TUNING_TO 2000
#define waitd chassis.waitUntilDone()
#define MAX_SPEED 127.0

// import pure pursuit files
ASSET(red_neg_first_mogo_txt);

struct tamtpParams {
    int turnTO = TO;
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
};

void turnAndMoveToPoint(
    float x
    , float y
    , tamtpParams params = {}
) {
    chassis.turnToPoint(
        x
        , y
        , params.turnTO
        , { .forwards = params.forwards }
    );
    waitd;

    if (params.move_to_pose) {
        lemlib::Pose pose = chassis.getPose();
        
        chassis.moveToPose(
            x
            , y
            , pose.theta
            , params.moveTO
            , {
                .forwards = params.forwards
                , .maxSpeed = params.mvMaxSpeed
            }
        );
    } else {
        chassis.moveToPoint(
            x
            , y
            , params.moveTO
            , {
                .forwards = params.forwards
                , .maxSpeed = params.mvMaxSpeed
            }
        );
    }

    if (!params.async) {
        waitd;
    }
}

// void turnAndMoveToPoint(
//     float x
//     , float y
//     , int turnTO
//     , int mvTO
//     , bool fwd = true
//     , bool async = false
//     , float mvMaxSpeed = 127.0
//     , bool move_to_pose = false
// ) {
//     chassis.turnToPoint(
//         x
//         , y
//         , turnTO
//         , {
//             .forwards = fwd
//         }
//     );
//     waitd;

//     if (move_to_pose) {
//         lemlib::Pose pose = chassis.getPose();

//         chassis.moveToPose(
//             x
//             , y
//             , pose.theta
//             , mvTO
//             , {
//                 .forwards = fwd
//             }
//         );
//     } else {
//         chassis.moveToPoint(
//             x
//             , y
//             , mvTO
//             , {
//                 .forwards = fwd
//                 , .maxSpeed = mvMaxSpeed 
//             }
//         );
//     }


//     if (!async) {
//         waitd;
//     };
// }

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
    // turnAndMoveToPoint(
    //     -24.5
    //     , 21.25
    //     , 500
    //     , 1500
    //     , true
    //     , true
    // );
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
    // chassis.turnToPoint(
    //     -12
    //     , 34.75
    //     , 600
    //     , {
    //         .forwards = false
    //     }
    // );
    // waitd;
    // chassis.moveToPoint(
    //     -12
    //     , 34.75
    //     , 750
    //     , {
    //         .forwards = false
    //         , .maxSpeed = 75
    //     }
    // );
    // waitd;
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
    // turnAndMoveToPoint(
    //     -22.65
    //     , 44
    //     , 650
    //     , TO
    //     , false
    // );
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

    // chassis.turnToPoint(
    //     -11.55
    //     , 49.25
    //     , 500
    //     , {
    //         .forwards = false
    //     }
    // );
    // waitd;
    // intake.intake();
    // chassis.moveToPoint(
    //     -11.55
    //     , 49.25
    //     , 800
    //     , {
    //         .forwards = false
    //     }
    // );
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
 * NOT a redo of the awp alg; slightly diff
 * but IS a redo of red negative auton to account
 * for the fixed IMU (now points won't be drift offset!)
 */
void red_neg_awp_redo() {
    // arm.pid.kP

    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(-54.75, 16, 0);

    // starts outtaking so it pushes the blue ring away
    intake.outtake();

    // TODO: point TOO FAR UP
    // scores preload on alliance stake
    // -61.5, 5 -> correct pt but too far

    //Nov 30 fixed this issues above
    turnAndMoveToPoint(-61 , 5.5  , {.turnTO = 1000, .moveTO = 1000, .forwards = false });
    pros::delay(500);
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(750);
    // retracts, so as not to disturb ring when turning
    // arm.set_pos(arm.START_POS);
    // pros::delay(250);

    // goes back to old point
    chassis.moveToPose(
        -54.75, 16 , 30 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    // lifts intake in prep to grab first red ring on top
    intake.lift(1);
    // starts INTAKING, since we WANT to get the red ring
    intake.intake();
    // turns and moves towards first ring on field (on top of stack)
    turnAndMoveToPoint(-49.212, -1.908, {.turnTO = 750 , .moveTO = 1250, .forwards = false});
    intake.lift(false);
    pros::delay(400);

//grab mogo code (old)
    // chassis.moveToPose(-29.035, 23.595, 49  , 2000, {.maxSpeed = 127});
    // chassis.waitUntil(3.4);
    // intake.brake();
    // chassis.waitUntil(33);
    // mogo.toggle();
    // waitd;

    //grab mogo code (new) (Works as of nov 30)
    //moves back to have a better angle to the mogo
    chassis.moveToPose( -52.759 , 8.715 , 30, 1000);

    //Nov 30 have to tune the value since the ring keeps on flying or not pickin up ;-;
    chassis.waitUntil(2.3);
    intake.brake();
    waitd;

    //moves to the mogo
    chassis.moveToPose(-30.932, 20.603, 60, 1500, {.forwards = true, .maxSpeed = 90});
    //wait until is pretty tuned (nov 30)
    chassis.waitUntil(28);
    mogo.toggle();
    waitd;

    //moves to the middle rings
    chassis.moveToPose(-23.527, 57.616, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

    //this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    //moves to the top rings! (works nov 30)
    chassis.moveToPose(-9.3, 51.395, 280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;

     //Getting the arm ready to score
    arm.set_pos(arm.SCORE_POS);

     //moves to the ladder for wp 
    chassis.moveToPose(-17.616, 20.185, 315, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;
}

void red_neg_elims(){
     pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(-54.75, 16, 0);

    // starts outtaking so it pushes the blue ring away
    intake.outtake();

    // TODO: point TOO FAR UP
    // scores preload on alliance stake
    // -61.5, 5 -> correct pt but too far

    //Nov 30 fixed this issues above
    turnAndMoveToPoint(-61 , 5.5  , {.turnTO = 1000, .moveTO = 1000, .forwards = false });
    pros::delay(500);
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(750);
    // retracts, so as not to disturb ring when turning
    // arm.set_pos(arm.START_POS);
    // pros::delay(250);

    // goes back to old point
    chassis.moveToPose(
        -54.75, 16 , 30 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    // lifts intake in prep to grab first red ring on top
    intake.lift(1);
    // starts INTAKING, since we WANT to get the red ring
    intake.intake();
    // turns and moves towards first ring on field (on top of stack)
    turnAndMoveToPoint(-49.212, -1.908, {.turnTO = 750 , .moveTO = 1250, .forwards = false});
    intake.lift(false);
    pros::delay(400);

//grab mogo code (old)
    // chassis.moveToPose(-29.035, 23.595, 49  , 2000, {.maxSpeed = 127});
    // chassis.waitUntil(3.4);
    // intake.brake();
    // chassis.waitUntil(33);
    // mogo.toggle();
    // waitd;

    //grab mogo code (new) (Works as of nov 30)
    //moves back to have a better angle to the mogo
    chassis.moveToPose( -52.759 , 8.715 , 30, 1000);

    //Nov 30 have to tune the value since the ring keeps on flying or not pickin up ;-;
    chassis.waitUntil(2.3);
    intake.brake();
    waitd;

    //moves to the mogo
    chassis.moveToPose(-30.932, 20.603, 60, 1500, {.forwards = true, .maxSpeed = 90});
    //wait until is pretty tuned (nov 30)
    chassis.waitUntil(28);
    mogo.toggle();
    waitd;

    //moves to the middle rings
    chassis.moveToPose(-23.527, 57.616, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

    //this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    //moves to the top rings! (works nov 30)
    chassis.moveToPose(-9.3, 51.395, 280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
     waitd;
    
    //moves to the alliance stake to have a better shot for pos corner
    chassis.moveToPose(-38.143, 2.674, 0, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

    
}

/**
 * ------------------------------------------------------------
 * RED POSITIVE
 * ------------------------------------------------------------
 */

// THREE RING, TWO STAKE AUTON -> does awp!!!
void red_positive() {
    // starting position
    chassis.setPose(
        -55
        , -36.5
        , 90
    );

    // turns and moves towards first mogo, clamping it
    // turnAndMoveToPoint(
    //     -24
    //     , -22
    //     , 500
    //     , 2500
    //     , true
    //     , true
    //     , 67.5
    // );
    turnAndMoveToPoint(
        -24
        , -22
        , {
            .turnTO = 500
            , .moveTO = 2500
            , .async = true
            , .mvMaxSpeed = 67.5
        }
    );
    chassis.waitUntil(24);
    mogo.toggle();
    waitd;

    // intakes preload ring (first ring overall)
    intake.intake();
    pros::delay(800);

    // lets go of first mogo
    mogo.toggle();
    pros::delay(250);

    // TODO: sometimes ring flat out doesn't get inside
    // goes to + gets first ring on field (second ring overall)
    // turnAndMoveToPoint(
    //     -32.4
    //     , -40
    //     , 650
    //     , TO
    //     , false
    //     , false
    //     , 115.0
    // );
    turnAndMoveToPoint(
        -32.4
        , -40
        , {
            .turnTO = 650
            , .forwards = false
            , .mvMaxSpeed = 115.0
        }
    );
    pros::delay(250);
    intake.brake();

    // go to + clamps second mogo on field
    // turnAndMoveToPoint(
    //     -15.5
    //     , -46.5
    //     , 1000
    //     , 2250
    //     , true
    //     , true
    //     , 70
    // );
    turnAndMoveToPoint(
        -15.5
        , -46.5
        , {
            .turnTO = 1000
            , .moveTO = 2250
            , .forwards = true
        }
    );
    chassis.waitUntil(15.5);
    mogo.toggle();
    waitd;

    // scores second ring
    intake.intake();
    pros::delay(500);

    // goes to corner of tile in front of corner
    // turnAndMoveToPoint(
    //     -56
    //     , -52
    //     , 500
    //     , 1000
    //     , false
    // );
    turnAndMoveToPoint(
        -56
        , -52
        , {
            .turnTO = 500
            , .moveTO = 1000
            , .forwards = false
        }
    );

    // moves slowly to corner
    // turnAndMoveToPoint(
    //     -95
    //     , -71
    //     , 1000
    //     , 1000
    //     , false
    //     , false
    //     , 75.0
    // );
    turnAndMoveToPoint(
        -95
        , -71
        , {
            .turnTO = 1000
            , .moveTO = 1000
            , .forwards = false
            , .mvMaxSpeed = 75.0
        }
    );

    pros::delay(100);
    // suddenly swings to reposition ring relative to intake
    chassis.swingToPoint(
        -132
        , -50
        , DriveSide::RIGHT
        , 500
        , {
            .forwards = false
            , .direction = lemlib::AngularDirection::CW_CLOCKWISE
        }
    );
    waitd;
    pros::delay(150);

    chassis.moveToPoint(
        -37.246
        , -13.228
        , 2500
        , {
            .forwards = false
        }
    );
    waitd;
    arm.set_pos(arm.LOADIN_POS);

    // stops intake in case the blue ring finds its way in
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
    // turnAndMoveToPoint(
    //     21
    //     , 17
    //     , 500
    //     , 1500
    //     , true
    //     , true
    //     , 80
    // );
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
    // turnAndMoveToPoint(
    //     36
    //     , 44
    //     , 650
    //     , TO
    //     , false
    //     , false
    //     , 75
    // );
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

    // chassis.turnToPoint(
    //     27
    //     , 10
    //     , TO
    // );
    // waitd;
    // chassis.moveToPoint(
    //     27
    //     , 10
    //     , TO
    //     , {
    //         .maxSpeed = 127
    //     }
    // );
    // waitd;
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
 * NOT a redo of the awp alg; slightly diff
 * but IS a redo of blue negative auton to account
 * for the fixed IMU (now points won't be drift offset!)
 */
void blue_neg_awp_redo() {
    // starts at top-right; inside of corner
    chassis.setPose(54.75, 16, 0);

    // starts outtaking so it pushes the blue ring away
    intake.outtake();

    // TODO: point TOO FAR UP
    // scores preload on alliance stake
    // -61.5, 5 -> correct pt but too far

    //Nov 30 fixed this issues above
    turnAndMoveToPoint(60.85 , 5.15  , {.turnTO = 700, .moveTO = 900, .forwards = false, .async=true });
    pros::delay(50);
    arm.set_pos(18000);
    waitd;
    pros::delay(350);
    // retracts, so as not to disturb ring when turning
    // arm.set_pos(arm.START_POS);
    // pros::delay(250);

    // goes back to old point
    chassis.moveToPose(47, 24 , -20 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    // lifts intake in prep to grab first red ring on top
    intake.lift(true);
    // starts INTAKING, since we WANT to get the red ring
    intake.intake();
    // turns and moves towards first ring on field (on top of stack)
    turnAndMoveToPoint(47, -1.908, {.turnTO = 750 , .moveTO = TO, .forwards = false});
    intake.lift(false);
    pros::delay(415);
    intake.brake();

    chassis.swingToPoint(22, 19.85, DriveSide::LEFT, 700);
    waitd;
    chassis.moveToPoint(22, 19.85, 1500, {.maxSpeed=85});
    chassis.waitUntil(28.85);
    mogo.toggle();
    waitd;
    pros::delay(25);

    chassis.turnToPoint(12, 30.313, 1000, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(12, 30.313, 900, {.forwards=false});
    waitd;
    
    pros::delay(250);

    chassis.turnToPoint(23.745, 29.42, 400);
    waitd;
    chassis.moveToPoint(23.745, 29.42, 800);
    waitd;
    
    // chassis.moveToPoint(9.373, 39.053, TO, {.forwards=false});
    // waitd;

    //grab mogo code (new) (Works as of nov 30)
    //moves back to have a better angle to the mogo
    // chassis.moveToPose( 52.759 , 8.715 , 30, 1000);

    // //Nov 30 have to tune the value since the ring keeps on flying or not pickin up ;-;
    // chassis.waitUntil(2.3);
    // intake.brake();
    // waitd;

    // //moves to the mogo
    // chassis.moveToPose(30.932, 20.603, 60, 1500, {.forwards = true, .maxSpeed = 90});
    // //wait until is pretty tuned (nov 30)
    // chassis.waitUntil(28);
    // mogo.toggle();
    // waitd;

//     //moves to the middle rings
//     chassis.moveToPose(23.527, 57.616, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

//     //this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
//     chassis.waitUntil(9);
//     intake.intake();
//     waitd;

//     //moves to the top rings! (works nov 30)
//     chassis.moveToPose(-9.3, 51.395, 280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
//     waitd;

//      //Getting the arm ready to score
//     arm.set_pos(arm.SCORE_POS);

//      //moves to the ladder for wp 
//     chassis.moveToPose(-17.616, 20.185, 315, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
//     waitd;
}

/**
 * ------------------------------------------------------------
 * Blue Positive 
 * ------------------------------------------------------------
 */

void blue_positive() {
    // starting position
    chassis.setPose(
        55
        , -36.5
        , -90
    );

    // turns and moves towards first mogo, clamping it
    // CHNG: 2500 -> 2000
    // turnAndMoveToPoint(
    //     24
    //     , -23.5
    //     , 500
    //     , 2000
    //     , true
    //     , true
    //     , 70
    // );
    turnAndMoveToPoint(
        24
        , -23.5
        , {
            .turnTO = 500
            , .moveTO = 2000
            , .async = true
            , .mvMaxSpeed = 70
        }
    );
    chassis.waitUntil(25.2);
    mogo.toggle();
    waitd;

    // intakes preload ring (first ring overall)
    intake.intake();
    pros::delay(1500);

    // lets go of first mogo
    mogo.toggle();
    pros::delay(150);

    // TODO: sometimes ring flat out doesn't get inside
    // goes to + gets first ring on field (second ring overall)
    // turnAndMoveToPoint(
    //     26.5
    //     , -42.5
    //     , 750
    //     , TO
    //     , false
    //     , false
    //     , 115.0
    // );
    turnAndMoveToPoint(
        26.5
        , -42.5
        , {
            .turnTO = 750
            , .forwards = false
            , .mvMaxSpeed = 115.0
        }
    );
    // pros::delay(200);
    intake.brake();
    pros::delay(50);

    // go to + clamps second mogo on field
    // turnAndMoveToPoint(
    //     0
    //     , -49.5
    //     , 500
    //     , 2250
    //     , true
    //     , true
    //     , 70
    // );
    turnAndMoveToPoint(
        0
        , -49.5
        , {
            .turnTO = 500
            , .moveTO = 2250
            , .async = true
            , .mvMaxSpeed = 70
        }
    );
    chassis.waitUntil(16);
    mogo.toggle();
    waitd;

    // scores second ring
    intake.intake();
    pros::delay(500);
    // goes to corner of tile in front of corner
    // turnAndMoveToPoint(
    //     44
    //     , -58
    //     , 500
    //     , 1000
    //     , false
    // );
    turnAndMoveToPoint(
        44
        , -58
        , {
            .turnTO = 500
            , .moveTO = 1000
            , .forwards = false
        }
    );

    // moves slowly to corner
    // turnAndMoveToPoint(
    //     81
    //     , -77.5
    //     , 1000
    //     , 2500
    //     , false
    //     , false
    //     , 75.0
    // );
    turnAndMoveToPoint(
        81
        , -77.5
        , {
            .turnTO = 1000
            , .moveTO = 2500
            , .forwards = false
            , .mvMaxSpeed = 75.0
        }
    );

    pros::delay(100);
    // suddenly swings to reposition ring relative to intake
    chassis.swingToPoint(
        132
        , -50
        , DriveSide::RIGHT
        , 500
        , {
            .forwards = false
            , .direction = lemlib::AngularDirection::CW_CLOCKWISE
        }
    );
    waitd;
    pros::delay(150);
    
    chassis.moveToPoint(
        16
        , -26.75
        , 2500
        , {
            .forwards = false
        }
    );
    // bc often overshoots on ladder for some reason
    chassis.waitUntil(80);
    chassis.cancelAllMotions();
    
    arm.set_pos(arm.LOADIN_POS);
}

void blue_positive_normal_points() {
    // starting position
    chassis.setPose(
        55
        , -36.5
        , -90
    );

    // turns and moves towards first mogo, clamping it
    turnAndMoveToPoint(
        24
        , -23.5
        , {
            .turnTO = 500
            , .moveTO = 2500
            , .async = true
            , .mvMaxSpeed = 70
        }
    );
    chassis.waitUntil(25.2);
    mogo.toggle();
    waitd;

    // intakes preload ring (first ring overall)
    intake.intake();
    pros::delay(800);

    // lets go of first mogo
    mogo.toggle();
    pros::delay(150);

    // TODO: sometimes ring flat out doesn't get inside
    // goes to + gets first ring on field (second ring overall)
    turnAndMoveToPoint(
        24
        , -62
        , {
            .turnTO = 750
            , .forwards = false
            , .mvMaxSpeed = 115.0
        }
    );
    pros::delay(200);
    intake.brake();
    pros::delay(50);
    
    // because bot inevitably overshoots, want to curve back to the mogo!
    chassis.moveToPose(
        0
        , -47
        , 270
        , 1000
        , {
            .minSpeed = 100.0
        }
    );
    chassis.waitUntil(40.0);
    mogo.toggle();
    waitd;
}

/**
 * ------------------------------------------------------------
 * Other Autons
 * ------------------------------------------------------------
 */


void test_auton() {
    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    chassis.setPose(
        0
        , 0
        , 90
    );

    // turnAndMoveToPoint(
    //     24
    //     , 24
    //     , 1000
    //     , 1000
    // );

    // turnAndMoveToPoint(
    //     -24
    //     , 24
    //     , 1000
    //     , 1000
    // );
}

void prog_skills() {
//     // policy of OVERSHOOTING on all rings!

//     std::cout << "Running PROG skills" << std::endl;

//     // mogo mech facing FIELD, at top left corner of mid-left tile (rel to driver station ig)
//     // old pt -62 -9
//     // -59, -17 -> adjustment for new starting pos
//     // -56.6, -19.1 -> adjustment to account for spec angle
//     // -59, -15.65 -> test 1 (angles all off)
//     chassis.setPose(-55.75, -7.25, 90);

//     // open mogo mech
//     mogo.toggle();
//     // wait a TINY second for it to open
//     pros::delay(500);

//     // moves forward
//     chassis.moveToPoint(-47, -7.25, 750);
//     waitd;

//     // // turns towards first mogo
//     // chassis.turnToPoint(-47, -24, 750);
//     // waitd;
//     // // MOVES towards first mogo
//     // chassis.moveToPoint(-47, -24, 1000);
//     // waitd;
//     turnAndMoveToPoint(-47, -24, 750, 1000);

//     // captures mogo; waits 500ms for it to settle
//     mogo.toggle();
//     pros::delay(500);

//     // intakes preload ring and waits
//     intake.intake();
//     pros::delay(1500);
//     intake.brake();

//     // turns & moves towards first ring on field, running intake as you go
//     // chassis.turnToPoint(-19.5, -23.5, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE });
//     // waitd;
//     // intake.intake();
//     // chassis.moveToPoint(-19.5, -23.5, 1000, { .forwards = false });
//     // waitd;
//     turnAndMoveToPoint(-19.5, -23.5, 1000, 1000, false);
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards second ring on field, STILL running intake!
//     // chassis.turnToPoint(-24.25, -50.5, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE});
//     // waitd;
//     // chassis.moveToPoint(-24.25, -50.5, 1000, { .forwards = false });
//     // waitd;
//     turnAndMoveToPoint(-24.25, -50.5, 1000, 1000, false);
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards third ring on field, STILL running intake!
//     // chassis.turnToPoint(-52.35, -46.25, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE});
//     // waitd;
//     // chassis.moveToPoint(-52.35, -46.25, 1000, { .forwards = false });
//     // waitd;
//     turnAndMoveToPoint(-52.35, -46.25, 1000, 1000, false);
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards fourth ring on field, STILL running intake!
//     // chassis.turnToPoint(-62.35, -48.4, 1000, { .forwards = false, .direction = AngularDirection::CCW_COUNTERCLOCKWISE});
//     // waitd;
//     // chassis.moveToPoint(-62.35, -48.4, 1000, { .forwards = false });
//     // waitd;
//     turnAndMoveToPoint(-62.35, -48.4, 1000, 1000, false);
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards fifth ring on field, STILL running intake!
//     chassis.turnToPoint(-44.5, -61.35, 1000, { .forwards = false, .direction = AngularDirection::CCW_COUNTERCLOCKWISE});
//     waitd;
//     chassis.moveToPoint(-44.5, -61.35, 1000, { .forwards = false });
//     waitd;
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // mogo filled; puts in corner!
//     chassis.turnToPoint(-66.6, -66.6, 750, { .direction = AngularDirection::CCW_COUNTERCLOCKWISE });
//     waitd;
//     // opens mogo mech so now we're PUSHING mogo, and then pushes it to corner; OVERSHOOTS SO THAT MOGO IS GUARANTEED TO GO IN!
//     mogo.toggle();
//     // waits for mogo mech to open
//     pros::delay(500);
//     // moves to corner
//     chassis.turnToPoint(-66.6, -66.6, 1000);
//     waitd;

//     // returns to previous point
//     chassis.moveToPoint(-44.5, -61.35, 1000, { .forwards = false });
//     waitd;
//     // turns & moves to point in front of second mogo
//     chassis.turnToPoint(-47, 4, 1000, { .direction = AngularDirection::CW_CLOCKWISE });
//     waitd;
//     chassis.moveToPoint(-47, 4, 1000);

    

    

//     // // turns AND moves towards the second ring on field WHILE intaking
//     // chassis.turnToPoint(-19.5, -47.5, 750, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE });
//     // waitd;
//     // chassis.moveToPoint(-19.5, -47.5, 750, { .forwards = false });
//     // waitd;
//     // // waits for ring to be intaked
//     // pros::delay(1500);
}

void red_cross_sawp() {
    // ASSUME PUSHING HAS ALR HAPPENED!
    chassis.setPose(
        -58.6
        , 47
        , 90
    );

    auto start = std::chrono::high_resolution_clock::now();

    // turns and moves towards mogo -> 42.6867
    // turnAndMoveToPoint(
    //     -24
    //     , 22
    //     , 1000
    //     , 1500
    //     , true
    //     , true
    // );
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
    // turnAndMoveToPoint(
    //     -24
    //     , 48
    //     , 1000
    //     , 1000
    //     , false
    // );
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
    // turnAndMoveToPoint(
    //     -45
    //     , -9.5
    //     , 1500
    //     , 2000
    //     , false
    //     , true
    // );
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
    
    // turnAndMoveToPoint(
    //     -23.5
    //     , -18.5
    //     , 2000
    //     , 2000
    //     , true
    //     , true
    // );
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
    // turnAndMoveToPoint(
    //     -25.5
    //     , -47.25
    //     , 1000
    //     , 1000
    //     , false
    // );
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