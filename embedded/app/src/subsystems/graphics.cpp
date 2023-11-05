#include "../../inc/subsystems/graphics.hh"


namespace graphics
{
	void draw_text(frame_t frame, int x, int y, const char* text)
	{
		lv_draw_label_dsc_t label_dsc;
		lv_draw_label_dsc_init(&label_dsc);
		label_dsc.color = black;
		lv_canvas_draw_text(frame, x, y, 120, &label_dsc, text);
	}


	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col)
	{
		lv_draw_rect_dsc_t rect_dsc;
		lv_draw_rect_dsc_init(&rect_dsc);
		rect_dsc.radius = 5;
		rect_dsc.bg_opa = LV_OPA_COVER;
		rect_dsc.bg_color = col;
		rect_dsc.border_width = 0;
		rect_dsc.border_opa = LV_OPA_0;
		lv_canvas_draw_rect((lv_obj_t*)frame, x, y, width, height, &rect_dsc);
	}


	void draw_rect(frame_t frame, pos_t pos, int width, int height, lv_color_t col)
	{
		draw_rect(frame, pos.x, pos.y, width, height, col);
	}


	bool entities_colliding(const graphics::entity_t& a, const graphics::entity_t& b)
	{
		int x1_right  = a.pos.x + a.w;
		int y1_bottom = a.pos.y + a.h;
		int x2_right  = b.pos.x + b.w;
		int y2_bottom = b.pos.y + b.h;

		// # Check if the boxes are not overlapping
		if (a.pos.x > x2_right  ||
			b.pos.x > x1_right  ||
			a.pos.y > y2_bottom ||
			b.pos.y > y1_bottom)
			return false;

		return true;
	}

}