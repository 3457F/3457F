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
        static constexpr double INIT_POS = 0.0 * 100;
        static constexpr double START_POS = 18.0 * 100;
        static constexpr double LOADIN_POS = 48.0 * 100; // 45 actual; 45 -> 55 -> 50
        static constexpr double DUNK_POS = 100.0 * 100; 
        static constexpr double SCORE_POS = 155.0 * 100;
        static constexpr double AUTON = 200.0 * 100;
        static constexpr double ALLIANCE_SCORE = 190.0 * 100;
        static constexpr double FORCE_POS = 200.0 * 100;
        static constexpr double HOLD_POS = 90.0 * 100;
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

        void extend();

        void retract();

        void set_pos(float target_val);

        // void loadin();

        // extended funcs

        void score_cycle();

        void start_pos();

        void force();

        void release_force();

        void hold();
};