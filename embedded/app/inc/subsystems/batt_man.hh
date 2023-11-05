#pragma once

#include <zephyr/kernel.h>

#define CONFIG_BATTMAN_USE_UTILITY_BELT
#ifdef CONFIG_BATTMAN_USE_UTILITY_BELT
#include "etl/queue.h"
#endif /* CONFIG_BATTMAN_USE_UTILITY_BELT */


namespace subsys { namespace nananananananana {

template<int queue_size>
class BattMan
{
public:
    using millivolts_t = uint16_t;
    using interval_t   = uint16_t;

    typedef struct {
        interval_t readings_interval;
    } metadata_t;

    BattMan();
    ~BattMan();

    millivolts_t take_reading();

#ifdef CONFIG_BATTMAN_USE_UTILITY_BELT

    const etl::queue<millivolts_t, queue_size> get_queue();
    const interval_t                           estimate_life();
    const metadata_t                           get_metadata();

    inline static const bool                   use_grappling_hook();
    inline static const bool                   use_smoke_bomb();

#endif /* CONFIG_BATTMAN_USE_UTILITY_BELT */

private:
    void init();

#ifdef CONFIG_BATTMAN_USE_UTILITY_BELT

    static etl::queue<millivolts_t, queue_size> readings_queue;
    static struct k_timer                       readings_timer;
    metadata_t                                  metadata;

    static void timer_handler();

#endif /* CONFIG_BATTMAN_USE_UTILITY_BELT */
};


} /* namespace subsys */
} /* namespace nananananananana */
