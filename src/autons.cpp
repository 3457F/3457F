#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "util.hpp"

// declares pure pursuit files

// red nostack awp
ASSET(mecha_red_pos_awp_corner_3_txt);
ASSET(mecha_red_pos_awp_mogo_txt);
ASSET(mecha_red_pos_awp_stake_txt);
ASSET(two_ring_txt);
ASSET(red_pos_steep_corner_txt)

void print_robot_pos(void *chassisVoid) {
  lemlib::Chassis *chassis = static_cast<lemlib::Chassis *>(chassisVoid);

  while (true) {
    lemlib::Pose pos = chassis->getPose();

    std::cout << "x: " << pos.x << " | y: " << pos.y
              << " | theta: " << pos.theta << std::endl;

    pros::delay(100);
  }
}
void moveDist(double x, int timeout, bool forwards = true, float maxSpeed = 127) {
  lemlib::Pose p = chassis.getPose();
  if (!forwards) {
    p.theta += 180;
  };
  chassis.moveToPoint(p.x + x * sin(p.theta), p.y + x * cos(p.theta), timeout,
                      {.forwards = forwards, .maxSpeed = maxSpeed});
}

/* Test Autons*/
void test_auton() {
  chassis.setPose(0, 0, 0);

  // chassis.moveToPoint(0, 24, 1000, {.maxSpeed = 80});
  // waitd;

  // chassis.turnToHeading(45, 1000, {.maxSpeed = 80});
  // waitd;

  // chassis.moveToPoint(24, 48, 1000, {.maxSpeed = 80});

  chassis.moveToPoint(0, 24, 1250, {.maxSpeed = 80});
  waitd;
  chassis.turnToHeading(90, 650);

  
}

void no_auton() { 
  chassis.setPose(0, 0, 0); 
}

void move_forward() {
  chassis.setPose(0, 0, 0);

  chassis.moveToPoint(0, 7, 1000);
  waitd;
}

void turnforRedrush() {
  chassis.setPose(-51.5, -59, 270);
  chassis.turnToHeading(250, 1000);
}

// Red Positive Autons
void red_rush() {
  chassis.setPose(-51.5, -59, 250);

  chassis.moveToPoint(-21.535, -46.274, 975,
                      {.forwards = false, .minSpeed = 120});
  intake.intake();
  waitd;
  intake.brake();
  doinker.toggle();
  pros::delay(350);
  chassis.moveToPoint(-37.503, -53.761, 900,
                      {.forwards = true, .minSpeed = 100});
  waitd;
  doinker.toggle();

  turnAndMoveToPoint(-18.038, -45.712, {.forwards = true, .mvMaxSpeed = 60});
  waitd;
  mogo.toggle();
  pros::delay(50);
  intake.intake();

  pros::delay(500);

  turnAndMoveToPoint(-45.641, -27.923, {.forwards = false});
  waitd;

  chassis.turnToHeading(270, 700);
  waitd;
  mogo.toggle();

  turnAndMoveToPoint(-25.475, -24.026, {.mvMaxSpeed = 60});
  waitd;
  mogo.toggle();

  turnAndMoveToPoint(-50.616, -35.524, {.forwards = false, .async = true});
  intake.intake();
  waitd;

  chassis.turnToHeading(32, 650);
  waitd;
  chassis.moveToPoint(-59.97, -48.971, 1200, {.forwards = false});
  intake.intake();
  waitd;
}
void red_pos_wp() {
  chassis.setPose(-60.263, -13.198, 180);

  // chassis.turnToHeading(180-31.5, 730);
  // old angle 180-35, 180-45 overshoot
  // scores on alliance stake
  chassis.turnToHeading(180 - 40, 730);
  pros::delay(450);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;

  // moves out of alliance stake
  chassis.moveToPoint(chassis.getPose().x + 10, chassis.getPose().y - 10, TO,
                      {.earlyExitRange = 5});
  waitd;

  // going to mogo
  chassis.moveToPose(-24.554, -24.248, 90, 1500, {.minSpeed = 60});
  chassis.waitUntil(20);
  arm.set_pos(arm.INIT_POS);
  mogo.clamp();
  waitd;

  turnAndMoveToPoint(-15.536, -14.281, {.forwards = false});
  waitd;
  doinker.toggle();
  waitd;
  chassis.moveToPoint(-31.322, -29.872, 900);
  waitd;

  chassis.turnToHeading(328, 650);
  waitd; 
  doinker.toggle();

  chassis.moveToPose(-39.02, -16.2, 84, 1200);
  waitd;

  chassis.turnToHeading(353, 500);
  waitd;
  doinker.toggle();
  chassis.moveToPoint(-37.656, -28.898, 800, {.forwards = false});
  intake.intake();
  waitd;
}

//Blue Negative Autons
void blue_negative_elims() {
    chassis.setPose(53.746, 16.9, 0);

    arm.set_pos(arm.SCORE_POS);
  
    // chassis.moveToPoint(56.864, 9.5, 1000, {.forwards = false});
    // waitd;
    chassis.turnToPoint(67.85, 0, 375, {.forwards = false});
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;
  
    pros::delay(225);
  
    // alliance stake end
    // alliance ring start

    chassis.moveToPoint(44.683, 26.06, 1400);
    waitd;

    arm.set_pos(arm.INTAKE_LIFT);

    chassis.moveToPoint(42.735, 9.495, 1000, {.forwards=false, .maxSpeed=80});
    waitd;

    arm.set_pos(arm.START_POS);
    intake.intake();
    pros::delay(540);
    intake.brake();

    // alliance ring end
    // mogo start
  
    chassis.turnToPoint(14.281, 26.645, 500);
    waitd;
  
    chassis.moveToPoint(14.281, 26.645, 1200, {.maxSpeed = 70});
    waitd;
  
    mogo.clamp(); // mogo end
    pros::delay(225);
  
    // middle ring start
  
    chassis.turnToPoint(3.5, 43, 800, {.forwards = false});
    waitd;
    intake.intake();
    chassis.moveToPoint(3.5, 43, 1400, {.forwards = false});
    waitd;
  
    pros::delay(200);
  
    chassis.turnToHeading(180, 600);
    waitd;
  
    chassis.moveToPoint(chassis.getPose().x - 1.45, 54.221, 900,
                        {.forwards = false, .maxSpeed = 100});
    waitd;
    pros::delay(400);
  
    // end of middle rings
    // start of single stack
  
    chassis.moveToPoint(13.307, 33.076, 900);
    waitd;
  
    chassis.moveToPoint(26.364, 52.175, 900, {.forwards = false});
    waitd;
  
    // end of single stack
    // corner start
  
    chassis.moveToPoint(42.785, 50.785, 1000, {.minSpeed = 115});
    waitd;
  
    chassis.moveToPoint(64.757, 71.079, 1000, {.forwards = false, .minSpeed = 115});
    waitd;
  
    pros::delay(400);
  
    chassis.swingToPoint(44.411, 14.503, DriveSide::LEFT, 600, {.forwards=false});
    waitd;

    chassis.turnToPoint(22.61, 1.267, 500, {.forwards=false});
    waitd;
    chassis.moveToPoint(22.61, 1.267, TO, {.forwards=false, .maxSpeed=80});
    waitd;
  
  //   chassis.moveToPoint(64.223, -63.587, 10000);
}
void blue_negative_wp() {
  chassis.setPose(53.746, 16.9, 0);

  // the commented out code is probs deprecated from when we ran no preload...?
  // arm.set_pos(arm.LOADIN_POS);
  // pros::delay(150);
  // intake.intake();
  // pros::delay(250);
  // intake.intake_brake_mode = pros::E_MOTOR_BRAKE_COAST;
  // intake.brake();
  arm.set_pos(arm.SCORE_POS);

  // chassis.moveToPoint(56.864, 9.5, 1000, {.forwards = false});
  // waitd;
  chassis.turnToPoint(67.85, 0, 375, {.forwards = false});
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;

  pros::delay(550);

  arm.set_pos(arm.START_POS);

  chassis.moveToPoint(44.683, 26.06, 1100);
  waitd;

  chassis.turnToPoint(20, 25, 700);
  waitd;

  chassis.moveToPoint(20, 25, 1000, {.maxSpeed = 90});
  waitd;

  mogo.clamp(); // mogo end
  pros::delay(225);

  // fds
  //  chassis.moveToPose(chassis.getPose().x-20, chassis.getPose().y+18.25, 127,
  //  1900, {.forwards = false, .minSpeed = 89.69});
  turnAndMoveToPoint(7.1, 38, {.turnTO = 650, .forwards = false,.async = true});
  intake.intake();
  waitd;

  pros::delay(450);

  chassis.turnToHeading(180, 600);
  waitd;

  chassis.moveToPoint(chassis.getPose().x-1.45, 54.221, 1200, {.forwards = false, .maxSpeed=100});
  waitd;
  pros::delay(500);

  chassis.moveToPoint(19.153, 25.475, 900);
  waitd;

  chassis.moveToPoint(24.22, 48.277, 1000, {.forwards=false, .maxSpeed=110});
  waitd;

  // corner start

  chassis.moveToPoint(56.571, 44.574, 1000, {.minSpeed=110});
  waitd;

  chassis.moveToPoint(64.757, 71.079, 1100, {.forwards=false, .minSpeed=110});
  waitd;

  pros::delay(250);

  chassis.turnToPoint(72.357, 54.513, 800, {.forwards=false});
  waitd;
  chassis.moveToPoint(72.357, 54.513, 800, {.forwards=false});
  waitd;

  chassis.turnToPoint(17.692, 17.68, TO, {.forwards = false});
  waitd;

  chassis.moveToPoint(17.692, 17.68, TO, {.forwards = false, .maxSpeed = 120});
  intake.brake();
  waitd;

  arm.set_pos(arm.SCORE_POS);


}

//Red Negative Autons
void red_neg_wp() {
  chassis.setPose(-60.263, 13.198, 0);

  /**
   * ADDED AT IOWA
   */
  // pros::delay(2000);

  /**
   * REMOVED SCORING ON ALLIANCE STAKE
   */
  // // chassis.turnToHeading(31.5, TO);
  chassis.turnToHeading(35, TO);
  pros::delay(500);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;

  chassis.moveToPoint(-46.913, 23.332, 1000);
  arm.set_pos(arm.INIT_POS);
  waitd;

  chassis.turnToHeading(90, 650);
  waitd;
  chassis.moveToPoint(-28.009, 23.332, 1200);
  waitd;
  mogo.toggle();

  // going to middle rings
  // TODO: seemingly "kicking out" ring cuz turns near end
  chassis.turnToHeading(226, 700);
  chassis.moveToPoint(-12.097, 36.515, 1200, {.forwards = false});
  intake.intake();
  waitd;
  pros::delay(900);

  //
  chassis.moveToPose(-9.69, 59.903, 188, 1300, {.forwards = false});
  waitd;
  pros::delay(1000);

  chassis.moveToPoint(-15.147, 34.635, 1400, {.forwards = true});
  waitd;

  // getting the ring in middle of the side
  chassis.turnToHeading(145, 700);
  waitd;
  chassis.moveToPoint(-29.568, 50.316, 1000, {.forwards = false});
  waitd;

  // going to ladder
  chassis.moveToPose(-16.893, 11.583, 315, 1500, {.forwards = false, .minSpeed = 120});
  chassis.waitUntil(40);
  arm.set_pos(arm.SCORE_POS);
  waitd;

}
//TODO: rewrite this mirror with blue negative_elims
void red_negative_elims() {
}

// Blue Positive Autons
void blue_rush() {
  chassis.setPose(54.395, -33.055, 90);

  chassis.moveToPose(15.061, -47.802, 81.8, 1250, {.forwards = false, .minSpeed = 125});
  intake.intake();
  chassis.waitUntil(23);
  doinker.toggle();
  waitd;
  doinker.toggle();

  turnAndMoveToPoint(41.37, -32.6, {.forwards = true, .async = true});
  chassis.waitUntil(7);
  doinker.toggle();
  waitd;
}

void blue_pos_wp() {
  chassis.setPose(56.746, -14.9, 270);

  chassis.turnToHeading(222.5, 650);
  // intake.outtake();
  // pros::delay(40);
  // intake.intake_brake_mode = pros::E_MOTOR_BRAKE_COAST;
  // intake.brake();
  arm.set_pos(arm.HOLD_POS);
  waitd;

  // arm.set_pos(arm.SCORE_POS);
  arm.set_pos(arm.ALLIANCE_SCORE);

  // chassis.setPose(57.534, -12.808, 45);

  // pros::delay(1000);

  // chassis.moveToPoint(22.912, -12.808, 1300, {.forwards = true, .maxSpeed =
  // 60}); chassis.waitUntil(26); mogo.toggle(); waitd;

  pros::delay(750);

  // chassis.turnToPoint(27.339, -20, 750);
  // waitd;
  chassis.moveToPoint(46.048, -25.195, TO);
  waitd;
  arm.set_pos(arm.START_POS);
  chassis.moveToPoint(23.831, -27, 1700, {.maxSpeed = 60});
  waitd;
  mogo.clamp();

  pros::delay(350);

  // gets the first blue ring on field (under the red ring)
  chassis.moveToPoint(chassis.getPose().x, -45.915, 1200, {.forwards = false});
  pros::delay(350);
  intake.intake();
  waitd;
  pros::delay(1000);

  // going to corner
  chassis.turnToPoint(66.511, -71.121, TO, {.forwards = false});
  waitd;
  chassis.moveToPoint(66.511, -71.121, TO, {.forwards = false});
  arm.set_pos(arm.HOLD_POS);
  waitd;

  pros::delay(1000);

  /** LADDER FIX @ IOWA -- DO NOT REMOVE */
  intake.brake();

  arm.set_pos(arm.SCORE_POS);

  chassis.moveToPose(0, 0, 150, 5000, {.forwards = false});
  waitd;
  

  // chassis.moveToPoint(35.709, -80.464, 1200, {.forwards=false});
  // intake.brake();
  // waitd;
  // chassis.turnToHeading(270, TO);
  // doinker.toggle();
  // waitd;
  // pros::delay(300);
  // chassis.moveToPoint(chassis.getPose().x+25, chassis.getPose().y-10, TO,
  // {.forwards=false}); waitd; chassis.turnToHeading(180, TO); waitd;
  // doinker.toggle();

  // intake.outtake();
}

//Solo wp
void red_sawp(){
  chassis.setPose(-60.263, 13.198, 0);

  chassis.turnToHeading(35, TO);
  pros::delay(500);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;

  chassis.moveToPoint(-46.913, 23.332, 1000);
  arm.set_pos(arm.INIT_POS);
  waitd;

  chassis.turnToHeading(90, 650);
  waitd;
  chassis.moveToPoint(-28.009, 23.332, 1200);
  waitd;
  mogo.toggle();

  chassis.turnToHeading(226, 700);
  chassis.moveToPoint(-12.097, 36.515, 1200, {.forwards = false});
  intake.intake();
  waitd;
  pros::delay(900);

  chassis.moveToPoint(-24.501, 24.306, 900);
  waitd;

  chassis.turnToHeading(180, 650);
  waitd;
  
  chassis.moveToPoint(-24.501, 45.744, 1000, {.forwards = false});
  waitd;

  chassis.moveToPose(-44.769, 4.428, 25, 2000, {.forwards = false});
  arm.set_pos(arm.INTAKE_LIFT);
  waitd;
}