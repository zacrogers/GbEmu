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


	typedef struct {
		// Location of the entity on the screen
		uint8_t x, y;
		// Rectangular width and weight
		uint8_t w, h;
		// Entities in like groups will collide. 0 = no collision
		uint8_t collision_group;
		uint8_t velocity;
	} entity_t;


	using frame_t = lv_obj_t*;


	// Drawing
	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col);

	// Physics
	bool entities_colliding(const graphics::entity_t& a, const graphics::entity_t& b);
}
