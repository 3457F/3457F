#include "main.h"

#include "util.hpp"

void red_rush_old() {
    chassis.setPose(-51.5, -59, 270);
  
    // go to middle mogo and grab it
    chassis.moveToPose(-14.562, -47.217, 251, 1250,{.forwards = false,
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
    chassis.moveToPose(-61.529, -30.524, 15, 2000, {.forwards = false, .minSpeed = 100});
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
void red_negative_awp_old() {
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
void red_pos_archived() {
    /** REPURPOSED IN NEW `red_pos()` FUNC (in `autons.cpp`) */
    chassis.setPose(-60.263, -13.198, 180);

    // scores on alliance stake
    chassis.turnToHeading(180-40, 730);
    pros::delay(450);
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;

    /** DELETED AT IOWA BECAUSE IT WAS COMPLETELY OFF */
    // waits to get out of alliance stake before reverting
    chassis.setPose(-60.263, -13.198, 180);

    // chassis.turnToHeading(180-31.5, 730);
    // old angle 180-35, 180-45 overshoot
    // scores on alliance stake
    chassis.turnToHeading(180-40, 730);
    pros::delay(450);
    arm.set_pos(arm.ALLIANCE_SCORE);
    waitd;

    arm.set_pos(arm.INIT_POS);

    // turns to mogo and tries getting it
    chassis.moveToPoint(
        -23.852
        , -30.495
        , 2000
        , {
            .maxSpeed = 60
        }
    );
    waitd;
    mogo.toggle();

    // get ring on field
    lemlib::Pose ring_on_field = chassis.getPose();

    intake.intake();
    turnAndMoveToPoint(
        ring_on_field.x - 4
        , -51.981
        , {
            .forwards = false
            , .mvMaxSpeed = 80
        }
    );
    // exist
    pros::delay(500);

    // come back
    chassis.moveToPoint(
        -23.852
        , -32.517
        , 1000
    );
    waitd;

    // turn around
    chassis.turnToPoint(
        -15.235
        , -23.901
        , 1000
        , {
            .forwards = false
        }
    );
    waitd;

    lemlib::Pose go_to_ladder = chassis.getPose();

    // open arm
    arm.set_pos(arm.ALLIANCE_SCORE);

    // k go
    chassis.moveToPoint(
        go_to_ladder.x + 2
        , go_to_ladder.y + 2
        , 1000
        , {
            .forwards = false
        }
    );
    waitd;
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