#include <cstdint>
#include <initializer_list>
#include "intake.hpp"
#include "pros/adi.h"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t intake_piston_port
    , std::uint8_t pressure_hood_piston_port
) : intake_motors(intake_motor_ports)
    , intake_piston(intake_piston_port, pros::E_ADI_DIGITAL_OUT)
    , pressure_hood_piston(pressure_hood_piston_port, pros::E_ADI_DIGITAL_OUT) {
    intake_brake_mode = brake_mode;

    intake_motors.set_brake_mode_all(brake_mode);
}

void Intake::intake() {
    intake_motors.move(-127);
}

void Intake::outtake() {
    intake_motors.move(127);
}

void Intake::brake() {
    intake_motors.brake();
}

void Intake::lift(bool set) {
    intake_piston.set_value(set);
}

// pressure hood -> ONLY FOR SMORG
void Intake::hood_toggle() {
    pressure_hood_piston.set_value(!pressure_hood_piston.get_value());
}

void Intake::toggle() {
    intake_piston.set_value(!intake_piston.get_value());
}