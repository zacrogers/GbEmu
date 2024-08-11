#pragma once

#include "../state.hh"
#include "../../subsystems/controls.hh"
#include "../../subsystems/display.hh"


namespace game
{

class PongGame: public StateBase
{
private:
    enum class PlayState { READY_TO_PLAY, PLAYING, GAME_FINISHED, SHOWING_SETTINGS };

    typedef struct {
        uint8_t player_a_score, player_b_score;
        uint32_t total_time_ms;
    } game_info_t;

public:
    struct settings {
        graphics::pos_t ball_start_velocity     { 5, 5 };
        graphics::pos_t ball_velocity_increment { 1, 1 };
        uint8_t winning_score                   { 5 };
        bool audio_enabled                      { false };
    };

    PongGame();
    ~PongGame() override;

    void draw                      () override;
    void show                      () override;

    void init();
    void close();

private:
/* Member Functions */
    /* Screen Initialisers */
    void init_main_screen();
    void init_start_screen();
    void init_game_over_screen();
    void init_pause_screen();
    void init_settings_menu();

    /* Button handlers */
    void handle_a_button           () override;
    void handle_b_button           () override;
    void handle_up_button          () override;
    void handle_down_button        () override;
    void handle_left_button        () override;
    void handle_right_button       () override;

    /* Button actions */
    // Play state
    void continue_game             ();
    void move_player_a_up          ();
    void move_player_a_down        ();
    void move_player_b_up          ();
    void move_player_b_down        ();
    // Ready to play state
    void start_game                ();
    // Game finished state
    void back_to_start_menu        ();

    /* Graphics */
    void draw_playing_state        ();
    void draw_background           ();
    void draw_paddles              ();
    void draw_ball                 ();

    void draw_ready_to_play_state  ();
    void draw_game_finished_state  ();

    /* Bounds & collision checking */
    void handle_collision          ();
    void check_endgoal_areas       ();

    bool x_out_of_bounds           (const int x) { return (x < 15 || x > 160); }
    bool y_out_of_bounds           (const int y) { return (y < 15 || y > 128); }

    bool player_b_scored           (const int x) { return (x < 10); }
    bool player_a_scored           (const int x) { return (x > 145); }

    bool somebody_won              ()
        { return (game_info.player_a_score == settings.winning_score) ||
                (game_info.player_b_score == settings.winning_score); }

/* Constants */
    static constexpr std::uint8_t    paddle_length  { 56 };
    static constexpr std::uint8_t    paddle_width   { 10 };
    static constexpr std::uint8_t    ball_size      { 10 };
    static constexpr graphics::pos_t ball_start     { 80, 25 };
    static constexpr graphics::pos_t player_a_start { 0, 50 };
    static constexpr graphics::pos_t player_b_start { 145, 50 };

/* Variables */
    struct settings      settings       {};
    game_info_t          game_info      { 0, 0, 0 };

    /* Entities */
    graphics::entity_t   player_a       { player_a_start, paddle_width, paddle_length, 0, {0, 5}, nullptr };
    graphics::entity_t   player_b       { player_b_start, paddle_width, paddle_length, 0, {0, 5}, nullptr };
    graphics::entity_t   ball           { ball_start, ball_size, ball_size, 0, settings.ball_start_velocity, nullptr };

    PlayState            play_state     { PlayState::READY_TO_PLAY };

    lv_obj_t*            main_screen;
    lv_obj_t*            start_screen;
    lv_obj_t*            game_over_screen;
    lv_obj_t*            settings_menu;

    graphics::frame_t    frame          { nullptr };
    int                  n_ticks        { 0 };
    bool                 bounced        { false };
    bool                 playing_ai     { true };
    uint16_t             num_hits       { 0 };
    bool                 player_won     { false };


    lv_obj_t*            start_game_label;
    lv_obj_t*            game_over_label;

    lv_obj_t*            player_a_score_label;
    lv_obj_t*            player_b_score_label;

#define CANVAS_WIDTH  200
#define CANVAS_HEIGHT  150
    lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
};

}
