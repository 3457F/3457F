#pragma once

#include "api.h"

class MogoMech {
    public:
        pros::adi::Port mogo_pistons;

        MogoMech(
            std::uint8_t mogo_pistons_port
        );

        void toggle();
};
