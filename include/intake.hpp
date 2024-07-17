#pragma once

#include "api.h"

class Intake {
    public:
        pros::Motor intake_motor;

        pros::motor_brake_mode_e brake_mode;

        Intake(
            std::int8_t intake_motor_port
            , pros::motor_brake_mode_e brake_mode
        );

        void intake();

        void outtake();

        void brake();
};
