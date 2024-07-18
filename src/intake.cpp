#include <cstdint>
#include "intake.hpp"

Intake::Intake(
    std::int8_t intake_motor_port
    , pros::motor_brake_mode_e intake_brake_mode
    , std::uint8_t intake_piston_port
) : intake_motor(intake_motor_port), intake_piston(intake_piston_port) {
    brake_mode = intake_brake_mode;
    intake_motor.set_brake_mode(intake_brake_mode);
}

void Intake::intake() {
    intake_motor.move(127);
}

void Intake::outtake() {
    intake_motor.move(-127);
}

void Intake::brake() {
    intake_motor.brake();
}

void Intake::lift(bool set) {
    intake_piston.set_value(set);
}
