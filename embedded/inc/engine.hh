#pragma once

#include "etl/function.h"
#include "etl/callback_service.h"

#include "controls.hh"
#include "display.hh"


class StateBase
{
public:
    enum class State { READY_TO_CLOSE, PREPARING_TO_CLOSE };

    virtual ~StateBase() {};

    virtual Display::frame get_frame() = 0;

    bool ready_to_close()     { return current_state == State::READY_TO_CLOSE; }
    bool preparing_to_close() { return current_state == State::PREPARING_TO_CLOSE; }

    void process_input(Controls::InputType& control)
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
    virtual void handle_a_button     () = 0;
    virtual void handle_b_button     () = 0;
    virtual void handle_up_button    () = 0;
    virtual void handle_down_button  () = 0;
    virtual void handle_left_button  () = 0;
    virtual void handle_right_button () = 0;

    State current_state { };
};

class MenuState: public StateBase
{
public:
    MenuState(){};
    ~MenuState() override {};

    Display::frame get_frame           () override  { return 0; }

private:
    void           handle_a_button     () override {};
    void           handle_b_button     () override {};
    void           handle_up_button    () override {};
    void           handle_down_button  () override {};
    void           handle_left_button  () override {};
    void           handle_right_button () override {};
};


class Engine
{
public:
    enum class State { MENU, GAME };

    Engine(Controls* p_controls, Display* p_display)
            :p_controls(p_controls), p_display(p_display)
    {
        current_state   = State::MENU;
        p_current_state = new MenuState();
    };

    ~Engine(){};


    void process_input()
    {
        if(p_controls && p_current_state)
        {
            auto last_control = p_controls->get_last_pressed();
            p_current_state->process_input(last_control);
        }
    }

    void process_frame()
    {
        if(p_current_state && p_display)
        {
            auto new_frame = p_current_state->get_frame();
            p_display->draw_frame(new_frame);
        }
    }


    void process()
    {
        process_input();
        process_frame();
    }


    void change_state(State new_state)
    {
        if(p_current_state && p_current_state->ready_to_close())
        {
            StateBase* tmp { nullptr };

            switch(new_state)
            {
                case State::MENU: tmp = new MenuState(); break;
                case State::GAME: tmp = new MenuState(); break;
                default: break;
            }

            if(tmp)
            {
                delete p_current_state;
                p_current_state = tmp;
            }
        }
    }

    // Display::frame update_frame(){};

private:
    // Subsystem pointers
    Controls*    p_controls      { nullptr };
    Display*     p_display       { nullptr };

    State        current_state   { State::MENU };
    StateBase*   p_current_state { nullptr };
};


