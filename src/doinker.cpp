#include "doinker.hpp"

Doinker::Doinker(
    std::uint8_t left_doinker_piston_port,
    std::uint8_t right_doinker_piston_port
) : left_doinker_piston(left_doinker_piston_port, pros::E_ADI_DIGITAL_OUT),
    right_doinker_piston(right_doinker_piston_port, pros::E_ADI_DIGITAL_OUT) {};

void Doinker::toggle() {
    left_doinker_piston.set_value(!left_doinker_piston.get_value());
    // right_doinker_piston.set_value(!right_doinker_piston.get_value());
}

void Doinker::left_toggle() {
    left_doinker_piston.set_value(!left_doinker_piston.get_value());
}

void Doinker::right_toggle() {
    right_doinker_piston.set_value(!right_doinker_piston.get_value());
}