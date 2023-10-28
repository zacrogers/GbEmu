#include "../inc/controls.hh"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(controls, LOG_LEVEL_DBG);



etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue { };
Controls::TriggerType           Controls::trigger_mapping[InputType::NUM_INPUTS]  {
    // Controls::TriggerType::ONE_SHOT,
    // Controls::TriggerType::ONE_SHOT,
    Controls::TriggerType::PERIODIC,
    Controls::TriggerType::PERIODIC,
    Controls::TriggerType::PERIODIC,
    Controls::TriggerType::PERIODIC,
    Controls::TriggerType::PERIODIC,
    Controls::TriggerType::PERIODIC
};

struct gpio_callback            Controls::cb_data[InputType::NUM_INPUTS]    { };
struct k_timer                  Controls::button_timers[InputType::NUM_INPUTS]    { };

struct gpio_dt_spec Controls::button[InputType::NUM_INPUTS] {
    GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(up_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(down_button), gpios)
};

uint64_t Controls::last_time = 0;

#define DEBOUNCE_TIMEOUT_MS 150
#define TIMER_RETRIGGER_MS 50


/*
    Button timer handlers
*/
void Controls::timer_handler(Controls::InputType input)
{
    Controls::input_queue.push(input);

    if(!gpio_pin_get_dt(&button[input]))
    {
        k_timer_stop(&button_timers[input]);
    }
}


void Controls::a_button_timer_handler(struct k_timer *dummy)
{
    Controls::timer_handler(InputType::A);
}


void Controls::b_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::B);
}


void Controls::up_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::UP);
}


void Controls::down_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::DOWN);
}


void Controls::clear_queue()
{
}


Controls::Controls()
{
    if (device_is_ready(button[Controls::InputType::A].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::A], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::A], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&cb_data[InputType::A], this->a_button_pressed , BIT(button[Controls::InputType::A].pin));
        gpio_add_callback(button[Controls::InputType::A].port, &cb_data[InputType::A]);
        k_timer_init(&Controls::button_timers[Controls::InputType::A], a_button_timer_handler, NULL);
    }

    if (device_is_ready(button[Controls::InputType::B].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::B], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[Controls::InputType::B], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&cb_data[InputType::B], this->b_button_pressed , BIT(button[Controls::InputType::B].pin));
        gpio_add_callback(button[Controls::InputType::B].port, &cb_data[InputType::B]);
        k_timer_init(&Controls::button_timers[Controls::InputType::B], b_button_timer_handler, NULL);
    }

    if (device_is_ready(button[Controls::InputType::UP].port))
    {
        int ret = gpio_pin_configure_dt(&button[Controls::InputType::UP], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[InputType::UP], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&cb_data[InputType::UP], this->up_button_pressed , BIT(button[InputType::UP].pin));
        gpio_add_callback(button[InputType::UP].port, &cb_data[InputType::UP]);
        k_timer_init(&Controls::button_timers[InputType::UP], up_button_timer_handler, NULL);
    }

    if (device_is_ready(button[Controls::InputType::DOWN].port))
    {
        int ret = gpio_pin_configure_dt(&button[InputType::DOWN], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[InputType::DOWN], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&cb_data[InputType::DOWN], this->down_button_pressed , BIT(button[InputType::DOWN].pin));
        gpio_add_callback(button[Controls::InputType::DOWN].port, &cb_data[InputType::DOWN]);
        k_timer_init(&Controls::button_timers[InputType::DOWN], down_button_timer_handler, NULL);
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
void Controls::handle_button(Controls::InputType input)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        if(Controls::trigger_mapping[input] == Controls::TriggerType::PERIODIC)
        {
            k_timer_start(&button_timers[input],
                        K_MSEC(TIMER_RETRIGGER_MS),
                        K_MSEC(TIMER_RETRIGGER_MS));
        }
        else if(Controls::trigger_mapping[input] == Controls::TriggerType::ONE_SHOT)
        {
            Controls::input_queue.push(input);
        }
    }

    Controls::last_time = now;
}


void Controls::a_button_pressed(const struct device *dev,
                                struct gpio_callback* cb,
                                uint32_t pins)
{
    handle_button(InputType::A);
}

void Controls::b_button_pressed(const struct device *dev,
                                struct gpio_callback* cb,
                                uint32_t pins)
{
    handle_button(InputType::B);

}

void Controls::up_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::UP);
}

void Controls::down_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::DOWN);
}