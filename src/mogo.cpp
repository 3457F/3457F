#include <cstdint>
#include "mogo.hpp"

MogoMech::MogoMech(
    std::uint8_t mogo_pistons_port
) : mogo_pistons(mogo_pistons_port, pros::E_ADI_DIGITAL_OUT) {};

void MogoMech::toggle() {
    mogo_pistons.set_value(!mogo_pistons.get_value());
};