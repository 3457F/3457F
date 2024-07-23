#pragma once

#include "api.h"

class Arm {
    public:
        pros::Motor arm_motor;

        pros::motor_brake_mode_e brake_mode;

        Arm(
            std::int8_t arm_motor_port

            , pros::motor_brake_mode_e arm_brake_mode
        );

        void arm_up();

        void arm_down();

        void brake();
};