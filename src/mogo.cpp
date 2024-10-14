#include <cstdint>
#include "pros/adi.h"

#include "util.hpp"
#include "mogo.hpp"

MogoMech::MogoMech(
    std::initializer_list<std::uint8_t> mogo_clamp_pistons_ports
    , std::uint8_t mogo_tilt_piston_port
) : mogo_tilt_piston(mogo_tilt_piston_port, pros::E_ADI_DIGITAL_OUT) {
    // DEFAULT opens mogo tilt piston!
    // mogo_tilt_piston.set_value(mogo_tilt_val);

    for (auto& mogo_clamp_piston_port : mogo_clamp_pistons_ports) {
        pros::adi::Port temp_clamp_piston(mogo_clamp_piston_port, pros::E_ADI_DIGITAL_OUT);
        // DEFAULT opens mogo tilt pistons!
        // temp_clamp_piston.set_value(mogo_tilt_val);

        mogo_clamp_pistons.push_back(temp_clamp_piston);
    };

    full_release();
};

void MogoMech::clamp(bool set) {
    for (auto& mogo_clamp_piston : mogo_clamp_pistons) {
        mogo_clamp_piston.set_value(set);
    };

    mogo_clamp_val = set;
}

void MogoMech::tilt(bool set) {
    mogo_tilt_piston.set_value(set);

    mogo_tilt_val = set;
}

void MogoMech::clamp_toggle() {
    this->clamp(!mogo_clamp_val);
}

void MogoMech::tilt_toggle() {
    this->tilt(!mogo_tilt_val);
}


void clampMogoTilt(void* mogoPointer) {
    MogoMech* mogoMech = (MogoMech *)(mogoPointer);

    // wait for a bit so mogo doesn't get stuck on robot...
    pros::delay(250);
    // ...THEN clamps
    // 1 -> 0
    mogoMech->tilt(1);
}

// function definitions for FULLY clamping and FULLY releasing mogo!
void MogoMech::full_clamp() {
    // driver wants to clamp!

    // clamps
    this->clamp(1);

    // runs in task so it doesn't disturb the
    // driver control loop!
    MogoMech* globalThisPointer = this;

    pros::Task(clampMogoTilt, globalThisPointer);
}

void MogoMech::full_release() {
    // unclamps
    this->clamp(0);
    // untilts
    this->tilt(0);
}

// clamp: 0 (unclamped) ; 1 (clamped)
// tilt: 0 (untilt) ; 1 (tilt)
void MogoMech::toggle() {
    // some boolean algebra i don't feel like explaining, basically both have to be 0
    bool is_fully_opened = !(mogo_clamp_val || mogo_tilt_val);

    // if fully opened
    if (is_fully_opened) {
        // fully clamp!
        full_clamp();

    // else if unclamped
    } else if (!is_fully_opened) {
        // unclamp!

        full_release();
    } else {
        // gives feedback to driver that disallowed
        controller.rumble(".");
    }
}