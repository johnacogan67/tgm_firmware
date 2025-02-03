/*
 * Copyright (c) 2024 WeeGee bv
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include <app_version.h>

#include "ble.h"
#include "ppg.h"
#include "acc.h"
#include "battery.h"
#include "tgm_service.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define GPIO_NODE DT_NODELABEL(gpio0)
#define SENS_ENABLE_PIN 8

static const struct device *gpio = DEVICE_DT_GET(GPIO_NODE);
static const struct device *temp_sensor = DEVICE_DT_GET(DT_NODELABEL(temp));

static struct k_work_delayable temperature_work;

static void temperature_work_handler(struct k_work *work)
{
	int err;
	struct sensor_value temp_value;
	int16_t centitemp; // Temperature in centi-degrees Celsius

	err = sensor_sample_fetch(temp_sensor);
	if (err)
	{
		LOG_ERR("Failed to fetch temperature sample with error %d", err);
		k_work_reschedule(&temperature_work, K_SECONDS(CONFIG_TEMPERATURE_MEASUREMENT_INTERVAL));
		return;
	}

	err = sensor_channel_get(temp_sensor, SENSOR_CHAN_DIE_TEMP, &temp_value);
	if (err)
	{
		LOG_ERR("Failed to get temperature value with error %d", err);
		k_work_reschedule(&temperature_work, K_SECONDS(CONFIG_TEMPERATURE_MEASUREMENT_INTERVAL));
		return;
	}

	centitemp = temp_value.val1 * 100 + temp_value.val2 / 10000;
	LOG_INF("Temperature: %d.%02d C", centitemp / 100, centitemp % 100);

	tgm_service_send_temp_notify(centitemp);

	k_work_reschedule(&temperature_work, K_SECONDS(CONFIG_TEMPERATURE_MEASUREMENT_INTERVAL));
}

int32_t battery_voltage_read(void)
{
	return battery_get_last_measurement();
}

struct tgm_service_cb tgm_service_callbacks = {
	.bat_cb = battery_voltage_read,
};

int main(void)
{
	int err;

	printk("TGM Application %s\n", APP_VERSION_STRING);

	// Initialize the gpio port
	if (!device_is_ready(gpio))
	{
		LOG_ERR("GPIO is not ready");
		return -1;
	}

	// Initialize the sens_enable pin as output
	err = gpio_pin_configure(gpio, SENS_ENABLE_PIN, GPIO_OUTPUT_HIGH);

	err = ble_init();
	if (err)
	{
		LOG_ERR("ble_init() returned %d", err);
		return err;
	}

	tgm_service_init(&tgm_service_callbacks);

	// Initialize the temperature monitoring
	k_work_init_delayable(&temperature_work, temperature_work_handler);

	ble_adv_start();

	err = battery_init(NULL);
	if (err)
	{
		LOG_ERR("battery_init() returned %d", err);
	}

	// Start the battery measurement before any of the sensors are started
	err = battery_start_measurement();
	if (err)
	{
		LOG_ERR("battery_start_measurement() returned %d", err);
	}

	err = ppg_init();
	if (err)
	{
		LOG_ERR("ppg_init() returned %d", err);
	}

	err = acc_init();
	if (err)
	{
		LOG_ERR("acc_init() returned %d", err);
	}

	// Enable the power for the sensors
	err = gpio_pin_set(gpio, SENS_ENABLE_PIN, 1);
	if (err)
	{
		LOG_ERR("Failed to enable sensor power");
		return err;
	}

	// Wait for the sensor to power up
	k_sleep(K_MSEC(100));

	// Start the sensors
	err = ppg_start();
	if (err)
	{
		LOG_ERR("Failed to start the PPG sensor with error %d", err);
	}

	err = acc_start();
	if (err)
	{
		LOG_ERR("Failed to start the accelerometer sensor with error %d", err);
	}

	k_work_reschedule(&temperature_work, K_NO_WAIT);

	return 0;
}
