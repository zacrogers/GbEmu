#pragma once

#include "../controls.hh"
#include "../display.hh"


class StateBase
{
public:
    enum class State { READY_TO_CLOSE, PREPARING_TO_CLOSE, RUNNING };

    virtual ~StateBase() = default;

    virtual void draw() = 0;
    virtual void show() = 0;

    inline bool ready_to_close()     { return current_state == State::READY_TO_CLOSE; }
    inline bool preparing_to_close() { return current_state == State::PREPARING_TO_CLOSE; }
    void set_current_state(StateBase::State state) { current_state = state; }
    void get_trigger_map();

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
    // virtual void init                () = 0;
    /* Button handlers */
    virtual void handle_a_button     () = 0;
    virtual void handle_b_button     () = 0;
    virtual void handle_up_button    () = 0;
    virtual void handle_down_button  () = 0;
    virtual void handle_left_button  () = 0;
    virtual void handle_right_button () = 0;

    StateBase::State      current_state { State::RUNNING };
    Controls::TriggerType trigger_map[Controls::InputType::NUM_INPUTS] { };
};


// class GameBase : public StateBase
// {

// };