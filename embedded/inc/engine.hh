#pragma once

#include "controls.hh"
#include "display.hh"
#include "state.hh"


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
                current_state = new_state;
            }
        }
    }

    // Display::frame update_frame(){};

private:
    // Subsystem pointers
    Controls*     p_controls      { nullptr };
    Display*      p_display       { nullptr };

    Engine::State current_state   { State::MENU };
    StateBase*    p_current_state { nullptr };
};


