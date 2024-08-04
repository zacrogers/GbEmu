#pragma once

#include <lvgl.h>

namespace graphics
{
	using frame_t           = lv_obj_t*;
	using Button            = lv_obj_t*;
	using Label             = lv_obj_t*;
	using Dropdown          = lv_obj_t*;
	using Rect              = lv_obj_t*;
	using collision_group_t = uint8_t;

	static lv_color_t red    = lv_color_make(255, 0, 0);
	static lv_color_t blue   = lv_color_hex(0x4034eb);
	static lv_color_t green  = lv_color_hex(0x34eb37);
	static lv_color_t purple = lv_color_make(0, 255, 255);
	static lv_color_t black  = lv_color_make(0, 0, 0);


	typedef struct { int        x, y; } pos_t;
	typedef struct { lv_coord_t w, h; } size_t;

	typedef struct {
		// Location of the entity on the screen
		pos_t pos;
		// Rectangular width and weight
		uint8_t w, h;
		// Entities in like groups will collide. 0 = no collision
		collision_group_t collision_group;
		// X, and Y velocity
		pos_t velocity;
		lv_obj_t* obj;
	} entity_t;

	typedef struct {
		lv_color_t bg_col;
		lv_color_t fg_col;
		lv_color_t text_col;
		lv_color_t accent_col;
	} style_t;

	// Screen objects

	Button* create_button(lv_obj_t*           parent,
							graphics::size_t  size,
							graphics::pos_t   pos,
							graphics::style_t style,
							lv_align_t        align = LV_ALIGN_CENTER);

	// Screen objects
	Label* create_label(lv_obj_t*         parent,
				       	graphics::size_t  size,
				       	graphics::pos_t   pos,
				       	graphics::style_t style,
				       	lv_align_t        align = LV_ALIGN_CENTER);


	// Drawing
	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col);
	void draw_rect(frame_t frame, pos_t pos, int width, int height, lv_color_t col=green);
	Rect* create_rect(lv_obj_t* parent, graphics::size_t size, pos_t pos, lv_color_t col=green);

	void draw_text(frame_t frame, int x, int y, const char* text);
	void draw_text(frame_t frame, pos_t pos, const char* text);

	// Physics
	bool entities_colliding(const graphics::entity_t& a, const graphics::entity_t& b);
}
