#include "../inc/controls.hh"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(controls, LOG_LEVEL_DBG);

struct gpio_dt_spec Controls::a_button = GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios);
struct gpio_dt_spec Controls::b_button = GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios);

etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue {};

uint64_t Controls::last_time = 0;

#define DEBOUNCE_TIMEOUT_MS 100


// static etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> input_queue;



// static struct gpio_callback a_btn_cb_data;
void Controls::a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    if ((now - Controls::last_time) > DEBOUNCE_TIMEOUT_MS)
    {
        Controls::input_queue.push(Controls::InputType::A);
    }
  Controls::last_time = now;
}

// static struct gpio_callback b_btn_cb_data;
// static void b_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
// {
//     // uint64_t now = k_uptime_get();
//     // if ((now - last_time) > DEBOUNCE_TIMEOUT_MS)
//     // {
//         auto i = Controls::InputType::B;
//         Controls::add_to_queue(i);
// //     }
// //   last_time = now;
// }


Controls::Controls()
{
     if (device_is_ready(a_button.port))
    {
        int ret = gpio_pin_configure_dt(&a_button, GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&a_button, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&a_btn_cb_data, this->a_button_pressed , BIT(a_button.pin));
        gpio_add_callback(a_button.port, &a_btn_cb_data);
        // ret = gpio_pin_configure_dt(&b_button, GPIO_INPUT);
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

    // if(!gpio_pin_get_dt(&b_button))
    // {
    //     return Controls::InputType::B;
    // }

    return Controls::InputType::NONE;
}
