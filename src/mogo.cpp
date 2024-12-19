#include <cstdint>
#include "mogo.hpp"

// task -- meant to be run every 20ms
// void handle_clamp_requests(void* mogoMechVoid) {
//     while (true) {
//         MogoMech* mogoMech = static_cast<MogoMech*>(mogoMechVoid);

//         // first off: do we want to clamp?
//         if (mogoMech->clamp_requested) {
//             // second of all: is there a mogo?
//             if (mogoMech->check_if_mogo()) {
//                 // yippee!!! clamp
//                 mogoMech->set(false);

//                 // handled request!
//                 mogoMech->clamp_requested = false;
//             }
//         }

//         pros::delay(20);
//     }
// }

MogoMech::MogoMech(
    std::uint8_t mogo_pistons_port
    
    // ours is an OFF-MOM (off-momentarily) switch;
    // ON always except for when pressed!
    , std::uint8_t limit_switch_port
) : mogo_pistons(mogo_pistons_port, pros::E_ADI_DIGITAL_OUT)
    , limit_switch(limit_switch_port, pros::E_ADI_DIGITAL_IN)
    // , clamp_request_handler(
    //     &handle_clamp_requests
    //     , this
    // ) {
    {
    // ENABLES mogo mech
    mogo_pistons.set_value(false);

    // NOT wanting to clamp rn
    clamp_requested = false;
};

bool MogoMech::check_if_mogo() {
    // false -> limit switch pressed
    // true -> limit switch not pressed --> PROBS WRONG!!!!
    return limit_switch.get_value();
}

void MogoMech::request_clamp() {
    clamp_requested = true;
}

void MogoMech::set(bool val) {
    mogo_pistons.set_value(val);
}

void MogoMech::toggle() {
    set(!mogo_pistons.get_value());
};

// run in `opcontrol();` (so every 20ms)
void MogoMech::handle_clamp_requests() {
    // debug -- prints current limit switch state
    printf("limit switch state: %d\n", this->check_if_mogo());

    // first off: do we want to clamp?
    if (this->clamp_requested) {
        // second of all: is there a mogo?
        if (this->check_if_mogo()) {
            // yippee!!! clamp
            set(true);

            // handled request!
            this->clamp_requested = false;
        }
    }
}