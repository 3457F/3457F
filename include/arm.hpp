#pragma once

#include "api.h"
#include "lemlib/pid.hpp"
#include "pros/rotation.hpp"

class Arm {
    public:
        pros::Motor arm_motor;

        pros::motor_brake_mode_e brake_mode;

        pros::Rotation encoder;

        lemlib::PID pid;
        int target;

        Arm(
            std::int8_t arm_motor_port

            , pros::motor_brake_mode_e arm_brake_mode,

            std::int8_t encoder_port
        );

        void move(int pos);

        void update();

        void arm_up();

        void arm_down();

        void brake();
};