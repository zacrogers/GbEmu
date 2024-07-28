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
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

static const char *now_str(void)
{
	static char buf[16]; /* ...HH:MM:SS.MMM */
	uint32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
		 h, min, s, ms);
	return buf;
}
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


static int process_mpu6050(const struct device *dev)
{
	struct sensor_value temperature;
	struct sensor_value accel[3];
	struct sensor_value gyro[3];
	int rc = sensor_sample_fetch(dev);

	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ,
					accel);
	}
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ,
					gyro);
	}
	if (rc == 0) {
		rc = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP,
					&temperature);
	}
	if (rc == 0) {
		printf("[%s]:%g Cel\n"
		       "  accel %f %f %f m/s/s\n"
		       "  gyro  %f %f %f rad/s\n",
		       now_str(),
		       sensor_value_to_double(&temperature),
		       sensor_value_to_double(&accel[0]),
		       sensor_value_to_double(&accel[1]),
		       sensor_value_to_double(&accel[2]),
		       sensor_value_to_double(&gyro[0]),
		       sensor_value_to_double(&gyro[1]),
		       sensor_value_to_double(&gyro[2]));
	} else {
		printf("sample fetch/get failed: %d\n", rc);
	}

	return rc;
}

#ifdef CONFIG_MPU6050_TRIGGER
static struct sensor_trigger trigger;

static void handle_mpu6050_drdy(const struct device *dev,
				const struct sensor_trigger *trig)
{
	int rc = process_mpu6050(dev);

	if (rc != 0) {
		printf("cancelling trigger due to failure: %d\n", rc);
		(void)sensor_trigger_set(dev, trig, NULL);
		return;
	}
}
#endif /* CONFIG_MPU6050_TRIGGER */


int main(void)
{
	const struct device *const dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
	uint32_t dtr = 0;

	if (usb_enable(NULL)) {
		return 0;
	}
	const struct pwm_dt_spec buzzer_dev = PWM_DT_SPEC_GET(DT_ALIAS(buzzerpwm));

	if (!pwm_is_ready_dt(&buzzer_dev))
	{
		printk("Error: PWM device not ready.\n");
		return 0;
	}

	while(1)
	{
		play_frequency(&buzzer_dev, 1000, 500); // Play 1kHz for 500ms
		k_msleep(500);                     // Pause for 500ms
		play_frequency(&buzzer_dev, 0, 500); // Play 1kHz for 500ms
		k_msleep(500);                     // Pause for 500ms
	}

    // play_frequency(&buzzer_dev, 1500, 500); // Play 1.5kHz for 500ms
    // k_msleep(500);                     // Pause for 500ms

    // play_frequency(&buzzer_dev, 2000, 500); // Play 2kHz for 500ms
    // k_msleep(500);

	// // connectivity::Wifi     wifi     { };
	// // wifi.init();
	// k_msleep(500);

// 	const struct device *const mpu6050 = DEVICE_DT_GET_ONE(invensense_mpu6050);

// 	if (!device_is_ready(mpu6050)) {
// 		printf("Device %s is not ready\n", mpu6050->name);
// 		return 0;
// 	}

// #ifdef CONFIG_MPU6050_TRIGGER
// 	trigger = (struct sensor_trigger) {
// 		.type = SENSOR_TRIG_DATA_READY,
// 		.chan = SENSOR_CHAN_ALL,
// 	};
// 	if (sensor_trigger_set(mpu6050, &trigger,
// 			       handle_mpu6050_drdy) < 0) {
// 		printf("Cannot configure trigger\n");
// 		return 0;
// 	}
// 	printk("Configured for triggered sampling.\n");
// #endif

	// while (!IS_ENABLED(CONFIG_MPU6050_TRIGGER)) {
	// 	int rc = process_mpu6050(mpu6050);

	// 	if (rc != 0) {
	// 		break;
	// 	}
	// 	k_sleep(K_SECONDS(2));
	// }


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
