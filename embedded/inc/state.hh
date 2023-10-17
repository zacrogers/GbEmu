#pragma once

#include "controls.hh"
#include "display.hh"


class StateBase
{
public:
    enum class State { READY_TO_CLOSE, PREPARING_TO_CLOSE };

    virtual ~StateBase() = default;

    virtual Display::frame_t get_frame() = 0;

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


class MenuState: public StateBase
{
private:
    enum Option { START_GAME = 0, SHUT_DOWN, NUM_OPTIONS, NONE };

public:
    MenuState();
    ~MenuState() override;

    Display::frame_t get_frame           () override;

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

