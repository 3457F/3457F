#include "doinker.hpp"
#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "mogo.hpp"
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

    // adds in early exit range for move
    float mvEarlyExitRange = 0.0;
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
                , .earlyExitRange = params.mvEarlyExitRange
            }
        );
    }

    if (!params.async) {
        waitd;
    }
}

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
 * PART OF THE ALG for 
 * `red_neg_awp_redo()` and
 * `red_neg_elims */
void red_neg_redo() {
    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(
        -54.75
        , 16
        , 0
    );

    // // starts outtaking so it pushes the blue ring away
    // intake.outtake();

    // // goes towards the alliance stake
    // turnAndMoveToPoint(
    //     -61
    //     , 6
    //     , {
    //         .turnTO = 500
    //         , .moveTO = 1000
    //         , .forwards = false
    //         , .async = true
    //     }
    // );
    // pros::delay(250);
    // // queues arm to score; waits for arm to score on
    // // alliance stake
    // arm.set_pos(arm.ALLIANCE_SCORE);
    // waitd;
    // pros::delay(750);

    // // goes back to old point (NOT retracting arm yet
    // // so it doesn't disturb the scored ring)
    // chassis.moveToPose(
    //     -54.75
    //     , 16
    //     , 30
    //     , 1000
    // );
    // waitd;

    // // starts retracting the arm
    // arm.set_pos(arm.START_POS);

    /**
     * TODO: try to make this cross less, so it doesn't interfere with the alliance's auton!
     */

    // // lifts intake in prep to grab first red ring on top
    // intake.lift(true);
    // // starts INTAKING, since we WANT to get the red ring
    // intake.intake();
    // // turns and moves towards first ring on field
    // // (on top of stack)
    // turnAndMoveToPoint(
    //     -49.094
    //     , 1.897
    //     , {
    //         .turnTO = 750
    //         , .moveTO = 1250
    //         , .forwards = false
    //         , .mvMaxSpeed = 65.0
    //     }
    // );
    // // drops intake onto the stack; waits for ring
    // // to get loaded into intake
    // intake.lift(false);
    // pros::delay(750);

    // moves back to have a better angle of the mogo
    chassis.moveToPose(
        -52.759
        , 8.715
        , 30
        , 1000
    );
    // immediately brakes intake to
    // ensure that the ring stays in the intake
    intake.brake();
    waitd;

    // moves to and clamps the mogo
    chassis.turnToPoint(
        -25.514
        , 22.911
        , 800
    );
    waitd;
    chassis.moveToPoint(
        -25.514
        , 22.911
        , 2500
        , {
            .maxSpeed = 50
        }
    );
    // chassis.waitUntil(23.35);
    chassis.waitUntil(25);
    mogo.toggle();
    waitd;

    // moves to the middle rings
    chassis.moveToPose(
        -21.849
        , 45.948
        , 180
        , 2500
        , {
            .forwards = false
            , .horizontalDrift = 2
            , .lead = 0
            , .maxSpeed = 127
        }
    );
    // starts intake only after a while to ensure that
    // we don't accidentally intake a blue ring
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    // moves to and scores top rings
    chassis.moveToPose(
        -9.3
        , 51.395
        , 280.3
        , 1000
        , {
            .forwards = false
            , .horizontalDrift = 2
            , .lead = 0
            , .maxSpeed = 127
        }
    );
    waitd;
}

/**
 * NOT a redo of the awp alg; slightly diff
 * but IS a redo of red negative auton to account
 * for the fixed IMU (now points won't be drift offset!)

 * ALSO NOT THE SAME AS `red_neg_elims`; TOUCHES LADDER
 * INSTEAD OF GOING FOR ALLIANCE STAKE

 * WORKS WITH NEW IMU POINTS
 */
void red_neg_awp_redo() {
    // go forward less on both stacks
    // go forward MORE on the alliance stake
    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(-54.75, 16, 0);

    // starts outtaking so it pushes the blue ring away

    // goes towards the alliance stake and scores
    // BIT too much forward
    // turnAndMoveToPoint(-60.459, 6.561, {.turnTO = 1000, .moveTO = 1000, .forwards = false });
    // turnAndMoveToPoint(-61, 6, {.turnTO = 500, .moveTO = 1000, .forwards = false , .async=true});
    // pros::delay(250);
    // arm.set_pos(arm.ALLIANCE_SCORE);
    // waitd;
    // pros::delay(750);

    chassis.moveToPose(-58.411, 12.028, 42.8, 1500, {.forwards = false});
    waitd;
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::c::delay(870);
    // goes back to old point (NOT retracting arm yet
    // so it doesn't disturb the scored ring)
    chassis.moveToPose(
        -47.303, 22.747 , 30 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    /**
     * TODO: try to make this cross less, so it doesn't interfere with the alliance's auton!
     */

    // lifts intake in prep to grab first red ring on top
    intake.lift(1);
    // starts INTAKING, since we WANT to get the red ring
    
    // turns and moves towards first ring on field (on top of stack)
    // -49.212, -1.908
    turnAndMoveToPoint(-49.094, 1.897, {.turnTO = 750 , .moveTO = 1250, .forwards = false, .mvMaxSpeed = 65.0});
    intake.intake();
    intake.lift(false);
    pros::delay(750);

    // moves back to have a better angle to the mogo
    chassis.moveToPose( -52.759 , 8.715 , 30, 1000);
    intake.brake();
    waitd;
    // moves to and clamps mogo
    chassis.turnToPoint(-25.514, 22.911, 800);
    waitd;
    chassis.moveToPoint(-25.514, 22.911, 2500, {.maxSpeed=50});
    chassis.waitUntil(23.35);
    mogo.toggle();
    waitd;

    // moves to the middle rings
    // -23.527, 57.616
    // too far LEFT
    // chassis.moveToPose(-23.663, 45.948, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    chassis.moveToPose(-21.849, 45.948, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

    // this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    //moves to the top rings! (works nov 30)
    chassis.moveToPose(-9.3, 51.395, 280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;

    //Getting the arm ready to score
    arm.set_pos(arm.SCORE_POS);

    //moves to the ladder for wp 
    chassis.moveToPose(-12.499, 12.499, 315, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;
}

void red_neg_elims(){
    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(-54.75, 16, 0);

    pros::delay(4500);

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
 * same as `red_neg_awp_redo()` but
 * with the shared red neg redo code
 * split out into `red_neg_redo()`
 */
void red_neg_awp_redo_new() {
    red_neg_redo();

    // queues arm to extend, in preparation to touch the ladder
    arm.set_pos(arm.SCORE_POS);

    // moves to the ladder to complete the auton win point
    // algorithm
    chassis.moveToPose(-12.499, 12.499, 315, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;
}

void red_neg_elims_new() {
    red_neg_redo();
    
    // moves to the alliance stake to have a better shot for pos corner
    chassis.moveToPose(
        -38.143
        , 2.674
        , 0
        , 3000
        , {
            .forwards = false
            , .horizontalDrift = 2
            , .lead = 0
            , .maxSpeed = 80.0
        }
    );
}

/**
 * ------------------------------------------------------------
 * RED POSITIVE
 * ------------------------------------------------------------
 */

 void red_rush(){
    chassis.setPose(-54.473,-59,270);

//for some reason drifts time to time, gotta slow it down def, but currently reseraching a way.
    //only running the floating cuz there is not need for hooks
    intake.floating();
    chassis.moveToPose(-10.266,-49.733, 261 , 1200, {.forwards = false, .maxSpeed =127, .minSpeed = 114});
    //setting arm to dunk POS to ensure that arm doesn't get in the way of the hooks later on!
    arm.set_pos(arm.START_POS);
    waitd;

    intake.brake();

    //doinker to touch/grab the goal
    doinker.toggle();
    pros::delay(420);

    //moving the robot and the goal back!
    chassis.moveToPose(-40.092, -56.182, 255, 1100, {.forwards = true});
    waitd;
    doinker.toggle();
    
    //going to clamp mogo
    chassis.moveToPose(-6.778, -50.238, 270, 1500, {.forwards = true, .maxSpeed = 60});
    waitd;
    mogo.toggle();
    intake.intake();
    pros::delay(450);

    //moves towards our positive corner and drops mogo.
    turnAndMoveToPoint( -47.985, -61.671, {.forwards =true});
    waitd;
    mogo.toggle();

    chassis.moveToPose(-23.592, -32.826, 0, 1150, {.forwards = true});
    waitd;
    chassis.moveToPose(-23.592,-27.826 , 0, 1300, {.forwards = true});
    waitd; 
    mogo.toggle();

    intake.lift(true);
    turnAndMoveToPoint(-41.944, -0.619, {.forwards = false, .mvMaxSpeed = 105});
    waitd;
   intake.lift(false);
   pros::delay(860);

   chassis.moveToPose(-39.475, -54.537, 342, 1300);
 }

// THREE RING, TWO STAKE AUTON -> does awp!!!
void red_positive() {
    // starting position
    chassis.setPose(
        -55
        , -36.5
        , 90
    );

    // turns and moves towards first mogo, clamping it
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
 * NOT a redo of the awp alg; slightly diff
 * but IS a redo of blue negative auton to account
 * for the fixed IMU (now points won't be drift offset!)
 */
void blue_neg_awp_redo() {
    // go forward less on both stacks
    // go forward MORE on the alliance stake
    pros::Task task(
        &print_robot_pos
        , &chassis
    );

    // starts at top-right; inside of corner
    chassis.setPose(54.75, 16, 0);

    // starts outtaking so it pushes the blue ring away
    intake.outtake();

    // goes towards the alliance stake and scores
    // BIT too much forward
    // turnAndMoveToPoint(-60.459, 6.561, {.turnTO = 1000, .moveTO = 1000, .forwards = false });
    turnAndMoveToPoint(59.9,5.9, {.turnTO = 500, .moveTO = 1000, .forwards = false , .async=true});
    pros::delay(250);
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;
    pros::delay(750);

    // goes back to old point (NOT retracting arm yet
    // so it doesn't disturb the scored ring)
    chassis.moveToPose(
        54.75, 16 , -30 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    /**
     * TODO: try to make this cross less, so it doesn't interfere with the alliance's auton!
     */

    // lifts intake in prep to grab first red ring on top
    intake.lift(1);
    // starts INTAKING, since we WANT to get the red ring
    intake.intake();
    // turns and moves towards first ring on field (on top of stack)
    // -49.212, -1.908
    turnAndMoveToPoint(47.078, -0.176, {.turnTO = 750 , .moveTO = 1250, .forwards = false, .mvMaxSpeed = 65.0});
    intake.lift(false);
    pros::delay(750);

    // moves back to have a better angle to the mogo
    chassis.moveToPose( 52.759 , 8.715 , 30, 1000);
    intake.brake();
    waitd;
    // moves to and clamps mogo
    chassis.turnToPoint(25.254, 20.047, 800);
    waitd;
    chassis.moveToPoint(25.254, 20.047, 2500, {.maxSpeed=50});
    chassis.waitUntil(23.6);
    mogo.toggle();
    waitd;

    // moves to the middle rings
    // -23.527, 57.616
    // too far LEFT
    // chassis.moveToPose(-23.663, 45.948, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    chassis.moveToPose(21.849, 45.948, -180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});

    // this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    //moves to the top rings! (works nov 30)
    chassis.moveToPose(9.3, 51.395, -280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;

    //Getting the arm ready to score
    arm.set_pos(arm.SCORE_POS);

    //moves to the ladder for wp 
    chassis.moveToPose(12.499, 12.499, -315, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;
}

/** same as blue_neg_awp_redo but goes next to alliance stake
    so is most convenient for aarav */
void blue_neg_elims() {
    // starts at top-right; inside of corner
    chassis.setPose(54.75, 16, 0);

    pros::delay(4500);

    // starts outtaking so it pushes the blue ring away
    intake.outtake();
    
    //Nov 30 fixed this issues above
    turnAndMoveToPoint(60.85 , 5.15, {.turnTO = 700, .moveTO = 900, .forwards = false, .async=true });
    pros::delay(50);
    arm.set_pos(18000);
    waitd;
    pros::delay(350);
    // retracts, so as not to disturb ring when turning

    // goes back to old point
    chassis.moveToPose(47, 24 , -20 , 1000);

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    /**
     * TODO: try to make this cross less, so it doesn't interfere with the alliance's auton!
     */

    // lifts intake in prep to grab first red ring on top
    intake.lift(true);
    // starts INTAKING, since we WANT to get the red ring
    intake.intake();

    // turns and moves towards first ring on field,
    // GETTING IT INTO the intake (but not scoring)
    turnAndMoveToPoint(47, -1.908, {.turnTO = 750 , .moveTO = TO, .forwards = false});
    intake.lift(false);
    pros::delay(415);
    intake.brake();

    waitd;


    // move to pose -- gets mogo 
    chassis.moveToPose(25.585, 21.773, 299, 1500,{.forwards = true, .maxSpeed = 85, .minSpeed =60});
    chassis.waitUntil(31);
    mogo.toggle();
    waitd;

    pros::delay(1050);

    // gets second ring on field
    chassis.moveToPose(5.706, 42.431, 135, 1500, { .forwards = false });
    pros::delay(500);
    intake.intake();
    waitd;

    // moves back to where mogo was, in prep to get
    // third (middle) ring
    chassis.moveToPose(25.585, 21.773, 138, 1500,{.forwards = false, .maxSpeed = 85, .minSpeed =60});
    waitd;


    // turns, moves towards and scores third (middle) ring
    chassis.turnToHeading(177, 850);
    waitd;
    // additional puts arm up in preparation to appraoch the ladder
    arm.set_pos(arm.SCORE_POS);
    chassis.moveToPose(23.636, 47.303, 177, 1200, {.forwards = false});
    waitd;

    // swings to ladder with intake side facing???
    chassis.swingToHeading(48, lemlib::DriveSide::RIGHT, 850, {.direction = lemlib::AngularDirection::CW_CLOCKWISE});
    waitd;

    // TODO: TEST
    //moves to the top rings! (works nov 30)
    chassis.moveToPose(9.3, 51.395, 280.3, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    waitd;

    //moves to the alliance stake to have a better shot for pos corner
    chassis.moveToPose(38.143, 2.674, 0, 3000, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
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

    mogo.request_clamp();

    chassis.moveToPoint(
        24
        , 0
        , 1000
    );
}

void prog_skills() {
    // CODED RED NEG; NOT GOING FOR RED POS FIRST BC PPL WERE ON THE FIELD WHEN WE WERE TUNING
    chassis.setPose(-60.478, -0.444, 270);

    // scores preload on alliance stake
    intake.intake();
    pros::delay(750);

    /* sunny's mogo approach */

    // turn to mogo 

    chassis.swingToHeading(
        0
        , DriveSide::LEFT
        , 1000
        , {
            .direction = AngularDirection::CW_CLOCKWISE
            , .maxSpeed = 80.0
        }
    );
    waitd;

    // actual moving to mogo

    // bc swing results in variable x coord due to drift;
    // want to move DIRECTLY up!
    lemlib::Pose approach_mogo_pose = chassis.getPose();

    chassis.moveToPoint(
        approach_mogo_pose.x
        , 23.721
        , 2000
        , {
            .maxSpeed = 70.0
        }
    );
    // TODO: verify is tuned
    // chassis.waitUntil(27);
    // mogo.toggle();
    mogo.request_clamp();
    waitd;

    // goes for first ring on field; intake still running!
    turnAndMoveToPoint(
        -23.721
        , 21.967
        , {
            .turnTO = 750
            , .moveTO = 1000
            , .forwards = false
        }
    );

    // goes for second ring on field; intake still running!
    turnAndMoveToPoint(
        -23.721
        , 47.303
        , {
            .turnTO = 750
            , .moveTO = 1500
            , .forwards = false
        }
    );

    // turns and scores third ring (which is on the autonomous line on red neg side)
    // TODO: needs to go faster
    chassis.moveToPose(
        0.047
        , 59.034
        , 270
        , 750
        , {
            .forwards = false
            , .lead = 0.2
            , .minSpeed = 90.0
        }
    );
    waitd;
    
    // turns and obtains fourth red ring on field, the upper lone ring on the blue negative side,
    // to score on the negative neutral wall stake
    chassis.moveToPose(
        24.016
        , 47.244
        , 300
        , 2000
        , {
            .forwards = false
            , .minSpeed = 60.0
        }
    );
    // rest of auton!
    waitd;

    // queues arm to go to load in position
    arm.set_pos(arm.LOADIN_POS);

    // waits a bit more to get ring
    pros::delay(500);

    // moves back to where the ring on autonomous line was
    // in prep to turn towards alliance stake
    chassis.moveToPose(
        -0.215
        , 50.118
        , 180
        , 750
        , {
            .lead = 0.2
            , .minSpeed = 60.0
        }
    );
    waitd;

    // moves towards neutral wall stake
    chassis.moveToPoint(
        -0.215
        , 57.027
        , 500
        , {
            .forwards = false
        }
    );
    waitd;

    // turns IN PLACE to the neutral wall stake
    chassis.turnToHeading(
        180
        , 1000
    );

    pros::delay(850);
    // stop intake so it doesn't inhibit the arm ;-;
    intake.brake();

    waitd;

    // intake.intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // pros::delay(250);
    intake.outtake();
    pros::delay(50);
    // queues arm to move to scoring position -> overshooting a bit!
    arm.set_pos(arm.ALLIANCE_SCORE);

    // as you do that, VERY SLOWLY moves forward
    chassis.moveToPoint(
        -0.215
        , 66.496
        , 750
        , {
            .forwards = false
            , .maxSpeed = 30.0
        }
    );

    pros::delay(250);
    intake.intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    waitd;

    // retracts arm
    arm.set_pos(arm.INIT_POS);

    // as you do that, move backwards from the stake!
    chassis.moveToPoint(
        -0.215
        , 50.118
        , 750 // extra time for arm to retract, so when we turn arm doesn't accidentally hit stake!
    );

    // starts intake again
    intake.intake();

    // turns towards and scores fifth ring on field, the top-most of the corner inverted L shape
    turnAndMoveToPoint(
        -47.047
        , 47.303
        , {
            .turnTO = 750
            , .moveTO = 2000
            , .forwards = false
            , .mvEarlyExitRange = 5.0
        }
    );

    // movetoposes towards sixth ring on field
    chassis.moveToPose(
        -58.947
        , 47.047
        , 90
        , 1000
        , {
            .forwards = false
            , .lead = 0.2
        }
    );
    waitd;

    // slowly moves backwards so we can face the seventh ring
    chassis.moveToPose(
        -47.047
        , 36.101
        , 180
        , 1500
        , {
            .lead = 0.2
        }
    );

    // moves towards and scores seventh ring on field! (slowly tho, bc
    // prev ring might still be inside)
    chassis.moveToPoint(
        -47.047
        , 58.904
        , 1500
        , {
            .forwards = false
            , .maxSpeed = 80.0
        }
    );
    // waits a bit, in case ring still intaking!
    pros::delay(2000);

    // turns and pushes mogo into corner
    turnAndMoveToPoint(
        -65.901
        , 65.382
        , {
            .turnTO = 750
            , .moveTO = 1000
        }
    );
    // releases mogo into corner!
    mogo.toggle();
    pros::delay(500);

    // // moves out of the corner
    // chassis.moveToPoint(
    //     -47.114
    //     , 59.034
    //     , 1000
    //     , {
    //         .forwards = false
    //     }
    // );

    // // turns towards next mogo
    // chassis.turnToPoint(
    //     -46.815
    //     , -23.343
    //     , 1000
    // );

    // moves towards mogo on "blue neg" side
    chassis.moveToPoint(
        41.636
        , 34.806
        , 8000
        , {
            .forwards = false
            , .maxSpeed = 80.0
        }
    );
    waitd;

    // turns towards mogo
    // chassis.turnToPoint(
    //     58.997
    //     , 23.663
    //     , 2000
    // );
    // waitd;
    chassis.turnToHeading(110, 1000);

    // moves to AND CLAMPS mogo
    chassis.moveToPoint(
        58.997
        , 23.663
        , 1000
    );
    // TODO: tune
    chassis.waitUntil(24.5);
    mogo.toggle();
    waitd;



    // // SWING to turn around, so that the ring is forced into intake if not already
    // chassis.swingToHeading(
    //     0
    //     , DriveSide::LEFT
    //     , 2500
    //     , {
    //         .direction = AngularDirection::CW_CLOCKWISE
    //     }
    // );
    // waitd;
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