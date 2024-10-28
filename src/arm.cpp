#include "arm.hpp"
#include "lemlib/pid.hpp"

Arm::Arm(
    std::int8_t arm_motor_port
    , pros::motor_brake_mode_e arm_brake_mode
    , std::int8_t encoder_port
): arm_motor(arm_motor_port), encoder(encoder_port), pid(5, 0, 4) {
    brake_mode = arm_brake_mode;

    arm_motor.set_brake_mode(arm_brake_mode);
    encoder.reset_position();
    pid.reset();

    target = 0;
}

void Arm::move(int pos) {
    arm_motor.move_absolute(pos, 100);
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