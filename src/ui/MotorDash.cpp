#include <format>

#include "MotorDash.hpp"
#include "consts.hpp"

rd_view_t* motor_temps = rd_view_create("MOTOR TEMPS");
static lv_style_t style_motor;

MyMotor::MyMotor(std::string name, std::int8_t motor_port) : motor(motor_port) {
    this->name = name;
    this->temp = 0;
}

std::vector<MyMotor> motors = {
    {"FRONT (L)", LEFT_FRONT_PORT}
    , {"FRONT (M)", LEFT_MID_PORT}
    , {"FRONT (B)", LEFT_BACK_PORT}

    , {"RIGHT (F)", RIGHT_FRONT_PORT}
    , {"RIGHT (M)", RIGHT_MID_PORT}
    , {"RIGHT (B)", RIGHT_BACK_PORT}

    , {"INTAKE", INTAKE_PORT}
};

std::vector<lv_obj_t*> motor_boxes = {};
// used to pass the index to lvgl, whose memory address will then be decoded and used to read the index
std::vector<int> motor_indexes = {};
lv_obj_t* motor_popup;
lv_obj_t* main_label = lv_label_create(rd_view_obj(motor_temps));

double c_to_f(double c) {
    return (c * 9/5) + 32;
}

void MyMotor::update_temp() {
    temp = motor.get_temperature();
}

void motor_temp_refresher(lv_timer_t* timer) {
    Info* motor_info = (Info*)(timer->user_data);

    auto motors_it = motor_info->motors->begin();
    auto motor_boxes_it = motor_info->motor_boxes->begin();

    while (motors_it != motors.end()) {
        (*motors_it).update_temp();

        // update temperature on motor text
        lv_label_set_text_fmt(*motor_boxes_it, "%s\n%s", (*motors_it).name.c_str(), (std::format("{:.2f}°F", c_to_f((*motors_it).temp))).c_str());

        // update color of the motor text
        if ((*motors_it).temp < 30.0) {
            // cool!

            lv_obj_set_style_text_color(*motor_boxes_it, lv_color_hex(GREEN), LV_PART_MAIN);
        } else if ((*motors_it).temp > 30.0) {
            if ((*motors_it).temp < 45.0) {
                lv_obj_set_style_text_color(*motor_boxes_it, lv_color_hex(ORANGE), LV_PART_MAIN);
            } else {
                lv_obj_set_style_text_color(*motor_boxes_it, lv_color_hex(RED_COLOR), LV_PART_MAIN);
            }
        }

        printf("motor %s: %f | ", (*motors_it).name.c_str(), (*motors_it).temp);

        ++motors_it;
        ++motor_boxes_it;
    }

    printf("\n");
}

// // used to display specific motor's temperature on click, AS A POPUP!
// static void disp_motor_info(lv_event_t* e) {
//     const char *motor_idx = (char *)lv_event_get_user_data(e);

//     motor_popup = lv_msgbox_create(NULL, (motors[*motor_idx].name + " temp").c_str(), (std::format("{:.2f}°C", motors[*motor_idx].temp)).c_str(), NULL, true);

//     lv_obj_add_style(lv_msgbox_get_close_btn(motor_popup), &style_motor, LV_PART_MAIN);
//     lv_obj_add_style(motor_popup, &style_motor, LV_PART_MAIN);
//     lv_obj_set_style_text_font(motor_popup, &lv_font_montserrat_14, LV_PART_MAIN);
//     lv_obj_set_style_text_font(lv_msgbox_get_title(motor_popup), &lv_font_montserrat_14, LV_PART_MAIN);
//     lv_obj_set_style_text_color(motor_popup, lv_color_hex(LIGHT_GREEN), LV_PART_MAIN);
//     lv_obj_set_style_text_font(lv_msgbox_get_close_btn(motor_popup), &lv_font_montserrat_24, LV_PART_MAIN);
//     lv_obj_set_style_text_color(lv_msgbox_get_close_btn(motor_popup), lv_color_hex(LIGHT_GREEN), LV_PART_MAIN);
//     lv_obj_set_style_border_opa(lv_msgbox_get_close_btn(motor_popup), 0, LV_PART_MAIN);
//     lv_obj_set_style_border_opa(motor_popup, 255, LV_PART_MAIN);
//     lv_obj_align(motor_popup, LV_ALIGN_CENTER, 0, 0);
// }

// lv_event_cb_t dispMotorInfo = disp_motor_info;

lv_obj_t* title = lv_label_create(rd_view_obj(motor_temps));


// "tempcheck()"
void motor_temp_initialize() {
    // sets up style for the motor boxes
    lv_style_init(&style_motor);
    lv_style_set_border_color(&style_motor, lv_color_hex(LIGHT_GREEN));
    lv_style_set_border_width(&style_motor, 2);
    lv_style_set_radius(&style_motor, 8);
    lv_style_set_text_color(&style_motor, lv_color_hex(BLACK));
    lv_style_set_bg_color(&style_motor, lv_color_hex(LIGHT_GREEN));
    lv_style_set_bg_opa(&style_motor, 0);
    // TODO: make 11pt font
    lv_style_set_text_font(&style_motor, &lv_font_montserrat_10);
    lv_style_set_text_align(&style_motor, LV_TEXT_ALIGN_CENTER);
    lv_style_set_pad_ver(&style_motor, 6);

    int motor_row = 0;
    char vector_probe = 0;

    for (int j = 0; j < motors.size(); j++) motor_indexes.push_back(j);

    for (int b = 0; b < motors.size(); b++) {
        lv_obj_t* motor_view = lv_label_create(rd_view_obj(motor_temps));

        lv_obj_add_style(motor_view, &style_motor, LV_PART_MAIN);
        lv_obj_add_flag(motor_view, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_size(motor_view, MB_WIDTH, MB_HEIGHT);
        lv_label_set_long_mode(motor_view, LV_LABEL_LONG_SCROLL);
        lv_label_set_text_fmt(motor_view, "%s\n%s", motors[b].name.c_str(), (std::format("{:.2f}°C", motors[b].temp)).c_str());
        lv_obj_set_style_transform_width(motor_view, 2, LV_STATE_PRESSED);
        lv_obj_set_style_transform_height(motor_view, 2, LV_STATE_PRESSED);

        // lv_obj_add_event_cb(motor_view, dispMotorInfo, LV_EVENT_CLICKED, &motor_indexes[b]);

        // only drawing four per row!
        if (b != 0 && b % 4 == 0) motor_row++;

        lv_obj_set_pos(
            motor_view

            // x
            , (MB_ROW_START_X + ((b % 4) * MB_CELL_W))
            , (MB_ROW_START_Y + ((motor_row) * MB_ROW_HEIGHT))
        );

        motor_boxes.push_back(motor_view);
    }

    // no image setting

    lv_obj_add_style(main_label, &style_motor, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_label, 6, LV_PART_MAIN);
    lv_obj_set_style_text_color(main_label, lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(main_label, &lv_font_montserrat_36, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(main_label, 255, LV_PART_MAIN);
    lv_obj_align(main_label, LV_ALIGN_TOP_MID, 0, 6);
    lv_label_set_text(main_label, "Motor Temperatures");

    Info* motor_info = new Info(&motors, &motor_boxes);
    lv_timer_t* motor_temp_timer = lv_timer_create(motor_temp_refresher, 500, motor_info);
}