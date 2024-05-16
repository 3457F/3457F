#pragma once

#include "main.h"

class MogoMech {
    public:
        pros::ADIDigitalOut mogo_pistons;
        bool mogo_mech_enabled = false;

        MogoMech(
            std::uint8_t mogo_pistons_port
        );

        void update();

        void toggle();
};
