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

    // Handle game opening & closing
    if(game_playing && p_current_game->ready_to_close())
    {
        open_menu();
    }
    else if(!game_playing && main_menu.ready_to_close())
    {
        auto new_game = main_menu.selected_game();
        start_game(new_game);
    }
}


void Engine::open_menu()
{
    game_playing = false;
    main_menu.show();
}


void Engine::start_game(GameType game)
{
        // unload and delete current game
        // load new game
        // set button trigger map
        game_playing = true;
        p_current_game->show();
}
