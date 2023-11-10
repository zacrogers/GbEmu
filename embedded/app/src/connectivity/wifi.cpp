#include "../../inc/connectivity/wifi.hh"

// #include "http_get.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vroom_wifi, LOG_LEVEL_DBG);

namespace connectivity
{

struct k_sem Wifi::wifi_connected{};
struct k_sem Wifi::ipv4_address_obtained{};


void Wifi::init()
{
    iface = net_if_get_default();

    if(iface )
    {
        LOG_INF("Got interface");
    }

	k_sem_init(&Wifi::wifi_connected, 0, 1);
	k_sem_init(&Wifi::ipv4_address_obtained, 0, 1);

    LOG_INF("Sems inited");


    if(!iface)
    {
        LOG_ERR("No interface yo");
   }

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);

    net_mgmt_init_event_callback(&ipv4_cb, wifi_mgmt_event_handler, NET_EVENT_IPV4_ADDR_ADD);

    net_mgmt_add_event_callback(&wifi_cb);
    net_mgmt_add_event_callback(&ipv4_cb);
}



void Wifi::connect(const credentials_t& creds)
{
    struct wifi_connect_req_params wifi_params = {
        .ssid        = (uint8_t*)creds.ssid.c_str(),
        .ssid_length = strlen(creds.ssid.c_str()),
        .psk         = (uint8_t*)creds.password.c_str(),
        .psk_length  = strlen(creds.password.c_str()),
        .band        = WIFI_FREQ_BAND_2_4_GHZ,
        .channel     = WIFI_CHANNEL_ANY,
        .security    = WIFI_SECURITY_TYPE_PSK,
        .mfp         = WIFI_MFP_OPTIONAL,
        .timeout     = SYS_FOREVER_MS
     };

    LOG_INF("Connecting to SSID: %s", wifi_params.ssid);

    if (iface && net_mgmt(NET_REQUEST_WIFI_CONNECT,
                iface, &wifi_params,
                sizeof(struct wifi_connect_req_params)))
    {
        LOG_ERR("WiFi Connection Request Failed");
    }
    k_sem_take(&wifi_connected, K_SECONDS(5));
}


void Wifi::disconnect()
{
    if (iface && net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0))
    {
        LOG_ERR("WiFi Disconnection Request Failed\n");
    }
}


void Wifi::log_status()
{
    struct wifi_iface_status status = {0};

    if (iface && net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status,
                    sizeof(struct wifi_iface_status)))
    {
        LOG_ERR("WiFi Status Request Failed");
    }

    if (status.state >= WIFI_STATE_ASSOCIATED)
    {
        LOG_INF("SSID: %-32s",  status.ssid);
        LOG_INF("Band: %s",     wifi_band_txt(status.band));
        LOG_INF("Channel: %d",  status.channel);
        LOG_INF("Security: %s", wifi_security_txt(status.security));
        LOG_INF("RSSI: %d",     status.rssi);
    }
}


Wifi::status_t Wifi::get_status()
{
    struct wifi_iface_status status = {0};

    if (iface && net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status,
                    sizeof(struct wifi_iface_status)))
    {
        LOG_ERR("WiFi Status Request Failed");
    }

    Wifi::status_t s = { 0 };

    if (status.state >= WIFI_STATE_ASSOCIATED)
    {
        s.connected      = true;
        s.last_connected = 0;
        s.ssid           = status.ssid;
        s.band           = wifi_band_txt(status.band);
        s.channel        = status.channel;
        s.security_txt   = wifi_security_txt(status.security);
        s.rssi           = status.rssi;
    }
    return s;
}


void Wifi::handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
    const struct wifi_status *status = (const struct wifi_status *)cb->info;

    if (status->status)
    {
        LOG_INF("Connection request failed (%d)\n", status->status);
    }
    else
    {
        LOG_INF("Connected\n");
        k_sem_give(&wifi_connected);
    }
}


void Wifi::handle_wifi_disconnect_result(struct net_mgmt_event_callback *cb)
{
    const struct wifi_status *status = (const struct wifi_status *)cb->info;

    if (status->status)
    {
        printk("Disconnection request (%d)\n", status->status);
    }
    else
    {
        printk("Disconnected\n");
        k_sem_take(&wifi_connected, K_NO_WAIT);
    }
}


void Wifi::handle_ipv4_result(struct net_if *iface)
{
    int i = 0;

    for (i = 0; i < NET_IF_MAX_IPV4_ADDR; i++) {

        char buf[NET_IPV4_ADDR_LEN];

        if (iface->config.ip.ipv4->unicast[i].addr_type != NET_ADDR_DHCP) {
            continue;
        }

        // LOG_INF("IPv4 address: %s\n",
        //         net_addr_ntop(AF_INET,
        //                         &iface->config.ip.ipv4->unicast[i].address.in_addr,
        //                         buf, sizeof(buf)));
        // LOG_INF("Subnet: %s\n",
        //         net_addr_ntop(AF_INET,
        //                         &iface->config.ip.ipv4->netmask,
        //                         buf, sizeof(buf)));
        // LOG_INF("Router: %s\n",
        //         net_addr_ntop(AF_INET,
        //                         &iface->config.ip.ipv4->gw,
        //                         buf, sizeof(buf)));
        }

        k_sem_give(&ipv4_address_obtained);
}


void Wifi::wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event,
                                    struct net_if *iface)
{
    switch (mgmt_event)
    {

        case NET_EVENT_WIFI_CONNECT_RESULT:
            handle_wifi_connect_result(cb);
            break;

        case NET_EVENT_WIFI_DISCONNECT_RESULT:
            handle_wifi_disconnect_result(cb);
            break;

        case NET_EVENT_IPV4_ADDR_ADD:
            handle_ipv4_result(iface);
            break;

        default:
            break;
    }
}

}