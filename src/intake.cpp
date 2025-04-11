#include <cstdint>
#include "intake.hpp"
#include "main.h"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t color_port
    , std::uint8_t distance_sensor_port
    , bool type
) : intake_motors(intake_motor_ports)
    , color_sensor(color_port)
    , distance_sensor(distance_sensor_port)
{
    intake_brake_mode = brake_mode;
    alliance_color = type;

    intake_motors.set_brake_mode_all(brake_mode);

    color_sort_task = nullptr;

    held_ring_color = 0;
}

// config
void Intake::set_brake_mode(pros::motor_brake_mode_e_t mode) {
    intake_motors.set_brake_mode_all(mode);
}

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
        held_ring_color = 0;
    }
    // Red hue is about 10-20.
    else {
        held_ring_color = 1;
    }

    if (alliance_color != held_ring_color) {
        sort_next_ring = true;
    }
}

void update_sort(void* intakeVoid) {
    Intake* intake = (Intake*) intakeVoid;

    while (true) {
        // in case arm is "moving out" of the loadin position, give it time to
        // leave before checking color sort criteria
        pros::delay(20);

        // If the ring is supposed to go in the arm, don't color sort.
        if (arm.target == arm.LOADIN_POS) continue;

        // periodically update held_ring_color, and whether to sort next ring
        intake->check_color();

        // Runs color sorting algorithm.
        if (intake->sort_next_ring && intake->is_ring_on_top()) {
            intake->brake();
            pros::delay(250);

            // Ring has left the intake -- doesn't need to be sorted any more.
            intake->sort_next_ring = false;
        }

        // Handle normal driver control.
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            intake->intake();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            intake->outtake();
        } else if (in_driver_control) { intake->brake(); }
    }
}