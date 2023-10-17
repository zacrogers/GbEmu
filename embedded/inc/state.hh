#pragma once

#include "controls.hh"
#include "display.hh"


class StateBase
{
public:
    enum class State { READY_TO_CLOSE, PREPARING_TO_CLOSE };

    virtual ~StateBase() = default;

    virtual void draw() = 0;

    inline bool ready_to_close()     { return current_state == State::READY_TO_CLOSE; }
    inline bool preparing_to_close() { return current_state == State::PREPARING_TO_CLOSE; }

    void process_input(const Controls::InputType& control)
    {
        switch (control)
        {
        case Controls::InputType::A:     handle_a_button();     break;
        case Controls::InputType::B:     handle_b_button();     break;
        case Controls::InputType::UP:    handle_up_button();    break;
        case Controls::InputType::DOWN:  handle_down_button();  break;
        case Controls::InputType::LEFT:  handle_left_button();  break;
        case Controls::InputType::RIGHT: handle_right_button(); break;

        // TODO: maybe use a joystick and implement these
        case Controls::InputType::UL:
        case Controls::InputType::UR:
        case Controls::InputType::DL:
        case Controls::InputType::DR:
            break;

        case Controls::InputType::NUM_INPUTS:
        case Controls::InputType::NONE:
        default:
            break;
        }
    }

private:
    /* Button handlers */
    virtual void handle_a_button     () = 0;
    virtual void handle_b_button     () = 0;
    virtual void handle_up_button    () = 0;
    virtual void handle_down_button  () = 0;
    virtual void handle_left_button  () = 0;
    virtual void handle_right_button () = 0;

    StateBase::State current_state { };
};


class PongGame: public StateBase
{
private:
    enum class PlayState { READY_TO_PLAY, PLAYING, MATCH_FINISHED, GAME_FINISHED };

    typedef struct {
        uint32_t total_time_ms;
        uint8_t player_a_score, player_b_score;
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

    void move_player_a_up          ();
    void move_player_a_down        ();

    // graphics
    void draw_ready_to_play_state  ();
    void draw_playing_state        ();
    void draw_match_finished_state ();
    void draw_game_finished_state  ();

    void handle_collision          ();

    void check_endgoal_areas       ();

    bool x_out_of_bounds           (int x) { return (x < 15 || x > 160); }
    bool y_out_of_bounds           (int y) { return (y < 15 || y > 128); }

    bool player_b_scored           (int x) { return (x < 15); }
    bool player_a_scored           (int x) { return (x > 160); }

    bool still_in_progress         ()
        { return (player_a_score != winning_score) || (player_b_score != winning_score); }

    uint8_t winning_score  = 5;
    game_info_t game_info = {
        .player_a_score = 0,
        .player_b_score = 0,
        .total_time_ms = 0
    };
    uint8_t player_a_score = 0;
    uint8_t player_b_score = 0;

    int squareVelocityX    = 5;
    int squareVelocityY    = 5;
    int paddle_length      = 70;

    uint8_t player_a_speed = 5, player_b_speed = 5, ball_speed = 5;
    int ball_start_x =25, ball_start_y = 25;

    graphics::entity_t player_a = { 15, 50, 15, paddle_length };
    graphics::entity_t player_b = { 170, 50, 15, paddle_length };
    graphics::entity_t ball     = { ball_start_x, ball_start_y, 15, 15, 1 };

    PlayState play_state { PlayState::PLAYING };

    bool going_left = false;

    graphics::frame_t frame { nullptr };
    int n_ticks = 0;
    bool bounced = false;

#define CANVAS_WIDTH  200
#define CANVAS_HEIGHT  150
    lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
};


class MenuState: public StateBase
{
private:
    enum Option { START_GAME = 0, SHUT_DOWN, NUM_OPTIONS, NONE };

public:
    MenuState();
    ~MenuState() override;

    void draw           () override;

private:
/* Member Functions */

    /* Button handlers */
    void             handle_a_button     () override;
    void             handle_b_button     () override;
    void             handle_up_button    () override;
    void             handle_down_button  () override;
    void             handle_left_button  () override;
    void             handle_right_button () override;

    /* Option processing */
    void             incr_option         ();
    void             decr_option         ();
    void             process_option      (const Option option);

    /* Menu option processes */
    void             start_game          ();
    void             shut_down           ();

    Option current_option     { Option::START_GAME };
    bool   option_is_selected { false };

    const char* menu_option_labels[NUM_OPTIONS] = {"Start Game", "Shut Down"};

    uint8_t current_row = 0;
};

