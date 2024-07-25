/**
 * IMPORTS:
*/
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include "main.h"
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

#include "robodash/api.h"

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

lemlib::MoveToPointParams linearPIDTestMoveToPointParams = {
	forwards: false
	, maxSpeed: 127
	, minSpeed: 0
	, earlyExitRange: 0
};

// controller definition
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor definitions
pros::Motor left_front(-11);
pros::Motor left_mid(-12);
pros::Motor left_back(-13);

pros::Motor right_front(20);
pros::Motor right_mid(19);
pros::Motor right_back(18);

pros::Imu imu(2);

// motor groups
pros::MotorGroup left_motors({
	-11
	, -12
	, -13
}, pros::MotorGearset::blue);

pros::MotorGroup right_motors({
	20
	, 19
	, 18
}, pros::MotorGearset::blue);

// liblem

lemlib::Drivetrain drivetrain(
	&left_motors, &right_motors,
	14,
	lemlib::Omniwheel::NEW_325,
	450,
	2
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(9, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              18, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(3, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              24, // derivative gain (kD)
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
Intake intake = Intake(-1, pros::E_MOTOR_BRAKE_HOLD, 'G');

MogoMech mogo = MogoMech('H');

Arm arm = Arm(10, pros::E_MOTOR_BRAKE_HOLD);


// void screenTaskFunc(void* chassis) {
// 	lemlib::Chassis* myChassis = (lemlib::Chassis *)(chassis);

// 	while (true) {
// 		char runningPIDTestBufferString[20];

// 		// the space in the format string is important... don't delete it!
// 		std::snprintf(
// 			runningPIDTestBufferString
// 			, sizeof(runningPIDTestBufferString)
// 			, " PID Test? %s"
// 			, runningPIDTest ? "YES" : "NO"
// 		);

// 		// first value: whether tuning PID / normal driver control is enabled
// 		// second value: if tuning PID, whether test PID auton running or not
// 		pros::lcd::print(
// 			0
// 			, "Tuning? %s%s"
// 			, tuningPID ? "YES" : "NO"
// 			, tuningPID ? runningPIDTestBufferString : ""
// 		);

// 		pros::lcd::print(1, "Pos X (Relative): %f", myChassis->getPose().x);
// 		pros::lcd::print(2, "Pos Y (Relative): %f", myChassis->getPose().y);
// 		pros::lcd::print(3, "Bot Heading (Relative): %f", myChassis->getPose().theta);

// 		pros::lcd::print(
// 			4
// 			, "CURRENT %s kP: %f"
// 			, runningLinearPIDTest ? "LINEAR" : "ANGULAR"
// 			, runningLinearPIDTest ? myChassis->lateralPID.kP : myChassis->angularPID.kP 
// 		);

// 		pros::lcd::print(
// 			5
// 			, "CURRENT %s kD: %f"
// 			, runningLinearPIDTest ? "LINEAR" : "ANGULAR"
// 			, runningLinearPIDTest ? myChassis->lateralPID.kD : myChassis->angularPID.kD
// 		);
		
// 		pros::delay(20);
// 	}
// }

// std::vector<std::string> split(const std::string& _input,
//                                const std::string& delimiter) {
// 	std::vector<std::string> tokens;

// 	std::string source = _input;

// 	size_t pos = 0;

// 	while ((pos = source.find(delimiter)) != std::string::npos) {
// 		tokens.push_back(source.substr(0, pos));

// 		source.erase(0, pos + delimiter.length());
// 	}

// 	// returns last entry AFTER delimiter
// 	tokens.push_back(source);

// 	return tokens;
// }

// void makeLowerCase(std::string& str) {
// 	std::transform(
// 		str.begin()		// passes in the
// 		, str.end()		// full string to be transformed

// 		, str.begin()	// section of string to start inserting
// 						// transformed string into
		
// 		// basically lambda function that returns lowercase version of
// 		// each character in the string
// 		, [](unsigned char c) { return std::tolower(c); }
// 	);
// }

// /**
//  * inspired by code from: https://github.com/meisZWFLZ/OverUnder781X
// */
// void tuningCLI() {
// 	lemlib::PID* pid = &(runningLinearPIDTest ? chassis.lateralPID
// 											  : chassis.angularPID);

// 	while (tuningPID) {
// 		try {
// 			// informs user they can start typing command
// 			std::cout << "enter command> ";

// 			// fetches command (WAITS UNTIL NEWLINE)
// 			// and formats it to lowercase
// 			std::string input;
// 			getline(std::cin, input);
// 			makeLowerCase(input);

// 			auto params = split(input, " ");

// 			std::string command = params.at(0);

// 			if (command == "s" || command == "set") {
// 				if (params.size() < 3) {
// 					std::cout << "not enough arguments to process request (need 3!)..." << std::endl;
// 					continue;
// 				}

// 				std::string whichGain = params.at(1);
// 				std::string valueToSetStr = params.at(2);

// 				float *constToSet = nullptr;


// 				if (whichGain == "p") {
// 					constToSet = &(pid->kP);
				
// 				} else if (whichGain == "i") {
// 					constToSet = &(pid->kI);
				
// 				} else if (whichGain == "d") {
// 					constToSet = &(pid->kD);
				
// 				} else {
// 					std::cout << " | INVALID gain to set!" << std::endl;
// 					continue;
// 				}

// 				// stores old value of the PID constant, to inform the user later
// 				float oldValue = (*constToSet);

// 				// initializes `valueToSet`
// 				float valueToSet = -1;

// 				/**
// 				 * if the user types in a certain string as the second parameter (that is, NOT a float),
// 				 * fetches the PID constant they want to change, and adds or subtracts 1 or 2, accordingly
// 				 * to get its new value
// 				*/
// 				if (valueToSetStr == "+1") {
// 					valueToSet = (*constToSet) + 1;
// 				} else if (valueToSetStr == "-1") {
// 					valueToSet = (*constToSet) - 1;
				
// 				} else if (valueToSetStr == "+2") {
// 					valueToSet = (*constToSet) + 2;
// 				} else if (valueToSetStr == "-2") {
// 					valueToSet = (*constToSet) - 2;
				
// 				} else {
// 					try {
// 						// tries to convert user's desired gain value to a float
// 						valueToSet = std::stof(valueToSetStr);
// 					} catch (const std::invalid_argument &e) {
// 						// we were scammed! the user didn't pass in a float!
// 						std::cout << " | Gain value not a valid float!" << std::endl;
// 						continue;
// 					}
// 				}

// 				// sets the new value of the PID constant!
// 				(*constToSet) = valueToSet;

// 				printf(" | successfully changed gain value! old value: %f, new value %f!\n", oldValue, *constToSet);

// 			} else if (command == "g" || command == "get") {
// 				if (params.size() < 2) {
// 					std::cout << "not enough arguments to process request (need 2!)..." << std::endl;
// 					continue;
// 				}

// 				std::string whatInfo = params.at(2);

// 				if (whatInfo == "mode") {
// 					std::cout << " | currently tuning " << (runningLinearPIDTest ? "LINEAR" : "ANGULAR") << " PID" << std::endl;
				
// 				} else if (whatInfo == "p") {
// 					std::cout << " | kP: " << pid->kP << std::endl;
				
// 				} else if (whatInfo == "i") {
// 					std::cout << " | kI: " << pid->kI << std::endl;
				
// 				} else if (whatInfo == "d") {
// 					std::cout << " | kD: " << pid->kD << std::endl;
				
// 				} else {
// 					std::cout << " | INVALID gain to fetch info for!" << std::endl;
// 				}
			
// 			} else if (command == "turn-left") { // returns robot to original position IF tuning angular PID
// 				if (!runningLinearPIDTest) {
// 					chassis.setPose(0, 0, 0);

// 					chassis.turnToHeading(-90, 1500, {}, false);
// 				}

// 			} else if (command == "run" || command == "r") {
// 				runningPIDTest = true;

// 				// resets position before runs, in case test auton is being run multiple times
// 				chassis.setPose(0, 0, 0);

// 				switch (runningLinearPIDTest) {
// 					// running linear PID test
// 					case true:
// 						chassis.moveToPoint(0, -24, 3000, linearPIDTestMoveToPointParams, false);
						
// 						break;
// 					default:
// 						chassis.turnToHeading(90, 1500, {}, false);

// 						pros::delay(1000);
// 				}
			
// 			} else if (command == "stop") {
// 				chassis.cancelAllMotions();
			
// 			} else if (command == "exit") {
// 				std::cout << " | switching to driver control..." << std::endl << std::endl << "---" << std::endl << std::endl;

// 				chassis.cancelAllMotions();
// 				tuningPID = false;

// 				break;
			
// 			} else {
// 				std::cout << "| not a valid command..." << std::endl;
// 			}
// 		} catch (std::exception e) {
// 			std::cout << " | something went wrong: " << e.what() << std::endl;
// 		}
// 	}
// }


rd::Selector selector({
    {"Red right side AWP", &red_right_side},
    {"Red left side AWP", &red_left_side},
   
});

// Create robodash console
rd::Console console;


// std::vector<std::string> autonNames = {
// 	"Red Right Side"
// 	, "Red Left Side"
// };

// std::map<std::string, void(*)()> autonMap = {
// 	// {"Red Right Side"} & {red_right_side}
// 	// , {"Red Left Side"} &{&red_left_side}
// };

// int autonMapSize = static_cast<int>(autonMap.size());

// int curr_auton = 0;

// void display_auton() {
// 	pros::lcd::clear_line(0);

// 	pros::lcd::set_text(0, autonNames[curr_auton]);
// }

// void left_btn_cb() {
// 	if (curr_auton > 0) {
// 		curr_auton -= 1;
// 		display_auton();
// 	}
// }

// void right_btn_cb() {
// 	if (curr_auton < (autonMapSize = 1)) {
// 		curr_auton += 1;
// 		display_auton();
// 	}
// }

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
	// Run the selected autonomous function
	selector.run_auton();
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_BRAKE);
	// red_left_side();

	// calls the auton chosen in the auton selector
	// autonMap[autonNames[curr_auton]]();

	// red_left_side();
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
	// Print hello 0-99 to the robodash console
	for (int i = 0; i < 100; i++) {
		console.printf("Hello %d\n", i);
		pros::delay(200);
	}
	chassis.setBrakeMode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST);
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
			// intake (HOLD)
			bool R1_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
			// outtake (HOLD)
			bool R2_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
	
			bool UP_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
			bool DOWN_pressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

			///// TOGGLE controls
			bool L2_new_press = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);

			/**
				* ARM
			*/
			if (UP_pressed == DOWN_pressed) {
					arm.brake();
			} else if (UP_pressed) {
					arm.arm_up();
			} else if (DOWN_pressed) {
					arm.arm_down();
			}

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
