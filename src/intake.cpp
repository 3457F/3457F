#include <cstdint>
#include "intake.hpp"
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

// meant to be run as a task, or every 20ms!
void Intake::update_sort(bool R1_pressed, bool R2_pressed) {
    // // keeps color sensor white LED on, so it can more accurately detect color
    // color_sensor.set_led_pwm(100);

    // // if in free driver control mode
    // if (state == 0) {
    //     // checks if we're dealing with a ring we don't want -- for testing it's red
    //     if (within(color_sensor.get_hue(), color, 10)) {
    //         // throw the red ring!
    //         state = 1;
    //         color_sort_task = new pros::Task(&throws_ring, this);
        
    //     // otherwise run normal driver control version of intake!
    //     } else {
            if (R1_pressed == R2_pressed) {
                brake();
            } else if (R1_pressed) {
                intake();
            } else if (R2_pressed) {
                outtake();
            }
    //     }    // if running the color sort task
    // } else if (state == 1) {
    //     // don't disturb it! wait until color sorting is done
    //     return;
    // }
}

// meant to be run as a task
void throws_ring(void* intakeVoid) {
    // TODO: literally not running when intake is naturally running,
    // only seems to happen when you're putting the ring directly on top of the 
    // color sensor

    std::cout << "trying to throw ring..." << std::endl;

    Intake* intake = (Intake*)(intakeVoid);

    intake->outtake();
    // TODO: tune delay
    pros::delay(25);

    intake->intake();
    // TODO: tune delay
    pros::delay(25);
    
    intake->outtake();
    //this is so that right before the ring gets towards the top and (falls down and scorces on the mogo :( ) it can outtake (which will fling the ring, and also doesn't need to be tuned as much in terms of delays)

    pros::delay(10);

    intake->brake();
    // TODO: tune delay
    pros::delay(750);
    
    // returns back to normal driver control
    intake->state = 0;
}

// -- [[ 
//      DEFINED OUTSIDE OF `Intake` CLASS
// ]]

// meant to be run as a task, or every 20ms!
void update_sort_auton(void* intakeVoid) {
    while (true) {
        Intake* intake = static_cast<Intake*>(intakeVoid);

        // keeps color sensor white LED on, so it can more accurately detect color
        intake->color_sensor.set_led_pwm(100);

        std::cout << "state: " << intake->state << " | ";

        // if in free driver control mode
        if (intake->state == 0) {
            // checks if we're dealing with a ring we don't want -- for testing it's red
            if (within(intake->color_sensor.get_hue(), intake->color, 30)) {
                // throw the red ring!
                intake->state = 1;
                intake->color_sort_task = new pros::Task(&throws_ring, intake);

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
            // return;
        }

        pros::delay(20);   
    }
}