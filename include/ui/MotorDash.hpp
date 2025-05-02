#pragma once

// < 30*C --> cool
// 30*C -> 45*C --> warm
// > 45*C --> HOT

#include <string>
#include <vector>

#include "main.h"
#include "liblvgl/lvgl.h"
#include "robodash/api.h"
#include "api.h"

// ADD OTHER SENSORS AND ABILITY TO SHOW NUMBERS FROM THEM BELOW THEIR BOX!!

#define LIGHT_GREEN 0xcfffe9
#define BLACK 0x071808

#define GREEN 0x00ff00
#define ORANGE 0xffa500
#define RED_COLOR 0xff0000

#define MB_WIDTH 97
#define MB_HEIGHT 39
#define MB_ROW_START_X 17
#define MB_ROW_START_Y 68
#define MB_CELL_W 116
#define MB_ROW_HEIGHT 58

#define MB_CELL_FONT_SIZE 14

void temp_check();
void temp_check_ctrl();

// class MyMotor {
//     public:
//         std::string name;
//         pros::Motor motor;

//         double temp;

//         // funcs

//         MyMotor(std::string name, std::int8_t motor_port);

//         void update_temp();
// };

enum class DeviceType { MOTOR, PNEUMATIC };
enum class PneumaticType { NONE, PNEUMATIC_IN, PNEUMATIC_OUT };

struct MyDeviceInfo {
    DeviceType type;

    pros::Motor* motor = nullptr;

    pros::adi::Port* pneumatics = nullptr;
    PneumaticType pneumatic_type = PneumaticType::NONE;
};

class MyDevice {
    public:
        std::string name;

        MyDeviceInfo info;

        MyDevice(
            std::string device_name
            , DeviceType device_type
            , std::int8_t* motor_port = nullptr
            , std::uint8_t* pneumatic_port = nullptr
        );
};

class MyMotor {
    public:
        std::string name;
        pros::Motor motor;

        double temp;

        // funcs

        MyMotor(std::string name, std::int8_t motor_port);

        void update_temp();
};

class MyPneumatic {
    public:
        std::string name;
        pros::adi::Port pneumatic;

        // funcs

        MyPneumatic(std::string name, std::uint8_t pneumatic_port);
};

struct Info {
    std::vector<MyMotor>* motors;
    std::vector<lv_obj_t*>* motor_boxes;
};

extern rd_view_t* motor_temps;
extern std::vector<MyDevice> devices;
extern std::vector<lv_obj_t*> device_boxes;
extern lv_obj_t* main_label;

void motor_temp_initialize();