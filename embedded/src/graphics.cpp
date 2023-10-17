#include "../inc/graphics.hh"


namespace graphics
{
	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col)
	{
		lv_draw_rect_dsc_t rect_dsc;
		lv_draw_rect_dsc_init(&rect_dsc);
		rect_dsc.radius = 0;
		rect_dsc.bg_opa = LV_OPA_COVER;
		rect_dsc.bg_color = col;
		rect_dsc.border_width = 0;
		rect_dsc.border_opa = LV_OPA_0;
		lv_canvas_draw_rect((lv_obj_t*)frame, x, y, width, height, &rect_dsc);
	}


	bool entities_colliding(const graphics::entity_t& a, const graphics::entity_t& b)
	{
		int x1_right  = a.x + a.w;
		int y1_bottom = a.y + a.h;
		int x2_right  = b.x + b.w;
		int y2_bottom = b.y + b.h;

		// # Check if the boxes are not overlapping
		if (a.x > x2_right  ||
			b.x > x1_right  ||
			a.y > y2_bottom ||
			b.y > y1_bottom)
			return false;

		return true;
	}

}