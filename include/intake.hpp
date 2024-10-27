#pragma once

#include "api.h"
#include "pros/motors.hpp"
#include <vector>

class Intake {
    public:
        pros::Motor intake_motor;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        Intake(
            std::int8_t intake_motor_port
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
        );

        void intake();

        void outtake();

        void brake();

        void lift(bool set);

        void toggle();
};
