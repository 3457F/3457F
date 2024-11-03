#include <cmath>

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

// starts at "360" (0)
// not reachable range is about 40 - 0
void calcDist(float curr_angle, float target_angle) {
    float curr_angle_act = curr_angle;

    // if it's below/at initial position
    if (curr_angle_act < 40) {
        curr_angle_act = 360 + curr_angle_act;
    }

    float dist = target_angle - curr_angle_act;
}

struct LoadInInfo;

// task
void loadin_unstuck(void* loadInInfoVoid) {
    LoadInInfo* loadInInfo = static_cast<LoadInInfo*>(loadInInfoVoid);

    // loadInInfo->intake->outtake();
    // pros::delay(500);
    // loadInInfo->intake->brake();

    loadInInfo->arm->state = 1;

    std::cout << "LOADIN_POS (half reset)" << std::endl;
    loadInInfo->arm->set_pos(loadInInfo->arm->LOADIN_POS);
}

// task; RUNS EVERY 20 MS TO UPDATE ARM PID
void update(void* fetchInfoVoid) {
    FetchInfo* fetchInfo = static_cast<FetchInfo*>(fetchInfoVoid);

    while (true) {
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

        // std::cout 
        // //         << "curr_angle (deg): "
        // //         << (curr_angle / 100)
        //         << " | target (deg): "
        //         << (*fetchInfo->target / 100)
        //         << " | error (c*): "
        //         << (error / 100)
        //         << " | voltage being set (mV): " 
        //         << pid_unit
        //         << " | curr position (*): " 
        //         << (fetchInfo->encoder->get_angle() / 100)
        //         << std::endl;
        
        // only if the error is suuper significant, move the motor
        if (std::abs(error) > 10) {
            fetchInfo->arm->arm_motor.move_voltage(pid_unit);
        }

        pros::delay(20);
    }
};

Arm::Arm(
    std::int8_t arm_motor_port
    , pros::motor_brake_mode_e arm_brake_mode
    , std::int8_t encoder_port
    , Intake* intake
): arm_motor(arm_motor_port)
    , encoder(encoder_port)
    , intake(intake)
    , loadInInfo(new LoadInInfo{
        intake
        , this
    })
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

    // already at start pos!
    // this->set_pos(START_POS);

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

void Arm::score_setup() {
    // START
    if (state == 0) {
        state = 1;

        std::cout << "LOADIN_POS" << std::endl;
        this->set_pos(LOADIN_POS);
    }
    
    // LOADIN
    else if (state == 1) {
        // first moves the 

        state = 2;

        std::cout << "VERT_POS" << std::endl;
        this->set_pos(VERT_POS);
    }
}

// to score
void Arm::score() {
    std::cout << "SCORE_POS" << std::endl;
    this->set_pos(SCORE_POS);
}


// BACK TO LOADIN
void Arm::load_in() {
    state = 1;

    pros::Task loadin_unstuck_task(loadin_unstuck, loadInInfo);
}

// BACK TO INIT
void Arm::init_pos() {
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