#include "arm.hpp"
#include "lemlib/pid.hpp"

#include "util.hpp"

// // meant to be run as a task
// float update_info(void* setInfoVoid) {
//     SetInfo* setInfo = static_cast<SetInfo*>(setInfoVoid);

//     float error = static_cast<float>(*setInfo->target - setInfo->curr_angle);

//     float pid_unit = setInfo->pid->update(error);

//     return pid_unit;
// };

// task
void update(void* fetchInfoVoid) {
    while (true) {
        FetchInfo* fetchInfo = static_cast<FetchInfo*>(fetchInfoVoid);

        std::int32_t curr_angle = fetchInfo->encoder->get_angle();

        float error = static_cast<float>(*fetchInfo->target - curr_angle);

        SetInfo setInfo = {
            fetchInfo->pid
            , error
            , fetchInfo->encoder->get_angle()
            , fetchInfo->target
        };

        // float pid_unit = update_info(&setInfo);
        float pid_unit = fetchInfo->pid->update(error);

        std::cout 
        //         << "curr_angle (deg): "
        //         << (curr_angle / 100)
                << " | target (deg): "
                << (*fetchInfo->target / 100)
                << " | error (c*): "
                << (error / 100)
                << " | voltage being set (mV): " 
                << pid_unit
                << " | curr position (*): " 
                << (fetchInfo->encoder->get_angle() / 100)
                << std::endl;

        fetchInfo->arm->arm_motor.move_voltage(pid_unit);

        pros::delay(20);
    }
};

Arm::Arm(
    std::int8_t arm_motor_port
    , pros::motor_brake_mode_e arm_brake_mode
    , std::int8_t encoder_port
): arm_motor(arm_motor_port)
    , encoder(encoder_port)
    // kP 2, kD 10
    , pid(1.5, 0, 5)
{
    brake_mode = arm_brake_mode;

    arm_motor.set_brake_mode(arm_brake_mode);
    // resets to 0
    encoder.reset_position();
    // resets built-up integral and derivative
    pid.reset();

    state = 0;

    this->set_pos(START_POS);

    FetchInfo* fetchInfo = new FetchInfo {
        &this->pid
        , this
        , &this->target
        , &this->encoder
    };

    pros::Task arm_task(update, static_cast<void*>(fetchInfo));
}

void Arm::set_pos(float target_val) {
    target = target_val;
}

// void Arm::move(int pos) {
//     arm_motor.move_absolute(pos, 100);
// }

// to load_in pos
void Arm::down_arrow() {
    // START
    if (state == 0) {
        state = 1;

        std::cout << "LOADIN_POS" << std::endl;
        this->set_pos(LOADIN_POS);
    }
    
    // LOADIN
    else if (state == 1) {
        state = 2;

        std::cout << "VERT_POS" << std::endl;
        this->set_pos(VERT_POS);
    }
}

// to score
void Arm::up_arrow() {
    std::cout << "SCORE_POS" << std::endl;
    this->set_pos(SCORE_POS);
}


// BACK TO LOADIN
void Arm::right_arrow() {
    state = 1;

    std::cout << "LOADIN_POS (half reset)" << std::endl;
    this->set_pos(LOADIN_POS);
}

// BACK TO INIT
void Arm::left_arrow() {
    state = 0;

    std::cout << "START_POS (FULL reset)" << std::endl;
    this->set_pos(START_POS);
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