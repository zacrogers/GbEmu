#include "../inc/engine.hh"


void Engine::process_input()
{
    auto last_control = p_controls->get_last_pressed();

    if(game_playing)
    {
        p_current_game->process_input(last_control);
    }
    else
    {
        main_menu.process_input(last_control);
    }
}

void Engine::process_frame()
{
    if(game_playing)
    {
        p_current_game->draw();
    }
    else
    {
        main_menu.draw();
    }
}


void Engine::process()
{
    process_input();
    process_frame();

    if(game_playing && p_current_game->ready_to_close())
    {
        // change_state(State::MENU);
        game_playing = false;
        main_menu.show();
    }
    else if(!game_playing && main_menu.ready_to_close())
    {
        // change_state(State::GAME);
        game_playing = true;
        p_current_game->show();
    }
}


void Engine::change_state(State new_state)
{
    // StateBase* tmp { nullptr };

    // switch(new_state)
    // {
    //     case State::MENU: tmp = new MenuState(); break;
    //     case State::GAME: tmp = new PongGame();  break;
    //     default: break;
    // }
    switch(new_state)
    {
        case State::MENU: game_playing = false; break;
        case State::GAME: start_game();  break;
        default: break;
    }

    // if(tmp)
    // {
    //     delete p_current_game;
    //     p_current_game = tmp;
    //     current_state = new_state;
    //     // p_controls->set_trigger_map(p_current_state.get_trigger_map());
    // }
}

    void Engine::start_game()
    {
        game_playing = true;
    }