#include "arm.hpp"
#include "lemlib/pid.hpp"

#include "util.hpp"

// meant to be run as a task
float update_info(void* setInfoVoid) {
    SetInfo* setInfo = static_cast<SetInfo*>(setInfoVoid);

    float error = static_cast<float>(setInfo->target - setInfo->curr_angle);
   
    float pid_unit = setInfo->pid.update(error);

    return pid_unit;
};

void update(void* fetchInfoVoid) {
    FetchInfo* fetchInfo = static_cast<FetchInfo*>(fetchInfoVoid);

    std::int32_t curr_angle;

    SetInfo setInfo = {
        fetchInfo->pid
        , fetchInfo->encoder->get_angle()
        , fetchInfo->target
    };

    float pid_unit = update_info(&setInfo);

    std::cout << "voltage being set: " 
              << pid_unit 
              << " | curr position: " 
              << fetchInfo->encoder->get_angle();

    fetchInfo->arm->arm_motor.move_voltage(pid_unit);
};

Arm::Arm(
    std::int8_t arm_motor_port
    , pros::motor_brake_mode_e arm_brake_mode
    , std::int8_t encoder_port
): arm_motor(arm_motor_port), encoder(encoder_port), pid(5, 0, 4) {
    brake_mode = arm_brake_mode;

    arm_motor.set_brake_mode(arm_brake_mode);
    // resets to 0
    encoder.reset_position();
    // resets built-up integral and derivative
    pid.reset();

    state = 0;

    this->set_pos(START_POS);

    // FetchInfo 

    // pros::Task arm_task(update, static_cast<void*>(fetchInfo));
}

void Arm::set_pos(float target_val) {
    target = target_val;
}

// void Arm::move(int pos) {
//     arm_motor.move_absolute(pos, 100);
// }

void Arm::down_arrow() {
    // START
    if (state == 0) {
        state = 1;
        this->set_pos(LOADIN_POS);
    }
    
    // LOADIN
    else if (state == 1) {
        state = 2;
        this->set_pos(SCORE_POS);
    }
}

// BACK TO LOADIN
void Arm::right_arrow() {
    state = 1;
    this->set_pos(LOADIN_POS);
}

void Arm::arm_up() {
    // counter-clockwise is up
    arm_motor.move(127);
}

void Arm::arm_down() {
    // clockwise is down
    arm_motor.move(-127);
}

void Arm::brake() {
    arm_motor.brake();
}