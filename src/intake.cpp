#include <algorithm>
#include <cstdint>
#include "intake.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.h"
#include "util.hpp"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , std::uint8_t floating_motor_port
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t intake_piston_port
    , std::uint8_t color_port
    , std::uint8_t limit_switch_port
    // TODO: fix
    , bool type
) : intake_motors(intake_motor_ports)
    , floating_motor(floating_motor_port)
    , intake_piston(intake_piston_port, pros::E_ADI_DIGITAL_OUT)
    , color_sensor(color_port)
    , limit_switch(limit_switch_port, pros::E_ADI_DIGITAL_IN)
{
    intake_brake_mode = brake_mode;
    color = type;

    intake_motors.set_brake_mode_all(brake_mode);

    state = 0;
    color_sort_task = nullptr;

    held_ring = 1;
}

void Intake::floating_intake() {
    floating_motor.move(-127);
}

void Intake::intake() {
    intake_motors.move(-127);
}

void Intake::intake_auton() {
    auton_running = 1;
}

void Intake::outtake() {
    intake_motors.move(127);
}

void Intake::outtake_auton() {
    auton_running = 2;
}

void Intake::brake() {
    intake_motors.brake();
}

void Intake::brake_auton() {
    auton_running = 0;
}

void Intake::lift(bool set) {
    intake_piston.set_value(set);
}

void Intake::toggle() {
    intake_piston.set_value(!intake_piston.get_value());
}

/**
 * limit switch detection
 */
bool Intake::is_ring_on_top() {
    return limit_switch.get_value();
}

// --------

/**
 * used to tune color sensor; prints detected hue values
 * to terminal
 */
void Intake::hues_debug() {
    // keeps color sensor white LED on, so it can more accurately detect color
    color_sensor.set_led_pwm(100);

    double hue = color_sensor.get_hue();

    printf("detecting hue: %f\n", hue);
}

// meant to be run as a task, called from `handle_driver_input` when
// opposite alliance ring finds its way into our intake
void throws_ring(void* intakeVoid) {
    Intake* intake = (Intake*)(intakeVoid);

    intake->brake();    
    pros::delay(75);
    
    // returns back to normal driver control, also
    // resets ring
    intake->held_ring = 0;
    intake->state = 0;
}

// for skills, when ring being intaked while arm at LOADIN_POS
// meant to be run as a TASK
void dunk_arm(void* armVoid) {
    Arm* arm = (Arm*)(armVoid);

    pros::delay(1000);
    arm->set_pos(arm->LOADIN_POS);
}

// /** meant to be run as a task, called EVERY 20 MS */
// void Intake::check_color_sensor() {
//     // keeps color sensor white LED on, so it can more accurately detect color
//     color_sensor.set_led_pwm(100);

//     double hue = color_sensor.get_hue();

//     if (within(hue, BLUE_MIN, BLUE_MAX)) {
//         // rmbr, default is red so if blue, NEED TO CHANGE!!!!

//         if (held_ring != 1) {
//             printf(
//                 "DETECTED RING: %d | with hue: %f\n", held_ring, hue
//             );
            
//             held_ring = 1;
//         }
//     } else {
//         printf("we have a %d ring!\n", held_ring);
//     }
// }

/** meant to be run as a task, called EVERY 20 MS */
void Intake::check_color_sensor() {
    // keeps color sensor white LED on, so it can more accurately detect color
    color_sensor.set_led_pwm(100);

    double hue = color_sensor.get_hue();

    // if it is a ring
    if (within(hue, BLUE_MIN, BLUE_MAX) || within(hue, RED_MIN, RED_MAX)) {
        if (arm.target == arm.LOADIN_POS) {
            pros::Task dunk_task(dunk_arm, static_cast<void*>(&arm));
        }
        
        // if (within(hue, BLUE_MIN, BLUE_MAX)) {
        //     // rmbr, default is red so if blue, NEED TO CHANGE!!!!
    
        //     if (held_ring != 1) {
        //         printf(
        //             "DETECTED RING: %d | with hue: %f\n", held_ring, hue
        //         );
                
        //         held_ring = 1;
        //     }
        // } else {
        //     printf("we have a %d ring!\n", held_ring);
        // }
    }
}

/** t h e  r i n g  i s  h e r e  ...  score or throw? */
void Intake::check_limit_switch() {
    // if bad then throw
    if (
        is_ring_on_top() // if it's a ring
        && (held_ring == !color) // AND it's the opposing ring
    ) {
        printf(
            // "THROWING OUT RING (held ring: %s | vs alliance: %s)\n"
            // , held_ring == 1 ? "red" : "blue"
            // , color == 1 ? "red" : "blue"
            "THROWING OUT RING (held ring: %d | vs alliance: %d)\n"
            , held_ring
            , color
        );
        color_sort_task = new pros::Task(&throws_ring, this);
    }
}

void Intake::handle_driver_input(bool R1_pressed, bool R2_pressed) {
    // no bad rings! BRRR KEEP GOING DRIVER
    if (state == 0) {
        if (R1_pressed == R2_pressed) {
            brake();
        } else if (R1_pressed) {
            intake();
        } else if (R2_pressed) {
            outtake();
        }
    // uh oh we are eliminating the bad rings. must wait!
    } else if (state == 1) {
        return;
    }
}