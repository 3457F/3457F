/**
 * IMPORTS:
*/
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include "consts.hpp"
#include "util.hpp"

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
 * GENERAL STUFF:
 */

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// TODO: add imu port
pros::Imu imu(16);

// ---

/**
 * MOTOR GROUPS:
 */
// front (mogo) -> back (intake) : ALL REVERSED
pros::MotorGroup left_motors({
	LEFT_FRONT_PORT
	, LEFT_MID_PORT
	, LEFT_BACK_PORT
}, pros::MotorGearset::blue);

// front (mogo) -> back (intake) : ALL NORMAL
pros::MotorGroup right_motors({
	RIGHT_FRONT_PORT
	, RIGHT_MID_PORT
	, RIGHT_BACK_PORT
}, pros::MotorGearset::blue);

// TODO: get ports
pros::Rotation horizontal(5);

/** liblem */

// TODO: verify all config settings
lemlib::Drivetrain drivetrain(
	&left_motors, &right_motors,
	11.75,
	lemlib::Omniwheel::NEW_275,
	600,
	2
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(DT_LATERAL_P, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              DT_LATERAL_D, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(DT_ANGULAR_P, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              DT_ANGULAR_D, // derivative gain (kD)
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

lemlib::OdomSensors sensors(
							// &vertical_track, // vert tracking wheel that kinda doesn't work
							nullptr, // vertical tracking wheel 1, set to nullptr as we are using IMEs
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

/**
 * SUBSYSTEM INITIALIZATION:
*/

// TODO: set intake piston port + color sensor port
Intake intake = Intake(
	{INTAKE_PORT}						
	, pros::E_MOTOR_BRAKE_HOLD	// brake mode of intake

	, 'G'						// intake piston port
	, 2
	, true						
);

// TODO: set ports
MogoMech mogo = MogoMech(MOGO_MECH_PORT);

// TODO: set ports
Arm arm = Arm(
	ARM_PORT
	, pros::E_MOTOR_BRAKE_HOLD
	, 11
	, &intake
);

// TODO: set port
Doinker doinker = Doinker(DOINKER_PORT);

/**
 * VARIABLE DEFINITION:
 */

bool L1_state = false;

// TODO: shld be ok if the task starts at the beginning...?
// pros::Task color_sort(&update_sort_auton, &intake);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	chassis.calibrate();

	// stormlib::initialize();

	auton_table.auton_populate(
        {
			// red neg
			Auton(red_neg_elims_new, Alliance::RED, Corner::NEGATIVE, "Red Neg Elims", "scores 4 rings, 2 stakes", 3, 0, true)
			, Auton(red_neg_awp_redo, Alliance::RED, Corner::NEGATIVE, "Red Neg SAWP (OLD)", "scores 4 rings, 2 stakes & touches ladder (AARAV CODE)", 3, 0, true)
			, Auton(red_neg_awp_redo_new, Alliance::RED, Corner::NEGATIVE, "Red Neg SAWP", "scores 4 rings, 2 stakes & touches ladder", 3, 0, true)
			
			// red pos
			, Auton(red_rush, Alliance::RED, Corner::POSITIVE, "Red Rush", "2 rings (pos corner) + 1 ring", 2, 1, false)

			// blue neg
			, Auton(blue_neg_elims, Alliance::BLUE, Corner::NEGATIVE, "Blue Neg Elims", "scores 4 rings, 2 stakes", 3, 0, true)
			, Auton(blue_neg_awp_redo, Alliance::BLUE, Corner::NEGATIVE, "Blue Neg SAWP", "scores 4 rings, 2 stakes & touches ladder", 3, 0, true)
			
			// blue pos
			, Auton(blue_rush, Alliance::BLUE, Corner::POSITIVE, "Blue Rush [DON'T USE]", "2 rings (pos corner) + 1 ring", 2, 1, false)
			, Auton(blue_pos_safe, Alliance::BLUE, Corner::POSITIVE, "Blue Positive Safe", "3 rings + clears positive corner", 3, 0, false)
        }
    );

	screen_init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {

}

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
	
	/** FOR DEBUGGING */
	// blue_neg_awp_redo();

	/** AUTON SELECTOR RUNNING */
	auton_run();                                                                                                                                                                   
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
	// brake mode back to coast!
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);
	intake.intake_motors.set_brake_mode_all(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);

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

		// ---

		///// TOGGLE controls

		// mogo mech
		bool L2_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);
		
		// doinker
		bool Y_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

		// arm
		bool DOWN_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN);
		bool RIGHT_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);
		bool L1_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		
		// mogo
		bool X_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);

		/**
		 * ARM:
		 */

		// if L1 PRESSED
		if (L1_pressed) {
			// "rising edge"; JUST pressed
			if (L1_state == false) {
				L1_state = true;
				arm.force();
			}
		} else {
			// "falling edge"; JUST released
			if (L1_state == true) {
				L1_state = false;
				arm.release_force();
			}

			if (DOWN_new_press) {
				arm.score_cycle();
			} else if (RIGHT_new_press) {
				arm.start_pos();
			}
		}

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

		printf("arm pos: %f\n", arm.encoder.get_position());

		// delay to save system resources
		pros::delay(DRIVER_TICK);
	}
}
