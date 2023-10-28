#include "../inc/controls.hh"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(controls, LOG_LEVEL_DBG);

struct gpio_dt_spec Controls::a_button = GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios);
struct gpio_dt_spec Controls::b_button = GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios);

etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue {};

uint64_t Controls::last_time = 0;

#define DEBOUNCE_TIMEOUT_MS 150


void my_work_handler(struct k_work *work)
{
    Controls::input_queue.push(Controls::InputType::B);
}

K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy);

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work);
    if(!gpio_pin_get_dt(&Controls::a_button))
    {
        k_timer_stop(&my_timer);
    }
}
/* start periodic timer that expires once every second */
// k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));



void Controls::clear_queue()
{
}

void Controls::a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        k_timer_start(&my_timer, K_MSEC(50), K_MSEC(50));


        // while(!gpio_pin_get_dt(&Controls::a_button))
        // {
        //     LOG_DBG("pressed");
        //     Controls::input_queue.push(Controls::InputType::A);
        // }
    }

    Controls::last_time = now;
}

void Controls::b_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        Controls::input_queue.push(Controls::InputType::B);
    }

    Controls::last_time = now;
}


Controls::Controls()
{
    if (device_is_ready(a_button.port))
    {
        int ret = gpio_pin_configure_dt(&a_button, GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&a_button, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&a_btn_cb_data, this->a_button_pressed , BIT(a_button.pin));
        gpio_add_callback(a_button.port, &a_btn_cb_data);
    }

    if (device_is_ready(b_button.port))
    {
        int ret = gpio_pin_configure_dt(&b_button, GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&b_button, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&b_btn_cb_data, this->b_button_pressed , BIT(b_button.pin));
        gpio_add_callback(b_button.port, &b_btn_cb_data);
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

    // if(gpio_pin_get_dt(&a_button))
    // {
    //     LOG_DBG("pressed");
    //     return Controls::InputType::A;
    // }

    return Controls::InputType::NONE;
}
