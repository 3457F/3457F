#include <cstdint>
#include "mogo.hpp"

MogoMech::MogoMech(
    std::uint8_t mogo_pistons_port
) : mogo_pistons(mogo_pistons_port, pros::E_ADI_DIGITAL_OUT)
{
    release();
};

void MogoMech::set(bool val) {
    mogo_pistons.set_value(val);
}

void MogoMech::toggle() {
    set(!mogo_pistons.get_value());
};

void MogoMech::clamp() {
    set(true);
}

void MogoMech::release() {
    set(false);
}

void unclamp_timer() {
    
}

void MogoMech::set_unclamp_timer() {

}