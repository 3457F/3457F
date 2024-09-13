#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "mogo.hpp"
#include "pros/imu.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <ctime>

#define TO 300 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()

/* OLD BLUE LEFT SIDE */
// void blue_left_side() {
//     std::cout << "Running BLUE left side auton" << std::endl;

//     // WORKS PERFECTLY (hits the ring a bit but this calibration is amazing)
//     // x: 61.638 ; robot at BACK of tile
//     chassis.setPose(55.597, -36.303, 270);

//     // GOING TO CAPTURE MIDDLE MOGO
//     // x: 11.358; hits the mogo a bit
//     // x: 12 -> works fine but crosses central barrier
//     chassis.moveToPose(17.399, -36.303, 270, TO);
//     waitd;
//     // 0.542 -47.704 -> goes WAYYY too far
//     // 6.778 -41.663 -> mogo mech slips OFF of mogo
//     chassis.turnToPoint(5.511, -44.294, TO);
//     waitd;
//     chassis.moveToPose(5.511, -44.294, 235, TO, {.maxSpeed = 72});
//     waitd;
//     // toggles mogo mech and waits for it to fully retract
//     mogo.toggle();
//     // might need to reduce timeout to reduce chances of crossing autonomous line
//     pros::delay(500);

//     // starts moving while leaving the premises so less chance of being caught crossing the autonomous barrier
//     chassis.moveToPose(17.399, -36.303, 235, TO, {.forwards = false});
//     // move waitd down so intake runs while moving back...?
//     waitd;

//     // instantly starts intake, and waits for preload to be fully scored
//     intake.intake();
//     pros::delay(500);

    
// };

// void blue_left_side() {
//     std::cout << "Running BLUE left side auton" << std::endl;

//     chassis.setPose(55.6, -36.3, 270);

//     chassis.moveToPose(41, -36.3, 270, 1800);
//     waitd;

//     chassis.turnToHeading(305, 1200);
//     waitd;

//     chassis.moveToPoint(27, -26, TO);
//     waitd;

//     mogo.toggle();
// }

void blue_left_side() {
    std::cout << "Running BLUE left side auton" << std::endl;

    chassis.setPose(55.6, -36.3, 270);

    // SWINGS towards mogo; locking RIGHT bc turning RIGHT!
    chassis.swingToPoint(23.5, -23.5, DriveSide::RIGHT, TO);
    waitd;
    // moves towards where mogo is
    // 23.5, -23.5 -> robot will be CENTERED on mogo position
    // overshoots a bit (22.5, -22.5) to ensure correct orientation of the mogo!
    // MIGHT NEED TO SLOW DOWN NEAR END!
    chassis.moveToPoint(21.5, -21.5, 2000, {.maxSpeed = 70});
    waitd;
    // retracts mogo mech
    mogo.toggle();
    // waits for mogo to be fully captured
    pros::delay(500);

    // starts intake
    intake.intake();
    // waits for preload ring to go in
    // waiting for longer because need to ensure the ring goes in correctly!
    pros::delay(1500);

    // turns AND moves towards first blue ring WITH INTAKE SIDE IN FRONT
    chassis.moveToPose(23.5, -47.5, 180, 1000, {.forwards = false});
    waitd;
    // waits a bit for the ring to be intaked!
    // waiting for longer because need to ensure the ring goes in correctly!
    pros::delay(1500);
    // STOPS intake in case blue ring is accidentally intaked too...?
    // -> MIGHT REMOVE
    intake.brake();

    // opens intake piston cuz we're getting the TOP ring
    intake.lift(1);
    // // turns around and CURVES towards the second ring (the BLUE TOP red bottom stack)
    // // it is the FORWARD portion of the bot in this segment
    // // y = -5 instead of y = 0 so we don't accidentally collide with opposing team ig?
    // // DECREASE TIMEOUT ONCE TUNED
    // chassis.moveToPose(47, -5, 0, 3000, {.forwards = false});
    // turns towards second ring
    chassis.turnToPoint(47, -5, 1000, {.forwards = false});
    waitd;
    // move towards second ring -> MIGHT CROSS INTO TEAMMATE'S AREA?
    chassis.moveToPoint(47, -5, 1500, {.forwards = false});
    waitd;
    // lifts intake down
    intake.lift(0);
    // waits for that to finish
    pros::delay(500);
    // starts intake running, to get TOP ring
    intake.intake();
    // waits for a bit to make sure the ring is intaked!
    // LOWER DELAY BC OF HIGHER TIMEOUT ABOVE; CHANGE AS NECESSARY!
    pros::delay(500);

    // keeps intake running in case the ring didn't get intaked
    // during the previous steps

    // // moves back to a initial mogo point, with mogo mech
    // // facing AWAY from the ladder, so mogo can be released AWAY
    // // from path of robot when it goes towards the ladder!
    // chassis.moveToPose(23.5, -23.5, 315, 1500, {.forwards = false});
    // waitd;
    // turns towards mogo point
    chassis.turnToPoint(21.5, -21.5, 500);
    // moves towards mogo point
    chassis.moveToPoint(21.5, -21.5, 1000);

    // turns away from ladder WITH MOGO MECH FRONT (to release mogo
    // away from the path btwn bot -> ladder)
    chassis.turnToPoint(12, -12, 1000, {.forwards = false});
    // releases mogo
    mogo.toggle();
    // waits for a bit to make sure mogo was fully uncaptured
    pros::delay(500);
    // // stops intake -> SEE IF NECESSARY
    // intake.brake();

    // turns TOWARDS ladder with mogo mech front (to touch ladder
    // with zipties)
    chassis.turnToPoint(12, -12, 1000);
    // MOVES towards ladder!
    chassis.moveToPoint(12, -12, 1000);

    // // turns around so mogo side (with zipties) is facing ladder,
    // // AND moves towards the ladder
    // chassis.moveToPose(-12, -12, 315, 3000);
    // waitd;
}

// void red_right_side() {
//     std::cout << "Running RED right side auton" << std::endl;
    
//     chassis.setPose(-58.5, -35.719, 90);

//     chassis.moveToPoint(-20.019, -36.498, TO);
//     waitd;
//     chassis.swingToHeading(135, DriveSide::RIGHT, 650);
//     waitd;
//     chassis.moveToPoint(-9.105, -42.929, 500);
//     waitd;
//     mogo.toggle();

//     pros::delay(100);

//     intake.intake();
//     chassis.turnToPoint(-23.332, -47.217, TO, {false});
//     waitd;
//     chassis.moveToPoint(-23.332, -47.217, TO, {false});
// };

/**
 * CURRENTLY WORKING:
 * - one ring, one mogo, touches ladder!
 */

void red_right_side() {
    // mogo starts automatically toggled open!

    std::cout << "Running RED right side auton" << std::endl;

    chassis.setPose(-55.6, -36.3, 90);

    // SWINGS towards mogo; locking LEFT bc TURNING left!
    chassis.swingToPoint(-23.5, -23.5, DriveSide::LEFT, TO);
    waitd;
    // moves towards where mogo is
    // -23.5, -23.5 -> robot will be CENTERED on mogo pos!
    // overshoots a bit to ensure correct orientation of the mogo!
    // CONSIDER USING MOVETOPOSE? only if it doesn't go straight
    chassis.moveToPoint(-23.5, -23.5, 1500);
    waitd;
    // captures mogo
    mogo.toggle();
    // waits for mogo to be fully captured
    pros::delay(500);

    // starts intake
    intake.intake();
    // waits for preload ring to go in
    // waiting for longer because need to ensure the ring goes in correctly!
    pros::delay(1500);

    // turns towards first red ring W/ INTAKE
    chassis.turnToPoint(-23.5, -47.5, 1000, {.forwards = false});
    waitd;
    // moves towards first red ring; INTAKE FORWARD OF ROBOT
    // (blue top, RED BOTTOM); INTAKE STILL RUNNING
    chassis.moveToPoint(-23.5, -47.5, 1000, {.forwards = false});
    waitd;
    // waits a bit for the ring to be intaked!
    // waiting for longer because need to ensure the ring goes in correctly!
    pros::delay(1500);
    // STOP intake in case the blue ring is accidentally intaked too!
    intake.brake();

    /* used with very last command for SIMPLE version of auton */
    // // turns around (which is equivalent to turning towards initial
    // // mogo position) -> this is to make sure mogo is released NOT
    // // in path of bot to ladder
    // chassis.turnToPoint(-23.5, -23.5, 1000, {.forwards = false});
    // // lets go of mogo (now the mogo is facing opposite direction!)
    // mogo.toggle();
    // // wait 500 ms to make sure it was fully uncaptured
    // pros::delay(500);

    // opens intake piston cuz we're getting the TOP ring
    intake.lift(1);
    // turns around and CURVES towards the second ring (the RED TOP blue bottom stack)
    // it is the FORWARD portion of the bot in this segment
    // y = -5 instead of y = 0 so we don't accidentally collide with opposing team ig?
    chassis.moveToPose(-47, -5, 0, 2500, {.forwards = false});
    waitd;
    // lifts intake down and starts intake running
    intake.lift(0);
    intake.intake();
    // waits for a bit to make sure the ring is intaked!
    // LOWER DELAY BC OF HIGHER TIMEOUT ABOVE; CHANGE AS NECESSARY!
    pros::delay(500);

    // keeps intake running in case the ring didn't get intaked
    // during the previous steps

    // moves back to a initial mogo point, with mogo mech
    // facing AWAY from the ladder, so mogo can be released AWAY
    // from path of robot when it goes towards the ladder!
    chassis.moveToPose(-23.5, -23.5, 45, 1500, {.forwards = false});
    // release mogo
    mogo.toggle();
    // waits for a bit to make sure mogo was fully uncaptured
    pros::delay(500);

    // turns around so mogo side (with zipties) is facing ladder,
    // AND moves toward the ladder
    chassis.moveToPose(-12, -12, 45, 3000);
    waitd;
}

// void red_left_side() {
//     // mogo starts automatically toggled open!

//     std::cout << "Running RED left side auton" << std::endl;
    
//     // starts at the SECOND-TO-LEFT tile from the top left, at the TOP RIGHT corner, facing RIGHT
//     chassis.setPose(-55.25, 40, 90);

//     // SWINGS towards mogo
//     chassis.swingToPoint(-24.25, 22.5, DriveSide::RIGHT, 750);
//     waitd;
//     // NOTE: can probably work with .maxSpeed = 100, but better to err on the side of caution
//     // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
//     chassis.moveToPoint(-24.25, 22.5, 750, {.maxSpeed = 70});
//     waitd;
//     // captures mogo
//     mogo.toggle();
//     // wait for mogo mech to fully retract
//     pros::delay(500);

//     // starts intake
//     intake.intake();
//     // waits for preload ring to go in
//     pros::delay(500);

//     // moves into FIRST SET OF RINGS, aims to intake just one ring
//     chassis.moveToPose(-5.6, 52, 180, 2000, {.forwards = false, .minSpeed = 72, .earlyExitRange = 10});
//     waitd;

//     // moves FURTHER, to get the second ring in the set -> THIS ONE IS MOST INACCURATE; moving SLOWLY
//     // OVERSHOOTS BY A LOT TO MAKE SURE RING GOTTEN!
//     chassis.moveToPoint(-7.6, 61, 1000, {.forwards = false, .minSpeed = 40});
//     waitd;

//     // wait a bit more for the second ring to be intaked, in case the moveToPoint function already finished before 1000ms timeout
//     pros::delay(1000);

//     // we do NOT want the blue rings to get in; stop just in case the blue ring does a funny
//     intake.brake();

//     // moves all the way back from the ring, in prep to turn to third ring
//     chassis.moveToPoint(-13, 30, 750);
//     waitd;

//     // turns towards fourth ring
//     chassis.turnToPoint(-19, 51, 1000, {.forwards = false});
//     waitd;

//     // resume intaking (to get fourth ring!)
//     intake.intake();
//     // move towards fourth ring and intake it
//     // -18, 50
//     chassis.moveToPoint(-19, 51, 1250, {.forwards = false, .maxSpeed = 60});
//     waitd;
//     // wait a bit to intake the ring, and then move in preparation to touch the ladder
//     pros::delay(1250);
//     // turns on outtake so blue ring doesn't get intaked on accident
//     intake.brake();

//     // turns MOGO away from ladder
//     chassis.turnToPoint(-9, 12, 750, {.forwards = false});
//     // releases mogo
//     mogo.toggle();
//     // waits a bit for mogo mech to fully open
//     pros::delay(500);

//     // STOPS intake incase rings get stuck
//     intake.brake();
//     // MOVES and turns so it will be facing towards ladder yes yes
//     // -11, 9
//     chassis.moveToPose(-9, 12, 135, 2500, {.lead = 0.7, .minSpeed = 80});
//     waitd;
// };

// FROM SEP 5 COMMIT
void red_left_side() { 
    std::cout << "Running RED left side auton" << std::endl;
    
    // starts at the SECOND-TO-LEFT tile from the top left, at the TOP RIGHT corner, facing RIGHT
    chassis.setPose(-55.25, 40, 90);

    // SWINGS towards mogo
    chassis.swingToPoint(-24.25, 22.5, DriveSide::RIGHT, 750);
    waitd;
    // NOTE: can probably work with .maxSpeed = 100, but better to err on the side of caution
    // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
    chassis.moveToPoint(-24.25, 22.5, 750, {.maxSpeed = 90});
    waitd;
    // captures mogo
    mogo.toggle();
    // wait for mogo mech to fully retract
    pros::delay(500);

    // starts intake
    intake.intake();
    // waits for preload ring to go in
    pros::delay(500);

    // moves into FIRST SET OF RINGS, aims to intake just one ring
    chassis.moveToPose(-5.6, 52, 180, 2000, {.forwards = false, .minSpeed = 72, .earlyExitRange = 10});
    waitd;

    // moves FURTHER, to get the second ring in the set -> THIS ONE IS MOST INACCURATE; moving SLOWLY
    // OVERSHOOTS BY A LOT TO MAKE SURE RING GOTTEN!
    chassis.moveToPoint(-7.6, 61, 1000, {.forwards = false, .minSpeed = 40});
    waitd;

    // wait a bit more for the second ring to be intaked, in case the moveToPoint function already finished before 1000ms timeout
    pros::delay(1000);

    // we do NOT want the blue rings to get in; stop just in case the blue ring does a funny
    intake.brake();

    // moves all the way back from the ring, in prep to turn to third ring
    chassis.moveToPoint(-13, 30, 750);
    waitd;

    // turns towards fourth ring
    chassis.turnToPoint(-19, 51, 1000, {.forwards = false});
    waitd;

    // resume intaking (to get fourth ring!)
    intake.intake();
    // move towards fourth ring and intake it
    // -18, 50
    chassis.moveToPoint(-19, 51, 1250, {.forwards = false, .maxSpeed = 60});
    waitd;
    // wait a bit to intake the ring, and then move in preparation to touch the ladder
    // changed 1250 -> 1000 ON FRI
    pros::delay(1250);
    // turns on outtake so blue ring doesn't get intaked on accident
    // changed brake -> outtake ON FRI
    intake.outtake();

    // turns MOGO away from ladder
    chassis.turnToPoint(-9, 12, 750, {.forwards = false});
    // releases mogo
    mogo.toggle();
    // waits a bit for mogo mech to fully open
    pros::delay(500);

    // STOPS intake incase rings get stuck
    intake.brake();
    // MOVES and turns so it will be facing towards ladder yes yes
    // -11, 9
    chassis.moveToPose(-9, 12, 135, 2500, {.lead = 0.7, .minSpeed = 80});
    waitd;
};



void blue_right_side(){    
    std::cout << "Running BLUE right side auton" << std::endl;

    // starts at the SECOND-TO-LEFT tile from the top-right, at the TOP-LEFT corner, facing left
    chassis.setPose(55.25, 40, 270);

    // 24.25, 22.5 INITIAL PT
    // 23.5, 23.5 ACTUAL pos of mogo...
    // 23, 24.5 "2 deg" off-pt RIGHT (rel to robot START POS)
    // 25, 22.5 "3 deg" HORIZONTALLY offset absolutely from mogo (but LEFT rel to robot start POS)

    /** NEW SOLUTION TO ACCOUNT FOR FACT THAT THE ROBOT ATTACHES TO MOGO
      * AT INCORRECT ANGLE IF IT GOES STRAIGHT FOR THE MOGO */

    // 25, 22.5 -> moved down and left (diagonally southeast) REL TO PATH JERRYIO
    // 

    /** 2nd solution: GO FAST AND CLAMP BFORE MOGO CAN REBOUND FORWARD, SO LESS BEND! */
    // -> bc otherwise LOT OF LEEWAY; it bounces back
    // SWINGS towards mogo
    // chassis.swingToPoint(24.25, 22.5, DriveSide::LEFT, 750);
    // waitd;
    // 235 deg is for point 
    chassis.swingToHeading(235, DriveSide::LEFT, 750);
    // // turns SLIGHTLY off-angle, just to make sure that 
    // chassis.turnToPoint(23, 20.5, 500);
    // moves towards that slightly off angle point, so that the robot
    // hits the mogo correctly
    // CORRECTION ON FRIDAY -> tried to set minSpeed HIGHER so it moves faster...?
    chassis.moveToPoint(23, 20.5, 750, {.minSpeed = 80});
    /**
      * OLD MOGO GETTING SOLUTION */
    // // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
    // // TODO: MIGHT NEED TO MOVE A BIT FARTHER!
    // // maxSpeed 80 -> 60
    // chassis.moveToPoint(24.25, 22.5, 1000, {.maxSpeed = 60});
    // waits for LESS time than the motion takes to complete (so that the mogo can toggle WHILE the 
    // bot is moving!)
    pros::delay(500);
    // captures mogo
    mogo.toggle();
    // waits for the rest of the movement to run
    waitd;
    // wait for mogo mech to fully retract
    pros::delay(500); 

    // starts intake
    intake.intake();
    // waits for preload ring to go in
    pros::delay(750);

    // moves into FIRST SET OF RINGS, aims to intake just one ring
    // for some reason the bot is crossing the auton line on this, so we're going
    // to move the target point a BIT away from the auton line
    // used to be 5.6 > 7.6 > 9.6 > 11.6
    // 11.6 + 1.2 for crazy funnel (too far) -> 12
    chassis.moveToPose(12, 52, 180, 2000, {.forwards = false, .lead = 0.55, .minSpeed = 72, .earlyExitRange = 10});
    // chassis.moveToPose(5.6, 52, 180, 2000, {.forwards = false, .minSpeed = 72, .earlyExitRange = 10});
    waitd;
    // 5.75, 33.3
    // chassis.moveToPose(5.6, 35.5, 315, 1000, {.forwards = false, .minSpeed = 72, .earlyExitRange = 15});
    // waitd;

    // moves FURTHER, to get the second ring in the set -> THIS ONE IS MOST INACCURATE; moving SLOWLY
    // OVERSHOOTS BY A LOT TO MAKE SURE RING GOTTEN!
    chassis.moveToPoint(12, 61, 1000, {.forwards = false, .minSpeed = 40});
    waitd;
    // wait a bit more for the second ring to be intaked, in case the moveToPoint function already finished before 1000ms timeout
    pros::delay(1000);

    // we do NOT want the blue rings to get in; outtake just in case the red ring does a funny
    intake.outtake();

    // moves all the way back from the ring, in prep to turn to third ring
    // AGAIN, UNDERSHOOTING FOR SOME REASON
    // chassis.moveToPose(13, 30, 315, 1250);

    chassis.moveToPoint(13, 30, 1000);
    waitd;

    // // // TURN RIGHT MORE
    // // turns towards fourth ring
    // // 19, 51
    // chassis.turnToPoint(23.5, 47.75, 1000, {.forwards = false});
    // waitd;

    // // resume intaking (to get fourth ring!)
    // intake.intake();
    // // move towards fourth ring and intake it
    // // 18, 50
    // chassis.moveToPoint(23.5, 47.75, 1250, {.forwards = false, .maxSpeed = 60});
    // waitd;
    // // wait a bit to intake the ring, and then move in preparation to touch the ladder
    // pros::delay(1250);
    // // turns on outtake so blue ring doesn't get intaked on accident
    // intake.brake();

    // // turns MOGO away from ladder
    // chassis.turnToPoint(9, 12, 750, {.forwards = false});
    // // releases mogo
    // mogo.toggle();
    // // waits a bit for mogo mech to fully open
    // pros::delay(500);

    // // STOPS intake incase rings get stuck
    // intake.brake();
    // // MOVES and turns so it will be facing towards ladder yes yes
    // // GO HALF AN INCH FARTHER
    // // 9, 12
    // chassis.moveToPose(8, 11, 225, 2500, {.lead = 0.7, .minSpeed = 80});
    // waitd;
};