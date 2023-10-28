#pragma once

#include "state.hh"
#include "../controls.hh"
#include "../display.hh"


class PongGame: public StateBase
{
private:
    enum class PlayState { READY_TO_PLAY, PLAYING, GAME_FINISHED };

    typedef struct {
        uint8_t player_a_score, player_b_score;
        uint32_t total_time_ms;
    } game_info_t;

public:
    typedef struct {
        graphics::entity_t entity;
        uint8_t score;
    } player_t;

    PongGame();
    ~PongGame() override;

    void draw                      () override;

private:
/* Member Functions */
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

    bool player_b_scored           (const int x) { return (x < 15); }
    bool player_a_scored           (const int x) { return (x > 160); }

    bool somebody_won              ()
        { return (game_info.player_a_score == winning_score) ||
                (game_info.player_b_score == winning_score); }

/* Constants */
    static const uint8_t             winning_score  { 5 };
    static const int                 paddle_length  { 66 };
    static constexpr graphics::pos_t     ball_start     { 35, 25 };

/* Variables */
    game_info_t          game_info      { 0, 0, 0 };

    /* Entities */
    graphics::entity_t   player_a       { {15, 50}, 15, paddle_length, 0, {0, 5} };
    graphics::entity_t   player_b       { {170, 50}, 15, paddle_length, 0, {0, 5} };
    graphics::entity_t   ball           { ball_start, 15, 15, 0, {5, 5} };

    PlayState            play_state     { PlayState::PLAYING };
    graphics::frame_t    frame          { nullptr };
    int                  n_ticks        { 0 };
    bool                 bounced        { false };
    bool                 playing_ai     { true };
    uint16_t             num_hits       { 0 };

#define CANVAS_WIDTH  200
#define CANVAS_HEIGHT  150
    lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
};

