/*
 * Copyright (c) 2024 WeeGee bv
 */

#ifndef MAXM86161_H
#define MAXM86161_H

#include <zephyr/drivers/i2c.h>

struct ppg_sample
{
    uint32_t red;
    uint32_t ir;
};

/**@file
 * @defgroup maxm86161 MAXM86161 Driver implementation
 * @{
 * @brief Driver for using the MAXM86161. Since this sensor is used in the context of PPG, the API is named ppg_sensor.
 */

/**
 * @brief Start the MAXM86161 sensor
 *
 * @param[in] i2c Pointer to the I2C device
 * @return int 0 on success, negative error code on failure
 */
int ppg_sensor_start(const struct i2c_dt_spec *i2c);

/**
 * @brief Stop the MAXM86161 sensor
 *
 * @param[in] i2c Pointer to the I2C device
 * @return int 0 on success, negative error code on failure
 */
int ppg_sensor_stop(const struct i2c_dt_spec *i2c);

/**
 * @brief Get the latest PPG data
 *
 * @param[in] i2c Pointer to the I2C device
 * @param[out] ppg_data Pointer to the PPG data struct
 * @param[out] sample_count Number of samples read
 * @return int 0 on success, negative error code on failure
 */
int ppg_sensor_get_data(const struct i2c_dt_spec *i2c, struct ppg_sample *ppg_data, uint8_t *sample_count);

/**
 * @brief Read a register from the PPG sensor
 *
 * @param[in] i2c Pointer to the I2C device
 * @param[in] reg Register address
 * @param[out] data Pointer to the data read
 * @return int 0 on success, negative error code on failure
 */
int ppg_sensor_read_reg(const struct i2c_dt_spec *i2c, uint8_t reg, uint8_t *data);

/**
 * @brief Write a register to the PPG sensor
 *
 * @param[in] i2c Pointer to the I2C device
 * @param[in] reg Register address
 * @param[in] data Data to write
 * @return int 0 on success, negative error code on failure
 */
int ppg_sensor_write_reg(const struct i2c_dt_spec *i2c, uint8_t reg, uint8_t data);

#endif // MAXM86161_H