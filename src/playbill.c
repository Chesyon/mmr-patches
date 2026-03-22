#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

#if CUSTOM_SCRIPT_MENUS

#define SCENE_SELECTOR_ADV_MENU GLOBAL_MENU_INFO.window_ids[0]
#define SCENE_SELECTOR_PARTICIPANT_LISTING GLOBAL_MENU_INFO.window_ids[1]
#define SCENE_SELECTOR_TOP_CHART GLOBAL_MENU_INFO.window_ids[2]
#define SCENE_SELECTOR_YESNO_MENU GLOBAL_MENU_INFO.window_ids[3]

SECTION_DATA_PLAYBILL struct preprocessor_flags PREPROC_INSTANT_NO_INPUT = {.flags_1 = 0b000000010, .timer_2 = true}; // Instant text without waiting for any input!
SECTION_DATA_PLAYBILL char SCENE_FORMATTER[] = "[M:S3] Scene %d";
SECTION_DATA_PLAYBILL char SCENE_INITIAL[] = "[M:S3] Initial";

SECTION_TEXT_PLAYBILL char* SceneOptionEntryFn(char* buffer, int option_id) {
    if(option_id != 0)
        sprintf(buffer, SCENE_FORMATTER, option_id);
    else
        return SCENE_INITIAL;
    return buffer;
}

SECTION_TEXT_PLAYBILL void CreateCustomControlsChart(int text_string_id) {
    struct window_params topchart_params = { .x_offset = 0x3, .y_offset = 0x9, .width = 0x1A, .height = 0x9, .screen = {SCREEN_SUB}, .box_type = {0xFA} };
    struct window_flags topchart_flags;
    SCENE_SELECTOR_TOP_CHART = CreateControlsChart(&topchart_params, topchart_flags, NULL, text_string_id);
}

SECTION_TEXT_PLAYBILL void CreatePlaybill(void) {
    int option_id = last_selected_scene >= 0 && last_selected_scene < TOTAL_SCENES ? last_selected_scene : 0;
    struct window_params menu_params = { .x_offset = 2, .y_offset = 3, .box_type = {0xFC} };
    struct window_params dbox_params = { .x_offset = 0x9, .y_offset = 0x12, .width = 0x15, .height = 0x4, .box_type = {0xFC} };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = false, .se_on = true, .set_choice = true, .partial_menu = true, .menu_title = true, .menu_lower_bar = true, .no_accept_button = false };
    struct window_extra_info menu_info = {.set_choice_id = option_id, .title_string_id = TEXT_STRING_SCENE_SELECTOR_TITLE, .title_height = 0x10 };
    int menu_options = TOTAL_SCENES; int menu_options_pp = 5;
    TEXTBOX_COLOR_ATTRIBUTES.r = 0xAA;
    TEXTBOX_COLOR_ATTRIBUTES.g = 0x34;
    TEXTBOX_COLOR_ATTRIBUTES.b = 0xA8;
    TextboxTransparent();
    SetBothScreensWindowsColor(3);
    SCENE_SELECTOR_ADV_MENU = CreateAdvancedMenu(&menu_params, menu_flags, &menu_info, SceneOptionEntryFn, menu_options, menu_options_pp);
    SCENE_SELECTOR_PARTICIPANT_LISTING = CreateDialogueBox(&dbox_params);
    CreateCustomControlsChart(TEXT_STRING_MYSTERYMAIL_BLURB);
    ShowStringIdInDialogueBox(SCENE_SELECTOR_PARTICIPANT_LISTING, PREPROC_INSTANT_NO_INPUT, TEXT_STRING_PARTICIPANT_NAME_START+option_id, NULL);
    GLOBAL_MENU_INFO.previous_option = option_id;
}

SECTION_TEXT_PLAYBILL void CreateYesNoSubMenu(void) {
    struct window_params menu_params = { .x_offset = 0x14, .y_offset = 3, .width = 0xA, .box_type = {0xFC} };
    struct window_flags menu_flags = { .a_accept = true, .b_cancel = true, .se_on = true, .menu_title = true, .menu_lower_bar = true, .no_accept_button = true};
    struct window_extra_info menu_info = {.title_height = 0x10, .title_string_id = TEXT_STRING_PLAY_SCENE_QUESTION};
    struct simple_menu_id_item simple_options[3];
    for(int i = 0; i < 2; i++) {
        simple_options[i].string_id = i+422;
        simple_options[i]._padding = 0;
        simple_options[i].result_value = i+1;
    }
    simple_options[2].string_id = NULL;
    simple_options[2]._padding = NULL;
    simple_options[2].result_value = NULL;
    SCENE_SELECTOR_YESNO_MENU = CreateSimpleMenuFromStringIds(&menu_params, menu_flags, &menu_info, simple_options, 3);
}

SECTION_TEXT_PLAYBILL void ClosePlaybill(void) {
    if(SCENE_SELECTOR_ADV_MENU >= 0)
        CloseAdvancedMenu(SCENE_SELECTOR_ADV_MENU);
    if(SCENE_SELECTOR_PARTICIPANT_LISTING >= 0)
        CloseDialogueBox(SCENE_SELECTOR_PARTICIPANT_LISTING);
    if(SCENE_SELECTOR_TOP_CHART >= 0)
        CloseControlsChart(SCENE_SELECTOR_TOP_CHART);
    if(SCENE_SELECTOR_YESNO_MENU >= 0)
        CloseSimpleMenu(SCENE_SELECTOR_YESNO_MENU);
    TextboxSolid();
    SpResetTextboxColor();
    SetBothScreensWindowsColor(2);
}

SECTION_TEXT_PLAYBILL bool UpdateSceneSelectorMenu(void) {
    int main_menu_id = SCENE_SELECTOR_ADV_MENU;
    int sub_menu_id = SCENE_SELECTOR_YESNO_MENU;
    int result;
    int current_menu_option;
    switch(GLOBAL_MENU_INFO.state) {
        case 0:;
            // Initial state; check for main scene select menu activity...
            if(!IsAdvancedMenuActive2(main_menu_id)) {
                result = GetAdvancedMenuResult(main_menu_id);
                GLOBAL_MENU_INFO.menu_results[0] = result;
                if(result >= 0)
                    GLOBAL_MENU_INFO.state++;
                else {
                    GLOBAL_MENU_INFO.return_val = -1;
                    return true;
                }
            }
            // If active, change the bottom window to show participants' names!
            else {
                current_menu_option = GetAdvancedMenuCurrentOption(main_menu_id);
                if(current_menu_option != GLOBAL_MENU_INFO.previous_option) {
                    ShowStringIdInDialogueBox(SCENE_SELECTOR_PARTICIPANT_LISTING, PREPROC_INSTANT_NO_INPUT, TEXT_STRING_PARTICIPANT_NAME_START+current_menu_option, NULL);
                    GLOBAL_MENU_INFO.previous_option = current_menu_option;
                }
            }
            break;
        case 1:;
            // smol state, just create the yes/no submenu and go on
            CreateYesNoSubMenu();
            GLOBAL_MENU_INFO.state++;
            break;
        case 2:;
            // Yes/no submenu activity check...
            if(!IsSimpleMenuActive(sub_menu_id)) {
                result = GetSimpleMenuResult(sub_menu_id);
                CloseSimpleMenu(sub_menu_id);
                SCENE_SELECTOR_YESNO_MENU = -1;
                if(result == 1) {
                    // If "yes" pressed, save the result and quit
                    result = GLOBAL_MENU_INFO.menu_results[0];
                    last_selected_scene = result;
                    GLOBAL_MENU_INFO.return_val = result;
                    return true;
                }
                else {
                    // If "no" pressed or exited otherwise, go back to the initial state
                    ResumeAdvancedMenu(main_menu_id);
                    GLOBAL_MENU_INFO.state = 0;
                }
            }
            break;
            
    }
    return false;
}

#endif
