#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_mayne, LOG_LEVEL_DBG);

#include "../inc/subsystems/display.hh"
#include "../inc/subsystems/controls.hh"
#include "../inc/engine.hh"
#include "../inc/connectivity/wifi.hh"

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
	     "Console device is not ACM CDC UART device");



void process_engine(lv_timer_t * timer)
{
	vroom::Engine* e = (vroom::Engine*)timer->user_data;
	e->process();
}
int main(void)
{
	int ret = usb_enable(NULL);
	if (ret != 0) {
		LOG_ERR("Failed to enable USB");
		return 0;
	}

	static Controls        controls { };
	static Display         display  { };
	static vroom::Engine   engine   { &controls, &display};

	lv_timer_create(process_engine, 30, &engine);

	while (1)
	{
		// engine.process();
		lv_timer_handler();
		k_msleep(30);
	}
	return 0;
}
