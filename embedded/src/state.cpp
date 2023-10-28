#include "../inc/state.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(menu, LOG_LEVEL_DBG);


PongGame::PongGame()
{
    lv_init();
    frame = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(frame, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED);
	lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(frame, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(frame, LV_ALIGN_CENTER, 0, 0);
}


PongGame::~PongGame()
{

}


void PongGame::draw_background()
{
    lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);
}


void PongGame::draw_paddles()
{
	graphics::draw_rect(frame, player_a.pos, player_a.w, player_a.h, graphics::red);
	graphics::draw_rect(frame, player_b.pos, player_b.w, player_b.h, graphics::purple);
}


void PongGame::draw_ball()
{
    graphics::draw_rect(frame, ball.pos, ball.w, ball.h);
}


void PongGame::draw_ready_to_play_state()
{
    draw_background();

    graphics::draw_text(frame, 70, 50, "Pong");
    graphics::draw_text(frame, 50, 75, "Start Game");
}


void PongGame::draw_playing_state()
{
    handle_collision();
    draw_background();
    draw_paddles();
    draw_ball();
}


void PongGame::draw_game_finished_state()
{
    draw_background();

    graphics::draw_text(frame, 70, 50, "Game Over Yo");
    game_info.player_a_score = 0;
    game_info.player_b_score = 0;
}


void PongGame::draw()
{
    switch(play_state)
    {
        case PlayState::READY_TO_PLAY:  draw_ready_to_play_state();  break;
        case PlayState::PLAYING:        draw_playing_state();        break;
        case PlayState::GAME_FINISHED:  draw_game_finished_state();  break;
        default: break;
    }
	lv_task_handler();
}


void PongGame::handle_collision()
{
    ball.pos.x += ball.velocity.x;
    ball.pos.y += ball.velocity.y;

    if(x_out_of_bounds(ball.pos.x))
    {
        if(player_a_scored(ball.pos.x))
        {
            game_info.player_a_score++;
            ball.pos.x = ball_start_x;
            ball.pos.y = ball_start_y;
        }
        if(player_b_scored(ball.pos.x))
        {
            game_info.player_b_score++;
            ball.pos.x = ball_start_x;
            ball.pos.y = ball_start_y;
        }
        if(somebody_won())
        {
            play_state = PlayState::GAME_FINISHED;
        }
    }

    // Ball was getting stuck and oscillating.
    // Let it get away from the paddle after a bounce
    // before doing another collision check
    if(n_ticks > 20)
    {
        n_ticks = 0;
        bounced=false;
    }

    if ((graphics::entities_colliding(ball, player_a) ||
        graphics::entities_colliding(ball, player_b)) &&
        !bounced)
    {
        ball.velocity.x = -ball.velocity.x;
        bounced = true;
    }

    // Bounce off the top and bottom of the screen
    if (y_out_of_bounds(ball.pos.y))
    {
        ball.velocity.y = -ball.velocity.y;
    }

    n_ticks++;
}


void check_endgoal_areas()
{

}

/* Button actions */
void PongGame::start_game()
{
    game_info.player_a_score = 0;
    game_info.player_b_score = 0;
    play_state = PlayState::PLAYING;
}


void PongGame::continue_game()
{

}


void PongGame::back_to_start_menu()
{
    play_state = PlayState::READY_TO_PLAY;
}


/* Button handlers */
void PongGame::handle_a_button()
{
    LOG_INF("A button pressed");
    switch(play_state)
    {
        case PlayState::READY_TO_PLAY:  start_game();         break;
        case PlayState::GAME_FINISHED:  back_to_start_menu(); break;
        case PlayState::PLAYING:
        default: break;
    }
}


void PongGame::handle_b_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING:          move_player_a_down(); break;
        case PlayState::READY_TO_PLAY:
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::move_player_a_up()
{
    player_a.pos.y -= player_a.velocity.y;
}


void PongGame::move_player_a_down()
{
    player_a.pos.y += player_a.velocity.y;
}


void PongGame::handle_up_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING: move_player_a_up(); break;
        case PlayState::READY_TO_PLAY:
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::handle_down_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING: move_player_a_down(); break;
        case PlayState::READY_TO_PLAY:
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::handle_left_button()   { }
void PongGame::handle_right_button()  { }


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
