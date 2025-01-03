// #include "selector.hpp"

// int two = 2;
// int three = 3;

// /** our lovely robodash view :D */
// rd_view_t* selectorView;

// /** LVGL Objects (defining outside class bc lvgl dum) */
// static lv_obj_t *vertLine;
// static lv_obj_t *horzLine;

// static lv_obj_t *autonScreen;
// static lv_obj_t *redButton;
// static lv_obj_t *redButtonLabel;
// static lv_obj_t *blueButton;
// static lv_obj_t *blueButtonLabel;

// static lv_obj_t *sideScreen;
// static lv_obj_t *posButton;
// static lv_obj_t *posButtonLabel;
// static lv_obj_t *negButton;
// static lv_obj_t *negButtonLabel;

// static lv_obj_t *optionScreen;
// static lv_obj_t *option1Button;
// static lv_obj_t *option1ButtonLabel;
// static lv_obj_t *option2Button;
// static lv_obj_t *option2ButtonLabel;
// static lv_obj_t *option3Button;
// static lv_obj_t *option3ButtonLabel;
// static lv_obj_t *option4Button;
// static lv_obj_t *option4ButtonLabel;

// static lv_obj_t *skillsScreen;
// static lv_obj_t *skills1Button;
// static lv_obj_t *skills1ButtonLabel;
// static lv_obj_t *skills2Button;
// static lv_obj_t *skills2ButtonLabel;
// static lv_obj_t *skills3Button;
// static lv_obj_t *skills3ButtonLabel;
// static lv_obj_t *skills4Button;
// static lv_obj_t *skills4ButtonLabel;

// static lv_obj_t *backButton1;
// static lv_obj_t *backButtonLabel1;
// static lv_obj_t *backButton2;
// static lv_obj_t *backButtonLabel2;

// static lv_obj_t *exitButton;
// static lv_obj_t *exitButtonLabel;
// static lv_obj_t *autonsButton;
// static lv_obj_t *autonsButtonLabel;
// static lv_obj_t *skillsButton;
// static lv_obj_t *skillsButtonLabel;

// static lv_obj_t *saveScreen;
// static lv_obj_t *saveExit;
// static lv_obj_t *saveLabel;

// static lv_obj_t *startScreen;

// /** LVGL Styles (defining outside class bc lvgl dum) */
// static lv_style_t myButtonStyleREL;
// static lv_style_t myButtonStylePR;
// static lv_style_t redStyle;
// static lv_style_t blueStyle;
// static lv_style_t redBackStyle;
// static lv_style_t blueBackStyle;
// static lv_style_t blackStyle;
// static lv_style_t whiteStyle;
// static lv_style_t selectedStyle;
// static lv_style_t backStyle;
// static lv_style_t menuStyle1;
// static lv_style_t menuStyle2;
// static lv_style_t style_btn_rel;
// static lv_style_t style_btn_pr;
// static lv_style_t style_bg;
// static lv_style_t invisibleStyle;
// static lv_style_t saveDispStyle;
// static lv_style_t teamNumStyle;

// // config variables
// int autonSlot = 1;
// bool selector::Selector::isRed
//      , selector::Selector::isSkills = false;
// bool selector::Selector::isDefault = true;

// selector::Selector::Selector(
//     int defaultAuton
//     , const char *slot1Name
//     , const char *slot2Name
//     , const char *slot3Name
//     , const char *slot4Name
// ) {
//     this->defaultAuton = 1;
//     this->slot1Name = slot1Name;
//     this->slot2Name = slot2Name;
//     this->slot3Name = slot3Name;
//     this->slot4Name = slot4Name;    
// };

// void selector::Selector::resetButtonHighlights() {
//     std::vector<lv_obj_t*> btns = {
//         option1Button
//         , option2Button
//         , option3Button
//         , option4Button
//         , skills1Button
//         , skills2Button
//         , skills3Button
//         , skills4Button
//     };

//     for (auto& btn : btns) {
//         lv_obj_add_style(
//             btn
//             , &blackStyle
//             , LV_PART_MAIN
//         );
//     }
// }

// void selector::Selector::autonMenu_btn_click_action(lv_event_t *event) {
    
// }