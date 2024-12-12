/**
 * IMPORTS:
*/
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include "main.h"
#include "arm.hpp"
#include "intake.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

#include "pros/rotation.hpp"
#include "pros/rtos.hpp"
#include "robodash/api.h"
#include "stormlib/api.hpp"
#include "stormlib/selector.hpp"

/**
 * CONFIG VARS:
*/
// whether PID is being tuned, or normal
// driver control should run
bool tuningPID = false;
// whether the physical PID tuner is being 
// used, or P, I, and D values are being set
// manually, through the C++ structs
// `lateralController` and
// `angularController`
bool usingPhysicalPIDTuner = false;
// if `tuningPID` is set to `true`, whether
// LINEAR PID is being tuned, or ANGULAR
// PID is being tuned
bool runningLinearPIDTest = false;

/**
 * RUNTIME VARS: (**DO NOT MODIFY**)
*/
// if `tuningPID` is set to `true`, whether
// the robot is CURRENTLY running a test
// auton with target kP and kD values!
bool runningPIDTest = false;
// represents the TARGET (NOT actual) kP,
// that you want to be set
int kp_target = 0;
// represents the TARGET (NOT actual) kD,
// that you want to be set
int kd_target = 0;

// constants
const int DRIVE_SPEED = 127;

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);


/**
 * NOTE: The mogo mech is the front of the robot, from a driving perspective and an auton perspective
 */


// 12 in -> track width (distance between wheels)

// 15.433 height (based on cad)
// -> 15.933 (added .5 after sunny cooked with budget bot)
// -> 15.75 -> measured height
// 14.00 -> width (based on cad)
// -> 14.25 -> measured width
// TECHNICALLY 14.48 in w/ side skirts (based on cad)

pros::Imu imu(7);

// motor groups
pros::MotorGroup left_motors({
	-15
	, -16
	// , -19
	, -19
}, pros::MotorGearset::blue);

pros::MotorGroup right_motors({
	17
	, 18
	// , 20
	, 20
}, pros::MotorGearset::blue);

pros::Rotation horizontal(5);

pros::Rotation vertical(8);

// liblem

lemlib::Drivetrain drivetrain(
	&left_motors, &right_motors,
	12,
	lemlib::Omniwheel::NEW_325,
	450,
	2
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              21, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(3.25, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              26, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// chnged to positive because tracking wheel is on mogo mech side
// which is on front of robot!
lemlib::TrackingWheel horizontal_track(&horizontal, lemlib::Omniwheel::NEW_2, 2.5); // changed from -3

lemlib::TrackingWheel vertical_track(&vertical, lemlib::Omniwheel::NEW_2, 2.1);

lemlib::OdomSensors sensors(
							&vertical_track, // vert tracking wheel that kinda doesn't work
							// nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_track, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

// driving functions
void tank() {
	left_motors.move((controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0) * DRIVE_SPEED);
	right_motors.move((controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127.0) * DRIVE_SPEED);
}

void arcade() {
	int move = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

	left_motors.move(((move + turn) / 127.0) * DRIVE_SPEED);
	right_motors.move(((move - turn) / 127.0) * DRIVE_SPEED);
}

/**
 * SUBSYSTEM INITIALIZATION:
*/

// right intake normal; left intake reversed 
Intake intake = Intake(
	
		{14, 1}						// left intake (reversed)
	
	, pros::E_MOTOR_BRAKE_HOLD	// brake mode of intake

	, 'G'
	, 10
	, true						// intake piston port
);

MogoMech mogo = MogoMech('A');

Arm arm = Arm(
	9
	, pros::E_MOTOR_BRAKE_HOLD
	, -4
	, &intake
);

// was E
Doinker doinker = Doinker('B');

// Hang hang = Hang('D')

// rd::Selector selector({
// 	{.name="RED LEFT SOLO AWP", .action=&red_cross_sawp}
// 	, {.name="RED NEGATIVE (5 ring)", .action=&red_negative_5_ring}
// 	// , {.name="RED POSITIVE (3 ring)", .action=&red_right_side}
//     // , {.name="BLUE POSITIVE (3 ring)", .action=&blue_left_side}
// 	, {.name = "BLUE NEGATIVE (5 ring)", .action=&blue_negative_5_ring}
// 	, {.name = "BLUE SAWP", .action=&blue_positive}
// 	,{.name = "RED SAWP", .action=&red_positive}
// });
stormlib::selector selector(
	stormlib::selector::E_SKILLS_1,
	"5 Ring (Negative)",	// E_RED_LEFT_1
	"SAWP (Negative)",		// E_RED_LEFT_2
	"Elims (Negative)",		// E_RED_LEFT_3
	"Misc"				// E_RED_RIGHT_1
);

// Create robodash console
rd::Console console;

// TODO: shld be ok if the task starts at the beginning...?
pros::Task color_sort(&update_sort_auton, &intake);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	chassis.calibrate();

	// alliance color is BLUE
	intake.color = intake.BLUE_HUE;

	// selector.initialize();

	// pros::Task screenTask(
	// 	screenTaskFunc			// function that is the task
	// 	, &chassis				// pointer to parameter to task
	// );
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// Focus auton selector on screen
	// selector.focus();

	// makes sure color sort task is running in preparation
	// for autonomous routine to start!
	color_sort.resume();
};

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD);
	
	// Run the selected autonomous function - COMMENT ONCE DONE TESTING AUTONS
	blue_neg_awp_redo();

	// UNCOMMENT AUTON SELECTOR ONCE DONE TESTING AUTONS

	/* stormlib */

	// std::cout << selector.getAuton() << std::endl;

	// default: skills
	// if (selector.getAuton() == stormlib::selector::E_SKILLS_1) {
	// 	prog_skills();
	
	// // red negative
	// } else if (selector.getAuton() == stormlib::selector::E_RED_LEFT_1) {
	// 	red_negative_5_ring();
	// } else if (selector.getAuton() == stormlib::selector::E_RED_LEFT_2) {
	// 	red_neg_awp_redo();
	// } else if (selector.getAuton() == stormlib::selector::E_RED_LEFT_3) {
	// 	red_neg_elims();
	
	// // red positive
	// } else if (selector.getAuton() == stormlib::selector::E_RED_RIGHT_1) {
	// 	red_positive();
	
	// // blue negative
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_RIGHT_1) {
	// 	blue_negative_5_ring();
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_RIGHT_2) {
	// 	blue_neg_awp_redo();
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_RIGHT_3) {
	// 	// blue_negative_safe();
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_RIGHT_4) {
	// 	blue_negative_5_ring();

	// // blue positive
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_LEFT_1) {
	// 	blue_positive();
	// } else if (selector.getAuton() == stormlib::selector::E_BLUE_LEFT_4) {
	// 	blue_positive_normal_points();
	// }

	/* robodash */
	// selector.run_auton();
};

/**
 * Runs the operator control code. This function will be started in its *own task*
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * *If no competition control is connected (the program is run straight from the controller),
 * this function will run immediately following initialize()*
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	color_sort.suspend();
	color_sort = nullptr;

	chassis.setPose(-54.779, 15.9, 0);

	// brake mode back to coast!
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);

	// bc hang open at end of some autons, just do this so yeah
	// hang.toggle();

	arm.set_pos(arm.INIT_POS);

	// starting hue tuned based on whatever venue we're at
	intake.STARTING_HUE = intake.color_sensor.get_hue();

	while (true) {
		/**
		* CONTROL FETCHING:
		*/
		///// HOLD controls
		// intake
		bool R1_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		// outtake
		bool R2_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

		///// TOGGLE controls
		// intake lift -> not using bc can just tip over a stack in driver control
		// bool X_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		// mogo mech
		bool L2_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);
		// toggle slapper
		bool Y_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
		// // toggle hang
		// bool DOWN_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);
		// arm test
		bool UP_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
		bool LEFT_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);


		// arm
		bool DOWN_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);
		bool UP_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP);
		bool RIGHT_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);
		bool LEFT_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT);

		/**
			* ARM
		*/
		if (DOWN_new_press) {
			arm.score_cycle(); 
		} else if (RIGHT_new_press) {
			arm.start_pos();
		}
		// } else if (RIGHT_new_press) {
		// 	arm.load_in();
		// } else if (UP_new_press) {
		// 	arm.score();
		// } else if (LEFT_new_press) {
		// 	arm.init_pos();
		// }

		/**
		 * DOINKER:
	     */
		if (Y_new_press) {
			doinker.toggle();
		}
	
		/**
		 * INTAKE:
		 */
		intake.update_sort(R1_pressed, R2_pressed);

		// /**
		//  * INTAKE LIFT: 
		//  */
		
		// if (X_new_press) {
		// 	intake.toggle();
		// }

		/**
		 * MOGO:
		 */

		if (L2_new_press) {
			mogo.toggle();
		}

		/**
		 * DRIVING:
		 */
		// replace with tank() if u really don't like tank that much
		arcade();

		lemlib::Pose pos = chassis.getPose();

		// delay to save system resources
		pros::delay(10);
	}
}
