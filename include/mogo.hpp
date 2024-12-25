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

        // core features
        void set(bool val);
        
        void toggle();
        
        // semantic convenience
        void clamp();

        void release();

        // implements limit switch
        bool check_if_mogo();


        // automatic clamp handlers!
        void request_clamp();

        void cancel_clamp();

        void handle_clamp_requests();

        void handle_clamp_requests_task(void* mogoMechVoid);
};
