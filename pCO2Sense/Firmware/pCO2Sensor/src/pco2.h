/** @file PCO2.h
 *  @brief PCO2 Module
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 17, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note System functions for PCO2 Sampling
 *
 *
 *  @bug  No known bugs
 */
#ifndef _PCO2_H
#define _PCO2_H

/** Remove STATIC and extern values if running TEST */
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
#include <math.h>
#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
#ifdef UNIT_TESTING
#include <stdio.h>
#endif

/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "AM08x5.h"
#include "LI8x0.h"
#include "max250.h"
#include "flowcontrol.h"
#include "statistics.h"

#include "errorlist.h"
//#include "rh.h"
#include "sht35.h"
#include "scheduler.h"
/************************************************************************
 *							MACROS
 ************************************************************************/
#define PCO2_STR_LEN (512)
#define PCO2_REPEAT_FOREVER (-1)
#define PCO2_REPEAT_NONE (0)
/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 * @enum PCO2 Status
 * @brief PCO2 Status
 */
typedef enum
{
  PCO2_OK = 0u,  /**< PCO2 Passed operation */
  PCO2_FAIL = 1u /**< PCO2 Failed operation */
} ePCO2Status_t;

/**
 * @enum PCO2 Mode
 * @brief PCO2 Mode
 */
typedef enum
{
  /** ZERO States */
  MODE_ZERO_PUMP_ON = 0x01,
  MODE_ZERO_PUMP_OFF = 0x02,
  MODE_ZERO_VENT = 0x03,
  MODE_ZERO_POST_CAL = 0x04,
  /** Span States */
  MODE_SPAN_GAS_ON = 0x06,
  MODE_SPAN_GAS_OFF = 0x07,
  MODE_SPAN_VENT = 0x08,
  MODE_SPAN_POST_CAL = 0x09,
  /** Span 2 States */
  MODE_SPAN2_GAS_ON = 0x0B,
  MODE_SPAN2_GAS_OFF = 0x0C,
  MODE_SPAN2_VENT = 0x0D,
  MODE_SPAN2_POST_CAL = 0x0E,
  /** Equilibrate States */
  MODE_EQUILIBRATE_PUMP_ON = 0x10,
  MODE_EQUILIBRATE_PUMP_OFF = 0x11,
  MODE_EQUILIBRATE_VENT = 0x12,
  MODE_EQUILIBRATE_POST_VENT = 0x13,
  /** Air States */
  MODE_AIR_PUMP_ON = 0x15,
  MODE_AIR_PUMP_OFF = 0x16,
  MODE_AIR_VENT = 0x17,
  MODE_AIR_POST_VENT = 0x018,
  /** Rest & Deploy States */
  MODE_REST = 0x20,
  MODE_DEPLOY = 0x21,
  /** Vent/Dry States */
  MODE_VENT = 0x23,
  /** Auto-Pressurize State */
  MODE_PRESSURIZE = 0x40,
  /** Purge + Clear input State */
  MODE_PRG1 = 0x24,
  MODE_PRG2 = 0x25,
  MODE_PRG3 = 0x26,
  MODE_PRG4 = 0x27,
  MODE_PRG5 = 0x28,
  MODE_PRG6 = 0x29,
  MODE_PRG7 = 0x30,
  MODE_PRG8 = 0x31,
  // MODE_PURGE = 0x32,
  // MODE_CLEAR = 0x25

} ePCO2Mode_t;

/**
 * @enum PCO2 Cal Mode
 * @brief PCO2 Cal Mode
 */
typedef enum
{
  PCO2_CAL_ZERO = 0, /**< Calibrate ZERO */
  PCO2_CAL_SPAN = 1, /**< Calibrate SPAN 1 */
  PCO2_CAL_SPAN2 = 2 /**< Calibrate SPAN 2 */
} ePCO2Cal_t;

/**
 * @enum PCO2 Sample Mode
 * @brief PCO2 Sample Mode
 */
typedef enum
{
  PCO2_SAMPLE_MODE_RUN = 0x00u,    /**< Generic Run Mode */
  PCO2_SAMPLE_MODE_NORMAL = 0x01u, /**< Normal Sample Mode */
  PCO2_SAMPLE_MODE_FAST = 0x02u,   /**< Fast Sample Mode */
  PCO2_SAMPLE_MODE_DEPLOY = 0x04u  /**< Deployment Mode */
} ePCO2SampleMode_t;

typedef struct
{
  bool mconfig;
  float ssct;
  float sscsc;
  char lsstds[24];
  double sstds;
  float sstdsf;
  float ssconc;
  char avalid[24];
  float pbias;
  char lpbm[24];
  float sscrh;
  char atrhs[24];
  char o2s[24];
  char manufacturer[24];
  char sscssn[24];
  char sssn[24];
  char ssn[24];
  char lssc[24];
} sManufacturer_t;

/**
 * @struct PCO2 Struct
 * @brief PCO2 Struct
 */
typedef struct pco2_t
{
  ePCO2SampleMode_t SampleMode; /**< Current Sample Mode */
  sTimeReg_t NormStart;         /**< Normal Mode Start Time for next run */
  sTimeReg_t NormInterval;      /**< Normal Mode Cycle Interval */
  sTimeReg_t FastInterval;      /**< Fast Mode Cycle Interval */
  sTimeReg_t FastChange;        /**< Fast Mode length before changing to normal mode */
  sTimeReg_t FastLength;        /**< Time left before changing to Normal Mode */
  sTimeReg_t O2Interval;        /**< O2 Calibration Cycle Interval */
  sTimeReg_t O2Time;            /**< O2 Calibration Time of Day */
  sTimeReg_t RunLength;         /**< Length of a single run with current settings */
  uint8_t sample;               /**< Sampling frequency (Hz).  1, 2, or 4. */
  struct
  {
    uint16_t equil;   /**< Total Equilibration Time (seconds) */
    uint16_t prequil; /**< Equilbrator time pumpon */
  } equil;
  struct
  {
    uint16_t pumpon;   /**< Pump on time during cycle (seconds) */
    uint16_t pumpoff;  /**< Pump off time before sample (seconds) */
    uint16_t pumpwait; /**< Wait time at start of pumpoff */
  } pump;
  struct
  {
    uint16_t flowon; /**< Span Flow ON duration (seconds) */
  } spanflow;
  
  struct
  {
    uint16_t purge; /**< Span Flow ON duration (seconds) */
  } purge;
  
  sLicorData_t LIData; /**< Most recent Licor Read */
  uint16_t sampleCO2;  /**< CO2 Sample time (seconds) */
  uint16_t LI_warmup;  /**< Licor warmup time (seconds) */
  uint8_t LI_heater;   /**< Licor Heater on of off (0=off, 1=on) */
  uint8_t zero_on;
  uint8_t span_on;
  uint16_t vent;                     /**< Length of time to vent before closing (seconds) */
  volatile sSysTimer_t *timeout1;    /**< Seconds countdown timer 1 */
  volatile sSysTimer_t *timeout2;    /**< Seconds countdown timer 2 */
  volatile sSysTimer_t *sampleTimer; /**< Sample seconds countdown timer */
  volatile uint16_t *cal_timeout;    /**< Calibration seconds countdown timer */
  float spandiff;                    /**< Span Gas Pressure Difference */
  float span;                        /**< Span Gas Concentration */
  float span2;                       /**< Span2 Gas Concentration */
  float bbc;                         /**< Band Broadening Coefficient */
  sManufacturer_t man;
  char mode[8];
} sPCO2_t;

/**
 *
 *
 */
typedef struct
{
  struct
  {
    float li_co2;         /** Span Off CO2 mean (ppm) */
    float li_temperature; /** Span Off mean temperature (degC) */
    float li_pressure;    /** Span Off Pressure (kPa) */
    float rh_temperature; /** Span Off RH mean (%) */
    float rh_rh;          /** Span Off RH mean (%) */
  } span;
  struct
  {
    float li_co2;         /** Equilibrator Off CO2 mean (ppm) */
    float li_temperature; /** Equilibrator Off mean temperature (degC) */
    float li_pressure;    /** Equilibrator Off Pressure (kPa) */
    float rh_temperature; /** Equilibrator Off RH mean (%) */
    float rh_rh;          /** Equilibrator Off RH mean (%) */
  } equil;
  struct
  {
    float li_co2;         /** Air Off CO2 mean (ppm) */
    float li_temperature; /** Air Off mean temperature (degC) */
    float li_pressure;    /** Air Off Pressure (kPa) */
    float rh_temperature; /** Air Off RH mean (%) */
    float rh_rh;          /** Air Off RH mean (%) */
  } air;
  float sw_xco2;  /** Seawater Dry CO2 (ppm) */
  float air_xco2; /** Atmospheric air Dry CO2 (ppm) */
  struct
  {
    float co2kzero;
    float co2kspan;
    float co2kspan2;
  } co2;
} sPCO2Dry_t;

#define PCO2_SAMPLES_PER_MODE (120)
typedef struct PCO2_run_data_t
{
  uint8_t sample_idx;
  char serialnum[12];
  uint8_t mode[PCO2_SAMPLES_PER_MODE];
  // time_t timestamp[PCO2_SAMPLES_PER_MODE];
  sTimeReg_t timestamp[PCO2_SAMPLES_PER_MODE];
  float CO2[PCO2_SAMPLES_PER_MODE];
  float Li_temp[PCO2_SAMPLES_PER_MODE];
  float Li_pres[PCO2_SAMPLES_PER_MODE];
  uint32_t Li_raw[PCO2_SAMPLES_PER_MODE];
  uint32_t Li_ref[PCO2_SAMPLES_PER_MODE];
  float RH[PCO2_SAMPLES_PER_MODE];
  float RH_T[PCO2_SAMPLES_PER_MODE];
  float O2[PCO2_SAMPLES_PER_MODE];
  float co2kzero[PCO2_SAMPLES_PER_MODE];
  float co2kspan[PCO2_SAMPLES_PER_MODE];
  float co2kspan2[PCO2_SAMPLES_PER_MODE];
} sPCO2_run_data_t;

/** @brief Full run data struct
 *
 */
typedef struct
{
  sPCO2_run_data_t ZPOFF;
  sPCO2_run_data_t ZPON;
  sPCO2_run_data_t ZPPCAL;
  sPCO2_run_data_t SPOFF;
  sPCO2_run_data_t SPON;
  sPCO2_run_data_t SPPCAL;
  sPCO2_run_data_t EPOFF;
  sPCO2_run_data_t EPON;
  sPCO2_run_data_t APOFF;
  sPCO2_run_data_t APON;
  sPCO2Dry_t DRY;
  char ERRORS[40];
} sPCO2_sdata_t;

typedef struct
{
  sStats_F_t temperature;
  sStats_F_t pressure;
  sStats_F_t co2;
  sStats_F_t o2;
  sStats_F_t rh;
  sStats_F_t rh_temperature;
  sStats_F_t raw1;
  sStats_F_t raw2;
  char timestamp[23];
} sPCO2_idata_mode_t;

typedef struct
{
  sPCO2_idata_mode_t ZPOFF;
  sPCO2_idata_mode_t ZPON;
  sPCO2_idata_mode_t ZPPCAL;
  sPCO2_idata_mode_t SPOFF;
  sPCO2_idata_mode_t SPON;
  sPCO2_idata_mode_t SPPCAL;
  sPCO2_idata_mode_t EPOFF;
  sPCO2_idata_mode_t EPON;
  sPCO2_idata_mode_t APOFF;
  sPCO2_idata_mode_t APON;
  struct
  {
    float co2kzero;
    float co2kspan;
    float co2kspan2;
  } COEFF;
  float sw_xco2;  /**< Seawater Dry CO2 (ppm) */
  float air_xco2; /**< Atmospheric air Dry CO2 (ppm) */
} sPCO2_idata_t;

/************************************************************************
 *							  Externs
 ************************************************************************/
#ifdef UNIT_TESTING
extern sPCO2_t pco2;
extern char pco2_str[512];
extern sStats_F_t pco2_li_co2;
extern sStats_F_t pco2_li_pres;
extern sStats_F_t pco2_li_temp;
extern sStats_U16_t pco2_li_raw1;
extern sStats_U16_t pco2_li_raw2;
extern sStats_F_t pco2_rh_rh;
extern sStats_F_t pco2_rh_temp;
extern sStats_F_t pco2_rh_pres;
extern sStats_F_t pco2_o2_o2;
#endif
/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Initialize the
 *
 * Run the pCO2 sample
 *
 * @param None
 *
 * @return None
 */
void PCO2_Initialize(void);

/** @brief Sensor to Low Power Mode
 *
 * Run the pCO2 Sensor in Low Power Mode
 *
 * @param None
 *
 * @return None
 */
void PCO2_LowPowerMode(void);

/** @brief Run PCO2 in Fast Mode
 *
 * Run the PCO2 sample in fast mode
 *
 * @param None
 *
 * @return None
 */
void PCO2_FastMode(void);

/** @brief Run PCO2 in Normal Mode
 *
 * Run the PCO2 sample in normal mode
 *
 * @param None
 *
 * @return None
 */
void PCO2_NormalMode(void);

/** @brief Start a Normal Mode run NOW
 *
 * Run the PCO2 sample in normal mode
 *
 * @param None
 *
 * @return None
 */
void PCO2_InstantRun(void);
void PCO2_InstantRun_with_Purge(void);

/** @brief Run PCO2 Routine
 *
 * Run the pCO2 sample routine
 *
 * @param purge Perform Purge at end of run(t/f)
 *
 * @return result
 */
void PCO2_Run(bool purge);

/** @brief Set Deploy Mode
 *
 * Put the PCO2 sensor in deployment mode
 *
 * @param None
 *
 * @return None
 */
ePCO2Status_t PCO2_Deploy(void);

/** @brief Get Sample Mode
 *
 * Get the PCO2 Sample Mode
 *
 * @param None
 *
 * @return Sample Mode
 */
ePCO2SampleMode_t PCO2_GetSampleMode(void);

/** @brief Set O2 Interval
 *
 * Set the O2 Cal Interval
 *
 * @param Pointer to time struct
 *
 * @return None
 */
void PCO2_SetO2Interval(sTimeReg_t *t);

/** @brief Set O2 Interval
 *
 * Set the O2 Cal Interval
 *
 * @param Pointer to time struct
 *
 * @return None
 */
void PCO2_GetO2Interval(sTimeReg_t *t);

/** @brief Set O2 Calibration Time
 *
 * Set the O2 calibration time
 *
 * @param *t Pointer to time struct
 *
 * @return None
 */
void PCO2_SetO2Time(sTimeReg_t *t);

/** @brief Get O2 Calibration Time
 *
 * Get the O2 calibration time
 *
 * @param *t Pointer to time struct
 *
 * @return None
 */
void PCO2_GetO2Time(sTimeReg_t *t);

/** @brief Calculate Number or Repeats in Interval
 *
 * Calculate the number of fast mode runs.
 *
 * Repeats = Length in Fast Mode / Fast Mode Interval
 *
 * @param None
 *
 * @return value Number or Repeats
 */
uint16_t PCO2_FastMode_CalculateRepeats(void);

/** @brief Get length of PCO2 Run
 *
 * Return the length of a PCO2 Run with current configuration
 *
 * @param *run Pointer to run time structure
 *
 * @return None
 */
void PCO2_GetRunLength(sTimeReg_t *run);

/** @brief Get Normal Mode Interval
 *
 * Return the length of the normal mode interval
 *
 * @param *t Pointer to time structure
 *
 * @return None
 */
void PCO2_GetNormalInterval(sTimeReg_t *t);

/** @brief Get Normal Start Time
 *
 * Return the normal mode Start Time
 *
 * @param *t Pointer to time structure
 *
 * @return None
 */
void PCO2_GetNormalStartTime(sTimeReg_t *t);

/** @brief Get Fast Mode Interval
 *
 * Return the length of the fast mode interval
 *
 * @param *t Pointer to time structure
 *
 * @return None
 */
void PCO2_GetFastInterval(sTimeReg_t *t);

/** @brief Get Fast Mode run time
 *
 * Get the time to run the sensor in fast mode before switching to normal mode
 *
 * @param *t Pointer to time struct
 *
 * @return None
 */
void PCO2_GetChange(sTimeReg_t *t);

/** @brief Get Sample Length
 *
 * Get the co2 sample Frequency
 *
 * @param None
 *
 * @return sample frequency (Hz)
 */
uint8_t PCO2_GetSample(void);

/** @brief Get Equilibration time
 *
 * Get the equilibration time
 *
 * @param None
 *
 * @return equilibration time (seconds)
 */
uint32_t PCO2_GetEquil(void);

/** @brief Get Licor Warmup Time
 *
 * Get the Licor Warmup Time
 *
 * @param None
 *
 * @return warmup time (seconds)
 */
uint32_t PCO2_GetWarmup(void);

/** @brief Get Span Flow ON Time
 *
 * Get the length of seconds the span gas flows
 *
 * @param None
 *
 * @return Span FLow ON time (seconds)
 */
uint32_t PCO2_GetPurge(void);

/** @brief Get Purge Time
 *
 * Get the length of seconds in Purge mode
 * Value is duplicated for Purge and Clear 
 * 
 * @param None
 *
 * @return Purge time (seconds)
 */
uint32_t PCO2_GetSpanFlowON(void);


/** @brief Get Pump ON Time
 *
 * Get the Pump ON TIme
 *
 * @param None
 *
 * @return Pump ON time (seconds)
 */
uint32_t PCO2_GetPumpON(void);

/** @brief Get Pump OFF Time
 *
 * Get the Pump OFF TIme
 *
 * @param None
 *
 * @return Pump OFF time (seconds)
 */
uint32_t PCO2_GetPumpOFF(void);

/** @brief Get Sample Length
 *
 * Get the length of time of a co2 sample
 *
 * @param None
 *
 * @return sample length (seconds)
 */
uint32_t PCO2_GetSampleCO2(void);

/** @brief Get Vent Time
 *
 * Get the length of the vent time
 *
 * @param None
 *
 * @return vent length (seconds)
 */
uint32_t PCO2_GetVent(void);

/** @brief Get Heater State
 *
 * Get the state of the heater
 *
 * @param None
 *
 * @return heater state (0=Off, 1=On)
 */
uint8_t PCO2_GetHeater(void);

/** @brief Get Span Diff
 *
 * Get the Pressure Differential required before a Span can be completed
 *
 * @param None
 *
 * @return span diff (kPa)
 */
float PCO2_GetSpanDiff(void);

/** @brief Get Span Concentration
 *
 * Get the concentration of span gas used to calibrate (ppm)
 *
 * @param None
 *
 * @return concentration (ppm)
 */
float PCO2_GetSpan(void);

/** @brief Get Span2 Concentration
 *
 * Get the concentration of span2 gas used to calibrate (ppm)
 *
 * @param None
 *
 * @return concentration (ppm)
 */
float PCO2_GetSpan2(void);

/**  @brief Get Licor Coefficients
 *
 * Get the Licor Coefficients
 *
 * @param *str Pointer to read string from Licor
 * @param *len Length of string
 * @param len_max Max length of string
 *
 *
 * @return None
 */
void PCO2_GetCoef(void);

/** @brief Set Normal Interval
 *
 * Set the normal interval.
 *
 * @param *t Pointer to time register setting Normal Interval
 *
 * @return None;
 */
void PCO2_SetNormalInterval(sTimeReg_t *t);

/** @brief Set Change from Fast Mode
 *
 * Set the length of time to sample in Fast Mode before changing to Normal Mode
 *
 * Max hours = 99
 * Max minutes = 59
 * Max seconds = 59
 *
 * @param *t Pointer to time struct.
 *
 * @return None
 */
void PCO2_SetChange(sTimeReg_t *t);

/** @brief Set Sample Frequency
 *
 * Set the sample frequency of the sensors.  Can be 1, 2 or 4 (Hz)
 *
 * @param freq Sample Frequency
 *
 * @return None
 */
void PCO2_SetSample(uint8_t freq);

/** @brief Set Span Diff Pressure
 *
 * Set the pressure difference before span can be performed in kPa.
 *
 * Value of 0 will indicated span always performed.
 *
 * @param diff Pressure difference for span (in kPa)
 *
 * @return None
 */
void PCO2_SetSpanDiff(float diff);

/** @brief Set Span
 *
 * Set the span concentration in ppm.
 *
 * @param ppm Concentration of span gas (ppm)
 *
 * @return None
 */
void PCO2_SetSpan(float ppm);

/** @brief Set Span2
 *
 * Set the span2 concentration in ppm.
 *
 * @param ppm Concentration of span2 gas (ppm)
 *
 * @return None
 */
void PCO2_SetSpan2(float ppm);

/** @brief Equilibration Time Length
 *
 * Length of equilibration time in seconds
 *
 * @param seconds Length of equilibration
 *
 * @return None
 */
void PCO2_SetEquil(uint32_t seconds);

/** @brief Licor Warmup Time
 *
 * Length of time for Licor Warmup
 *
 * @param seconds Length of Licor Warmup
 *
 * @return None
 */
void PCO2_SetWarmup(uint32_t seconds);

/** @brief Span Flow ON Time
 *
 * Length of time for the span gas to flow
 *
 * @param seconds Length of span flow on
 *
 * @return None
 */
void PCO2_SetSpanFlowON(uint32_t seconds);

/** @brief Set Purge Time
 *
 * Length of time for the purge
 *
 * @param seconds Length of purge flow on or off
 *
 * @return None
 */
void PCO2_SetPurge(uint32_t seconds);

/** @brief Pump ON Time
 *
 * Length of pump ON time during cycles
 *
 * @param seconds Length of pump on Time (seconds)
 *
 * @return None
 */
void PCO2_SetPumpON(uint32_t seconds);

/** @brief Pump OFF Time
 *
 * Length of pump OFF time during cycles
 *
 * @param seconds Length of pump off Time (seconds)
 *
 * @return None
 */
void PCO2_SetPumpOFF(uint32_t seconds);

/** @brief Set CO2 Sample Time
 *
 * Length of time to sample CO2 during cycles
 *
 * @param seconds Length of co2 sample time (seconds)
 *
 * @return None
 */
void PCO2_SetSampleCO2(uint32_t seconds);

/** @brief Set Vent Open time
 *
 * Length of time to vent after pumping in seconds
 *
 * @param seconds Length of vent time (seconds)
 *
 * @return None
 */
void PCO2_SetVent(uint32_t seconds);

/** @brief Set Heater On/Off
 *
 * Set the Licor Heater ON (1) or OFF (0)
 *
 * @param state Heater state (0=OFF, 1=ON)
 *
 * @return None
 */
void PCO2_SetHeater(uint8_t state);

/** @brief Schedule O2 Calibration Time
 *
 * Schedule the O2 calibration time
 *
 * @param None
 *
 * @return None
 */
void PCO2_ScheduleO2Cal(void);

/** @brief Calibrate O2 Sensor
 *
 * Calibrate the O2 Sensor
 *
 * @param None
 *
 * @return None
 */
void PCO2_CalO2(void);

/** @brief Initialize Basic Tasks
 *
 * Initialize the basic PCO2 tasks
 *
 * @param None
 *
 * @return None
 */
void PCO2_Task_Init(void);

/** @brief Stream sensor data
 *
 * Stream Sensor Data to console
 *
 * @param None
 *
 * @return None
 */
void PCO2_StreamSensors(void);

/** @brief Run the Zero Calibration
 *
 *
 *
 * @param None
 *
 * @return Status
 */
ePCO2Status_t PCO2_State_Zero(void);

/** @brief Run the Span Calibration
 *
 *
 *
 * @param span Span concentration
 *
 * @return Status
 */
ePCO2Status_t PCO2_State_Span(float span);

/** @brief Run the System Configuration
 *
 *
 *
 * @param None
 *
 * @return Status
 */
ePCO2Status_t PCO2_State_Config(void);

/** @brief Run the Purge routine
 *
 * @param None
 * 
 * @return Status
 */
ePCO2Status_t PCO2_State_Purge(void);

/** @brief Calculate Run Time
 *
 * Calculate how long the system takes to run.
 *
 * @param *t Pointer to time structure
 *
 * @return None
 */
void PCO2_Calculate_RunTime(sTimeReg_t *t);

/** @brief Run Instanly
 *
 * Not often used
 */
void PCO2_InstantRun_Test(void);

void PCO2_Get_sdata(void);

void PCO2_Get_idata(void);

void PCO2_Set_mconfig(bool val);
void PCO2_Set_ssct(float val);
void PCO2_Set_sscsc(float val);
void PCO2_Set_lsstds(char *val);
void PCO2_Set_sstds(double val);
void PCO2_Set_sstdsf(float val);
void PCO2_Set_avalid(char *val);
void PCO2_Set_pbias(float val);
void PCO2_Set_lpbm(char *val);
void PCO2_Set_sscrh(float val);
void PCO2_Set_atrhs(char *val);
void PCO2_Set_o2s(char *val);
void PCO2_Set_manufacturer(char *val);
void PCO2_Set_sscssn(char *val);
void PCO2_Set_sssn(char *val);
void PCO2_Set_ssn(char *val);
void PCO2_Set_lssc(char *val);

bool PCO2_Get_mconfig(void);
float PCO2_Get_ssct(void);
float PCO2_Get_sscsc(void);
void PCO2_Get_lsstds(char *val);
double PCO2_Get_sstds(void);
float PCO2_Get_sstdsf(void);
float PCO2_Get_ssconc(void);
void PCO2_Get_avalid(char *val);
float PCO2_Get_pbias(void);
void PCO2_Get_lpbm(char *val);
float PCO2_Get_sscrh(void);
void PCO2_Get_atrhs(char *val);
void PCO2_Get_o2s(char *val);
void PCO2_Get_manufacturer(char *val);
void PCO2_Get_sscssn(char *val);
void PCO2_Get_sssn(char *val);
void PCO2_Get_ssn(char *val);
void PCO2_Get_lssc(char *val);

#endif // _PCO2_H