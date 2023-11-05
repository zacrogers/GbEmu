#include "../inc/engine.hh"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(engine, LOG_LEVEL_DBG);

namespace vroom
{

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


void Engine::load_game(GameType game)
{
    if(game == current_game)
    {
        p_current_game->show();
        return;
    }

    current_game = game;

    StateBase* new_game { nullptr };

    switch(game)
    {
        case GameType::PONG:         new_game = new game::PongGame();  break;
        case GameType::WIFI_MANAGER: new_game = new pages::Wifi(wifi_conn); break;
        case GameType::SNAKE:
        case GameType::GAMEBOY:
        case GameType::BT_CONTROLLER:
        case GameType::SERIAL_MONITOR:
        break;
    }

    if(new_game)
    {
        new_game->show();
        delete p_current_game;
        p_current_game = new_game;
        new_game = nullptr;
    }
}


void Engine::start_game(GameType game)
{
    LOG_INF("Start game: %d", static_cast<int>(game));
    load_game(game);
    // set button trigger map
    game_playing = true;
}

}
