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

        /** toggles both doinkers */
        void toggle();

        /** only toggles left doinker */
        void left_toggle();
        /** only toggles right doinker */
        void right_toggle();
};