#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_mayne, LOG_LEVEL_DBG);

#include "../inc/subsystems/display.hh"
#include "../inc/subsystems/controls.hh"
#include "../inc/engine.hh"
#include "../inc/connectivity/wifi.hh"


int main(void)
{
	connectivity::Wifi     wifi     { };
	wifi.init();
	k_msleep(500);

	static Controls        controls { };
	static Display         display  { };
	static vroom::Engine   engine   { &controls, &display, wifi };

	while (1)
	{

		engine.process();
		k_msleep(1);
	}
	return 0;
}
