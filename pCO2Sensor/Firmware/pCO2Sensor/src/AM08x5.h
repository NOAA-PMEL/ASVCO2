/** @file AM08x5.h
 * @brief Ambiq AM0805 RTC Driver
 *
 * @author Matt Casari, matthew.casari@noaa.gov
 * @date February 27, 2018
 * @version 1.0.0
 *
 * @copyright National Oceanic and Atmospheric Administration
 * @copyright Pacific Marine Environmental Lab
 * @copyright Environmental Development Division
 *
 *	@note Controls the AM0805 and preps date/time for user
 * @note
 *
 *
 * @bug  No known bugs
 */
#ifndef _AM08X5_H
#define _AM08X5_H

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
#include <time.h>
#include <stdio.h>
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "sysconfig.h"
#include "logging.h"
#include "errorlist.h"
#include "buffer8.h"
/************************************************************************
 *							MACROS
 ************************************************************************/
/** AM08X5 Specific Settings */
#define AM08X5_TIMEOUT (SYSTEM_RTC_TIMEOUT_MS)
#define AM08X5_REGISTER_LENGTH (255)

/** Registers in RTC */
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
#define TRICKLE_REG (0x20)
#define BREF_REG (0x21)
#define ACAL_FLT_REG (0x26)
#define EXTENDED_ADDR_REG (0x3F)

/** AM08X5 Pin Values */
#define AM08X5_OUTPUT_NIRQ1 (0x01)
#define AM08X5_OUTPUT_NIRQ2 (0x02)
#define AM08X5_OUTPUT_TIRQ (0x03)
#define AM08X5_OUTPUT_FOUT (AM08X5_OUTPUT_NIRQ1)
#define AM08X5_SQW_OUT_OFF (0x00)
#define AM08X5_CRYSTAL_CONFIG_PIN (AM08X5_OUTPUT_NIRQ1)

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 * @enum Status
 * AM0805 Status
 */
typedef enum
{
  AM08X5_OK = 0,  /**< Status OK */
  AM08X5_FAIL = 1 /**< Status FAIL */
} eAM08X5Status_t;

/**
 * @enum Square Wave Output Select
 * Square Wave Output Select
 */
typedef enum
{
  SQFS_CENTURY = 0x00u,       /** Century */
  SQFS_32768 = 0x01u,         /** 32768 Hz */
  SQFS_8192 = 0x02u,          /** 8192 Hz */
  SQFS_4096 = 0x03u,          /** 4096 Hz */
  SQFS_2048 = 0x04u,          /** 2048 Hz */
  SQFS_1024 = 0x05u,          /** 1024 Hz */
  SQFS_512 = 0x06u,           /** 512 Hz */
  SQFS_256 = 0x07u,           /** 256Hz */
  SQFS_128 = 0x08u,           /** 128Hz */
  SQFS_64 = 0x09u,            /** 64Hz */
  SQFS_32 = 0x0Au,            /** 32Hz */
  SQFS_16 = 0x0Bu,            /** 16Hz */
  SQFS_8Hz = 0x0Cu,           /** 8Hz */
  SQFS_4Hz = 0x0Du,           /** 4Hz */
  SQFS_2Hz = 0x0Eu,           /** 2Hz */
  SQFS_1Hz = 0x0Fu,           /** 1Hz */
  SQFS_1_2Hz = 0x10u,         /** 1/2 Hz */
  SQFS_1_4Hz = 0x11u,         /** 1/4 Hz */
  SQFS_1_8Hz = 0x12u,         /** 1/8 Hz */
  SQFS_1_16Hz = 0x13u,        /** 1/16 Hz */
  SQFS_1_32Hz = 0x14u,        /** 1/32 Hz */
  SQFS_1_60Hz = 0x15u,        /** 1/60 Hz */
  SQFS_16384 = 0x16u,         /** 16385 Hz */
  SQFS_100 = 0x17u,           /** 100Hz */
  SQFS_1HR = 0x18u,           /** 1 Hour */
  SQFS_1DAY = 0x19u,          /** 1 Day */
  SQFS_TIRQ = 0x1Au,          /** TIRQ Tied */
  SQFS_NTIRQ = 0x1Bu,         /** nTIRQ Tied */
  SQFS_1YEAR = 0x1Cu,         /** 1 Year */
  SQFS_1HZ_TO_CNT = 0x1Du,    /** 1Hz To Counters */
  SQFS_1_32_FROM_ACAL = 0x1E, /** 1/32 Hz from Acal */
  SQFS_1_8_FROM_ACAL = 0x1F   /** 1/8 Hz from Acal */
} eAM08X5_SQFS_t;

/**
 * @enum Datestamp
 * Datestamp formatting
 */
typedef enum
{
  DS_DATE_NONE = 0x00,   /** No Datestamp */
  DS_MMsDDsYYYY = 0x01,  /** MM/DD/YYYY */
  DS_MMsDDsYY = 0x02,    /** MM/DD/YY */
  DS_MM_DD_YYYY = 0x03,  /** MM-DD-YYYY */
  DS_MM_DD_YY = 0x04,    /** MM-DD-YY */
  DS_YYYY_MM_DD = 0x05,  /** YYYY-MM-DD */
  DS_YY_MM_DD = 0x06,    /** YY-MM-DD */
  DS_YYsMMsDD = 0x07,    /** YY/MM/DD */
  DS_YYYYsMMsDD = 0x08,  /** YYYY/MM/DD */
  DS_DD_MMM_YYYY = 0x09, /** DD Mon YYYY (example, 12 Jul 2019)*/
  DS_EPOCH = 0x10,       /** POSIX/UNIX Timestamp */
} AM08X5_Datestamp_t;

/**
 * @enum Timestamp
 * Timestamp formatting
 */
typedef enum
{
  TS_TIME_NONE = 0x00,        /** No Timestamp */
  TS_TIME_HH_MM_SS = 0x01,    /** HH:MM:SS */
  TS_TIME_HH_MM_SS_SS = 0x02, /** HH:MM:SS.SSS */
  TS_TIME_HH = 0x03,          /** HH */
  TS_TIME_HH_MM = 0x04,       /** HH:MM */
  TS_EPOCH = 0xA0             /** POSIX/UNIX Timestamp */
} AM08X5_Timestamp_t;

/**
 * @struct Time Register Structure
 * Structure of Time Register Data
 */
typedef struct
{
  uint8_t hundredth;
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t date;
  uint8_t weekday;
  uint8_t month;
  uint8_t year;
  uint8_t century;
  uint8_t mode;
} sTimeReg_t;

/**
 * @struct Timer Structure
 * Structure keeping all timer information
 */
typedef struct
{
  sGPIO_t pin;         /**< Timer Interrupt Pin */
  uint8_t range;       /**< 0=Period in uSeconds, 1=Period in Seconds */
  uint8_t repeat;      /**< 0-6, See datasheet for interrupt output type */
  uint8_t RTCTimerPin; /**< 0=Disable, 1=nTIRQ, 2=nIRQ1+nTIRQ,3=nIRQ2+nTIRQ,4=CLKOUT+nTIRQ,5=CLKOUT(High)+nTIRQ(Low) */
  uint32_t period;     /**< Period of the counter */
} sAM08X5Timer_t;

/**
 * @struct Alarm Structure
 * Structure keeping all info necessary for alarms
 */
typedef struct
{
  sGPIO_t pin;          /**< IRQ Pin */
  sTimeReg_t alarm;     /**< Alarm Date/Time */
  sTimeReg_t nextAlarm; /**< Next Alarm Date/Time */
  sTimeReg_t repeat;    /**< Repeat frequency/offset */
  int16_t repeatCycles; /**< -1 = Inifinity, 0 = No Repeat, 1-32767 # Cycles to/left in Repeat */
  uint8_t RTCAlarmPin;  /**< Pin on Ambiq Alarm is Attached to */
} sAM08X5Alarm_t;

/**
 * @struct External Interrupt Pin
 * External Interrupt Pin for AM0805
 */
typedef struct
{
  sGPIO_t pin;
  sTimeReg_t timer;
} sAM08X5External_t;

/**
 * @struct AM0805 Time Structure
 * Time structure for AM0805 (Current Time, etc.)
 */
typedef struct
{
  sTimeReg_t time;       /**< Current time */
  char timestamp[32];    /**< Current timestamp */
  AM08X5_Datestamp_t ds; /**< User selected datestamp */
  AM08X5_Timestamp_t ts; /**< User selected timestamp */
} sAM08X5Time_t;

/**
 * @struct AM08X5 Calibration Structure
 * Calibration structure for offset in RTC
 */
typedef struct
{
  float freq;     /**< Measured Frequency of 32kHz Oscillator */
  int16_t offset; /**< Frequency offset */
  uint8_t calxt;
  uint8_t oscstat;
} sAM08X5Cal_t;

/**
 * @struct Ambiq AM08X5 RTC
 * Ambiq AM0805 RTC Structure
 */
typedef struct
{
  sI2C_t I2C;             /**< I2C Struct */
  sAM08X5Alarm_t nIRQ1;   /**< IRQ 1 Pin */
  sAM08X5Alarm_t nIRQ2;   /**< IRQ 2 Pin */
  sAM08X5External_t TIRQ; /**< Timer Interrupt */
  sAM08X5External_t EXTI; /**< External Interrupt Pin */
  sAM08X5External_t WDI;  /**< Watchdog Interrupt Pin */
  sAM08X5Time_t time;     /**< Current Time */
  sAM08X5Cal_t cal;       /**< Calibration Values */
  sAM08X5Timer_t timer;   /**< 1 Second Timer Pin */
} sAM08X5_t;

#ifdef UNIT_TESTING
extern PERSISTENT sAM08X5_t RTC;
#endif

extern sAM08X5_t RTC;

/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Initialize RTC
 *
 * Initialize the AM08X5 RTC
 *
 * @param None
 *
 * @return result
 */
eAM08X5Status_t AM08X5_Initialize(void);

/** @brief Set IRQ Pin
 *
 * Sets the RTC IRQ pin on the Micro
 *
 * @param None
 *
 * @return None
 */
void AM08X5_SetIRQ(void);

/** @brief Get RTC Register Value
 *
 * Retreive RTC Register Value
 *
 * @param reg Register to read
 * @param *temp Value in register
 *
 * @return None
 */
void AM08X5_GetRegisterValue(uint8_t reg, char *temp);

/** @brief Get Current Time
 *
 * Get the current time from the RTC
 *
 * @param *t Pointer to time struct
 *
 * @return None
 */
void AM08X5_GetTime(sTimeReg_t *t);

/** @brief Get Timestamp in ISO8601 Format
 *
 * Get the current timestamp in ISO8601 format.
 * YYYY-MM-DDTHH:MM:SSZ
 *
 * @param *t Pointer to the timestamp string to write to
 *
 * @return None
 */
void AM08X5_GetTimestamp_ISO8601(char *str);

/** @brief Get Timestamp in ISO8601 Format w/hundredths of seconds
 *
 * Get the current timestamp in ISO8601 format.
 * YYYY-MM-DDTHH:MM:SS.SSSZ
 *
 * @param *t Pointer to the timestamp string to write to
 *
 * @return None
 */
void AM08X5_GetTimestamp_ISO8601_w_hundredths(char *str);

/** @brief Get Timestamp
 *
 * Get the current timestamp
 *
 * @param *str Pointer to the timestamp string to write to
 * @param ds Datestamp format
 * @param ts Timestamp format
 *
 * @return None
 */
void AM08X5_GetTimestamp(char *str, AM08X5_Datestamp_t ds, AM08X5_Timestamp_t ts);

/** @brief Set Time
 *
 * Sets RTC time based on time string
 *
 * @param *tstr Pointer to date/time string
 *
 * @return result
 */
eAM08X5Status_t AM08X5_SetTime(char *tstr);

/** @brief RTC Crystal Frequency Offset Calculator
 *
 * Calculates the necessary frequency offset for the installed crsytal
 * and updates the registers on the RTC with calculated values.
 *
 * @brief frequency Frequency measured on output pin (~32.768kHz)
 *
 * @return result
 */
eAM08X5Status_t AM08X5_OffsetCalculation(float frequency);

/** @brief Set Crystal Calibration Signal Output
 *
 * Sets the output pin to the crystal frequency
 *
 * @pararm None
 *
 * @result None
 */
void AM08X5_CrystalCal_Set(void);

/** @brief Clear Crystal Calibration Signal Output
 *
 * Clears the crystal frequency output pin
 *
 * @param None
 *
 * @return None
 */
void AM08X5_CrystalCal_Clear(void);

/** @brief Set Alarm
 *
 * Set alarm and structure to requested date/time
 *
 * @param *alarm Pointer to Alarm structure
 * @param *t Pointer to Date/time structure to set alarm to
 *
 * @return None
 */
void AM08X5_AlarmSet(sAM08X5Alarm_t *alarm, sTimeReg_t *t);

/** @brief Get Alarm
 *
 * Get the time th alarm is currently set to.
 *
 * @param *alarm Pointer to alarm structure
 * @param *str String pointer to ISO time string returned
 *
 * @return None
 */
void AM08X5_AlarmGet_Str(sAM08X5Alarm_t *alarm, char *str);

/** @brief Set Alarm with Repeat
 *
 * Set alarm and structure to requested date/time and repeat as requested
 *
 * @param *alarm Pointer to Alarm structure
 * @param *t Pointer to Date/time structure to set alarm to
 * @param *rpt Pointer to Repeat offset time structure
 *
 * @return None
 */
void AM08X5_AlarmSet_with_Repeat(sAM08X5Alarm_t *alarm, sTimeReg_t *t, sTimeReg_t *rpt);

/** @brief Set Net Alarm
 *
 * Set alarm and structure to the next alarm request and update structure
 *
 * @param *alarm Pointer to Alarm structure
 *
 * @return None
 */
void AM08X5_AlarmSet_Next(sAM08X5Alarm_t *t);

/** @brief Clear Alarm
 *
 * Clear alarm and structure
 *
 * @param *alarm Pointer to alarm structure to clear.
 *
 * @return result
 */
void AM08X5_AlarmClear(sAM08X5Alarm_t *alarm);

/** @brief Set Timer
 *
 * Set the timer ability
 *
 * @param *t Pointer to timer struct
 * @param *delay Pointer to delay time structure
 *
 * @return result
 */
void AM08X5_TimerSet(sAM08X5Timer_t *t, sTimeReg_t *delay);

/** @brief Clear Timer
 *
 * Clear the timer and structure
 *
 * @param *t Pointer to timer struct
 *
 * @return result
 */
void AM08X5_TimerClear(sAM08X5Timer_t *t);

/** @brief Compare Two Time Structs
 *
 * Compares two time structures.  If T1 > T2, response is TRUE, otherwise FALSE
 *
 * @param t1 Time Structure to compare against
 * @param t2 Time Structure comparing with
 *
 * @return True or False
 */
uint8_t AM08X5_compare_time(sTimeReg_t *t1, sTimeReg_t *t2);

/** Compare the time strucutres.
 *
 * Compares the two time structures and return the results.
 *
 * @param *t1 Pointer to time reg #1
 * @param *t2 Pointer to time reg #2
 *
 * @return result (FALSE for t1 <= t2, TRUE for t1 > t2)
 */
uint8_t AM08X5_compare_time_non_epoch(sTimeReg_t *t1, sTimeReg_t *t2);
/** @brief Clear Time Struct
 *
 * Clears the Time Structure.
 *
 * @param t Time Structure
 *
 * @return None
 */
void AM08X5_clear_time_struct(sTimeReg_t *t);

/** @brief Convert seconds to time struct
 *
 * Converts an integer second count into a provided time structure
 *
 * @param *t Pointer to time struct
 * @param seconds Total number of seconds
 *
 * @return result
 */
void AM08X5_seconds_to_time_struct(sTimeReg_t *t, uint32_t seconds);

/** @brief Sum Two Time Structures
 *
 * Sums two time structures and stores result in the third
 *
 * @param *t1 Pointer to first time structure
 * @param *t2 Pointer to second time structure
 * @param *t3 Pointer to sum of t1 + t2
 *
 * @return result;
 */
eAM08X5Status_t AM08X5_add_time_struct(sTimeReg_t *t1, sTimeReg_t *t2, sTimeReg_t *t3);

/** @brief Convert Timestamp to Time Structure
 *
 * Converts an IEEE formatted time string to a Time Structure
 *
 * @param *str Pointer to timestamp string
 * @param *t Pointer to time structure
 *
 * @return result
 */
eAM08X5Status_t AM08X5_convert_timestamp(char *ts, sTimeReg_t *t);

/** Convert ISO8601 timestamp to time struct
 *
 * Convert a user provided ISO8601 time stamp to a time structure
 *
 * @param *ts Pointer to ISO8601 time stamp string
 * @param *t Pointer to time structure
 *
 * @return Result of conversion.  True for pass, False for fail
 */
eAM08X5Status_t AM08X5_convert_timestamp_ISO8601(char *ts, sTimeReg_t *t);

/** Convert time only timestamp to time structure
 *
 * Convert a user provided, time-only, timestamp to a time structure
 *
 * @param *ts Pointer to time only timestamp string
 * @param *t Pointer to time structure
 *
 * @return Result of conversion.  True for pass, False for fail.
 */
eAM08X5Status_t AM08X5_convert_timestamp_timeonly(char *ts, sTimeReg_t *t);

/** @brief Create ISO Timestamp from Time Structure
 *
 * Create an IEEE formatted time string from a Time Structure
 *
 * @param *str Pointer to timestamp string
 * @param *t Pointer to time structure
 *
 * @return result
 */
eAM08X5Status_t AM08X5_get_timestamp(char *ts, sTimeReg_t *t);

/** @brief Compare two times
 *
 * Compare two time structures to determine which is greater
 *
 * @param *t1 Pointer to first time structure
 * @param *t2 Pointer to second time structure
 *
 * @return Result.  True for t1 > t2, False for T2 <= T1
 */
uint8_t AM08X5_compare_time(sTimeReg_t *t1, sTimeReg_t *t2);

/** @brief Convert BCD to Dec
 *
 * Convert Binary Coded Decimal (BCD) to Decimal format
 *
 * @param *t Pointer to time struct to convert
 *
 * @return None
 */
void AM08X5_BCD_to_Dec(sTimeReg_t *t);

/** @brief Convert Dec to BCD
 *
 * Convert Decimal to Binary Coded Decimal (BCD) format
 *
 * @param *t Pointer to time struct to convert
 *
 * @return None
 */
void AM08X5_DEC_to_BCD(sTimeReg_t *t);

/** @brief Set RTC Charging ON
 *
 * Turn the charging circuit on for the RTC battery
 *
 */
void AM08X5_Charge_ON(void);

/** @brief Set RTC Charging OFF
 *
 * Turn the charging circuit off for the RTC battery
 *
 */
void AM08X5_Charge_OFF(void);

/** @brief Set RTC to RC clock
 *
 * Turn the low power RC clock on for the RTC
 *
 */
void AM08X5_Autocal_RC_Set(void);

/** @brief Set RTC to RC clock
 *
 * Turn the low power RC clock off for the RTC
 *
 */
void AM08X5_Autocal_RC_Clear(void);
#endif // _AM08X5_H
