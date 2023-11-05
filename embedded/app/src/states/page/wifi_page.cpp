#include "../../../inc/states/page/wifi_page.hh"
#include "../../../inc/credentials.hh"

#include "etl/string.h"
#include "etl/to_string.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wifi_page, LOG_LEVEL_DBG);

extern const lv_font_t lv_font_montserrat_20;
extern const connectivity::Wifi::credentials_t creds;

namespace pages
{

Wifi::~Wifi()
{
    // lv_obj_del(ssid_label);
    // lv_obj_del(rssi_label);
    lv_obj_del(main_screen);
    // lv_obj_clean(lv_scr_act());
}


void Wifi::init_screen()
{
    main_screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
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
    lv_label_set_text(ssid_label,"SSID: NC");
    lv_obj_set_style_text_font(ssid_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_add_flag(ssid_label, LV_OBJ_FLAG_HIDDEN);

    rssi_label = lv_label_create(main_screen);
    lv_obj_set_scrollbar_mode(rssi_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_width( rssi_label, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( rssi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( rssi_label, LV_ALIGN_CENTER );
    lv_label_set_text(rssi_label,"RSSI: NC");
    lv_obj_set_style_text_font(rssi_label, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_add_flag(rssi_label, LV_OBJ_FLAG_HIDDEN);

    conn_spinner = lv_spinner_create(main_screen, 1000, 50);
    lv_obj_set_size(conn_spinner, 100, 100);
    lv_obj_set_align(conn_spinner, LV_ALIGN_CENTER);

    LOG_INF("Inited Wifi");
}


void Wifi::draw()
{
    // if(needs_redrawing())
    // {
    //     gui_has_changed = false;
        lv_task_handler();
    // }
}


void Wifi::show()
{
    set_current_state(StateBase::State::RUNNING);
    lv_scr_load(main_screen);
    lv_task_handler();
}


void Wifi::draw_wifi_stats(const conn::Wifi::status_t& wifi_status)
{
    lv_obj_clear_flag(ssid_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(rssi_label, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(ssid_label, "SSID: %s", wifi_status.ssid.c_str());
    lv_label_set_text_fmt(rssi_label, "RSSI: %d", wifi_status.rssi);
}


/* Button handlers */
void Wifi::handle_a_button()
{
    wifi_conn.connect(creds);
    auto status = wifi_conn.get_status();
    draw_wifi_stats(status);
    lv_obj_add_flag(conn_spinner, LV_OBJ_FLAG_HIDDEN);
    gui_has_changed = true;
}


void Wifi::handle_b_button()
{
    LOG_WRN("Pressed b button");
    wifi_conn.disconnect();
    connectivity::Wifi::status_t status = { 0 };
    draw_wifi_stats(status);
    set_current_state(StateBase::State::READY_TO_CLOSE);
}


void Wifi::handle_up_button()
{
    LOG_WRN("Pressed up button");
}


void Wifi::handle_down_button()
{
    LOG_WRN("Pressed down button");
}


void Wifi::handle_left_button()
{


}


void Wifi::handle_right_button()
{


}


}