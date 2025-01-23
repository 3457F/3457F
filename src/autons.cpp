#include "main.h"
#include "util.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "pros/rtos.hpp"

#include <chrono>
#include <set>

// import pure pursuit files
ASSET(red_neg_first_mogo_txt);
ASSET(third_ring_txt);
ASSET(blue_rush_first_part_txt);

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

    chassis.moveToPose(
        -58.411
        // -57.047
        , 12.028
        // , 13.492
        , 42.8, 1500, {.forwards = false});
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
    chassis.setPose(
        -54.473
        , -59
        , 270
    );

    // setting arm to dunk POS to ensure that arm doesn't get in the way of the hooks later on
    arm.set_pos(arm.DUNK_POS);

    // for some reason drifts time to time, gotta slow it down def, but currently reseraching a way.
    // only running the floating cuz there is not need for hooks
    intake.floating();
    chassis.moveToPose(
        -10.266
        , -49.733
        , 261
        , 1250
        , {
            .forwards = false
            , .maxSpeed = 127
            , .minSpeed = 114
        }
    );
    waitd;

    //doinker to touch/grab the goal
    doinker.toggle();
    pros::delay(420);

    //moving the robot and the goal back!
    chassis.moveToPose(-40.092, -56.182, 261, 1200, {.forwards = true});
    waitd;
    doinker.toggle();
    
    //going to clamp mogo
    chassis.moveToPose(-6.778, -50.238, 270, 1200, {.forwards = true, .minSpeed = 60});
    waitd;
    mogo.toggle();
    intake.intake();
    pros::delay(450);

    //moves towards our positive corner and drops mogo.
    turnAndMoveToPoint( -47.985, -61.671, {.forwards =true});
    waitd;
    mogo.toggle();

    // chassis.moveToPose(-23.592, -32.826, 0, 1150, {.forwards = true});
    // waitd;
    chassis.moveToPose(-24.592,-27.826 , 0, 2500, {.forwards = true,});
    waitd; 
    mogo.toggle();

//     intake.lift(true);
//     turnAndMoveToPoint(-41.066, -6.291, {.forwards = false, .mvMaxSpeed = 105});
//     waitd;
//    intake.lift(false);
//    pros::delay(860);

    chassis.turnToPoint(-75, 0, 650, {.forwards = false});
    waitd; 
    intake.brake();
    chassis.moveToPose(-39.475, -54.537, 342, 1300);
}

// TODO: need to tune
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
    // so that the arm doesn't start tweaking out
    arm.set_pos(arm.LOADIN_POS);

    turnAndMoveToPoint(
        // 59.36
        58.522
        // , 10.414
        // , 11.017
        , 10.522
        , {
            .turnTO = 750
            , .turnDir = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
            , .moveTO = 1000
            , .forwards = false
        }
    );
    waitd;
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(770);

    lemlib::Pose away_from_alliance = chassis.getPose();

    /* NEW CODE: moves back from alliance stake;
     * d/n retract arm so as not to disturb scored ring */
    // chassis.moveToPose(
    //     47.123
    //     , 29.42
    //     , away_from_alliance.theta
    //     , 1000
    // );

    chassis.moveToPoint(
        47.123
        , 29.42
        , 500
    );
    waitd;

    // starts retracting the arm
    arm.set_pos(arm.START_POS);

    /**
     * TODO: try to make this cross less, so it doesn't interfere with the alliance's auton!
     */

    // lifts intake in prep to grab first blue ring on top
    intake.lift(1);
    // starts intaking
    intake.floating();
    // // turns and moves towards first ring on field (on top of stack)
    // turnAndMoveToPoint(
    //     47.123
    //     , -1.822
    //     , {
    //         .turnTO = 750
    //         , .moveTO = 1250
    //         , .forwards = false
    //         , .mvMaxSpeed = 65.0
    //     }
    // );
    chassis.moveToPoint(
        47.123
        , -1.822
        , 1000
        , {
            .forwards = false
            , .maxSpeed = 60.0
        }
    );
    waitd;
    // once there, drop intake so that the intake exerts pressure on the top red ring,
    // queueing it into the robot
    intake.lift(false);
    // ring intakes RLLY fast;
    // stop it after 100ms
    // to ensure blue ring gets "queued"
    pros::delay(200);
    intake.brake();

    // moves back to have a better angle to the mogo
    chassis.moveToPose(
        52.759
        , 8.715
        , 30
        , 200
    );
    waitd;
    // moves to and clamps mogo
    chassis.turnToPoint(25.254, 20.047, 800);
    waitd;
    chassis.moveToPoint(
        25.254
        , 20.047
        , 1500
        , {
            .maxSpeed = 70.0
        }
    );
    chassis.waitUntil(24.5);
    mogo.toggle();
    waitd;

    // moves to the middle rings
    // -23.527, 57.616
    // too far LEFT
    // chassis.moveToPose(-23.663, 45.948, 180, 2500, {.forwards = false, .horizontalDrift =2, .lead = 0,.maxSpeed=127});
    chassis.moveToPose(
        25.775
        , 47.384
        , -180
        , 2000
        , {
            .forwards = false
            , .horizontalDrift = 2
            , .lead = 0
            , .maxSpeed = 127
        }
    );
    
    // this wait until is for the first ring that we picked up with the intake lift since when turning fast and intaking is like ;-;
    chassis.waitUntil(9);
    intake.intake();
    waitd;

    // // queues NEXT ring only to be intaked after turning
    // pros::delay(100);
    // intake.brake();

    // moves to the top rings! (works nov 30)
    chassis.moveToPose(
        9.77
        , 54.077
        , -280.3
        , 1500
        , {
            .forwards = false
            , .horizontalDrift = 2
            , .lead = 0
            , .maxSpeed = 127
        }
    );
    intake.intake();
    waitd;

    chassis.turnToHeading(
        0
        , 750
    );
    waitd;

    //Getting the arm ready to score
    arm.set_pos(arm.SCORE_POS);

    //moves to the ladder for wp 
    // chassis.moveToPose(
    //     12.499
    //     , 12.499
    //     , -315
    //     , 3000
    //     , {
    //         .forwards = false
    //         , .horizontalDrift = 2
    //         , .lead = 0
    //         , .maxSpeed = 127
    //     }
    // );
    turnAndMoveToPoint(
        12.499
        , 12.499
        , {
            .turnTO = 750
            , .moveTO = 1000
            , .forwards = false
            , .mvMaxSpeed = 60.0
        }
    );
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

// TODO: need to fix
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

void blue_pos_safe() {
    // starts near the top ring
    chassis.setPose(
        54.428
        , -14.184
        , 180
    );

    // moves towards the stack with blue ring on top;
    // RUNS ONLY FLOATING to ensure the ring is only "queued in"
    // and preload is not thrown out of the robot
    intake.lift(true);
    turnAndMoveToPoint(
        49.75
        , -5.122
        , {
            .forwards = false
        }
    );
    intake.floating();
    waitd;
    // plops intake onto stack to create friction that intakes the
    // blue ring on the stack 
    intake.lift(false);
    pros::delay(300);

    // goes to mogo and clamps it
    turnAndMoveToPoint(
        // 32.211
        31.382
        // , -18.374
        , -18.929
        // , -19.905
        , {
            .async = true
            , .mvMaxSpeed = 80
        }
    );
    chassis.waitUntil(16.75);
    // chassis.moveToPose(
    //     30.161
    //     , -20.394
    //     , 240
    //     , 1200
    // );
    mogo.clamp();
    waitd;

    // return;

    intake.brake();
    intake.intake();
    // waits to ensure all queued rings are scored
    pros::delay(150);

    // goes to the leftmost stack in corner
    turnAndMoveToPoint(21.784, -51.699, {.moveTO = 900, .forwards = false});
    intake.intake();
    waitd;

    // goes to negative corner
    chassis.moveToPose(64.562, -65.146, 294.6, 2000, {.forwards = false});
    chassis.waitUntil(10);
    doinker.toggle(); 
    waitd;
    pros::delay(500);

    // turns around to force the doinker to clear the corner
    // also starts outtaking in case the sneaky red ring is upon us
    intake.outtake();
    chassis.turnToHeading(180, 700);
    waitd;
    doinker.toggle();

    // turns back horizontal
    chassis.turnToHeading(270, 650);

    // moves back just cuz 
    lemlib::Pose move_back_pose = chassis.getPose();
    chassis.moveToPoint(
        30.649
        , move_back_pose.y
        , 1000
    );
    waitd;

    doinker.toggle();
    arm.set_pos(arm.SCORE_POS);
    // goes towards ladder for awp
    turnAndMoveToPoint(
        16.452
        , -17.937
        , {
            .turnTO = 750
            , .moveTO = 1000
            , .forwards = false
        }
    );
}

void blue_rush() {
    chassis.setPose(
        54.573
        , -59
        , 90
    );

    arm.set_pos(arm.START_POS);

    intake.floating();

    // get first blue ring on field
    chassis.follow(
        blue_rush_first_part_txt
        , 10
        , 1000
        , false
    );
    waitd;

    // move to mogo
    turnAndMoveToPoint(
        14.967
        , -48.623
        , {
            .turnTO = 500
            , .turnDir = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
            , .moveTO = 750
            , .forwards = false
        }
    );

    // grab goal with doinker
    doinker.toggle();
    pros::delay(420);

    // moves robot + goal back
    chassis.moveToPoint(
        30.805
        , -45.901
        , 2000
        , {
            .maxSpeed = 30.0
        }
    );
    waitd;

    // release mogo and turn around
    doinker.toggle();
    chassis.moveToPoint(
        41.694
        , -43.674
        , 500
    );
    turnAndMoveToPoint(
        30.805
        , -45.901
        , {
            .turnTO = 500
            , .turnDir = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
            , .moveTO = 750
        }
    );
}

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

    // mogo.request_clamp();

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
    pros::delay(1000);

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
    chassis.waitUntil(27);
    mogo.toggle();
    // mogo.request_clamp();
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

void prog_skills_1095r() {
    chassis.setPose(
        -59.678
        , -0.444
        , 270
    );

    // scores preload on alliance stake
    intake.intake();
    pros::delay(1000);

    /** swing */

    // turn to red positive mogo
    // chassis.swingToHeading(
    //     180
    //     , DriveSide::RIGHT
    //     , 1000
    //     , {
    //         .direction = AngularDirection::CCW_COUNTERCLOCKWISE
    //         , .maxSpeed = 80.0
    //     }
    // );
    // waitd;

    /** approach red positive mogo, clamp */

    // moves forward, in prep to turn towards mogo
    chassis.moveToPoint(
        -47.244
        , -0.444
        , 500
        , {
            .forwards = false
        }
    );
    waitd;

    // turns and moves towards mogo, clamping it!
    turnAndMoveToPoint(
        -46.207
        , -23.757
        , {
            .turnTO = 750
            , .moveTO = 1000
            , .async = true
            , .mvMaxSpeed = 65.0
            , .waitUntil = 24
            , .usePoseX = true
        }
    );
    mogo.clamp();
    waitd;


    /** first ring on field (red positive) */

    turnAndMoveToPoint(
        -23.404
        , -25.052
        , {
            .turnTO = 750
            , .moveTO = 750
            , .forwards = false
            , .mvMaxSpeed = 65.0
        }
    );


    /** second ring on field (red positive)
     * -> aim to score into alliance stake!
    */

    // TODO: kinda sucks at getting mogo ngl...
    turnAndMoveToPoint(
        23.542
        , -44.973
        , {
            .turnTO = 750
            , .moveTO = 2000
            , .forwards = false
            , .async = true
        }
    );
    pros::delay(1500);
    // moveTO is intentionally set higher than it needs
    // to be, so we wait 500ms less so the robot is
    // more or less near the second ring before the 
    // arm goes up, giving the first ring time
    // to be scored
    arm.set_pos(arm.LOADIN_POS);
    waitd;


    /** align with wall stake */

    // turns horizontal with field,
    // in preparation to reverse and align
    // with the neutral wall stake
    chassis.turnToHeading(270, 500);

    // STOP intake
    intake.brake();

    lemlib::Pose align_wall_stake_pose = chassis.getPose();

    // come back, in prep to turn to "bottom
    // neutral wall stake
    chassis.moveToPoint(
        2.379
        , align_wall_stake_pose.y
        , 1000
    );

    // extend arm
    arm.set_pos(arm.DUNK_POS);

    // turn to alliance stake
    chassis.turnToHeading(0, 1000);
    waitd;

    /**
     * score on alliance stake + get third red ring on field
     * (red positive)
     */

    // start intaking again, now that arm is up
    intake.intake();

    // RAM into wall, to get the third red ring on field
    chassis.moveToPoint(
        2.379
        , -63.273
        , 500
        , {
            .forwards = false
        } 
    );
    waitd;

    // score!
    arm.set_pos(arm.SCORE_POS);
    // wait to score
    pros::delay(1000);
    
    // move back
    chassis.moveToPoint(
        2.379
        , -47.337
        , 1000
    );
    waitd;

    // turn down, starts retracting arm now
    // that we're safe
    arm.set_pos(arm.INIT_POS);
    chassis.turnToHeading(90, 1000);
    waitd;

    /**
     * gets red rings 4 - 6 (red positive)
     */

    // gets other rings, sLOOWER
    chassis.moveToPoint(
        -59.811
        , -46.948
        , 5000
        , {
            .forwards = false
            , .maxSpeed = 40.0
        }
    );
    waitd;

    /**
     * gets seventh red ring on field (red positive)
     */
    turnAndMoveToPoint(
        -47.126
        , -58.749
        , {
            .turnTO = 100
            , .turnDir = AngularDirection::CCW_COUNTERCLOCKWISE
            , .moveTO = 1000
            , .forwards = false
        }
    );

    /**
     * puts mogo into positive corner
     */

    // turns towards red positive corner of field and scores mogo
    // NOTE: OVERSHOOTS INTENTIONALLY
    turnAndMoveToPoint(
        -66.005
        , -65.637
        , {
            .turnTO = 750
            , .turnDir = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
            , .moveTO = 750
        }
    );
    // afterwards, releases mogo
    mogo.release();


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