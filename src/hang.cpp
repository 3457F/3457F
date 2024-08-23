#include "hang.hpp"

Hang::Hang(
    std::uint8_t hang_piston_port
) : hang_piston(hang_piston_port, pros::E_ADI_DIGITAL_OUT) {};

void Hang::toggle() {
    hang_piston.set_value(!hang_piston.get_value());
}