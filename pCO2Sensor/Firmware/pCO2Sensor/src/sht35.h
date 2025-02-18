/** @file sht35.h
 *  @brief Sensirion SHT-35 Digital Relative Humidity and Temparture Sensor (I2C)
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date July 6, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note Driver interface for the Sensirion SHT-35 RH/T Digital I2C Sensor
 *  @note Requires pCO2 BSP and TI Driverlib
 *  @note This driver largely leveraged from Sensirion Example Code found
 *  @note https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/13_Sample_Codes_Software/Humidity_Sensors/Sensirion_Humidity_Sensors_SHT3x_Sample_Code.pdf
 *
 *  @bug  No known bugs
 */
#ifndef _SHT35_H
#define _SHT35_H

/** Remove STATIC and PERSISTENT values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
#endif
#ifndef PERSISTENT
#define PERSISTENT
#endif
#else
#ifndef STATIC
#define STATIC static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent
#endif
#endif

/************************************************************************
 *			STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <math.h>
#ifdef UNIT_TESTING
#include <stdio.h>
#endif

/************************************************************************
 *			HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "logging.h"

/************************************************************************
 *			Structs & Enums
 ************************************************************************/
/**
 * @enum SHT3X Error Codes
 *
 */
typedef enum
{
  SHT_OK = 0x00,             /**< SHT3X OK */
  SHT_ACK_ERROR = 0x01,      /**< SHT3X No Acknowledgment from sensor */
  SHT_CHECKSUM_ERROR = 0x02, /**< SHT3X Checksum Mismatch */
  SHT_TIMEOUT_ERROR = 0x04,  /**< SHT3X Timeout */
  SHT_PARAM_ERROR = 0x08,
  SHT_FAIL = 0x10, /**< SHT3X Fail (generic) */
} eSHTError_t;
/**
 * @enum SHT3X Sensor Commands
 * Sensor Commands for SHT3X
 */
typedef enum
{
  CMD_READ_SERIALNBR = 0x3780,  /**< Read Serial Number */
  CMD_READ_STATUS = 0xF32D,     /**< Read Status Register */
  CMD_CLEAR_STATUS = 0x3041,    /**< Clear Status Register */
  CMD_HEATER_ENABLE = 0x306D,   /**< Enable Heater */
  CMD_HEATER_DISABLE = 0x3066,  /**< Disable Heater */
  CMD_SOFT_RESET = 0x30A2,      /**< Soft Reset */
  CMD_MEAS_CLOCKSTR_H = 0x2C06, /**< Measurement: clock stretching, high repeatability */
  CMD_MEAS_CLOCKSTR_M = 0x2C0D, /**< Measurement: clock stretching, medium repeatability */
  CMD_MEAS_CLOCKSTR_L = 0x2C10, /**< Measurement: clock strecthing, low repeatability */
  CMD_MEAS_POLLING_H = 0x2400,  /**< Measurement: polling, high repeatability */
  CMD_MEAS_POLLING_M = 0x240B,  /**< Measurement: polling, medium repeatability */
  CMD_MEAS_POLLING_L = 0x2416,  /**< Measurement: polling, low repeatability */
  CMD_MEAS_PERI_05_H = 0x2032,  /**< Measurement: periodic 0.5mps, high repeatability */
  CMD_MEAS_PERI_05_M = 0x2024,  /**< Measurement: periodic 0.5mps, medium repeatability */
  CMD_MEAS_PERI_05_L = 0x202F,  /**< Measurement: periodic 0.5mps, low repeatability */
  CMD_MEAS_PERI_1_H = 0x2130,   /**< Measurement: periodic 1 mps, high repeatability */
  CMD_MEAS_PERI_1_M = 0x2126,   /**< Measurement: periodic 1 ps, medium repeatability */
  CMD_MEAS_PERI_1_L = 0x212D,   /**< Measurement: periodic 1 mps, low repeatability */
  CMD_MEAS_PERI_2_H = 0x2236,   /**< Measurement: periodic 2 mps, high repeatability */
  CMD_MEAS_PERI_2_M = 0x2220,   /**< Measurement: periodic 2 mps, medium repeatability */
  CMD_MEAS_PERI_2_L = 0x222B,   /**< Measurement: periodic 2 mps, low repeatability */
  CMD_MEAS_PERI_4_H = 0x2334,   /**< Measurement: periodic 4 mps, high repeatability */
  CMD_MEAS_PERI_4_M = 0x2322,   /**< Measurement: periodic 4 mps, medium repeatability */
  CMD_MEAS_PERI_4_L = 0x2329,   /**< Measurement: periodic 4 mps, low repeatability */
  CMD_MEAS_PERI_10_H = 0x2737,  /**< Measurement: periodic 10 mps, high repeatability */
  CMD_MEAS_PERI_10_M = 0x2721,  /**< Measurement: periodic 10 mps, medium repeatability */
  CMD_MEAS_PERI_10_L = 0x272A,  /**< Measurement: periodic 10 mps, low repeatability */
  CMD_FETCH_DATA = 0xE000,      /**< Readout measurements for periodic mode */
  CMD_R_AL_LIM_LS = 0xE102,     /**< Read alert limits, low set */
  CMD_R_AL_LIM_LC = 0xE109,     /**< Read alert limits, low clear */
  CMD_R_AL_LIM_HS = 0xE11F,     /**< Read alert limits, high set */
  CMD_R_AL_LIM_HC = 0xE114,     /**< Read alert limits, high clear */
  CMD_W_AL_LIM_HS = 0x611D,     /**< Write alert limits, high set */
  CMD_W_AL_LIM_HC = 0x6116,     /**< Write alert limits, high clear */
  CMD_W_AL_LIM_LC = 0x610B,     /**< Write alert limits, low clear */
  CMD_W_AL_LIM_LS = 0x6100,     /**< Write alert limits, low set */
  CMD_NO_SLEEP = 0x303E,        /**< No Sleep */
} etCommands;

/**
 * @enum SHT3X Measurement Repeatability
 * SHT3X Measurment Repeatability
 */
typedef enum
{
  REPEATAB_HIGH,   /**< High Repeatability */
  REPEATAB_MEDIUM, /**< Medium Repeatability */
  REPEATAB_LOW,    /**< Low Repeatability */
} etRepeatability;

/**
 * @enum SHT3X Measurement Mode
 * SHT3X Measurement Mode
 */
typedef enum
{
  MODE_CLKSTRETCH, /**< Clock Stretching */
  MODE_POLLING,    /**< Polling */
} etMode;

/**
 * @enum SHT3X Measurement Frequency
 * SHT3X Measurement Frequency
 */
typedef enum
{
  FREQUENCY_HZ5,  /**< 0.5 measurements per second */
  FREQUENCY_1HZ,  /**< 1.0 measurements per second */
  FREQUENCY_2HZ,  /**< 2.0 measurements per second */
  FREQUENCY_4HZ,  /**< 4.0 measurements per second */
  FREQUENCY_10HZ, /**< 10.0 measurements per second */
} etFrequency;

/**
 * @union SHT3X Status Register
 * SHT3X Status Register
 */
typedef union
{
  uint16_t u16;
  struct
  {
#ifdef LITTLE_ENDIAN            /**< Little endian bit-order */
    uint16_t CrcStatus : 1;     /**< Write data checksum status */
    uint16_t CmdStatus : 1;     /**< Command Status */
    uint16_t Reserve0 : 2;      /**< Reserved */
    uint16_t ResetDetected : 1; /**< System Reset Detected */
    uint16_t Reserve1 : 5;      /**< Reserved */
    uint16_t T_Alert : 1;       /**< Temperature Tracking alert */
    uint16_t RH_Alert : 1;      /**< Humidity Tracking alert */
    uint16_t Reserve2 : 1;      /**< Reserved */
    uint16_t HeaterStatus : 1;  /**< Heater Status */
    uint16_t Reserve3 : 1;      /**< Reserved */
    uint16_t AlertPending : 1;  /**< Alert pending status */
#else
    uint16_t AlertPending : 1;  /**< Alert pending status */
    uint16_t Reserve3 : 1;      /**< Reserved */
    uint16_t HeaterStatus : 1;  /**< Heater Status */
    uint16_t Reserve2 : 1;      /**< Reserved */
    uint16_t RH_Alert : 1;      /**< Humidity Tracking alert */
    uint16_t T_Alert : 1;       /**< Temperature Tracking alert */
    uint16_t Reserve1 : 5;      /**< Reserved */
    uint16_t ResetDetected : 1; /**< System Reset Detected */
    uint16_t Reserve0 : 2;      /**< Reserved */
    uint16_t CmdStatus : 1;     /**< Command Status */
    uint16_t CrcStatus : 1;     /**< Write data checksum status */
#endif
  } bit;
} regStatus;

/**
 * @struct
 *
 */
typedef struct
{
  sI2C_t I2C;        /**< SHT3X I2C Port */
  uint8_t slaveAddr; /**< Slave Address */
#ifdef SHT3X_ALERT_PIN
  sGPIO_t Alert; /**< SHT3X Alert Pin */
#endif
#ifdef SHT3X_RESET_PIN
  sGPIO_t Reset; /**< SHT3X Reset Pin */
#endif
  float humidity;         /**< Last humidity reading (&RH) */
  float temperature;      /**< Last temperature reading (degC) */
  uint32_t serial_number; /**< SHT35 Serial Number */
} sSHT3X_t;

/************************************************************************
 *			Function Prototypes
 ************************************************************************/
/** @brief Initialize the SHT3X
 *
 *  Initialize the I2C bus for comms with SHT3X.
 *
 * @param i2cAddress SHT3X I2C Address, 0x44 ADDR, pin low / 0x45 ADDR pin high
 *
 * @return None
 */
void SHT3X_Init(uint8_t i2cAddress);

/** @brief Set the I2C Address
 *
 * Sets the SHT3X I2C Address.
 *
 * @param i2cAddress SHT3X I2C Address, 0x44 ADDR pin low / 0x45 ADDR pin high
 *
 * @return
 */
void SHT3X_SetI2CAddr(uint8_t i2cAddress);

uint32_t SHT3X_Get_SerialNumber(void);

/** @brief Read the serial number
 *
 * Reads the SHT3X Serial number from the device.
 *
 * @param *serialNumber pointer to SHT3X Serial Number variable
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
uint32_t SHT3X_ReadSerialNumber(void);

/** @brief Read SHT3X Status Register
 *
 * Read the SHT3X Status Register from the sensor.
 *
 * @param *status Pointer to status variable
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_ReadStatus(uint16_t *status);

/** @brief Clear Alerts
 *
 * Clear all alert flags in status register from sensor.
 *
 * @param None
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_ClearAllAlertFlags(void);

/** @brief Get Temp & Humidity
 *
 * Get the temperature (DegC) and humidity (%RH) from the sensor
 *
 * @param *temperature Pointer to temperature
 * @param *humidity Pointer to humidity
 * @param repeatability Repeatability for the measurement (low, medium or high)
 * @param mode Command mode (clock stretching, polling)
 * @param timeout Timeout in milliseconds
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_GetTempAndHumidity(float *temperature,
                                     float *humidity,
                                     etRepeatability repeatability,
                                     etMode mode,
                                     uint8_t timeout);
/** @brief Get Temp & Humidity w/Clock Stretching
 *
 * Get the temperature (DegC) and humidity (%RH) from the sensor.  This function
 * uses the i2c clock stretching for waiting until measurement is ready.
 *
 * @param *temperature Pointer to temperature
 * @param *humidity Pointer to humidity
 * @param repeatability Repeatability for the measurement (low, medium or high)
 * @param timeout Clock stretching timeout in milliseconds
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_GetTempAndHumidityClckStrech(float *temperature,
                                               float *humidity,
                                               etRepeatability repeatability,
                                               uint8_t timeout);
/** @brief Get Temp & Humidity w/Polling
 *
 * Get the temperature (DegC) and humidity (%RH) from the sensor.  This function
 * polls every 1ms until measurement is ready.
 *
 * @param *temperature Pointer to temperature
 * @param *humidity Pointer to humidity
 * @param repeatability Repeatability for the measurement (low, medium or high)
 * @param timeout Polling timeout in milliseconds
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_GetTempAndHumidityPolling(float *temperature,
                                            float *humidity,
                                            etRepeatability repeatability,
                                            uint8_t timeout);
/** @brief Start periodic measurement.
 *
 * Stats the SHT35 on periodic measurements.
 *
 * @param repeatability Repeatability for the measurement (low, medium or high)
 * @param frequency Measurement frequency [0.5 1, 2, 4, 10] Hz
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_StartPeriodicMeasurement(etRepeatability repeatability,
                                           etFrequency frequency);
/** @brief Reads last measurement from sensor buffer
 *
 * Reads the last measurement from the SHT35 buffer
 *
 * @param *temperature Pointer to temperature
 * @param *humidity Pointer to humidity
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_ReadMeasurementBuffer(float *temperature,
                                        float *humidity);
/** @brief Enable the heater
 *
 * Enables the SHT3X Heater.
 *
 * @param None
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_EnableHeater(void);

/** @brief Disable the heater
 *
 * Disable the SHT3X Heater.
 *
 * @param
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_DisableHeater(void);

/** @brief Set Alert Limits on sensor
 *
 * Sets the alert limits on the SHT3X
 *
 * @param humidityHighSet
 * @param temperatureHighSet
 * @param humidityHighClear
 * @param temperatureHighClear
 * @param humidityLowClear
 * @param temperatureLowClear
 * @param humidityLowSet
 * @param temperatureLowSet
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_SetAlertLimit(float humidityHighSet,
                                float temperatureHighSet,
                                float humidityHighClear,
                                float temperatureHighClear,
                                float humidityLowClear,
                                float temperatureLowClear,
                                float humidityLowSet,
                                float temperatureLowSet);

/** @brief Get Alert Limits on sensor
 *
 * Read alert limits on sensor.
 *
 * @param *humidityHighSet
 * @param *temperatureHighSet
 * @param *humidityHighClear
 * @param *temperatureHighClear
 * @param *humidityLowClear
 * @param *temperatureLowClear
 * @param *humidityLowSet
 * @param *temperatureLowSet
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_GetAlertlimits(float *humidityHighSet,
                                 float *temperatureHighSet,
                                 float *humidityHighClear,
                                 float *temperatureHighClear,
                                 float *humidityLowClear,
                                 float *temperatureLowClear,
                                 float *humidityLowSet,
                                 float *temperatureLowSet);

/** @brief Return state of alert pin
 *
 * Returns the state of the SHT3X alert pin.
 *
 * @param None
 *
 * @return state  True: Alert-Pin is high
 *                False: Alert-pin is low
 */
bool SHT3X_ReadAlert(void);

/** @brief Reset sensor without removing from power.
 *
 * Calls the soft reset mechanism that forces the sensor into a well-defined
 * state without removing the power supply.
 *
 * @param None
 *
 * @return eSHTError_t Error:     ACK_ERROR = No Acknowledgement from sensor
 *                            CHECKSUM_ERROR = Checksum Mismatch
 *                            TIMEOUT_ERROR = Timeout
 *                            NO_ERROR  = No Error
 */
eSHTError_t SHT3X_SoftReset(void);

/** @brief Hard reset
 *
 * Reset the sensor by pulling down the reset pin.
 *
 * @param None
 *
 * @return None
 */
void SHT3X_HardReset(void);

#endif // _SHT35_H