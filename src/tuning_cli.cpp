#include <sstream>
#include <type_traits>

#include "tuning_cli.hpp"
#include "pros/misc.h"

// define string names for enums
std::vector<std::string> TuningCLIStateNames = {
    "NONE"
    , "CHOOSE COMMAND"
    , "CHOOSE CONSTANT"
    , "CHOOSE VALUE"
    , "DISPLAY VALUE"
    , "RUN TEST AUTON"
};

std::vector<std::string> CommandBtnsNames = {
	"NONE"
	, "SET VARIABLE"
	, "GET VARIABLE"
	, "MOVE FORWARD"
	, "TURN LEFT"
	, "STOP AUTON"
	, "EXIT_X"			// probably won't be used; just here in case
	, "EXIT_A"			// probably won't be used; just here in case
};

std::vector<std::string> PIDBtnsNames = {
	"NONE"
	, "kP"
	, "kD"
};

std::vector<std::string> ValBtnsNames = {
	"NONE"
	, "PLUS ONE"
	, "PLUS TWO"
	, "MINUS ONE"
	, "MINUS TWO"
};

std::vector<std::string> CtrlBtnsNames = {
	"NONE"
	, "NEXT"
};

// RMBR: 20 ms delay for normal pros funcs;
// 50 ms delay for printing to controller!

namespace TuningCLI {
	/**
	 * string that clears controller text
	 */
	const std::string ctrlClearText = "               ";

    /**
     * CONFIG vars:
     */
	// legacy: when we had the actual optical shaft encoders to adjust kP and kD :P
    // const bool usingPhysicalPIDTuner = false;

    const bool runningLinearPIDTest = true;

    /**
     * RUNTIME vars:
     */
	
	/**
	 * DEFAULTS to true; in other words, driver control NOT default enabled!
	 */
    bool tuningPID = true;

	/**
	 * Whether we're tuning linear or angular PID
	 * TODO: consider adding wrapper to runningLinearPIDTest so when changed, will automatically change the pid variable!
	 */
	lemlib::PID* pid = &(TuningCLI::runningLinearPIDTest ? chassis.lateralPID
											  			 : chassis.angularPID);

	// runtime vars that handle input
	TuningCLIState state = TuningCLIState::CMD;
	CommandBtns command = CommandBtns::NONE;
	PIDBtns pid_const = PIDBtns::NONE;
	float* pid_const_var = nullptr;
	bool pid_test_auton_ran = false;

	std::string curr_text = "";

	void resetState() {
		state = TuningCLIState::CMD;
		command = CommandBtns::NONE;
		pid_const = PIDBtns::NONE;
		pid_const_var = nullptr;
		pid_test_auton_ran = false;
	};

	/**
	 * Function to print if necessary, and clear the
	 * controller before doing so
	 */
	// void ctrlPrint(std::uint8_t line, std::uint8_t col, std::string text) {
	void ctrlPrint(std::string text) {
		// IF printing a string that's not already on the screen
		if (text != curr_text) {
			std::cout << "getting new text!" << std::endl;
			
			curr_text = text;

			// clears line (prints max length string with whitespace)
			// so artifacts from a bigger line do not persist
			// when printing a shorter line
			controller.clear_line(0);

			// prints PID text to terminal
			std::cout << (TuningCLI::runningLinearPIDTest ? "[LINEAR] " : "[ANGULAR] ")
					  << "kP: " << TuningCLI::pid->kP
					  << " | "
					  << "kD: " << TuningCLI::pid->kD
					  << std::endl;

			pros::delay(50);
		}

		// prints line anyways bc has to be constantly printed in order to work
		controller.set_text(0, 0, text);
	}
};

void tuning_cli_screen_task(void* chassis) {
	lemlib::Chassis* myChassis = (lemlib::Chassis *)(chassis);

	while (true) {
		// char runningPIDTestBufferString[20];
		
		// space in format string is important!
		std::stringstream running_pid_test_ss;
		running_pid_test_ss <<
			"Tuning? "
			<< (TuningCLI::tuningPID ? "YES" : "NO")
			<< " | PID test? "
			<< (myChassis->isInMotion() ? "YES" : "NO");
		std::string running_pid_test_text = running_pid_test_ss.str();
		
		// first value: whether tuning PID / normal driver control is enabled
		// second value: if tuning PID, whether test PID auton running or not
		pros::lcd::set_text(0, running_pid_test_text);

		pros::lcd::print(1, "Pos X (Relative): %f", myChassis->getPose().x);
		pros::lcd::print(2, "Pos Y (Relative): %f", myChassis->getPose().y);
		pros::lcd::print(3, "Bot Heading (Relative): %f", myChassis->getPose().theta);

		pros::lcd::print(
			4
			, "CURRENT %s kP: %f"
			, TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR"
			, TuningCLI::pid->kP
		);

		pros::lcd::print(
			5
			, "CURRENT %s kD: %f"
			, TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR"
			, TuningCLI::pid->kD
		);
		
		pros::delay(20);
	}
}

/**
 * INPUT part of the tuning cli function
 */
void btnListener(void* param) {
	std::cout << "started btnListener!" << std::endl << std::endl;
	
	while (TuningCLI::tuningPID) {
		if (TuningCLI::state == TuningCLIState::CMD) {
			btnMapper<CommandBtns> cmdBtnMap(
				{ 
					CommandBtns::SET
					, CommandBtns::GET
					, CommandBtns::MOVEFWD
					, CommandBtns::TURNLEFT
					, CommandBtns::EXIT_X
					, CommandBtns::EXIT_A
				}, {
					pros::E_CONTROLLER_DIGITAL_Y		// SET
					, pros::E_CONTROLLER_DIGITAL_B		// GET
					, pros::E_CONTROLLER_DIGITAL_UP 	// move forward (pid tuning auton)
					, pros::E_CONTROLLER_DIGITAL_LEFT	// turn left (pid tuning auton)
					, pros::E_CONTROLLER_DIGITAL_X
					, pros::E_CONTROLLER_DIGITAL_A
				}
			);

			// has to reach x AND a pressed (or 2)
			int exit_cond = 0;

			// for (const auto& commandBtn : cmdBtnMap.btnMap) {
			CommandBtns commandBtn = cmdBtnMap.firstBtn;

			// if a button is pressed, handle it accordingly
			if (commandBtn != CommandBtns::NONE) {
				if (cmdBtnMap.exit_cond >= 2) {
					std::cout << "switching to driver control!" << std::endl;

					TuningCLI::resetState();

					TuningCLI::tuningPID = false;
				} else {
					std::cout << "sending to command: " << CommandBtnsNames[(int)commandBtn] << std::endl;

					TuningCLI::command = commandBtn;
				}

				if (commandBtn == CommandBtns::STOP) {
					chassis.cancelAllMotions();
				} else if ((commandBtn == CommandBtns::MOVEFWD) || (commandBtn == CommandBtns::TURNLEFT)) {
					TuningCLI::state = TuningCLIState::TUNE;
				} else {
					TuningCLI::state = TuningCLIState::VAR;
				}

				std::cout << "setting state to: " << TuningCLIStateNames[(int)TuningCLI::state] << std::endl;
			}
		} else if (TuningCLI::state == TuningCLIState::VAR) {
			btnMapper<PIDBtns> varBtnMap(
				{ 
					PIDBtns::P
					, PIDBtns::D
				}, {
					pros::E_CONTROLLER_DIGITAL_X	// kP
					, pros::E_CONTROLLER_DIGITAL_A	// kD
				}
			);

			PIDBtns pidBtn = varBtnMap.firstBtn;

			// if a button is being pressed
			if (pidBtn != PIDBtns::NONE) {
				TuningCLI::pid_const = pidBtn;

				if (TuningCLI::command == CommandBtns::SET) {
					TuningCLI::state = TuningCLIState::VAL;
				} else if (TuningCLI::command == CommandBtns::GET) {					
					TuningCLI::state = TuningCLIState::DISP;
				}

				std::cout << "using PID const: " << PIDBtnsNames[(int)pidBtn] << std::endl;
				std::cout << "setting state to: " << TuningCLIStateNames[(int)TuningCLI::state] << std::endl;
			}
		} else if (TuningCLI::state == TuningCLIState::VAL) {
			btnMapper<ValBtns> valBtnMap(
				{
					ValBtns::PLUSONE
					, ValBtns::PLUSTWO
					, ValBtns::MINUSONE
					, ValBtns::MINUSTWO
				}, {
					pros::E_CONTROLLER_DIGITAL_UP		// +1
					, pros::E_CONTROLLER_DIGITAL_RIGHT	// +2
					, pros::E_CONTROLLER_DIGITAL_DOWN	// -1
					, pros::E_CONTROLLER_DIGITAL_LEFT	// -2
				}
			);

			ValBtns valBtn = valBtnMap.firstBtn;

			// if a button is being pressed
			if (valBtn != ValBtns::NONE) {
				TuningCLI::pid_const_var = &(TuningCLI::pid_const == PIDBtns::P ? TuningCLI::pid->kP
																				: TuningCLI::pid->kD);
				
				switch (valBtn) {
					case ValBtns::PLUSONE:
						++(*TuningCLI::pid_const_var);
						break;
					case ValBtns::PLUSTWO:
						*TuningCLI::pid_const_var += 2;
						break;
					case ValBtns::MINUSONE:
						--(*TuningCLI::pid_const_var);
						break;
					case ValBtns::MINUSTWO:
						*TuningCLI::pid_const_var -= 2;
						break;
				}

				TuningCLI::state = TuningCLIState::DISP;
			
				std::cout << "modifying const to: " << ValBtnsNames[(int)valBtn] << std::endl;
				std::cout << "setting state to: " << TuningCLIStateNames[(int)TuningCLI::state] << std::endl;
			}
		} else if (TuningCLI::state == TuningCLIState::DISP) {
			btnMapper<CtrlBtns> ctrlBtnMap(
				{ CtrlBtns::NEXT }
				, { pros::E_CONTROLLER_DIGITAL_B }
			);

			CtrlBtns ctrlBtn = ctrlBtnMap.firstBtn;

			// if a button is being pressed
			if (ctrlBtn != CtrlBtns::NONE) {
				switch (ctrlBtn) {
					case CtrlBtns::NEXT:
						std::cout << "moving to next command!" << std::endl;

						TuningCLI::resetState();

						break;
				}
			}
		}
		
		// delay to save system resources
		pros::delay(20);
	}
}

/**
 * PRINTING part of the tuning cli function
 */
void tuningCLI() {
	// informs user what MODE they're tuning
	std::cout << "currently tuning " << (TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR") << " PID!" << std::endl << std::endl; 

	// starts btn listener task
	pros::Task btn_listener(
		btnListener
		, (void*)"" // dummy param bc required by pros::Task
	);

	while (TuningCLI::tuningPID) {
		try {
			if (TuningCLI::state == TuningCLIState::CMD) {
				TuningCLI::ctrlPrint("enter command>");	
			} else if (TuningCLI::state == TuningCLIState::VAR) {
				switch (TuningCLI::command) {
					case CommandBtns::SET:
						TuningCLI::ctrlPrint("const to set:");

						break;
					case CommandBtns::GET:
						TuningCLI::ctrlPrint("const to get:");

						break;
					default:
						break;
				}
			} else if (TuningCLI::state == TuningCLIState::VAL) {
				switch (TuningCLI::pid_const) {
					case PIDBtns::P:
						TuningCLI::ctrlPrint("NEW kP val: ");

						break;
					case PIDBtns::D:
						TuningCLI::ctrlPrint("NEW kD val: ");

						break;
					default:
						break;
				}
 			} else if (TuningCLI::state == TuningCLIState::DISP) {
				// displays either the kP or kD constant, based on what the user changed
				std::stringstream pid_const_text_ss;
				pid_const_text_ss << 
					((TuningCLI::pid_const == PIDBtns::P) ? "kP" : "kD")
					 << " val: "
					 << ((TuningCLI::pid_const == PIDBtns::P) ? TuningCLI::pid->kP : TuningCLI::pid->kD);
				
				// converts it to a string and prints it to the controller!
				std::string pid_const_text = pid_const_text_ss.str();
				TuningCLI::ctrlPrint(pid_const_text);

			} else if (TuningCLI::state == TuningCLIState::TUNE) {
				// CODE STOPS AT THIS IF STATEMENT
				
				// waits until robot has come to a complete stop
				if (!chassis.isInMotion()) {
					if (!TuningCLI::pid_test_auton_ran) {
						TuningCLI::ctrlPrint("running...");
						
						chassis.setPose(0, 0, 0);

						switch (TuningCLI::command) {
							case CommandBtns::MOVEFWD:
								// turns around: NOT part of PID testing, just so it's more convenient and so that the robot doesn't go all around the place
								// chassis.turnToHeading(180, 1500, {}, false);

								// TuningCLI::resetState();

								chassis.moveToPoint(0, 24, 3000, {}, false);
								
								TuningCLI::resetState();

								break;
							case CommandBtns::TURNLEFT:
								chassis.turnToHeading(-90, 1500, {}, false);

								TuningCLI::resetState();

								break;
						}
					}
				}
			}
			
			
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
			TuningCLI::ctrlPrint("ERROR");

			std::cout << " | something went wrong. please remove the try/catch statements and manually catch the error!" << std::endl;
		}
	}
}
