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


        static constexpr double START_POS = 0.0; // 0.0 -> 360.0
        // static constexpr double START_POS = 30.0 * 100; // 330.0
        // static constexpr double LOADIN_POS = -50.0 * 100; // 310.0
        // static constexpr double LOADIN_POS = 310.0 * 100;
        // 50.0 -> 38.0 -> 45.0 -> 50.0
        static constexpr double LOADIN_POS = 48.0 * 100;

        // aarav wanted to adjust +7*
        // basically score pos at this point lmao
        // 135.0 -> 150.0 -> 155.0 -> 157.0 -> 175.0 -> 165.0
        static constexpr double VERT_POS = 165.0 * 100;
        
        static constexpr double SCORE_POS = 195.0 * 100; // -135.0

        pros::Motor arm_motor;

        pros::motor_brake_mode_e brake_mode;

        pros::Rotation encoder;

        lemlib::PID pid;

        Intake* intake;

        LoadInInfo* loadInInfo;

        float target;

        // -1: not reset
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

        void set_pos(float target_val);

        void score_setup();

        void score();

        void load_in();

        void init_pos();

        // void move(int pos);

        void arm_up();

        void arm_down();

        void brake();
};