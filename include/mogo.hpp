#pragma once

#include "api.h"

class MogoMech {
    public:
        pros::adi::Port mogo_pistons;
        bool clamp_requested;
        // pros::Task clamp_request_handler;

        pros::adi::Port limit_switch;

        MogoMech(
            std::uint8_t mogo_pistons_port
            , std::uint8_t limit_switch_port
        );

        bool check_if_mogo();

        void request_clamp();

        void set(bool val);
        
        void toggle();

        void handle_clamp_requests();
};
