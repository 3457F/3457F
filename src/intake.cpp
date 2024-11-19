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

void Intake::outtake() {
    intake_motors.move(127);
}

void Intake::brake() {
    intake_motors.brake();
}

void Intake::lift(bool set) {
    intake_piston.set_value(set);
}

void Intake::toggle() {
    intake_piston.set_value(!intake_piston.get_value());
}

// meant to be run as a task
void throws_ring(void* intakeVoid) {
    Intake* intake = (Intake*)(intakeVoid);

    intake->intake();
    // TODO: tune delay
    // 69 -> 50
    pros::delay(50);

    intake->brake();
    // TODO: tune delay
    pros::delay(50);
    
    // returns back to normal driver control
    intake->state = 0;
}

void Intake::update_sort(bool R1_pressed, bool R2_pressed) {
    // keeps color sensor white LED on, so it can more accurately detect color
    color_sensor.set_led_pwm(100);

    // if in free driver control mode
    if (state == 0) {
        // checks if we're dealing with a ring we don't want -- for testing it's red
        if (within(color_sensor.get_hue(), RED_HUE, 10)) {
            // throw the red ring!
            state = 1;
            color_sort_task = new pros::Task(&throws_ring, this);
        
        // otherwise run normal driver control version of intake!
        } else {
            if (R1_pressed == R2_pressed) {
                brake();
            } else if (R1_pressed) {
                intake();
            } else if (R2_pressed) {
                outtake();
            }
        }    // if running the color sort task
    } else if (state == 1) {
        // don't disturb it! wait until color sorting is done
        return;
    }
}