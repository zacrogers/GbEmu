#pragma once

#include "state.hh"
#include "../controls.hh"
#include "../display.hh"

#include <lvgl.h>

class MenuState: public StateBase
{
private:
    enum Option { START_GAME = 0, SHUT_DOWN, NUM_OPTIONS, NONE };

public:
    MenuState();
    ~MenuState() override;

    void draw           () override;
    void show           () override;

private:
/* Member Functions */
void draw_main_screen();

    /* Button handlers */
    void             handle_a_button     () override;
    void             handle_b_button     () override;
    void             handle_up_button    () override;
    void             handle_down_button  () override;
    void             handle_left_button  () override;
    void             handle_right_button () override;

    /* Option processing */
    void             incr_option         ();
    void             decr_option         ();
    void             process_option      (const Option option);

    /* Menu option processes */
    void             start_game          ();
    void             shut_down           ();

    Option current_option     { Option::START_GAME };
    bool   option_is_selected { false };

    const char* menu_option_labels[NUM_OPTIONS] = {"Start Game", "Shut Down"};

    uint8_t current_row = 0;
    lv_obj_t* main_screen;
    lv_obj_t* ui_Start_Game_Button;
    lv_obj_t* ui_Start_Game_Label;
};

