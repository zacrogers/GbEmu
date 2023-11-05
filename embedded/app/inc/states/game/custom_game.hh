#pragma once

#include "state.hh"

class CustomGame: public StateBase
{
public:
    CustomGame();
    ~CustomGame() override;

    void draw                      () override;
    void show                      () override;

    void load_script();
    void draw_foreground();
    void draw_background();
    void draw_entities();
    void handle_collision();

private:
/* Member Functions */
    /* Button handlers */
    void handle_a_button           () override;
    void handle_b_button           () override;
    void handle_up_button          () override;
    void handle_down_button        () override;
    void handle_left_button        () override;
    void handle_right_button       () override;
};
