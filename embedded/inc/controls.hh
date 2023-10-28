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
    static void update();
    static void clear_queue();

// private:
    static struct gpio_dt_spec a_button;
    static struct gpio_dt_spec b_button;

    static bool input_pressed[InputType::NUM_INPUTS];

    static etl::queue<InputType, INPUT_QUEUE_SIZE> input_queue;
    static uint64_t last_time;
    struct gpio_callback a_btn_cb_data;
    struct gpio_callback b_btn_cb_data;

    static void a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);
    static void b_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);

public:
    // static void add_to_queue(const InputType& input) { input_queue.push(input); }
};
