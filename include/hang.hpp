// port D

#pragma once

#include "api.h"

class Hang {
    public:
        pros::adi::Port hang_piston;

        Hang(
            std::uint8_t hang_piston_port
        );

        void toggle();
};