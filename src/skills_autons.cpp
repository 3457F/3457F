#include "main.h"

#define skillsMSpd .maxSpeed = 100

/**
 *
 *
 *
 *
 *
 *
 *
 * skulls
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
 * my crazy recoding (not complete) */
// based on: https://www.youtube.com/watch?v=sKWXhcAJLJ4
void skills() {
  // initializing
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  chassis.setPose(-59.142, 0, -90);

  // scores onto alliance stake
  intake.intake();
  pros::delay(500);

  // moves towards mogo & clamps
  chassis.moveToPoint(
      // -47.982
      -47, 0, 1250, {.forwards = false, skillsMSpd});
  waitd;
  intake.brake();
  // TODO: curves unnecssarily, need to fix
  chassis.moveToPose(
      // -49.463
      -47, 23.96, 0, 2000, {.maxSpeed = 60});
  // lemlib::Pose get_mogo_pose = chassis.getPose();
  // turnAndMoveToPoint(
  //     get_mogo_pose.x
  //     , 23.96
  //     , {
  //         .mvMaxSpeed = 60
  //     }
  // );
  waitd;
  mogo.clamp();
  pros::delay(150);

  // ring #1
  intake.intake();
  // chassis.turnToHeading(
  //     270,
  //     500
  // );
  lemlib::Pose get_first_ring = chassis.getPose();
  // chassis.moveToPoint(
  //     -32.952
  //     , get_first_ring.y
  //     , 1000
  //     , {
  //         .forwards = false
  //     }
  // );
  turnAndMoveToPoint(-27, get_first_ring.y,
                     {.forwards = false, .mvMaxSpeed = 60});

  // ring #2
  chassis.turnToHeading(225, 1000);
  waitd;
  chassis.moveToPose(22.153, 40.535, 270, 1000, {.forwards = false});
  waitd;

  // set arm up to get ring
  arm.set_pos(arm.LOADIN_POS);

  // lemlib::Pose get_blue_neg_rings = chassis.getPose();

  // // ring #3
  // // chassis.moveToPoint(52.633, 42.166, TO, {.forwards=false, skillsMSpd});
  // chassis.moveToPoint(
  //     47.178
  //     , get_blue_neg_rings.y
  //     , 1000
  //     , {
  //         .forwards = false
  //     }
  // );
  // waitd;
  // // move further, slower for ring #4
  // chassis.moveToPoint(
  //     58.806
  //     , get_blue_neg_rings.y
  //     , 1000
  //     , {
  //         .forwards = false
  //     }
  // );
  // waitd;

  // // wait for stuff to properly intake
  // pros::delay(1000);

  // so ring gets in arm
  pros::delay(250);
  // hopefully ring is in arm by now... don't wanna break the motor
  intake.brake();

  /** scoring on top wall stake (relative to pathjerry) */
  // comes back to align with wall stake
  turnAndMoveToPoint(0, 39.777, {.turnTO = 500});

  // turn towards the wall stake
  chassis.turnToHeading(-180, 1000, {skillsMSpd});
  waitd;

  return;

  // ready up to collect top-most middle red ring
  intake.intake();
  arm.set_pos(arm.HOLD_POS);

  // move towards alliance stake, hopefully
  // scoring ring in front
  chassis.moveToPoint(0, 63.032, 500, {.forwards = false});
  waitd;

  // score ring on wall stake!
  arm.set_pos(arm.DUNK_POS);
  pros::delay(750);
  arm.set_pos(arm.HOLD_POS);

  // auton reset but without the distance sensors ;-;
  // chassis.setPose(0, 62.776, chassis.getPose().theta);

  // move backwards
  chassis.moveToPoint(0, 47.107, TO, {skillsMSpd});
  waitd;
  chassis.turnToHeading(90, 750);
  waitd;
  chassis.moveToPoint(-59.142, 48.204, 2000, {.forwards = false, skillsMSpd});
  waitd;
  pros::delay(250);

  chassis.moveToPose(-35.341, 68.149, 135, TO, {.forwards = false});
  waitd;
  pros::delay(150);
  chassis.swingToHeading(-75, DriveSide::RIGHT, TO);
  waitd;
  chassis.moveToPoint(-56.287, 60.18, TO);
  mogo.release();

  // end of first mogo
}


void aarav_skills() {
  chassis.setPose(60, 0, 270);

  // scoring alliance stake
  intake.intake();
  pros::delay(250);
  intake.brake();

  // moving to first mogo
  chassis.moveToPoint(-47, 0, 800, {.forwards = false});
  waitd;
  chassis.moveToPose(-47, -23.733, 180, 1100,
                     {.forwards = true, .maxSpeed = 70});
  chassis.waitUntil(21);
  mogo.toggle();
  intake.intake();
  waitd;

  // going to rings
  turnAndMoveToPoint(-24.306, -25.585, {.moveTO = 1500, .forwards = false});
  waitd;
  // going down for the other ring
  turnAndMoveToPoint(23.678, -47.124, {.moveTO = 2000, .forwards = false});
  waitd;
  arm.set_pos(arm.LOADIN_POS);

  // coming to the wall stake, and grabbing the ring near it
  chassis.moveToPoint(0, -47.5, 900);
  waitd;
  intake.brake();
  arm.set_pos(arm.DUNK_POS);

  // getting ring and score wall stake (maybe time for odom reset)
  intake.intake();
  turnAndMoveToPoint(0, -60, {.forwards = false});
  waitd;
  arm.set_pos(arm.SCORE_POS);
  pros::delay(500);

  // coming back and aliging to 3 rings
  chassis.moveToPose(0, -47.5, 90, 1000, {.forwards = false, .minSpeed = 95});
  waitd;
  arm.set_pos(arm.START_POS);
  intake.intake();
  // getting 3 rings
  turnAndMoveToPoint(-61.334, -47.5, {.moveTO = 1700, .forwards = false});
  waitd;

  // going for the other ring near the corner
  turnAndMoveToPoint(-47.498, -59.3, {.moveTO = 650, .forwards = false});
  waitd;

  // goes to corner and drops
  turnAndMoveToPoint(-59, -59.3, {.forwards = true});
  waitd;
  mogo.toggle();

  // neoooooow to the ring all the way down to the field, and to load it inot lb
  // for alliacne stake
  intake.intake();
  arm.set_pos(arm.LOADIN_POS);
  turnAndMoveToPoint(46.242, -47.607, {.moveTO = 2600, .forwards = false});
  waitd;

  // gets blue ring mogo
  chassis.moveToPose(57.448, -26.559, 22.5, 1350,
                     {.forwards = true, .minSpeed = 70});
  chassis.waitUntil(21);
  mogo.toggle();
  waitd;

  // clearing corner
  chassis.moveToPose(57.448, -59.787, 315, 1250, {.forwards = false});
  intake.brake();
  chassis.waitUntil(5);
  doinker.toggle();
  waitd;
  // turns to get rings and also place goal into corner
  chassis.turnToHeading(128, 700);
  waitd;
  doinker.toggle();
  mogo.toggle();

  // aligsn to mogo goal
  chassis.moveToPose(47, -18.671, 0, 1500, {.forwards = true});
  waitd;
  // gets mogo
  chassis.moveToPoint(47, 0, 1900, {.maxSpeed = 65});
  chassis.waitUntil(18.5);
  mogo.toggle();

  // turns to alliance stake
  chassis.turnToHeading(270, 670);
  waitd;
  arm.set_pos(arm.PREALLIANCE_SCORE);

  // goes to alliance stake (maybe a time to odom reset)
  chassis.moveToPoint(58.936, 0, 650, {.forwards = false});
  waitd;
  arm.set_pos(arm.ALLIANCE_SCORE);
  pros::delay(150);
  intake.intake();

  // moves back to reset
  chassis.moveToPoint(47, 0, 650);
  arm.set_pos(arm.START_POS);
  waitd;

  // goes to the ring near ladder
  turnAndMoveToPoint(23.441, -23.831, {.moveTO = 1500, .forwards = false});
  waitd;
  pros::delay(200);
  intake.brake();

  // downnnnnn to the ladder and gets 3 rings
  chassis.moveToPose(-44.964, 45.256, 130, 3100, {.forwards = false});
  chassis.waitUntil(63);
  intake.intake();
  waitd;

  // gets rings
  turnAndMoveToPoint(-61.334, 47.01, {.moveTO = 650, .forwards = false});
  waitd;
  turnAndMoveToPoint(-47.108, 58.703, {.moveTO = 650, .forwards = false});
  waitd;

  // drops mogo (time to reset using side distnace?!)
  turnAndMoveToPoint(-61.919, 60.652, {.forwards = true});
  waitd;
  intake.brake();
  mogo.toggle();
  arm.set_pos(arm.LOADIN_POS);
  intake.intake();

  // comes and gets ring to wall stake
  turnAndMoveToPoint(-17.388, 45.061, {.forwards = false});
  waitd;

  // neoeowow to mogo
  chassis.moveToPose(-42.904, 26.714, 52, 2500, {.forwards = true});
  chassis.waitUntil(29);
  mogo.toggle();
  waitd;

  // aligns to wall stake and scores wall satke (maybe odom reset)
  chassis.moveToPose(0, 45, 180, 1900, {.forwards = false});
  waitd;
  arm.set_pos(arm.DUNK_POS);
  chassis.moveToPoint(0, 60, 1300, {.forwards = false});
  waitd;
  arm.set_pos(arm.LOADIN_POS);
  pros::delay(600);
  arm.set_pos(arm.START_POS);

  // goes to rings and collects them
  turnAndMoveToPoint(24, 47.275, {.moveTO = 1400, .forwards = false});
  waitd;
  turnAndMoveToPoint(24, 23.624, {.moveTO = 1300, .forwards = false});
  waitd;
  turnAndMoveToPoint(46.925, 46.913, {.moveTO = 1300, .forwards = false});
  waitd;
  turnAndMoveToPoint(46.925, 58.801, {.forwards = false});
  waitd;
  turnAndMoveToPoint(46.535, 40.482, {.forwards = false});
  waitd;
  turnAndMoveToPoint(58.618, 47.108, {.forwards = false});
  waitd;
  // clearing corner and dropping mogo
  pros::delay(350);
  intake.brake();
  doinker.toggle();
  chassis.moveToPose(64.061, 57.924, 200, 1500);
  waitd;
  chassis.turnToHeading(45, 750);
  waitd;
  mogo.toggle();
  doinker.toggle();

  // hang
  arm.set_pos(arm.LOADIN_POS);
  chassis.moveToPose(9.117, 8.52, 224, 3500,
                     {.forwards = true, .maxSpeed = 85, .minSpeed = 85});
  waitd;
  arm.set_pos(arm.START_POS);
}