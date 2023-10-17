/*
 * Copyright (c) 2019 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * Based on ST7789V sample:
 * Copyright (c) 2019 Marc Reilly
 *
 * SPDX-License-Identifier: Apache-2.0
 */


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
	Engine   engine   = Engine( &controls, &display );

	while (1)
	{
		engine.process();
		// k_msleep(20);
	}
	return 0;
}
