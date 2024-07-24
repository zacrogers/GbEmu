#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main_mayne, LOG_LEVEL_DBG);

#include "../inc/subsystems/display.hh"
#include "../inc/subsystems/controls.hh"
#include "../inc/engine.hh"
#include "../inc/connectivity/wifi.hh"

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
	     "Console device is not ACM CDC UART device");


#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>

// #define PWM_NODE DT_NODELABEL(pwm)
#define PWM_CHANNEL 0
#define PERIOD_USEC (USEC_PER_SEC / 1000U)


void play_frequency(const struct pwm_dt_spec *buzzer_dev, uint32_t freq_hz, uint32_t duration_ms)
{
    uint32_t period = USEC_PER_SEC / freq_hz;
    uint32_t pulse = period / 2; // 50% duty cycle
	pwm_set_dt(buzzer_dev, PWM_HZ(freq_hz), PWM_HZ(freq_hz) / 2);
    // int ret = pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, period, pulse, 0);
    // if (ret) {
    //     printk("Error %d: failed to set pulse width\n", ret);
    //     return;
    // }

    k_msleep(duration_ms);
	pwm_set_dt(buzzer_dev, PWM_HZ(1), PWM_HZ(1));

    // // Stop PWM
    // pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, 0, 0, 0);
}

static const struct pwm_dt_spec buzzer_dev = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));


int main(void)
{
	const struct device *const dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
	uint32_t dtr = 0;

	if (usb_enable(NULL)) {
		return 0;
	}
	// const struct pwm_dt_spec buzzer_dev = PWM_DT_SPEC_GET(DT_ALIAS(buzzerpwm));

	if (!pwm_is_ready_dt(&buzzer_dev))
	{
		printk("Error: PWM device %s not ready.\n");
		return 0;
	}

	play_frequency(&buzzer_dev, 1000, 500); // Play 1kHz for 500ms
    k_msleep(500);                     // Pause for 500ms

    // play_frequency(&buzzer_dev, 1500, 500); // Play 1.5kHz for 500ms
    // k_msleep(500);                     // Pause for 500ms

    // play_frequency(&buzzer_dev, 2000, 500); // Play 2kHz for 500ms
    // k_msleep(500);

	// connectivity::Wifi     wifi     { };
	// wifi.init();
	k_msleep(500);

	static Controls        controls { };
	static Display         display  { };
	static vroom::Engine   engine   { &controls, &display};

	while (1)
	{
		engine.process();
		k_msleep(10);
	}
	return 0;
}
