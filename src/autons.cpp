#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

#define TO 1200 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()

void red_right_side() {
    // start: 160.5, -60.5
    chassis.setPose(-149.5, -28.5, 180);
    //setting thingys up!
    chassis.moveToPose(-149.5, 5.5, 180, TO, {false, 30});
    // moving ring out of the way. WEE!!!
    waitd;
    chassis.moveToPose(-149.5, -28.5, 180, TO, {true,30});
    //coming back to orgin
    waitd;
    chassis.turnToPoint(-120.5, 5, TO, {false, AngularDirection::CW_CLOCKWISE, 30});
    //turning to double stack. (Double PLAY!!!!)
    waitd;
    intake.lift(true);
    //intake lifting up to get top ring since we are red
    chassis.moveToPoint(-135.131, -10.679, TO, {false, 30});
    //moving to the double stack
    waitd;
    intake.lift(false);
    // moving down since how it works
    pros::delay(500);
    intake.intake();
     chassis.moveToPoint(-105.131, -20.679, TO, {false, 30});
     //moving to ensure the ring gets eaten by the intake
     waitd;
    pros::delay(750);
    intake.brake();
    // chassis.moveToPoint(-120.782, -0.817, TO, {false, 30});
    // waitd;
    chassis.turnToPoint(-180, -9, TO, {true, AngularDirection::CW_CLOCKWISE, 0});
    //turning to alliacne color wall stake
    waitd;
    // chassis.turnToHeading(270,TO);
    waitd;
    chassis.moveToPose(-160, -14, 270, TO, {true,30});
    //going to alliacne stake
    waitd;
    intake.intake();
    //intake moving to score preloaded ring (PLS WORK RAHHHHH)
    pros::delay(700);
    chassis.moveToPose(-140, -17, 270,TO, {false});
    // moving back to ensure if it misses the ring doesn't break chain of the intake
    waitd;
    bool MogoMech = true;
    // chassis.turnToHeading(123,TO);
    chassis.moveToPose(-100,-40, 123, TO, {true,30});
    // moving to mogo to pick up
    bool MogoMech = false;
    // hope mogo go boom
    waitd;
    chassis.turnToHeading(0, TO);
    // turn to ring we abt to score
    intake.intake();
    waitd;
    chassis.moveToPose(-57,-100, 0, TO,{false, 30});
    // going to ring (NOT WORKING RN)
 

};