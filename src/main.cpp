/**
 * IMPORTS:
*/
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include "main.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

#include "robodash/api.h"

// constants
const int DRIVE_SPEED = 127;

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);


/**
 * NOTE: The mogo mech is the front of the robot, from a driving perspective and an auton perspective
 */
/**
 * TODO: add dt ports!
 */
pros::Motor left_front(2);
pros::Motor left_mid(-3);
pros::Motor left_back(-4);
pros::Motor right_front(-10);
pros::Motor right_mid(9);
pros::Motor right_back(8);

pros::Imu imu(1);

// reversed port 9 from 9 -> -9
pros::MotorGroup left_motors({
	2
	, -3
	, -4
}, pros::MotorGearset::blue);

// motor groups
pros::MotorGroup right_motors({
	-10
	, 9
	, 8
}, pros::MotorGearset::blue);

// track width: 15 inches NOT 15 inches, but 12.426 (cad moment :nerd:)
// -> measured by tape measure: ~ 12 and 7 / 16 inches (~12.4375)
// wheelbase: 16 inches
lemlib::Drivetrain drivetrain(
	&left_motors, &right_motors,
	12.426,
	lemlib::Omniwheel::NEW_275,
	450,

	// bc we have central traction wheels; refer to
	// https://lemlib.readthedocs.io/en/stable/tutorials/6_lateral_motion.html
	8
);

// lateral PID controller -> FOR REFERENCE, maine bot constants WERE kP 8 kD 21
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller -> FOR REFERENCE, maine bot constants WERE kP 3 and kD 24
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
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

Intake intake = Intake(
	{
		-6						// left intake (reversed)
		, -7						// right intake (normal)
	}
	, pros::E_MOTOR_BRAKE_HOLD	// brake mode of intake

	/**
	 * TODO: add intake piston ports!
	 */
	, 'D'						// intake piston port

	, 'B'						// pressure hood port
);

/**
 * TODO: add mogo ports!
 */
MogoMech mogo = MogoMech(
	{ 'C' } // -> ONE piston
	, 'A'		  // -> TWO pistons
);

// Arm arm = Arm(10, pros::E_MOTOR_BRAKE_HOLD);

Doinker doinker = Doinker('E');

// Hang hang = Hang('D');

rd::Selector selector({
    {.name="RED LEFT SOLO AWP", .action=&red_left_side_solo_awp}
    , {.name="BLUE LEFT (3 ring)", .action=&blue_left_side}
	, {.name = "BLUE RIGHT (4 ring)", .action=&blue_right_side}
    , {.name="RED LEFT (4 ring)", .action=&red_left_side}
	, {.name="RED RIGHT (3 ring)", .action=&red_right_side}
});

// Create robodash console
rd::Console console;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// pros::lcd::initialize();

	chassis.calibrate();

	// pros::lcd::register_btn0_cb(left_btn_cb);
	// pros::lcd::register_btn2_cb(right_btn_cb);

// 	pros::Task screenTask(
// 		screenTaskFunc			// function that is the task
// 		, &chassis				// pointer to parameter to task
// 	);
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
	selector.focus();
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
		
	// Run the selected autonomous function - UNCOMMENT ONCE DONE TESTING AUTONS
	// selector.run_auton();

	red_left_side_solo_awp();
};

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
	// brake mode back to coast!
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);
	intake.intake_motors.set_brake_mode_all(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);

	// bc hang open at end of some autons, just do this so yeah
	// hang.toggle();

	// opcontrol runs forever! (while in driver control; it's its own task so we gucci)
	while (true) {
//		if (!tuningPID) {
			/* normal driver control */

//			bool X_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X);
//			bool A_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);

			// if both the X and A buttons are pressed
//			if (X_pressed && A_pressed) {
//				printf("X and A were both pressed, transferring to tuning PID mode...\n\n---\n\n");
//				tuningPID = true;

//				continue;
//			}

		/**
		* CONTROL FETCHING:
		*/
		///// HOLD controls
		// intake
		bool R1_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		// outtake
		bool R2_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

		// bool UP_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
		// bool DOWN_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

		///// TOGGLE controls
		// intake lift -> not using bc can just tip over a stack in driver control
		// bool X_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
		// mogo mech
		bool L2_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);
		// toggle slapper
		bool Y_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);
		// toggle tilt
		bool DOWN_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);

		/**
			* ARM
		*/
		// if (UP_pressed == DOWN_pressed) {
		// 		arm.brake();
		// } else if (UP_pressed) {
		// 		arm.arm_up();
		// } else if (DOWN_pressed) {
		// 		arm.arm_down();
		// }

		// /**
		//  * HANG:
		//  */
		// if (DOWN_new_press) {
		// 	hang.toggle();
		// }

		/**
		 * INTAKE:
		 */
		if (R1_pressed == R2_pressed) {
			// if both controls are pressed or depressed, brake (stop) the intake

			std::cout << "braking" << std::endl;

			intake.brake();
		} else if (R1_pressed) {
			// intaking

			std::cout << "intaking" << std::endl;

			// intake.intake();
			intake.outtake();
		} else if (R2_pressed) {
			// outtaking

			std::cout << "outtaking" << std::endl;

			// intake.outtake();
			intake.intake();
		}

		// /**
		//  * INTAKE LIFT: 
		//  */
		
		// if (X_new_press) {
		// 	intake.toggle();
		// }

		/**
		 * MOGO:
		 */

		// clamp + tilt toggling
		if (L2_new_press) {
			std::cout << "toggling!" << std::endl;

			mogo.toggle();
		}

		// just tilt toggling
		if (DOWN_new_press) {
			// // cannot tilt when clamp isn't in effect!
			// if (mogo.mogo_clamp_val) {
			// 	// gives feedback to driver that disallowed
			// 	controller.rumble(".");
			
			// // otherwise all is well and tilt can toggle :D
			// } else {
			// 	mogo.clamp(0);
			// }
			mogo.tilt(1);
		}



		/**
		 * DOINKER:
	     */
		if (Y_new_press) {
			doinker.toggle();
		}

		/**
		 * DRIVING:
		 */
		// replace with tank() if u really don't like tank that much
		arcade();
//		} else {
			/* tuning PID! wee! */

			// tuningCLI();
//		}

		// delay to save system resources
		pros::delay(20);
	
	}
}


