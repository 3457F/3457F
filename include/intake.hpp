#pragma once

#include "api.h"

class Intake {
    public:
        pros::Motor intake_motor;

        pros::adi::DigitalOut intake_piston;

        pros::motor_brake_mode_e brake_mode;

        Intake(
            std::int8_t intake_motor_port
            , pros::motor_brake_mode_e brake_mode
            , std::uint8_t intake_piston_port
        );

        void intake();

        void outtake();

        void brake();

        void lift(bool set);
};
