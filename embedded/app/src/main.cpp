#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_mayne, LOG_LEVEL_DBG);

// #include "../inc/display.hh"
// #include "../inc/controls.hh"
// #include "../inc/engine.hh"

#include "../inc/connectivity/wifi.hh"

int main(void)
{
	connectivity::Wifi wifi {};
	wifi.init();

	static connectivity::Wifi::credentials_t creds = {
		.ssid     = "Carnival",
		.password = "Thebluesking"
	};

	wifi.connect(creds);
	k_msleep(5000);
	wifi.log_status();
	k_msleep(5000);
	wifi.disconnect();

	// static Controls        controls { };
	// static Display         display  { };
	// static vroom::Engine   engine   { &controls, &display };

	while (1)
	{

		// engine.process();
		k_msleep(1000);
	}
	return 0;
}
