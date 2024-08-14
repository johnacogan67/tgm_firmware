/*
 * Copyright (c) 2024 WeeGee bv
 */

#include <zephyr/kernel.h>

#include <app/drivers/maxm86161.h>

#include <app_version.h>

#include "ble.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
	int err;

	printk("TGM Application %s\n", APP_VERSION_STRING);

	err = ble_init();
	if (err)
	{
		LOG_ERR("ble_init() returned %d", err);
		return err;
	}

	ble_adv_start();

	return 0;
}
