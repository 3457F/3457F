#include <cstdint>

#include "mogo.hpp"

MogoMech::MogoMech(std::uint8_t mogo_pistons_port) : mogo_pistons(mogo_pistons_port) {};

void MogoMech::update() {
    mogo_pistons.set_value(mogo_mech_enabled);
}

void MogoMech::toggle() {
    mogo_mech_enabled = !mogo_mech_enabled;

    this->update();
};
