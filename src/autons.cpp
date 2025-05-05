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
    chassis.setPose(60.263, 13.198, 0);

    // alliance stake
    chassis.turnToHeading(325, 700);
    // waits for turn to mostly finish
    pros::delay(200);
    // runs in the extra 500ms
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;

    // backs up
    // moves out without lifting arm to avoid getting stuck
    // on alliance stake
    chassis.moveToPoint(46.913, 23.332, 900, { .minSpeed = 80, .earlyExitRange = 4 });
    
    // goes for middle mogo
    chassis.moveToPose(16.009, 23.332, 270, 1800, { .maxSpeed = 95, .minSpeed = 60, .earlyExitRange = 2 });
    arm.set_pos(arm.INIT_POS);
    chassis.waitUntil(22.65);
    mogo.toggle();
    pros::delay(100);
    // ensures robot at exact heading for next movetopose
    chassis.turnToHeading(270, 100);

    // goes for bottom auton line ring
    chassis.moveToPose(9.447, 34.055, 139.8, 1000, { .forwards = false, .maxSpeed = 120, .earlyExitRange = 10 });
    intake.intake();
    // goes for top auton line ring
    chassis.moveToPose(9.263, 61.636, 179.5, 1100, { .forwards = false, .maxSpeed = 100, .earlyExitRange = 10 });
    pros::delay(400);

    // goes back to mogo point
    chassis.moveToPose(23.633, 23.633, 144.4, 1400, {.forwards = true,.lead = 0.5, .minSpeed = 120}); 
    exit_condition(lemlib::Pose(23.633, 23.633, 215.6), std::numbers::e);

    // goes for lone stack in middle of corner
    chassis.turnToPoint(26.396, 45.740, 500, {.forwards = false});
    waitd;
    chassis.moveToPoint(26.396, 45.740, 1000, {.forwards = false, .minSpeed=80});
    exit_condition(lemlib::Pose(26.396, 45.740), std::numbers::e);

    // aligns with corner
    chassis.moveToPose(37.819, 34.318, 301, 1000, {.forwards = true, .minSpeed=80, .earlyExitRange = 2});
    // TODO: could go for second red ring, but runs risk of
    // getting a blue ring instead, so DOESN'T DO IT
    chassis.turnToPoint(65.545, 65.1773, 500, {.forwards = false});
    waitd;

    // getting corner ring
    chassis.moveToPoint(65.545, 65.177, 1200, {.forwards = false, .minSpeed = 90});  
    exit_condition(lemlib::Pose (64.347, 63.426), std::numbers::e);
    pros::delay(500);
    
    // backs out of corner
    chassis.turnToHeading(77, 300);
    chassis.moveToPoint(46.109, 43.345,1300, {.forwards = true, .minSpeed=80});
    exit_condition(lemlib::Pose (46.109, 43.345), 0.5);

    // goes for stack in middle of blue side
    chassis.turnToPoint(45.214, -1.133, 600, {.forwards = false});  
    waitd;
    chassis.moveToPoint(45.846, -1.131, 1900, {.forwards = false, .minSpeed = 100});
    chassis.waitUntil(6);
    intake.lift(true);
    exit_condition(lemlib::Pose (45.214, -1.133), 1);
    intake.lift(false);
    pros::delay(400);

    // attempts to drop entire mogo in positive corner

    // TODO: auton times out when going for corner
    chassis.moveToPose(53.017, -57.336, 348.5, 2000, {.forwards = true, .minSpeed = 127});
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
  chassis.moveToPose(-16.009, 23.332, 90, 1800,{.maxSpeed=95, .minSpeed=60, .earlyExitRange=2});
  arm.set_pos(arm.INIT_POS);
  chassis.waitUntil(22.65);
  mogo.toggle();
  pros::delay(100);
  // ensure robot at exact heading for next movetopose
  chassis.turnToHeading(90,100);


//top rings

  chassis.moveToPose(-9.447, 34.055, 220.2, 1000, {.forwards = false,.maxSpeed = 120, .earlyExitRange = 10});
  intake.intake();
  chassis.moveToPose(-9.263, 61.636, 180.5, 1100, {.forwards = false, .maxSpeed = 100, .earlyExitRange = 10});
  pros::delay(400);
  chassis.moveToPose(-23.633, 23.633, 215.6, 1400, {.forwards = true,.lead = 0.5, .minSpeed = 120}); 
  exit_condition(lemlib::Pose(-23.633, 23.633, 215.6), std::numbers::e);

  chassis.turnToPoint(-26.396, 45.740, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-26.396, 45.740, 1000, {.forwards = false, .minSpeed=80});
  exit_condition(lemlib::Pose(-26.396, 45.740), std::numbers::e);
 
  // corner
  chassis.moveToPose(-37.819, 34.318, 59.0, 1000, {.forwards = true, .minSpeed=80, .earlyExitRange = 2});
  // TODO: could go for second red ring, but runs risk of
  // getting a blue ring instead, so DOESN'T DO IT
  chassis.turnToPoint(-65.545, 65.1773, 500, {.forwards = false});
  waitd;
  chassis.moveToPoint(-65.545, 65.177, 1200, {.forwards = false, .minSpeed = 90});  
  exit_condition(lemlib::Pose (-64.347, 63.426), std::numbers::e);
  pros::delay(500);
  chassis.turnToHeading(77, 300);
  chassis.moveToPoint(-46.109, 43.345,1300, {.forwards = true, .minSpeed=80});
  exit_condition(lemlib::Pose (-46.109, 43.345), 0.5);

  chassis.turnToPoint(-45.214, -1.133, 600, {.forwards = false});  
  waitd;
  chassis.moveToPoint(-45.846, -1.131, 1900, {.forwards = false, .minSpeed = 100});
  chassis.waitUntil(6);
  intake.lift(true);
  exit_condition(lemlib::Pose (-45.214, -1.133), 1);
  intake.lift(false);
  pros::delay(400);
  

  // TODO: auton times out when going for corner
  chassis.moveToPose(-53.017, -57.336, 197.5, 2000, {.forwards = true, .minSpeed = 127});
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