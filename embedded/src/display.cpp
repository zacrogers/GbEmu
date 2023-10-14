#include "../inc/display.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(display, LOG_LEVEL_INF);


static uint16_t get_rgb565_color(enum Display::corner corner, uint8_t grey)
{
	uint16_t color = 0;
	uint16_t grey_5bit;

	switch (corner) {
	case Display::corner::TOP_LEFT:
		color = 0xF800u;
		break;
	case Display::corner::TOP_RIGHT:
		color = 0x07E0u;
		break;
	case Display::corner::BOTTOM_RIGHT:
		color = 0x001Fu;
		break;
	case Display::corner::BOTTOM_LEFT:
		grey_5bit = grey & 0x1Fu;
		/* shift the green an extra bit, it has 6 bits */
		color = grey_5bit << 11 | grey_5bit << (5 + 1) | grey_5bit;
		break;
	}
	return 0x8D61;
}

int tile_size = 8;


static void fill_buffer_rgb565(enum Display::corner corner, uint8_t grey, uint8_t *buf, size_t buf_size)
{
	uint16_t color = get_rgb565_color(corner, grey);

	bool t = false;
	int row = 0;
	bool rr = false;

	for (size_t idx = 0; idx < buf_size; idx += 2)
	{
		if((idx % tile_size) == 0)
		{
			t = !t;

			if((++row % (tile_size*30)) == 0)
			{
				rr = !rr;
				row=0;
			}
		}

		if(t)
		{
			if(rr)
			{
				*(buf + idx + 0) = (0x8D61 >> 8) & 0xFFu;
				*(buf + idx + 1) = (0x8D61 >> 0) & 0xFFu;
			}
			else
			{
				*(buf + idx + 0) = (0x07E0u >> 8) & 0xFFu;
				*(buf + idx + 1) = (0x07E0u >> 0) & 0xFFu;

			}
		}
		else
		{
			if(rr)
			{
				*(buf + idx + 0) = (0x7461 >> 8) & 0xFFu;
				*(buf + idx + 1) = (0x7461 >> 0) & 0xFFu;
			}
			else
			{
				*(buf + idx + 0) = (0xF800u >> 8) & 0xFFu;
				*(buf + idx + 1) = (0xF800u >> 0) & 0xFFu;
			}
		}
	}
}


Display::Display(/* args */)
{
    init_display();

	// lv_obj_t *count_label;
	char count_str[11] = {0};

	fill_buffer_rgb565(TOP_LEFT, 0, buf, buf_size);
	x = 0;
	y = 0;
	display_write(display_dev, x, y, &buf_desc, buf);

	display_blanking_off(display_dev);

	// grey_count = 0;
	x = 0;
	y = capabilities.y_resolution - rect_h;

	// int count = 0;
	// static lv_style_t text_style;
	// lv_style_init(&text_style);
	// lv_style_set_text_color(&text_style, lv_color_black());
	// lv_obj_add_style(count_label, &text_style, 0);
	// lv_obj_refresh_style(count_label, LV_PART_ANY, LV_STYLE_PROP_ANY);

	// count_label = lv_label_create(lv_scr_act());
	// lv_obj_align(count_label, LV_ALIGN_CENTER, 0, 0);
	// lv_task_handler();
	// display_blanking_off(display_dev);
}


Display::~Display()
{
}


void Display::draw_frame(frame f)
{

}

void Display::init_display()
{
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

	if (!device_is_ready(display_dev))
	{
		LOG_ERR("Device %s not found. Aborting sample.", display_dev->name);

		// return 0;
	}

	LOG_INF("Display sample for %s", display_dev->name);
	display_get_capabilities(display_dev, &capabilities);

	if (capabilities.screen_info & SCREEN_INFO_MONO_VTILED)
    {
		rect_w = 16;
		rect_h = 8;
	}
    else
    {
		rect_w = 2;
		rect_h = 1;
	}

	h_step = rect_h;
	scale = (capabilities.x_resolution / 8) / rect_h;

	rect_w *= scale;
	rect_h *= scale;

	rect_w = 160;
	rect_h = 128;

	if (capabilities.screen_info & SCREEN_INFO_EPD)
    {
		grey_scale_sleep = 10000;
	}
    else
    {
		grey_scale_sleep = 100;
	}

	buf_size = rect_w * rect_h;

	if (buf_size < (capabilities.x_resolution * h_step))
    {
		buf_size = capabilities.x_resolution * h_step;
	}
	buf_size *= 2;
	// fill_buffer_fnc = fill_buffer_rgb565;

	buf = static_cast<uint8_t*>(k_malloc(buf_size));

	if (buf == NULL)
	{
		LOG_ERR("Could not allocate memory. Aborting sample.");
		// return 0;
	}

    blank_screen();

	buf_desc.pitch = rect_w;
	buf_desc.width = rect_w;
	buf_desc.height = rect_h;
}

void Display::blank_screen()
{
	buf_desc.buf_size = buf_size;
	buf_desc.pitch = capabilities.x_resolution;
	buf_desc.width = capabilities.x_resolution;
	buf_desc.height = h_step;

	for (int idx = 0; idx < capabilities.y_resolution; idx += h_step)
    {
		display_write(display_dev, 0, idx, &buf_desc, buf);
	}
}