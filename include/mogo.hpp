#pragma once

#include "api.h"

class MogoMech {
    public:
        pros::adi::Port mogo_pistons;

        MogoMech(
            std::uint8_t mogo_pistons_port
        );

        // core features
        void set(bool val);
        
        void toggle();
        
        // semantic convenience
        void clamp();

        void release();

        // to unclamp mogo at end of driver control
        void set_unclamp_timer();
};
