#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

#define TO 1200 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()

void red_right_side() {
    // start: 160.5, -60.5
    chassis.setPose(-149.5, -28.5, 180);

    chassis.moveToPose(-149.5, 5.5, 180, TO, {false, 30});
    waitd;
    chassis.moveToPose(-149.5, -28.5, 180, TO, {true,30});
    waitd;
    chassis.turnToPoint(-120.5, 5, TO, {false, AngularDirection::CW_CLOCKWISE, 30});
    waitd;
    intake.lift(true);
    chassis.moveToPoint(-135.131, -10.679, TO, {false, 30});
    waitd;
    intake.lift(false);
    pros::delay(500);
    intake.intake();
    pros::delay(750);
    intake.brake();
    // chassis.moveToPoint(-120.782, -0.817, TO, {false, 30});
    // waitd;
    mogo.toggle();
    chassis.turnToPoint(-180, -9, TO, {true, AngularDirection::CW_CLOCKWISE, 30});
    waitd;
    chassis.moveToPoint(-180, -9, TO, {true, 50});
    waitd;
    intake.intake();

};