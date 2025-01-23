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
        // REDO:
        // red range: 9 - 15 (12 apparently?)
        // blue RANGE: 200 - 320ish
        // blue actual: ~240 - 270
        // steady state: just say 20-40
        double STARTING_HUE;

        const double RED_MIN = 0.0;
        const double RED_MAX = 15.0;

        const double STEADY_MIN = 20.0;
        const double STEADY_MAX = 40.0;

        const double BLUE_MIN = 200.0;
        const double BLUE_MAX = 300.0;

        pros::MotorGroup intake_motors;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        pros::Optical color_sensor;

        // 0 -> running freely
        // 1 -> running color sort task
        int state;

        bool debounce;

        pros::Task* color_sort_task;

        bool color;

        // 0 -> not running (CONTINUALLY BRAKING)
        // 1 -> intaking
        // 2 -> outtaking
        int auton_running;

        // 0 -> none
        // 1 -> red
        // 2 -> blue
        int first_received;
        bool first_ended = false;
        // same as first_received
        int second_received;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
            , std::uint8_t color_port
            , bool type
        );

        void intake();
        void intake_auton();
        void floating();

        void outtake();
        void outtake_auton();

        void brake();

        void brake_auton();

        void lift(bool set);

        void toggle();

        void hues_debug();

        void handle_driver_input(bool R1_pressed, bool R2_pressed);
        void update_sort(bool R1_pressed, bool R2_pressed);
};
