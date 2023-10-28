#include "../../inc/states/pong.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pong_game, LOG_LEVEL_DBG);


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
    if(playing_ai)
    {
        if(ball.pos.y > player_b.pos.y)
        {
            move_player_b_up();
        }
        else if(ball.pos.y < player_b.pos.y)
        {
            move_player_b_down();
        }
    }

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
            ball.pos.x = ball_start.x;
            ball.pos.y = ball_start.y;
        }
        if(player_b_scored(ball.pos.x))
        {
            game_info.player_b_score++;
            ball.pos.x = ball_start.x;
            ball.pos.y = ball_start.y;
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
        num_hits++;
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
    num_hits = 0;
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
        {
            if(!playing_ai) move_player_b_up();
            break;
        }
        default: break;
    }
}


void PongGame::handle_b_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING:
        {
            if(!playing_ai) move_player_b_down();
            break;
        }
        case PlayState::READY_TO_PLAY:
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::move_player_a_up()
{
    if(player_a.pos.y > 0)
        player_a.pos.y -= player_a.velocity.y;
}


void PongGame::move_player_a_down()
{
    if(player_a.pos.y < CANVAS_HEIGHT - paddle_length)
        player_a.pos.y += player_a.velocity.y;
}


void PongGame::move_player_b_up()
{
    if(player_b.pos.y > 15)
        player_b.pos.y -= player_b.velocity.y;
}


void PongGame::move_player_b_down()
{
    if((player_b.pos.y) < (CANVAS_HEIGHT - paddle_length))
        player_b.pos.y += player_b.velocity.y;
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
