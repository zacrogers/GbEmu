#include "../inc/state.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(menu, LOG_LEVEL_ERR);


PongGame::PongGame()
{
    lv_init();
    frame = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(frame, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(frame, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(frame, LV_ALIGN_CENTER, 0, 0);
}


PongGame::~PongGame()
{

}


graphics::frame_t PongGame::get_frame()
{

	// // paddles
	// graphics::draw_rect(frame, player_a.x, player_a.y, 15, 80, graphics::blue);
	// graphics::draw_rect(frame, player_b.x, player_b.y, 15, 80, graphics::blue);

    // for(int i = 0; i < 50; ++i)
    // {
        lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);
        if(ball.x < 200 && ball.x > 0)
            ball.x+=5;
        else
            ball.x=15;

        // ball
        graphics::draw_rect(frame, ball.x, ball.y, 15, 15, graphics::green);
        lv_task_handler();
        // k_msleep(1000);
    // }

    return frame;
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
    // if(player_a.y > player_a_speed)
    //     player_a.y -= player_a_speed;
}


void PongGame::handle_down_button()
{
    // if(player_a.y <= 100)
    //     player_a.y += player_a_speed;
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
