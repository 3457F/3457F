#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "mogo.hpp"
#include "pros/imu.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <ctime>

#define TO 1200 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()

// lemlib::TurnToPointParams turnWithIntake = {
//     .forwards = false
//     , .direction = lemlib::AngularDirection::AUTO
//     , .maxSpeed = 127
//     , .minSpeed = 0
//     , .earlyExitRange = 0
// };

// lemlib::SwingToPointParams swingWithIntake = {
//     .forwards = false
//     , .direction = lemlib::AngularDirection::AUTO
//     , .maxSpeed = 127
//     , .minSpeed = 0
//     , .earlyExitRange = 0
// };

// lemlib::MoveToPointParams moveWithIntake = {
//     .forwards = false
//     , .maxSpeed = 127
//     , .minSpeed = 0
//     , .earlyExitRange = 0
// };

// lemlib::MoveToPoseParams moveWithIntakePose = {
//     .forwards = false
//     , .horizontalDrift = 5
//     , .lead = 0.6
//     , .maxSpeed = 127
//     , .minSpeed = 0
//     , .earlyExitRange = 0
// };

void red_right_side() {
    chassis.setPose(-58.5, -35.719, 90);

    chassis.moveToPoint(-20.019, -36.498, TO);
    waitd;
    chassis.swingToHeading(135, DriveSide::RIGHT, 650);
    waitd;
    chassis.moveToPoint(-9.105, -42.929, 500);
    waitd;
    mogo.toggle();

    pros::delay(100);

    intake.intake();
    chassis.turnToPoint(-23.332, -47.217, TO, {false});
    waitd;
    chassis.moveToPoint(-23.332, -47.217, TO, {false});
};

void red_left_side(){    
    std::cout << "Running left side auton" << std::endl;
    
    // start position (as per initial path) -> X: -59.386, 30.348
    chassis.setPose(-55.293, 40.092, 90);

    // go to mogo
    // chassis.turnToPoint(-30.25, 25.378, TO);
    // old pt -30.25, 25.378
    // moved up and a BIT left -30.25, 27.522
    // MOVED FORWARD -29.471; 27.132
    chassis.swingToPoint(-27.717, 26.158, DriveSide::LEFT, TO);
    waitd;
    chassis.moveToPoint(-27.717, 26.158, TO);
    waitd;
    // captures mogo
    mogo.toggle();
    // wait for mogo mech to fully retract
    pros::delay(500);

    // starts intake
    intake.intake();
    // waits for preload ring to go in
    pros::delay(250);

    // moves into first set of rings, aims to intake just one ring
    // chassis.moveToPose(-3.746, 51.493, 180, 12000, {.forwards = false});
    // chassis.moveToPose(-3.746, 52, 180, 12000, {.forwards = false});
    // change timeout; might be too big but not affecting auton rn
    // TRY NOT TO TURN WHILE INTAKING PRELOAD
    chassis.moveToPose(-3.746, 52, 180, 2500, {.forwards = false, .minSpeed = 72, .earlyExitRange = 10});
    waitd;
    // moves further forward, getting second ring
    // change timeout; might be too big but not affecting auton rn
    chassis.moveToPoint(-4.25, 60, 2000, {.forwards = false});
    waitd;

    intake.outtake();
    // chassis.moveToPoint(-8.91, 45.744, 2500);

    // moves all the way back from the ring, in prep to turn to third ring
    chassis.moveToPoint(-13, 30.153, 2500);
    // chassis.turnToPoint(-23.332, 46.718, 1000, {.forwards = false});
    chassis.turnToPoint(-22.552, 48.082, 1000, {.forwards = false});
    chassis.moveToPoint(-22.552, 48.082, 1500, {.forwards = false});

    // REVERSE INTAKE WHILE MOVING BACK
    // REVERSE INTAKE WHILE MOVING BACK
    // REVERSE INTAKE WHILE MOVING BACK
    // REVERSE INTAKE WHILE MOVING BACK
    // REVERSE INTAKE WHILE MOVING BACK

    // moves a bit forward (slower), aims to intake second (non-preloaded) ring
    // chassis.moveToPose(-3.35, )
    // chassis.turnToPoint(-9.008, 32.394, TO, turnWithIntake);
    // waitd;
    // chassis.moveToPoint(-9.008, 32.394, TO, moveWithIntake);
    // waitd;
    // // chassis.moveToPose(-5.402, 34.245, 180, TO);
    // waitd;
    // // wait for first set of rings to be intaked
    // pros::delay(250);

    // // getting two rings (the one with the blue rings on top and the RED RINGS ON THE BOTTOM)
    // // MOTION CHAINING TEST
    // chassis.moveToPose(-6, 56, 180, TO, {.minSpeed=72, .earlyExitRange=8});
    // waitd;
    
    // // getting in position to gather the third ring (the lone pair with blue ring on top and the RED RING ON THE BOTTOM)
    // // MOTION CHAINING TEST
    // chassis.moveToPose(-23.916, 35.025, 180, TO, {.minSpeed= 72, .earlyExitRange=8});
    // waitd;
    // // actually gets the red ring
    // chassis.moveToPose(-23.916, 56.852, 180, TO);
    // waitd;

    // // deploys intake piston (remember, intake STILL RUNNING!)
    // intake.lift(1);
    // // moves to final ring (another lone pair, with RED RING ON TOP THIS TIME and blue ring on bottom)
    // chassis.moveToPose(-47.303, -0, 0, TO);
    // waitd;
    // // waits for ring to be intaked
    // pros::delay(250);

    // releases mogo & lowers intake
    // mogo.toggle();
    // intake.lift(0);

    // // moves away from mogo
    // chassis.moveToPose(-47.303, 5, 0, TO);
    // waitd;

    // // stops intake
    // intake.brake();
};