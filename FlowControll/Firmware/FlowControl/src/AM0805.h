/** @file AM0805.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date October 1, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

#ifndef _AM0805_H
#define _AM0805_H

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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "bsp.h"
#ifdef UNIT_TESTING
// #include "../test/MSP430FR5994.h"
#endif

/************************************************************************
*							MACROS
************************************************************************/
#ifndef LENGTH_OF
#define LENGTH_OF(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef MASK
#define MASK(a) (1u << (a))
#endif

/** AM0805 Macros - Registers */
#define HUNDREDTHS_REG (0x00)
#define YEARS_REG (0x06)
#define ALARM_HUNDRS_REG (0x08)
#define STATUS_REG (0x0F)
#define CONTROL_1_REG (0x10)
#define CONTROL_2_REG (0x11)
#define INT_MASK_REG (0x12)
#define SQW_REG (0x13)
#define CAL_XT_REG (0x14)
#define CAL_RC_HI_REG (0x15)
#define CAL_RC_LOW_REG (0x16)
#define SLEEP_CTRL_REG (0x17)
#define TIMER_CTRL_REG (0x18)
#define TIMER_REG (0x19)
#define TIMER_INITIAL_REG (0x1A)
#define WDT_REG (0x1B)
#define OSC_CONTROL_REG (0x1C)
#define OSC_STATUS_REG (0x1D)
#define CONFIG_KEY_REG (0x1F)
#define TRICKLE_CHARGE_REG (0x20)
#define ACAL_FLT_REG (0x26)
#define EXTENDED_ADDR_REG (0x3F)

/** AM0805 Macros - Settings */
#define SLAVE_ADDR (0xD2)
#define UNLOCK_KEY (0x9D)

/** AM0805 Macro - Trickle Charger (REG=0x20) */
#define TRICKLE_ENABLE (0xA0)
#define TRICKLE_DISABLE (0x00)
#define TRICKLE_DIODE_STANDARD (0x08)
#define TRICKLE_DIODE_SCHOTTKY (0x04)
#define TRICKLE_RES_DISABLE (0x00)
#define TRICKLE_RES_3K (0x01)
#define TRICKLE_RES_6K (0x02)
#define TRICKLE_RES_11k (0x03)

/** AM0805 Macro - Control 2  (REG=0x11) */
#define OUT1S_MASK (0x03)
#define OUT2S_MASK (0x1C)
#define OUT1S_SQW (0x01)
#define OUT1S_SQW_INT (0x02)
#define OUT1S_nAIRQ (0x03)
#define OUT2S_nIRQ (0x00)
#define OUT2S_SQW (0x04)
#define OUT2S_nAIRQ (0xUO)

/** AM0805 Macro - Square Wave SQW (REG=0x13) */
#define SQW_ENABLED (0x80)
#define SQW_DISABLED (0x00)
#define SQW_T_CENTURY (0x00)
#define SQW_T_32768HZ (0x01)
#define SQW_T_8192HZ (0x02)
#define SQW_T_4096HZ (0x03)
#define SQW_T_2048HZ (0x04)
#define SQW_T_1024HZ (0x05)
#define SQW_T_512HZ (0x06)
#define SQW_T_256HZ (0x07)
#define SQW_T_128HZ (0x08)
#define SQW_T_64HZ (0x09)
#define SQW_T_32HZ (0x0A)
#define SQW_T_16HZ (0x0B)
#define SQW_T_8HZ (0x0C)
#define SQW_T_4HZ (0x0D)
#define SQW_T_2HZ (0x0E)
#define SQW_T_1HZ (0x0F)
#define SQW_T_1_2HZ (0x10)
#define SQW_T_1_4HZ (0x11)
#define SQW_T_1_8HZ (0x12)
#define SQW_T_1_16HZ (0x13)
#define SQW_T_1_32HZ (0x14)
#define SQW_T_1_60HZ (0x15)
#define SQW_T_16384HZ (0x16)
#define SQW_T_100HZ (0x17)
#define SQW_T_1_HOUR (0x18)
#define SQW_T_1_DAY (0x19)
#define SQW_T_TIRQ (0x1A)
#define SQW_T_NOT_TIRQ (0x1B)
#define SQW_T_1_YEAR (0x1C)
#define SQW_T_1HZ_TO_COUNTERS (0x1D)
#define SQW_T_1_32_FROM_ACAL (0x1E)
#define SQW_T_1_8_FROM_ACAL (0x1F)

/** AM0805 Macro - XT Cal (REG=0x14) */
#define CALXT_NORMAL (0x00)
#define CALXT_COARSE (0x80)
#define CALXT_CLEAR (0x00)

/** AM0805 Macro - Countdown Timer control (REG=0x18) */
#define CTCR_CLEAR (0x00)
#define CTCR_TE (0x80)
#define CTCR_TM (0x40)
#define CTCR_TRPT (0x20)

/** AM0805 Macro - Interrupt Mask Register (REG=0x12) */
#define INTMASK_CEB (0x80)
#define INTMASK_IM_LEVEL (0x0)
#define INTMASK_IM_FAST (0x60)
#define INTMASK_IM_MID (0x40)
#define INTMASK_IM_SLOW (0x20)
#define INTMASK_BATT_LOW (0x10)
#define INTMASK_TIMER_INT (0x08)
#define INTMASK_ALARM_INT (0x04)
#define INTMASK_XT2_INTE (0x02)
#define INTMASK_XT1_INTE (0x01)

/** AM0805 Macro - Osc. Status (REG=0x1D) */
#define OSCSTAT_CLEAR (0x00)
#define OSCSTAT_XTCAL_X0 (0x00)
#define OSCSTAT_XTCAL_X122 (0x40)
#define OSCSTAT_XTCAL_X244 (0x80)
#define OSCSTAT_XTCAL_X366 (0xC0)
#define OSCSTAT_LKO2 (0x20)
#define OSCSTAT_OMODE (0x10)
#define OSCSTAT_OF (0x02)
#define OSCSTAT_ACF (0x01)

/************************************************************************
*							Structs & Enums
************************************************************************/
/**
 *  @enum  AM0805 Status
 *  @brief AM0805 Status returns
 */
typedef enum
{
    AM0805_OK = 0,   /**< RTC Okay */
    AM0805_FAIL = 1, /**< RTC Failure */
} eAM0805Status_t;

/** 
 *  @enum AM0805 Output Pins
 *  @brief All output pins on AM0805
 */
typedef enum
{
    AM0805_nIRQ = 0,  /**< nIRQ Pin */
    AM0805_nIRQ2 = 1, /**< nIRQ2 Pin */
    AM0805_nTIRQ = 2  /**< nTIRQ Pin */
} eAM0805OutPins_t;

/**
 *  @enum AM0805 Oscillator Selection
 *  @brief AM0805 Oscillator Selection Enum
 */
typedef enum
{
    AM0805_OSC_32768Hz_NoSwitch = 0,   /**< No change in Oscillator, but clear key registers */
    AM0805_OSC_32768Hz_AutoSwitch = 1, /**< 128 Hz RC Oscillator */
    AM0805_OSC_128Hz = 2               /**< 32768 Hz Crystal Oscillator */
} eAM0808osc_t;

/** 
 *  @enum AM0805 Alarms
 *  @brief AM0805 Alarm Types
 */
typedef enum
{
    AM0805_Alarm_Disable = 0, /**< Disable Alarms */
    AM0805_Alarm_1Year = 1,   /**< 1 Year Alarm */
    AM0805_Alarm_1Month = 2,  /**< 1 Month Alarm */
    AM0805_Alarm_1Week = 3,   /**< 1 Week Alarm */
    AM0805_Alarm_1Day = 4,    /**< 1 Day Alarm */
    AM0805_Alarm_1Hour = 5,   /**< 1 Hour Alarm */
    AM0805_Alarm_1Min = 6,    /**< 1 Minute Alarm */
    AM0805_Alarm_1Sec = 7,    /**< 1 Second Alarm */
    AM0805_Alarm_10Hz = 8,    /**< 10 Hz Alarm */
    AM0805_Alarm_100Hz = 9,   /**< 100Hz Alarm */
} eAMBIQ_Alarm_t;

/** 
 *  @struct AM0805 Data Struct
 *  @brief AM0805 Data Structure and values
 */
typedef struct
{
    struct tm time;   /**< Current time struct */
    time_t now;       /**< Total seconds since Jan 1, 1970 */
    char timestr[80]; /**< Time string */
} sAM0805_t;

/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Initialize the AM0805 RTC
 *
 *	Intializes the AM0805 RTC, sets the timers and clears all flags
 *
 *  @param None
 *
 *  @return Result
 */
eAM0805Status_t AM0805_Init(void);

/** @brief Set Alarm on 
 *
 *  Set an alarm on Pin nIRQ
 *
 *  @param alarm Alarm to set 
 *  
 *  @return Result
 */
eAM0805Status_t AM0805_SetAlarm(eAMBIQ_Alarm_t alarm);

/** @brief Get Current Time
 *
 *  Get the current time
 *
 *  @param None
 *
 *  @return Result
 */
eAM0805Status_t AM0805_GetTime(void);

/** @brief Calculate RTC Offset
 *
 *  Calculate the RTC offset based on output frequency
 *  of the crystal.  
 *
 *  @param frequency Frequency of output pin
 *
 *  @return Result
 */
eAM0805Status_t AM0805_RTC_OffsetCalculation(float frequency);

/** @brief Set Calibration Frequency 
 *
 *  Output 32768Hz frequency on selected pin
 *
 *  @param Pin pin to output frequnency on
 *
 *  @return Result
 */
eAM0805Status_t AM0805_SetCalFreq(eAM0805OutPins_t Pin);

/************************************************************************
*				STATIC FUNCTION PROTOTYPES (FOR TEST CASES)
************************************************************************/

#ifdef STATIC
STATIC eAM0805Status_t am_get_time(sAM0805_t *p);
STATIC eAM0805Status_t am_osc_sel(eAM0808osc_t osc);
STATIC eAM0805Status_t am_config_sqw(uint8_t Freq, eAM0805OutPins_t Pin);
STATIC eAM0805Status_t am_set_alarm_on_zeros(eAMBIQ_Alarm_t alarm, uint8_t intmode, uint8_t pin);
STATIC uint8_t bcd2dec(uint8_t bcdno);
STATIC uint8_t dec2bcd(uint8_t decno);
STATIC eAM0805Status_t readreg(uint8_t register, uint8_t *val);
STATIC eAM0805Status_t writereg(uint8_t register, uint8_t val);
STATIC eAM0805Status_t clrreg(uint8_t reg, uint8_t mask);
STATIC eAM0805Status_t setreg(uint8_t addr, uint8_t mask);
#endif

#endif // _AM0805_H
