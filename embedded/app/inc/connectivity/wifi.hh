#pragma once

#include "etl/string.h"

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>
#include <errno.h>

namespace connectivity
{

class Wifi
{
public:
    static constexpr uint8_t ssid_max_len = 32;
    static constexpr uint8_t pw_max_len   = 32;

    typedef struct {
        etl::string<ssid_max_len> ssid;
        etl::string<pw_max_len>   password;
    } credentials_t;

    typedef struct {
        bool                      connected;
        int64_t                   last_connected;
        etl::string<ssid_max_len> ssid;
        etl::string<ssid_max_len> band;
        uint8_t                   channel;
        etl::string<ssid_max_len> security_txt;
        int16_t                   rssi;
    } status_t;

public:
    Wifi() {};
    ~Wifi() {};
    void     init       ();
    void     connect    (const credentials_t& creds);
    void     disconnect ();
    void     log_status ();
    status_t get_status ();

    static struct k_sem            wifi_connected;
    static struct k_sem            ipv4_address_obtained;

private:
    static status_t                status;
    struct net_if                  *iface;

    struct net_mgmt_event_callback wifi_cb;
    struct net_mgmt_event_callback ipv4_cb;

    static void handle_wifi_connect_result    (struct net_mgmt_event_callback *cb);
    static void handle_wifi_disconnect_result (struct net_mgmt_event_callback *cb);
    static void handle_ipv4_result            (struct net_if *iface);
    static void wifi_mgmt_event_handler       (struct net_mgmt_event_callback *cb,
                                               uint32_t mgmt_event,
                                               struct net_if *iface);
};

}
