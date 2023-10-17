#pragma once

#include <lvgl.h>

namespace graphics
{
	typedef enum
	{
		BLACK = 0x0000,
		RED   = 0xf800,
		GREEN = 0x07e0,
		BLUE  = 0x001f
	} colour_rgb565;

	static lv_color_t blue = lv_color_make(255, 0, 0);
	static lv_color_t red = lv_color_make(0, 255, 0);
	static lv_color_t green = lv_color_make(0, 0, 255);


	using frame_t = lv_obj_t*;

	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col);
}
