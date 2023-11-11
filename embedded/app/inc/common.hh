#pragma once

enum class GameType { PONG, SNAKE, GAMEBOY, BT_CONTROLLER, SERIAL_MONITOR, WIFI_MANAGER };

#include <zephyr/kernel.h>

#include "etl/circular_buffer.h"
#include "etl/string.h"
#include "etl/delegate.h"


namespace vroom
{
namespace logging
{

using process_cb = etl::delegate<void(void)>;

template <class T>
class Sensor
{
public:
    virtual T take_reading() = 0;
};


template <typename T,
        const uint8_t QUEUE_SIZE=10,
        const uint16_t PERIOD_S=5>
class DataLogger
{
public:
    static constexpr uint8_t label_max_len { 10 };
    static constexpr uint8_t n_timers      { 1 };

    using label_t = etl::string<label_max_len>;

    DataLogger(label_t label, Sensor<T>& sensor): label(label)
    {
        cb->template set<Sensor<T>, &Sensor<T>::take_reading>(sensor);
        k_timer_init(&timer, timer_handler, NULL);
    }
    ~DataLogger() {};

    void                      start        ();
    // {
    //     k_timer_start(&timer, K_SECONDS(PERIOD_S), K_SECONDS(PERIOD_S));
    // }
    void                      stop         ();
    void                      add_new      (T val);
    etl::circular_buffer
        <T, QUEUE_SIZE>       get_queue    ();
    T                         get_last_val ();
    void                      clear_queue  ();

private:
    using sensor_cb = etl::delegate<T(void)>;

    bool                                 loop_back        { true };
    T                                    last_value       { };
    etl::string<label_max_len>           label            { };
    struct k_timer                       timer            { };
    sensor_cb                            cb               {};
    process_cb                           proc_cb          { };
    int32_t                              started_at       { 0 };
    etl::circular_buffer<T, QUEUE_SIZE>  queue            { };

    void timer_handler    (struct k_timer *dummy);
};

} /* namespace logging */
} /* namespace vroom */