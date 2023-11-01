#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#include "../inc/display.hh"
#include "../inc/controls.hh"
#include "../inc/engine.hh"


int main(void)
{
	Controls controls { };
	Display  display  { };
	Engine   engine   { &controls, &display };

	while (1)
	{
		engine.process();
		k_msleep(1);
	}
	return 0;
}
