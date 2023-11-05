#pragma once

#include "common.hh"
#include "subsystems/controls.hh"
#include "subsystems/display.hh"
#include "states/state.hh"
#include "states/game/pong.hh"
#include "states/page/main_menu.hh"
#include "states/page/wifi_page.hh"

namespace vroom
{
class Engine
{
public:
    enum class State { MENU, GAME };
    enum class Game  { PONG, SNAKE };

    Engine(Controls* p_controls, Display* p_display, connectivity::Wifi& wifi_conn)
            :p_controls(p_controls), p_display(p_display), wifi_conn(wifi_conn)
    {
        // load_game(current_game);
        // p_current_game = new pages::Wifi(wifi_conn);
        p_current_game = new game::PongGame();
        game_playing = false;
        main_menu.show();
    };

    ~Engine() {};

    void process_input ();
    void process_frame ();
    void process       ();
    void open_menu     ();
    void start_game    (GameType game);
    void load_game     (GameType game);

private:
    Controls*     p_controls      { nullptr };
    Display*      p_display       { nullptr };
    MenuState     main_menu       { };
    bool          game_playing    { false };
    StateBase*    p_current_game  { nullptr };
    GameType      current_game    { GameType::PONG };

    connectivity::Wifi& wifi_conn;
};

}
