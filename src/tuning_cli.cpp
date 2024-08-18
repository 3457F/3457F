#include "tuning_cli.hpp"

lemlib::MoveToPointParams linearPIDTestMoveToPointParams = {
	.forwards = false
	, .maxSpeed = 127
	, .minSpeed = 0
	, .earlyExitRange = 0
};

namespace TuningCLI {
    /**
     * CONFIG vars:
     */
    const bool usingPhysicalPIDTuner = false;

    const bool runningLinearPIDTest = false;

    /**
     * RUNTIME vars:
     */
    bool tuningPID = false;

    bool runningPIDTest = false;
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

std::vector<std::string> split(const std::string& _input,
                               const std::string& delimiter) {
	std::vector<std::string> tokens;

	std::string source = _input;

	size_t pos = 0;

	while ((pos = source.find(delimiter)) != std::string::npos) {
		tokens.push_back(source.substr(0, pos));

		source.erase(0, pos + delimiter.length());
	}

	// returns last entry AFTER delimiter
	tokens.push_back(source);

	return tokens;
}

void makeLowerCase(std::string& str) {
	std::transform(
		str.begin()		// passes in the
		, str.end()		// full string to be transformed

		, str.begin()	// section of string to start inserting
						// transformed string into
		
		// basically lambda function that returns lowercase version of
		// each character in the string
		, [](unsigned char c) { return std::tolower(c); }
	);
}

void tuningCLI() {
	lemlib::PID* pid = &(TuningCLI::runningLinearPIDTest ? chassis.lateralPID
											  : chassis.angularPID);

	// informs user what MODE they're tuning
	std::cout << "currently tuning " << (TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR") << " PID!" << std::endl << std::endl;

	while (TuningCLI::tuningPID) {
		try {
			// informs user they can start typing command
			std::cout << "enter command> ";

			// fetches command (WAITS UNTIL NEWLINE)
			// and formats it to lowercase
			std::string input;
			getline(std::cin, input);
			makeLowerCase(input);

			auto params = split(input, " ");

			std::string command = params.at(0);

			if (command == "s" || command == "set") {
				if (params.size() < 3) {
					std::cout << "not enough arguments to process request (need 3!)..." << std::endl;
					continue;
				}

				std::string whichGain = params.at(1);
				std::string valueToSetStr = params.at(2);

				float *constToSet = nullptr;


				if (whichGain == "p") {
					constToSet = &(pid->kP);
				
				} else if (whichGain == "i") {
					constToSet = &(pid->kI);
				
				} else if (whichGain == "d") {
					constToSet = &(pid->kD);
				
				} else {
					std::cout << " | INVALID gain to set!" << std::endl;
					continue;
				}

				// stores old value of the PID constant, to inform the user later
				float oldValue = (*constToSet);

				// initializes `valueToSet`
				float valueToSet = -1;

				/**
				 * if the user types in a certain string as the second parameter (that is, NOT a float),
				 * fetches the PID constant they want to change, and adds or subtracts 1 or 2, accordingly
				 * to get its new value
				*/
				if (valueToSetStr == "+1") {
					valueToSet = (*constToSet) + 1;
				} else if (valueToSetStr == "-1") {
					valueToSet = (*constToSet) - 1;
				
				} else if (valueToSetStr == "+2") {
					valueToSet = (*constToSet) + 2;
				} else if (valueToSetStr == "-2") {
					valueToSet = (*constToSet) - 2;
				
				} else {
					try {
						// tries to convert user's desired gain value to a float
						valueToSet = std::stof(valueToSetStr);
					} catch (const std::invalid_argument &e) {
						// we were scammed! the user didn't pass in a float!
						std::cout << " | Gain value not a valid float!" << std::endl;
						continue;
					}
				}

				// sets the new value of the PID constant!
				(*constToSet) = valueToSet;

				printf(" | successfully changed gain value! old value: %f, new value %f!\n", oldValue, *constToSet);

			} else if (command == "g" || command == "get") {
				if (params.size() < 2) {
					std::cout << "not enough arguments to process request (need 2!)..." << std::endl;
					continue;
				}

				std::string whatInfo = params.at(1);

				if (whatInfo == "mode") {
					std::cout << " | currently tuning " << (TuningCLI::runningLinearPIDTest ? "LINEAR" : "ANGULAR") << " PID" << std::endl;
				
				} else if (whatInfo == "p") {
					std::cout << " | kP: " << pid->kP << std::endl;

				} else if (whatInfo == "i") {
					std::cout << " | kI: " << pid->kI << std::endl;
				
				} else if (whatInfo == "d") {
					std::cout << " | kD: " << pid->kD << std::endl;
				
				} else {
					std::cout << " | INVALID gain to fetch info for!" << std::endl;
				}
			
			} else if (command == "turn-left") { // returns robot to original position IF tuning angular PID
				if (!TuningCLI::runningLinearPIDTest) {
					chassis.setPose(0, 0, 0);

					chassis.turnToHeading(-90, 1500, {}, false);
				}

			} else if (command == "run" || command == "r") {
				TuningCLI::runningPIDTest = true;

				// resets position before runs, in case test auton is being run multiple times
				chassis.setPose(0, 0, 0);

				// running linear PID test
				if (TuningCLI::runningLinearPIDTest) {
					chassis.moveToPoint(0, -24, 3000, linearPIDTestMoveToPointParams, false);
				
				// running angular PID test
				} else {
					chassis.turnToHeading(90, 1500, {}, false);

					pros::delay(1000);
				}
			
			} else if (command == "stop") {
				chassis.cancelAllMotions();
			
			} else if (command == "exit") {
				std::cout << " | switching to driver control..." << std::endl << std::endl << "---" << std::endl << std::endl;

				chassis.cancelAllMotions();
				TuningCLI::tuningPID = false;

				break;
			
			} else {
				std::cout << "| not a valid command..." << std::endl;
			}
		} catch (std::exception e) {
			std::cout << " | something went wrong. please remove the try/catch statements and manually catch the error!" << std::endl;
		}
	}
}
