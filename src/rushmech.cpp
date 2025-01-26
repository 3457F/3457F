#include "rushmech.hpp"

RushMech::RushMech(
    std::uint8_t rush_mech_piston_port
) : rush_mech_piston(rush_mech_piston_port, pros::E_ADI_DIGITAL_OUT) {
    rush_mech_piston.set_value(false);
}

void RushMech::set(bool val) {
    rush_mech_piston.set_value(val);
}

void RushMech::toggle() {
    set(!rush_mech_piston.get_value());
}