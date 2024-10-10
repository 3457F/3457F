#include <cstdint>
#include "mogo.hpp"
#include "pros/adi.h"

void clampMogo(void* mogoPointer) {
    MogoMech* mogoMech = (MogoMech *)(mogoPointer);

    // wait for a bit so mogo doesn't get stuck on robot...
    pros::delay(250);
    // ...THEN clamps
    mogoMech->clamp(0);
}

MogoMech::MogoMech(
    std::initializer_list<std::uint8_t> mogo_clamp_pistons_ports
    , std::uint8_t mogo_tilt_piston_port
) : mogo_tilt_piston(mogo_tilt_piston_port, pros::E_ADI_DIGITAL_OUT) {
    // DEFAULT opens mogo tilt piston!
    mogo_tilt_piston.set_value(mogo_tilt_open);

    for (auto& mogo_clamp_piston_port : mogo_clamp_pistons_ports) {
        pros::adi::Port temp_clamp_piston(mogo_clamp_piston_port, pros::E_ADI_DIGITAL_OUT);
        // DEFAULT opens mogo tilt pistons!
        temp_clamp_piston.set_value(mogo_tilt_open);

        mogo_clamp_pistons.push_back(temp_clamp_piston);
    }
};

void MogoMech::clamp(bool set) {
    for (auto& mogo_clamp_piston : mogo_clamp_pistons) {
        mogo_clamp_piston.set_value(set);
    };

    mogo_clamp_open = set;
}

void MogoMech::tilt(bool set) {
    mogo_tilt_piston.set_value(set);

    mogo_tilt_open = set;
}

void MogoMech::clamp_toggle() {
    this->clamp(!mogo_clamp_open);
}

void MogoMech::tilt_toggle() {
    this->tilt(!mogo_tilt_open);
}

void MogoMech::toggle() {
    bool is_fully_opened = mogo_clamp_open && mogo_tilt_open;

    // if fully opened
    if (is_fully_opened) {
        // driver wants to clamp!

        // tilts
        this->tilt(0);
        // runs in task so it doesn't disturb the
        // driver control loop!
        MogoMech* globalThisPointer = this;

        pros::Task(clampMogo, globalThisPointer);

    // else if unclamped
    } if (!is_fully_opened) {
        // driver wants to clamp!

        // unclamps
        this->clamp(1);
        // untilts
        this->tilt(1);
    }
}