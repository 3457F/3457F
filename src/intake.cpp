#include <cstdint>
#include "intake.hpp"
#include "main.h"
#include "pros/rtos.hpp"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , std::uint8_t intake_piston_port
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t color_port
    , std::uint8_t distance_sensor_port
    , bool type
) : intake_motors(intake_motor_ports)
    , intake_piston(intake_piston_port, pros::E_ADI_DIGITAL_OUT)
    , color_sensor(color_port)
    , distance_sensor(distance_sensor_port)
{
    // init intake motors
    intake_brake_mode = brake_mode;
    intake_motors.set_brake_mode_all(brake_mode);

    // init intake piston
    intake_lifted = false;
    intake_piston.set_value(intake_lifted);
    
    // init color sort
    alliance_color = type;
    color_sensor.set_led_pwm(100);
    color_sensor.set_integration_time(3);
    
    color_sort_task = nullptr;

    held_ring_color = 0;

    jamCounter = 0;
    enableJam = false;
}

// config
void Intake::set_brake_mode(pros::motor_brake_mode_e_t mode) {
    intake_motors.set_brake_mode_all(mode);
}

// intake lift functions
void Intake::lift(bool value) {
    intake_lifted = value;
    intake_piston.set_value(intake_lifted);
}

// intake functions
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


// color sort functions

/**
 * whether a ring is either...
 * - in the arm, or
 * - about to be scored onto the mogo
 */
bool Intake::is_ring_on_top() {
    // TODO: test, might have to refine if distance value defaults to 0
    return distance_sensor.get() < 35;
}

void Intake::check_color() {
    // Blue hue is about 220-230.
    if (color_sensor.get_hue() > 150) {
        held_ring_color = BLUE;
    }

    // Red hue is about 10-20.
    if (color_sensor.get_hue() < 50) {
        held_ring_color = RED;
    }

    if (alliance_color != held_ring_color) {
        sort_next_ring = true;
    }
}

void update_sort(void* intakeVoid) {
    Intake* intake = (Intake*) intakeVoid;

    while (true) {
        pros::delay(20);

        intake->color_sensor.set_led_pwm(100);

        if (
            (intake->intake_motors.get_actual_velocity(0) == 0 && intake->intake_motors.get_efficiency() == 0) && 
            (arm.target != arm.LOADIN_POS) && (intake->intake_motors.get_voltage()) < -6000
            )
        {   
            intake->outtake();
            pros::delay(100);
            intake->intake();
        }

        std::cout << intake->jamCounter << std::endl;

        // Handle normal driver control.
        if (in_driver_control) {
            if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
                // printf("intaking!\n");
                intake->intake();
            
            } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
                // printf("outtaking!\n");
                intake->outtake();
            } else {
                // printf("braking!\n");
                intake->brake();
            }
        }

        // If the ring is supposed to go in the arm, don't color sort.
        if (arm.target == arm.LOADIN_POS) continue;

        // TODO: temporarily not using `intake.check_color()`

        // Runs color sorting algorithm.
        if (intake->is_ring_on_top() && false) {
            if (intake->color_sensor.get_rgb().blue>15) {
                intake->held_ring_color = intake->BLUE;
            } else if (intake->color_sensor.get_rgb().red>35) {
                intake->held_ring_color = intake->RED;
            }

            std::cout << "b, r:"
                    << intake->color_sensor.get_rgb().blue
                    << ", " << intake->color_sensor.get_rgb().red
                    << std::endl;

            if ((intake->alliance_color != intake->held_ring_color)) {
                printf("sorting ring!\n");

                pros::delay(50);
                intake->brake();
                pros::delay(150);

                // intake->sort_next_ring = false;
            }

            // start intake again after color sorting
            intake->intake();
        }
    }
}