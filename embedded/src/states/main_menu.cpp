#include "../../inc/states/main_menu.hh"

// #include "ui.h"
#include <lvgl.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(menu, LOG_LEVEL_DBG);

/*
    Menu State
*/
MenuState::MenuState()
{
    lv_init();
lv_obj_t* ui_Main_Menu_Scrren = lv_obj_create(lv_scr_act());
lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
lv_obj_set_scrollbar_mode(ui_Main_Menu_Scrren, LV_SCROLLBAR_MODE_OFF);
lv_obj_set_style_bg_color(ui_Main_Menu_Scrren, lv_color_hex(0x272846), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Main_Menu_Scrren, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_align( ui_Main_Menu_Scrren, LV_ALIGN_CENTER );
lv_obj_set_width( ui_Main_Menu_Scrren, 160);
lv_obj_set_height( ui_Main_Menu_Scrren, 128);
lv_obj_set_style_radius(ui_Main_Menu_Scrren, 0, LV_PART_MAIN);
lv_obj_set_style_border_width(ui_Main_Menu_Scrren, 0, LV_PART_MAIN);
// lv_style_t main_style;
// lv_style_init(&main_style);
// lv_style_set_border_width(&main_style, 0);
// lv_obj_add_style(ui_Main_Menu_Scrren, &main_style, LV_PART_MAIN);
// lv_obj_t* ui_Game_Select_Dropdown = lv_dropdown_create(ui_Main_Menu_Scrren);
// lv_dropdown_set_options( ui_Game_Select_Dropdown, "Pong\nSnake\nGameboy" );
// lv_obj_set_width( ui_Game_Select_Dropdown, 105);
// lv_obj_set_height( ui_Game_Select_Dropdown, LV_SIZE_CONTENT);   /// 1
// lv_obj_set_x( ui_Game_Select_Dropdown, 2 );
// lv_obj_set_y( ui_Game_Select_Dropdown, 34 );
// lv_obj_set_align( ui_Game_Select_Dropdown, LV_ALIGN_CENTER );
// lv_obj_add_flag( ui_Game_Select_Dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
// lv_obj_set_scrollbar_mode(ui_Game_Select_Dropdown, LV_SCROLLBAR_MODE_OFF);
// lv_obj_set_style_text_font(ui_Game_Select_Dropdown, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t* ui_Start_Game_Button = lv_btn_create(ui_Main_Menu_Scrren);
lv_obj_set_scrollbar_mode(ui_Start_Game_Button, LV_SCROLLBAR_MODE_OFF);
lv_obj_set_width( ui_Start_Game_Button, 115);
lv_obj_set_height( ui_Start_Game_Button, 52);
lv_obj_set_x( ui_Start_Game_Button, 1 );
lv_obj_set_y( ui_Start_Game_Button, -22 );
lv_obj_set_align( ui_Start_Game_Button, LV_ALIGN_CENTER );
// lv_obj_add_flag( ui_Start_Game_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
// lv_obj_clear_flag( ui_Start_Game_Button, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Start_Game_Button, lv_color_hex(0x130059), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Start_Game_Button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_Start_Game_Button, lv_color_hex(0x1200FF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_Start_Game_Button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Start_Game_Button, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_Start_Game_Button, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_t* ui_Start_Game_Label = lv_label_create(ui_Start_Game_Button);
lv_obj_set_scrollbar_mode(ui_Start_Game_Label, LV_SCROLLBAR_MODE_OFF);
lv_obj_set_width( ui_Start_Game_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Start_Game_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Start_Game_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_Start_Game_Label,"Start");
lv_obj_set_style_text_font(ui_Start_Game_Label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
}


MenuState::~MenuState()
{

}


void  MenuState::draw()
{
	lv_task_handler();
}

/*
    Button handlers
*/
void MenuState::handle_a_button()
{
    set_current_state(StateBase::State::READY_TO_CLOSE);
    LOG_ERR("A pressed");
    if(option_is_selected)
    {
        process_option(current_option);
    }
    else
    {
        option_is_selected = true;
    }
}


void MenuState::handle_b_button()
{
    option_is_selected = false;
}


void MenuState::handle_up_button()
{
    incr_option();
}


void MenuState::handle_down_button()
{
    decr_option();
}


void MenuState::handle_left_button()
{

}


void MenuState::handle_right_button()
{

}


/*
    Menu option processing
*/
void MenuState::incr_option()
{
    uint8_t op_int = (uint8_t)current_option;

    if(op_int > 0)
    {
        op_int--;
    }
    else
    {
        op_int = (NUM_OPTIONS - 1);
    }

    current_option = (MenuState::Option)op_int;
}


void MenuState::decr_option()
{
    // if(current_option < NUM_OPTIONS) current_option++;
}


void MenuState::process_option(const MenuState::Option option)
{
    switch(option)
    {
        case MenuState::Option::START_GAME: start_game(); break;
        case MenuState::Option::SHUT_DOWN:  shut_down(); break;
        default: break;
    }
}


void MenuState::start_game()
{

}


void MenuState::shut_down()
{

}
