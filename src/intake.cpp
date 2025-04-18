#include <cstdint>
#include "intake.hpp"
#include "main.h"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , std::uint8_t intake_piston_port
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t color_port
    , std::uint8_t distance_sensor_port
    , bool type
) : intake_motors(intake_motor_ports)
    , intake_piston(intake_piston_port)
    , color_sensor(color_port)
    , distance_sensor(distance_sensor_port)
{
    // init intake motors
    intake_brake_mode = brake_mode;
    intake_motors.set_brake_mode_all(brake_mode);

    // init intake piston
    intake_lifted = false;
    intake_piston.set_value(intake_lifted);
    
    // init color sort
    alliance_color = type;
    color_sensor.set_led_pwm(100);
    color_sensor.set_integration_time(3);
    
    color_sort_task = nullptr;

    held_ring_color = 0;
}

// config
void Intake::set_brake_mode(pros::motor_brake_mode_e_t mode) {
    intake_motors.set_brake_mode_all(mode);
}

// intake lift functions
void Intake::lift(bool value) {
    intake_lifted = value;
    intake_piston.set_value(intake_lifted);
}

// intake functions
void Intake::intake() {
    intake_motors.move(-127);
}

void Intake::intake_auton() {
    auton_running = 1;
}

void Intake::outtake() {
    intake_motors.move(127);
}

void Intake::outtake_auton() {
    auton_running = 2;
}

void Intake::brake() {
    intake_motors.brake();
}

void Intake::brake_auton() {
    auton_running = 0;
}

// color sort functions

/**
 * whether a ring is either...
 * - in the arm, or
 * - about to be scored onto the mogo
 */
bool Intake::is_ring_on_top() {
    // TODO: test, might have to refine if distance value defaults to 0
    return distance_sensor.get() < 35;
}

void Intake::check_color() {
    // Blue hue is about 220-230.
    if (color_sensor.get_hue() > 150) {
        held_ring_color = BLUE;

        // If the robot was sitting in the default state of "RED" beforehand,
        // make sure to correct this misconception.
        if (alliance_color == BLUE) {
            sort_next_ring = false;
        }
    }

    // Red hue is about 10-20.
    if (color_sensor.get_hue() < 50) {
        // If the ring has been blue, don't "overwrite" it with red.
        if (held_ring_color == BLUE) return;
        
        held_ring_color = RED;
    }

    if (alliance_color != held_ring_color) {
        sort_next_ring = true;
    }
}

void update_sort(void* intakeVoid) {
    Intake* intake = (Intake*) intakeVoid;

    while (true) {
        pros::delay(20);

        intake->color_sensor.set_led_pwm(100);

        // Handle normal driver control.
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            // printf("intaking!\n");
            intake->intake();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            // printf("outtaking!\n");
            intake->outtake();
        } else if (in_driver_control) {
            // printf("braking!\n");
            intake->brake();
        }

        // If the ring is supposed to go in the arm, don't color sort.
        if (arm.target == arm.LOADIN_POS) continue;

        // Periodically check whether to sort the ring currently in the intake.
        intake->check_color();

        // Runs color sorting algorithm.
        if (intake->is_ring_on_top() && color_sorting) {
            printf("sorting ring!\n");

            if (intake->color_sensor.get_rgb().blue>15) {
                intake->held_ring_color = intake->BLUE;
            } else if (intake->color_sensor.get_rgb().red>35) {
                intake->held_ring_color = intake->RED;
            }

            std::cout << "b, r:"
                      << intake->color_sensor.get_rgb().blue
                      << ", " << intake->color_sensor.get_rgb().red
                      << std::endl;

            if ((intake->alliance_color != intake->held_ring_color)) {
                pros::delay(20);
                intake->brake();
                pros::delay(150);

                // reset held ring color
                intake->held_ring_color = RED;
            }

            // start intake again after color sorting
            intake->intake();
        }
    }
}