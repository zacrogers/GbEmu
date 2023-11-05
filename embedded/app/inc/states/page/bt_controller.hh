#pragma once

#include "../state.hh"

class BtController : public StateBase
{
public:
    BtController();
    ~BtController() override;

    void draw() override;
    void show() override;

private:
/* Member Functions */
    void draw_main_screen();

    /* Button handlers */
    void             handle_a_button     () override;
    void             handle_b_button     () override;
    void             handle_up_button    () override;
    void             handle_down_button  () override;
    void             handle_left_button  () override;
    void             handle_right_button () override;
};