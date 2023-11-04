#pragma once

#include "common.hh"
#include "subsystems/controls.hh"
#include "subsystems/display.hh"
#include "states/state.hh"
#include "states/pong.hh"
#include "states/main_menu.hh"

namespace vroom
{
class Engine
{
public:
    enum class State { MENU, GAME };
    enum class Game  { PONG, SNAKE };

    Engine(Controls* p_controls, Display* p_display)
            :p_controls(p_controls), p_display(p_display)
    {
        p_current_game = new game::PongGame();
        main_menu.show();
    };

    ~Engine() {};

    void process_input ();
    void process_frame ();
    void process       ();
    void open_menu     ();
    void start_game    (GameType game);

private:
    Controls*     p_controls      { nullptr };
    Display*      p_display       { nullptr };
    MenuState     main_menu       { };
    bool          game_playing    { false };
    StateBase*    p_current_game  { nullptr };
};

}
