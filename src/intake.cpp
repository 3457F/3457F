#include <cstdint>
#include "intake.hpp"
#include "main.h"
#include "pros/adi.hpp"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t color_port
    , std::uint8_t distance_sensor_port
    // TODO: fix
    , bool type
) : intake_motors(intake_motor_ports)
    , color_sensor(color_port)
    , distance_sensor(distance_sensor_port)
{
    intake_brake_mode = brake_mode;
    color = type;

    intake_motors.set_brake_mode_all(brake_mode);

    color_sort_task = nullptr;

    held_ring = RED; // 0 --> red, 1 --> blue

    sort_next = false;
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
 * limit switch detection
 */
bool Intake::is_ring_on_top() {
    return distance_sensor.get()>5;
}

void Intake::check_color() {
    if (color_sensor.get_hue() > 150) {
        held_ring = 1;
    } else {
        held_ring = 0;
    }

    if (color != held_ring) {
        sort_next = true;
    }
}

void update_sort(void* b) {
    Intake* a = ((Intake *) b);
    while (true) {
        pros::delay(20);
        // if (arm.target == arm.LOADIN_POS) { continue; }

        // a->check_color();

        // if (a->sort_next) {
        //     a->brake();
        //     pros::delay(250);
        //     a->sort_next = false;
        // }

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            a->intake();
        } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            a->outtake();
        } else if (curr) { a->brake(); }
    }
}
