#pragma once

#include <zephyr/drivers/gpio.h>

#include "etl/function.h"
#include "etl/queue_spsc_locked.h"

void QueueLock()
{
//   OsDisableInterrupts();
}

void QueueUnlock()
{
//   OsEnableInterrupts();
}


class Controls
{
public:
    static const size_t INPUT_QUEUE_SIZE = 0xFF;

    enum InputType { A, B, UP, DOWN, LEFT, RIGHT, UL, UR, DL, DR, NUM_INPUTS, NONE };

    Controls(/* args */);
    ~Controls();

    const InputType get_last_pressed()
    {
        if(!input_queue.empty())
        {
            InputType last_pressed;

            if(input_queue.pop(last_pressed))
            {
                return last_pressed;
            }
        }

        return InputType::NONE;
    }


private:
    struct gpio_dt_spec a_button = GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios);
    struct gpio_dt_spec b_button = GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios);


    etl::function_fv<QueueLock>        queueLock   { };
    etl::function_fv<QueueUnlock>      queueUnlock { };

    etl::queue_spsc_locked
        <InputType, INPUT_QUEUE_SIZE>  input_queue { queueLock, queueUnlock };

    void init_gpio(){}
    void add_to_queue(InputType input) { input_queue.push(input); };
};

Controls::Controls(/* args */)
{
}

Controls::~Controls()
{
}
