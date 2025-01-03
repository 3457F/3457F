// #pragma once

// #include <vector>

// // #include "robodash/api.h"

// namespace selector {
//     class Selector {
//         public:
//             enum AUTON_SLOTS {
//                 // red positive
//                 E_RED_POS_1 = 1,
//                 E_RED_POS_2 = 2,
//                 E_RED_POS_3 = 3,
//                 E_RED_POS_4 = 4,

//                 // red negative
//                 E_RED_NEG_1 = 5,
//                 E_RED_NEG_2 = 6,
//                 E_RED_NEG_3 = 7,
//                 E_RED_NEG_4 = 8,

//                 // blue positive
//                 E_BLUE_POS_1 = 9,
//                 E_BLUE_POS_2 = 10,
//                 E_BLUE_POS_3 = 11,
//                 E_BLUE_POS_4 = 12,

//                 // blue negative
//                 E_BLUE_NEG_1 = 13,
//                 E_BLUE_NEG_2 = 14,
//                 E_BLUE_NEG_3 = 15,
//                 E_BLUE_NEG_4 = 16,

//                 // skills
//                 E_SKILLS_1 = 17,
//                 E_SKILLS_2 = 18,
//                 E_SKILLS_3 = 19,
//                 E_SKILLS_4 = 20,
//             };

//             static bool isRed, isSkills, isDefault;

//             Selector(
//                 int defaultAuton
//                 , const char *slot1Name = "QUAL"
//                 , const char *slot2Name = "ELIM"
//                 , const char *slot3Name = "AWP"
//                 , const char *slot4Name = "NO_LADDER"
//             );

//             void initialize();

//             static int getAuton();

//         private:
//             static void initStyle(
//                 lv_style_t* style
//                 , lv_color_t bg_color
//                 , lv_color_t bg_grad_color
//                 , int radius
//                 , lv_color_t text_color
//                 , int border_width
//             );

//             // menu button funcs
//             static void autonMenu_btn_click_action(lv_event_t *event);
//             static void skillsMenu_btn_click_action(lv_event_t *event);

//             // button funcs -- normal autons
//             static void red_btn_click_action(lv_event_t *event);
//             static void blue_btn_click_action(lv_event_t *event);
//             static void pos_btn_click_action(lv_event_t *event);
//             static void neg_btn_click_action(lv_event_t *event);
//             static void opt1_btn_click_action(lv_event_t *event);
//             static void opt2_btn_click_action(lv_event_t *event);
//             static void opt3_btn_click_action(lv_event_t *event);
//             static void opt4_btn_click_action(lv_event_t *event);

//             // button funcs -- skills autons
//             static void skl1_btn_click_action(lv_event_t *event);
//             static void skl2_btn_click_action(lv_event_t *event);
//             static void skl3_btn_click_action(lv_event_t *event);
//             static void skl4_btn_click_action(lv_event_t *event);

//             // button funcs -- back buttons
//             static void back_btn_click_action1(lv_event_t *event);
//             static void back_btn_click_action2(lv_event_t *event);

//             static void menuSetup();

//             static void resetButtonHighlights();

//             int defaultAuton;

//             const char *slot1Name, *slot2Name, *slot3Name, *slot4Name;
//     };
// }; // namespace Selector