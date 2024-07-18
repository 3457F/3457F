#pragma once

#include "api.h"

class MogoMech {
    public:
        pros::adi::DigitalOut mogo_pistons;
        bool mogo_mech_enabled = false;

        MogoMech(
            std::uint8_t mogo_pistons_port
        );

        void update();

        void toggle();
};
