#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
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

/**
 *
 *
 *
 *
 *
 *
 *
 * NEW AUTONSSSSS
 *
 *
 *
 *
 *
 *
 *
 *
 */

/**
 * Designed as a backup, in case our odometry fails, so that we can still leave
 * the starting line
 */
void move_forward() {
  chassis.setPose(0, 0, 0);

  chassis.moveToPoint(0, 7, 1000);
  waitd;
}

void red_positive_awp_nostack() {
  chassis.setPose(-54.726, -36.09, 90);

  arm.set_pos(arm.LOADIN_POS);
  chassis.moveToPoint(-46, -36.09, TO, {.maxSpeed = 80});
  waitd;

  intake.intake();
  chassis.turnToPoint(-26.8751, -25.061, 650);
  waitd;

  chassis.moveToPoint(-26.8751, -25.061, 850, {.maxSpeed = 60});
  waitd;

  intake.intake_brake_mode = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST;
  intake.brake();
  arm.set_pos(arm.HOLD_POS);
  mogo.clamp();
  pros::delay(150);

  // second ring

  turnAndMoveToPoint(-21.77, -55.98, {.forwards = false, .async = true});
  pros::delay(250);
  intake.intake();
  waitd;

  pros::delay(450);
  chassis.moveToPose(-61.529, -30.524, 15, 2000,
                     {.forwards = false, .minSpeed = 100});
  waitd;

  doinker.toggle();
  chassis.moveToPoint(-62.504, -63.377, 1250, {.forwards = false});
  waitd;
  chassis.turnToHeading(310, 1200);
  waitd;

  // arm.set_pos(arm.LOADIN_POS);
  doinker.toggle();

  arm.set_pos(arm.SCORE_POS);
  turnAndMoveToPoint(-16.9, -18.179, {.forwards = false});

  // got rid of wall stake
  //  turnAndMoveToPoint(-56.75, -80.773, {.turnTO = 400, .moveTO = 1800,
  //  .forwards = false}); waitd; chassis.turnToHeading(180, 650); waitd;
  //  // alliance stake
  //  chassis.moveToPose(-59, -32.86, 156, 5000, {.forwards = false,
  //  .maxSpeed=80}); waitd;

  // intake.brake();
  // arm.set_pos(arm.ALLIANCE_SCORE);
  // pros::delay(250);

  // chassis.moveToPoint(-39.158, -20.305, 700);
  // waitd;
  // arm.set_pos(arm.SCORE_POS);
  // turnAndMoveToPoint(-9.572, -8.336, {.turnTO=750, .forwards=false});
  // waitd;
}

void turnforrush() {
  chassis.setPose(-51.5, -59, 270);
  chassis.turnToHeading(250, 1000);
}

void red_rush_barcbots() {
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

void red_rush() {
  chassis.setPose(-51.5, -59, 270);

  // go to middle mogo and grab it
  chassis.moveToPose(-14.562, -47.217, 251, 1250,
                     {.forwards = false,
                      // TODO: bc of such a high speed, the turn at the end
                      // sometimes drifts .minSpeed = 118
                      .minSpeed = 110});
  intake.intake();
  chassis.waitUntil(4);
  doinker.toggle();
  waitd;

  // TODO: what is the point of this ;-;
  intake.brake();
  // releases doinker, hooking properly onto the mogo
  doinker.toggle();
  pros::delay(500);

  // come back, pulling mogo with it
  chassis.moveToPoint(-51.5, -59, 1300);
  chassis.waitUntil(3);
  // "unrelease" doinker, so that mogo is dropped
  // TODO: might be inconsistent if doinker gets stuck!
  doinker.toggle();
  waitd;

  // turns towards the second mogo
  chassis.turnToHeading(37, 700);
  doinker.toggle();
  waitd;

  // goes and clamps second mogo
  chassis.moveToPoint(-24.988, -27.728, 1750,
                      {.forwards = true, .maxSpeed = 70});
  waitd;

  mogo.toggle();
  intake.intake();

  // comes back
  turnAndMoveToPoint(-61.627, -43.319, {.forwards = false});
  waitd;

  // turns towards corner
  chassis.turnToHeading(10, 600);
  waitd;

  // extend doinker
  doinker.toggle();

  // turn, clearing corner
  chassis.turnToHeading(300, 500);

  // turn back, hopefully aligned with a ring
  chassis.turnToHeading(0, 500);

  // chassis.moveToPose(-13.685, -60.469, 0, 1500, {.forwards = false});
  // waitd;
  // intake.intake();
  // turnAndMoveToPoint(-58.801,-25.39, {.forwards = false});
  // waitd;
  // chassis.turnToHeading(0, 650);

  // // doinker.toggle();

  // chassis.moveToPose(-26.45, -27.728, 37, 1300);
  // waitd;

  //     chassis.waitUntil(4);

  // mogo.toggle();
  // doinker.toggle();

  // chassis.moveToPoint(-54.473, -59, TO);
  // waitd;

  // chassis.moveToPose(-19.453, -55.811, 82, 2000, {.forwards = true,
  // .maxSpeed=60}); waitd;

  // mogo.toggle();
  // chassis.turnToHeading(90, 1400);
  // waitd;

  // chassis.moveToPoint(-18.596, -55.421, TO);
  // waitd;
  // mogo.toggle();

  //     //going to clamp mogo
  //     chassis.moveToPose(-6.778, -50.238, 270, 1200, {.forwards = true,
  //     .minSpeed = 60}); waitd; mogo.toggle(); intake.intake();
  //     pros::delay(450);

  //     //moves towards our positive corner and drops mogo.
  //     turnAndMoveToPoint( -47.985, -61.671, {.forwards =true});
  //     waitd;
  //     mogo.toggle();

  //     // chassis.moveToPose(-23.592, -32.826, 0, 1150, {.forwards = true});
  //     // waitd;
  //     chassis.moveToPose(-24.592,-27.826 , 0, 2500, {.forwards = true,});
  //     waitd;
  //     mogo.toggle();

  // //     intake.lift(true);
  // //     turnAndMoveToPoint(-41.066, -6.291, {.forwards = false, .mvMaxSpeed
  // = 105});
  // //     waitd;
  // //    intake.lift(false);
  // //    pros::delay(860);

  //     chassis.turnToPoint(-75, 0, 650, {.forwards = false});
  //     waitd;
  //     intake.brake();
  //     chassis.moveToPose(-39.475, -54.537, 342, 1300);
  //
}
// just a mirror of aarav's auton
void blue_negative_awp() {
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

  // chassis.turnToPoint(17.692, 17.68, TO, {.forwards = false});
  // waitd;

  // chassis.moveToPoint(17.692, 17.68, TO, {.forwards = false, .maxSpeed = 70});
  // intake.brake();
  // waitd;

  // arm.set_pos(arm.SCORE_POS);

  chassis.moveToPoint(64.223, -63.587, 10000);
}

void red_negative_awp() {
  chassis.setPose(-54.211, 16.319, 90);

  chassis.turnToHeading(45, 600);
  waitd;

  arm.set_pos(arm.HOLD_POS);
  chassis.moveToPoint(-57.25, 9, 600, {.forwards = false});
  waitd;

  arm.set_pos(arm.ALLIANCE_SCORE);
  pros::delay(750);
  chassis.moveToPoint(-54.211, 16.319, 875);
  waitd;

  arm.set_pos(arm.HOLD_POS);
  chassis.turnToHeading(90, 750);
  waitd;

  chassis.moveToPose(-24.736, 28.412, 45, 1500, {.maxSpeed = 80});
  waitd;
  mogo.clamp();
  pros::delay(250);

  chassis.turnToPoint(-9, 38.639, 850, {.forwards = false});
  waitd;

  intake.intake();
  chassis.moveToPoint(-14.5, 38.639, 2000, {.forwards = false});
  waitd;

  chassis.moveToPose(-16, 63.029, 170, 1500,
                     {.forwards = false, .lead = 0.1, .maxSpeed = 70});
  waitd;
  pros::delay(750);

  // arm.set_pos(arm.LOADIN_POS);
  chassis.moveToPoint(-17.877, 32.151, 1000);
  waitd;

  chassis.turnToPoint(-30.182, 54.73, 750, {.forwards = false});
  waitd;

  chassis.moveToPoint(-30.182, 54.73, 850, {.forwards = false});
  waitd;

  // arm.set_pos(arm.INIT_POS);
  chassis.moveToPose(0, 0, 324.5, TO, {.forwards = false});
  waitd;
}

void red_negative_elims() {
  chassis.setPose(-52.135, 26.182, -90);

  chassis.turnToPoint(-6.793, 38.639, 350, {.forwards = false});
  waitd;
  intake.intake();
  chassis.moveToPoint(-6.793, 38.639, 1500, {.forwards = false});
  waitd;

  pros::delay(350);
  intake.brake();

  chassis.moveToPose(-18.586, 23.586, -180, 1500, {.maxSpeed = 75});
  waitd;
  mogo.clamp();
  pros::delay(250);

  chassis.turnToPoint(-18.586, 46.685, TO, {.forwards = false});
  waitd;
  intake.intake();
  chassis.moveToPoint(-18.586, 46.685, TO, {.forwards = false});
  waitd;

  pros::delay(350);

  chassis.turnToPoint(-4.64, 50.837, 850, {.forwards = false});
  waitd;
  chassis.moveToPoint(-4.64, 50.837, TO, {.forwards = false});
  waitd;
  pros::delay(250);

  chassis.moveToPose(-23.586, 23.586, -180, TO);
  waitd;
  chassis.turnToPoint(-40.349, 40.975, 850, {.forwards = false});
  waitd;
  chassis.moveToPoint(-40.349, 40.975, TO, {.forwards = false});
  waitd;
  pros::delay(250);

  chassis.turnToPoint(-60.959, 60.959, 850, {.forwards = false});
  waitd;
  intake.lift(true);
  chassis.moveToPoint(-60.959, 60.959, 1000, {.forwards = false});
  waitd;
  chassis.turnToHeading(-225, TO);
  waitd;
  pros::delay(1000);
}

void red_negative_five_ring() {

  // Where the robot is placed the ring is going to the side of the bot, not in
  // the intake. (if you need a visual pranesh js let aarav know) to futher
  // explain the intake will be rushing to the top ring (near auton line), and
  // having a ring in the intake will like spit out the ring
  //  on the last bot we able to carry two rings cuz we had the hooks and
  //  floating moving separtly, unlike where ts robot's intake moves all at once
  //  cuz there is one motor

  chassis.setPose(-54.319, 31.127, 270);

  // zooming to the rings on auton line (going for the ring that is like on the
  // right when looking at red alliance driver station)
  turnAndMoveToPoint(-10.177, 42.041, {.moveTO = 1300, .forwards = false});
  intake.intake();
  waitd;
  // to lazy to stop intake cuz the intake likes to stroke the ring lol (ts
  // bascially allows us to not have to tune when to stop intake <3 limit
  // switch)
  intake.is_ring_on_top();
  intake.brake();

  // moving to mogo (if ts works first try i get $5 @codemygame)
  chassis.moveToPose(-17.875, 27.132, 56.5, 1950, {.forwards = true});
  waitd;
  mogo.toggle();

  // going and intaking to the middle ring stack (ring stack: red ring is bottom
  // and blue ring is ontop)
  chassis.moveToPose(-17.875, 27.132, 56.5, 1200, {.forwards = false});
  // This delay is for the est time that the robot takes to turn (ion want the
  // ring to go flying thas why delay)
  pros::delay(650);
  intake.intake();
  waitd;

  // Going to where the preload was (Pranesh if you are confused please refer to
  // the first comment, before where we set the pose)
  chassis.moveToPose(-48.959, 24.111, 54, 1500, {.forwards = false});
  waitd;

  // moving to the ring stack
  chassis.moveToPose(-47.595, 6.961, 355, 1500, {.forwards = false});
  intake.lift(true);
  waitd;

  // this is so when the red ring gets to the limit switch, and the 800ms delay
  // to know that the ring is scored and we can stop the intake before we pick
  // up the blue ring!!!
  intake.lift(false);
  intake.is_ring_on_top();
  pros::delay(800);
  intake.brake();
}

void blue_rush() {
  chassis.setPose(54.395, -33.055, 90);

  chassis.moveToPose(15.061, -47.802, 81.8, 1250,
                     {.forwards = false, .minSpeed = 125});
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

void moveDist(double x, int timeout, bool forwards = true,
              float maxSpeed = 127) {
  lemlib::Pose p = chassis.getPose();
  if (!forwards) {
    p.theta += 180;
  };
  chassis.moveToPoint(p.x + x * sin(p.theta), p.y + x * cos(p.theta), timeout,
                      {.forwards = forwards, .maxSpeed = maxSpeed});
}

void red_neg() {
  chassis.setPose(-57.534, 12.808, 45);

  chassis.moveToPoint(-58.534, 1.808, 1300, {.forwards = true, .maxSpeed = 60});
  chassis.waitUntil(26);
  mogo.toggle();
  waitd;

  // chassis.turnToPoint(chassis.getPose().x+5.75, 40.481, 540,
  // {.forwards=false});
  chassis.turnToHeading(212, 700);
  waitd;
  chassis.moveToPoint(chassis.getPose().x + 6.1, 40.481, 1200,
                      {.forwards = false});
  intake.intake();
  waitd;
  pros::delay(500);
  chassis.turnToHeading(180, 650);
  waitd;
  chassis.moveToPoint(chassis.getPose().x + 2.75, 59.997, 1200,
                      {.forwards = false, .maxSpeed = 110});
  waitd;

  pros::delay(350);

  chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y - 30, TO);
  waitd;
  chassis.turnToPoint(chassis.getPose().x - 15, chassis.getPose().y + 25, 450,
                      {.forwards = false});
  waitd;
  intake.intake();
  chassis.moveToPoint(chassis.getPose().x - 15, chassis.getPose().y + 25, TO,
                      {.forwards = false});
  arm.set_pos(arm.LOADIN_POS);
  waitd;

  pros::delay(250);

  chassis.turnToPoint(-57.216, 12.975, 720, {.forwards = false});
  waitd;
  chassis.moveToPoint(-57.216, 12.975, 1400, {.forwards = false});
  pros::delay(500);
  arm.set_pos(arm.HOLD_POS);
  waitd;
  chassis.turnToHeading(41, 800);
  waitd;
  intake.outtake();
  pros::delay(85);
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  intake.brake();
  arm.set_pos(arm.ALLIANCE_SCORE);

  pros::delay(500);

  chassis.moveToPoint(-56.657, 20.731, TO);
  waitd;
  chassis.moveToPoint(-10, 12, TO, {.forwards = false, .maxSpeed = 100});
  arm.set_pos(arm.HOLD_POS);
  waitd;
  arm.set_pos(arm.HOLD_POS);
}

// IMPORTANT: **STARTING DELAY 3 SECONDS**
void red_pos() {
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
  arm.set_pos(arm.HOLD_POS);
  mogo.clamp();
  waitd;

  pros::delay(500);

  // keep arm up for ladder

  // turn towards bottom ring
  intake.intake();
  turnAndMoveToPoint(-23.597, -46.974,
                     {.turnTO = 800, .moveTO = 1500, .forwards = false});
  waitd;
  intake.brake();

  // aligns so that the robot doinker is in position for the corner
  chassis.moveToPose(-58.045, -33.817, 10, 1500, {.forwards = false});
  // does this so ring is not flung out
  // while the robot is turning
  chassis.waitUntil(7);
  intake.intake();
  waitd;
  doinker.toggle();
  // chassis.follow(
  //     red_pos_steep_corner_txt,
  //     5,
  //     1000,
  //     false
  // );
  // chassis.waitUntil(7);
  // intake.intake();
  // waitd;
  // doinker.toggle();

  // moves towards corner
  // -63.069 -54.39
  turnAndMoveToPoint(
      // -63.787,
      -63.069,

      // -56.064,
      -54.39,
      {
          .forwards = false,
      });
  waitd;

  // turns, kicking rings in corner with doinker
  chassis.turnToHeading(290, 800);
  waitd;
  // brings up doinker so that it doesn't prevent us from intaking the new
  // ring
  doinker.toggle();
  pros::delay(750);

  // moves into the kicked rings, hopefully getting red
  // w old angle: (-56.131, -65.394)
  chassis.moveToPoint(-46.802, -63.002, 800, {.forwards = false});
  intake.intake();
  waitd;

  arm.set_pos(arm.SCORE_POS);
  // turns, moves to ladder (ARM STILL UP!)
  turnAndMoveToPoint(
      -20.248, -21.138,
      {.forwards = false, .async = true, .mvMaxSpeed = 80, .waitUntil = 5});
  mogo.toggle();
  intake.brake();
  waitd;
}

// IMPORTANT: **STARTING DELAY 3 SECONDS**
void red_pos_with_corner() {
  chassis.setPose(-60.263, -13.198, 180);

  pros::delay(3000);

  // chassis.turnToHeading(180-31.5, 730);
  chassis.turnToHeading(180 - 35, 730);
  waitd;

  // scores on alliance stake
  arm.set_pos(arm.ALLIANCE_SCORE);
  pros::delay(1000);

  // moves out of alliance stake
  chassis.moveToPoint(chassis.getPose().x + 10, chassis.getPose().y - 10, TO);
  waitd;

  // waits to get out of alliance stake before reverting
  arm.set_pos(arm.INIT_POS);

  // turns to mogo and tries getting it
  chassis.moveToPoint(-23.852, -30.495, 2000, {.maxSpeed = 60});
  waitd;
  mogo.toggle();

  // get ring on field
  lemlib::Pose ring_on_field = chassis.getPose();

  intake.intake();
  turnAndMoveToPoint(ring_on_field.x - 4, -51.981,
                     {.forwards = false, .mvMaxSpeed = 80});
  // // exist
  // pros::delay(500);

  // immediately turns and moves towards corner
  turnAndMoveToPoint(-55.196, -60.07, {.forwards = false});

  // turns down so doinker yeah
  chassis.turnToHeading(0, 750);

  // // come back
  // chassis.moveToPoint(
  //     -23.852
  //     , -32.517
  //     , 1000
  // );
  // waitd;

  // // turn around
  // chassis.turnToPoint(
  //     -15.235
  //     , -23.901
  //     , 1000
  //     , {
  //         .forwards = false
  //     }
  // );
  // waitd;

  // lemlib::Pose go_to_ladder = chassis.getPose();

  // // open arm
  // arm.set_pos(arm.ALLIANCE_SCORE);

  // // k go
  // chassis.moveToPoint(
  //     go_to_ladder.x + 2
  //     , go_to_ladder.y + 2
  //     , 1000
  //     , {
  //         .forwards = false
  //     }
  // );
  // waitd;
}

void red_neg_2() {
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
  chassis.moveToPose(-16.893, 11.583, 315, 1500,
                     {.forwards = false, .minSpeed = 120});
  chassis.waitUntil(40);
  arm.set_pos(arm.SCORE_POS);
  waitd;

  // chassis.turnToHeading(212, 700);
  // waitd;
  // chassis.moveToPoint(chassis.getPose().x+11.0, 43.481, 2000, {.forwards =
  // false}); intake.intake(); waitd; pros::delay(750);
  // chassis.turnToHeading(180, 650);
  // waitd;
  // chassis.moveToPoint(chassis.getPose().x+2.65, 63.997, 1400,
  // {.forwards=false, .maxSpeed=80}); waitd;

  // pros::delay(1000);

  // chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y-30, TO);
  // waitd;
  // chassis.turnToPoint(chassis.getPose().x-7.5, chassis.getPose().y+15.5, 450,
  // {.forwards=false}); waitd; intake.intake();
  // chassis.moveToPoint(chassis.getPose().x-7.5, chassis.getPose().y+15.5, TO,
  // {.forwards=false}); waitd;

  // chassis.moveToPoint(-46.621, 32.491, TO);
  // waitd;

  // chassis.moveToPoint(-31.809, 27.035, 2000, {.maxSpeed=70});
  // waitd;
  // mogo.toggle();

  //     // chassis.turnToPoint(chassis.getPose().x+5.75, 40.481, 540,
  //     {.forwards=false}); chassis.turnToHeading(212, 700); waitd;
  //     arm.set_pos(arm.INIT_POS);
  //     chassis.moveToPoint(chassis.getPose().x+11.0, 43.481, 2000, {.forwards
  //     = false}); intake.intake(); waitd; pros::delay(750);
  //     chassis.turnToHeading(180, 650);
  //     waitd;
  //     chassis.moveToPoint(chassis.getPose().x+2.65, 63.997, 1400,
  //     {.forwards=false, .maxSpeed=80}); waitd;

  //     pros::delay(1000);

  //     chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y-30, TO);
  //     waitd;
  //     chassis.turnToPoint(chassis.getPose().x-7.5, chassis.getPose().y+15.5,
  //     450, {.forwards=false}); waitd; intake.intake();
  //     chassis.moveToPoint(chassis.getPose().x-7.5, chassis.getPose().y+15.5,
  //     TO, {.forwards=false}); waitd;

  //     pros::delay(500);

  //     // chassis.moveToPoint(-56.657, 20.731, TO);
  //     waitd;
  //     chassis.moveToPoint(0, 0, TO, {.forwards=false});
  //     // intake.brake();
  //     arm.set_pos(arm.DUNK_POS);
  //     waitd;
  //     arm.set_pos(arm.ALLIANCE_SCORE);
}

void blue_safe() {
  chassis.setPose(57.5, 23.5, 270);

  chassis.moveToPoint(28, 23.5, 1300, {.forwards = true, .maxSpeed = 75});
  chassis.waitUntil(25);
  mogo.toggle();
  waitd;

  chassis.turnToHeading(180, 700);
  waitd;
  intake.intake();

  chassis.moveToPoint(27.728, 50.421, 1500, {.forwards = false});
  waitd;

  pros::delay(1200);

  arm.set_pos(arm.SCORE_POS);

  turnAndMoveToPoint(15.451, 22.942, {.forwards = false});
}

void no_auton() { chassis.setPose(0, 0, 0); }
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