#include <cstdint>
#include "intake.hpp"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t intake_piston_port
    , std::uint8_t color_port
    , bool type
) : intake_motors(intake_motor_ports), intake_piston(intake_piston_port, pros::E_ADI_DIGITAL_OUT), color_sensor(color_port) {
    intake_brake_mode = brake_mode;
    color = type;

    intake_motors.set_brake_mode_all(brake_mode);
}

void Intake::intake() {
    intake_motors.move(-127);
}

void Intake::outtake() {
    intake_motors.move(127);
}

void Intake::update_sort() {
    if (color) {
        if (std::abs(color_sensor.get_hue()-200) < 50) {
            intake();
            pros::delay(50);
            brake();
            pros::delay(100);
        }
    } else {
        if (std::abs(color_sensor.get_hue()-12) < 10) {
            intake();
            pros::delay(10);
            brake();
            pros::delay(100);
        }
    }
}

void Intake::brake() {
    intake_motors.brake();
}

void Intake::lift(bool set) {
    intake_piston.set_value(set);
}

void Intake::toggle() {
    intake_piston.set_value(!intake_piston.get_value());
}