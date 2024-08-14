/*
 * Copyright (c) 2024 WeeGee bv
 */

#ifndef TGM_SERVICE_H_
#define TGM_SERVICE_H_

/**@file
 * @defgroup tgm_service TGM Service implementation
 * @{
 * @brief API for the TGM Bluetooth service.
 */

/**
 * @brief TGM Service 128 bit UUID for the GATT service and its characteristics
 *
 * Generated using guidgenerator.com
 */
#define BT_UUID_TGM_VAL \
    BT_UUID_128_ENCODE(0x3a0ff000, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_PPG_VAL \
    BT_UUID_128_ENCODE(0x3a0ff001, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_ACC_VAL \
    BT_UUID_128_ENCODE(0x3a0ff002, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_TEMP_VAL \
    BT_UUID_128_ENCODE(0x3a0ff003, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_BAT_VAL \
    BT_UUID_128_ENCODE(0x3a0ff004, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_UUID_VAL \
    BT_UUID_128_ENCODE(0x3a0ff005, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM_FW_VAL \
    BT_UUID_128_ENCODE(0x3a0ff006, 0x98c4, 0x46b2, 0x94af, 0x1aee0fd4c48e)

#define BT_UUID_TGM BT_UUID_DECLARE_128(BT_UUID_TGM_VAL)
#define BT_UUID_TGM_PPG BT_UUID_DECLARE_128(BT_UUID_TGM_PPG_VAL)
#define BT_UUID_TGM_ACC BT_UUID_DECLARE_128(BT_UUID_TGM_ACC_VAL)
#define BT_UUID_TGM_TEMP BT_UUID_DECLARE_128(BT_UUID_TGM_TEMP_VAL)
#define BT_UUID_TGM_BAT BT_UUID_DECLARE_128(BT_UUID_TGM_BAT_VAL)
#define BT_UUID_TGM_UUID BT_UUID_DECLARE_128(BT_UUID_TGM_UUID_VAL)
#define BT_UUID_TGM_FW BT_UUID_DECLARE_128(BT_UUID_TGM_FW_VAL)

#define CONFIG_PPG_SAMPLES_PER_FRAME 10
#define CONFIG_ACC_SAMPLES_PER_FRAME 10
#define CONFIG_TEMP_SAMPLES_PER_FRAME 10

struct ppg_sample
{
    uint32_t red;
    uint32_t ir;
};

/** @brief PPG Data Struct used by the TGM service to inform the client of new PPG data. */
struct tgm_service_ppg_data_t
{
    /** Frame counter*/
    uint32_t frame_counter;
    /** PPG data. */
    struct ppg_sample ppg_data[CONFIG_PPG_SAMPLES_PER_FRAME];
};

struct acc_sample
{
    int16_t x;
    int16_t y;
    int16_t z;
};
/** @brief Accelerometer Data Struct used by the TGM service to inform the client of new accelerometer data. */
struct tgm_service_acc_data_t
{
    /** Frame counter*/
    uint32_t frame_counter;
    /** ACC data. */
    struct acc_sample acc_data[CONFIG_ACC_SAMPLES_PER_FRAME];
};

struct temp_sample
{
    int16_t temp;
};
/** @brief Temperature Data Struct used by the TGM service to inform the client of new temperature data. */
struct tgm_service_temp_data_t
{
    /** Frame counter*/
    uint32_t frame_counter;
    /** Temperature data. */
    struct temp_sample temp_data[CONFIG_TEMP_SAMPLES_PER_FRAME];
};

/** @brief Callback type for when PPG data is pulled. */
typedef void (*tgm_service_ppg_cb_t)(struct tgm_service_ppg_data_t *ppg_data);

/** @brief Callback type for when accelerometer data is pulled. */
typedef void (*tgm_service_acc_cb_t)(struct tgm_service_acc_data_t *acc_data);

/** @brief Callback type for when temperature data is pulled. */
typedef void (*tgm_service_temp_cb_t)(struct tgm_service_temp_data_t *temp_data);

/** @brief Callback type for when the battery value is pulled. */
typedef int32_t (*tgm_service_bat_cb_t)(void);

/** @brief Callback struct used by the TGM Service. */
struct tgm_service_cb
{
    /** PPG data callback. */
    tgm_service_ppg_cb_t ppg_cb;
    /** Accelerometer data callback. */
    tgm_service_acc_cb_t acc_cb;
    /** Temperature data callback. */
    tgm_service_temp_cb_t temp_cb;
    /** Battery value callback. */
    tgm_service_bat_cb_t bat_cb;
};

/** @brief Initialize the TGM Service.
 *
 * This function registers application callback functions with the TGM Service
 *
 * @param[in] callbacks Struct containing pointers to callback functions
 *			used by the service. This pointer can be NULL
 *			if no callback functions are defined.
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int tgm_service_init(struct tgm_service_cb *callbacks);

/** @brief Notify the client of a PPG data change.
 *
 * This function notifies the connected client device of an update to the PPG
 * data
 *
 * @param[in] ppg_data tgm_service_ppg_data_t struct containing a frame counter and PPG sensor data
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int tgm_service_send_ppg_notify(struct tgm_service_ppg_data_t *ppg_data);

/** @brief Notify the client of an accelerometer data change.
 *
 * This function notifies the connected client device of an update to the accelerometer
 * data
 *
 * @param[in] acc_data tgm_service_acc_data_t struct containing a frame counter and accelerometer sensor data
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int tgm_service_send_ppg_notify(struct tgm_service_ppg_data_t *ppg_data);

/** @brief Notify the client of a temperature data change.
 *
 * This function notifies the connected client device of an update to the temperature
 * data
 *
 * @param[in] temp_data tgm_service_temp_data_t struct containing a frame counter and temperature sensor data
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int tgm_service_send_temp_notify(struct tgm_service_temp_data_t *temp_data);

/** @brief Notify the client of a battery value change.
 *
 * This function notifies the connected client device of an update to the battery
 * value
 *
 * @param[in] battery_value Battery value in mV (uint16)
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int tgm_service_send_battery_notify(int32_t battery_value);

/**
 * @}
 */

#endif /* TGM_SERVICE_H_ */