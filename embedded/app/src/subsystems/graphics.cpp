#include "../../inc/subsystems/graphics.hh"


namespace graphics
{
	void draw_text(frame_t frame, int x, int y, const char* text)
	{
		static lv_draw_label_dsc_t label_dsc;
		lv_draw_label_dsc_init(&label_dsc);
		label_dsc.color = black;
		lv_canvas_draw_text(frame, x, y, 120, &label_dsc, text);
	}


	void draw_rect(frame_t frame, int x, int y, int width, int height, lv_color_t col)
	{
		static lv_draw_rect_dsc_t rect_dsc;
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

	Button* create_button(lv_obj_t*           parent,
							graphics::size_t  size,
							graphics::pos_t   pos,
							graphics::style_t style,
							lv_align_t        align)
	{
		lv_obj_t*  btn = lv_btn_create(parent);

		lv_obj_set_scrollbar_mode     (btn, LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_width              (btn, size.w);
		lv_obj_set_height             (btn, size.h);
		lv_obj_set_x                  (btn, pos.x);
		lv_obj_set_y                  (btn, pos.y);
		lv_obj_set_align              (btn, align );
		lv_obj_set_style_bg_color     (btn, style.bg_col, LV_PART_MAIN | LV_STATE_DEFAULT );
		lv_obj_set_style_bg_opa       (btn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
		lv_obj_set_style_border_color (btn, style.accent_col, LV_PART_MAIN | LV_STATE_DEFAULT );
		lv_obj_set_style_border_opa   (btn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
		lv_obj_set_style_border_width (btn, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
		lv_obj_set_style_border_side  (btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);

		return (Button*)(btn);
	}

	Rect* create_rect(lv_obj_t* parent, graphics::size_t size, pos_t pos, lv_color_t col)
	{
		lv_obj_t*  rect = lv_obj_create(parent);

		lv_obj_set_scrollbar_mode     (rect, LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_width              (rect, size.w);
		lv_obj_set_height             (rect, size.h);
		lv_obj_set_x                  (rect, pos.x);
		lv_obj_set_y                  (rect, pos.y);
		// lv_obj_set_align              (btn, align);
		// lv_obj_set_style_bg_color     (btn, style.bg_col, LV_PART_MAIN | LV_STATE_DEFAULT );
		// lv_obj_set_style_bg_opa       (btn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
		// lv_obj_set_style_border_color (btn, style.accent_col, LV_PART_MAIN | LV_STATE_DEFAULT );
		// lv_obj_set_style_border_opa   (btn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
		// lv_obj_set_style_border_width (btn, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
		// lv_obj_set_style_border_side  (btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);

		return (Rect*)(rect);
	}
}