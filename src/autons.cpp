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

// void prog_skills() {
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
//     // turns towards first mogo
//     chassis.turnToPoint(-47, -24, 750);
//     waitd;
//     // MOVES towards first mogo
//     chassis.moveToPoint(-47, -24, 1000);
//     waitd;

//     // captures mogo; waits 500ms for it to settle
//     mogo.toggle();
//     pros::delay(500);

//     // intakes preload ring and waits
//     intake.intake();
//     pros::delay(1500);
//     intake.brake();

//     // turns & moves towards first ring on field, running intake as you go
//     chassis.turnToPoint(-19.5, -23.5, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE });
//     waitd;
//     intake.intake();
//     chassis.moveToPoint(-19.5, -23.5, 1000, { .forwards = false });
//     waitd;
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards second ring on field, STILL running intake!
//     chassis.turnToPoint(-24.25, -50.5, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE});
//     waitd;
//     chassis.moveToPoint(-24.25, -50.5, 1000, { .forwards = false });
//     waitd;
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards third ring on field, STILL running intake!
//     chassis.turnToPoint(-52.35, -46.25, 1000, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE});
//     waitd;
//     chassis.moveToPoint(-52.35, -46.25, 1000, { .forwards = false });
//     waitd;
//     // waits for ring to be intaked
//     pros::delay(1500);

//     // turns & moves towards fourth ring on field, STILL running intake!
//     chassis.turnToPoint(-62.35, -48.4, 1000, { .forwards = false, .direction = AngularDirection::CCW_COUNTERCLOCKWISE});
//     waitd;
//     chassis.moveToPoint(-62.35, -48.4, 1000, { .forwards = false });
//     waitd;
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
//     chassis.moveToPoint(-47, 4, 1000)

    

    

//     // // turns AND moves towards the second ring on field WHILE intaking
//     // chassis.turnToPoint(-19.5, -47.5, 750, { .forwards = false, .direction = AngularDirection::CW_CLOCKWISE });
//     // waitd;
//     // chassis.moveToPoint(-19.5, -47.5, 750, { .forwards = false });
//     // waitd;
//     // // waits for ring to be intaked
//     // pros::delay(1500);
// }

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

    // added new point to move towards, so we can get a 
    // better angle when moving towards and capturing the mogo
    chassis.moveToPoint(47.474, -36.3, TO);

    // SWINGS towards mogo; locking RIGHT bc turning RIGHT!
    chassis.swingToPoint(23.5, -23.5, DriveSide::RIGHT, TO);
    waitd;
    // moves towards where mogo is
    // 23.5, -23.5 -> robot will be CENTERED on mogo position
    // WAS at 21.5, -21.5 but moved less forward so resetting to 23.5
    // overshoots a bit (22.5, -22.5) to ensure correct orientation of the mogo!
    // MIGHT NEED TO SLOW DOWN NEAR END! -> go faster so that mogo doesn't have time to vibrate
    chassis.moveToPoint(17.5, -19.5, 2000, {.maxSpeed = 90});
    waitd;
    // funny number we calculated with pythagorean theorem,
    // then tuned
    // 20.8 WITH the moving forward at beginning, only hits corner of mogo
    chassis.waitUntil(21.5);
    // retracts mogo mech
    mogo.toggle();
    // waits for movement to finish
    waitd;
    // waits for mogo to be fully captured
    pros::delay(500);

    // moves back so preload ring doesn't hit ladder when intake runs
    chassis.moveToPoint(23.5, -23.5, 1000, {.forwards = false});
    waitd;
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


    // VERY INCONSISTENT AND OFTEN ENDS UP INTAKING RED RING!
    // // opens intake piston cuz we're getting the TOP ring
    // intake.lift(1);
    // // // turns around and CURVES towards the second ring (the BLUE TOP red bottom stack)
    // // // it is the FORWARD portion of the bot in this segment
    // // // y = -5 instead of y = 0 so we don't accidentally collide with opposing team ig?
    // // // DECREASE TIMEOUT ONCE TUNED
    // // chassis.moveToPose(47, -5, 0, 3000, {.forwards = false});
    // // turns towards second ring
    // chassis.turnToPoint(47, -5, 1000, {.forwards = false});
    // waitd;
    // // move towards second ring -> MIGHT CROSS INTO TEAMMATE'S AREA?
    // chassis.moveToPoint(47, -5, 1500, {.forwards = false});
    // waitd;
    // // lifts intake down
    // intake.lift(0);
    // // waits for that to finish
    // pros::delay(500);
    // // starts intake running, to get TOP ring
    // intake.intake();
    // // waits for a bit to make sure the ring is intaked!
    // // LOWER DELAY BC OF HIGHER TIMEOUT ABOVE; CHANGE AS NECESSARY!
    // pros::delay(500);

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
<<<<<<< HEAD
    // mogo.toggle();
=======
    mogo.toggle();
>>>>>>> 967d8ce342ed8ea90441c242cfc35a6766c7b8cb
    // waits for a bit to make sure mogo was fully uncaptured
    pros::delay(500);
    // // stops intake -> SEE IF NECESSARY
    // intake.brake();

    // // moved bottom right from 12, -12 so that only
    // // FRONT of robot touches ladder!
    // // turns TOWARDS ladder with mogo mech front (to touch ladder
    // // with zipties)
    // // 16 -16, literally where the mogo was for some reason
    // chassis.turnToPoint(14, -14, 1000);
    // // MOVES towards ladder!
    // chassis.moveToPoint(14, -14, 1000);

    // // // turns around so mogo side (with zipties) is facing ladder,
    // // // AND moves towards the ladder
    // // chassis.moveToPose(-12, -12, 315, 3000);
    // // waitd;

    // // toggle hang to touch ladder bc zipties are not reliable 
    // hang.toggle();
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

    // added new point to move towards, so we can get a 
    // better angle when moving towards and capturing the mogo
    chassis.moveToPoint(-47.474, -36.3, TO);

    // SWINGS towards mogo; locking LEFT bc TURNING left!
    chassis.swingToPoint(-23.5, -23.5, DriveSide::LEFT, TO);
    waitd;
    // moves towards where mogo is
    // -23.5, -23.5 -> robot will be CENTERED on mogo pos!
    // overshoots a bit to ensure correct orientation of the mogo!
    // CONSIDER USING MOVETOPOSE? only if it doesn't go straight
    // go fast forward and CLAMP
    // minSpeed 90 might be too small?
    // -23.5, -23.5 -> orig (W/O OVERSHOOT!)
    // -19.5, -19.5 -> moves too left
    // -17.5, -19.5 -> hopefully angle adjusted so mogo
    chassis.moveToPoint(-17.5, -19.5, 1500, {.minSpeed = 90});
    // funny number we calculated with pythagorean theorem,
    // then tuned
    // 20.8 WITH the moving forward at beginning, only hits corner of mogo
    chassis.waitUntil(21.5);
    // captures mogo
    mogo.toggle();
    waitd;
    // waits for mogo to be fully captured -> might be useless since we have the waituntil
    pros::delay(500);

    // moves back so preload ring doesn't hit ladder when intake runs
    chassis.moveToPoint(-23.5, -23.5, 1000, {.forwards = false});
    waitd;
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
    // 1500 -> higher chance of blue ring being intaked
    // 1250 -> higher chance of robot running off before ring intaked
    //      - WHY IS IT DOING THIS SOMETIMES AND WORKING PERFECTLY OTHER TIMES? 
    pros::delay(1500);
    // OUTTAKE intake in case the blue ring is accidentally intaked too!
    intake.outtake();

    /* used with very last command for SIMPLE version of auton */
    // // turns around (which is equivalent to turning towards initial
    // // mogo position) -> this is to make sure mogo is released NOT
    // // in path of bot to ladder
    // chassis.turnToPoint(-23.5, -23.5, 1000, {.forwards = false});
    // // lets go of mogo (now the mogo is facing opposite direction!)
    // mogo.toggle();
    // // wait 500 ms to make sure it was fully uncaptured
    // pros::delay(500);

    // NOT GOING FOR THIRD RING; will first tune this (fri)
    // // opens intake piston cuz we're getting the TOP ring
    // intake.lift(1);
    // // turns around and CURVES towards the second ring (the RED TOP blue bottom stack)
    // // it is the FORWARD portion of the bot in this segment
    // // y = -5 instead of y = 0 so we don't accidentally collide with opposing team ig?
    // chassis.moveToPose(-47, -5, 0, 2500, {.forwards = false});
    // waitd;
    // // lifts intake down and starts intake running
    // intake.lift(0);
    // intake.intake();
    // // waits for a bit to make sure the ring is intaked!
    // // LOWER DELAY BC OF HIGHER TIMEOUT ABOVE; CHANGE AS NECESSARY!
    // pros::delay(500);

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

    // stops intake before it hits the ladder!
    intake.brake();

    // // turns around so mogo side (with zipties) is facing ladder,
    // // AND moves toward the ladder
    // // moving a bit LESS forward because the THIRD ziptie passes (so like a eighth of an inch should average out
    // // to one of the zipties hitting the ladder)
    // chassis.moveToPose(-12.25, -12.25, 45, 3000);
    // waitd;
    
    // // lifts up hang at the end bc the zipties are not reliable
    // hang.toggle();
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

// FROM SEP 5 COMMIT -> WORKS (kinda) WITHOUT MOGO OVERSHOOT AND 
// MOVE FORWARD THEN TURN FOR MOGO!
// - IF ABOVEMENTIONED MODS DONT WORK REVERT TO THAT COMMIT!
// issue with overshoot & move forward vers:
// hooks hit mogo! (mogo is tilted TOO FAR TOWARDS ROBOT when clamped)
// OVERSHOOT AND MVWFWD NEED TO BE TUNED!
void red_left_side() { 
    std::cout << "Running RED left side auton" << std::endl;
    
    // starts at the SECOND-TO-LEFT tile from the top left, at the TOP RIGHT corner, facing RIGHT
    chassis.setPose(-55.25, 40, 90);

    // moves SLIGHTLY forward, so will be in correct position
    // to turn to correct angle towards mogo
    chassis.moveToPoint(-47, 40, TO);
    
    // -21.25, 20.5; 23.5 waitUntil -> HITS THE LADDER!
    // -  - so decreasing everything by 2 inches...?
    // -  - -23.5, 23.5 -> RIGHT ON MOGO! ; 21.5 waitUntil; DOES NOT CLAMP
    // SWINGS towards mogo
    chassis.swingToPoint(-23.5, 23.5, DriveSide::RIGHT, 750);
    waitd;
    // NOTE: can probably work with .maxSpeed = 100, but better to err on the side of caution
    // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
    // overshoots a bit
    chassis.moveToPoint(-23.5, 23.5, 750, {.maxSpeed = 90});
    // waits until robot AT mogo (NOT full path)
    chassis.waitUntil(22.5);
    // captures mogo
    mogo.toggle();
    // waits for rest of motion to finish
    waitd;
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
    intake.outtake();

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

    // waits 500ms in case blue ring accidentally stuck

    // // turns MOGO away from ladder
    // chassis.turnToPoint(-9, 12, 750, {.forwards = false});
    // // releases mogo -> not doing bc still scored!
    // mogo.toggle();
    // waits a bit for mogo mech to fully open
    pros::delay(500);

    // STOPS intake incase rings get stuck
    intake.brake();
    // MOVES and turns so it will be facing towards ladder yes yes
    // -11, 9
    // DO NOT CHANGE LADDER APPROACH PT FROM -9, 12;
    // THIS SEEMS TO BE WORKING FINE
    // chassis.moveToPose(-9, 12, 135, 2500, {.lead = 0.7, .minSpeed = 80});
    // chassis.moveToPose(-9, 12, 135, 2500, {.minSpeed = 80});
    // IF NOT RELEASING MOGO USE A PT SHIFTED RIGHT 2 UP 2! (-7, 14)
    // chassis.moveToPose(-7, 14, 135, 2500, {.minSpeed = 80});
    // waitd;

    // // toggle hang to touch ladder bc zipties are not reliable 
    // hang.toggle();
};

// FROM SEP 5 COMMIT -> WORKS (kinda) WITHOUT MOGO OVERSHOOT AND 
// MOVE FORWARD THEN TURN FOR MOGO!
// - IF ABOVEMENTIONED MODS DONT WORK REVERT TO THAT COMMIT!
// issue with overshoot & move forward vers:
// hooks hit mogo! (mogo is tilted TOO FAR TOWARDS ROBOT when clamped)
// OVERSHOOT AND MVWFWD NEED TO BE TUNED!
void red_left_side_no_ladder() { 
    std::cout << "Running RED left side auton" << std::endl;
    
    // starts at the SECOND-TO-LEFT tile from the top left, at the TOP RIGHT corner, facing RIGHT
    chassis.setPose(-55.25, 40, 90);

    // moves SLIGHTLY forward, so will be in correct position
    // to turn to correct angle towards mogo
    chassis.moveToPoint(-47, 40, TO);
    
    // -21.25, 20.5; 23.5 waitUntil -> HITS THE LADDER!
    // -  - so decreasing everything by 2 inches...?
    // -  - -23.5, 23.5 -> RIGHT ON MOGO! ; 21.5 waitUntil; DOES NOT CLAMP
    //       - at this point it's going too far right though
    //       - 23.5 waitUntil too long; 22.5 waitUntil too short try 
    //         - 23 waitUntil only works when approach point -24.5, 23.5
    // SWINGS towards mogo
    chassis.swingToPoint(-26.25, 25, DriveSide::RIGHT, 750);
    waitd;
    // NOTE: can probably work with .maxSpeed = 100, but better to err on the side of caution
    // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
    // overshoots a bit
    chassis.moveToPoint(-26.25, 25, 1000, {.maxSpeed = 90});
    // waits until robot AT mogo (NOT full path)
    // waitUntil 22.5 -> 23.5 so more like blue right (which is more reliable)
    chassis.waitUntil(22.5);
    // captures mogo
    mogo.toggle();
    // waits for rest of motion to finish
    waitd;
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
    intake.outtake();

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

    // // turns MOGO away from ladder
    // chassis.turnToPoint(-9, 12, 750, {.forwards = false});
    // // releases mogo -> not doing bc still scored!
    // mogo.toggle();
    // waits a bit for mogo mech to fully open / in case blue ring accidentally stuck
    // waits a bit longer so that while turning it doesn't outtake and then blue ring gets stuck in front of robot 
    // while it's moving to the last ring
    pros::delay(750);

    // continuing to intake bc there's a 1% chance red ring hasn't been intaked yet,
    // but a 2938402384039823% chance the blue ring is still in intake

    // lifts intake in preparation to get the red ring on TOP of the stake
    intake.lift(1);
    // turns towards the last red ring (on top of stack, on red side)
    // -47, 0 ON RING but TOO FAR
    // -43, 4 still too far, also a bit too much right
    // -40.5, 13.5 going a bit too far still (is this a timeout issue?)
    // -37, 19 -> bit too far right, but PERFECT point!
    // WORKS FINE!
    chassis.turnToPoint(-36, 18.25, 750, {.forwards = false});
    waitd;
    // moves towards the last red ring (on top of stack, on red side)
    // -47, 0 ON RING but TOO FAR
    // -43, 4 still too far, also a bit too much right
    // -40.5, 13.5 going a bit too far still (is this a timeout issue?)
    // -37, 19 -> bit too far right, but PERFECT point!
    chassis.moveToPoint(-36, 18.25, 2000, {.forwards = false});
    waitd;
    // starts intake
    intake.intake();
    // unlifts intake
    intake.lift(0);
    // wait a bit for the ring to be intaked
    pros::delay(1000);

    // turns on outtake so blue ring not intaked on accident!
    // intake.outtake();
    // move back!
    chassis.moveToPoint(-30.5, 28.5, 750);
    waitd;
    // waits a bit longer so blue ring fully out
    pros::delay(750);

    // stops intake at end of autons
    intake.brake();


    // // MOVES and turns so it will be facing towards ladder yes yes
    // // -11, 9
    // // DO NOT CHANGE LADDER APPROACH PT FROM -9, 12; -> changed bc w/o releasing mogo the position is off
    // // THIS SEEMS TO BE WORKING FINE
    // // chassis.moveToPose(-9, 12, 135, 2500, {.lead = 0.7, .minSpeed = 80});
    // // chassis.moveToPose(-9, 12, 135, 2500, {.minSpeed = 80});
    // // IF NOT RELEASING MOGO USE A PT SHIFTED RIGHT 2 UP 2! (-7, 14)
    // chassis.moveToPose(-7, 14, 135, 2500, {.minSpeed = 80});
    // waitd;

    // // toggle hang to touch ladder bc zipties are not reliable 
    // hang.toggle();
};



void blue_right_side() {    
    std::cout << "Running BLUE right side auton" << std::endl;

    // starts at the SECOND-TO-LEFT tile from the top-right, at the TOP-LEFT corner, facing left
    chassis.setPose(55.25, 40, 270);

    // moves a bit forward, so it will be in the correct position
    // to turn to correct angle towards the mogo
    chassis.moveToPoint(47, 40, TO);

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
    // chassis.swingToHeading(235, DriveSide::LEFT, 750);
    
    chassis.swingToPoint(21.25, 20.5, DriveSide::LEFT, 750);

    // moves towards that slightly off angle point, so that the robot
    // hits the mogo correctly
    // overshoots faster!
    chassis.moveToPoint(21.25, 20.5, 750, {.minSpeed = 90});
    // waits until robot AT mogo (NOT waiting for full path!)
    chassis.waitUntil(23.5);
    // captures mogo
    mogo.toggle();
    // waits for rest of motion to finish
    waitd;
    // wait for mogo mech to fully retract
    pros::delay(500); 
    /**
      * OLD MOGO GETTING SOLUTION */
    // // goes aggressively towards mogo (OVERSHOOTS A BIT TO FORCE MOGO INTO CORRECT ORIENTATION)
    // // TODO: MIGHT NEED TO MOVE A BIT FARTHER!
    // // maxSpeed 80 -> 60
    // chassis.moveToPoint(24.25, 22.5, 1000, {.maxSpeed = 60});
    // waits for LESS time than the motion takes to complete (so that the mogo can toggle WHILE the 
    // bot is moving!)

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
    // turns towards fourth ring
    // 19, 51
    chassis.turnToPoint(23.5, 47.75, 1000, {.forwards = false});
    waitd;

    // resume intaking (to get fourth ring!)
    intake.intake();
    // move towards fourth ring and intake it
    // 18, 50
    chassis.moveToPoint(23.5, 47.75, 1250, {.forwards = false, .maxSpeed = 60});
    waitd;
    // wait a bit to intake the ring, and then move in preparation to touch the ladder
    pros::delay(1250);
    // turns on outtake so blue ring doesn't get intaked on accident
    intake.outtake();

    // turns MOGO away from ladder
    chassis.turnToPoint(9, 12, 750, {.forwards = false});
    // releases mogo
    // mogo.toggle();
    // waits a bit for mogo mech to fully open
    pros::delay(500);

    // STOPS intake incase rings get stuck
    intake.brake();
    // // MOVES and turns so it will be facing towards ladder yes yes
    // // GO HALF AN INCH FARTHER
    // // 9, 12
    // chassis.moveToPose(8, 11, 225, 2500, {.lead = 0.7, .minSpeed = 80});
    // waitd;

    // // toggle hang to touch ladder bc zipties are not reliable 
    // hang.toggle();
};