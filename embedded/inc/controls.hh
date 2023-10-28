#pragma once

#include <zephyr/drivers/gpio.h>

#include "etl/queue.h"
#include "etl/queue_spsc_locked.h"


class Controls
{
public:
    static constexpr size_t INPUT_QUEUE_SIZE = 0xFF;

    enum InputType   { A, B, UP, DOWN, LEFT, RIGHT, UL, UR, DL, DR, NUM_INPUTS, NONE };
    enum TriggerType { ONE_SHOT, PERIODIC };

    Controls();
    ~Controls();

    static const InputType get_last_pressed();
    static void update();
    static void clear_queue();


// private:
    static struct gpio_dt_spec button[InputType::NUM_INPUTS];

    static void button_work_handler(struct k_work *work);
    static void up_button_timer_handler(struct k_timer *dummy);
    static void down_button_timer_handler(struct k_timer *dummy);
    static void button_timer_handler(struct k_timer *dummy);

    static bool           input_pressed[InputType::NUM_INPUTS];
    static TriggerType    trigger_mapping[InputType::NUM_INPUTS];
    static struct k_timer button_timers[InputType::NUM_INPUTS];
    static struct k_timer button_timer;

    static InputType current_input;

    static etl::queue<InputType, INPUT_QUEUE_SIZE> input_queue;
    static uint64_t last_time;

    struct gpio_callback a_btn_cb_data;
    struct gpio_callback b_btn_cb_data;
    struct gpio_callback up_btn_cb_data;
    struct gpio_callback down_btn_cb_data;

    static void a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);
    static void b_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);
    static void up_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);
    static void down_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins);

public:
    // static void add_to_queue(const InputType& input) { input_queue.push(input); }
};
