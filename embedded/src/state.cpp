#include "../inc/state.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(menu, LOG_LEVEL_ERR);


PongGame::PongGame()
{

}


PongGame::~PongGame()
{

}

graphics::frame_t PongGame::get_frame()
{

}


/* Button handlers */
void PongGame::handle_a_button()
{

}


void PongGame::handle_b_button()
{

}


void PongGame::handle_up_button()
{

}


void PongGame::handle_down_button()
{

}


void PongGame::handle_left_button()
{

}


void PongGame::handle_right_button()
{

}


/*
    Menu State
*/

MenuState::MenuState()
{

}


MenuState::~MenuState()
{

}


graphics::frame_t  MenuState::get_frame()
{
    return NULL;
}

/*
    Button handlers
*/
void MenuState::handle_a_button()
{
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
