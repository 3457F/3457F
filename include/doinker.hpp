#pragma once

#include "api.h"

class Doinker {
    public:
        pros::adi::Port doinker_piston;

        Doinker(
            std::uint8_t doinker_piston_port
        );

        void toggle();
};