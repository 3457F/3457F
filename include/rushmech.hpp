#pragma once

#include "api.h"

class RushMech {
    public:
        pros::adi::Port rush_mech_piston;

        RushMech(
            std::uint8_t rush_mech_piston_port
        );

        void set(bool val);

        void toggle();
};