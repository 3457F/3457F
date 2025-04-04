#pragma once

#include "api.h"
#include "pros/colors.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include <vector>

void throws_ring(void* intakeVoid);

class Intake {
    public:
        double STARTING_HUE;

        const double RED_MIN = 5.0;
        const double RED_MAX = 15.0;

        // w/o 10-17
        const double STEADY_MIN = 30.0;
        const double STEADY_MAX = 40.0;

        const double BLUE_MIN = 220.0;
        const double BLUE_MAX = 240.0;

        pros::MotorGroup intake_motors;

        /** defining STUFF that it uses */
        pros::Motor floating_motor;

        pros::adi::Port intake_piston;

        pros::motor_brake_mode_e_t intake_brake_mode;

        pros::Optical color_sensor;

        pros::adi::Port limit_switch;

        // 0 -> running freely
        // 1 -> running color sort task
        int state;

        int color_in_intake;

        pros::Task* color_sort_task;

        bool color;

        // 0 -> not running (CONTINUALLY BRAKING)
        // 1 -> intaking
        // 2 -> outtaking
        int auton_running;

        /**
         * 0 - red (DEFAULT)
         * 1 - blue
         */
        int held_ring;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , std::uint8_t floating_motor_port
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t intake_piston_port
            , std::uint8_t color_port
            , std::uint8_t limit_switch_port
            , bool type
        );

        // config
        void set_brake_mode(pros::motor_brake_mode_e_t mode);

        void intake();
        void intake_auton();
        void floating_intake();

        void outtake();
        void outtake_auton();

        void brake();

        void brake_auton();

        void lift(bool set);

        void toggle();

        bool is_ring_on_top();

        // ------

        void check_color_sensor();
        void check_limit_switch();

        void hues_debug();

        void handle_driver_input(bool R1_pressed, bool R2_pressed);
        void update_sort(bool R1_pressed, bool R2_pressed);
};
