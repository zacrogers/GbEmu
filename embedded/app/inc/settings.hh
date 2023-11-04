#pragma once

#include "version.hh"
#include "connectivity/wifi.hh"

#include "etl/string.h"

namespace settings
{

namespace conn = connectivity;

namespace bt
{

}


namespace device
{

typedef struct
{
    uint16_t milli_volts;
    int64_t  timestamp;
} __attribute__((__packed__)) batt_reading_t;

constexpr const uint8_t username_max_len = 10;
constexpr const uint8_t label_max_len    = 30;

typedef struct {
    uint16_t minutes_played;
    uint16_t times_played;
} game_stats_t;


typedef struct {
    uint8_t                       screen_brightness;
    int64_t                       time_played;
    bool                          power_saving_active;
    bool                          battery_monitor_active;
    bool                          wifi_active;
    bool                          bt_active;
    etl::string<username_max_len> username;
} __attribute__((__packed__)) ctx_t;

typedef enum
{
    screen_brightness,
    time_played,
    power_saving_active,
    battery_monitor_active,
    wifi_active,
    bt_active,
    username
} ctx_fields_e;


static const etl::string<10> ctx_labels[] = {
    "screen_brightness",
    "time_played",
    "power_saving_active",
    "battery_monitor_active",
    "wifi_active",
    "bt_active",
    "username"
};

}

class Core
{
public:
    Core();
    ~Core();

    static conn::Wifi::credentials_t load_wifi_creds        ();
    static bool                      save_wifi_creds        (conn::Wifi::credentials_t creds);

    static uint8_t                   load_screen_brightness ();
    static bool                      save_screen_brightness (uint8_t val);

    static device::ctx_t             load_device_ctx        ();
    static bool                      save_device_ctx        (device::ctx_t);
};

}