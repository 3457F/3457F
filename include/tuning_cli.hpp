#pragma once

// for format strings
#include <sstream>
#include <variant>

#include "util.hpp"

enum class CommandBtns;
enum class PIDBtns;
enum class ValBtns;
enum class CtrlBtns;
enum class TuningCLIState;

template <typename btn, bool>
class btnMap {
    public:
        std::map<btn, bool> america;

        btnMap(
            std::initializer_list<std::variant<CommandBtns, PIDBtns, ValBtns, CtrlBtns>> btns 
            , std::initializer_list<bool> states
        );
};

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

std::vector<std::string> split(const std::string& _input
                               , const std::string& delimiter);

void makeLowerCase(std::string& str);

/**
 * inspired by code from: https://github.com/meisZWFLZ/OverUnder781X
*/
void tuningCLI();
