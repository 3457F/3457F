#pragma once

#include "pros/distance.hpp"
#include "pros/motor_group.hpp"
#include "pros/optical.hpp"

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

        pros::motor_brake_mode_e_t intake_brake_mode;

        pros::Optical color_sensor;

        pros::Distance distance_sensor;

        pros::Task* color_sort_task;

        int alliance_color;

        // 0 -> not running (CONTINUALLY BRAKING)
        // 1 -> intaking
        // 2 -> outtaking
        int auton_running;

        const int RED = 0;
        const int BLUE = 1;

        /**
         * 0 - red (DEFAULT)
         * 1 - blue
         */
        int held_ring_color;

        /** since the color sensor defaults to red, after a blue ring leaves,
          * the robot goes back to detecting red. there has to be some way for
          * the robot to "remember" that it saw a blue ring */
        bool has_been_blue;

        bool sort_next_ring;

        Intake(
            std::initializer_list<std::int8_t> intake_motor_ports
            , pros::motor_brake_mode_e_t brake_mode
            , std::uint8_t color_port
            , std::uint8_t distance_sensor_port
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

        bool is_ring_on_top();

        // ------

        void check_color();
};

void update_sort(void* intake);
