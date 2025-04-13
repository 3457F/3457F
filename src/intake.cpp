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
    has_been_blue = false;
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
        held_ring_color = BLUE;
        has_been_blue = true;
    }
    // Red hue is about 10-20.
    else if (color_sensor.get_hue() < 50) {
        held_ring_color = RED;
    }

    // don't succumb... don't succumb to the temptation... don't think it's
    // a red ring... please no... PLEASE NOOOOO
    if ((alliance_color == BLUE) && has_been_blue) {
        return;
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

        // periodically update held_ring_color, and whether to sort next ring
        intake->check_color();

        // printf("detected hue: %d (%.2f)\n", intake->held_ring_color, intake->color_sensor.get_hue());
        printf("color: %s | hue: %f | next: %s | ", 
            intake->alliance_color == RED ? "RED" : "BLUE",
            intake->color_sensor.get_hue(),
            intake->sort_next_ring == true ? "TRUE" : "FALSE"
        );
        if (intake->held_ring_color == intake->BLUE) {
            printf("detected blue... grrr\n");
        }
        if (intake->held_ring_color == intake->RED) {
            printf("detected red... grrr\n");
        }

        // Runs color sorting algorithm.
        if (intake->sort_next_ring && intake->is_ring_on_top()) {
            printf("sorting ring!\n");

            // TODO: tune a delay before this
            intake->brake();
            pros::delay(250);

            // now that the ring is gone, we don't know anything about
            // new rings
            intake->has_been_blue = false;
            // Ring has left the intake -- doesn't need to be sorted any more.
            intake->sort_next_ring = false;
        }
    }
}