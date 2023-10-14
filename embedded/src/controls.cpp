#include "../inc/controls.hh"



Controls::Controls()
{

}


Controls::~Controls()
{

}


const Controls::InputType Controls::get_last_pressed()
{
    if(!input_queue.empty())
    {
        auto last_pressed = input_queue.front();
        input_queue.pop();
        return last_pressed;
    }

    return Controls::InputType::NONE;
}
