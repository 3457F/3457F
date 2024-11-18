#pragma once

#include "api.h"
#include "pros/colors.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include <vector>

void throws_ring(void* intakeVoid);
void update_sort_auton(void* intakeVoid);

class Intake {
    public:
        const int RED_HUE = 10;

        // within 220 and 230 -> blue
        const int BLUE_HUE = 235;

        pros::MotorGroup intake_motors;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        pros::Optical color_sensor;

        int state;

        pros::Task* color_sort_task;

        bool color;

        // 0 -> not running
        // 1 -> intaking
        // 2 -> outtaking
        int auton_running;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
            , std::uint8_t color_port
            , bool type
        );

        void intake();
        void intake_auton();

        void outtake();
        void outtake_auton();

        void brake();
        void brake_auton();

        void lift(bool set);

        void toggle();

        void update_sort(bool R1_pressed, bool R2_pressed);
};
