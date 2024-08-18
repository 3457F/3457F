#include "slapper.hpp"

Slapper::Slapper(
    std::uint8_t slapper_piston_port
) : slapper_piston(slapper_piston_port, pros::E_ADI_DIGITAL_OUT) {};

void Slapper::toggle() {
    slapper_piston.set_value(!slapper_piston.get_value());
}