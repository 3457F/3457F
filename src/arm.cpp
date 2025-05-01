#include <cmath>

#include "consts.hpp"

#include "arm.hpp"
#include "lemlib/pid.hpp"
#include "main.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "util.hpp"
#include "intake.hpp"

struct LoadInInfo;

// calculates error when max_val = both max_val and 0!
float calc_error(float curr_val, float expected_val) {
    // std::cout << "expecting " << expected_val << " but instead got " << curr_val << "...";

    float error = expected_val - curr_val;

    // std::cout << " | final error: " << error << std::endl;

    return error;
}

// task; RUNS EVERY 20 MS TO UPDATE ARM PID
void update(void* fetchInfoVoid) {
    FetchInfo* fetchInfo = static_cast<FetchInfo*>(fetchInfoVoid);

    while (true) {
        std::int32_t curr_angle = fetchInfo->encoder->get_position();

        // float error = calc_error(
        //     static_cast<float>(*fetchInfo->target)
        //     , static_cast<float>(curr_angle)
        // );

        float error = calc_error(
            static_cast<float>(curr_angle)
            , static_cast<float>(*fetchInfo->target)
        );

        // std::cout << "error: " << error << std::endl;

        SetInfo setInfo = {
            fetchInfo->pid
            , error
            , fetchInfo->encoder->get_position()
            , fetchInfo->target
        };

        // float pid_unit = update_info(&setInfo);
        float pid_unit = fetchInfo->pid->update(error);

        // std::cout << " | resultant pid unit: " << pid_unit << std::endl;

        fetchInfo->arm->arm_motor.move_voltage(-pid_unit);
        
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
    , pid(ARM_P, 0, ARM_D)
{
    brake_mode = arm_brake_mode;

    arm_motor.set_brake_mode(arm_brake_mode);

    // resets built-up integral and derivative
    pid.reset();

    // COMMENT OUT IF ARM WILL NOT ALWAYS START AT INIT_POS
    // encoder.reset_position();

    std::cout << "INIT_POS (INITIALIZATION)";
    this->set_pos(INIT_POS);
    state = 0;

    FetchInfo* fetchInfo = new FetchInfo {
        &this->pid
        , this
        , &this->target
        , &this->encoder
    };

    pros::Task arm_task(update, static_cast<void*>(fetchInfo));
}

void Arm::debug() {
    float error_deg = (
        calc_error(
            this->encoder.get_position()
            , this->target
            // , 360 * 100
        ) / 100
    );

    std::cout 
        << "curr_angle (*): "
        << (this->encoder.get_position() / 100)
        << " | target (*): "
        << (this->target / 100)
        << " | error (*): "
        << error_deg
        // << " | voltage being set (mV): " 
        // << pid_unit
        // << " | curr position (*): " 
        // << (fetchInfo->encoder->get_angle() / 100)
        << std::endl;
}

void Arm::brake() {
    arm_motor.brake();
}


void Arm::extend() {
    // counter-clockwise is up
    arm_motor.move(-127);
}

void Arm::retract() {
    // clockwise is down
    arm_motor.move(127);
}

void Arm::set_pos(float target_val) {
    target = target_val;
}

// void Arm::move(int pos) {
//     arm_motor.move_absolute(pos, 100);
// }

// void score_cycle(void* armVoid) {
//     Arm* arm = static_cast<Arm*>(armVoid);

//     if (arm->state == 0 || arm->state == 2) {
//         arm->state = 1;
//         arm->set_pos(arm->LOADIN_POS);
//     } else if (arm->state == 1) {
//         arm->state = 2;
//         intake.intake();
//     }
// }

void Arm::score_cycle() {
    // whether at START_POS or SCORE_POS, return to LOADIN_POS
    if (state == 0 || state == 2) {
        state = 1;
        intake->intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        std::cout << "LOADIN_POS" << std::endl;


        // intake->outtake();
        // pros::delay(200);
        this->set_pos(LOADIN_POS);
        // intake->intake(); 
        // pros::delay(800);
        // intake->brake();
    }
    
    // when at LOADIN_POS, go to SCORE_POS
    else if (state == 1) {
        state = 2;
        intake->outtake();
        // intake->
        pros::delay(50);
        intake->brake();
        intake->intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        std::cout << "SCORE_POS" << std::endl;
        this->set_pos(SCORE_POS);
        // pros::delay(1000);
        // intake->intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    }
}

void Arm::start_pos() {
    state = 0;
    intake->intake_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    std::cout << "START_POS" << std::endl;
    this->set_pos(START_POS);
}

void Arm::force() {
    last_pos = target;

    set_pos(FORCE_POS);
}

void Arm::score() {
    last_pos = target;

    set_pos(SCORE_POS);
}

void Arm::release() {
    target = LOADIN_POS;
}

// basically you go here to "prepare" arm for scoring, so it's much faster
// to go to scoring when u actually get to alliance stake than if you did score
// pos only when you got there
void Arm::hold() {
    target = HOLD_POS;
}

void arm_update(void* armVoid) {
    Arm* arm = (Arm*) armVoid;

    while(true) {
        // // if arm has a ring, automatically get arm out of the way of the intake
        // if ((arm->target == arm->LOADIN_POS) && intake.is_ring_on_top()) {
        //     pros::delay(400);
            
        //     // TODO: is this necessary?
        //     // move hooks slightly so ring doesn't get stuck on it
        //     intake.outtake();
        //     pros::delay(400);
        //     intake.brake();

        //     // if driver has not moved arm out yet, do it for them!
        //     if (arm->target == arm->LOADIN_POS) {
        //         pros::delay(300);
        //         arm->set_pos(arm->HOLD_POS);
        //     }
        // }

        pros::delay(35);
    }
}