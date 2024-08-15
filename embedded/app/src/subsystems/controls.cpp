#include "../../inc/subsystems/controls.hh"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(controls, LOG_LEVEL_DBG);


struct gpio_callback            Controls::cb_data[InputType::NUM_INPUTS]          { };
struct k_timer                  Controls::button_timers[InputType::NUM_INPUTS]    { };
uint64_t                        Controls::last_time[InputType::NUM_INPUTS]        { };
etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue { };

Controls::TriggerType           Controls::trigger_mapping[InputType::NUM_INPUTS]  {
    [InputType::A]      = Controls::TriggerType::ONE_SHOT,
    [InputType::B]      = Controls::TriggerType::ONE_SHOT,
    [InputType::START]  = Controls::TriggerType::ONE_SHOT,
    [InputType::SELECT] = Controls::TriggerType::ONE_SHOT,
    [InputType::UP]     = Controls::TriggerType::PERIODIC,
    [InputType::DOWN]   = Controls::TriggerType::PERIODIC,
    [InputType::LEFT]   = Controls::TriggerType::ONE_SHOT,
    [InputType::RIGHT]  = Controls::TriggerType::ONE_SHOT
};


struct gpio_dt_spec Controls::button[InputType::NUM_INPUTS] {
    GPIO_DT_SPEC_GET(DT_NODELABEL(a_button),      gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(b_button),      gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(start_button),  gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(select_button), gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(up_button),     gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(down_button),   gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(left_button),   gpios),
    GPIO_DT_SPEC_GET(DT_NODELABEL(right_button),  gpios)
};


#define DEBOUNCE_TIMEOUT_MS 100
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


void Controls::left_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::LEFT);
}


void Controls::right_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::RIGHT);
}


void Controls::start_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::START);
}


void Controls::select_button_timer_handler(struct k_timer *dummy)
{
   Controls::timer_handler(InputType::SELECT);
}


void Controls::clear_queue()
{
}


Controls::Controls()
{
    // Init buttons
    bool a      = init_gpio_pin(InputType::A,      a_button_timer_handler,      a_button_pressed);
    bool b      = init_gpio_pin(InputType::B,      b_button_timer_handler,      b_button_pressed);
    bool up     = init_gpio_pin(InputType::UP,     up_button_timer_handler,     up_button_pressed);
    bool down   = init_gpio_pin(InputType::DOWN,   down_button_timer_handler,   down_button_pressed);
    bool left   = init_gpio_pin(InputType::LEFT,   left_button_timer_handler,   left_button_pressed);
    bool right  = init_gpio_pin(InputType::RIGHT,  right_button_timer_handler,  right_button_pressed);
    bool start  = init_gpio_pin(InputType::START,  start_button_timer_handler,  start_button_pressed);
    bool select = init_gpio_pin(InputType::SELECT, select_button_timer_handler, select_button_pressed);

    LOG_INF("|   Button Status   |\n |-------------------|\n | A | B | UP | DOWN | LEFT | RIGHT |\n| %d | %d | %d  |  %d   | %d | %d |",
            //   START | SELECT |\n \
            //   %d  |  %d   |",
             a, b,
             up, down,
             left, right
            // start, select
    );
}


bool Controls::init_gpio_pin(InputType input, k_timer_expiry_t timer_fn, gpio_callback_handler_t gpio_cb)
{
    if (device_is_ready(button[input].port))
    {
        int ret = gpio_pin_configure_dt(&button[input], GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&button[input], GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&cb_data[input], gpio_cb , BIT(button[input].pin));
        ret = gpio_add_callback(button[input].port, &cb_data[input]);
        k_timer_init(&Controls::button_timers[input], timer_fn, NULL);

        return ret == 0;
    }
    return false;
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
    if ((now - Controls::last_time[input]) > DEBOUNCE_TIMEOUT_MS)
    {
        if(Controls::trigger_mapping[input] == Controls::TriggerType::PERIODIC)
        {
            k_timer_start(&button_timers[input],
                        K_MSEC(TIMER_RETRIGGER_MS),
                        K_MSEC(TIMER_RETRIGGER_MS));
        }
        else if(Controls::trigger_mapping[input] == Controls::TriggerType::ONE_SHOT)
        {
            LOG_DBG("I: %d", input);
            Controls::input_queue.push(input);
        }
    }

    Controls::last_time[input] = now;
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

void Controls::left_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::LEFT);
}

void Controls::right_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::RIGHT);
}

void Controls::start_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::START);
}

void Controls::select_button_pressed(const struct device *dev,
                                    struct gpio_callback* cb,
                                    uint32_t pins)
{
    handle_button(InputType::SELECT);
}
