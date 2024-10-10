/**
 * ---
 * COMMANDS:
 * 
 * CMD: (state)
 * - Y -> SET
 * - B -> GET
 * - UP -> MOVEFWD (pid test auton - LATERAL)
 * - LEFT -> TURNLEFT (pid test auton - ANGULAR)
 * - X & A -> exit pid tuner
 * 
 * ---
 * 
 * ---
 * VAR: (state)
 * - X -> kP
 * - A -> kD
 * ---
 * 
 * ---
 * VAL: (state)
 * - UP -> PLUS ONE
 * - RIGHT -> PLUS TWO
 * - DOWN -> MINUS ONE
 * - LEFT -> MINUS TWO
 * ---
 * 
 * ---
 * DISP: (state)
 * - B -> NEXT (command)
 * ---
 * 
 * REMEMBER:
 * - tuning:
 * - doesn't oscillate? record kP and kD; INCREASE kP
 * - otherwise (oscillates)? INCREASE kD
 */

#pragma once

#include <vector>

#include "util.hpp"

/**
 * enums representing different "commands" represented by buttons!
 */

enum class TuningCLIState {
	NONE = 0
	, CMD = 1
	, VAR = 2
	, VAL = 3
	, DISP = 4
	, TUNE = 5
};


enum class CommandBtns {
	NONE = 0
	, SET = 1
	, GET = 2
	, MOVEFWD = 3
	, TURNLEFT = 4
	, STOP = 5
	, EXIT_X = 6
	, EXIT_A = 7
};

enum class PIDBtns {
	NONE = 0
	, P = 1
	, D = 2
};

enum class ValBtns {
	NONE = 0
	, PLUSONE = 1
	, PLUSTWO = 2
	, MINUSONE = 3
	, MINUSTWO = 4
};

enum class CtrlBtns {
	NONE = 0
	, NEXT = 1
};

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
        // std::vector<buttonState<btn>> btnMap;
        // std::vector<btn> btnMap;
        btn firstBtn;

        // for CommandBtns handling ONLY
        int exit_cond = 0;

        btnMapper(
            std::initializer_list<btn> btns 
            , std::initializer_list<pros::controller_digital_e_t> states
        );
};


template <typename btn>
btnMapper<btn>::btnMapper(
    std::initializer_list<btn> btns 
    , std::initializer_list<pros::controller_digital_e_t> states
): firstBtn(btn::NONE) {
    if (btns.size() != states.size()) {
        std::cout << "GRRR NOT SAME SIZE";
    }

    auto btnIt = btns.begin();
    auto statesIt = states.begin();

    for (int i = 0; i < btns.size(); i++) {
        btn stateBtn = *btnIt;
        bool state = controller.get_digital_new_press(*statesIt);

        if (state) {
            firstBtn = stateBtn;
            if constexpr (std::is_same_v<btn, CommandBtns>) {
                // exit_cond = 2 ONLY if both pressed!
                if ((firstBtn == CommandBtns::EXIT_A)
					|| (firstBtn == CommandBtns::EXIT_X)) {
                    exit_cond++;
                }
            } else {
                break;
            }
        }

        ++btnIt;
        ++statesIt;
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
