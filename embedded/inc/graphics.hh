#pragma once

#include <lvgl.h>

namespace graphics
{
	using frame_t           = lv_obj_t*;
	using collision_group_t = uint8_t;
	typedef enum
	{
		BLACK = 0x0000,
		RED   = 0xf800,
		GREEN = 0x07e0,
		BLUE  = 0x001f
	} colour_rgb565;

	static lv_color_t blue   = lv_color_make(255, 0, 0);
	static lv_color_t red    = lv_color_make(0, 255, 0);
	static lv_color_t green  = lv_color_make(0, 0, 255);
	static lv_color_t purple = lv_color_make(255, 255, 0);
	static lv_color_t black  = lv_color_make(0, 0, 0);


	typedef struct { uint8_t x, y; } pos_t;

	typedef struct {
		// Location of the entity on the screen
		pos_t pos;
		// Rectangular width and weight
		uint8_t w, h;
		// Entities in like groups will collide. 0 = no collision
		collision_group_t collision_group;
		// X, and Y velocity
		pos_t velocity;
	} entity_t;


	// Drawing
	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col);
	void draw_rect(frame_t frame, pos_t pos, int width, int height, lv_color_t col=green);

	void draw_text(frame_t frame, int x, int y, const char* text);
	void draw_text(frame_t frame, pos_t pos, const char* text);

	// Physics
	bool entities_colliding(const graphics::entity_t& a, const graphics::entity_t& b);
}
