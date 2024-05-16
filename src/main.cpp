#include "main.h"

// constants
const int DRIVE_SPEED = 127;

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor definitions
pros::Motor left_front(-20);
pros::Motor left_mid(-10);
pros::Motor left_back(-9);

pros::Motor right_front(8);
pros::Motor right_mid(7);
pros::Motor right_back(6);

// motor groups
pros::Motor_Group left_motors({
	left_front
	, left_mid
	, left_back
});

pros::Motor_Group right_motors({
	right_front
	, right_mid
	, right_back
});

// driving functions
void tank() {
	left_motors.move((controller.get_analog(ANALOG_LEFT_Y) / 127.0) * DRIVE_SPEED);
	right_motors.move((controller.get_analog(ANALOG_RIGHT_Y) / 127.0) * DRIVE_SPEED);
}

void arcade() {
	int move = controller.get_analog(ANALOG_LEFT_Y);
	int turn = controller.get_analog(ANALOG_RIGHT_X);

	left_motors.move(((move + turn) / 127.0) * DRIVE_SPEED);
	right_motors.move(((move - turn) / 127.0) * DRIVE_SPEED);
}

/**
 * SUBSYSTEM INITIALIZATION:
*/
Intake intake = Intake(1, pros::E_MOTOR_BRAKE_HOLD);

MogoMech mogo = MogoMech('H');

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
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
void competition_initialize() {}

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
void autonomous() {}

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
void opcontrol() {
	while (true) {
		/**
		 * DRIVING:
		*/
		arcade();
		
		/**
		 * CONTROL FETCHING:
		*/
		///// HOLD controls
		// intake (HOLD)
		bool R1_pressed = controller.get_digital(DIGITAL_R1);
		// outtake (HOLD)
		bool R2_pressed = controller.get_digital(DIGITAL_R2);

		///// TOGGLE controls
		bool L2_new_press = controller.get_digital(DIGITAL_L2);

		/**
		 * INTAKE:
		*/
		if (R1_pressed == R2_pressed) {
			// if both controls are pressed or depressed, brake (stop) the intake

			intake.brake();
		} else if (R1_pressed) {
			// intaking

			intake.intake();
		} else if (R2_pressed) {
			// outtaking

			intake.outtake();
		}

		/**
		 * MOGO:
		*/

		if (L2_new_press) {
			mogo.toggle();
		}


		// delay to save system resources
		pros::delay(20);
	}
}
