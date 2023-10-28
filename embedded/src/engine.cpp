#include "../inc/engine.hh"


void Engine::process_input()
{
    if(p_controls && p_current_state)
    {
        auto last_control = p_controls->get_last_pressed();
        p_current_state->process_input(last_control);
    }
}

void Engine::process_frame()
{
    if(p_current_state && p_display)
    {
        p_current_state->draw();
        // p_display->draw_frame(new_frame);
    }
}


void Engine::process()
{
    process_input();
    process_frame();
}


void Engine::change_state(State new_state)
{
    if(p_current_state && p_current_state->ready_to_close())
    {
        StateBase* tmp { nullptr };

        switch(new_state)
        {
            case State::MENU: tmp = new MenuState(); break;
            case State::GAME: tmp = new PongGame();  break;
            default: break;
        }

        if(tmp)
        {
            delete p_current_state;
            p_current_state = tmp;
            current_state = new_state;
            // p_controls->set_trigger_map(p_current_state.get_trigger_map());
        }
    }
}

