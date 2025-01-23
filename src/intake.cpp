#include <algorithm>
#include <cstdint>
#include "intake.hpp"
#include "pros/adi.hpp"
#include "pros/rtos.h"
#include "util.hpp"

Intake::Intake(
    std::initializer_list<std::int8_t> intake_motor_ports
    , pros::motor_brake_mode_e_t brake_mode
    , std::uint8_t intake_piston_port
    , std::uint8_t color_port
    , bool type
) : intake_motors(intake_motor_ports), intake_piston(intake_piston_port, pros::E_ADI_DIGITAL_OUT), color_sensor(color_port) {
    intake_brake_mode = brake_mode;
    color = type;

    intake_motors.set_brake_mode_all(brake_mode);

    state = 0;
    color_sort_task = nullptr;
}
void Intake::floating(){
    pros::Motor floating(1);
    floating.move(-127);
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

void Intake::hues_debug() {
    double hue = color_sensor.get_hue();

    // if we escape the starting hues
    if (!within(hue, STARTING_HUE, 10)) {
        // print the hue!
        std::cout << "significant hue: " << hue << std::endl;
    }
}

void floating(){
    pros::Motor floating(1);
    floating.move(127);
}

// meant to be run as a task
void throws_ring(void* intakeVoid) {
    // TODO: literally not running when intake is naturally running,
    // only seems to happen when you're putting the ring directly on top of the 
    // color sensor

    // std::cout << "trying to throw ring..." << std::endl;

    Intake* intake = (Intake*)(intakeVoid);

    // moves ring a bit more up the intake
    intake->intake();
    pros::delay(75); 


    intake->brake();    
    pros::delay(50);
    
    // returns back to normal driver control
    intake->state = 0;

    pros::delay(200);
    intake->debounce = false;
}

void Intake::handle_driver_input(bool R1_pressed, bool R2_pressed) {
    if (R1_pressed == R2_pressed) {
        brake();
    } else if (R1_pressed) {
        intake();
    } else if (R2_pressed) {
        outtake();
    }
}

// meant to be run as a task, or every 10ms!
void Intake::update_sort(bool R1_pressed, bool R2_pressed) {
    // keeps color sensor white LED on, so it can more accurately detect color
    color_sensor.set_led_pwm(100);

    // debug: print hue detected

    if (state == 0) {
        if (within_range(color_sensor.get_hue(), RED_MIN, RED_MAX) && debounce == false) {
            printf("throwing red!");
            state = 1;
            debounce = true;
            color_sort_task = new pros::Task(&throws_ring, this);
        }

        printf("detected hue: %f\n", color_sensor.get_hue());

        handle_driver_input(R1_pressed, R2_pressed);
    } else if (state == 1) {
        // if running color sort task
        return;
    }
}

// -- [[ 
//      DEFINED OUTSIDE OF `Intake` CLASS
// ]]

// meant to be run as a task
void update_sort_auton(void* intakeVoid) {
    Intake* intake = static_cast<Intake*>(intakeVoid);

    while (true) {
        // keeps color sensor white LED on, so it can more accurately detect color
        intake->color_sensor.set_led_pwm(100);

        // if in free "auton control" mode
        // (following instructions of whatever
        // the auton wants to run at that time)
        if (intake->state == 0) {
            // checks if we're dealing with a ring we don't want (opposite alliance)
            if (within(intake->color_sensor.get_hue(), intake->color, 28)) {
                // throw the opposite alliance ring
             
                intake->color_sort_task = new pros::Task(&throws_ring, intake);
                intake->state = 1;
            
            // otherwise run normal driver control version of intake!
            } else {
                std::cout << "auton req: " << intake->auton_running << std::endl;

                if (intake->auton_running == 0) {
                    intake->brake();
                } else if (intake->auton_running == 1) {
                    intake->intake();
                } else if (intake->auton_running == 2) {
                    intake->outtake();
                }
            }
        
        // if running the color sort task
        } else if (intake->state == 1) {
            // don't disturb it! wait until color sorting is done
        }

        pros::delay(20);   
    }
}