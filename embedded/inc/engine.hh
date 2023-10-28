#pragma once

#include "controls.hh"
#include "display.hh"
#include "states/state.hh"
#include "states/pong.hh"
#include "states/main_menu.hh"

class Engine
{
public:
    enum class State { MENU, GAME };

    Engine(Controls* p_controls, Display* p_display)
            :p_controls(p_controls), p_display(p_display)
    {
        current_state   = State::GAME;
        p_display->init_display();
        p_current_state = new PongGame();

        // p_display->blank_screen();
    };

    ~Engine(){};

    void process_input();
    void process_frame();
    void process();
    void change_state(State new_state);

private:
    // Subsystem pointers
    Controls*     p_controls      { nullptr };
    Display*      p_display       { nullptr };

    Engine::State current_state   { State::GAME };
    StateBase*    p_current_state { nullptr };
};


