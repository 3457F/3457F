#pragma once

#include <vector>

#include "liblvgl/lvgl.h"

#include "Auton.hpp"

// heavily inspired by: https://github.com/udderfool/pringle-21S/blob/main/src/ui/brainui.cpp

#define LIGHT_GREEN lv_color_hex(0xcfffe9)
#define DARK_GREEN lv_color_hex(0x071808)

#define COLOR_RED lv_color_hex(0xff2a00)
#define COLOR_BLUE lv_color_hex(0x0066cc)
#define GRAY lv_color_hex(0x5d5d5d)
#define CYAN lv_color_hex(0x00b5bc)
#define ORANGE lv_color_hex(0xdb8826)
#define NEON_GREEN lv_color_hex(0x84f03b)

void screen_init();
void auton_run();

extern lv_obj_t* main_screen;
extern lv_obj_t* auton_selector;
extern lv_obj_t* motor_temps;
// skipping automatic auto builder and manual auto builder
extern lv_obj_t* page_switch;
extern lv_obj_t* ring_ind;

extern std::vector<lv_color_t> color_table;
extern bool no_selection;
extern int scr_page;

extern AutonTable auton_table;
extern std::vector<Auton> autons;