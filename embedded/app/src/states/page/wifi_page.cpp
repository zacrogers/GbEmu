#include "../../../inc/states/wifi_page.hh"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wifi_page, LOG_LEVEL_DBG);

namespace pages
{

extern const lv_font_t lv_font_montserrat_20;


Wifi::~Wifi()
{

}


void Wifi::init_screen()
{
    main_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x272846), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(main_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_align( main_screen, LV_ALIGN_CENTER );
    lv_obj_set_width( main_screen, 160);
    lv_obj_set_height( main_screen, 128);
    lv_obj_set_style_radius(main_screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(main_screen, 0, LV_PART_MAIN);

    ssid_label = lv_label_create(main_screen);
    lv_obj_set_scrollbar_mode(ssid_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( ssid_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ssid_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( ssid_label, LV_ALIGN_TOP_MID );
    lv_label_set_text(ssid_label,"SSID: ");
    lv_obj_set_style_text_font(ssid_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    rssi_label = lv_label_create(main_screen);
    lv_obj_set_scrollbar_mode(rssi_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( rssi_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( rssi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( rssi_label, LV_ALIGN_CENTER );
    lv_label_set_text(rssi_label,"RSSI: ");
    lv_obj_set_style_text_font(rssi_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
}


void Wifi::draw()
{

}


void Wifi::show()
{

}


void Wifi::draw_wifi_stats(const conn::Wifi::status_t& wifi_status)
{

}


/* Button handlers */
void Wifi::handle_a_button()
{


}


void Wifi::handle_b_button()
{


}


void Wifi::handle_up_button()
{


}


void Wifi::handle_down_button()
{


}


void Wifi::handle_left_button()
{


}


void Wifi::handle_right_button()
{


}


}