#pragma once

#include "../state.hh"
#include "../../common.hh"
#include "../../subsystems/controls.hh"
#include "../../subsystems/display.hh"

#include <lvgl.h>

class MenuState: public StateBase
{
private:
    enum Option { START_GAME = 0, SHUT_DOWN, SELECT_GAME, NUM_OPTIONS, NONE };

public:
    MenuState();
    ~MenuState() override;

    void     draw           () override;
    void     show           () override;
    GameType selected_game  ();

private:
/* Member Functions */
    void             draw_main_screen    ();

    /* Button handlers */
    void             handle_a_button     () override;
    void             handle_b_button     () override;
    void             handle_up_button    () override;
    void             handle_down_button  () override;
    void             handle_left_button  () override;
    void             handle_right_button () override;

    /* Menu option processes */
    void             start_game                ();
    void             shut_down                 ();
    bool             game_select_dropdown_open ();

    // this needs to match GameType in common.hh BT_CONTROLLER, SERIAL_MONITOR, WIFI_MANAGER
    const char* game_dropdown_options { "Pong\nSnake\nGB Emu\nBT\nSerial\nWIFI" };
    Option      current_option        { Option::START_GAME };
    bool        option_is_selected    { false };
    const char* menu_option_labels[NUM_OPTIONS] = { "Start Game", "Shut Down" };

    uint8_t  current_row = 0;
    lv_obj_t* main_screen;
    lv_obj_t* start_game_button;
    lv_obj_t* start_game_label;
    lv_obj_t* game_select_dropdown;
};

