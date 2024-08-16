#include "../../../inc/states/game/pong.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(pong_game, LOG_LEVEL_DBG);

// static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
//                               lv_menu_builder_variant_t builder_variant)
// {
//     lv_obj_t * obj = lv_menu_cont_create(parent);

//     lv_obj_t * img = NULL;
//     lv_obj_t * label = NULL;

//     if(icon) {
//         img = lv_image_create(obj);
//         lv_image_set_src(img, icon);
//     }

//     if(txt) {
//         label = lv_label_create(obj);
//         lv_label_set_text(label, txt);
//         lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//         lv_obj_set_flex_grow(label, 1);
//     }

//     if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
//         lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
//         lv_obj_swap(img, label);
//     }

//     return obj;
// }

static lv_obj_t * create_slider(lv_obj_t * parent, int32_t min, int32_t max,
                                int32_t val)
{
    // lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t * slider = lv_slider_create(parent);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    // if(icon == NULL) {
    //     lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    // }

    return slider;
}

namespace game
{

PongGame::PongGame()
{
    init_main_screen();
    init_start_screen();
    init_pause_screen();
    init_game_over_screen();
    init_settings_menu();

    init();
}


PongGame::~PongGame()
{
//    lv_obj_clean(frame);
//    lv_obj_clean(main_screen);
   lv_obj_del(frame);
   lv_obj_del(main_screen);
   lv_obj_del(start_screen);
//    lv_obj_del(game_over_screen);
//    lv_obj_clean(lv_scr_act());
}

void PongGame::close()
{

}

void PongGame::init_main_screen()
{
    main_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);

    player_a.obj = (lv_obj_t*)create_rect(main_screen, {player_a.w, player_a.h}, player_a.pos);
    player_b.obj = (lv_obj_t*)create_rect(main_screen, {player_b.w, player_b.h}, player_b.pos);
    ball.obj = (lv_obj_t*)create_rect(main_screen, {ball.w, ball.h}, ball.pos);

    // Score labels
    player_a_score_label = lv_label_create(main_screen);
    player_b_score_label = lv_label_create(main_screen);

    lv_obj_set_style_text_color(player_a_score_label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_color(player_b_score_label, lv_color_hex(0xffffff), LV_PART_MAIN);

    lv_obj_set_style_pad_left(player_a_score_label, 25, LV_PART_MAIN);
    lv_obj_set_style_pad_right(player_b_score_label, 25, LV_PART_MAIN);

    lv_obj_set_align(player_a_score_label, LV_ALIGN_TOP_LEFT );
    lv_obj_set_align(player_b_score_label, LV_ALIGN_TOP_RIGHT );
    lv_label_set_text_fmt(player_a_score_label, "%d", 0);
    lv_label_set_text_fmt(player_b_score_label, "%d", 0);

}


void PongGame::init_start_screen()
{
    start_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(start_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(start_screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );

    start_game_label = lv_label_create(start_screen);
    lv_obj_set_scrollbar_mode(start_game_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( start_game_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( start_game_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( start_game_label, LV_ALIGN_CENTER );
    lv_label_set_text(start_game_label,"Press Start to Play\nPress Select For settings");
    lv_obj_set_style_text_font(start_game_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
}


void PongGame::init_game_over_screen()
{
    game_over_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(game_over_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(game_over_screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );

    game_over_label = lv_label_create(game_over_screen);
    lv_obj_set_scrollbar_mode(game_over_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( game_over_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( game_over_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( game_over_label, LV_ALIGN_CENTER );
    lv_label_set_text(game_over_label,"Game over");
    lv_obj_set_style_text_font(game_over_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x272846), LV_PART_MAIN | LV_STATE_DEFAULT );
}


void PongGame::init_pause_screen()
{

}

static lv_style_t style_base;
static lv_style_t style_focused;

void PongGame::init_settings_menu()
{
    // Initialize base style
    lv_style_init(&style_base);
    lv_style_set_bg_color(&style_base, graphics::black);
    lv_style_set_bg_grad_color(&style_base, graphics::blue);
    lv_style_set_radius(&style_base, 10);
    lv_style_set_border_width(&style_base, 2);
    lv_style_set_border_color(&style_base, graphics::white);
    lv_style_set_outline_width(&style_base, 0);

    // Initialize focused style
    lv_style_init(&style_focused);
    lv_style_set_bg_color(&style_focused, graphics::blue);
    lv_style_set_bg_grad_color(&style_focused, graphics::purple);
    lv_style_set_border_width(&style_focused, 3);
    lv_style_set_border_color(&style_focused, graphics::red);
    lv_style_set_outline_width(&style_focused, 4);
    lv_style_set_outline_color(&style_focused, graphics::red);


    settings_menu = lv_menu_create(start_screen);
    lv_menu_set_mode_root_back_btn(settings_menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    // lv_obj_add_event_cb(settings_menu, back_event_handler, LV_EVENT_CLICKED, settings_menu);
    lv_obj_set_size(settings_menu, LV_HOR_RES-10, LV_HOR_RES);
    lv_obj_center(settings_menu);

    /*Create a main page*/
    settings_page = lv_menu_page_create(settings_menu, NULL);

    menu_slider_group = lv_group_create();

    cont = lv_menu_cont_create(settings_page);
    // label = lv_label_create(cont);
    // lv_label_set_text(label, "Item 1");

    slider = create_slider(cont, 1, 10, slider_val);
    lv_group_add_obj(menu_slider_group, slider);
    lv_obj_add_style(slider, &style_base, 0);
    lv_obj_add_style(slider, &style_focused, LV_STATE_FOCUSED);

    cont = lv_menu_cont_create(settings_page);
    // label = lv_label_create(cont);

    // lv_label_set_text(label, "Item 2");
    sliderb = create_slider(cont, 1, 10, slider_val);
    lv_group_add_obj(menu_slider_group, sliderb);
    lv_obj_add_style(sliderb, &style_base, 0);
    lv_obj_add_style(sliderb, &style_focused, LV_STATE_FOCUSED);

    cont = lv_menu_cont_create(settings_page);
    // label = lv_label_create(cont);

    // lv_label_set_text(label, "Item 3");
    sliderc = create_slider(cont, 1, 10, slider_val);
    lv_group_add_obj(menu_slider_group, sliderc);
    lv_obj_add_style(sliderc, &style_base, 0);
    lv_obj_add_style(sliderc, &style_focused, LV_STATE_FOCUSED);

    lv_menu_set_page(settings_menu, settings_page);
    lv_group_focus_obj(slider);
}




void PongGame::draw_background()
{
    // lv_canvas_fill_bg(frame, graphics::blue, LV_OPA_COVER);
}


void PongGame::draw_paddles()
{
    lv_obj_set_pos(player_a.obj, player_a.pos.x, player_a.pos.y);
    lv_obj_set_pos(player_b.obj, player_b.pos.x, player_b.pos.y);
}


void PongGame::draw_ball()
{
    lv_obj_set_pos(ball.obj, ball.pos.x, ball.pos.y);
}


void PongGame::draw_ready_to_play_state()
{
    // lv_scr_load(start_screen);
    // draw_background();

    // graphics::draw_text(frame, 70, 50, "Pong");
    // graphics::draw_text(frame, 50, 75, "Start Game");
}


void PongGame::draw_playing_state()
{
    if(game_paused) return;

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
    draw_paddles();
    draw_ball();
}


void PongGame::draw_game_finished_state()
{
    init();

    if(player_won)
    {
        lv_label_set_text(game_over_label,"You won");
    }
    else
    {
        lv_label_set_text(game_over_label,"Game over");
    }

    lv_scr_load(game_over_screen);

    game_info.player_a_score = 0;
    game_info.player_b_score = 0;
}


void PongGame::show()
{
    init();
    set_current_state(StateBase::State::RUNNING);
    play_state = PlayState::READY_TO_PLAY;

    lv_scr_load(start_screen);
}


void PongGame::draw()
{
    switch(play_state)
    {
        case PlayState::READY_TO_PLAY:     draw_ready_to_play_state();  break;
        case PlayState::PLAYING:           draw_playing_state();        break;
        case PlayState::GAME_FINISHED:     draw_game_finished_state();  break;
        case PlayState::SHOWING_SETTINGS:    break;
        default: break;
    }
	// lv_task_handler();
}


void PongGame::handle_collision()
{
    ball.pos.x += ball.velocity.x;
    ball.pos.y += ball.velocity.y;

    if(x_out_of_bounds(ball.pos.x))
    {
        if(player_a_scored(ball.pos.x))
        {
            // game_info.player_a_score++;
            lv_label_set_text_fmt(player_a_score_label, "%d", ++game_info.player_a_score);

            ball.pos.x = ball_start.x;
            ball.pos.y = ball_start.y;

            ball.velocity.x = settings.ball_start_velocity.x;
            ball.velocity.y = settings.ball_start_velocity.y;
        }
        if(player_b_scored(ball.pos.x))
        {
            // game_info.player_b_score++;
            lv_label_set_text_fmt(player_b_score_label, "%d", ++game_info.player_b_score);

            ball.pos.x = ball_start.x;
            ball.pos.y = ball_start.y;

            ball.velocity.x = -settings.ball_start_velocity.x;
            ball.velocity.y = -settings.ball_start_velocity.y;

        }
        if(somebody_won())
        {
            play_state = PlayState::GAME_FINISHED;

            player_won = (game_info.player_a_score == settings.winning_score);
        }
        LOG_INF("Score: A-%d, B-%d", game_info.player_a_score, game_info.player_b_score);
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

        if(ball.velocity.x > 0)
        {
            ball.velocity.x++;
        }
        else
        {
            ball.velocity.x--;
        }

        if(ball.velocity.y > 0)
        {
            ball.velocity.y++;
        }
        else
        {
            ball.velocity.y--;
        }
    }

    // Bounce off the top and bottom of the screen
    if (y_out_of_bounds(ball.pos.y))
    {
        ball.velocity.y = -ball.velocity.y;
    }

    n_ticks++;
}


/* Player Movement */
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



void check_endgoal_areas()
{

}


/* Button actions */
void PongGame::init()
{
    game_info.player_a_score = 0;
    game_info.player_b_score = 0;

    num_hits                 = 0;
    player_won               = false;

    ball.pos.x               = PongGame::ball_start.x;
    ball.pos.y               = PongGame::ball_start.y;

    player_a.pos.x           = PongGame::player_a_start.x;
    player_b.pos.y           = PongGame::player_b_start.y;
}


void PongGame::start_game()
{
    init();
    play_state = PlayState::PLAYING;
    lv_scr_load(main_screen);
}


void PongGame::continue_game()
{

}


void PongGame::back_to_start_menu()
{
    init();
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
            if(!playing_ai)
                move_player_b_up();
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
            if(!playing_ai)
                move_player_b_down();
            break;
        }
        case PlayState::READY_TO_PLAY:
        case PlayState::GAME_FINISHED:
            set_current_state(StateBase::State::READY_TO_CLOSE);
        default:
            break;
    }
}


void PongGame::handle_up_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING:       move_player_a_up(); break;
        case PlayState::READY_TO_PLAY: lv_group_focus_prev(menu_slider_group); break;
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::handle_down_button()
{
    switch(play_state)
    {
        case PlayState::PLAYING:       move_player_a_down(); break;
        case PlayState::READY_TO_PLAY: lv_group_focus_next(menu_slider_group); break;
        case PlayState::GAME_FINISHED:
        default: break;
    }
}


void PongGame::handle_left_button()
{
    uint32_t t = LV_KEY_LEFT;
    lv_event_send(lv_group_get_focused(menu_slider_group), LV_EVENT_KEY, &t);
}


void PongGame::handle_right_button()
{
    uint32_t t = LV_KEY_RIGHT;
    lv_event_send(lv_group_get_focused(menu_slider_group), LV_EVENT_KEY, &t);
}


void PongGame::handle_start_button()
{
    game_paused = !game_paused;
}


void PongGame::handle_select_button()
{
    switch(play_state)
    {
        case PlayState::READY_TO_PLAY: lv_scr_load(settings_menu); break;
        case PlayState::PLAYING:
        case PlayState::GAME_FINISHED:
        default: break;
    }
    // play_state = PlayState::SHOWING_SETTINGS;
    // init_settings_menu();
}

}
