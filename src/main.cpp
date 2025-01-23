/**
 * IMPORTS:
*/
#include <cmath>
#include <algorithm>
#include <map>
// for format strings
#include <sstream>
#include <cstdio>

#include "pros/misc.h"
#include "pros/motors.h"

#include "main.h"

// constants
const int DRIVE_SPEED = 127;

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor definitions
pros::Motor left_front(2);
pros::Motor left_mid(-3);
pros::Motor left_back(-4);

pros::Motor right_front(-10);
pros::Motor right_mid(9);
pros::Motor right_back(8);

// TODO: add imu port
pros::Imu imu(1);

// motor groups
pros::MotorGroup left_motors({
	2
	, -3
	, -4
}, pros::MotorGearset::blue);

pros::MotorGroup right_motors({
	-10
	, 9
	, 8
}, pros::MotorGearset::blue);

// liblem

lemlib::Drivetrain drivetrain(
	&left_motors, &right_motors,
	11.878,
	lemlib::Omniwheel::NEW_275,
	600,
	2
);

// lateral PID controller -> using kP 10 kD 3
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller: using kP 2, kD 10 bc VERY accurate
// "TUNED" constant values: kP 5, kD 31 OR kP 6, kD 43
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

lemlib::Chassis chassis(drivetrain // drivetrain settings
                        , lateral_controller // lateral PID settings
                        , angular_controller // angular PID settings
                        , sensors // odometry sensors
);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();

	chassis.calibrate();

	pros::Task screenTask(
		tuning_cli_screen_task
		, &chassis
	);
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
void competition_initialize() {};

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
void autonomous() {};

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void test_input() {
	bool A_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
	bool X_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);

	std::stringstream ss;
	ss << "A: " << (A_pressed ? "YES" : "NO") << " | X: " << (X_pressed ? "YES" : "NO") << "  ";
	std::string keyString = ss.str();

	std::string testStringM = "MMMMMMMMMMMMMMMMMMMMMMM";
	std::string testLower = "enter command:";
	std::string testStringSpace = "               ";
	std::string testString2 = "            -";

	controller.set_text(0, 0, testLower);
}

void opcontrol() {
	// opcontrol runs forever! (while in driver control; it's its own task so we gucci)
	while (true) {
		if (!TuningCLI::tuningPID) {
			/* normal driver control */

			bool X_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
			bool A_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);

			// if both the X and A buttons are pressed
			if (X_pressed && A_pressed) {
				printf("X and A were both pressed, transferring to tuning PID mode...\n\n---\n\n");
				TuningCLI::tuningPID = true;

				continue;
			}
		} else {
			/* tuning PID! wee! */

			tuningCLI();
			
			// test_input();
		}

		// delay to save system resources
		pros::delay(20);
	}
}
