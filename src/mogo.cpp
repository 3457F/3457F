#include <cstdint>
#include "mogo.hpp"
#include "pros/adi.h"

MogoMech::MogoMech(
    std::uint8_t mogo_clamp_piston_port
    , std::initializer_list<std::uint8_t> mogo_tilt_pistons_ports
) : mogo_clamp_piston(mogo_clamp_piston_port, pros::E_ADI_DIGITAL_OUT) {
    // DEFAULT opens mogo clamp piston!
    mogo_clamp_piston.set_value(mogo_clamp_open);

    for (auto& mogo_tilt_piston_port : mogo_tilt_pistons_ports) {
        pros::adi::Port temp_tilt_piston(mogo_tilt_piston_port, pros::E_ADI_DIGITAL_OUT);
        // DEFAULT opens mogo tilt pistons!
        temp_tilt_piston.set_value(mogo_tilt_open);

        mogo_tilt_pistons.push_back(temp_tilt_piston);
    }
};

void MogoMech::clamp(bool set) {
    mogo_clamp_piston.set_value(set);

    mogo_clamp_open = set;
}

void MogoMech::tilt(bool set) {
    for (auto& mogo_tilt_piston : mogo_tilt_pistons) {
        mogo_tilt_piston.set_value(set);
    };

    mogo_tilt_open = set;
}

void MogoMech::clamp_toggle() {
    this->clamp(!mogo_clamp_piston.get_value());
}

void MogoMech::tilt_toggle() {
    this->tilt(!mogo_tilt_open);
}

void MogoMech::toggle() {
    bool is_fully_clamped = mogo_clamp_open && mogo_tilt_open;

    // if clamped
    if (is_fully_clamped){
        // driver wants to unclamp!

        // untilts
        this->tilt(1);
        // runs in task so it doesn't disturb the
        // driver control loop!
        pros::Task([this](void* param) {
            // wait for a bit so mogo doesn't get stuck on robot...
            pros::delay(250);
            // ...THEN unclamps
            this->clamp(1);
        });

    // else if unclamped
    } if (!is_fully_clamped) {
        // driver wants to clamp!

        // clamps
        this->clamp(0);
        // tilts
        this->tilt(0);
    }
}