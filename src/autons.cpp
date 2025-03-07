#include "doinker.hpp"
#include "intake.hpp"
#include "main.h"
#include "pros/colors.hpp"
#include "pros/motors.h"
#include "pros/rtos.h"
#include "ui/Auton.hpp"
#include "util.hpp"

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"

#include "pros/rtos.hpp"

#include <chrono>
#include <future>
#include <set>

#define skillsMSpd .maxSpeed = 100

// declares pure pursuit files

// red nostack awp
ASSET(mecha_red_pos_awp_corner_3_txt);
ASSET(mecha_red_pos_awp_mogo_txt);
ASSET(mecha_red_pos_awp_stake_txt);
ASSET(two_ring_txt)

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

        pros::delay(100);
    } 
}

void test_auton() {
    chassis.setPose(0,0,0);

    // chassis.moveToPoint(0, 24, 1000, {.maxSpeed = 80});
    // waitd;

    // chassis.turnToHeading(45, 1000, {.maxSpeed = 80});
    // waitd;

    // chassis.moveToPoint(24, 48, 1000, {.maxSpeed = 80});

    chassis.moveToPose(24, 24, 45, 1250, {.lead = 0.6, .maxSpeed = 80});
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
    chassis.setPose(
        -54.726
        , -36.09
        , 90
    );
    
    arm.set_pos(arm.LOADIN_POS);
    chassis.moveToPoint(-46, -36.09, TO, {.maxSpeed=80});
    waitd; 
    intake.intake();
    chassis.turnToPoint(-26.8751, -25.061, 650);
    waitd;
    chassis.moveToPoint(-26.8751, -25.061, 850, {.maxSpeed=60});
    waitd;
    intake.intake_brake_mode = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST;
    intake.brake();
    arm.set_pos(arm.HOLD_POS);
    mogo.clamp();
    pros::delay(150);

    // second ring

    chassis.moveToPose(-21.77, -55.985, 340, 1500, {.forwards = false});
    pros::delay(250);
    intake.intake(); 
    waitd;
    pros::delay(450);
    chassis.moveToPose( -61.529, -30.524, 15, 2000, {.forwards = false, .minSpeed = 100  });
    waitd;

    doinker.toggle();
    chassis.moveToPoint(-62.504, -63.377, 1250, {.forwards = false});
    waitd;
    chassis.turnToHeading(310, 1200);
    waitd;

    // arm.set_pos(arm.LOADIN_POS);
    doinker.toggle();
    turnAndMoveToPoint(-56.75, -80.773, {.turnTO = 400, .moveTO = 1800, .forwards = false});
    waitd; 
    chassis.turnToHeading(180, 650);
    waitd;
    // alliance stake
    chassis.moveToPose(-59, -32.86, 156, 5000, {.forwards = false, .maxSpeed=80});
    waitd; 

    intake.brake();
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(250);

    chassis.moveToPoint(-39.158, -20.305, 700);
    waitd;
    arm.set_pos(arm.SCORE_POS);
    turnAndMoveToPoint(-9.572, -8.336, {.turnTO=750, .forwards=false});
    waitd;
}

void red_rush(){
    chassis.setPose(
        -54.473
        , -59
        , 270
    );

    // setting arm to dunk POS to ensure that arm doesn't get in the way of the hooks later on
    arm.set_pos(arm.DUNK_POS);

    // for some reason drifts time to time, gotta slow it down def, but currently reseraching a way.
    // only running the floating cuz there is not need for hooks
 
    chassis.moveToPose(
        -16.757
        , -47.412
        , 230
        , 1250
        , {
            .forwards = false
            , .maxSpeed = 127
            , .minSpeed = 111
        }
    );
    intake.intake();

    doinker.toggle();
    waitd;
    pros::delay(250);
    intake.brake();
    doinker.toggle();
    //doinker to touch/grab the goal
 
    pros::delay(420);

    //moving the robot and the goal back!
    chassis.moveToPoint(-34.487, -41.816, TO, {.forwards = true, .minSpeed = 100});
    waitd;
    doinker.toggle();

    pros::delay(250);

    chassis.turnToPoint(-15.536, -34.354, TO);
    waitd;
    doinker.toggle();

    chassis.moveToPoint(-26.84, -36.693, TO, {.maxSpeed=60});
    waitd;
    mogo.clamp();

    turnAndMoveToPoint(-57.066, -38.442, {.forwards = false, .async = true});
    chassis.waitUntil(4);
    intake.intake();
    waitd; 
    pros::delay(800);
   chassis.turnToHeading(0, 750);
   waitd;
   chassis.moveToPoint(-61.724,-59.609, 1000, {.forwards = false});
   waitd;
   pros::delay(400);
   chassis.turnToHeading(13, 600);
   waitd;
   doinker.toggle();
   chassis.moveToPoint(-65.817,-64.757, 600,{.forwards = false});
   waitd;
   chassis.turnToHeading(270, 1000);
   waitd;
   doinker.toggle();
//     chassis.waitUntil(4);


    
    
    // mogo.toggle();
    // doinker.toggle();

    // chassis.moveToPoint(-54.473, -59, TO);
    // waitd;
   

    // chassis.moveToPose(-19.453, -55.811, 82, 2000, {.forwards = true, .maxSpeed=60});
    // waitd;

    // mogo.toggle();
    // chassis.turnToHeading(90, 1400);
    // waitd;
    
    // chassis.moveToPoint(-18.596, -55.421, TO);
    // waitd;
    // mogo.toggle();
    
//     //going to clamp mogo
//     chassis.moveToPose(-6.778, -50.238, 270, 1200, {.forwards = true, .minSpeed = 60});
//     waitd;
//     mogo.toggle();
//     intake.intake();
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
// //     turnAndMoveToPoint(-41.066, -6.291, {.forwards = false, .mvMaxSpeed = 105});
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
    chassis.setPose(53.746, 16.9, 270);

    // chassis.turnToHeading(315, 600);
    // waitd;

    arm.set_pos(arm.LOADIN_POS);
    pros::delay(150);
    intake.intake();
    pros::delay(250);
    intake.intake_brake_mode = pros::E_MOTOR_BRAKE_COAST;
    intake.brake();
    arm.set_pos(arm.SCORE_POS);

    chassis.moveToPoint(56.864, 9.5, 1000, {.forwards = false});
    waitd;
    chassis.turnToPoint(67.85, 1, 600, {.forwards=false});
    waitd;

    arm.set_pos(arm.ALLIANCE_SCORE);

    pros::delay(750);

    chassis.moveToPoint(36.231, 32.856, TO);
    waitd;

    arm.set_pos(arm.HOLD_POS);

    chassis.turnToPoint(20.15, 26.75, 850);
    waitd;
    chassis.moveToPoint(20.15, 26.75, 1350, {.maxSpeed=50});
    waitd;
    mogo.clamp();

    pros::delay(500);
//fds
    chassis.moveToPose(chassis.getPose().x-20, chassis.getPose().y+18.25, 127, 1900, {.forwards = false, .minSpeed = 89.69});
    pros::delay(600);
    intake.intake();
    waitd;

    pros::delay(1000);

    // chassis.turnToHeading(180, TO, {.maxSpeed=60});
    // waitd;
    // chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y+20, TO, {.forwards=false,.maxSpeed=90});
    // waitd;

    // pros::delay(1000);

    chassis.moveToPoint(20.831, 29.373, 1300, {.minSpeed = 110});
    waitd;
    chassis.moveToPose(23.051, 52.954, 180, 1200, {.forwards= false, .maxSpeed=100, .minSpeed = 80});
    waitd;

    pros::delay(1000);
    chassis.turnToPoint(17.692, 17.68, TO, {.forwards=false});
    waitd;
    chassis.moveToPoint(17.692, 17.68, TO, {.forwards=false, .maxSpeed=70});
    waitd;
    arm.set_pos(arm.SCORE_POS);
}

void red_negative_awp() {
    chassis.setPose(-54.211, 16.319, 90);

    chassis.turnToHeading(45, 600);
    waitd;
    arm.set_pos(arm.HOLD_POS);
    chassis.moveToPoint(-57.25, 9, 600, {.forwards=false});
    waitd;
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(750);
    chassis.moveToPoint(-54.211, 16.319, 875);
    waitd;
    arm.set_pos(arm.HOLD_POS);
    chassis.turnToHeading(90, 750);
    waitd;

    chassis.moveToPose(-24.736, 28.412, 45, 1500, {.maxSpeed=90});
    waitd;
    mogo.clamp();
    pros::delay(250);

    chassis.turnToPoint(-9, 38.639, 850, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(-14.5 , 38.639, 2000, {.forwards=false});
    waitd;
    chassis.moveToPose(-16, 63.029, 170, 1500, {.forwards=false, .lead=0.1, .maxSpeed=70});
    waitd;
    pros::delay(750);

    // arm.set_pos(arm.LOADIN_POS);
    chassis.moveToPoint(-17.877, 32.151, 1000);
    waitd;
    chassis.turnToPoint(-30.182, 54.73, 750, {.forwards=false});
    waitd;
    chassis.moveToPoint(-30.182, 54.73, 850, {.forwards=false});
    waitd;

    // arm.set_pos(arm.INIT_POS);
    chassis.moveToPose(0, 0, 324.5, TO, {.forwards = false});
    waitd;
}

void red_negative_elims() {
    chassis.setPose(-52.135, 26.182, -90);

    chassis.turnToPoint(-6.793, 38.639, 350, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(-6.793, 38.639, 1500, {.forwards=false});
    waitd;

    pros::delay(350);
    intake.brake();

    chassis.moveToPose(-18.586, 23.586, -180, 1500, {.maxSpeed=75});
    waitd;
    mogo.clamp();
    pros::delay(250);

    chassis.turnToPoint(-18.586, 46.685, TO, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(-18.586, 46.685, TO, {.forwards=false});
    waitd;

    pros::delay(350);

    chassis.turnToPoint(-4.64, 50.837, 850, {.forwards=false});
    waitd;
    chassis.moveToPoint(-4.64, 50.837, TO, {.forwards=false});
    waitd;
    pros::delay(250);

    chassis.moveToPose(-23.586, 23.586, -180, TO);
    waitd;
    chassis.turnToPoint(-40.349, 40.975, 850, {.forwards=false});
    waitd;
    chassis.moveToPoint(-40.349, 40.975, TO, {.forwards=false});
    waitd;
    pros::delay(250);

    chassis.turnToPoint(-60.959, 60.959, 850, {.forwards=false});
    waitd;
    intake.lift(true);
    chassis.moveToPoint(-60.959, 60.959, 1000, {.forwards=false});
    waitd;
    chassis.turnToHeading(-225, TO);
    waitd;
    pros::delay(1000);
}
// set
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

ASSET(skills_1_txt);

// based on: https://www.youtube.com/watch?v=sKWXhcAJLJ4
void skills() {
    // initializing
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.setPose(
        -59.142
        , 0
        , -90
    );

    // scores onto alliance stake
    intake.intake();
    pros::delay(500);

    // moves towards mogo & clamps
    chassis.moveToPoint(
        // -47.982
        -47
        , 0
        , 1250
        , {
            .forwards = false
            , skillsMSpd
        }
    );
    waitd;
    intake.brake();
    // TODO: curves unnecssarily, need to fix
    chassis.moveToPose(
        // -49.463
        -47
        , 23.96
        , 0
        , 2000
        , {
            .maxSpeed = 60
        }
    );
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
    turnAndMoveToPoint(
        -27
        , get_first_ring.y
        , {
            .forwards = false
            , .mvMaxSpeed = 60
        }
    );

    // ring #2
    chassis.turnToHeading(
        225
        , 1000
    );
    waitd;
    chassis.moveToPose(
        22.153
        , 40.535
        , 270
        , 1000
        , {
            .forwards = false
        }
    );
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
    turnAndMoveToPoint(
        0, 39.777
        , {
            .turnTO = 500
        }
    );
    
    // turn towards the wall stake
    chassis.turnToHeading(
        -180
        , 1000
        , {skillsMSpd}
    );
    waitd;

    return;

    // ready up to collect top-most middle red ring
    intake.intake();
    arm.set_pos(arm.HOLD_POS);

    // move towards alliance stake, hopefully
    // scoring ring in front
    chassis.moveToPoint(
        0
        , 63.032
        , 500
        , {
            .forwards = false
        }
    );
    waitd;
    
    // score ring on wall stake!
    arm.set_pos(arm.DUNK_POS);
    pros::delay(750);
    arm.set_pos(arm.HOLD_POS);

    // auton reset but without the distance sensors ;-;
    // chassis.setPose(0, 62.776, chassis.getPose().theta);
    
    // move backwards
    chassis.moveToPoint(
        0
        , 47.107
        , TO
        , {skillsMSpd}
    );
    waitd;
    chassis.turnToHeading(90, 750);
    waitd;
    chassis.moveToPoint(-59.142, 48.204, 2000, {.forwards=false, skillsMSpd});
    waitd;
    pros::delay(250);
    
    chassis.moveToPose(-35.341, 68.149, 135, TO, {.forwards=false});
    waitd;
    pros::delay(150);
    chassis.swingToHeading(-75, DriveSide::RIGHT, TO);
    waitd;
    chassis.moveToPoint(-56.287, 60.18, TO);
    mogo.release();

    // end of first mogo
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
  chassis.turnToHeading(90, 650);
}

void aarav_skills(){
    chassis.setPose(60,0, 270);

    //scoring alliance stake
    intake.intake();
    pros::delay(250);
    intake.brake();

    //moving to first mogo 
    chassis.moveToPoint(-47, 0, 800, {.forwards = false});
    waitd;
    chassis.moveToPose(-47, -23.733, 180, 1100, {.forwards = true, .maxSpeed = 70});
    chassis.waitUntil(21);
    mogo.toggle();
    intake.intake();
    waitd;

    //going to rings
    turnAndMoveToPoint(-24.306, -25.585, { .moveTO = 1500,.forwards = false });
    waitd;
    //going down for the other ring
    turnAndMoveToPoint(23.678, -47.124, {.moveTO = 2000, .forwards = false });
    waitd; 
    arm.set_pos(arm.LOADIN_POS);

    //coming to the wall stake, and grabbing the ring near it
    chassis.moveToPoint(0, -47.5, 900);
    waitd;
    intake.brake();
    arm.set_pos(arm.DUNK_POS);

    //getting ring and score wall stake (maybe time for odom reset)
    intake.intake();
    turnAndMoveToPoint(0, -60, {.forwards = false});
    waitd;
    arm.set_pos(arm.SCORE_POS);
    pros::delay(500); 

    //coming back and aliging to 3 rings
    chassis.moveToPose(0, -47.5, 90, 1000,{.forwards = false, .minSpeed = 95});
    waitd;
    arm.set_pos(arm.START_POS);
    intake.intake();
    //getting 3 rings
    turnAndMoveToPoint(-61.334, -47.5, {.moveTO = 1700, .forwards = false});
    waitd;

    //going for the other ring near the corner
    turnAndMoveToPoint(-47.498, -59.3, {.moveTO = 650,.forwards = false});
    waitd;

    //goes to corner and drops
    turnAndMoveToPoint(-59, -59.3, {.forwards = true});
    waitd; 
    mogo.toggle();

    //neoooooow to the ring all the way down to the field, and to load it inot lb for alliacne stake
    intake.intake();
    arm.set_pos(arm.LOADIN_POS);
    turnAndMoveToPoint(46.242, -47.607, {.moveTO = 2600 , .forwards = false});
    waitd;

    //gets blue ring mogo 
    chassis.moveToPose(57.448, -26.559, 22.5, 1350, {.forwards = true, .minSpeed = 70});
    chassis.waitUntil(21);
    mogo.toggle();
    waitd; 

    //clearing corner
    chassis.moveToPose(57.448, -59.787, 315, 1250,{.forwards = false});
    intake.brake();
    chassis.waitUntil(5);
    doinker.toggle();
    waitd;
    //turns to get rings and also place goal into corner
    chassis.turnToHeading(128, 700);
    waitd;
    doinker.toggle();
    mogo.toggle();

    //aligsn to mogo goal
    chassis.moveToPose(47, -18.671, 0, 1500, {.forwards = true});
    waitd;
    //gets mogo
    chassis.moveToPoint(47, 0, 1900, {.maxSpeed = 65});
    chassis.waitUntil(18.5);
    mogo.toggle();

    //turns to alliance stake 
    chassis.turnToHeading(270, 670);
    waitd;
    arm.set_pos(arm.PREALLIANCE_SCORE);

    //goes to alliance stake (maybe a time to odom reset)
    chassis.moveToPoint(58.936, 0, 650, {.forwards = false});
    waitd;
    arm.set_pos(arm.ALLIANCE_SCORE);
    pros::delay(150);
    intake.intake();

    //moves back to reset
    chassis.moveToPoint(47, 0,650);
    arm.set_pos(arm.START_POS);
    waitd;

    //goes to the ring near ladder
    turnAndMoveToPoint(23.441, -23.831, {.moveTO = 1500, .forwards = false});
    waitd;
    pros::delay(200);
    intake.brake();

    //downnnnnn to the ladder and gets 3 rings
    chassis.moveToPose(-44.964, 45.256, 130, 3100, {.forwards = false});
    chassis.waitUntil(63);
    intake.intake();
    waitd;

    //gets rings
    turnAndMoveToPoint(-61.334, 47.01, {.moveTO = 650, .forwards = false});
    waitd;
    turnAndMoveToPoint(-47.108, 58.703, {.moveTO = 650, .forwards = false});
    waitd;

    //drops mogo (time to reset using side distnace?!)
    turnAndMoveToPoint(-61.919, 60.652, {.forwards = true});
    waitd;
    intake.brake();
    mogo.toggle();
    arm.set_pos(arm.LOADIN_POS);
    intake.intake();

    //comes and gets ring to wall stake
    turnAndMoveToPoint(-17.388, 45.061, {.forwards = false});
    waitd;

    // neoeowow to mogo
    chassis.moveToPose(-42.904, 26.714, 52, 2500, {.forwards = true});
    chassis.waitUntil(29);
    mogo.toggle();
    waitd;

    //aligns to wall stake and scores wall satke (maybe odom reset)
    chassis.moveToPose(0, 45, 180, 1900,{.forwards = false});
    waitd;
    arm.set_pos(arm.DUNK_POS);
    chassis.moveToPoint(0, 60, 1300, {.forwards = false});
    waitd;
    arm.set_pos(arm.LOADIN_POS);
    pros::delay(600);
    arm.set_pos(arm.START_POS);

    //goes to rings and collects them
    turnAndMoveToPoint(24, 47.275, {.moveTO = 1400, .forwards = false}) ;
    waitd;
    turnAndMoveToPoint(24, 23.624,{.moveTO = 1300, .forwards = false});
    waitd;
    turnAndMoveToPoint(46.925, 46.913,{.moveTO = 1300, .forwards = false});
    waitd;
    turnAndMoveToPoint(46.925, 58.801, {.forwards = false});
    waitd;
    turnAndMoveToPoint(46.535, 40.482, {.forwards = false});
    waitd;
    turnAndMoveToPoint(58.618, 47.108,  {.forwards = false});
    waitd;
    //clearing corner and dropping mogo
    pros::delay(350);
    intake.brake();
    doinker.toggle();
    chassis.moveToPose(64.061, 57.924, 200, 1500);
    waitd;
    chassis.turnToHeading(45, 750);
    waitd;
    mogo.toggle();
    doinker.toggle();

    //hang
    arm.set_pos(arm.LOADIN_POS);
    chassis.moveToPose(9.117, 8.52, 224, 3500, {.forwards = true, .maxSpeed = 85, .minSpeed = 85});
    waitd;
    arm.set_pos(arm.START_POS);
}

void blue_rush(){
    chassis.setPose(54.395,-33.055, 90);
    

    chassis.moveToPose(15.061, -47.802, 81.8, 1250, {.forwards = false, .minSpeed = 125});
    intake.intake();
    chassis.waitUntil(23);
    doinker.toggle();
    waitd;
    doinker.toggle();

    turnAndMoveToPoint(41.37, -32.6, {.forwards = true, . async = true});
    chassis.waitUntil(7);
    doinker.toggle();
    waitd;



    
}

void moveDist(double x, int timeout, bool forwards=true, float maxSpeed=127) {
    lemlib::Pose p = chassis.getPose();
    if (!forwards) { p.theta+=180; };
    chassis.moveToPoint(p.x+x*sin(p.theta), p.y+x*cos(p.theta), timeout, {.forwards=forwards, .maxSpeed=maxSpeed});
}

void red_neg(){
    chassis.setPose(-57.5, 23.5, 90);

    chassis.moveToPoint(-15, 23.5, 1300, {.forwards = true, .maxSpeed = 60});
    chassis.waitUntil(26);
    mogo.toggle();
    waitd;

    chassis.turnToPoint(chassis.getPose().x+3.975, 40.481, 540, {.forwards=false});
    waitd;
    chassis.moveToPoint(chassis.getPose().x+3.975, 40.481, 1000, {.forwards = false, .maxSpeed=110});
    intake.intake();
    waitd;
    pros::delay(500);
    chassis.turnToHeading(180, 650);
    waitd;
    chassis.moveToPoint(chassis.getPose().x, 61.997, 1200, {.forwards=false, .maxSpeed=110});
    waitd;

    pros::delay(350);

    intake.brake();
    chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y-30, TO);
    waitd;
    chassis.turnToPoint(chassis.getPose().x-13, chassis.getPose().y+25, 450, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(chassis.getPose().x-13, chassis.getPose().y+25, TO, {.forwards=false, .maxSpeed=110});
    arm.set_pos(arm.LOADIN_POS);
    waitd;

    pros::delay(250);

    chassis.turnToPoint(-54.883, 14.85, 720, {.forwards=false});
    waitd;
    chassis.moveToPoint(-54.883, 14.85, 1300, {.forwards=false});
    waitd;

    chassis.turnToHeading(48, 650);
    waitd;
    intake.outtake();
    pros::delay(85);
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    intake.brake();
    arm.set_pos(arm.ALLIANCE_SCORE);

    pros::delay(500);

    chassis.moveToPoint(-49.02, 20.731, TO);
    waitd;
    chassis.moveToPoint(-10, 0, TO, {.forwards=false, .maxSpeed=100});
    arm.set_pos(arm.HOLD_POS);
    waitd;
}