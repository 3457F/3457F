#pragma once

#include "api.h"
#include "pros/colors.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include <vector>

class Intake {
    public:
        const int RED_HUE = 10;

        pros::MotorGroup intake_motors;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        pros::Optical color_sensor;

        int state;

        pros::Task* color_sort_task;

        bool color;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
            , std::uint8_t color_port
            , bool type
        );

        void intake();

        void outtake();

        void brake();

        void lift(bool set);

        void toggle();

        void update_sort(bool R1_pressed, bool R2_pressed);
};
