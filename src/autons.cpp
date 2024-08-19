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

void red_right_side() {

};

void red_left_side(){    
    std::cout << "Running left side auton" << std::endl;
    
    // start position (as per initial path) -> X: -59.386, 30.348
    chassis.setPose(-59.386, 30.348, 90);

    // go to mogo
    chassis.moveToPose(-27.814, 26.645, 124, TO);
    waitd;

    // go to first set of rings
    chassis.moveToPose(-5.402, 34.245, 180, TO);
    waitd;

    // start intaking rings
    intake.intake();
    pros::delay(250);

    // move to second set of rings
    
};