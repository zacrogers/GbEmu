#pragma once

#include <zephyr/drivers/gpio.h>

#include "etl/queue.h"
#include "etl/queue_spsc_locked.h"


class Controls
{
public:
    static const size_t INPUT_QUEUE_SIZE = 0xFF;

    enum InputType { A, B, UP, DOWN, LEFT, RIGHT, UL, UR, DL, DR, NUM_INPUTS, NONE };

    Controls();
    ~Controls();

    const InputType get_last_pressed();

private:
    struct gpio_dt_spec a_button = GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios);
    struct gpio_dt_spec b_button = GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios);


    etl::queue<InputType, INPUT_QUEUE_SIZE>  input_queue { };

    void init_gpio(){}
    void add_to_queue(const InputType& input) { input_queue.push(input); }
};
