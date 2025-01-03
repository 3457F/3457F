#pragma once

#include "api.h"
#include "lemlib/pid.hpp"
#include "pros/rotation.hpp"

#include "intake.hpp"
#include "util.hpp"

// clockwise UP
// counter-clockwise DOWN
class Arm {
    public:
        // actual score position -> 195.4316402 (195)
        // OUT OF SIZE score position -> 130 (FULLY HORIZ -> OUT OF SIZE)

        // angle constants for different states of the arm
        
        // relative to the LOWEST POSITION when "retracted"
        // (not facing outwards)
        // -> start pos 330.0
        // -> load in pos 310.0
        // -> vert pos 225.0

        // length of arm: 12.159
        // ~8in can fully extend (6 to be safe)
        // -> ~60* (60.43164023*) max FROM HORIZ; ~30* (29.56835977*) max FROM VERT
        // -> score pos 195.0


        static constexpr double INIT_POS = 0.0 * 100;
        static constexpr double START_POS = 18.0 * 100;
        static constexpr double LOADIN_POS = 52.0 * 100; // 52
        static constexpr double DUNK_POS = 100.0 * 100; 
        static constexpr double SCORE_POS = 177.0 * 100;
        static constexpr double AUTON = 200.0 * 100;
        static constexpr double ALLIANCE_SCORE = 280 * 100;
        static constexpr double FORCE_POS = 290 * 100;

        double last_pos = 0;

        pros::Motor arm_motor;

        pros::motor_brake_mode_e brake_mode;

        pros::Rotation encoder;

        lemlib::PID pid;

        Intake* intake;

        LoadInInfo* loadInInfo;

        float target;

        // 0: START_POS
        // 1: LOADIN_POS
        // 2: SCORE_POS
        int state;

        Arm(
            std::int8_t arm_motor_port

            , pros::motor_brake_mode_e arm_brake_mode,

            std::int8_t encoder_port

            , Intake* intake
        );

        void debug();

        // base funcs

        void brake();

        void arm_up();

        void arm_down();

        void set_pos(float target_val);

        // extended funcs

        void score_cycle();

        void start_pos();

        void force();

        void release_force();
};