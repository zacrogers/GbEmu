#pragma once

#include "../state.hh"
#include "../../common.hh"
#include "../../subsystems/controls.hh"
#include "../../subsystems/display.hh"
#include "../../connectivity/wifi.hh"

#include <lvgl.h>

namespace pages
{
namespace conn = connectivity;

using rssi_observable    = vroom::logging::sensor_observable<conn::Wifi::rssi_t>;
using channel_observable = vroom::logging::sensor_observable<conn::Wifi::channel_t>;

class Wifi:
    public StateBase,
    public rssi_observable,
    public channel_observable
{
public:
    Wifi(conn::Wifi& wifi_conn): wifi_conn(wifi_conn)
    {
        init_screen();
        // wifi_conn.init();
        rssi_observable::add_observer(rssi_logger);
        channel_observable::add_observer(channel_logger);
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
    lv_obj_t* conn_spinner;

    conn::Wifi&                wifi_conn;
    // conn::Wifi::credentials_t& creds;
    vroom::logging::DataLogger<conn::Wifi::channel_t> channel_logger { "Channel", nullptr };
    vroom::logging::DataLogger<conn::Wifi::rssi_t>    rssi_logger    { "Rssi", nullptr };

    static constexpr uint8_t num_subpages { 1 };
};
}