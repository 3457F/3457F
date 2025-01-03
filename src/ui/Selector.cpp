#include "Selector.hpp"

#define FULLY_VIS 255

/** EXTERNS */
bool no_selection;
int scr_page = 0;
AutonTable auton_table;

// TODO: wtf is this
int j = 0;
int selected = 0;
/** "0" as per Jordan is RED; "1" as per Jordan is BLUE */
Alliance red_blu_store = Alliance::RED;
/** "0" as per Jordan is NEGATIVE; "1" as per Jordan is POSITIVE */
Corner pos_neg_store = Corner::NEGATIVE;
std::vector<Auton> curated_autons = {};
bool page_side[2]{true, false};

/**
 * styles that will be applied to all the
 * lvgl objects
 */
static lv_style_t style_bg;
static lv_style_t style_btn;
static lv_style_t style_ind;
static lv_style_t style_switch;

lv_obj_t* main_screen = lv_tileview_create(NULL);
lv_obj_t* auton_selector = lv_tileview_add_tile(
    main_screen
    , 0
    , 0
    , LV_DIR_NONE                                       // no predefined layout
);
lv_obj_t* motor_temps = lv_tileview_add_tile(
    main_screen
    , 1
    , 0
    , LV_DIR_NONE
);
// skipping auto builder and man builder
lv_obj_t* overlay = lv_img_create(auton_selector);
lv_obj_t* auton_select_up = lv_btn_create(auton_selector);
lv_obj_t* auton_select_down = lv_btn_create(auton_selector);
lv_obj_t* red_blu = lv_switch_create(auton_selector);
lv_obj_t* pos_neg = lv_switch_create(auton_selector);
lv_obj_t* neg_ind = lv_obj_create(auton_selector);
lv_obj_t* pos_ind = lv_obj_create(auton_selector);
lv_obj_t* red_blu_ind = lv_obj_create(auton_selector);
lv_obj_t* auton_table_lv = lv_table_create(auton_selector);
lv_obj_t* selected_auton = lv_label_create(auton_selector);
lv_obj_t* selected_auton_box = lv_obj_create(auton_selector);
lv_obj_t* auton_desc = lv_label_create(auton_selector);
lv_obj_t* mogo_ring_1 = lv_obj_create(auton_selector);
lv_obj_t* mogo_ring_2 = lv_obj_create(auton_selector);
lv_obj_t* mogo_ring_back_1 = lv_obj_create(auton_selector);
lv_obj_t* mogo_ring_back_2 = lv_obj_create(auton_selector);
lv_obj_t* alliance_ring = lv_obj_create(auton_selector);
lv_obj_t* ring_ind = lv_obj_create(auton_selector);
lv_obj_t* page_switch;

LV_IMG_DECLARE(brainuioverlay);

// skipping overlay definition

std::vector<lv_color_t> color_table = {
    COLOR_RED
    , COLOR_BLUE
    , GRAY
    , CYAN
    , ORANGE
    , GRAY
    , NEON_GREEN
};

void auton_run() {
    autons[selected].autonCall();
}

/**
 * resets all indicators and switches
 */
static void clear() {
    no_selection = true;

    lv_obj_set_style_bg_color(
        red_blu_ind
        , GRAY
        , LV_PART_MAIN          // means that is the MAIN PART of the lvgl object
    );
    lv_obj_set_style_bg_color(
        pos_ind
        , GRAY
        , LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        neg_ind
        , GRAY
        , LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        mogo_ring_1
        , GRAY
        , LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        mogo_ring_2
        , GRAY
        , LV_PART_MAIN
    );
    lv_label_set_text(selected_auton, "No Auton Selected");
    lv_obj_clear_flag(alliance_ring, LV_OBJ_FLAG_HIDDEN);
}

/**
 * prints autons to LVGL screen based on how many autons there are for that corner + alliance
 */
static void list_update() {
    const char* first_name = j < autons.size() ? autons[j].name.c_str() : "";
    const char* second_name = j + 1 < autons.size() ? autons[j + 1].name.c_str() : "";
    const char* third_name = j + 2 < autons.size() ? autons[j + 2].name.c_str() : "";
    const char* fourth_name = j + 3 < autons.size() ? autons[j + 3].name.c_str() : "";

    lv_table_set_cell_value(auton_table_lv, 0, 0, first_name);
    lv_table_set_cell_value(auton_table_lv, 1, 0, second_name);
    lv_table_set_cell_value(auton_table_lv, 2, 0, third_name);
    lv_table_set_cell_value(auton_table_lv, 3, 0, fourth_name);
}

static void auton_curate(lv_event_t* e) {
    autons.clear();
    j = 0;
    lv_obj_has_state(red_blu, LV_STATE_CHECKED) == true ? red_blu_store = Alliance::BLUE : red_blu_store = Alliance::RED;
    lv_obj_has_state(pos_neg, LV_STATE_CHECKED) == true ? pos_neg_store = Corner::POSITIVE : pos_neg_store = Corner::NEGATIVE;

    for (auto auton : autons) {
        if (auton.alliance == red_blu_store || auton.alliance == Alliance::ALLIANCE_NONE) {
            if (auton.corner == pos_neg_store || auton.corner == Corner::CORNER_NONE) {
                curated_autons.push_back(auton);
            }
        }
    }
    list_update();
}

static void select_auton(lv_event_t* e) {
    uint16_t row, col;
    lv_table_get_selected_cell(auton_table_lv, &row, &col);
    
    if (row == 0) selected = j;
    if (row == 1) selected = j + 1;
    if (row == 2) selected = j + 2;
    if (row == 3) selected = j + 3;

    clear();

    // if an actual auton is selected
    if (strcmp(lv_table_get_cell_value(auton_table_lv, row, 0), "") != 0) {
        no_selection = false;

        lv_color_t red_blu_color = lv_obj_has_state(
            red_blu
            , LV_STATE_CHECKED
        ) == true ? COLOR_BLUE : COLOR_RED;

        lv_obj_set_style_bg_color(red_blu_ind, red_blu_color, LV_PART_MAIN);

        // sets positive and negative indicators accordingly
        lv_obj_has_state(
            pos_neg
            , LV_STATE_CHECKED
        )   
            ? lv_obj_set_style_bg_color(pos_ind, ORANGE, LV_PART_MAIN)
            : lv_obj_set_style_bg_color(neg_ind, CYAN, LV_PART_MAIN);

        color_table[2] = lv_obj_get_style_bg_color(red_blu_ind, LV_PART_MAIN);
        lv_label_set_text(selected_auton, autons[selected].desc.c_str());
        
        lv_obj_set_style_bg_color(mogo_ring_1, color_table[autons[selected].alliance], LV_PART_MAIN);
        lv_obj_set_size(mogo_ring_back_1, 46, (78 - (13 * curated_autons[selected].mogo1)));
        
        lv_obj_set_style_bg_color(mogo_ring_2, color_table[autons[selected].alliance], LV_PART_MAIN);
        lv_obj_set_size(mogo_ring_back_2, 46, (78 - (13 * curated_autons[selected].mogo2)));

        // TODO: huh...???
        curated_autons[selected].alliance_ring == true
            ? lv_obj_add_flag(alliance_ring, LV_OBJ_FLAG_HIDDEN)
            : lv_obj_clear_flag(alliance_ring, LV_OBJ_FLAG_HIDDEN);
    }
}

static void up_down_btn(lv_event_t* e) {
    if (lv_obj_has_state(auton_select_up, LV_EVENT_CLICKED)) {
        if (curated_autons.size() % 4 == 0 && j < 4) {
            j = (curated_autons.size() - 4);
        } else if (j < 4) {
            j = (autons.size() - (autons.size() % 4));
        } else {
            j -= 4;
        }
    } else if (lv_obj_has_state(auton_select_down, LV_EVENT_CLICKED)) {
        j += 4;
        if (j >= autons.size()) j = 0;
    }

    list_update();
}

lv_obj_t* screens[2] = {auton_selector, motor_temps};

static void page_switch_btn(lv_event_t* e) {
    const bool *getside = (bool*)lv_event_get_user_data(e);
    scr_page = *getside ? (scr_page + 1) % 2 : scr_page == 0 ? 1 : (scr_page - 1) % 2;
    lv_obj_set_tile(main_screen, screens[scr_page], LV_ANIM_ON);
    lv_obj_set_parent(page_switch, screens[scr_page]);
}

lv_event_cb_t autonCurate = auton_curate;
lv_event_cb_t selectAuton = select_auton;
lv_event_cb_t upDownBtn = up_down_btn;
lv_event_cb_t pageSwitchBtn = page_switch_btn;

// TODO: group inits by object
void screen_init() {
    /**
     * Sets up background style
     */

    lv_style_init(&style_bg);
    lv_style_set_border_width(&style_bg, 0);
    lv_style_set_radius(&style_bg, 0);
    // sort of a light green...?
    lv_style_set_text_color(&style_bg, LIGHT_GREEN);
    // very dark green
    lv_style_set_bg_color(&style_bg, DARK_GREEN);

    /**
     * Sets up button style
     */
    lv_style_init(&style_btn);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_radius(&style_btn, 6);
    lv_style_set_border_color(&style_btn, DARK_GREEN);
    lv_style_set_text_color(&style_btn, DARK_GREEN);
    lv_style_set_bg_color(&style_btn, LIGHT_GREEN);
    lv_style_set_text_align(&style_btn, LV_TEXT_ALIGN_CENTER);
    lv_style_set_bg_opa(&style_btn, FULLY_VIS);

    /**
     * Sets up indicator style
     */
    lv_style_init(&style_ind);
    lv_style_set_border_width(&style_ind, 0);
    lv_style_set_radius(&style_ind, 0);
    lv_style_set_bg_color(&style_ind, GRAY);

    /**
     * Sets up switch style
     */
    lv_style_init(&style_switch);
    lv_style_set_border_width(&style_switch, 3);
    lv_style_set_outline_width(&style_switch, 2);
    lv_style_set_radius(&style_switch, 16);
    lv_style_set_border_color(&style_switch, DARK_GREEN);
    lv_style_set_outline_color(&style_switch, LIGHT_GREEN);

    lv_img_set_src(overlay, &brainuioverlay); // "S:brainuioverlay.bin"
    lv_obj_move_foreground(overlay);

    lv_scr_load(main_screen);
    lv_obj_add_style(main_screen, &style_bg, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
    
    lv_obj_set_size(auton_select_up, 11, 58);
    lv_obj_set_pos(auton_select_up, 242, 50);
    lv_obj_add_style(auton_select_up, &style_btn, LV_PART_MAIN);
    
    lv_obj_set_size(auton_select_down, 11, 58);
    lv_obj_set_pos(auton_select_down, 242, 118);
    lv_obj_add_style(auton_select_down, &style_btn, LV_PART_MAIN);

    lv_obj_set_size(ring_ind, 117, 177);
    lv_obj_set_pos(ring_ind, 345, 3);

    lv_obj_set_size(mogo_ring_1, 46, 78);
    lv_obj_set_size(mogo_ring_2, 46, 78);
    lv_obj_set_size(mogo_ring_back_1, 46, 78);
    lv_obj_set_size(mogo_ring_back_2, 46, 78);

    lv_obj_set_size(alliance_ring, 66, 14);
    lv_obj_set_pos(mogo_ring_1, 260, 149);
    lv_obj_set_pos(mogo_ring_2, 341, 149);
    lv_obj_set_pos(mogo_ring_back_1, 260, 149);
    lv_obj_set_pos(mogo_ring_back_2, 341, 149);
    lv_obj_set_pos(alliance_ring, 267, 72);

    lv_obj_move_background(mogo_ring_1);
    lv_obj_move_background(mogo_ring_2);

    lv_obj_add_style(ring_ind, &style_ind, LV_PART_MAIN);
    lv_obj_add_style(mogo_ring_1, &style_ind, LV_PART_MAIN);
    lv_obj_add_style(mogo_ring_2, &style_ind, LV_PART_MAIN);
    lv_obj_add_style(mogo_ring_back_1, &style_ind, LV_PART_MAIN);
    lv_obj_add_style(mogo_ring_back_2, &style_ind, LV_PART_MAIN);
    lv_obj_add_style(alliance_ring, &style_ind, LV_PART_MAIN);

    lv_obj_set_scrollbar_mode(mogo_ring_back_1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(mogo_ring_back_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(alliance_ring, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_pad_top(auton_table_lv, 8, LV_PART_ITEMS);
    lv_obj_set_style_pad_bottom(auton_table_lv, 6, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(auton_table_lv, 4, LV_PART_ITEMS);
    
    lv_obj_add_style(auton_table_lv, &style_bg, LV_PART_MAIN);
    lv_obj_add_style(auton_table_lv, &style_btn, LV_PART_ITEMS);

    lv_table_set_col_width(auton_table_lv, 0, 230);
    
    lv_obj_move_background(auton_table_lv);

    lv_obj_add_style(selected_auton, &style_bg, LV_PART_MAIN);
    lv_obj_set_pos(selected_auton, 5, 182);
    lv_obj_set_size(selected_auton, 230, 47);

    lv_obj_set_size(selected_auton_box, 230, 230);
    lv_obj_set_pos(selected_auton_box, 5, 5);
    lv_obj_move_background(selected_auton_box);

    lv_obj_set_style_pad_left(selected_auton, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_top(selected_auton, 8, LV_PART_MAIN);
    lv_label_set_text(selected_auton, "No Auton Selected");
    no_selection = true;

    lv_obj_set_size(auton_table_lv, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(auton_table_lv, 5, 60);

    lv_obj_move_foreground(auton_select_up);
    lv_obj_move_foreground(auton_select_down);

    lv_obj_add_style(red_blu, &style_switch, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(red_blu, &style_switch, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_style(red_blu, &style_switch, LV_PART_KNOB);
    lv_obj_set_style_pad_all(red_blu, 0, LV_PART_KNOB);
    lv_obj_set_style_pad_hor(red_blu, 2, LV_PART_KNOB);
    lv_obj_set_style_bg_color(red_blu, COLOR_RED, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(red_blu, COLOR_BLUE, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(red_blu, LIGHT_GREEN, LV_PART_KNOB);
    lv_obj_set_size(red_blu, 42, 20);
    lv_obj_set_pos(red_blu, 62, 10);
    lv_obj_move_foreground(red_blu);

    lv_obj_add_style(pos_neg, &style_switch, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(pos_neg, &style_switch, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_add_style(pos_neg, &style_switch, LV_PART_KNOB);
    lv_obj_set_style_pad_all(pos_neg, 0, LV_PART_KNOB);
    lv_obj_set_style_pad_hor(pos_neg, 2, LV_PART_KNOB);
    lv_obj_set_style_bg_color(pos_neg, CYAN, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(pos_neg, ORANGE, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(pos_neg, LIGHT_GREEN, LV_PART_KNOB);
    lv_obj_set_size(pos_neg, 42, 20);
    lv_obj_set_pos(pos_neg, 177, 10);
    lv_obj_move_foreground(pos_neg);

    lv_obj_set_size(neg_ind, 66, 30);
    lv_obj_set_pos(neg_ind, 412, 197);

    lv_obj_set_scrollbar_mode(neg_ind, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(neg_ind, &style_ind, LV_PART_MAIN);

    lv_obj_set_size(pos_ind, 66, 66);
    lv_obj_set_pos(pos_ind, 412, 126);
    lv_obj_add_style(pos_ind, &style_ind, LV_PART_MAIN);

    lv_obj_set_size(red_blu_ind, 62, 79);
    lv_obj_set_pos(red_blu_ind, 266, 22);
    lv_obj_add_style(red_blu_ind, &style_ind, LV_PART_MAIN);

    lv_obj_set_pos(auton_desc, 7, 45);
    lv_obj_add_style(auton_desc, &style_bg, LV_PART_MAIN);
    lv_label_set_text(auton_desc, "auton selection:");

    lv_obj_add_event_cb(red_blu, autonCurate, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(pos_neg, autonCurate, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(auton_select_up, upDownBtn, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(auton_select_down, upDownBtn, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(auton_table_lv, selectAuton, LV_EVENT_VALUE_CHANGED, NULL);
    
    // TODO: was "lv_event_send"...?
    lv_event_send(red_blu, LV_EVENT_CLICKED, NULL);
}