#include "doinker.hpp"

Doinker::Doinker(
    std::uint8_t doinker_piston_port
) : doinker_piston(doinker_piston_port, pros::E_ADI_DIGITAL_OUT) {};

void Doinker::toggle() {
    doinker_piston.set_value(!doinker_piston.get_value());
}