#include "../../inc/states/main_menu.hh"

// #include "ui.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(menu, LOG_LEVEL_DBG);

/*
    Menu State
*/
MenuState::MenuState()
{
    main_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x272846), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(main_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_align( main_screen, LV_ALIGN_CENTER );
    lv_obj_set_width( main_screen, 160);
    lv_obj_set_height( main_screen, 128);
    lv_obj_set_style_radius(main_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(main_screen, 0, LV_PART_MAIN);

    start_game_button = lv_btn_create(main_screen);
    lv_obj_set_scrollbar_mode(start_game_button, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( start_game_button, 115);
    lv_obj_set_height( start_game_button, 52);
    lv_obj_set_x( start_game_button, 1 );
    lv_obj_set_y( start_game_button, -22 );
    lv_obj_set_align( start_game_button, LV_ALIGN_CENTER );
    lv_obj_set_style_bg_color(start_game_button, lv_color_hex(0x130059), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(start_game_button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(start_game_button, lv_color_hex(0x1200FF), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(start_game_button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(start_game_button, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(start_game_button, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);

    start_game_label = lv_label_create(start_game_button);
    lv_obj_set_scrollbar_mode(start_game_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( start_game_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( start_game_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( start_game_label, LV_ALIGN_CENTER );
    lv_label_set_text(start_game_label,"Start");
    lv_obj_set_style_text_font(start_game_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    game_select_dropdown = lv_dropdown_create(main_screen);
    lv_dropdown_set_options( game_select_dropdown, "Pong\nSnake\nGameboy" );
    lv_obj_set_width( game_select_dropdown, 105);
    lv_obj_set_height( game_select_dropdown, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( game_select_dropdown, 2 );
    lv_obj_set_y( game_select_dropdown, 34 );
    lv_obj_set_align( game_select_dropdown, LV_ALIGN_CENTER );
    lv_obj_add_flag( game_select_dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
    lv_obj_set_style_text_font(game_select_dropdown, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
}


MenuState::~MenuState()
{
    lv_obj_clean(main_screen);
    lv_obj_clean(start_game_button);
    lv_obj_clean(start_game_label);
}


void MenuState::show()
{
    set_current_state(StateBase::State::RUNNING);
    lv_scr_load(main_screen);
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
    if(game_select_dropdown_open())
    {
        uint32_t t = LV_KEY_ENTER;
        lv_event_send(game_select_dropdown, LV_EVENT_KEY, &t);
    }
    else
    {
        // handle sending selected game to engine
        set_current_state(StateBase::State::READY_TO_CLOSE);
    }
}


bool MenuState::game_select_dropdown_open()
{
    return lv_dropdown_is_open(game_select_dropdown);
}


void MenuState::handle_b_button()
{
    if(game_select_dropdown_open())
    {
        lv_dropdown_close(game_select_dropdown);
    }
    else
    {
        lv_dropdown_open(game_select_dropdown);
    }
}


void MenuState::handle_up_button()
{
    uint32_t t = LV_KEY_UP;
    lv_event_send(game_select_dropdown, LV_EVENT_KEY, &t);
}


void MenuState::handle_down_button()
{
    uint32_t t = LV_KEY_DOWN;
    lv_event_send(game_select_dropdown, LV_EVENT_KEY, &t);
}


void MenuState::handle_left_button()
{

}


void MenuState::handle_right_button()
{

}


void MenuState::start_game()
{

}


void MenuState::shut_down()
{

}
