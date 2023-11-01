#include "../inc/display.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(display, LOG_LEVEL_INF);

#include <lvgl.h>


Display::Display()
{
    init_display();
}


Display::~Display()
{
}

void Display::init_display()
{
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device %s not found. Aborting sample.", display_dev->name);

		// return 0;
	}

	LOG_INF("Display sample for %s", display_dev->name);
	display_get_capabilities(display_dev, &capabilities);
    // lv_init();
    lv_task_handler();
	display_blanking_off(display_dev);
}


void Display::draw_frame(graphics::frame_t& frame)
{
	// lv_task_handler();
}