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


void PongGame::draw_ready_to_play_state()
{
    // Fill background
    lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);

}


void PongGame::draw_playing_state()
{
    handle_collision();

    // Fill background
    lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);

	// Draw paddles
	graphics::draw_rect(frame, player_a.x, player_a.y, player_a.w, player_a.h, graphics::red);
	graphics::draw_rect(frame, player_b.x, player_b.y, player_b.w, player_b.h, graphics::red);

    // Draw ball
    graphics::draw_rect(frame, ball.x, ball.y, ball.w, ball.h, graphics::green);
}


void PongGame::draw_match_finished_state()
{
    // Fill background
    lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);

}


void PongGame::draw_game_finished_state()
{
    // Fill background
    lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);

}


void PongGame::draw()
{
    switch(play_state)
    {
        case PlayState::READY_TO_PLAY:  draw_ready_to_play_state();  break;
        case PlayState::PLAYING:        draw_playing_state();        break;
        case PlayState::MATCH_FINISHED: draw_match_finished_state(); break;
        case PlayState::GAME_FINISHED:  draw_game_finished_state();  break;
        default: break;
    }
	lv_task_handler();
}


void PongGame::handle_collision()
{
    ball.x += squareVelocityX;
    ball.y += squareVelocityY;

    if (x_out_of_bounds(ball.x))
    {
        if(player_a_scored(ball.x))
        {
            player_a_score++;
        }
        if(player_b_scored(ball.x))
        {
            player_b_score++;
        }
        if(!still_in_progress())
        {

        }
    }
    if(n_ticks > 20) {
        n_ticks = 0;
        bounced=false;
    }

    if ((graphics::entities_colliding(ball, player_a) ||
        graphics::entities_colliding(ball, player_b)) && !bounced)
    {
        squareVelocityX = -squareVelocityX;
        bounced = true;
    }

    // Bounce off the top and bottom of the screen
    if (y_out_of_bounds(ball.y))
    {
        squareVelocityY = -squareVelocityY;
    }

    n_ticks++;
}

void check_endgoal_areas()
{

}

/* Button handlers */
void PongGame::handle_a_button()
{

}


void PongGame::handle_b_button()
{

}


void PongGame::move_player_a_up()
{

}


void PongGame::move_player_a_down()
{

}



void PongGame::handle_up_button()
{
    // if(player_a.y > player_a_
    switch (play_state)
    {
    case PlayState::PLAYING: move_player_a_up(); break;

    default: break;
    }
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


void  MenuState::draw()
{
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
