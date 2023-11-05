#pragma once


#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

namespace connectivity
{

class Bt
{
public:
    Bt()
    {
        int err;

        err = bt_enable(NULL);
        if (err) {
            printk("Bluetooth init failed (err %d)\n", err);
        }

        printk("Bluetooth initialized\n");
    }

    ~Bt();
    static void start_scan();
	static char addr_str[BT_ADDR_LE_STR_LEN];
// private:
    static struct bt_conn *default_conn;

    static void disconnected(struct bt_conn *conn, uint8_t reason);
    static void connected(struct bt_conn *conn, uint8_t err);
    static void device_found(const bt_addr_le_t *addr,
                            int8_t rssi, uint8_t type,
                            struct net_buf_simple *ad);
};

}