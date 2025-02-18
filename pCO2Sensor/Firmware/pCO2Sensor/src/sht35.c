/** @file sht35.c
 * @brief Sensirion SHT-35 Digital Relative Humidity and Temparture Sensor (I2C)
 *
 * @author Matt Casari, matthew.casari@noaa.gov
 * @date July 6, 2018
 * @version 1.0.0
 *
 * @copyright National Oceanic and Atmospheric Administration
 * @copyright Pacific Marine Environmental Lab
 * @copyright Environmental Development Division
 *
 * @note Driver interface for the Sensirion SHT-35 RH/T Digital I2C Sensor
 * @note Requires pCO2 BSP and TI Driverlib
 * @note This driver largely leveraged from Sensirion Example Code found
 * @note https: *www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/13_Sample_Codes_Software/Humidity_Sensors/Sensirion_Humidity_Sensors_SHT3x_Sample_Code.pdf
 *
 * @bug  No known bugs
 */
#include "sht35.h"

/************************************************************************
 *			Static Variables
 ************************************************************************/
PERSISTENT sSHT3X_t SHT35 = {
    .I2C = {.param = {.selectClockSource = SHT35_I2C_CLK_SRC,
                      .i2cClk = SHT35_I2C_CLK_FREQ,
                      .dataRate = SHT35_I2C_DES_FREQ,
                      .byteCounterThreshold = SHT35_I2C_BYTE_CNT_THRESHOLD,
                      .autoSTOPGeneration = SHT35_I2C_AUTOSTOP},
            .baseAddress = SHT35_I2C_PORT,
            .mode = SHT35_I2C_MODE,
            .slaveAddress = SHT35_I2C_SLAVE_ADDR,
            .timeout = SHT35_I2C_TIMEOUT},
#ifdef SHT3X_RESET_PIN
    .Reset = {

    },
#endif
#ifdef SHT3X_ALERT_PIN
    .Alert = {

    },
#endif
    /**                             . */
};

#ifdef SHT3X_RESET_PIN
#define RESET_LOW (BSP_GPIO_ClearPin(&SHT35.Reset))
#defnie RESET_HIGH(BSP_GPIO_SetPin(&SHT35.Reset))
#endif

#define POLYNOMIAL (0x131) /** P(x) = x^8 + x^5 + x^4 + 1 = 100110001 */
/************************************************************************
 *			Static Functions
 ************************************************************************/

/** @brief Calculate Temp
 *
 * Calculate the temperature from the raw value
 *
 * @param rawValue Raw Temp Values
 *
 * @return Converted temperature
 */
STATIC float SHT3X_CalcTemperature(uint16_t rawValue)
{
  return (float)(175.0f * (float)rawValue / 65535.0f - 45.0f);
}

/** @brief Calculate Humidity
 *
 * Calculate Humidity from the raw value
 *
 * @param rawValue Raw Humidity Value
 *
 * @return Converted Humidity
 */
STATIC float SHT3X_CalcHumidity(uint16_t rawValue)
{
  return (float)(100.0f * (float)rawValue / 65535.0f);
}

/** @brief Calculate Raw Temp
 *
 * Calculate Raw Temperature values from temperature
 *
 * @param temperature Floating point temperature
 *
 * @return raw temperature values
 */
STATIC uint16_t SHT3X_CalcRawTemperature(float temperature)
{
  return (uint16_t)((temperature * 45.0f) / 175.0f * 65535.0f);
}

/** @brief Calculate Raw Humidity
 *
 * Calculate Raw Humidty values from humidity
 *
 * @param humidity Floating point humidity
 *
 * @return raw humidity values
 */
STATIC uint16_t SHT3X_CalcRawHumidity(float humidity)
{
  return (uint16_t)((uint16_t)humidity / 100.0f * 65535.0f);
}

/** @brief Calculate CRC
 *
 * Calculates 8 bit crc from data.
 *
 * @param data[] Data array to CRC
 * @param nbrOfBytes Number of bytes in data array
 *
 * @return CRC value
 */
STATIC uint8_t SHT3X_CalcCrc(uint8_t data[],
                             uint8_t nbrOfBytes)
{
  uint8_t bit;        /** Bit mask */
  uint8_t crc = 0xFF; /** Calculated checksum */
  uint8_t byteCtr;    /** Byte Counter */

  for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
  {
    crc ^= (data[byteCtr]);
    for (bit = 8; bit > 0; --bit)
    {
      if (crc & 0x80)
      {
        crc = (crc << 1) ^ POLYNOMIAL;
      }
      else
      {
        crc = (crc << 1);
      }
    }
  }

  return crc;
}

/** @brief Check CRC
 *
 * Calculate the CRC and verify against CRC passed in.
 *
 * @param data[] Data array
 * @param nbrOfBytes Number of bytes in data array
 * @param checksum CRC passed in to verify
 *
 * @return Result
 */
STATIC eSHTError_t SHT3X_CheckCrc(uint8_t data[],
                                  uint8_t nbrOfBytes,
                                  uint8_t checksum)
{
  uint8_t crc;

  /** Calculate the CRC */
  crc = SHT3X_CalcCrc(data, nbrOfBytes);

  /** Verify the CRC */
  if (crc != checksum)
  {
    return SHT_CHECKSUM_ERROR;
  }
  else
  {
    return SHT_OK;
  }
}

/** @brief Send 2 bytes & CRC
 *
 * Sends 2 bytes and CRC over I2C
 *
 * @param data Data to send
 *
 * @return Error
 */
STATIC eSHTError_t SHT3X_Write2BytesAndCrc(uint16_t data)
{
  eSHTError_t error = SHT_FAIL; /**< error code */
  uint8_t bytes[2];             /**< read data array */
  uint8_t checksum;             /**< checksum byte */

  bytes[0] = data >> 8;
  bytes[1] = data & 0xFF;

  checksum = SHT3X_CalcCrc(bytes, 2);

  /** write two data bytes and one checksum byte */
  /** write MSB */
  BSP_I2C_put(&SHT35.I2C, bytes[0]);

  /** Write LSB */
  BSP_I2C_put(&SHT35.I2C, bytes[1]);

  /** Write checksum */
  BSP_I2C_put(&SHT35.I2C, checksum);

  return error;
}

/** @brief Read 2 bytes and CRC
 *
 * Reads 2 bytes of data from I2C and validates CRC
 *
 * @param *data Pointer to data
 * @param finalAckNack Ack or Nack the data
 * @param timeout Time before failure
 *
 * @return Error
 */
STATIC eSHTError_t SHT3X_Read2BytesAndCrc(uint16_t *data,
                                          bool finaleAckNack,
                                          uint8_t timeout)
{
  eSHTError_t error = SHT_FAIL;
  uint8_t bytes[3]; /** Combine data (2bytes) and checksum (1byte) */

  /** Read the two data bytes and one checksum byte */
  BSP_I2C_gets(&SHT35.I2C, bytes, 3);

  /** Validate the CRC */
  error = SHT3X_CheckCrc(bytes, 2, bytes[2]);

  return error;
}

/** @brief Ready 4 bytes and CRC
 *
 * Read 4 bytes and CRC over I2C
 *
 * @param *data Pointer to data
 * @param timeout Time before failure
 *
 * @return Error
 */
STATIC eSHTError_t SHT3X_Read4BytesAndCrcs(uint16_t *data,
                                           uint8_t timeout)
{
  eSHTError_t error = SHT_FAIL;
  uint8_t bytes[6]; /** Combine data (2bytes) and checksum (1byte) */
  /** Read the two data bytes and one checksum byte */
  if (BSP_FAIL == BSP_I2C_gets(&SHT35.I2C, bytes, 6))
  {
    Error(ERROR_MSG_00400200, LOG_Priority_High);
    return error;
  }

  /** Check CRC */
  error = SHT3X_CheckCrc(bytes, 2, bytes[2]);
  error |= SHT3X_CheckCrc(&bytes[3], 2, bytes[5]);

  if (error != SHT_FAIL)
  {
    data[0] = (bytes[0] << 8) + bytes[1];
    data[1] = (bytes[3] << 8) + bytes[4];
  }
  return error;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC eSHTError_t SHT3X_WriteAlertLimitData(float humidity, float temperature)
{
  eSHTError_t error = SHT_FAIL;
  uint16_t rawHumidity;
  uint16_t rawTemperature;

  if ((humidity < 0.0f) || (humidity > 100.0f) || (temperature < -45.0f) || (temperature > 130.0f))
  {
    error = SHT_FAIL;
  }
  else
  {
    rawHumidity = SHT3X_CalcRawHumidity(humidity);
    rawTemperature = SHT3X_CalcRawTemperature(temperature);
    error = SHT3X_Write2BytesAndCrc((rawHumidity & 0xFE00) | ((rawTemperature >> 7) & 0x001FF));
  }

  return error;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC eSHTError_t SHT3X_ReadAlertLimitData(float *humidity, float *temperature)
{
  eSHTError_t error = SHT_FAIL;
  uint16_t data;

  error = SHT3X_Read2BytesAndCrc(&data, 1, 0);
  if (error == SHT_OK)
  {
    *humidity = SHT3X_CalcHumidity(data & 0xFE00);
    *temperature = SHT3X_CalcTemperature(data << 7);
  }
  return error;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC eSHTError_t SHT3X_StartWriteAccess(void)
{
  eSHTError_t error = SHT_FAIL;

  error = SHT_OK;
  return error;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC eSHTError_t SHT3X_StartReadAccess(void)
{
  eSHTError_t error = SHT_OK;

  return error;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC void SHT3X_StopAccess(void)
{

  return;
}

/** @brief
 *
 *
 *
 * @param
 *
 * @return
 */
STATIC eSHTError_t SHT3X_WriteCommand(etCommands command)
{
  eSHTError_t error = SHT_FAIL;
  uint8_t writecommand[2];
  uint16_t temp;
  temp = command & 0xFF00;
  writecommand[0] = (uint8_t)(temp >> 8);
  temp = command & 0x00FF;
  writecommand[1] = (uint8_t)temp;

  /** Write to command over I2C */
  BSP_I2C_puts(&SHT35.I2C, &writecommand[0], 2);

  error = SHT_OK;
  return error;
}

/************************************************************************
 *			        Functions
 ************************************************************************/

/***********************************************************************/
void SHT3X_Init(uint8_t i2cAddress)
{
  Log("RH Initialize", LOG_Priority_High);
  /** Initialize the I/0 Pins */

  /** Alert Pin Setup */
#ifdef SHT3X_ALERT_PIN
  BSP_GPIO_Init(&SHT35.Alert);
#endif

  /** Reset Init and Set Low */
#ifdef SHT3X_RESET_PIN
  BSP_GPIO_Init(&SHT35.Reset);
  RESET_LOW();
#endif

  /** Initialize I2C */
  BSP_I2C_Init(&SHT35.I2C);
  SHT3X_SetI2CAddr(i2cAddress << 1);

  /** Release Reset */
#ifdef SHT3X_RESET_PIN
  RESET_HIGH();
#endif

  return;
}

/***********************************************************************/
void SHT3X_SetI2CAddr(uint8_t i2cAddress)
{
  BSP_I2C_SetSlaveAddress(&SHT35.I2C, i2cAddress);
}

uint32_t SHT3X_Get_SerialNumber(void)
{
  return SHT35.serial_number;
}
/***********************************************************************/
uint32_t SHT3X_ReadSerialNumber(void)
{
  eSHTError_t error;
  uint16_t serialNumWords[2];
  uint32_t serialNumber = 0;

  error = SHT3X_StartWriteAccess();

  error |= SHT3X_WriteCommand(CMD_READ_SERIALNBR);

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }
  if (error == SHT_OK)
  {
    error = SHT3X_Read4BytesAndCrcs(serialNumWords, 100);
  }

  SHT3X_StopAccess();
  if (error == SHT_OK)
  {
    serialNumber = (uint32_t)(serialNumWords[0]) << 16;
    serialNumber |= serialNumWords[1];

    SHT35.serial_number = serialNumber;
  }
  else
  {
    Error("SHT35: Error Reading Serial Number", LOG_Priority_High);
    Error("SHT35: Serial Number not set", LOG_Priority_High);
  }
  return serialNumber;
}

/***********************************************************************/
eSHTError_t SHT3X_ReadStatus(uint16_t *status)
{
  eSHTError_t error = SHT_FAIL;

  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_READ_STATUS);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_Read2BytesAndCrc(status, 1, 0);
  }

  SHT3X_StopAccess();

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_ClearAllAlertFlags(void)
{
  eSHTError_t error = SHT_FAIL;

  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_CLEAR_STATUS);
  }

  SHT3X_StopAccess();

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_GetTempAndHumidity(float *temperature,
                                     float *humidity,
                                     etRepeatability repeatability,
                                     etMode mode,
                                     uint8_t timeout)
{
  eSHTError_t error = SHT_FAIL;

  switch (mode)
  {
  case MODE_CLKSTRETCH:
    error = SHT3X_GetTempAndHumidityClckStrech(temperature, humidity, repeatability, timeout);
    break;
  case MODE_POLLING:
    error = SHT3X_GetTempAndHumidityPolling(temperature, humidity, repeatability, timeout);
    break;
  default:
    error = SHT_PARAM_ERROR;
    break;
  }

  if (error != SHT_OK)
  {
    *temperature = NAN;
    *humidity = NAN;
  }

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_GetTempAndHumidityClckStrech(float *temperature,
                                               float *humidity,
                                               etRepeatability repeatability,
                                               uint8_t timeout)
{
  eSHTError_t error;     /** error code */
  uint16_t rawValueTemp; /** temperature raw value from sensor */
  uint16_t rawValueHumi; /** humidity raw value from sensor */

  error = SHT3X_StartWriteAccess();

  /** if no error ... */
  if (error == SHT_OK)
  {
    /** start measurement in clock stretching mode */
    /** use depending on the required repeatability, the corresponding command */
    switch (repeatability)
    {
    case REPEATAB_LOW:
      error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_L);
      break;
    case REPEATAB_MEDIUM:
      error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_M);
      break;
    case REPEATAB_HIGH:
      error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_H);
      break;
    default:
      error = SHT_PARAM_ERROR;
      break;
    }
  }
  /** if no error, start read access */
  if (error == SHT_OK)
    error = SHT3X_StartReadAccess();
  /** if no error, read temperature raw values */
  if (error == SHT_OK)
    error = SHT3X_Read2BytesAndCrc(&rawValueTemp, 0, timeout);
  /** if no error, read humidity raw values */
  if (error == SHT_OK)
    error = SHT3X_Read2BytesAndCrc(&rawValueHumi, 1, 0);

  SHT3X_StopAccess();

  /** if no error, calculate temperature in °C and humidity in %RH */
  if (error == SHT_OK)
  {
    *temperature = SHT3X_CalcTemperature(rawValueTemp);
    *humidity = SHT3X_CalcHumidity(rawValueHumi);
  }

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_GetTempAndHumidityPolling(float *temperature,
                                            float *humidity,
                                            etRepeatability repeatability,
                                            uint8_t timeout)
{
  eSHTError_t error;     /** error code */
  uint16_t rawValueTemp; /** temperature raw value from sensor */
  uint16_t rawValueHumi; /** humidity raw value from sensor */
  uint16_t rawValue[2];  /** temperature and humidity raw value from sensor */

  error = SHT3X_StartWriteAccess();

  /** if no error ... */
  if ((error != SHT_PARAM_ERROR) && (error != SHT_FAIL))
  {
    /** start measurement in polling mode */
    /** use depending on the required repeatability, the corresponding command */
    switch (repeatability)
    {
    case REPEATAB_LOW:
      error = SHT3X_WriteCommand(CMD_MEAS_POLLING_L);
      break;
    case REPEATAB_MEDIUM:
      error = SHT3X_WriteCommand(CMD_MEAS_POLLING_M);
      break;
    case REPEATAB_HIGH:
      error = SHT3X_WriteCommand(CMD_MEAS_POLLING_H);
      break;
    default:
      error = SHT_PARAM_ERROR;
      break;
    }
  }

  /** if no error, wait until measurement ready */
  timeout /= 5;
  if ((error != SHT_PARAM_ERROR) && (error != SHT_FAIL))
  {
    /** poll every 1ms for measurement ready until timeout */
    while (timeout--)
    {
      /** check if the measurement has finished */
      error = SHT3X_StartReadAccess();

      /** if measurement has finished -> exit loop */
      if (error == SHT_PARAM_ERROR)
        break;
    }
    /** if no error, read temperature and humidity raw values */
    if ((error != SHT_FAIL) && (error != SHT_PARAM_ERROR))
    {
      error = SHT3X_Read4BytesAndCrcs(&rawValue[0], 10);
    }

    SHT3X_StopAccess();

    /** if no error, calculate temperature in °C and humidity in %RH */
    if ((error != SHT_PARAM_ERROR) && (error != SHT_FAIL))
    {
      rawValueTemp = rawValue[0];
      rawValueHumi = rawValue[1];
      *temperature = SHT3X_CalcTemperature(rawValueTemp);
      *humidity = SHT3X_CalcHumidity(rawValueHumi);
    }
  }
  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_StartPeriodicMeasurement(etRepeatability repeatability,
                                           etFrequency frequency)
{
  eSHTError_t error; /** error code */

  error = SHT3X_StartWriteAccess();

  /** if no error, start periodic measurement */
  if (error == SHT_OK)
  {
    /** use depending on the required repeatability and frequency, */
    /** the corresponding command */
    switch (repeatability)
    {
    case REPEATAB_LOW: /** low repeatability */
      switch (frequency)
      {
      case FREQUENCY_HZ5: /** low repeatability, 0.5 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_L);
        break;
      case FREQUENCY_1HZ: /** low repeatability, 1.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_L);
        break;
      case FREQUENCY_2HZ: /** low repeatability, 2.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_L);
        break;
      case FREQUENCY_4HZ: /** low repeatability, 4.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_L);
        break;
      case FREQUENCY_10HZ: /** low repeatability, 10.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_L);
        break;
      default:
        error |= SHT_PARAM_ERROR;
        break;
      }
      break;

    case REPEATAB_MEDIUM: /** medium repeatability */
      switch (frequency)
      {
      case FREQUENCY_HZ5: /** medium repeatability, 0.5 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_M);
        break;
      case FREQUENCY_1HZ: /** medium repeatability, 1.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_M);
        break;
      case FREQUENCY_2HZ: /** medium repeatability, 2.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_M);
        break;
      case FREQUENCY_4HZ: /** medium repeatability, 4.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_M);
        break;
      case FREQUENCY_10HZ: /** medium repeatability, 10.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_M);
        break;
      default:
        error |= SHT_PARAM_ERROR;
        break;
      }
      break;
    case REPEATAB_HIGH: /** high repeatability */
      switch (frequency)
      {
      case FREQUENCY_HZ5: /** high repeatability, 0.5 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_H);
        break;
      case FREQUENCY_1HZ: /** high repeatability, 1.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_H);
        break;
      case FREQUENCY_2HZ: /** high repeatability, 2.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_H);
        break;
      case FREQUENCY_4HZ: /** high repeatability, 4.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_H);
        break;
      case FREQUENCY_10HZ: /** high repeatability, 10.0 Hz */
        error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_H);
        break;
      default:
        error |= SHT_PARAM_ERROR;
        break;
      }
      break;
    default:
      error |= SHT_PARAM_ERROR;
      break;
    }
  }
  SHT3X_StopAccess();
  return error;
}
/***********************************************************************/
eSHTError_t SHT3X_ReadMeasurementBuffer(float *temperature,
                                        float *humidity)
{
  eSHTError_t error = SHT_FAIL;
  uint16_t rawValueTemp;
  uint16_t rawValueHumidity;

  error = SHT3X_StartWriteAccess();

  /** Read measurements */
  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_FETCH_DATA);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_Read2BytesAndCrc(&rawValueTemp, 1, 0);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_Read2BytesAndCrc(&rawValueHumidity, 0, 0);
  }

  /** If no error, calculate temp in degC and humidity in %RH */
  if (error == SHT_OK)
  {
    *temperature = SHT3X_CalcTemperature(rawValueTemp);
    *humidity = SHT3X_CalcHumidity(rawValueHumidity);
  }

  SHT3X_StopAccess();

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_EnableHeater(void)
{
  eSHTError_t error = SHT_FAIL;

  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_HEATER_DISABLE);
  }

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_DisableHeater(void)
{
  eSHTError_t error = SHT_FAIL;

  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_HEATER_DISABLE);
  }

  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_SetAlertLimit(float humidityHighSet,
                                float temperatureHighSet,
                                float humidityHighClear,
                                float temperatureHighClear,
                                float humidityLowClear,
                                float temperatureLowClear,
                                float humidityLowSet,
                                float temperatureLowSet)
{
  eSHTError_t error = SHT_FAIL;

  /** Write humidity and temperature alter limters, high set */
  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_W_AL_LIM_HS);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_WriteAlertLimitData(humidityHighSet, temperatureHighSet);
  }

  SHT3X_StopAccess();

  /** Write humidity and temperature alter limters, high clear */
  if (error == SHT_OK)
  {
    if (error == SHT_OK)
    {
      error = SHT3X_WriteCommand(CMD_W_AL_LIM_HC);
    }

    if (error == SHT_OK)
    {
      error = SHT3X_WriteAlertLimitData(humidityHighClear, temperatureHighClear);
    }

    SHT3X_StopAccess();
  }

  /** Write humidity and temperature alter limters, low clear */
  if (error == SHT_OK)
  {
    if (error == SHT_OK)
    {
      error = SHT3X_WriteCommand(CMD_W_AL_LIM_LC);
    }

    if (error == SHT_OK)
    {
      error = SHT3X_WriteAlertLimitData(humidityLowClear, temperatureLowClear);
    }

    SHT3X_StopAccess();
  }

  /** Write humidity and temperature alter limters, low set */
  if (error == SHT_OK)
  {
    if (error == SHT_OK)
    {
      error = SHT3X_WriteCommand(CMD_W_AL_LIM_LS);
    }

    if (error == SHT_OK)
    {
      error = SHT3X_WriteAlertLimitData(humidityLowSet, temperatureLowSet);
    }

    SHT3X_StopAccess();
  }
  return error;
}

/***********************************************************************/
eSHTError_t SHT3X_GetAlertlimits(float *humidityHighSet,
                                 float *temperatureHighSet,
                                 float *humidityHighClear,
                                 float *temperatureHighClear,
                                 float *humidityLowClear,
                                 float *temperatureLowClear,
                                 float *humidityLowSet,
                                 float *temperatureLowSet)
{
  eSHTError_t error = SHT_FAIL;

  /** Read humidity and temperature alter limters, high set */
  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_R_AL_LIM_HS);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_ReadAlertLimitData(humidityHighSet, temperatureHighSet);
  }

  SHT3X_StopAccess();

  /** Read humidity and temperature alter limters, high clear */
  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_R_AL_LIM_HC);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_ReadAlertLimitData(humidityHighClear, temperatureHighClear);
  }

  SHT3X_StopAccess();

  /** Read humidity and temperature alter limters, low clear */
  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_R_AL_LIM_LC);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_ReadAlertLimitData(humidityLowClear, temperatureLowClear);
  }

  SHT3X_StopAccess();

  /** Read humidity and temperature alter limters, low set */
  error = SHT3X_StartWriteAccess();

  if (error == SHT_OK)
  {
    error = SHT3X_WriteCommand(CMD_R_AL_LIM_LS);
  }

  if (error == SHT_OK)
  {
    error = SHT3X_StartReadAccess();
  }

  if (error == SHT_OK)
  {
    error = SHT3X_ReadAlertLimitData(humidityLowSet, temperatureLowSet);
  }

  SHT3X_StopAccess();

  return error;
}

/***********************************************************************/
bool SHT3X_ReadAlert(void)
{
  /** This pin is not implement in pCO2 */

  return 0;
}

/***********************************************************************/
eSHTError_t SHT3X_SoftReset(void)
{
  eSHTError_t error = SHT_FAIL;
  uint8_t cmd[2];
  cmd[0] = (uint8_t)(CMD_SOFT_RESET >> 8);
  cmd[1] = (uint8_t)(CMD_SOFT_RESET & 0x00FF);

  if (BSP_I2C_SendData(&SHT35.I2C, SHT35.slaveAddr, cmd, 2) == BSP_OK)
  {
    error = SHT_OK;

    /** Wait 50 ms for reset */
    _delay_ms(50);
  }

  return error;
}

/***********************************************************************/
void SHT3X_HardReset(void)
{
  /** The pCO2 Sensor has no dedicated power cycling capability to RH sensor */
}