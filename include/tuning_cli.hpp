#pragma once

// for format strings
#include <sstream>
#include <variant>
#include <vector>

#include "util.hpp"

enum class TuningCLIState;

enum class NoneBtns;
enum class CommandBtns;
enum class PIDBtns;
enum class ValBtns;
enum class CtrlBtns;

// std::variant<CommandBtns, PIDBtns, ValBtns, CtrlBtns>
template <typename btn>
class buttonState {
    public:
        const btn button;
        const bool state;

        buttonState(
            btn stateButton
            , bool stateState
        );

        btn isPressed() const;
};

/**
 * DEFINING BTN STATE CLASS
 */
template <typename btn>
buttonState<btn>::buttonState(
	btn stateButton
	, bool stateState
) : button(stateButton), state(stateState) {};

template <typename btn>
btn buttonState<btn>::isPressed() const {
	if (state) {
		return button;
	} else {
        return btn::NONE;
    }
};

// std::variant<CommandBtns, PIDBtns, ValBtns, CtrlBtns>
template <typename btn>
class btnMapper {
    public:
        std::vector<buttonState<btn>> btnMap;

        btnMapper(
            std::initializer_list<btn> btns 
            , std::initializer_list<pros::controller_digital_e_t> states
        );
};

template <typename btn>
btnMapper<btn>::btnMapper(
    std::initializer_list<btn> btns 
    , std::initializer_list<pros::controller_digital_e_t> states
) {
    if (btns.size() != states.size()) {
        std::cout << "GRRR NOT SAME SIZE";
    }

    auto btnIt = btns.begin();
    auto statesIt = states.begin();

    for (int i = 0; i < btns.size(); i++) {
        btn stateBtn = *btnIt;
        pros::controller_digital_e_t state = *statesIt;

        buttonState<btn> btnState(
            stateBtn
            , state
        );

        btnMap.push_back(btnState);
    }
}

namespace TuningCLI {
    /**
     * CONFIG vars:
     */


    /**
     * whether the physical PID tuner is being
     * used, or P, I, and D values are being
     * set manually in the code
     * 
     * THIS IS A LEGACY VARIABLE, KEPT JUST IN CASE
     * PHYSICAL PID TUNER IS RE-IMPLEMENTED
     */
    extern const bool usingPhysicalPIDTuner;

    /**
     * if `tuningPID` is set to `true`, whether
     * LINEAR PID is being tuned, or ANGULAR
     * PID is being tuned
     */
    extern const bool runningLinearPIDTest;


    /**
     * RUNTIME vars:
     */


    /**
     * whether PID is being tuned, or normal
     * driver control should run
     */
    extern bool tuningPID;

    /**
     * if `tuningPID` is set to `true`, whether
     * the robot is CURRENTLY running a test
     * auton with target kP and kD values!
     * 
     * should ALWAYS be set to `false` initially
     */
    extern bool runningPIDTest;
};

void tuning_cli_screen_task(void* chassis);

// std::vector<std::string> split(const std::string& _input
//                                , const std::string& delimiter);

// void makeLowerCase(std::string& str);

/**
 * inspired by code from: https://github.com/meisZWFLZ/OverUnder781X
*/
void tuningCLI();
