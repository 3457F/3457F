#include <cstdint>
#include "intake.hpp"

Intake::Intake(
    std::int8_t intake_motor_port
    , pros::motor_brake_mode_e intake_brake_mode
) : intake_motor(intake_motor_port) {
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
