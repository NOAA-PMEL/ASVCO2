/** @file max250.h
 *  @brief MAX250 O2 Sensor drivers
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 15, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note MAX250 Configuration drivers
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _MAX250_H
#define _MAX250_H

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
 *						STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <string.h>
#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
#include <math.h>

#ifdef UNIT_TESTING
#include <stdio.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "AM08X5.h"
#include "logging.h"
#include "errorlist.h"
/************************************************************************
 *							MACROS
 ************************************************************************/
#define O2_MIN_VOLTAGE (MAX250_MIN_VOLTAGE)
#define O2_MAX_VOLTAGE (MAX250_MAX_VOLTAGE)

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum MAX250 Status
 * MAX250 pass or fail status
 */
typedef enum
{
    MAX250_OK = 0u,
    MAX250_FAIL = 1u
} eMAX250Status_t;

/**
 *  @struct MAX250 Analog Struct
 *  All information regarding pins and setup for MAX250
 */
typedef struct
{
    sAnalog_t Analog; /**< Analog Port Information */
    sGPIO_t Pin;      /**< Analog Read Pin */
    sGPIO_t Power;    /**< Voltage Reference Power Pin */
    bool RefOn;       /**< Is Power currently set on Reference? */
    float O2;         /**< Current O2 Reading in PPM NaN if not read */
    float cal;        /**< Calibration value */
    float zero;
    float max;
} sMAX250_t;

#ifdef UNIT_TESTING
extern sMAX250_t MAX250;
#endif

/** Initialize MAX250
 *
 *  Initialize the MAX250 Oxygen Sensor Analog Pin and prep for reading
 *
 *  @param None
 *
 *  @return None
 */
void MAX250_Init(void);

/** Read MAX250
 *
 *  Read the MAX250 Oxygen Sensor Analog signal and conver to percent O2
 *
 *  @param None
 *
 *  @return None
 */
float MAX250_Read(void);

/** Read MAX250 Voltage Directly
 *
 * Read the voltage without converting to %O2
 *
 * @param None
 *
 * @return voltage
 */
float MAX250_ReadVoltage(void);

/** @brief Set MAX250 Calibration
 *
 * Set the MAX250 calibration value.
 *
 * @param cal Calibration value
 *
 * @return None
 */
void MAX250_SetCal(float cal);

/** @brief Get MAX250 Calibration Coefficient
 *
 * Get the MAX250 Calibration Coefficient
 *
 * @param None
 *
 * @return Coefficient
 */
float MAX250_GetCal(void);

/** @brief Turn MAX250 VREF ON
 *
 * Turn the 2.5V Voltage Reference ON
 *
 * @param None
 *
 * @return None
 */
void MAX250_PowerON(void);

/** @brief Turn MAX250 VREF OFF
 *
 * Turn the 2.5V Voltage Reference OFF
 *
 * @param None
 *
 * @return None
 */
void MAX250_PowerOFF(void);

#endif // _MAX250_H
