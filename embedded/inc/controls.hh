#pragma once

#include <zephyr/drivers/gpio.h>

#include "etl/queue.h"
#include "etl/queue_spsc_locked.h"


class Controls
{
public:
    static constexpr size_t INPUT_QUEUE_SIZE = 3;

    enum InputType   { A, B, UP, DOWN, LEFT, RIGHT, UL, UR, DL, DR, NUM_INPUTS, NONE };
    enum TriggerType { ONE_SHOT, PERIODIC };

    Controls();
    ~Controls();

    static const InputType get_last_pressed();
    static void clear_queue();
    static void set_trigger_map(TriggerType trigger_map[InputType::NUM_INPUTS]);


private:
    static struct gpio_dt_spec                     button[InputType::NUM_INPUTS];
    static TriggerType                             trigger_mapping[InputType::NUM_INPUTS];
    static struct k_timer                          button_timers[InputType::NUM_INPUTS];
    static struct gpio_callback                    cb_data[InputType::NUM_INPUTS];
    static uint64_t                                last_time[InputType::NUM_INPUTS];
    static etl::queue<InputType, INPUT_QUEUE_SIZE> input_queue;

    /* Timer Handlers */
    static void timer_handler             (InputType input);
    static void a_button_timer_handler    (struct k_timer *dummy);
    static void b_button_timer_handler    (struct k_timer *dummy);
    static void up_button_timer_handler   (struct k_timer *dummy);
    static void down_button_timer_handler (struct k_timer *dummy);

    /* Button handlers */
    static void handle_button             (InputType input);
    static void a_button_pressed          (const struct device *dev,
                                            struct gpio_callback* cb, uint32_t pins);
    static void b_button_pressed          (const struct device *dev,
                                            struct gpio_callback* cb, uint32_t pins);
    static void up_button_pressed         (const struct device *dev,
                                            struct gpio_callback* cb, uint32_t pins);
    static void down_button_pressed       (const struct device *dev,
                                            struct gpio_callback* cb, uint32_t pins);

};
