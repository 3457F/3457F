#include "tuning_cli.hpp"

// RMBR: 20 ms delay for normal pros funcs;
// 50 ms delay for printing to controller!

lemlib::MoveToPointParams linearPIDTestMoveToPointParams = {
	.forwards = false
	, .maxSpeed = 127
	, .minSpeed = 0
	, .earlyExitRange = 0
};

/**
 * enums representing different "commands" represented by buttons!
 */
enum class CommandBtns {
	NONE = 0
	, SET = 1
	, GET = 2
	, TUNE = 3
};

enum class PIDBtns {
	NONE = 0
	, P = 1
	, D = 2
};

enum class ValBtns {
	PLUSONE = 0
	, PLUSTWO = 1
	, MINUSONE = 2
	, MINUSTWO = 3
};

enum class CtrlBtns {
	NEXT = 0
};

enum class TuningCLIState {
	CMD = 0
	, VAR = 1
	, VAL = 2
	, DISP = 3
	, TUNE = 4
};

/**
 * DEFINING BTN MAP CLASS
 */
template <typename btn>
btnMap<btn>::btnMap(
	std::initializer_list<std::variant<CommandBtns, PIDBtns, ValBtns, CtrlBtns>> btns 
	, std::initializer_list<bool> states
) {
	
};

namespace TuningCLI {
	/**
	 * string that clears controller text
	 */
	const std::string ctrlClearText = "               ";

    /**
     * CONFIG vars:
     */
    const bool usingPhysicalPIDTuner = false;

    const bool runningLinearPIDTest = false;

    /**
     * RUNTIME vars:
     */
    bool tuningPID = true;

	/**
	 * Whether we're tuning linear or angular PID
	 */
	lemlib::PID* pid = &(TuningCLI::runningLinearPIDTest ? chassis.lateralPID
											  			 : chassis.angularPID);

    bool runningPIDTest = false;

	// runtime var GROUP -> handles input
	TuningCLIState state = TuningCLIState::CMD;

	CommandBtns command = CommandBtns::NONE;
	PIDBtns pid_const = PIDBtns::NONE;
	float* pid_const_var = nullptr;

	std::string curr_text = ""; 

	/**
	 * Function to print if necessary, and clear the
	 * controller before doing so
	 */
	void ctrlPrint(std::uint8_t line, std::uint8_t col, std::string text) {
		// clears line (prints max length string with whitespace)
		// so artifacts from a bigger line do not persist
		// when printing a shorter line
		if (text != curr_text) {
			std::cout << "getting new text!" << std::endl;
			
			curr_text = text;

			// controller.set_text(line, col, ctrlClearText);
			controller.clear_line(0);

			pros::delay(50);
		}

		// prints line anyways bc has to be constantly printed in order to work
		controller.set_text(line, col, text);
	}
};

void tuning_cli_screen_task(void* chassis) {
	lemlib::Chassis* myChassis = (lemlib::Chassis *)(chassis);

	while (true) {
		char runningPIDTestBufferString[20];

		// the space in the format string is important... don't delete it!
		std::snprintf(
			runningPIDTestBufferString
			, sizeof(runningPIDTestBufferString)
			, " PID Test? %s"
			, TuningCLI::runningPIDTest ? "YES" : "NO"
		);

		// first value: whether tuning PID / normal driver control is enabled
		// second value: if tuning PID, whether test PID auton running or not
		pros::lcd::print(
			0
			, "Tuning? %s%s"
			, TuningCLI::tuningPID ? "YES" : "NO"
			, TuningCLI::tuningPID ? runningPIDTestBufferString : ""
		);

		pros::lcd::print(1, "Pos X (Relative): %f", myChassis->getPose().x);
		pros::lcd::print(2, "Pos Y (Relative): %f", myChassis->getPose().y);
		pros::lcd::print(3, "Bot Heading (Relative): %f", myChassis->getPose().theta);

		pros::lcd::print(
			4
			, "CURRENT %s kP: %f"
			, TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR"
			, TuningCLI::runningLinearPIDTest ? myChassis->lateralPID.kP : myChassis->angularPID.kP 
		);

		pros::lcd::print(
			5
			, "CURRENT %s kD: %f"
			, TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR"
			, TuningCLI::runningLinearPIDTest ? myChassis->lateralPID.kD : myChassis->angularPID.kD
		);
		
		pros::delay(20);
	}
}

void btnListener(void* param) {
	std::cout << "started btnListener!" << std::endl;
	
	while (TuningCLI::tuningPID) {
		if (TuningCLI::state == TuningCLIState::CMD) {
			std::map<CommandBtns, bool> cmdBtnMap {
				// SET: X
				{
					CommandBtns::SET
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_X
						)
					)
				
				// GET: A
				}, {
					CommandBtns::GET
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_A
						)
					)
				}
			};

			for (const auto& btn : cmdBtnMap) {
				// if a button is pressed
				if (btn.second) {
					TuningCLI::state = TuningCLIState::VAR;
					TuningCLI::command = btn.first;
				
					// ignore all other buttons!
					break;
				}
			}
		} else if (TuningCLI::state == TuningCLIState::VAR) {
			std::map<PIDBtns, bool> varBtnMap {
				// P: X
				{
					PIDBtns::P
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_X
						)
					)
				
				// D: A
				}, {
					PIDBtns::D
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_A
						)
					)
				}
			};

			for (const auto& btn : varBtnMap) {
				// if a button is being pressed
				if (btn.second) {
					TuningCLI::pid_const = btn.first;

					if (TuningCLI::command == CommandBtns::SET) {
						TuningCLI::state = TuningCLIState::VAL;
					} else if (TuningCLI::command == CommandBtns::GET) {
						TuningCLI::state = TuningCLIState::DISP;
					}

					// ignore all other buttons!
					break;
				}
			}
		} else if (TuningCLI::state == TuningCLIState::VAL) {
			std::map<ValBtns, bool> valBtnMap = {
				// +1: UP ARROW
				{
					ValBtns::PLUSONE
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_UP
						)
					)
				
				// +2: RIGHT ARROW
				}, {
					ValBtns::PLUSTWO
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_RIGHT
						)
					)
				
				// -1: DOWN ARROW
				}, {
					ValBtns::MINUSONE
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_DOWN
						)
					)
				
				// -2: LEFT ARROW
				}, {
					ValBtns::MINUSTWO
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_LEFT
						)
					)
				}
			};

			for (const auto& btn : valBtnMap) {
				// if a button is being pressed
				if (btn.second) {
					std::cout << "button pressed!" << std::endl;

					TuningCLI::pid_const_var = &(TuningCLI::pid_const == PIDBtns::P ? TuningCLI::pid->kP
																					: TuningCLI::pid->kD);
					switch (btn.first) {
						case ValBtns::PLUSONE:
							++(*TuningCLI::pid_const_var);
							TuningCLI::state = TuningCLIState::DISP;

							break;
						case ValBtns::MINUSONE:
							--(*TuningCLI::pid_const_var);
							TuningCLI::state = TuningCLIState::DISP;

							break;
						case ValBtns::PLUSTWO:
							*TuningCLI::pid_const_var += 2;
							TuningCLI::state = TuningCLIState::DISP;

							break;
						case ValBtns::MINUSTWO:
							*TuningCLI::pid_const_var -= 2;
							TuningCLI::state = TuningCLIState::DISP;

							break;
					}
				
					// ignore all other buttons!
					break;
				}
			}
		} else if (TuningCLI::state == TuningCLIState::DISP) {
			std::map<CtrlBtns, bool> ctrlBtnMap = {
				{
					CtrlBtns::NEXT
					, static_cast<bool>(
						controller.get_digital_new_press(
							pros::E_CONTROLLER_DIGITAL_B
						)
					)
				}
			};

			for (const auto& btn : ctrlBtnMap) {
				switch (btn.first) {
					case CtrlBtns::NEXT:
						// IF the NEXT button is pressed,
						// RESET ALL VARS!
						if (btn.second) {
							TuningCLI::state = TuningCLIState::CMD;

							TuningCLI::command = CommandBtns::NONE;
							TuningCLI::pid_const = PIDBtns::NONE;
							TuningCLI::pid_const_var = nullptr;

							std::string curr_text = "";

							break;
						}
				}

				// ignore all other buttons!
				break;
			}
		} else if (TuningCLI::state == TuningCLIState::TUNE) {

		}
		
		// delay to save system resources
		pros::delay(20);
	}
}

void tuningCLI() {
	// informs user what MODE they're tuning
	std::cout << "currently tuning " << (TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR") << " PID!" << std::endl << std::endl; 

	// starts btn listener task
	pros::Task btn_listener(
		btnListener
		, (void*)""
	);

	while (TuningCLI::tuningPID) {
		try {
			if (TuningCLI::state == TuningCLIState::CMD) {
				// informs user they can start typing command
				// std::cout << "enter command> ";
				
				TuningCLI::ctrlPrint(0, 0, "enter command:");	
			} else if (TuningCLI::state == TuningCLIState::VAR) {
				switch (TuningCLI::command) {
					case CommandBtns::SET:
						TuningCLI::ctrlPrint(0, 0, "const to set:");

						break;
					case CommandBtns::GET:
						TuningCLI::ctrlPrint(0, 0, "const to get:");

						break;
					default:
						break;
				}
			} else if (TuningCLI::state == TuningCLIState::VAL) {
				switch (TuningCLI::pid_const) {
					case PIDBtns::P:
						TuningCLI::ctrlPrint(0, 0, "NEW kP val: ");

						break;
					case PIDBtns::D:
						TuningCLI::ctrlPrint(0, 0, "NEW kD val: ");

						break;
					default:
						break;
				}
 			} else if (TuningCLI::state == TuningCLIState::DISP) {
				std::stringstream pid_const_text_ss;
				pid_const_text_ss << 
					((TuningCLI::pid_const == PIDBtns::P) ? "kP" : "kD")
					 << " val: "
					 << ((TuningCLI::pid_const == PIDBtns::P) ? TuningCLI::pid->kP : TuningCLI::pid->kD);
				std::string pid_const_text = pid_const_text_ss.str();

				TuningCLI::ctrlPrint(0, 0, pid_const_text);
			}
			
		// 	} else if (command == "turn-left") { // returns robot to original position IF tuning angular PID
		// 		if (!TuningCLI::runningLinearPIDTest) {
		// 			chassis.setPose(0, 0, 0);

		// 			chassis.turnToHeading(-90, 1500, {}, false);
		// 		}

		// 	} else if (command == "run" || command == "r") {
		// 		TuningCLI::runningPIDTest = true;

		// 		// resets position before runs, in case test auton is being run multiple times
		// 		chassis.setPose(0, 0, 0);

		// 		// running linear PID test
		// 		if (TuningCLI::runningLinearPIDTest) {
		// 			chassis.moveToPoint(0, -24, 3000, linearPIDTestMoveToPointParams, false);
				
		// 		// running angular PID test
		// 		} else {
		// 			chassis.turnToHeading(90, 1500, {}, false);

		// 			pros::delay(1000);
		// 		}
			
		// 	} else if (command == "stop") {
		// 		chassis.cancelAllMotions();
			
		// 	} else if (command == "exit") {
		// 		std::cout << " | switching to driver control..." << std::endl << std::endl << "---" << std::endl << std::endl;

		// 		chassis.cancelAllMotions();
		// 		TuningCLI::tuningPID = false;

		// 		break;
			
		// 	} else {
		// 		std::cout << "| not a valid command..." << std::endl;
		// 	}
			pros::delay(50);
		} catch (std::exception e) {
			TuningCLI::ctrlPrint(0, 0, "ERROR");

			std::cout << " | something went wrong. please remove the try/catch statements and manually catch the error!" << std::endl;
		}
	}
}
