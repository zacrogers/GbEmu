#pragma once

#include "state.hh"
#include "../common.hh"
#include "../subsystems/controls.hh"
#include "../subsystems/display.hh"
#include "../connectivity/wifi.hh"

namespace pages
{
    namespace conn = connectivity;
class Wifi: public StateBase
{
public:
    Wifi(conn::Wifi& wifi_conn): wifi_conn(wifi_conn)
        {
            init_screen();
        }
    ~Wifi() override;

    void     draw           () override;
    void     show           () override;

private:
    void             init_screen();
    void             draw_wifi_stats(const conn::Wifi::status_t& wifi_status);
    /* Button handlers */
    void             handle_a_button     () override;
    void             handle_b_button     () override;
    void             handle_up_button    () override;
    void             handle_down_button  () override;
    void             handle_left_button  () override;
    void             handle_right_button () override;

    lv_obj_t* main_screen;
    lv_obj_t* ssid_label;
    lv_obj_t* band_label;
    lv_obj_t* channel_label;
    lv_obj_t* security_label;
    lv_obj_t* rssi_label;
    lv_obj_t* instructions_label;

    conn::Wifi& wifi_conn;
};
}