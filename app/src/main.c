/*
 * Copyright (c) 2024 WeeGee bv
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include <app_version.h>

#include "ble.h"
#include "ppg.h"
#include "acc.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define GPIO_NODE DT_NODELABEL(gpio0)
#define SENS_ENABLE_PIN 8

static const struct device *gpio = DEVICE_DT_GET(GPIO_NODE);

int main(void)
{
	int err;

	printk("TGM Application %s\n", APP_VERSION_STRING);

	// Initialize the gpio port
	if (!device_is_ready(gpio)){
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

	ble_adv_start();

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

	return 0;
}
