#include "../inc/state.hh"

MenuState::MenuState()
{

}


MenuState::~MenuState()
{

}


Display::frame MenuState::get_frame()
{
    return 0;
}

void MenuState::handle_a_button()
{
    if(current_row > 0) { current_row--; }
    else                { current_row = (NUM_OPTIONS - 1); }
}


void MenuState::handle_b_button()
{
    if(current_row < NUM_OPTIONS) current_row++;
}


void MenuState::handle_up_button()
{
    if(current_row > 0) { current_row--; }
    else                { current_row = (NUM_OPTIONS - 1); }
}


void MenuState::handle_down_button()
{
    if(current_row < NUM_OPTIONS) current_row++;
}


void MenuState::handle_left_button()
{
    if(current_row > 0) { current_row--; }
    else                { current_row = (NUM_OPTIONS - 1); }
}


void MenuState::handle_right_button()
{
    if(current_row < NUM_OPTIONS) current_row++;
}