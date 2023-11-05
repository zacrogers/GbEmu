#include "../../inc/connectivity/bt.hh"

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(bt, LOG_LEVEL_DBG);


namespace connectivity
{

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = Bt::connected,
    .disconnected = Bt::disconnected,
};

struct bt_conn *Bt::default_conn;

void Bt::device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	static char addr_str[BT_ADDR_LE_STR_LEN];
	int err;

	if (Bt::default_conn) {
		return;
	}

	/* We're only interested in connectable events */
	if (type != BT_GAP_ADV_TYPE_ADV_IND &&
	    type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
		return;
	}

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	LOG_INF("Device found: %s (RSSI %d)\n", addr_str, rssi);

	/* connect only to devices in close proximity */
	if (rssi < -70) {
		return;
	}

	if (bt_le_scan_stop()) {
		return;
	}
    const struct bt_le_conn_param conn_param  = {
        .interval_min = 0x0018,
        .interval_max = 0x0028,
        .latency      = 0,
        .timeout      = 400
    };

    struct bt_conn_le_create_param create_param = { 0 };
    create_param.interval = 0;
    create_param.interval_coded = 0;
    create_param.options = BT_LE_SCAN_OPT_NONE;
    create_param.timeout = 0;
    create_param.window = 0;
    create_param.window_coded = 0;

	err = bt_conn_le_create(addr, &create_param, &conn_param, &Bt::default_conn);
	if (err) {
		LOG_INF("Create conn to %s failed (%d)\n", addr_str, err);
		start_scan();
	}
}


void Bt::start_scan()
{
	int err;

	/* This demo doesn't require active scan */
    const bt_le_scan_param param = {
        .type = BT_LE_SCAN_TYPE_PASSIVE,
        .options = BT_LE_SCAN_OPT_NONE,
        .interval = 0x0060,
        .window = 0x0060,
        .timeout = 0,
        .interval_coded = 0,
        .window_coded = 0
    };

	err = bt_le_scan_start(&param, Bt::device_found);
	if (err) {
		LOG_INF("Scanning failed to start (err %d)\n", err);
		return;
	}

	LOG_INF("Scanning successfully started\n");
}

void Bt::connected(struct bt_conn *conn, uint8_t err)
{
	static char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (err) {
		LOG_INF("Failed to connect to %s (%u)\n", addr, err);

		bt_conn_unref(Bt::default_conn);
		Bt::default_conn = nullptr;

		start_scan();
		return;
	}

	if (conn != Bt::default_conn) {
		return;
	}

	LOG_INF("Connected: %s\n", addr);

	bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
}

void Bt::disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	if (conn != Bt::default_conn) {
		return;
	}

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	LOG_INF("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	bt_conn_unref(Bt::default_conn);
	Bt::default_conn = nullptr;

	start_scan();
}

}
