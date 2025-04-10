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
        // 25 originally; comp update went from 30 -> 35
        // 35 -- hella low cuz of slop, so 35 -> 40 -> 44
        static constexpr double LOADIN_POS = 44 * 100; // 25 originally
        static constexpr double DUNK_POS = 87.0 * 100; 
        static constexpr double SCORE_POS = 160.0 * 100; // 140 originally
        static constexpr double AUTON = 200.0 * 100;
        // TODO: made it a little bit lower like 197 so that
        // the ring doesn't get stuck on arm poly
        static constexpr double ALLIANCE_SCORE = 236.0 * 100;
        static constexpr double INTAKE_LIFT = 238.0 * 100;
        static constexpr double PREALLIANCE_SCORE = 170.0 * 100;
        static constexpr double FORCE_POS = 300.0 * 100;
        static constexpr double HOLD_POS = 90.0 * 100;
        static constexpr double MOGO_SCORE = 208.0 * 100;
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

        void release();

        void hold();

        void score();
};