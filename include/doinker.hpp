#pragma once

#include "api.h"

class Doinker {
    public:
        // pros::adi::Port doinker_piston;
        pros::adi::Port left_doinker_piston;
        pros::adi::Port right_doinker_piston;

        Doinker(
            std::uint8_t left_doinker_piston_port,
            std::uint8_t right_doinker_piston_port
        );

        void toggle();
};