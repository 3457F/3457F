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
        // 1 is extend piston, 0 is retract piston
        std::vector<pros::adi::Port> mogo_clamp_pistons;
        bool mogo_clamp_val = true;

        /**
         * RETRACTING tilts mogo;
         * EXTENDING untilts mogo! */
        // std::vector<pros::adi::Port> mogo_tilt_pistons;
        // 1 is retract piston; 0 is extend piston
        pros::adi::Port mogo_tilt_piston;
        bool mogo_tilt_val = true;

        // clamping: clamp pistons EXTEND (1), THEN retract tilt pistons (1) to tilt

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

        // FULLY clamping and FULLY releasing functions (DEBUG too)
        void full_clamp();
        void full_release();

        // driver control functions
        void toggle();
        void tilt_toggle(); 
};
