#pragma once

#include <zephyr/drivers/gpio.h>

#include "etl/queue.h"
#include "etl/queue_spsc_locked.h"


class Controls
{
public:
    static constexpr size_t INPUT_QUEUE_SIZE = 0xFF;

    enum InputType { A, B, UP, DOWN, LEFT, RIGHT, UL, UR, DL, DR, NUM_INPUTS, NONE };

    Controls();
    ~Controls();

    static const InputType get_last_pressed();

// private:
    static struct gpio_dt_spec a_button;
    static struct gpio_dt_spec b_button;


    static etl::queue<InputType, INPUT_QUEUE_SIZE>  input_queue;

public:
    static void add_to_queue(const InputType& input) { input_queue.push(input); }
};
