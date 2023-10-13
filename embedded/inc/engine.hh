#pragma once

#include "controls.hh"
#include "display.hh"


class Engine
{
public:
    Engine(Controls* controls, Display* display)
            :controls(controls), display(display) {};
    ~Engine(){};

    void process_input()
    {
        auto last_control = controls->get_last_pressed();

        switch (last_control)
        {
        case Controls::InputType::A:
            handle_a_button();
            break;

        case Controls::InputType::B:
            handle_b_button();
            break;

        case Controls::InputType::UP:
        case Controls::InputType::DOWN:
        case Controls::InputType::LEFT:
        case Controls::InputType::RIGHT:
            handle_dpad(last_control);
            break;

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

    void handle_dpad(Controls::InputType direction){}
    void handle_a_button(){}
    void handle_b_button(){}


    void process()
    {
        process_input();
    }

    // Display::frame update_frame(){};

private:
    Controls* controls { nullptr };
    Display*  display  { nullptr };
};


