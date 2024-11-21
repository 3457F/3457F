#include "intake.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "mogo.hpp"
#include "pros/imu.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <chrono>

#define TO 1200 // default timeout for when im lazy to specify
#define waitd chassis.waitUntilDone()
#define MAX_SPEED 127.0

// struct tamtpParams {
//     int turnTO = TO;
//     int moveTO = TO;

//     bool fwd = true;

//     bool async = false;

//     float mvMaxSpeed = MAX_SPEED;

//     /** runs `chassis.moveToPose()` instead of `chassis.moveToPoint()`
//     after turning to the point. it was more of a test on my side to see
//     why the robot kept turning right before it reached  */
//     bool move_to_pose = false; 
// };

// void turnAndMoveToPoint(
//     float x
//     , float y
//     , tamtpParams params = {}
// ) {
//     chassis.turnToPoint(
//         x
//         , y
//         , params.turnTO
//         , { .forwards = params.fwd }
//     );
//     waitd;

//     if (params.move_to_pose) {
//         lemlib::Pose pose = chassis.getPose();
        
//         chassis.moveToPose(x, y, )
//     }
// }

void turnAndMoveToPoint(
    float x
    , float y
    , int turnTO
    , int mvTO
    , bool fwd = true
    , bool async = false
    , float mvMaxSpeed = 127.0
    , bool move_to_pose = false
) {
    chassis.turnToPoint(x, y, turnTO, { .forwards = fwd });
    waitd;

    if (move_to_pose) {
        lemlib::Pose pose = chassis.getPose();

        chassis.moveToPose(x, y, pose.theta, mvTO, { .forwards = fwd });
    } else {
        chassis.moveToPoint(x, y, mvTO, { .forwards = fwd, .maxSpeed = mvMaxSpeed });
    }


    if (!async) {
        waitd;
    };
}

void red_negative() {
    // starts at the center of the intersection of the top two tiles, the mogo mech facing directly rightward
    chassis.setPose(-58.6, 47, 90);

    // turns and moves towards the first mogo, clamping it
    turnAndMoveToPoint(-24.5, 21.25, 500, 1500, true, true);
    chassis.waitUntil(29.5);
    mogo.toggle();
    waitd;

    // intakes the preload ring
    intake.intake();

    // immediately starts moving 
    chassis.turnToPoint(-12, 34.75, 600, {.forwards=false});
    waitd;
    chassis.moveToPoint(-12, 34.75, 750, {.forwards=false, .maxSpeed=75});
    waitd;

    pros::delay(350);

    chassis.moveToPoint(-22.65, 22, TO);
    waitd;
    turnAndMoveToPoint(-22.65, 44, 650, TO, false);
    waitd;

    pros::delay(400);    
    intake.brake();

    chassis.turnToPoint(-11.55, 49.25, 500, {.forwards=false});
    waitd;
    intake.intake();
    chassis.moveToPoint(-11.55, 49.25, 800, {.forwards=false});
    waitd;

    pros::delay(250);

    chassis.moveToPoint(-40, 49.25, TO);
}

void red_negative_5_ring() {
    red_negative();
    waitd;

    pros::delay(25);
    chassis.turnToHeading(135, TO);
    waitd;

    chassis.moveToPose(-70, 70, 135, 1350, {.forwards=false, .maxSpeed=70});
    pros::delay(50);
    intake.brake();
    chassis.waitUntil(7.5);
    intake.outtake();
    waitd;

    pros::delay(250);

    intake.intake();

    pros::delay(500);

    chassis.turnToHeading(90, TO);
    waitd;

    chassis.moveToPoint(-65, 57.5, TO, {.forwards=false});
    waitd;

    chassis.moveToPoint(-33.968, 23.456, TO);
    waitd;
}

void red_negative_sawp() {
    red_negative();

    chassis.waitUntil(15);
    intake.brake();
    waitd;

    chassis.setPose(-40, 49.25, chassis.getPose().theta);

    chassis.turnToPoint(-50.25, 11.648, 650);
    waitd;
    chassis.moveToPose(-50.25, -30, 180, 2650, {.minSpeed=75});

    chassis.waitUntil(30);
    mogo.toggle();
    waitd;

    double offset = -11.5;
    chassis.moveToPoint(-50, offset, 1000, {.forwards=false});
    waitd;

    lemlib::Pose pos = chassis.getPose();
    double a = 1;
    chassis.turnToPoint(-10000000000, pos.y, 650);
    waitd;
    pos = chassis.getPose();
    chassis.moveToPoint(pos.x-a, pos.y, TO, {.maxSpeed=85});
    waitd;

    intake.intake();

    pros::delay(750);

    intake.brake();

    // chassis.moveToPoint(-54.471, pos.y, TO, {.maxSpeed=50});
    // waitd;
}

// TWO (theo THREE) RING, TWO STAKE AUTON -> does awp!!!
void red_positive() {
    // starting position
    chassis.setPose(-55, -36.5, 90);

    // turns and moves towards first mogo, clamping it
    turnAndMoveToPoint(-24, -22, 500, 2500, true, true, 67.5);
    chassis.waitUntil(24);
    mogo.toggle();
    waitd;

    // intakes preload ring (first ring overall)
    intake.intake();
    pros::delay(800);

    // lets go of first mogo
    mogo.toggle();
    pros::delay(250);

    // TODO: sometimes ring flat out doesn't get inside
    // goes to + gets first ring on field (second ring overall)
    turnAndMoveToPoint(-31.4, -40, 650, TO, false, false, 115.0);
    pros::delay(250);
    // intake.brake();

    // go to + clamps second mogo on field
    turnAndMoveToPoint(-15.5, -46.5, 500, 2250, true, true, 70);
    pros::delay(10);
    intake.brake();
    chassis.waitUntil(15.5);
    mogo.toggle();
    waitd;

    // scores second ring
    intake.intake();
    pros::delay(500);

    // TODO: testing rn

    // goes to corner of tile in front of corner
    turnAndMoveToPoint(-56, -52, 500, 1000, false);

    // moves slowly to corner
    turnAndMoveToPoint(-95, -71, 1000, 1000, false, false, 75.0);

    pros::delay(100);
    // suddenly swings to reposition ring relative to intake
    chassis.swingToPoint(-132, -50, DriveSide::RIGHT, 500, {.forwards = false, .direction = lemlib::AngularDirection::CW_CLOCKWISE});
    waitd;
    pros::delay(150);
    // quickly turn right back, to "jam" ring into intake
    // chassis.swingToPoint(-111, -90, DriveSide::LEFT, 500, {.forwards = false, .direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});


    // arm.set_pos(arm.LOADIN_POS);
    chassis.moveToPoint(-37.246, -13.228, 2500, {.forwards=false});
    waitd;
    arm.set_pos(arm.LOADIN_POS);
}

void red_positive_approach_mogo_side() {
    chassis.setPose(-55, -36.5, 90);

    turnAndMoveToPoint(-24, -22, 1000, 1000, true, true);
    // 29.5 -> 26.5 -> 24.5
    chassis.waitUntil(24.5);
    mogo.toggle();
    waitd;

    intake.intake();
    pros::delay(750);
    mogo.toggle();

    // -31.5 -> 35.5 so aligned w mogo
    turnAndMoveToPoint(-18, -35.5, 1000, 750);

    // -43.25 -> -45.25
    turnAndMoveToPoint(-2.75, -45.25, 750, 1000);
    chassis.waitUntil(29.5);
    mogo.toggle();
}

void blue_negative() {
    chassis.setPose(58.6, 47, 270);

    turnAndMoveToPoint(24.5, 21.25, 500, 1500, true, true);
    chassis.waitUntil(29.5);
    mogo.toggle();
    waitd;

    intake.intake();

    // chassis.turnToPoint(12, 34.75, 600, {.forwards=false});
    // waitd;
    // chassis.moveToPoint(16, 36, 750, {.forwards=false, .maxSpeed=65});
    // waitd;

    turnAndMoveToPoint(16, 38, 600, 750, false, false, 65);


    pros::delay(500);

    // chassis.moveToPoint(34, 20, TO);
    // waitd;
    // turnAndMoveToPoint(34, 30, 500, TO, false);
    // waitd;

    // pros::delay(500);    
    // intake.brake();

    // chassis.turnToPoint(11.55, 49.25, 500, {.forwards=false});
    // waitd;
    // intake.intake();
    // chassis.moveToPoint(11.55, 49.25, TO, {.forwards=false});
    // waitd;

    // pros::delay(250);

    // chassis.moveToPoint(40, 49.25, TO);
    // waitd;
}

void blue_negative_5_ring() {
    blue_negative();

    pros::delay(25);
    chassis.turnToHeading(-135, TO);
    waitd;

    chassis.moveToPose(70, 70, -135, 1350, {.forwards=false, .maxSpeed=70});
    pros::delay(50);
    intake.brake();
    chassis.waitUntil(7.5);
    intake.outtake();
    waitd;

    pros::delay(250);

    intake.intake();

    pros::delay(500);

    chassis.turnToHeading(-90, TO);
    waitd;

    chassis.moveToPoint(65, 57.5, TO, {.forwards=false});
    waitd;

    chassis.moveToPoint(33.968, 23.456, TO);
    waitd;
}

void blue_negative_sawp() {
    blue_negative();

    intake.brake();
    waitd;

    chassis.turnToPoint(54.211, 11.648, 650);
    waitd;
    chassis.moveToPose(54.211, 11.648, -180, TO);
    waitd;

    chassis.moveToPoint(54.211, -5, 750, {.maxSpeed=75});
    waitd;

    mogo.toggle();

    chassis.moveToPoint(54.211, 0, 250);
    waitd;

    chassis.turnToHeading(270, 650);
    waitd;

    lemlib::Pose pos = chassis.getPose();
    int a = 10;
    chassis.moveToPoint(pos.x+a, pos.y, TO, {.maxSpeed=85});
    waitd;

    intake.intake();

    pros::delay(750);

    intake.brake();

    chassis.moveToPoint(54.471, pos.y, TO, {.maxSpeed=50});
    waitd;
}

void blue_positive() {
    // starting position
    chassis.setPose(55, -36.5, -90);

    // turns and moves towards first mogo, clamping it
    turnAndMoveToPoint(24, -23.5, 500, 2500, true, true, 70);
    chassis.waitUntil(25.2);
    mogo.toggle();
    waitd;

    // intakes preload ring (first ring overall)
    intake.intake();
    pros::delay(800);

    // lets go of first mogo
    mogo.toggle();
    pros::delay(150);

    // TODO: sometimes ring flat out doesn't get inside
    // goes to + gets first ring on field (second ring overall)
    turnAndMoveToPoint(26.5, -42.5, 750, TO, false, false);
    pros::delay(200);
    intake.brake();
    pros::delay(50);
    // intake.brake();

    // go to + clamps second mogo on field
    turnAndMoveToPoint(0, -49.5, 500, 2250, true, true, 70);
    chassis.waitUntil(16);
    mogo.toggle();
    waitd;

    // scores second ring
    intake.intake();
    pros::delay(500);

    // TODO: testing rn

    // goes to corner of tile in front of corner
    turnAndMoveToPoint(44, -57, 500, 1000, false);

    // moves slowly to corner
    turnAndMoveToPoint(81, -77.5, 1000, 2500, false, false, 75.0);

    pros::delay(100);
    // suddenly swings to reposition ring relative to intake
    chassis.swingToPoint(132, -50, DriveSide::RIGHT, 500, {.forwards = false, .direction = lemlib::AngularDirection::CW_CLOCKWISE});
    waitd;
    pros::delay(150);
    // quickly turn right back, to "jam" ring into intake
    // chassis.swingToPoint(-111, -90, DriveSide::LEFT, 500, {.forwards = false, .direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});


    // arm.set_pos(arm.LOADIN_POS);
    chassis.moveToPoint(17.5, -15, 2500, {.forwards=false});
    waitd;
    arm.set_pos(arm.LOADIN_POS);
}

void red_left_shortened_no_ladder() {
    // ASSUME PUSHING HAS ALR HAPPENED!
    chassis.setPose(-58.6, 47, 90);

    auto start = std::chrono::high_resolution_clock::now();

    // turns and moves towards mogo -> 42.6867
    // -27.5, 26.75 -> too rightward (rel to actual position, rel to bot POV)
    // -24, 24 -> tiny bit too high
    turnAndMoveToPoint(-24, 22, 1000, 1500, true, true);

    // clamps pre-emptively
    // 40.68 -> only finishes afterward...?
    // 25 -> about 2-3 inches early
    // 28 -> 1 inch
    // 30 -> BIT too late...? (0.5 in)
    chassis.waitUntil(29.5);
    mogo.toggle();

    // waits for rest of movement to finish
    waitd;

    // intakes preload ring + waits
    intake.intake();
    // 500ms -> 400
    pros::delay(750);

    // comes back so can rotate -> USING MOGO SIDE BC RAMMED INTO WALL W/ INTAKE SIDE!
    // chassis.moveToPoint(-24, 48, 1000);
    turnAndMoveToPoint(-24, 48, 1000, 1000, false);
    waitd;
    // 2000 -> 1750
    pros::delay(2000);

    turnAndMoveToPoint(-42, 10, 1000, 2000, false);
    waitd;
}

// red side, NEW BOT!
void red_cross_sawp() {
    // ASSUME PUSHING HAS ALR HAPPENED!
    chassis.setPose(-58.6, 47, 90);

    auto start = std::chrono::high_resolution_clock::now();

    // turns and moves towards mogo -> 42.6867
    // -27.5, 26.75 -> too rightward (rel to actual position, rel to bot POV)
    // -24, 24 -> tiny bit too high
    turnAndMoveToPoint(-24, 22, 1000, 1500, true, true);

    // clamps pre-emptively
    // 40.68 -> only finishes afterward...?
    // 25 -> about 2-3 inches early
    // 28 -> 1 inch
    // 30 -> BIT too late...? (0.5 in)
    chassis.waitUntil(29.5);
    mogo.toggle();

    // waits for rest of movement to finish
    waitd;

    // intakes preload ring + waits
    intake.intake();
    // 500ms -> 400
    pros::delay(400);

    /** THIS CAUSES THE HOOKS TO GO AJLKSDFJKLADSFJLKASDJFLKSADJF */
    // // turns and moves towards first ring on field (INTAKE STILL ON!) + waits
    // // -25 48.75 -> ring not forced into intake
    // turnAndMoveToPoint(-24, 62, 1000, 1000, false);
    // // waits a lot more for ring bc lots of drift on this robot
    // pros::delay(1000);

    // comes back so can rotate -> USING MOGO SIDE BC RAMMED INTO WALL W/ INTAKE SIDE!
    // chassis.moveToPoint(-24, 48, 1000);
    turnAndMoveToPoint(-24, 48, 1000, 1000, false);
    waitd;
    // 1750 -> 1000
    pros::delay(1000);

    /** NOT GOING FOR RN BC DOESNT WORK */
    // // raises intake + turns and moves towards second ring on field (INTAKE STILL ON!) + waits
    // // -> point TOO LEFT (almost 12" left)
    // intake.lift(1);
    // // -47, 0 TOO far left
    // // -50, 0 TOO far left (still)
    // turnAndMoveToPoint(-55, 0, 1000, 2000, false);
    // pros::delay(2000);

    // drops mogo + goes for second one
    // TODO: might need to stop intake here, in case accidentally intakes blue ring..?
    // turnAndMoveToPoint(-23.5, -23.5, 1000, 1000);
    // using 2 points bc going AROUND ladder (moving with INTAKE forward first, so can drop mogo BEHIND and THEN turn around)
    // turnTO 2000 -> 1500
    // mvTO needs most of that time so doesn't drift
    turnAndMoveToPoint(-45, -9.5, 1500, 2000, false, true);
    // drops mogo midway so we don't have to care about it
    chassis.waitUntil(40);
    // "drops" old mogo
    mogo.toggle();
    // waits for rest of movement
    waitd;
    
    // pros::delay(500);
    // -23.5, -23.5 too low
    // -23.5, -20.5/-19.5 -> DRIFT SO INACCURATE SO UNDERSHOOT SO CAN REACH MOGO!
    turnAndMoveToPoint(-23.5, -18.5, 2000, 2000, true, true);
    chassis.waitUntil(25);
    // "gets" new mogo + waits
    mogo.toggle();

    // waits for rest of movement to finish
    waitd;

    // turns + moves towards third ring on field
    // x: -23.5 (angled bit too left due to drift; need to compensate!)
    // ALSO ACTUAL ROBOT OVERSHOOTS A LOT BC OF DRIFT...!!!
    turnAndMoveToPoint(-25.5, -47.25, 1000, 1000, false);
    // waits for third ring to be intaked
    pros::delay(500);

    // going to ladder!
    chassis.moveToPoint(-19.5, -4, 2000);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time elapsed: " << elapsed.count() << "seconds." << std::endl;
}

void test_auton_task(void* chassisVoid) {
    lemlib::Chassis* chassis = static_cast<lemlib::Chassis*>(chassisVoid);

    while (true) {
        lemlib::Pose pos = chassis->getPose();

        std::cout << "x: " << pos.x
				  << "y: " << pos.y
				  << "theta: " << pos.theta
				  << std::endl;

        pros::delay(20);
    } 
}

void test_auton() {
    pros::Task task(&test_auton_task, &chassis);

    chassis.setPose(-48, -24, 90);

    chassis.moveToPoint(-24, -24, 1000);
    waitd;
}