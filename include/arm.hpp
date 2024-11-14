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


        // static constexpr double START_POS = 330.0 * 100; // 0.0 -> 360.0
        static constexpr double START_POS = 0.0 * 100; // "330" -> 0
        // 17 (barely went) -> 40 (barely went) -> 90 (overshot) -> 70 -> 60 (hung over) -> 55 (SLIGHTLY HIGH) -> 53
        // 53 is a bit too high so went 50
        static constexpr double LOADIN_POS = 50.0 * 100; // "313" -> 343
        // 138 (basically vertical) -> 148 (better but not enough) -> 158 -> 168
        // 168 -> 166
        static constexpr double SCORE_POS = 166.0 * 100; // "192" -> 222

        


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