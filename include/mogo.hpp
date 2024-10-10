#pragma once

#include "api.h"
#include <initializer_list>
#include <vector>

class MogoMech {
    public:
        /**
         * RETRACTING clamps mogo;
         * EXTENDING unclamps mogo! */
        // pros::adi::Port mogo_clamp_piston;
        std::vector<pros::adi::Port> mogo_clamp_pistons;
        bool mogo_clamp_open = true;

        /**
         * RETRACTING tilts mogo;
         * EXTENDING untilts mogo! */
        // std::vector<pros::adi::Port> mogo_tilt_pistons;
        pros::adi::Port mogo_tilt_piston;
        bool mogo_tilt_open = true;

        /**
         * The mogo clamp piston AND the mogo tilt
         * pistons start OPEN! */
        MogoMech(
            std::initializer_list<std::uint8_t> mogo_clamp_pistons_ports
            , std::uint8_t mogo_tilt_piston_port
        );

        // INTERNAL FUNCTIONS
        void clamp(bool set);
        void tilt(bool set);

        // DEBUG FUNCTION
        void clamp_toggle();

        // driver control functions
        void toggle();
        void tilt_toggle();
};
