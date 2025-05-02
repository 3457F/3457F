#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "util.hpp"
#include <numbers>

// declares pure pursuit files

// red nostack awp
ASSET(red_neg_txt);


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
void exit_condition(lemlib::Pose target, double exitDist){
  chassis.waitUntil(fabs(chassis.getPose().distance(target)) - exitDist);
  chassis.cancelMotion();
}

// Red Positive Autons
void red_rush() { 
  arm.set_pos(arm.DUNK_POS);
  chassis.setPose(-51.5, -59, 250);

  chassis.moveToPoint(-21.535, -46.274, 840, {.forwards = false, .minSpeed = 127});
  intake.intake();
  waitd;
  intake.brake();
  doinker.left_toggle();
  pros::delay(350);
  chassis.moveToPoint(-37.503, -53.761, 900, {.forwards = true, .minSpeed = 100});
  waitd;
  doinker.left_toggle();
  
  chassis.turnToPoint(-18.038, -52.869, 750);
  waitd;
  chassis.moveToPoint(-18.038, -52.869, TO, {.forwards = true, .maxSpeed=70});
  waitd;
  mogo.toggle();

  pros::delay(150);

  chassis.moveToPose(-4.818, -64.638, 304.7, 1850, {.forwards = false});
  pros::delay(700);
  intake.intake();
  waitd;
  arm.set_pos(arm.SCORE_POS);
  pros::delay(200);

  chassis.moveToPoint(-45.641, -27.923, 1300, {.forwards = true});
  waitd;

  chassis.turnToHeading(270, 700);
  arm.set_pos(arm.INIT_POS);
  waitd;
  mogo.toggle();

  chassis.turnToPoint(-24.306, -32.795, 770);
  waitd;
  intake.brake();
  chassis.moveToPoint(-24.306, -32.795, TO, {.maxSpeed=70});
  waitd;
  mogo.toggle();

    // zed
  chassis.turnToPoint(-17.680, -19.543, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-17.680, -19.543, 1000, {.forwards = false});
  waitd;
  chassis.turnToHeading(230, 400);
  waitd;
  doinker.left_toggle();
  pros::delay(350);

  chassis.moveToPoint(-31.907, -31.431, 1000);
  waitd;

  chassis.turnToHeading(325, 600);
  waitd;
  doinker.left_toggle();
  pros::delay(50); 

  intake.intake();
  chassis.turnToPoint(-29.178, -53.843,200, {.forwards = false});
  
  chassis.moveToPoint(-29.178, -53.843, 1000, {.forwards = false});
  waitd;
  
}
void red_pos_wp() {
  intake.alliance_color = intake.RED;

  chassis.setPose(-60.263, -13.198, 180);

  // chassis.turnToHeading(180-31.5, 730);
  // old angle 180-35, 180-45 overshoot
  // scores on alliance stake
  chassis.turnToHeading(180 - 40, 730);
  pros::delay(450);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  pros::delay(200);

  // moves out of alliance stake
  chassis.moveToPoint(chassis.getPose().x + 10, chassis.getPose().y - 10, TO,
                      {.minSpeed=35, .earlyExitRange = 5});
  waitd;

  // going to mogo
  chassis.moveToPose(-24.554, -24.248, 90, 1500, {.minSpeed = 60});
  chassis.waitUntil(24);
  arm.set_pos(arm.INIT_POS);
  mogo.clamp();
  waitd;

  turnAndMoveToPoint(-16.264, -13.213, {.forwards = false});
  chassis.turnToHeading(180+58.25, 800);
  waitd;
  doinker.toggle();
  pros::delay(350);
  waitd;
  chassis.turnToPoint(-45.256, -30.262, 600, {.forwards = true});
  waitd;
  chassis.moveToPoint(-45.256, -30.262, 1000, {.forwards = true});
  chassis.turnToHeading(285, 650);
  waitd;
  doinker.toggle();

  pros::delay(240);

  // chassis.moveToPoint(-50.899, -22.424, 700);
  // waitd;
  chassis.turnToPoint(-21.937, -49.556, 800, {.forwards=false});
  waitd;

  intake.intake();
  // turnAndMoveToPoint(-39.994, -39.421, {.forwards = false});
  // waitd;
  // pros::delay(1000);
  chassis.moveToPoint(-21.937, -49.556, 2500, {.forwards = false, .maxSpeed=40});
  waitd;
  pros::delay(750);
  // intake.intake();

  chassis.moveToPoint(-9.631, -10.582, 1300, {.forwards = false});
  arm.set_pos(arm.SCORE_POS);
  waitd;
}
void red_pos_elims(){

  chassis.setPose(-55.500, -23.500, 90);

  chassis.moveToPoint(-24.000, -23.500, 1100, {.maxSpeed =70});
  chassis.waitUntil(24);
  mogo.toggle();
  exit_condition(lemlib::Pose (-24.000, -23.500), std::numbers::e);

  //gettin first ring
  chassis.turnToPoint(-13.316, -12.292, 650, {.forwards = false});
  pros::delay(400);
  chassis.moveToPoint(-13.316, -12.292, 1000, {.forwards = false, .maxSpeed = 80});
  exit_condition(lemlib::Pose (-13.316, -12.292), std::numbers::e);
  doinker.left_toggle();
  pros::delay(250);

  chassis.moveToPose(-39.477, -25.372, 59, 1300, {.forwards = true, .minSpeed = 100});
  exit_condition(lemlib::Pose(-39.477, -25.372), std::numbers::e);

  chassis.turnToHeading(270, 400);
  waitd;
  doinker.left_toggle();

  chassis.turnToPoint(-25.844, -34.215, 300, {.forwards = false});
  waitd;
  chassis.moveToPoint(-25.844, -34.215, 800, {.forwards = false});
  intake.intake();
  exit_condition(lemlib::Pose (-25.844, -34.215), 2);
  pros::delay(200);

  //red bottom stack
  chassis.turnToPoint(-24.370, -48.493,500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-24.370, -48.493, 1000, {.forwards = false});
  exit_condition(lemlib::Pose (-24.370, -48.493), std::numbers::e);

  chassis.turnToPoint(-43.530, -53.743, 500, {.forwards = false});
  waitd;

  chassis.moveToPoint(-46.846, -55.954, 1000, {.forwards = false});
  exit_condition(lemlib::Pose (-46.846, -55.954), std::numbers::e);

  chassis.moveToPoint(-69.322, -64.429, 950, {.forwards = false, .maxSpeed = 80});
  exit_condition(lemlib::Pose (-69.322, -64.429), std::numbers::e);
  pros::delay(800); 
  chassis.moveToPoint(-52.741, -57.704, 800);
  exit_condition(lemlib::Pose(-52.741, -57.704), 2);
  doinker.right_toggle();
  pros::delay(250);
  chassis.moveToPoint(-60.479, -60.468, 700, {.forwards = false});
  pros::delay(200);
  chassis.turnToPoint(-55.500, -23.500, 700, {.forwards = false});
  waitd;

  chassis.moveToPose(-21.975, -51.533, 257, 1300, {.forwards = false, .minSpeed = 100});
  intake.brake();
  chassis.waitUntil(12);
  // mogo.toggle();
  doinker.right_toggle();
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  chassis.turnToHeading(90, 700);


  
}

//Blue Negative Autons
void blue_negative_elims() {
    intake.alliance_color = intake.BLUE;

    chassis.setPose(53.746, 16.9, 0);

    arm.set_pos(arm.SCORE_POS);
  
    // chassis.moveToPoint(56.864, 9.5, 1000, {.forwards = false});
    // waitd;
    chassis.turnToPoint(67.85, 0, 375, {.forwards = false});
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;
  
    pros::delay(200);
  
    // alliance stake end
    // // alliance ring start

    // chassis.moveToPoint(44.683, 26.06, 1400);
    // waitd;

    // arm.set_pos(arm.INTAKE_LIFT);

    // chassis.moveToPoint(42.735, 9.495, 1000, {.forwards=false, .maxSpeed=80});
    // waitd;

    // arm.set_pos(arm.START_POS);
    // intake.intake();
    // pros::delay(540);
    // intake.brake();

    // // alliance ring end
    // mogo start
  
    chassis.moveToPoint(44.683, 26.06, 850);
    waitd;

    arm.set_pos(arm.START_POS);
  
    chassis.moveToPoint(14.281, 26.645, 1200, {.maxSpeed = 70});
    waitd;
  
    mogo.clamp(); 

    // mogo end

    pros::delay(225);
  
    // mid doinker ring start

    chassis.moveToPoint(9.3, 10.271, 800, {.forwards=false});
    waitd;
    chassis.turnToHeading(73.25, 800);
    pros::delay(150);
    doinker.left_toggle();
    pros::delay(250);

    chassis.moveToPoint(14.281, 26.645, 1200);
    waitd;
    chassis.turnToHeading(90+17.5, 800);
    pros::delay(350);
    doinker.left_toggle();
    waitd;

    // middle ring start
  
    chassis.turnToPoint(3.5, 43, 200, {.forwards = false});
    waitd;
    intake.intake();
    chassis.moveToPoint(3.5, 43, 1400, {.forwards = false});
    waitd;
  
    pros::delay(200);
  
    chassis.turnToHeading(180, 600);
    waitd;
  
    chassis.moveToPoint(chassis.getPose().x - 2.25, 60.221, 1100,
                        {.forwards = false, .maxSpeed = 100});
    waitd;
    pros::delay(400);
  
    // end of middle rings
    // start of single stack
  
    chassis.moveToPoint(13.307, 33.076, 900);
    waitd;
  
    chassis.moveToPoint(26.364, 57.826, 1200, {.forwards = false});
    waitd;
  
    // end of single stack
    // corner start
    waitd;
  
    chassis.moveToPose(73.307, 91.323, 180+45, 1200, {.forwards = false, .minSpeed = 115});
    waitd;
  
    pros::delay(400);
  
    chassis.turnToPoint(44.411, 14.503, 600, {.forwards=false});
    waitd;

    chassis.moveToPoint(99.544, 57.826, 800, {.forwards=false});
    waitd;

    chassis.turnToPoint(22.61, 1.267, 800);
    waitd;
    chassis.moveToPoint(22.61, 1.267, TO, {.maxSpeed=80});
    waitd;
  
  //   chassis.moveToPoint(64.223, -63.587, 10000);
}
void blue_negative_wp() {
  chassis.setPose(60.263, 13.198, 0);

  chassis.turnToHeading(-35, 700);
  pros::delay(200);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  
  chassis.moveToPoint(46.913, 23.332, 900, {.minSpeed=80});
  exit_condition(lemlib::Pose(-46, 23), 4);
  arm.set_pos(arm.INIT_POS);

  chassis.turnToHeading(-90, 650, {.minSpeed=50, .earlyExitRange=5});
  waitd;
  chassis.moveToPoint(28.009, 23.332, 1200,{.maxSpeed=70, .minSpeed=40});
  exit_condition(lemlib::Pose(-28, 23), 0.5);
  chassis.waitUntil(15.65);
  mogo.toggle();
  waitd;

  chassis.turnToPoint(12, 37.503, 600, {.forwards = false});
  intake.intake();
  waitd;
  chassis.moveToPoint(12, 37.5032, 1000, {.forwards = false, .minSpeed=60, .earlyExitRange=5});
  pros::delay(300);

  chassis.turnToPoint(11.5, 57.005, 300, {.forwards = false});
  waitd;
  chassis.moveToPoint(11.5, 57.005, 1000, {.forwards = false, .maxSpeed=70});
  waitd;
  pros::delay(500);

  chassis.moveToPose(23.633, 23.449, -215, 1200, {.forwards = true, .lead = .5, .minSpeed=90});
  waitd;
  
  // chassis.turnToPoint(23.449, 47.214, 500, {.forwards = false});
  // waitd;
  chassis.moveToPoint(28.449, 47.214, 1000, {.forwards = false});
  waitd;
  pros::delay(450);

  //stack
  // chassis.turnToPoint(49.161, -0.885, 550);
  // waitd;
  chassis.moveToPoint(52.161, 1.5, 2000, {.forwards = false, .maxSpeed=70, .earlyExitRange=5});
  pros::delay(500);
  intake.lift(true);
  waitd;
  intake.lift(false);
  pros::delay(600);

  chassis.moveToPoint(48.401, 15.895, 900);
  waitd;
  pros::delay(500);

  //ladder
  chassis.moveToPoint(31.765, -0.685, 1000, {.forwards = false, .maxSpeed = 110});
  waitd;
  arm.set_pos(arm.LADDER_POS);
  pros::delay(200);
  intake.brake();

}

//Red Negative Autons
void red_neg_wp() {

  chassis.setPose(-60.263, 13.198, 0);

  chassis.turnToHeading(35, 700);
  pros::delay(200);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  
  chassis.moveToPoint(-46.913, 23.332, 900, {.minSpeed=80});
  exit_condition(lemlib::Pose(-46, 23), 4);
  arm.set_pos(arm.INIT_POS);

  chassis.turnToHeading(90, 650, {.minSpeed=50, .earlyExitRange=5});
  waitd;
  chassis.moveToPoint(-28.009, 23.332, 1200,{.maxSpeed=70, .minSpeed=40});
  exit_condition(lemlib::Pose(-28, 23), 0.5);
  chassis.waitUntil(15.65);
  mogo.toggle();
  waitd;

  chassis.turnToPoint(-9, 37.503, 600, {.forwards = false});
  intake.intake();
  waitd;
  chassis.moveToPoint(-9, 37.5032, 1000, {.forwards = false, .minSpeed=60});
  exit_condition(lemlib::Pose(-7.973, 38.003), 5);
  pros::delay(300);

  chassis.turnToPoint(-8.875, 59.005, 300, {.forwards = false});
  waitd;
  chassis.moveToPoint(-8.875, 59.005, 1000, {.forwards = false, .maxSpeed=70});
  waitd;
  pros::delay(500);

  chassis.moveToPose(-23.633, 23.449, 215, 1200, {.forwards = true, .lead = .5, .minSpeed=60});
  waitd;
  
  chassis.turnToPoint(-23.449, 47.214, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-23.449, 47.214, 1000, {.forwards = false});
  exit_condition(lemlib::Pose(-23.449, 47.214), std::numbers::e);
  pros::delay(450);

  //stack
  chassis.turnToPoint(-43.161, -0.685, 550);
  waitd;
  chassis.moveToPoint(-43.161, -0.685, 2000, {.forwards = false, .maxSpeed=100});
  pros::delay(500);
  intake.lift(true);
  waitd;
  intake.lift(false);
  pros::delay(600);

  //ladder
 chassis.turnToHeading(270, 650);

  chassis.moveToPoint(-31.765, -0.685, 1000, {.forwards = false, .minSpeed = 110});
  waitd;
  arm.set_pos(arm.LADDER_POS);
  pros::delay(200);
  intake.brake();

  

}

void red_negative_elims() {
  chassis.setPose(-60.263, 13.198, 0);

//alliance stake
  chassis.turnToHeading(35, 700);
  pros::delay(200);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  
//mogo
  chassis.moveToPoint(-46.913, 23.332, 900, {.minSpeed=80, .earlyExitRange=4});
  // exit_condition(lemlib::Pose(-46, 23), 4);
  chassis.moveToPose(-20.009, 23.332, 90, 1800,{.maxSpeed=95, .minSpeed=70, .earlyExitRange=2});
  arm.set_pos(arm.INIT_POS);
  chassis.waitUntil(22.65);
  mogo.toggle();
  pros::delay(100);
  // chassis.turnToHeading(90, 650, {.minSpeed=50, .earlyExitRange=5});
  // waitd;
  // chassis.moveToPoint(-28.009, 23.332, 1200,{.maxSpeed=70, .minSpeed=40});
  // exit_condition(lemlib::Pose(-28, 23), 0.5);
 


//top rings
  chassis.moveToPose(-11.000, 33.503, 220, 1100, {.forwards = false, .minSpeed=100,.earlyExitRange = 10});
  intake.intake();
  chassis.moveToPose(-10.875, 52.005, 180.5, 1000, {.forwards = false, .minSpeed=100,.earlyExitRange = 13});
  waitd;
  chassis.moveToPose(-23.633, 23.449, 215, 900, {.forwards = true, .lead = .5, .minSpeed=100});
  waitd;
  
//middle stack
  chassis.moveToPoint(-23.449, 47.214, 1000, {.forwards = false, .minSpeed  = 100});
  exit_condition(lemlib::Pose(-23.449, 47.214), std::numbers::e);

//corner
  chassis.moveToPose(-40.213, 35.424, 53, 1000, {.forwards = true, .minSpeed=100});
  exit_condition(lemlib::Pose (-40.213, 35.424), std::numbers::e);

  // chassis.turnToPoint(-60.847, 61.4005, 500, {.forwards = false});
  // waitd;
  chassis.moveToPoint(-60.847, 61.400, 1000, {.forwards = false,  .maxSpeed=110, .minSpeed=90, .earlyExitRange = 9});;
  exit_condition(lemlib::Pose (-60.847, 61.400), std::numbers::pi+1);
  chassis.moveToPoint(-60.847, 61.400, 700, {.forwards = false, .maxSpeed=70});
  pros::delay(400);

  chassis.moveToPoint(-46.846, 43.530,1200,{.forwards = true, .minSpeed = 100});
  exit_condition(lemlib::Pose (-46.846, 43.530), 0.5);
  pros::delay(400);

  // chassis.turnToPoint(-47.030, 0.236, 700, {.forwards = false});  
  // waitd;
  chassis.moveToPoint(-47.030, 0.236, 1200, {.forwards = false, .minSpeed = 80});
  chassis.waitUntil(10);
  intake.lift(true);
  chassis.waitUntil(18);
 
  chassis.turnToHeading(0,200);

  exit_condition(lemlib::Pose (-47.030, 0.236), std::numbers::e+8);
  intake.lift(false);

  

  
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

  pros::delay(2000);
  chassis.setPose(60.263, -13.198, 180);

  // chassis.turnToHeading(180-31.5, 730);
  // old angle 180-35, 180-45 overshoot
  // scores on alliance stake
  chassis.turnToHeading(215, 730);
  arm.set_pos(arm.ALLIANCE_SCORE);
  pros::delay(300);
  waitd;

  // moves out of alliance stake
  chassis.moveToPoint(chassis.getPose().x - 10, chassis.getPose().y - 10, TO,
                      {.earlyExitRange = 5});
  waitd;

  // going to mogo
  chassis.moveToPose(24.554, -24.248, 90, 1500, {.minSpeed = 60});
  chassis.waitUntil(20);
  arm.set_pos(arm.INIT_POS);
  mogo.clamp();
  waitd;

  chassis.turnToPoint(16.896, -11.312, 600, {.forwards = false});
  waitd;
  chassis.moveToPoint(16.896, -11.312, 1000, {.forwards = false});
  waitd;
  doinker.right_toggle();
  pros::delay(350);
  waitd;

  chassis.moveToPoint(31.334, -22.174, 1000);
  waitd;
  chassis.turnToHeading(90, 500);
  waitd;

  chassis.moveToPoint(47.899, -22.369, 800);
  waitd;
  chassis.turnToHeading(52, 500);
  waitd;
  doinker.right_toggle();
  pros::delay(50);
  waitd;

  chassis.turnToPoint(17.497, -52.966, 300, {.forwards = false});
  waitd;
  chassis.moveToPoint(17.497, -52.966, 1900, {.forwards = false, .maxSpeed = 70});
  intake.intake();
  waitd;
  pros::delay(1200);

  chassis.turnToPoint(36.596, -36.985, 400);
  waitd;
  chassis.moveToPoint(36.596, -36.985, 1000);
  waitd;

  chassis.turnToPoint(14.379, -19.446, 600 , {.forwards = false});
  arm.set_pos(arm.SCORE_POS);
  waitd;

  chassis.moveToPoint(14.379, -19.446, 900, {.forwards = false});

  waitd;



}

void blue_pos_elims(){
  chassis.setPose(55.500, -23.500, -90);

  chassis.moveToPoint(24.000, -23.500, 1100, {.maxSpeed =70});
  chassis.waitUntil(26);
  mogo.toggle();
  exit_condition(lemlib::Pose (24.000, -23.500), std::numbers::e);

  //gettin first ring
  chassis.turnToPoint(11.555, -11.555, 650, {.forwards = false});
  pros::delay(400);
  chassis.moveToPoint(11.555, -11.555, 1000, {.forwards = false, .maxSpeed = 80});
  exit_condition(lemlib::Pose (11.555, -11.555), std::numbers::e);
  doinker.right_toggle();
  pros::delay(250);
 
  chassis.moveToPose(39.477, -25.372,  90, 1400, {.forwards = true, .minSpeed = 100});
  pros::delay(1200);
  doinker.right_toggle(); 
  waitd;


  // chassis.turnToHeading(90, 650, {.maxSpeed = 90});
  // waitd;


  chassis.turnToPoint(25.844, -34.215, 300, {.forwards = false});
  waitd;
  chassis.moveToPoint(25.844, -34.215, 800, {.forwards = false});
  intake.intake();
  exit_condition(lemlib::Pose (25.844, -34.215), 2);
  pros::delay(200);

  //red bottom stack
  chassis.turnToPoint(24.370, -48.493,500, {.forwards = false});
  waitd;
  chassis.moveToPoint(24.370, -48.493, 1000, {.forwards = false});
  exit_condition(lemlib::Pose (-24.370, -48.493), std::numbers::e);

  chassis.turnToPoint(43.530, -53.743, 500, {.forwards = false});
  waitd;

  chassis.moveToPoint(46.846, -55.954, 1000, {.forwards = false});
  exit_condition(lemlib::Pose (-46.846, -55.954), std::numbers::e);

  //corner stack
  chassis.moveToPoint(69.771, -67.008, 950, {.forwards = false, .maxSpeed = 80});
  exit_condition(lemlib::Pose (69.771, -67.008), std::numbers::e);
  pros::delay(700); 
  chassis.moveToPoint(46.190, -55.770, 950, {.minSpeed = 50});  
  exit_condition(lemlib::Pose(46.190, -55.770), 2);
  doinker.left_toggle();
  chassis.moveToPoint(54.849, -59.823, 700, {.forwards = false});
  exit_condition(lemlib::Pose(54.849, -59.823), 2);
  chassis.turnToPoint(55.500, -23.500, 700, {.forwards = false, .minSpeed = 127});
  waitd;

  chassis.moveToPose(21.975, -51.533, -257, 1300, {.forwards = false, .minSpeed = 127});

  chassis.waitUntil(7);
  mogo.toggle();
  exit_condition(lemlib::Pose(21.975, -51.533),5 );
  doinker.left_toggle();
  // waitd;
  chassis.turnToHeading(270, 600, {});
  intake.brake();

}

//Solo wp
void red_sawp(){
  chassis.setPose(-60.263, 13.198, 0);

//alliance stake
  chassis.turnToHeading(35, 700);
  pros::delay(200);
  arm.set_pos(arm.ALLIANCE_SCORE);
  waitd;
  
//mogo
  chassis.moveToPoint(-46.913, 23.332, 900, {.minSpeed=80});
  exit_condition(lemlib::Pose(-46, 23), 4);
  arm.set_pos(arm.INIT_POS);
  chassis.turnToHeading(90, 650, {.minSpeed=50, .earlyExitRange=5});
  waitd;
  chassis.moveToPoint(-28.009, 23.332, 1200,{.maxSpeed=70, .minSpeed=40});
  exit_condition(lemlib::Pose(-28, 23), 0.5);
  chassis.waitUntil(15.65);
  mogo.toggle();
  waitd;

//top rings
  chassis.moveToPose(-11.000, 33.503, 220, 1300, {.forwards = false, .minSpeed=50,.earlyExitRange = 10});
  intake.intake();
  chassis.moveToPose(-10.875, 52.005, 180.5, 1300, {.forwards = false, .minSpeed=50,.earlyExitRange = 13});
  waitd;
  chassis.moveToPose(-23.633, 23.449, 215, 900, {.forwards = true, .lead = .5, .minSpeed=90});
  waitd;
  
//middle stack
  chassis.turnToPoint(-23.449, 47.214, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-23.449, 47.214, 1000, {.forwards = false});
  exit_condition(lemlib::Pose(-23.449, 47.214), std::numbers::e);
 

  chassis.turnToPoint(-47.033, 21.817, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-47.03, 21.817, 1000, {.forwards = false});
  exit_condition(lemlib::Pose(-46.293, 21.817), std::numbers::e);
  chassis.turnToHeading(0, 500);
  waitd;
  chassis.moveToPoint(-47.477, -21.793, 1900, {.forwards = false, .maxSpeed =50});
   chassis.waitUntil(2);
  mogo.toggle();
  intake.lift(true);
  chassis.waitUntil(12.5);
  intake.lift(false);
  exit_condition(lemlib::Pose(-46.477, -21.345), std::numbers::e);

 chassis.turnToHeading(90, 500);
  waitd;
  chassis.moveToPoint(-24.765, -21.345, 1000, {.forwards = true, .maxSpeed = 60});
  chassis.waitUntil(3);
  intake.brake();
  exit_condition(lemlib::Pose (-24.765, -21.345), 2);
  mogo.toggle();







  
  
  //stack
  // chassis.turnToPoint(-43.161, -0.685, 550);
  // waitd;
  // chassis.moveToPoint(-43.161, -0.685, 2000, {.forwards = false, .maxSpeed=100});
  // pros::delay(500);
  // intake.lift(true);
  // waitd;
  // intake.lift(false);
  // pros::delay(600);


}