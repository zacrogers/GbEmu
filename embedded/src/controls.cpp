#include "../inc/controls.hh"
#include <zephyr/kernel.h>


static struct gpio_callback a_btn_cb_data;

struct gpio_dt_spec Controls::a_button = GPIO_DT_SPEC_GET(DT_NODELABEL(a_button), gpios);
struct gpio_dt_spec Controls::b_button = GPIO_DT_SPEC_GET(DT_NODELABEL(b_button), gpios);
etl::queue<Controls::InputType, Controls::INPUT_QUEUE_SIZE> Controls::input_queue {};



static void a_button_pressed(const struct device *dev, struct gpio_callback* cb, uint32_t pins)
{
    if(!gpio_pin_get_dt(&Controls::a_button))
    {
        auto i = Controls::InputType::A;
        Controls::add_to_queue(i);
    }
}



Controls::Controls()
{
    if (device_is_ready(a_button.port))
    {
        int ret = gpio_pin_configure_dt(&a_button, GPIO_INPUT);

        ret = gpio_pin_interrupt_configure_dt(&a_button, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&a_btn_cb_data, a_button_pressed , BIT(a_button.pin));
        gpio_add_callback(a_button.port, &a_btn_cb_data);
        // ret = gpio_pin_configure_dt(&b_button, GPIO_INPUT);
    }
}



Controls::~Controls()
{

}


const Controls::InputType Controls::get_last_pressed()
{
    if(!Controls::input_queue.empty())
    {
        auto last_pressed = Controls::input_queue.front();
        Controls::input_queue.pop();
        return last_pressed;
    }

    return Controls::InputType::NONE;
}
