#pragma once

enum class GameType { PONG, SNAKE, GAMEBOY, BT_CONTROLLER, SERIAL_MONITOR, WIFI_MANAGER };

#include "etl/circular_buffer.h"
#include "etl/string.h"
#include "etl/delegate.h"
#include "etl/observer.h"


namespace vroom
{
namespace logging
{

using process_cb = etl::delegate<void(void)>;


template <class T>
class Backend
{
public:
    virtual void process_log (T val) = 0;
};


template<uint8_t str_len>
struct loggable_t {
    virtual etl::string<str_len> to_string() = 0;
};


template <class T>
using sensor_observer = etl::observer<T>;
template <class T>
using sensor_observable = etl::observable<sensor_observer<T>, 1>;


template <class T, const uint8_t QUEUE_SIZE=10, const uint16_t PERIOD_S=5>
class DataLogger: public sensor_observer<T>
{
public:
    using log_buffer_t = etl::circular_buffer<T, QUEUE_SIZE>;
    static constexpr uint8_t label_max_len { 10 };

    DataLogger(etl::string<label_max_len> label, Backend<T>* backend)
        : label(label), p_backend(backend)
    {

    }
    ~DataLogger() {};

    /* Sensor observer */
    void notification (T val) override
    {
        queue.push(val);

        if(p_backend)
        {
            p_backend->process_log(val);
        }
    }

    log_buffer_t  get_queue    ();
    T             get_last_val ();
    void          clear_queue  ();

private:
    etl::string<label_max_len>  label      { };
    process_cb                  proc_cb    { };
    log_buffer_t                queue      { };
    Backend<T>*                 p_backend  { nullptr };
};

} /* namespace logging */
} /* namespace vroom */