#include "../inc/controls.hh"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(controls, LOG_LEVEL_DBG);



etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue {};
struct k_timer Controls::button_timers[InputType::NUM_INPUTS] {};
struct k_timer Controls::button_timer {};
struct gpio_dt_spec Controls::button[InputType::NUM_INPUTS] {
    GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(up_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(down_button), gpios)
};

Controls::InputType Controls::current_input = Controls::InputType::NONE;

uint64_t Controls::last_time = 0;

#define DEBOUNCE_TIMEOUT_MS 150

void Controls::button_timer_handler(struct k_timer *dummy)
{
    Controls::input_queue.push(Controls::current_input);
    if(!gpio_pin_get_dt(&button[Controls::current_input]))
    {
        k_timer_stop(&button_timers[Controls::current_input]);
    }
}

void Controls::up_button_timer_handler(struct k_timer *dummy)
{
    Controls::input_queue.push(Controls::InputType::UP);
    if(!gpio_pin_get_dt(&button[Controls::InputType::UP]))
    {
        k_timer_stop(&button_timers[Controls::InputType::UP]);
    }
}

void Controls::down_button_timer_handler(struct k_timer *dummy)
{
    Controls::input_queue.push(Controls::InputType::DOWN);
    if(!gpio_pin_get_dt(&button[Controls::InputType::DOWN]))
    {
        k_timer_stop(&button_timers[Controls::InputType::DOWN]);
    }
}

void Controls::clear_queue()
{
}


Controls::Controls()
{
    k_timer_init(&Controls::button_timer, button_timer_handler, NULL);
    if (device_is_ready(button[Controls::InputType::A].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::A], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::A], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&a_btn_cb_data, this->a_button_pressed , BIT(button[Controls::InputType::A].pin));
        gpio_add_callback(button[Controls::InputType::A].port, &a_btn_cb_data);
        k_timer_init(&Controls::button_timers[Controls::InputType::A], button_timer_handler, NULL);
    }
    if (device_is_ready(button[Controls::InputType::B].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::B], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::B], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&b_btn_cb_data, this->b_button_pressed , BIT(button[Controls::InputType::B].pin));
        gpio_add_callback(button[Controls::InputType::B].port, &b_btn_cb_data);
        k_timer_init(&Controls::button_timers[Controls::InputType::B], button_timer_handler, NULL);
    }
    if (device_is_ready(button[Controls::InputType::UP].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::UP], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::UP], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&up_btn_cb_data, this->up_button_pressed , BIT(button[Controls::InputType::UP].pin));
        gpio_add_callback(button[Controls::InputType::UP].port, &up_btn_cb_data);
        k_timer_init(&Controls::button_timers[Controls::InputType::UP], up_button_timer_handler, NULL);
    }
    if (device_is_ready(button[Controls::InputType::DOWN].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::DOWN], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::DOWN], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&down_btn_cb_data, this->down_button_pressed , BIT(button[Controls::InputType::DOWN].pin));
        gpio_add_callback(button[Controls::InputType::DOWN].port, &down_btn_cb_data);
        k_timer_init(&Controls::button_timers[Controls::InputType::DOWN], down_button_timer_handler, NULL);
    }

}


Controls::~Controls()
{

}


const Controls::InputType Controls::get_last_pressed()
{
    if(!input_queue.empty())
    {
        Controls::InputType last_pressed;
        input_queue.pop_into(last_pressed);
        return last_pressed;
    }

    return Controls::InputType::NONE;
}


/*
    Button Handlers
*/

void Controls::a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        current_input = InputType::A;
        k_timer_start(&button_timers[InputType::A], K_MSEC(50), K_MSEC(50));
    }

    Controls::last_time = now;
}


void Controls::b_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        current_input = InputType::B;
        k_timer_start(&button_timers[InputType::B], K_MSEC(50), K_MSEC(50));
    }

    Controls::last_time = now;
}

void Controls::up_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        current_input = InputType::UP;
        k_timer_start(&button_timers[InputType::UP], K_MSEC(50), K_MSEC(50));
    }

    Controls::last_time = now;
}

void Controls::down_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        current_input = InputType::DOWN;
        k_timer_start(&button_timers[InputType::DOWN], K_MSEC(50), K_MSEC(50));
    }

    Controls::last_time = now;
}