#include "main.h"

#include "util.hpp"

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