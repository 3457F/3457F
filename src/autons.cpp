#include "main.h"
#include "util.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "pros/rtos.hpp"

#include <chrono>
#include <set>

// declares pure pursuit files

// red nostack awp
ASSET(mecha_red_pos_awp_corner_txt);
ASSET(mecha_red_pos_awp_mogo_txt);
ASSET(mecha_red_pos_awp_stake_txt);

void print_robot_pos(void* chassisVoid) {
    lemlib::Chassis* chassis
        = static_cast<
            lemlib::Chassis*
        >(chassisVoid);

    while (true) {
        lemlib::Pose pos = chassis->getPose();

        std::cout << "x: " << pos.x
				  << " | y: " << pos.y
				  << " | theta: " << pos.theta
				  << std::endl;

        pros::delay(20);
    } 
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

void red_positive_awp_nostack() {
    // START POS WHEN APPROACHING MOGO STRAIGHT
    // chassis.setPose(
    //     -58.386
    //     , -24
    //     , 90
    // );

    // MOGO APPROACH CODE FOR STRAIGHT
    // chassis.moveToPoint(
    //     // -30.323
    //     -28.69
    //     // , -23.647
    //     , -24
    //     , 1500
    //     , {
    //         .maxSpeed = 70
    //     }
    // );
    // chassis.waitUntil(25.5);
    // mogo.clamp();
    // waitd;
    
    // // approaches mogo
    // chassis.follow(
    //     mecha_red_pos_awp_mogo_txt
    //     , 15
    //     , 1500
    // );
    // chassis.waitUntil(25.5);
    // mogo.toggle();
    // waitd;

    chassis.setPose(
        -58.568
        , -40.501
        , 90
    );
    
    /* turn in place and move to mogo */
    chassis.turnToPoint(
        -26.332
        , -26.5
        , 750
        , {
            .direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
        }
    );
    waitd;

    chassis.moveToPoint(
        -26.332
        , -26.5
        , 1500
        , {
            .maxSpeed = 60
        }
    );
    waitd;
    chassis.waitUntil(27);
    mogo.clamp();
    // waitd;

    // turns INTAKE SIDE towards the first stack
    chassis.turnToPoint(
        -24.395
        , -48.771
        , 1000
        , {
            .forwards = false
            , .direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE
        }
    );
    waitd;

    // start intake to score preload + ring stack!
    intake.intake();

    // goes to ring stack
    chassis.moveToPoint(
        -24.395
        , -48.771
        , 1500
        , {
            .forwards = false
        }
    );
    waitd;

    // go to corner stack
    chassis.follow(
        mecha_red_pos_awp_corner_txt
        , 5
        , 2000
        , false
    );
    // TODO: add color sensor logic to queue intake robot until after turn
    // TODO: cumulative dist or dist after last call?
    // lift ONCE the ring stack ring is in
    chassis.waitUntil(7);
    intake.lift(1);
    waitd; 

    // queues arm so next ring goes into arm; CORNER RINGS NOT LIKELY TO EASILIY ENTER ROBOT
    arm.set_pos(arm.LOADIN_POS);

    /* turns like ajksldfjaslkdfjasdf */
    // TODO: color sort
    chassis.turnToHeading(
        90
        , 750
    );
    waitd;

    // drops intake... hopefully onto red ring!
    intake.toggle();

    chassis.turnToHeading(
        45
        , 750
    );

    chassis.turnToHeading(
        90
        , 750
    );

    chassis.turnToHeading(
        45
        , 750
    );

    // TODO: hopefully alr oriented w point...?
    // moves backwards
    chassis.moveToPoint(
        -47.054
        , -46.969
        , 1500
    );

    /* moves towards alliance steak */
    chassis.turnToHeading(
        180
        , 750
    );

    chassis.follow(
        mecha_red_pos_awp_stake_txt
        , 5
        , 1500
        , false
    );

    // extend arm to score!
    // TODO: ARM GETS STUCK WHEN SCORING ON ALLIANCE
    arm.set_pos(arm.ALLIANCE_SCORE);

    pros::delay(750);

    arm.set_pos(arm.START_POS);
}

void red_negative_five_ring(){
    
    //Where the robot is placed the ring is going to the side of the bot, not in the intake. (if you need a visual pranesh js let aarav know)
        //to futher explain the intake will be rushing to the top ring (near auton line), and having a ring in the intake will like spit out the ring
            // on the last bot we able to carry two rings cuz we had the hooks and floating moving separtly, unlike where ts robot's intake moves all at once cuz there is one motor
    
    chassis.setPose(-54.319, 31.127, 270);

    //zooming to the rings on auton line (going for the ring that is like on the right when looking at red alliance driver station)
    turnAndMoveToPoint(-10.177, 42.041, { .moveTO = 1300, .forwards = false});
    intake.intake(); 
    waitd;
    //to lazy to stop intake cuz the intake likes to stroke the ring lol (ts bascially allows us to not have to tune when to stop intake <3 limit switch)
    intake.is_ring_on_top(); 
    intake.brake();

    //moving to mogo (if ts works first try i get $5 @codemygame)
    chassis.moveToPose(-17.875, 27.132, 56.5, 1950, {.forwards = true});
    waitd;
    mogo.toggle();

    //going and intaking to the middle ring stack (ring stack: red ring is bottom and blue ring is ontop)
    chassis.moveToPose(-17.875, 27.132, 56.5, 1200, {.forwards = false}); 
    //This delay is for the est time that the robot takes to turn (ion want the ring to go flying thas why delay)
    pros::delay(650);
    intake.intake(); 
    waitd;

    //Going to where the preload was (Pranesh if you are confused please refer to the first comment, before where we set the pose)
    chassis.moveToPose(-48.959, 24.111, 54, 1500, {.forwards = false});
    waitd; 
    
    //moving to the ring stack
    chassis.moveToPose(-47.595, 6.961, 355, 1500, {.forwards = false});
    intake.lift(true);
    waitd;

    //this is so when the red ring gets to the limit switch, and the 800ms delay to know that the ring is scored and we can stop the intake before we pick up the blue ring!!!
    intake.lift(false);
    intake.is_ring_on_top(); 
    pros::delay(800);
    intake.brake();

}

void move_forward() {
    chassis.setPose(
        0
        , 0
        , 90
    );

    chassis.moveToPoint(
        24
        , 0
        , 1000
    );
}