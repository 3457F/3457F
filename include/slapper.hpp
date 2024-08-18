#pragma once

#include "api.h"

class Slapper {
    public:
        pros::adi::Port slapper_piston;

        Slapper(
            std::uint8_t slapper_piston_port
        );

        void toggle();
};