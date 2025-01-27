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

class MyMotor {
    public:
        std::string name;
        pros::Motor motor;

        double temp;

        // funcs

        MyMotor(std::string name, std::int8_t motor_port);

        void update_temp();
};

struct Info {
    std::vector<MyMotor>* motors;
    std::vector<lv_obj_t*>* motor_boxes;
};

extern rd_view_t* motor_temps;
extern std::vector<MyMotor> motors;
extern std::vector<lv_obj_t*> motor_boxes;
extern lv_obj_t* main_label;

void motor_temp_initialize();