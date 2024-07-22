#include "arm.hpp"

Arm::Arm(
    std::int8_t arm_motor_port
    , pros::motor_brake_mode_e arm_brake_mode
): arm_motor(arm_motor_port) {
    brake_mode = arm_brake_mode;

    arm_motor.set_brake_mode(arm_brake_mode);
}

void Arm::arm_up() {
    // counter-clockwise is up
    arm_motor.move(-127);
}

void Arm::arm_down() {
    // clockwise is down
    arm_motor.move(127);
}

void Arm::brake() {
    arm_motor.brake();
}