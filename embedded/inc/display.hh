#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>


class Display
{
public:
enum corner {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

typedef void (*fill_buffer)(enum corner corner, uint8_t grey,
                                uint8_t *buf, size_t buf_size);

public:
    using frame = int;
    Display(/* args */);
    ~Display();



    void draw_frame(frame);

private:
    void init_display();
    void blank_screen();


	size_t                           x;
	size_t                           y;
	size_t                           rect_w;
	size_t                           rect_h;
	size_t                           h_step;
	size_t                           scale;
	size_t                           grey_count;
	uint8_t                          *buf;
	int32_t                          grey_scale_sleep;

	const struct device              *display_dev;
	struct display_capabilities      capabilities;
	struct display_buffer_descriptor buf_desc;
	size_t                           buf_size = 0;
	fill_buffer                      fill_buffer_fnc = NULL;
};


