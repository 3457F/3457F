#pragma once

#include "api.h"

class Intake {
    public:
        pros::MotorGroup intake_motors;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
        );

        void intake();

        void outtake();

        void brake();

        void lift(bool set);

        void toggle();
};