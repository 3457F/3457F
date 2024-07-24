#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/imu.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <ctime>

#define TO 1200 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()

void red_right_side() {
    // start: 160.5, -60.5
    chassis.setPose(-149.5, -28.5, 180);
    //setting thingys up!
    chassis.moveToPose(-149.5, 0, 180, TO, {false, 0, 0.6, 100, 80});
    // moving ring out of the way. WEE!!!
    waitd;
    chassis.moveToPose(-149.5, -32.5, 180, TO, {true,0, 0.6, 100, 80});
    //coming back to orgin
    waitd;
    chassis.turnToPoint(-120.5, 5, TO, {false, AngularDirection::CW_CLOCKWISE, 100, 80});
    //turning to double stack. (Double PLAY!!!!)
    waitd;
    intake.lift(true);
    //intake lifting up to get top ring since we are red
    chassis.moveToPoint(-135.131, -10.679, TO, {false, 30});
    //moving to the double stack
    waitd;
    intake.lift(false);
    // moving down since how it works
    pros::delay(250);
    intake.intake();
     chassis.moveToPoint(-105.131, -20.679, TO, {false, 30});
     //moving to ensure the ring gets eaten by the intake
     waitd;
    pros::delay(500);
    intake.brake();
    // chassis.moveToPoint(-120.782, -0.817, TO, {false, 30});
    // waitd;
    chassis.turnToPoint(-180, -9, 750, {true, AngularDirection::CW_CLOCKWISE, 70});
    //turning to alliacne color wall stake
    waitd;
    // chassis.turnToHeading(270,TO);
    waitd;
    chassis.moveToPose(-180, -15, 270, TO, {true,0, 0.6, 50});
    //going to alliacne stake
    waitd;
    intake.intake();
    //intake moving to score preloaded ring (PLS WORK RAHHHHH)
    pros::delay(TO);
    chassis.moveToPose(-140, -17, 270,TO, {false});
    // moving back to ensure if it misses the ring doesn't break chain of the intake
    waitd;
    // bool MogoMech = true;
    // chassis.turnToHeading(123,TO);
    chassis.moveToPose(-80.548,-65, 120, TO, {true,2, 0.6, 80, 50});
    // moving to mogo to pick up
     // hope mogo go boom
    waitd;
    pros::delay(500);
    // turn to ring we abt to score
    intake.intake();
    waitd;
    chassis.turnToHeading(0, TO);
    waitd;
    chassis.moveToPoint(-70, -120, TO, {false, 100});
};

void red_left_side(){
    // chassis.setPose(-149.849,22.632,0); 
    // //setting bozos up
    // lemlib::MoveToPoseParams();
    // chassis.moveToPose(-148.849, -27.364, 0, TO, {false, 2 ,0.6 ,128 ,80});
    // //moving bozo out of the way
    // waitd;
    // chassis.moveToPose(-167.175, -2.119, 270, TO, {false});
    
    
    chassis.setPose(-54.513, 43.502, 90);
    //setting things up
    chassis.swingToHeading(120 , DriveSide::LEFT, 500);
    //turning towards the mogo
    waitd;
    mogo.toggle();
    chassis.moveToPoint(-20.993, 19.531, TO, {true, 80, 0, 6});
    waitd;
    chassis.swingToHeading(105, DriveSide::LEFT, 250);
    waitd;
    mogo.toggle();
    //getting the mogo
    intake.intake();
    //scoring preload
    chassis.turnToPoint(-23.332, 53.432, TO, {false});
    waitd;
    //turning to next ring
    chassis.moveToPoint(-23.332, 45.432, TO, {false});
    waitd;
    //moving the next ring and scoring
    pros::delay(150);
    chassis.swingToPoint(-3.453, 53,DriveSide::LEFT, TO, {false});
    waitd;
    //moving to center rings
    chassis.moveToPoint(-9.3, 53, TO, {false});
    waitd;
    //grabbing center rings
    chassis.swingToHeading(-10, DriveSide::RIGHT, 850);
    waitd;
    chassis.moveToPoint(-5.207, 44.307, TO, {false, 60});
    waitd;
    //getting second center ring
    pros:pros::c::delay(500);
    //going to double stack
    chassis.moveToPose(-50.031, -2.978, 225, TO, {false});
    waitd;
    intake.lift(true);
    

}
