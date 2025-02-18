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
 *	@note System functions for PCO2 Sampling
 *
 *
 *  @bug  No known bugs
 */

#include "pco2.h"
#include "logging.h"
/************************************************************************
 *					        Static Variables
 ************************************************************************/
const char PCO2_StandardHeader[] = "minute,t,t_std,press,p_std,pco2,pco2_std,o2,o2_std,rh,rh_std,rh_t,rh_t_std,raw_1,raw_1_std,raw_2,raw_2_std";
STATIC char pco2_str[PCO2_STR_LEN];
STATIC bool SPAN_SKIPPED;
PERSISTENT char PCO2_IData[4096];

PERSISTENT sStats_F_t pco2_li_co2;
PERSISTENT sStats_F_t pco2_li_pres;
PERSISTENT sStats_F_t pco2_li_temp;
PERSISTENT sStats_F_t pco2_li_raw1;
PERSISTENT sStats_F_t pco2_li_raw2;

PERSISTENT sStats_F_t pco2_rh_rh;
PERSISTENT sStats_F_t pco2_rh_temp;
PERSISTENT sStats_F_t pco2_rh_pres;
PERSISTENT sStats_F_t pco2_o2_o2;

PERSISTENT uint32_t db_val;
PERSISTENT uint32_t db_val2;

PERSISTENT sPCO2_t pco2 = {
    .SampleMode = PCO2_SAMPLE_MODE_FAST,
    .NormStart = {.year = DEFAULT_PCO2_NORM_START_YEAR,
                  .month = DEFAULT_PCO2_NORM_START_MONTH,
                  .date = DEFAULT_PCO2_NORM_START_DATE,
                  .hour = DEFAULT_PCO2_NORM_START_HOUR,
                  .minute = DEFAULT_PCO2_NORM_START_MINUTE,
                  .second = DEFAULT_PCO2_NORM_START_SECOND},
    .NormInterval = {
        .year = DEFAULT_PCO2_NORM_INTERVAL_YEAR,
        .month = DEFAULT_PCO2_NORM_INTERVAL_MONTH,
        .date = DEFAULT_PCO2_NORM_INTERVAL_DATE,
        .hour = DEFAULT_PCO2_NORM_INTERVAL_HOUR,
        .minute = DEFAULT_PCO2_NORM_INTERVAL_MINUTE,
        .second = DEFAULT_PCO2_NORM_INTERVAL_SECOND},
    .FastInterval = {.year = DEFAULT_PCO2_FAST_INTERVAL_YEAR, .month = DEFAULT_PCO2_FAST_INTERVAL_MONTH, .date = DEFAULT_PCO2_FAST_INTERVAL_DATE, .hour = DEFAULT_PCO2_FAST_INTERVAL_HOUR, .minute = DEFAULT_PCO2_FAST_INTERVAL_MINUTE, .second = DEFAULT_PCO2_FAST_INTERVAL_SECOND},
    .FastChange = {.year = DEFAULT_PCO2_FAST_CHANGE_YEAR, .month = DEFAULT_PCO2_FAST_CHANGE_MONTH, .date = DEFAULT_PCO2_FAST_CHANGE_DATE, .hour = DEFAULT_PCO2_FAST_CHANGE_HOUR, .minute = DEFAULT_PCO2_FAST_CHANGE_MINUTE, .second = DEFAULT_PCO2_FAST_CHANGE_SECOND},
    .FastLength = {.year = DEFAULT_PCO2_FAST_LENGTH_YEAR, .month = DEFAULT_PCO2_FAST_LENGTH_MONTH, .date = DEFAULT_PCO2_FAST_LENGTH_DATE, .hour = DEFAULT_PCO2_FAST_LENGTH_HOUR, .minute = DEFAULT_PCO2_FAST_LENGTH_MINUTE, .second = DEFAULT_PCO2_FAST_LENGTH_SECOND},
    .O2Interval = {.year = DEFAULT_PCO2_O2_INTERVAL_YEAR, .month = DEFAULT_PCO2_O2_INTERVAL_MONTH, .date = DEFAULT_PCO2_O2_INTERVAL_DATE, .hour = DEFAULT_PCO2_O2_INTERVAL_HOUR, .minute = DEFAULT_PCO2_O2_INTERVAL_MINUTE, .second = DEFAULT_PCO2_O2_INTERVAL_SECOND

    },
    .O2Time = {.year = DEFAULT_PCO2_O2_TIME_YEAR, .month = DEFAULT_PCO2_O2_TIME_MONTH, .date = DEFAULT_PCO2_O2_TIME_DATE, .hour = DEFAULT_PCO2_O2_TIME_HOUR, .minute = DEFAULT_PCO2_O2_TIME_MINUTE, .second = DEFAULT_PCO2_O2_TIME_SECOND

    },
    .RunLength = {.year = DEFAULT_PCO2_RUN_LENGTH_YEAR, .month = DEFAULT_PCO2_RUN_LENGTH_MONTH, .date = DEFAULT_PCO2_RUN_LENGTH_DATE, .hour = DEFAULT_PCO2_RUN_LENGTH_HOUR, .minute = DEFAULT_PCO2_RUN_LENGTH_MINUTE, .second = DEFAULT_PCO2_RUN_LENGTH_SECOND},
    .equil = {.equil = DEFAULT_PCO2_EQUIL, .prequil = DEFAULT_PCO2_PREQUIL},
    .pump = {.pumpon = DEFAULT_PCO2_PUMPON, .pumpoff = DEFAULT_PCO2_PUMPOFF},
    .spanflow = {.flowon = DEFAULT_PCO2_SPANFLOWON},
    .purge = {.purge = DEFAULT_PCO2_PURGE},
    .sampleCO2 = DEFAULT_PCO2_SAMPLECO2,
    .LI_warmup = DEFAULT_PCO2_LI_WARMUP,
    .LI_heater = DEFAULT_PCO2_LI_HEATER,
    .zero_on = true,
    .span_on = true,
    .vent = DEFAULT_PCO2_VENT,
    .spandiff = DEFAULT_PCO2_SPANDIFF,
    .span = DEFAULT_PCO2_SPAN,
    .span2 = DEFAULT_PCO2_SPAN2,
    .timeout1 = &sysinfo.timer1,
    .timeout2 = &sysinfo.timer2,
    .sampleTimer = &sysinfo.sampleTimer,
    .sample = DEFAULT_PCO2_SAMPLE_FREQ};

PERSISTENT sPCO2_t pco2_temp;
PERSISTENT sPCO2Dry_t pco2_dry;
PERSISTENT sPCO2_sdata_t run_data;

PERSISTENT sPCO2_idata_t i_data;

const char PCO2_MODE_ZERO_PUMP_ON[] = "ZPON  ";
const char PCO2_MODE_ZERO_PUMP_OFF[] = "ZPOFF ";
const char PCO2_MODE_ZERO_POST_CAL[] = "ZPPCAL";
const char PCO2_MODE_SPAN_GAS_ON[] = "SPON  ";
const char PCO2_MODE_SPAN_GAS_OFF[] = "SPOFF ";
const char PCO2_MODE_SPAN_POST_CAL[] = "SPPCAL";
const char PCO2_MODE_SPAN2_GAS_ON[] = "SP2ON  ";
const char PCO2_MODE_SPAN2_GAS_OFF[] = "SP2OFF";
const char PCO2_MODE_SPAN2_POST_CAL[] = "SP2PC ";
const char PCO2_MODE_EQUILIBRATE_PUMP_ON[] = "EPON  ";
const char PCO2_MODE_EQUILIBRATE_PUMP_OFF[] = "EPOFF ";
const char PCO2_MODE_AIR_PUMP_ON[] = "APON  ";
const char PCO2_MODE_AIR_PUMP_OFF[] = "APOFF ";
const char PCO2_MODE_REST[] = "REST  ";
const char PCO2_MODE_DEPLOY[] = "DPLY  ";

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC ePCO2Status_t PCO2_State_Rest(void);
STATIC ePCO2Status_t PCO2_State_Deploy(void);
//STATIC ePCO2Status_t PCO2_State_Purge(void);
STATIC ePCO2Status_t PCO2_State_Equilibrate(void);
STATIC ePCO2Status_t PCO2_State_Air(void);
STATIC ePCO2Status_t PCO2_State_Calibrate(ePCO2Cal_t state, float concentration);
STATIC ePCO2Status_t PCO2_State_Sample(const char *mode, sPCO2_run_data_t *rdata, sPCO2_idata_mode_t *idata);
STATIC ePCO2Status_t PCO2_SetMode(ePCO2Mode_t mode);
STATIC void PCO2_SetSampleTimeout(volatile sSysTimer_t *t, uint16_t ms);
STATIC void PCO2_Calculate_DryCO2(sPCO2Dry_t *T, sPCO2_idata_t *idata);
void PCO2_Set_O2Cal(void);
STATIC void calculate_pumpwait(void);
STATIC void calculate_prequil(void);
STATIC void PCO2_Licor_Warmup(void);
STATIC ePCO2Status_t PCO2_State_PumpOn(ePCO2Mode_t mode);
STATIC ePCO2Status_t PCO2_State_PumpOff(ePCO2Mode_t mode);
STATIC void clear_run_data(sPCO2_run_data_t *data);
STATIC void clear_full_run_data(sPCO2_sdata_t *data);
STATIC void clear_idata_mode_data(sPCO2_idata_mode_t *data);
STATIC void clear_idata_data(sPCO2_idata_t *data);
/************************************************************************
 *					         Functions
 ************************************************************************/
void PCO2_Initialize(void)
{

  Log("pCO2 Initialize Start", LOG_Priority_High);
  BSP_Hit_WD();

  /** Take Board out of Low Current mode */
  BSP_Low_Current_Mode_OFF();

  /** Clear Error Codess */
  Error_Clear();

  /** Close the Console */
  CONSOLE_Close();

  /** Initialize the Flow Controller */
  if (FLOW_OK != FLOW_Init())
  {
    Error(ERROR_MSG_00010002, LOG_Priority_High);
  }

  CONSOLE_Init();
  COMMAND_Init();

  Log("Flow Control Initialize", LOG_Priority_High);

  /** Initialize the O2 Sensor */
  MAX250_Init();

  /** Initialize the RH Sensor */
  SHT3X_Init(SHT35_I2C_SLAVE_ADDR);
  SHT3X_ReadSerialNumber();

  /** Initialize the Licor */
  if (LICOR_OK != LI8x0_Init())
  {
    Error(ERROR_MSG_00010001, LOG_Priority_High);
  }

  /** Calculate the pumpoff and prequil times */
  calculate_prequil();
  calculate_pumpwait();

  /** Trickle Charger On */
  AM08X5_Charge_ON();

  /** Set Defaults */
  Log("pCO2 Initialize Complete", LOG_Priority_High);
  return;
}

void PCO2_LowPowerMode(void)
{
  /** Trickle Charge Off */
  AM08X5_Charge_OFF();

  /** Take Board out of Low Current mode */
  BSP_Low_Current_Mode_ON();

  /** Power the Flow Controller Down */
  FLOW_Sleep();

  /** Power the Licor Off */
  LI8x0_PowerOFF();
}

ePCO2SampleMode_t PCO2_GetSampleMode(void)
{
  return pco2.SampleMode;
}

void PCO2_Calculate_RunTime(sTimeReg_t *t)
{
  sTimeReg_t temp_s;
  sTimeReg_t *temp = &temp_s;

  /** Calculate the pumpoff and prequil times */
  calculate_prequil();
  calculate_pumpwait();

  /** Add Init Times */
  AM08X5_clear_time_struct(t);
  AM08X5_seconds_to_time_struct(temp, pco2.LI_warmup);
  AM08X5_add_time_struct(temp, t, t);

  /** Add Zero Times */
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpon);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.sampleCO2 * 3);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);

  /** Add Span Times */
  AM08X5_seconds_to_time_struct(temp, pco2.spanflow.flowon);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.sampleCO2 * 3);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);

  /** Add Equil Times */
  AM08X5_seconds_to_time_struct(temp, pco2.equil.equil);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.sampleCO2 * 2);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);

  /** Add Air Times */
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpon);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.sampleCO2 * 2);
  AM08X5_add_time_struct(temp, t, t);
  AM08X5_seconds_to_time_struct(temp, pco2.pump.pumpoff);
  AM08X5_add_time_struct(temp, t, t);

  /** Add Purge Times */
  AM08X5_seconds_to_time_struct(temp, pco2.purge.purge * 2);
  AM08X5_add_time_struct(temp, t, t);

  return;
}

void PCO2_FastMode(void)
{
  /** Set the mode */
  pco2.SampleMode = PCO2_SAMPLE_MODE_FAST;
  Log("Entering Fast Mode", LOG_Priority_High);

  /** Run the Function */
#ifndef TEST_NO_RUN
  PCO2_Run(false);
#else
  Log("Run Skipped", LOG_Priority_High);
#endif
  return;
}

void PCO2_NormalMode(void)
{
  /** Set the mode to NORMAL */
  pco2.SampleMode = PCO2_SAMPLE_MODE_NORMAL;
  Log("Entering Normal Mode", LOG_Priority_High);

  /** Run the Main Function */
#ifndef TEST_NO_RUN
  PCO2_Run(false);
#endif
  Error_GetFlags(run_data.ERRORS);
}

void PCO2_InstantRun(void)
{

  /** Set the mode to NORMAL */
  pco2.SampleMode = PCO2_SAMPLE_MODE_NORMAL;
  Log("", LOG_Priority_Low);
  Log("User Commanded Run", LOG_Priority_High);

  /** Sensor Init */
  LI8x0_PowerON();

  /** Run the Main Function */
#ifndef TEST_NO_RUN
  PCO2_Run(false);
#endif
  Error_GetFlags(run_data.ERRORS);

  _delay_ms(2000);
  PMMCTL0 |= PMMSWBOR;
}

void PCO2_InstantRun_with_Purge(void)
{
  /** Set the mode to NORMAL */
  pco2.SampleMode = PCO2_SAMPLE_MODE_NORMAL;
  Log("", LOG_Priority_Low);
  Log("User Commanded Run", LOG_Priority_High);

  /** Sensor Init */
  LI8x0_PowerON();

  /** Run the Main Function */
#ifndef TEST_NO_RUN
  PCO2_Run(true);
#endif
  Error_GetFlags(run_data.ERRORS);

  _delay_ms(2000);
  PMMCTL0 |= PMMSWBOR;
}

void PCO2_InstantRun_Test(void)
{

  sPCO2_t pco2_temp = pco2;

  pco2.equil.equil = DEFAULT_PCO2_EQUIL_QUICK;
  pco2.equil.prequil = DEFAULT_PCO2_PREQUIL_QUICK;
  pco2.pump.pumpon = DEFAULT_PCO2_PUMPON_QUICK;
  pco2.pump.pumpoff = DEFAULT_PCO2_PUMPOFF_QUICK;
  pco2.sampleCO2 = DEFAULT_PCO2_SAMPLECO2_QUICK;
  pco2.LI_warmup = DEFAULT_PCO2_LI_WARMUP_QUICK;
  pco2.LI_heater = DEFAULT_PCO2_LI_HEATER_QUICK;
  pco2.vent = DEFAULT_PCO2_VENT_QUICK;
  pco2.zero_on = false;
  pco2.span_on = false;

  /** Set the mode to NORMAL */
  pco2.SampleMode = PCO2_SAMPLE_MODE_NORMAL;
  Log("User Commanded Run - Fast Intervals", LOG_Priority_High);

  /** Run the main function */
  PCO2_Run(false);

  /** Get the Error Flags */
  Error_GetFlags(run_data.ERRORS);

  /** Revert to original settings */
  pco2 = pco2_temp;
}

void PCO2_Run(bool purge)
{
  PCO2_Initialize();

  db_val = 0;

  /** Clear the sdata struct */
  clear_full_run_data(&run_data);

  /** Clear the idata struct */
  clear_idata_data(&i_data);

  /** Wakeup the Flow Controller */
  FLOW_Wakeup();

  /** Indicate Start of Run */
  Log("Start PCO2 Run", LOG_Priority_High);

  Message("Start\r\n");
  
  BSP_Hit_WD();

  /** Configure System */
  if (PCO2_OK != PCO2_State_Config())
  {
    Error(ERROR_MSG_00010010, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }
  
  /** Zero Mode*/
  BSP_Hit_WD();
  if (PCO2_OK != PCO2_State_Zero())
  {
    Error(ERROR_MSG_00010010, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }

  /** Span Mode*/
  BSP_Hit_WD();
  SPAN_SKIPPED = true;
  if (PCO2_OK != PCO2_State_Span(pco2.span))
  {
    Error(ERROR_MSG_00010040, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }

  /** Equilibrate Mode*/
  BSP_Hit_WD();
  if (PCO2_OK != PCO2_State_Equilibrate())
  {
    Error(ERROR_MSG_00010100, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }

  /** Air Mode */
  BSP_Hit_WD();
  if (PCO2_OK != PCO2_State_Air())
  {
    Error(ERROR_MSG_00010200, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }

  /** Purge Mode */
  if(purge == true)
  {
    BSP_Hit_WD();
    if (PCO2_OK != PCO2_State_Purge())
    {
      Error(ERROR_MSG_00010A00, LOG_Priority_High);
      PCO2_State_Rest();
      return;
    }
  }

  /** Rest Mode*/
  BSP_Hit_WD();
  if (PCO2_OK != PCO2_State_Rest())
  {
    Error(ERROR_MSG_00010400, LOG_Priority_High);
    PCO2_State_Rest();
    return;
  }
  Log("", LOG_Priority_High);

  /** Display the Coefficients */
  PCO2_GetCoef();

  /** Update idata coefficients*/
  i_data.COEFF.co2kzero = LI8x0_Get_KZero_float();
  i_data.COEFF.co2kspan = LI8x0_Get_KSpan_float();
  i_data.COEFF.co2kspan2 = LI8x0_Get_KSpan2_float();

  /** Calculate the Dry CO2 */
  BSP_Hit_WD();
  if (SPAN_SKIPPED == false)
  {
    PCO2_Calculate_DryCO2(&pco2_dry, &i_data);
  }

  /** Indicate End of Run */
  BSP_Hit_WD();
  Log("", LOG_Priority_High);
  Log("Run Complete\r\n", LOG_Priority_High);
}

ePCO2Status_t PCO2_Deploy(void)
{
  ePCO2Status_t result = PCO2_FAIL;
  /** Deploy */
  if (PCO2_OK != PCO2_State_Deploy())
  {
    Error(ERROR_MSG_00010800, LOG_Priority_High);
  }
  else
  {
    result = PCO2_OK;
  }
  return result;
}

uint16_t PCO2_FastMode_CalculateRepeats(void)
{

  sTimeReg_t t;
  sTimeReg_t zero;
  uint16_t count = 0;

  AM08X5_clear_time_struct(&t);
  AM08X5_clear_time_struct(&zero);

  /** Compare the time and increment to calculate the number of FastMode repeats */
  while (AM08X5_compare_time_non_epoch(&pco2.FastChange, &t) == true)
  {
    count++;
    AM08X5_add_time_struct(&t, &pco2.FastInterval, &t);
  }

  return count;
}

void PCO2_GetRunLength(sTimeReg_t *t)
{
  sTimeReg_t temp;

  /** Clear the time structs */
  AM08X5_clear_time_struct(t);
  AM08X5_clear_time_struct(&temp);

  /** Add all modes together */
  /** Add Config */
  temp.second = 10;
  AM08X5_add_time_struct(t, &temp, t);

  /** Add Zero Times */
  AM08X5_clear_time_struct(&temp);
  temp.second = pco2.pump.pumpon;
  AM08X5_add_time_struct(t, &temp, t);
}

void PCO2_GetNormalInterval(sTimeReg_t *t)
{
  /** Clear time struct */
  AM08X5_clear_time_struct(t);
  /** Transfer data in time struct */
  *t = pco2.NormInterval;
}

void PCO2_GetNormalStartTime(sTimeReg_t *t)
{

  /** Clear Offset time struct */
  sTimeReg_t offset;
  AM08X5_clear_time_struct(&offset);
  offset.hour = 1;

  /** Get the current time */
  AM08X5_GetTime(&pco2.NormStart);

  /** Round up to next hour */
  AM08X5_add_time_struct(&pco2.NormStart, &offset, &pco2.NormStart);
  pco2.NormStart.minute = 0;
  pco2.NormStart.second = 0;

  /** Transfer data in time struct */
  *t = pco2.NormStart;
}

void PCO2_GetFastInterval(sTimeReg_t *t)
{
  /** Clear time struct */
  AM08X5_clear_time_struct(t);
  /** Transfer data in time struct */
  *t = pco2.FastInterval;
}

void PCO2_GetChange(sTimeReg_t *t)
{
  /** Clear time struct */
  AM08X5_clear_time_struct(t);
  /** Transfer data in time struct */
  *t = pco2.FastChange;
}

uint8_t PCO2_GetSample(void)
{
  return pco2.sample;
}

uint32_t PCO2_GetEquil(void)
{
  return pco2.equil.equil;
}

uint32_t PCO2_GetWarmup(void)
{
  return pco2.LI_warmup;
}

uint32_t PCO2_GetSpanFlowON(void)
{
  return pco2.spanflow.flowon;
}

uint32_t PCO2_GetPurge(void)
{
  return pco2.purge.purge;
}
uint32_t PCO2_GetPumpON(void)
{
  return pco2.pump.pumpon;
}

uint32_t PCO2_GetPumpOFF(void)
{
  return pco2.pump.pumpoff;
}

uint32_t PCO2_GetSampleCO2(void)
{
  return pco2.sampleCO2;
}

uint32_t PCO2_GetVent(void)
{
  return pco2.vent;
}

uint8_t PCO2_GetHeater(void)
{
  return pco2.LI_heater;
}

float PCO2_GetSpanDiff(void)
{
  return pco2.spandiff;
}

float PCO2_GetSpan(void)
{
  return pco2.span;
}

float PCO2_GetSpan2(void)
{
  return pco2.span2;
}

void PCO2_GetCoef(void)
{
  /** Get the Licor Calibration Coefficients */
  memset(pco2_str, 0, PCO2_STR_LEN);
  strcat(pco2_str, "Licor - ");
  LI8x0_GetCal();
  LI8x0_CreateString_Cal(&pco2_str[8], 0);

  /** Get the O2 Cal Coefficients */
  strcat(pco2_str, "\r\n\r\nO2 - \r\n");
  char temp[32];
  memset(temp, 0, 32);
  float o2cal = MAX250_GetCal();
  sprintf(temp, "  o2cal= %.3f", o2cal);
  strcat(pco2_str, temp);
  Coeff(pco2_str);
}

void PCO2_SetO2Interval(sTimeReg_t *t)
{
  pco2.O2Interval = *t;
}

void PCO2_GetO2Interval(sTimeReg_t *t)
{
  *t = pco2.O2Interval;
}

void PCO2_SetO2Time(sTimeReg_t *t)
{
  pco2.O2Time = *t;
}

void PCO2_GetO2Time(sTimeReg_t *t)
{
  sTimeReg_t now;
  sTimeReg_t temp;

  AM08X5_GetTime(&now);

  *t = pco2.O2Time;
  t->year = now.year;
  t->month = now.month;
  t->date = now.date;

  if (AM08X5_compare_time(t, &now) != true)
  {
    AM08X5_clear_time_struct(&temp);
    temp.hour = 24;
    AM08X5_add_time_struct(t, &temp, t);
  }
}

void PCO2_SetNormalInterval(sTimeReg_t *t)
{

  if ((0 < t->year) && (0 < t->month) && (0 < t->date))
  {
    return;
  }

  if ((60 > t->hour) && (60 > t->minute) && (100 > t->hour))
  {
    pco2.NormInterval = *t;
  }
  return;
}

void PCO2_SetChange(sTimeReg_t *t)
{
  if ((0 < t->year) && (0 < t->month) && (0 < t->date))
  {
    return;
  }

  if ((60 > t->hour) && (60 > t->minute) && (100 > t->hour))
  {
    pco2.FastChange = *t;
  }
}

void PCO2_SetSample(uint8_t freq)
{
  switch (freq)
  {
  case 1:
  case 2:
  case 4:
    pco2.sample = freq;
    break;
  default:
    break;
  }
  return;
}

void PCO2_SetSpanDiff(float diff)
{
  pco2.spandiff = diff;
}

void PCO2_SetSpan(float ppm)
{
  pco2.span = ppm;
}

void PCO2_SetSpan2(float ppm)
{
  pco2.span2 = ppm;
}

void PCO2_SetEquil(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.equil.equil = seconds;
  }
}

void PCO2_SetWarmup(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.LI_warmup = seconds;
  }
}

void PCO2_SetSpanFlowON(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.spanflow.flowon = seconds;
  }
}

void PCO2_SetPurge(uint32_t seconds)
{
  if(1000 > seconds)
  {
    pco2.purge.purge = seconds;
  }
}
void PCO2_SetPumpON(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.pump.pumpon = seconds;
  }
}

void PCO2_SetPumpOFF(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.pump.pumpoff = seconds;
  }
}

void PCO2_SetSampleCO2(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.sampleCO2 = seconds;
  }
}

void PCO2_SetVent(uint32_t seconds)
{
  if (1000 > seconds)
  {
    pco2.vent = seconds;
  }
}

void PCO2_SetHeater(uint8_t state)
{
  BSP_Hit_WD();
  if (2 > state)
  {
    /** Set Heater*/
    LI8x0_Heater(state);

    /** Set the state in the struct */
    pco2.LI_heater = state;
  }
}

void PCO2_CalO2(void)
{
  BSP_Hit_WD();
  Log("O2 Calibration Start\r\n", LOG_Priority_High);
  /** Set Valves to Air */
  FLOW_SetMode(FLOW_STATE_AIR_PUMP_ON);
#ifndef TEST_QUICK_PERIODS
  _delay_ms(60000);
#else
  _delay_ms(1000);
#endif

  FLOW_SetMode(FLOW_STATE_AIR_PUMP_OFF);
  PCO2_Set_O2Cal();

  /** Set Valves to Rest */
  FLOW_SetMode(FLOW_STATE_REST);

  Log("O2 Calibration Complete\r\n", LOG_Priority_High);
}
/************************************************************************
 *					           Static Functions
 ************************************************************************/
/** @brief PCO2 Set Timeout
 *
 * Sets a timeout timer
 *
 * @param *t Pointer to Timer Struct
 * @param seconds Seconds to timeout for
 *
 * @return result
 */
STATIC void PCO2_SetTimeout(volatile sSysTimer_t *t, uint16_t seconds)
{
  t->tCounter = seconds;
  t->tFlag = true;
}

/** @brief PCO2 Set Sample Timeout
 *
 * Sets a timeout timer
 *
 * @param *t Pointer to Timer Struct
 * @param seconds Seconds to timeout for
 *
 * @return result
 */
STATIC void PCO2_SetSampleTimeout(volatile sSysTimer_t *t, uint16_t ms)
{
  uint16_t cnt = 0;
  if (ms > 0)
  {
    cnt = 1000 / ms;
    BSP_Hit_WD();
    t->tCounter = AMBIQ_RTC_SYSTICK_FREQ / cnt;
    t->tFlag = true;
  }
  else
  {
    t->tFlag = false;
  }
  BSP_Hit_WD();
}

/** @brief PCO2 Wait for Timeout
 *
 * Wait for timeout to complete
 *
 * @param *t Pointer to Timer Struct
 *
 * @return result
 */
STATIC void PCO2_WaitOnTimeout(volatile sSysTimer_t *t)
{
  while (t->tFlag == true)
  {
    _delay_ms(500);
    BSP_Hit_WD();
  }
  COMMAND_puts("\r");
  // Message("\r");
  // _delay_ms(20);
  // Message("\r");
}

/** @brief PCO2 Get Timeout
 *
 * Get a timeout timer
 *
 * @param *t Pointer to Timer Struct
 *
 * @return status of timeout
 */
STATIC bool PCO2_GetTimeout(volatile sSysTimer_t *t)
{
  bool val = t->tFlag;
  val ^= 1;
  return val;
}

/** @brief PCO2 Configuration State
 *
 * Configures the PCO2
 *
 * @param None
 *
 * @return result
 */
ePCO2Status_t PCO2_State_Config(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);
  Log("Start PCO2 Config", LOG_Priority_Low);

  /** Out of Low Current Mode */
  BSP_Low_Current_Mode_OFF();

  PCO2_Initialize();

  /** Start the Licor */
  PCO2_Licor_Warmup();
  LI8x0_Config();
  LI8x0_GetInfo();

  /** Start a timer for 10 seconds */
  PCO2_SetTimeout(pco2.timeout1, pco2.LI_warmup);
  PCO2_WaitOnTimeout(pco2.timeout1);

  /** Configure the Licor */
  Log("Configure Licor", LOG_Priority_High);
  if (LICOR_OK != LI8x0_Config())
  {
    Error(ERROR_MSG_00010010, LOG_Priority_High);
  }
  else
  {
    result = PCO2_OK;
  }

  Log("End PCO2 Config", LOG_Priority_Low);
  return result;
}

STATIC void get_sdata_str(char *temp, sPCO2_run_data_t *data, uint8_t idx, uint8_t mode_num)
{
  char mode[7];
  memset(mode, 0, 7);
  switch (mode_num)
  {
  case 0:
    strncpy(mode, "ZPON", 4);
    break;
  case 1:
    strncpy(mode, "ZPOFF", 5);
    break;
  case 2:
    strncpy(mode, "ZPPCAL", 6);
    break;
  case 3:
    strncpy(mode, "SPON", 4);
    break;
  case 4:
    strncpy(mode, "SPOFF", 5);
    break;
  case 5:
    strncpy(mode, "SPPCAL", 6);
    break;
  case 6:
    strncpy(mode, "EPON", 4);
    break;
  case 7:
    strncpy(mode, "EPOFF", 5);
    break;
  case 8:
    strncpy(mode, "APON", 4);
    break;
  case 9:
    strncpy(mode, "APOFF", 5);
    break;
  default:
    strncpy(mode, "FAIL", 4);
    break;
  }

  char timestamp[32];
  AM08X5_get_timestamp(timestamp, &data->timestamp[idx]);

  sprintf(temp, "%s, %s, %9.3f, %7.3f, %7.3f, %6ld, %6ld, %7.3f, %6.3f, %7.3f,,,,,,\n",
          mode,
          timestamp,
          data->CO2[idx],
          data->Li_temp[idx],
          data->Li_pres[idx],
          data->Li_raw[idx],
          data->Li_ref[idx],
          data->RH[idx],
          data->RH_T[idx],
          data->O2[idx]);
}

void PCO2_Get_sdata(void)
{

  const char DataHeader[] = "State,TS,CO2(ppm),Li_Temp(C),Li_Pres(kPa),Li_Raw,Li_Ref,RH(%),RH_T(C),O2(%),xCO2(dry),xCO2(wet), co2kzero, co2kspan, co2kspan2, errors\n";
  Message(DataHeader);
  char temp[256];

  /** ZPON */
  for (uint8_t i = 0; i < run_data.ZPON.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.ZPON, i, 0);
    Message(temp);
  }

  /** ZPOFF */
  for (uint8_t i = 0; i < run_data.ZPOFF.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.ZPOFF, i, 1);
    Message(temp);
  }

  /** ZPPCAL */
  for (uint8_t i = 0; i < run_data.ZPPCAL.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.ZPPCAL, i, 2);
    Message(temp);
  }

  /** SPON */
  for (uint8_t i = 0; i < run_data.SPON.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.SPON, i, 3);
    Message(temp);
  }

  /** SPOFF */
  for (uint8_t i = 0; i < run_data.SPOFF.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.SPOFF, i, 4);
    Message(temp);
  }

  /** SPPCAL */
  for (uint8_t i = 0; i < run_data.SPPCAL.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.SPPCAL, i, 5);
    Message(temp);
  }

  /** EPON */
  for (uint8_t i = 0; i < run_data.EPON.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.EPON, i, 6);
    Message(temp);
  }

  /** EPOFF */
  for (uint8_t i = 0; i < run_data.EPOFF.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.EPOFF, i, 7);
    Message(temp);
  }

  /** APON */
  for (uint8_t i = 0; i < run_data.APON.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.APON, i, 8);
    Message(temp);
  }

  /** APOFF */
  for (uint8_t i = 0; i < run_data.APOFF.sample_idx; i++)
  {
    memset(temp, 0, 256);
    get_sdata_str(temp, &run_data.APOFF, i, 9);
    Message(temp);
  }

  /** DRY */
  memset(temp, 0, 256);
  char air[16];
  char sw[16];
  memset(air, 0, 16);
  memset(sw, 0, 16);
  if (run_data.DRY.air_xco2 == NAN)
  {
    strcpy(air, "NAN");
  }
  else
  {
    sprintf(air, "%7.3f", run_data.DRY.air_xco2);
  }

  if (run_data.DRY.sw_xco2 == NAN)
  {
    strcpy(sw, "NAN");
  }
  else
  {
    sprintf(sw, "%7.3f", run_data.DRY.sw_xco2);
  }
  sprintf(temp, "DRY,,,,,,,,,,%s,%s,,,,\n", air, sw);
  Message(temp);

  /** COEFF */
  memset(temp, 0, 256);
  char co2kzero[16] = "";
  char co2kspan[16] = "";
  char co2kspan2[16] = "";

  LI8x0_Get_Zero(co2kzero);
  LI8x0_Get_Span(co2kspan);
  LI8x0_Get_Span2(co2kspan2);
  sprintf(temp, "COEFF,,,,,,,,,,,, %s, %s, %s,\n", co2kzero, co2kspan, co2kspan2);
  Message(temp);

  /** Errors */
  sprintf(temp, "ERR,,,,,,,,,,,,,,,%s\n", run_data.ERRORS);
  Message(temp);
}

void PCO2_Get_idata(void)
{

  const char header[] = "State,TS,Li_Temp_ave(C),Li_Temp_sd,Li_Pres_ave(kPa),Li_Pres_sd,CO2_ave(PPM),CO2_SD,O2_ave(%),O2_S,RH_ave(%),RH_sd,RH_T_ave(C),Rh_T_sd,Li_RawSample_ave,Li_RawSample_sd,Li_RawDetector_ave,Li_RawReference_sd, SW_xCO2(dry),Atm_xCO2(dry),co2kzero,co2kspan,co2kspan2,eFlags\n";

  Message(header);

  char temp[300];
  memset(temp, 0, 300);

  /** ZPON */
  sprintf(temp, "ZPON, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.ZPON.timestamp,
          i_data.ZPON.temperature.mean,
          i_data.ZPON.temperature.std,
          i_data.ZPON.pressure.mean,
          i_data.ZPON.pressure.std,
          i_data.ZPON.co2.mean,
          i_data.ZPON.co2.std,
          i_data.ZPON.o2.mean,
          i_data.ZPON.o2.std,
          i_data.ZPON.rh.mean,
          i_data.ZPON.rh.std,
          i_data.ZPON.rh_temperature.mean,
          i_data.ZPON.rh_temperature.std,
          (uint32_t)i_data.ZPON.raw1.mean,
          (uint32_t)i_data.ZPON.raw1.std,
          (uint32_t)i_data.ZPON.raw2.mean,
          (uint32_t)i_data.ZPON.raw2.std);
  
  Message(temp);

  /** ZPOFF */
  sprintf(temp, "ZPOFF, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.ZPOFF.timestamp,
          i_data.ZPOFF.temperature.mean,
          i_data.ZPOFF.temperature.std,
          i_data.ZPOFF.pressure.mean,
          i_data.ZPOFF.pressure.std,
          i_data.ZPOFF.co2.mean,
          i_data.ZPOFF.co2.std,
          i_data.ZPOFF.o2.mean,
          i_data.ZPOFF.o2.std,
          i_data.ZPOFF.rh.mean,
          i_data.ZPOFF.rh.std,
          i_data.ZPOFF.rh_temperature.mean,
          i_data.ZPOFF.rh_temperature.std,
          (uint32_t)i_data.ZPOFF.raw1.mean,
          (uint32_t)i_data.ZPOFF.raw1.std,
          (uint32_t)i_data.ZPOFF.raw2.mean,
          (uint32_t)i_data.ZPOFF.raw2.std);
  Message(temp);

  /** ZPPCAL */
  sprintf(temp, "ZPPCAL, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.ZPPCAL.timestamp,
          i_data.ZPPCAL.temperature.mean,
          i_data.ZPPCAL.temperature.std,
          i_data.ZPPCAL.pressure.mean,
          i_data.ZPPCAL.pressure.std,
          i_data.ZPPCAL.co2.mean,
          i_data.ZPPCAL.co2.std,
          i_data.ZPPCAL.o2.mean,
          i_data.ZPPCAL.o2.std,
          i_data.ZPPCAL.rh.mean,
          i_data.ZPPCAL.rh.std,
          i_data.ZPPCAL.rh_temperature.mean,
          i_data.ZPPCAL.rh_temperature.std,
          (uint32_t)i_data.ZPPCAL.raw1.mean,
          (uint32_t)i_data.ZPPCAL.raw1.std,
          (uint32_t)i_data.ZPPCAL.raw2.mean,
          (uint32_t)i_data.ZPPCAL.raw2.std);
  Message(temp);

  /** SPON */
  sprintf(temp, "SPON, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.SPON.timestamp,
          i_data.SPON.temperature.mean,
          i_data.SPON.temperature.std,
          i_data.SPON.pressure.mean,
          i_data.SPON.pressure.std,
          i_data.SPON.co2.mean,
          i_data.SPON.co2.std,
          i_data.SPON.o2.mean,
          i_data.SPON.o2.std,
          i_data.SPON.rh.mean,
          i_data.SPON.rh.std,
          i_data.SPON.rh_temperature.mean,
          i_data.SPON.rh_temperature.std,
          (uint32_t)i_data.SPON.raw1.mean,
          (uint32_t)i_data.SPON.raw1.std,
          (uint32_t)i_data.SPON.raw2.mean,
          (uint32_t)i_data.SPON.raw2.std);
  Message(temp);

  /** SPOFF */
  sprintf(temp, "SPOFF, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,\n",
          i_data.SPOFF.timestamp,
          i_data.SPOFF.temperature.mean,
          i_data.SPOFF.temperature.std,
          i_data.SPOFF.pressure.mean,
          i_data.SPOFF.pressure.std,
          i_data.SPOFF.co2.mean,
          i_data.SPOFF.co2.std,
          i_data.SPOFF.o2.mean,
          i_data.SPOFF.o2.std,
          i_data.SPOFF.rh.mean,
          i_data.SPOFF.rh.std,
          i_data.SPOFF.rh_temperature.mean,
          i_data.SPOFF.rh_temperature.std,
          (uint32_t)i_data.SPOFF.raw1.mean,
          (uint32_t)i_data.SPOFF.raw1.std,
          (uint32_t)i_data.SPOFF.raw2.mean,
          (uint32_t)i_data.SPOFF.raw2.std);
  Message(temp);

  /** SPPCAL */
  sprintf(temp, "SPPCAL, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.SPPCAL.timestamp,
          i_data.SPPCAL.temperature.mean,
          i_data.SPPCAL.temperature.std,
          i_data.SPPCAL.pressure.mean,
          i_data.SPPCAL.pressure.std,
          i_data.SPPCAL.co2.mean,
          i_data.SPPCAL.co2.std,
          i_data.SPPCAL.o2.mean,
          i_data.SPPCAL.o2.std,
          i_data.SPPCAL.rh.mean,
          i_data.SPPCAL.rh.std,
          i_data.SPPCAL.rh_temperature.mean,
          i_data.SPPCAL.rh_temperature.std,
          (uint32_t)i_data.SPPCAL.raw1.mean,
          (uint32_t)i_data.SPPCAL.raw1.std,
          (uint32_t)i_data.SPPCAL.raw2.mean,
          (uint32_t)i_data.SPPCAL.raw2.std);
  Message(temp);

  /** EPON */
  sprintf(temp, "EPON, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.EPON.timestamp,
          i_data.EPON.temperature.mean,
          i_data.EPON.temperature.std,
          i_data.EPON.pressure.mean,
          i_data.EPON.pressure.std,
          i_data.EPON.co2.mean,
          i_data.EPON.co2.std,
          i_data.EPON.o2.mean,
          i_data.EPON.o2.std,
          i_data.EPON.rh.mean,
          i_data.EPON.rh.std,
          i_data.EPON.rh_temperature.mean,
          i_data.EPON.rh_temperature.std,
          (uint32_t)i_data.EPON.raw1.mean,
          (uint32_t)i_data.EPON.raw1.std,
          (uint32_t)i_data.EPON.raw2.mean,
          (uint32_t)i_data.EPON.raw2.std);
  Message(temp);

  /** EPOFF */
  sprintf(temp, "EPOFF, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.EPOFF.timestamp,
          i_data.EPOFF.temperature.mean,
          i_data.EPOFF.temperature.std,
          i_data.EPOFF.pressure.mean,
          i_data.EPOFF.pressure.std,
          i_data.EPOFF.co2.mean,
          i_data.EPOFF.co2.std,
          i_data.EPOFF.o2.mean,
          i_data.EPOFF.o2.std,
          i_data.EPOFF.rh.mean,
          i_data.EPOFF.rh.std,
          i_data.EPOFF.rh_temperature.mean,
          i_data.EPOFF.rh_temperature.std,
          (uint32_t)i_data.EPOFF.raw1.mean,
          (uint32_t)i_data.EPOFF.raw1.std,
          (uint32_t)i_data.EPOFF.raw2.mean,
          (uint32_t)i_data.EPOFF.raw2.std);
  Message(temp);
  
  /** APON */
  sprintf(temp, "APON, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.APON.timestamp,
          i_data.APON.temperature.mean,
          i_data.APON.temperature.std,
          i_data.APON.pressure.mean,
          i_data.APON.pressure.std,
          i_data.APON.co2.mean,
          i_data.APON.co2.std,
          i_data.APON.o2.mean,
          i_data.APON.o2.std,
          i_data.APON.rh.mean,
          i_data.APON.rh.std,
          i_data.APON.rh_temperature.mean,
          i_data.APON.rh_temperature.std,
          (uint32_t)i_data.APON.raw1.mean,
          (uint32_t)i_data.APON.raw1.std,
          (uint32_t)i_data.APON.raw2.mean,
          (uint32_t)i_data.APON.raw2.std);
  Message(temp);

  /** APOFF */
  sprintf(temp, "APOFF, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld,,,,,,\n",
          i_data.APOFF.timestamp,
          i_data.APOFF.temperature.mean,
          i_data.APOFF.temperature.std,
          i_data.APOFF.pressure.mean,
          i_data.APOFF.pressure.std,
          i_data.APOFF.co2.mean,
          i_data.APOFF.co2.std,
          i_data.APOFF.o2.mean,
          i_data.APOFF.o2.std,
          i_data.APOFF.rh.mean,
          i_data.APOFF.rh.std,
          i_data.APOFF.rh_temperature.mean,
          i_data.APOFF.rh_temperature.std,
          (uint32_t)i_data.APOFF.raw1.mean,
          (uint32_t)i_data.APOFF.raw1.std,
          (uint32_t)i_data.APOFF.raw2.mean,
          (uint32_t)i_data.APOFF.raw2.std);
  Message(temp);

  /** COEFF */
  sprintf(temp, "COEFF,,,,,,,,,,,,,,,,,,,, %0.8f, %0.8f, %0.8f,\n",
          i_data.COEFF.co2kzero,
          i_data.COEFF.co2kspan,
          i_data.COEFF.co2kspan2);
  Message(temp);

  sprintf(temp, "DRY,,,,,,,,,,,,,,,,,,%0.6f,%0.6f,,,,\n", i_data.air_xco2, i_data.sw_xco2);
  Message(temp);

  sprintf(temp, "ERR,,,,,,,,,,,,,,,,,,,,,,,%s\n\n", run_data.ERRORS);
  Message(temp);
}

void PCO2_Set_mconfig(bool val)
{
  pco2.man.mconfig = val;
}

void PCO2_Set_ssct(float val)
{
  pco2.man.ssct = val;
}

void PCO2_Set_sscsc(float val)
{
  pco2.man.sscsc = val;
}

void PCO2_Set_lsstds(char *val)
{
  strcpy(pco2.man.lsstds, val);
}

void PCO2_Set_sstds(double val)
{
  pco2.man.sstds = val;
}

void PCO2_Set_sstdsf(float val)
{
  pco2.man.sstdsf = val;
}

void PCO2_Set_avalid(char *val)
{
  strcpy(pco2.man.avalid, val);
}

void PCO2_Set_pbias(float val)
{
  pco2.man.pbias = val;
}

void PCO2_Set_lpbm(char *val)
{
  strcpy(pco2.man.lpbm, val);
}

void PCO2_Set_sscrh(float val)
{

  pco2.man.sscrh = val;
}
void PCO2_Set_atrhs(char *val)
{
}
void PCO2_Set_o2s(char *val) { strcpy(pco2.man.o2s, val); }
void PCO2_Set_manufacturer(char *val)
{
  uint8_t len = strlen(val);
  if (len > 23)
  {
    len = 23;
  }
  strncpy(pco2.man.manufacturer, val, len);
}

void PCO2_Set_sscssn(char *val)
{
  uint8_t len = strlen(val);
  if (len > 23)
  {
    len = 23;
  }
  strncpy(pco2.man.sscssn, val, len);
}

void PCO2_Set_sssn(char *val)
{
  uint8_t len = strlen(val);
  if (len > 23)
  {
    len = 23;
  }
  strncpy(pco2.man.sssn, val, len);
}

void PCO2_Set_ssn(char *val)
{
  uint8_t len = strlen(val);
  if (len > 23)
  {
    len = 23;
  }
  strncpy(pco2.man.ssn, val, len);
}

void PCO2_Set_lssc(char *val)
{
  uint8_t len = strlen(val);
  if (len > 23)
  {
    len = 23;
  }
  strncpy(pco2.man.lssc, val, len);
}

bool PCO2_Get_mconfig(void) { return pco2.man.mconfig; }
float PCO2_Get_ssct(void) { return pco2.man.ssct; }
float PCO2_Get_sscsc(void) { return pco2.man.sscsc; }
void PCO2_Get_lsstds(char *val) { strcpy(val, pco2.man.lsstds); }
double PCO2_Get_sstds(void) { return pco2.man.sstds; }
float PCO2_Get_sstdsf(void) { return pco2.man.sstdsf; }
float PCO2_Get_ssconc(void) { return PCO2_GetSpan2(); }
void PCO2_Get_avalid(char *val) { strcpy(val, pco2.man.avalid); }
float PCO2_Get_pbias(void) { return pco2.man.pbias; }
void PCO2_Get_lpbm(char *val) { strcpy(val, pco2.man.lpbm); }
float PCO2_Get_sscrh(void) { return pco2.man.sscrh; }
void PCO2_Get_atrhs(char *val)
{
  sprintf(val, "%ld", SHT3X_Get_SerialNumber());
}
void PCO2_Get_o2s(char *val) { strcpy(val, pco2.man.o2s); }
void PCO2_Get_manufacturer(char *val) { strcpy(val, pco2.man.manufacturer); }
void PCO2_Get_sscssn(char *val) { strcpy(val, pco2.man.sscssn); }
void PCO2_Get_sssn(char *val) { strcpy(val, pco2.man.sssn); }
void PCO2_Get_ssn(char *val) { strcpy(val, pco2.man.ssn); }
void PCO2_Get_lssc(char *val) { strcpy(val, pco2.man.lssc); }


/** @brief PCO2 Purge State
 * 
 * Purges/dries PCO2 system.  Per issue #253, flow is:
 * 1. ABABAA, pump on, wait 40 seconds.
 * 2. BAABBA, pump off, wait PURGE TIME
 * 3. BABAAA, pump off, wait 20 seconds
 * 4. BBBAAA, pump off, wait VENT TIME
 * 5. ABBAAA, pump off, wait VENT TIME
 * 6. REST, pump off, wait 60 seconds
 * 7. BBBABB, pump off, wait PURGE TIME
 * 8. ABBAAB, pump off, wait VENT TIME
 *
 * @param None
 * 
 * @return result 
*/
ePCO2Status_t PCO2_State_Purge(void)
{
  uint32_t ms_time = 0;
  uint8_t fail_count = 0;
  
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);
  Log("PCO2 Purge", LOG_Priority_Low);
  
  /** Set the flow controller to PRG1, Pump ON, wait 40 seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG1))
  {
    Error(ERROR_MSG_00010A01, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) 40*1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG2, Pump ON, wait PURGE seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG2))
  {
    Error(ERROR_MSG_00010A02, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) pco2.purge.purge * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG3, Pump ON, wait 20 seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG3))
  {
    Error(ERROR_MSG_00010A03, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) 20 * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG4, Pump ON, wait PURGE seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG4))
  {
    Error(ERROR_MSG_00010A04, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) pco2.vent * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG5, Pump ON, wait 40 seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG5))
  {
    Error(ERROR_MSG_00010A05, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) pco2.vent * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG6, Pump ON, wait PURGE seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG6))
  {
    Error(ERROR_MSG_00010A06, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) 60 * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG7, Pump ON, wait 20 seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG7))
  {
    Error(ERROR_MSG_00010A07, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) pco2.purge.purge * 1000;
  _delay_ms(ms_time);
  
  /** Set the flow controller to PRG8, Pump ON, wait PURGE seconds */
  if (PCO2_OK != PCO2_SetMode(MODE_PRG8))
  {
    Error(ERROR_MSG_00010A08, LOG_Priority_Med);
    fail_count++;
  }
  ms_time = (uint32_t) pco2.vent * 1000;
  _delay_ms(ms_time);

//  /** Set the flow controller to PURGE, Pump OFF */
//  if (PCO2_OK != PCO2_SetMode(MODE_PURGE))
//  {
//    Error(ERROR_MSG_00010A00, LOG_Priority_Med);
//  }
//
//  /** Wait PURGE seconds */
//  _delay_ms(pco2.purge.purge * 1000);
//
//
//  /** Set the flow controller to CLEAR */
//  if (PCO2_OK != PCO2_SetMode(MODE_CLEAR))
//  {
//    Error(ERROR_MSG_00010A00, LOG_Priority_Med);
//  }
//  else
//  {
//    result = PCO2_OK;
//  }
//
//  /** Wait PURGE seconds */
//  _delay_ms(pco2.purge.purge * 1000);
  if(fail_count > 0)
  {
    result = PCO2_FAIL;
  } else {
    result = PCO2_OK;
  }
  return result;
}


/** @brief PCO2 Rest State
 *
 * Puts PCO2 in Rest State
 *
 * @param None
 *
 * @return result
 */
STATIC ePCO2Status_t PCO2_State_Rest(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);
  Log("PCO2 Rest", LOG_Priority_Low);

  /** Set the flow controller to REST, Pump OFF */
  if (PCO2_OK != PCO2_SetMode(MODE_REST))
  {
    Error(ERROR_MSG_00010800, LOG_Priority_Med);
  }
  else
  {
    result = PCO2_OK;
  }

  /** Turn the Flow Controller Off */
  FLOW_Sleep();

  /** Turn the Licor OFF */
  LI8x0_PowerOFF();

  /** Low Current Mode ON */
  BSP_Low_Current_Mode_ON();

  return result;
}

/** @brief PCO2 Deploy State
 *
 * Puts PCO2 in Deploy State
 *
 * @param None
 *
 * @return result
 */
STATIC ePCO2Status_t PCO2_State_Deploy(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("PCO2 Deploy", LOG_Priority_Low);
  /** Set the flow controller to Deploy, Pump OFF */
  if (PCO2_OK != PCO2_SetMode(MODE_DEPLOY))
  {
    Error(ERROR_MSG_00012000, LOG_Priority_Med);
  }
  else
  {
    result = PCO2_OK;
  }

  /** Turn the Licor OFF */
  LI8x0_PowerOFF();

  /** Low Current Mode ON */
  BSP_Low_Current_Mode_ON();

  return result;
}

void PCO2_Set_O2Cal(void)
{
  Log("Start PCO2 Recalibrate O2", LOG_Priority_Low);

  /** Read O2 Voltage */
  float voltage = MAX250_ReadVoltage();
  float cal = 20.947 / voltage;
  char temp[64];
  sprintf(temp, "O2 Cal= %7.3f", cal);
  Coeff(temp);
  MAX250_SetCal(cal);
}

/** @brief PCO2 Zero State
 *
 * State which calibrates the Licor against a known zero gas
 *
 * @param None
 *
 * @return result
 */
ePCO2Status_t PCO2_State_Zero(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);
  Log("Start Zero", LOG_Priority_Low);
  /** Clear the char arrays */
  memset(pco2_str, 0, PCO2_STR_LEN);

  /** Low Current Mode OFF */
  BSP_Low_Current_Mode_OFF();

  /** Perform Pumpon state */
  if (PCO2_OK != PCO2_State_PumpOn(MODE_ZERO_PUMP_ON))
  {
    Error(ERROR_MSG_00020002, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_ZERO_PUMP_ON,
                                   &run_data.ZPON,
                                   &i_data.ZPON))
  {
    Error(ERROR_MSG_00020004, LOG_Priority_High);
    return result;
  }

  /** Perform Zero Pumpoff */
  if (PCO2_OK != PCO2_State_PumpOff(MODE_ZERO_PUMP_OFF))
  {
    Error(ERROR_MSG_00101000, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_ZERO_PUMP_OFF,
                                   &run_data.ZPOFF,
                                   &i_data.ZPOFF))
  {
    Error(ERROR_MSG_00020020, LOG_Priority_High);
    return result;
  }

  /** Calibrate the Licor (ZERO) */
  Log("Start Zero Cal", LOG_Priority_Low);
  if (pco2.zero_on == true)
  {
    if (PCO2_OK != PCO2_State_Calibrate(PCO2_CAL_ZERO, 0.0))
    {
      Error(ERROR_MSG_00020040, LOG_Priority_High);
      return result;
    }
  }

  if (PCO2_OK != PCO2_SetMode(MODE_ZERO_POST_CAL))
  {
    Error(ERROR_MSG_00020080, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_ZERO_POST_CAL,
                                   &run_data.ZPPCAL,
                                   &i_data.ZPPCAL))
  {
    Error(ERROR_MSG_00020100, LOG_Priority_High);
    return result;
  }

  Log("End Zero", LOG_Priority_Low);
  result = PCO2_OK;
  return result;
}

/** @brief PCO2 Span State
 *
 * State which calibrates the Licor against a known span gas
 *
 * @param None
 *
 * @return result
 */
ePCO2Status_t PCO2_State_Span(float span)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);
  Log("Start Span", LOG_Priority_Low);
  /** Clear the char arrays */
  memset(pco2_str, 0, PCO2_STR_LEN);

  /** Low Current Mode OFF */
  BSP_Low_Current_Mode_OFF();

  /** Perform Pumpon state */
  if (PCO2_OK != PCO2_State_PumpOn(MODE_SPAN_GAS_ON))
  {
    Error(ERROR_MSG_00100200, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_SPAN_GAS_ON,
                                   &run_data.SPON,
                                   &i_data.SPON))
  {
    Error(ERROR_MSG_00040004, LOG_Priority_High);
    return result;
  }

  /** Perform Pumpoff */
  if (PCO2_OK != PCO2_State_PumpOff(MODE_SPAN_GAS_OFF))
  {
    Error(ERROR_MSG_00101000, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_SPAN_GAS_OFF,
                                   &run_data.SPOFF,
                                   &i_data.SPOFF))
  {
    Error(ERROR_MSG_00040020, LOG_Priority_High);
    return result;
  }

  /** Calibrate the Licor (SPAN) */
  if (pco2.span_on == true)
  {
    /** Use the i_data average pressures to calculate the pressure diff */
    float diff = i_data.SPON.pressure.mean - i_data.SPOFF.pressure.mean;

    /** Calibrate if the pressure in the Licor is > the spandiff pressure */
    if (diff > pco2.spandiff)
    {
      Log("Start Span Cal", LOG_Priority_Low);
      if (PCO2_OK != PCO2_State_Calibrate(PCO2_CAL_SPAN, span))
      {
        Error(ERROR_MSG_00040080, LOG_Priority_High);
        return result;
      }
      SPAN_SKIPPED = false;
    }
    else
    {
      Log("Span Cal Skipped", LOG_Priority_High);
      Error(ERROR_MSG_00040400, LOG_Priority_High);
      SPAN_SKIPPED = true;
    }
  }

  /** Set Mode PostCal */
  if (PCO2_OK != PCO2_SetMode(MODE_SPAN_POST_CAL))
  {
    Error(ERROR_MSG_00040100, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_SPAN_POST_CAL,
                                   &run_data.SPPCAL,
                                   &i_data.SPPCAL))
  {
    Error(ERROR_MSG_00040200, LOG_Priority_High);
    return result;
  }

  /** Add Span Statistical Data to Dry Data Calculations */
  pco2_dry.span.li_co2 = pco2_li_co2.mean;
  pco2_dry.span.li_temperature = pco2_li_temp.mean;
  pco2_dry.span.li_pressure = pco2_li_pres.mean;
  pco2_dry.span.rh_rh = pco2_rh_rh.mean;
  pco2_dry.span.rh_temperature = pco2_rh_temp.mean;

  /** Return */
  Log("End Span", LOG_Priority_Low);
  result = PCO2_OK;
  return result;
}

/** @brief PCO2 Equilibrate State
 *
 * State which equilibrates the sea water in air to read pCO2 in the water.
 *
 * @param None
 *
 * @return result
 */
STATIC ePCO2Status_t PCO2_State_Equilibrate(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;

  Log("", LOG_Priority_High);
  Log("Start Equilibration", LOG_Priority_Low);
  /** Clear the char arrays */
  memset(pco2_str, 0, PCO2_STR_LEN);

  /** Turn the Licor OFF */
  Log("Licor OFF", LOG_Priority_Low);
  LI8x0_PowerOFF();

  /** Perform Pumpon state */
  BSP_Hit_WD();
  if (PCO2_OK != PCO2_State_PumpOn(MODE_EQUILIBRATE_PUMP_ON))
  {
    Error(ERROR_MSG_00100200, LOG_Priority_High);
    return result;
  }

  /** Turn the Licor ON */
  PCO2_Licor_Warmup();

  /** Remove Power Supply from Low Power Mode */
  BSP_Low_Current_Mode_OFF();

  /** Wait for Licor Warmup Timeout */
  PCO2_SetTimeout(pco2.timeout1, pco2.LI_warmup);
  PCO2_WaitOnTimeout(pco2.timeout1);
  Log("LI Warmup complete", LOG_Priority_Low);
  Log("Equilibration Pump Complete", LOG_Priority_Low);

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_EQUILIBRATE_PUMP_ON,
                                   &run_data.EPON,
                                   &i_data.EPON))
  {
    Error(ERROR_MSG_00100004, LOG_Priority_High);
    return result;
  }

  Log("Equilibrate Pump Off", LOG_Priority_High);

  /** Perform Pumpoff */
  if (PCO2_OK != PCO2_State_PumpOff(MODE_EQUILIBRATE_PUMP_OFF))
  {
    Error(ERROR_MSG_00101000, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_EQUILIBRATE_PUMP_OFF,
                                   &run_data.EPOFF,
                                   &i_data.EPOFF))
  {
    Error(ERROR_MSG_00100040, LOG_Priority_High);
    return result;
  }

  /** Add Equil Statistical Data to Dry Data Calculations */
  pco2_dry.equil.li_co2 = pco2_li_co2.mean;
  pco2_dry.equil.li_temperature = pco2_li_temp.mean;
  pco2_dry.equil.li_pressure = pco2_li_pres.mean;
  pco2_dry.equil.rh_rh = pco2_rh_rh.mean;
  pco2_dry.equil.rh_temperature = pco2_rh_temp.mean;

  /** Return */
  Log("End Equilibration", LOG_Priority_Low);
  result = PCO2_OK;
  return result;
}

/** @brief PCO2 Air State
 *
 * State which measures CO2 in the air.
 *
 * @param None
 *
 * @return result
 */
STATIC ePCO2Status_t PCO2_State_Air(void)
{
  BSP_Hit_WD();
  ePCO2Status_t result = PCO2_FAIL;
  Log("", LOG_Priority_High);

  /** Clear the char arrays */
  memset(pco2_str, 0, PCO2_STR_LEN);

  /** Low Current Mode OFF */
  BSP_Low_Current_Mode_OFF();

  /** Perform Pumpon state */
  if (PCO2_OK != PCO2_State_PumpOn(MODE_AIR_PUMP_ON))
  {
    Error(ERROR_MSG_00100200, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_AIR_PUMP_ON,
                                   &run_data.APON,
                                   &i_data.APON))
  {
    Error(ERROR_MSG_00100400, LOG_Priority_High);
    return result;
  }

  /** Perform Pumpoff */
  if (PCO2_OK != PCO2_State_PumpOff(MODE_AIR_PUMP_OFF))
  {
    Error(ERROR_MSG_00101000, LOG_Priority_High);
    return result;
  }

  /** Sample the CO2 */
  if (PCO2_OK != PCO2_State_Sample(PCO2_MODE_AIR_PUMP_OFF,
                                   &run_data.APOFF,
                                   &i_data.APOFF))
  {
    Error(ERROR_MSG_00104000, LOG_Priority_High);
    return result;
  }

  /** Add Air Statistical Data to Dry Data Calculations */
  pco2_dry.air.li_co2 = pco2_li_co2.mean;
  pco2_dry.air.li_temperature = pco2_li_temp.mean;
  pco2_dry.air.li_pressure = pco2_li_pres.mean;
  pco2_dry.air.rh_rh = pco2_rh_rh.mean;
  pco2_dry.air.rh_temperature = pco2_rh_temp.mean;

  /** Calibrate O2 at conclusion of Air */
  PCO2_Set_O2Cal();

  /** Return */
  result = PCO2_OK;
  return result;
}

STATIC void PCO2_Licor_Warmup(void)
{
  char sstr[64];
  BSP_Hit_WD();

  /** Turn the Licor ON */
  memset(sstr, NULL, 64);
  Log("LI Warmup Start", LOG_Priority_High);
  sprintf(sstr, "Licor ON %u seconds", pco2.LI_warmup);
  Log(sstr, LOG_Priority_Low);
  LI8x0_PowerON();
}

STATIC ePCO2Status_t PCO2_State_PumpOn(ePCO2Mode_t mode)
{
  ePCO2Status_t result = PCO2_FAIL;
  char sstr[64];
  memset(sstr, NULL, 64);
  BSP_Hit_WD();

  /** Set the mode */
  switch (mode)
  {
  case MODE_ZERO_PUMP_ON:
    strcpy(pco2.mode, PCO2_MODE_ZERO_PUMP_ON);
    if (PCO2_OK != PCO2_SetMode(MODE_ZERO_PUMP_ON))
    {
      Error(ERROR_MSG_00020002, LOG_Priority_High);
      return result;
    }
    break;
  case MODE_SPAN_GAS_ON:
    strcpy(pco2.mode, PCO2_MODE_SPAN_GAS_ON);
    if (PCO2_OK != PCO2_SetMode(MODE_SPAN_GAS_ON))
    {
      Error(ERROR_MSG_00040002, LOG_Priority_High);
      return result;
    }
    break;
  case MODE_SPAN2_GAS_ON:
    strcpy(pco2.mode, PCO2_MODE_SPAN2_GAS_ON);
    if (PCO2_OK != PCO2_SetMode(MODE_SPAN2_GAS_ON))
    {
      Error(ERROR_MSG_00080002, LOG_Priority_High);
      return result;
    }
    break;
  case MODE_AIR_PUMP_ON:
    /** Set the flow controller to Air, Pump ON */
    strcpy(pco2.mode, PCO2_MODE_AIR_PUMP_ON);
    if (PCO2_OK != PCO2_SetMode(MODE_AIR_PUMP_ON))
    {
      Error(ERROR_MSG_00100200, LOG_Priority_High);
      return result;
    }
    strcpy(pco2.mode, PCO2_MODE_AIR_PUMP_ON);
    break;
  case MODE_EQUILIBRATE_PUMP_ON:
    /** Put Power Supply in Low Power Mode */
    BSP_Hit_WD();
    BSP_Low_Current_Mode_ON();
    /** Set AIR Mode Pump OFF */
    strcpy(pco2.mode, PCO2_MODE_EQUILIBRATE_PUMP_ON);
    if (PCO2_OK != PCO2_SetMode(MODE_EQUILIBRATE_PUMP_ON))
    {
      Error(ERROR_MSG_00100002, LOG_Priority_High);
      return result;
    }

    break;
  default:

    break;
  }

  /** Wait for Pump ON Timeout */
  BSP_Hit_WD();
  if (mode == MODE_EQUILIBRATE_PUMP_ON)
  {
    /** Report Start of Pump ON */
    sprintf(sstr, "Start Equil Pump %u seconds", pco2.equil.prequil);
    Log(sstr, LOG_Priority_High);
    PCO2_SetTimeout(pco2.timeout1, pco2.equil.prequil);

    /** Autocal the RTC */
    AM08X5_Autocal_RC_Set();
  }
  else if (mode == MODE_SPAN_GAS_ON)
  {
    /** Report Start of Pump ON */
    sprintf(sstr, "Start Span Gas %u seconds", pco2.spanflow.flowon);
    Log(sstr, LOG_Priority_High);
    PCO2_SetTimeout(pco2.timeout1, pco2.spanflow.flowon);
  }
  else
  {
    /** Report Start of Pump ON */
    sprintf(sstr, "Start Pump %u seconds", pco2.pump.pumpon);
    Log(sstr, LOG_Priority_High);
    PCO2_SetTimeout(pco2.timeout1, pco2.pump.pumpon);
  }
  PCO2_WaitOnTimeout(pco2.timeout1);
  Log("Pump ON Complete", LOG_Priority_High);

  /** Turn off the RTC autocal */
  if (mode == MODE_EQUILIBRATE_PUMP_ON)
  {
    AM08X5_Autocal_RC_Clear();
  }
  return PCO2_OK;
}

STATIC ePCO2Status_t PCO2_State_PumpOff(ePCO2Mode_t mode)
{
  ePCO2Status_t result = PCO2_FAIL;
  char sstr[64];
  memset(sstr, NULL, 64);
  BSP_Hit_WD();

  /** Set the mode */
  Log("Pump OFF Start", LOG_Priority_High);

  /** Turn the pump or gas off */
  if (PCO2_OK != PCO2_SetMode(mode))
  {
    switch (mode)
    {
    case MODE_ZERO_PUMP_OFF:
      Error(ERROR_MSG_00020008, LOG_Priority_High);
      break;
    case MODE_SPAN_GAS_OFF:
      Error(ERROR_MSG_00040008, LOG_Priority_High);
      break;
    case MODE_SPAN2_GAS_OFF:
      Error(ERROR_MSG_00080008, LOG_Priority_High);
      break;
    case MODE_EQUILIBRATE_PUMP_OFF:
      Error(ERROR_MSG_00100008, LOG_Priority_High);
      break;
    case MODE_AIR_PUMP_OFF:
      Error(ERROR_MSG_00100800, LOG_Priority_High);
      break;
    default:
      Log("Invalid Pump Off Mode", LOG_Priority_High);
      break;
    }
    return result;
  }

  /** Up the mode to the next state (Pump OFF + 1 = VENT or equivalent)*/
  mode += 1;

  /** Wait for the settling time */
  sprintf(sstr, "Start Settling %u seconds", pco2.pump.pumpwait);
  Log(sstr, LOG_Priority_High);
  PCO2_SetTimeout(pco2.timeout1, pco2.pump.pumpwait);
  PCO2_WaitOnTimeout(pco2.timeout1);
  BSP_Hit_WD();
  Log("Settling Complete", LOG_Priority_High);

  /** Set Vent Mode */
  if (PCO2_OK != PCO2_SetMode(mode))
  {
    switch (mode)
    {
    case MODE_ZERO_VENT:
      Error(ERROR_MSG_00020010, LOG_Priority_High);
      break;
    case MODE_SPAN_VENT:
      Error(ERROR_MSG_00040010, LOG_Priority_High);
      break;
    case MODE_SPAN2_VENT:
      Error(ERROR_MSG_00080010, LOG_Priority_High);
      break;
    case MODE_EQUILIBRATE_VENT:
      Error(ERROR_MSG_00100010, LOG_Priority_High);
      break;
    case MODE_AIR_VENT:
      Error(ERROR_MSG_00101000, LOG_Priority_High);
      break;
    default:
      Log("Invalid Pump Off Mode", LOG_Priority_High);
      break;
    }
    return result;
  }

  /** Set VENT Timer */
  sprintf(sstr, "Start Vent %u seconds", pco2.vent);
  Log(sstr, LOG_Priority_High);
  PCO2_SetTimeout(pco2.timeout1, pco2.vent);
  PCO2_WaitOnTimeout(pco2.timeout1);
  BSP_Hit_WD();
  Log("Vent Complete", LOG_Priority_High);

  /** Up the mode to the next state (Vent + 1 = Post-Cal or Post-Vent)*/
  mode += 1;

  /** Set Post-Cal/Post-Vent */
  if (PCO2_OK != PCO2_SetMode(mode))
  {
    switch (mode)
    {
    case MODE_ZERO_POST_CAL:
      Error(ERROR_MSG_00020040, LOG_Priority_High);
      break;
    case MODE_SPAN_POST_CAL:
      Error(ERROR_MSG_00040100, LOG_Priority_High);
      break;
    case MODE_SPAN2_POST_CAL:
      Error(ERROR_MSG_00080080, LOG_Priority_High);
      break;
    case MODE_EQUILIBRATE_POST_VENT:
      Error(ERROR_MSG_00100020, LOG_Priority_High);
      break;
    case MODE_AIR_POST_VENT:
      Error(ERROR_MSG_00102000, LOG_Priority_High);
      break;
    default:
      Log("Invalid Post-Cal/Post-Vent Mode", LOG_Priority_High);
      break;
    }
    return result;
  }

  /** 2 Second Delay to stabilize */
  sprintf(sstr, "Start Stabilize %u seconds", VENT_CLOSE_STABILIZE_TIME);
  Log(sstr, LOG_Priority_High);
  PCO2_SetTimeout(pco2.timeout1, VENT_CLOSE_STABILIZE_TIME);
  PCO2_WaitOnTimeout(pco2.timeout2);

  Log("Pumpoff Complete", LOG_Priority_High);

  return PCO2_OK;
}

/** @brief pCO2 Calibration Routine
 *
 * Routine to Calibrate the pCO2 Licor (CO2) sensor.
 *
 * @param state Calibration State (Zero, Span, Span2)
 *
 * @return None
 */
STATIC ePCO2Status_t PCO2_State_Calibrate(ePCO2Cal_t state, float concentration)
{
  ePCO2Status_t result = PCO2_FAIL;

  /** Choose which type of calibration */
  switch (state)
  {
  case PCO2_CAL_ZERO:
    Log("PCO2 Cal: Zero", LOG_Priority_Low);
    if (LICOR_FAIL == LI8x0_Zero())
    {
      Error(ERROR_MSG_00020001, LOG_Priority_High);
    }
    else
    {
      result = PCO2_OK;
    }
    break;
  case PCO2_CAL_SPAN:
    Log("PCO2 Cal: Span", LOG_Priority_Low);
    if (LICOR_FAIL == LI8x0_Span(concentration))
    {
      Error(ERROR_MSG_00040001, LOG_Priority_High);
    }
    else
    {
      result = PCO2_OK;
    }
    break;
  case PCO2_CAL_SPAN2:
    Log("PCO2 Cal: Span2", LOG_Priority_Low);
    if (LICOR_FAIL == LI8x0_Span2(concentration))
    {
      Error(ERROR_MSG_00080001, LOG_Priority_High);
    }
    else
    {
      result = PCO2_OK;
    }
    break;
  default:
    Log("PCO2 Cal: FAIL", LOG_Priority_Low);
    Error(ERROR_MSG_0130, LOG_Priority_Low);
    break;
  }

  /** Save Data */
  /** @todo Implement This for datalogger */
  if (PCO2_OK == result)
  {
    /** DATALOGGER_Write_LicorCal(pco2_str); */
  }

  return result;
}

/** @brief PCO2 Sample Routine
 *
 * Routine to sample the CO2 (Licor) Sensor
 *
 * @param None
 *
 * @return None
 */
STATIC ePCO2Status_t PCO2_State_Sample(const char *mode,
                                       sPCO2_run_data_t *rdata,
                                       sPCO2_idata_mode_t *idata)
{
  ePCO2Status_t result = PCO2_FAIL;
  uint16_t maxLicorSamples = ((pco2.sampleCO2 * 11) / 10) * 2;
  uint16_t minLicorSamples = ((pco2.sampleCO2 * 9) / 10) * 2;
  uint16_t sampleCnt = 0;
  sLicorData_t li_data;
  float rh_temperature = 0.0;
  float rh_humidity = 0.0;
  float o2_o2 = 0.0;
  char temp[175];
  sTimeReg_t tsample;
  BSP_Hit_WD();

  /** Clear the Licor UART Buffer */
  LI8x0_ClearBuffer();

  /** Clear the Stats Structs */
  STATS_Clear(&pco2_li_co2);
  STATS_Clear(&pco2_li_temp);
  STATS_Clear(&pco2_li_pres);
  STATS_Clear(&pco2_o2_o2);
  STATS_Clear(&pco2_rh_rh);
  STATS_Clear(&pco2_rh_temp);
  STATS_Clear(&pco2_rh_pres);
  STATS_Clear(&pco2_li_raw1);
  STATS_Clear(&pco2_li_raw2);

  /** Set Timeout */
  PCO2_SetTimeout(pco2.timeout1, pco2.sampleCO2);

  Log("Sample - Real Time", LOG_Priority_Low);

  char sstr[64];
  memset(sstr, NULL, 64);

  sprintf(sstr, "Sample Data %u seconds", pco2.sampleCO2);

  Log(sstr, LOG_Priority_Low);

  const char DataHeader[] = "State,TS,SN,CO2(ppm),Li_Temp(C),Li_Pres(kPa),Li_RawSample,Li_RawReference,RH(%),RH_T(C),O2(%)";
  Data(DataHeader);

  /** Grab current time */
  AM08X5_GetTime(&tsample);

  /** Number of samples */
  uint8_t numSamples = pco2.sampleCO2 * pco2.sample;
  uint16_t sampleTimeoutMs = 1000 / pco2.sample;
  while ((PCO2_GetTimeout(pco2.timeout1) == false) || (sampleCnt < numSamples))
  {
    BSP_Hit_WD();
    PCO2_SetSampleTimeout(pco2.sampleTimer, sampleTimeoutMs);

    /** Clear Licor Data Struct */
    LI8x0_Clear_Data(&li_data);
    /** Query Licor */
    LI8x0_Sample_Query();

    /** Read O2 */
    o2_o2 = MAX250_Read();

    /** Read RH */
    SHT3X_GetTempAndHumidity(&rh_temperature, &rh_humidity, REPEATAB_HIGH, MODE_POLLING, 10);

    /** Prepend mode to output */
    memset(pco2_str, NULL, PCO2_STR_LEN);

    uint16_t len = 0;
    len += strlen(mode);
    strcpy(pco2_str, mode);
    strcat(pco2_str, ", ");
    len += 1;
    /** Datalogger Write Raw Vals */
    /** @todo Implement This for datalogger */
    sTimeReg_t sTime;
    AM08X5_GetTime(&sTime);
    AM08X5_GetTimestamp_ISO8601_w_hundredths(temp);
    len += strlen(temp);
    strcat(pco2_str, temp);
    strcat(pco2_str, ", ");
    len += 1;

    /** Read Licor */
    if (LI8x0_Sample_Read(&li_data) == LICOR_OK)
    {

      sprintf(temp, "%s, %9.3f, %7.3f, %7.3f, %6ld, %6ld, %7.3f, %6.3f, %7.3f\0",
              sysinfo.SerialNumber, li_data.co2, li_data.celltemp, li_data.cellpres,
              li_data.raw.co2, li_data.raw.co2ref, rh_humidity, rh_temperature, o2_o2);
      if ((len + strlen(temp)) < 512)
      {
        strcat(pco2_str, temp);
      }
      Data(pco2_str);

      /** Add Data to Stats (Floats) */
      STATS_Add(&pco2_li_co2, li_data.co2);
      STATS_Add(&pco2_li_temp, li_data.celltemp);
      STATS_Add(&pco2_li_pres, li_data.cellpres);
      STATS_Add(&pco2_o2_o2, o2_o2);
      STATS_Add(&pco2_rh_rh, rh_humidity);
      STATS_Add(&pco2_rh_temp, rh_temperature);
      STATS_Add(&pco2_li_raw1, (float)li_data.raw.co2);
      STATS_Add(&pco2_li_raw2, (float)li_data.raw.co2ref);

      /** Add Data to Run Data struct */
      if (rdata->sample_idx < PCO2_SAMPLES_PER_MODE)
      {
        rdata->CO2[rdata->sample_idx] = li_data.co2;
        rdata->Li_pres[rdata->sample_idx] = li_data.cellpres;
        rdata->Li_temp[rdata->sample_idx] = li_data.celltemp;
        rdata->Li_raw[rdata->sample_idx] = li_data.raw.co2;
        rdata->Li_ref[rdata->sample_idx] = li_data.raw.co2ref;
        rdata->RH[rdata->sample_idx] = rh_humidity;
        rdata->RH_T[rdata->sample_idx] = rh_temperature;
        rdata->O2[rdata->sample_idx] = o2_o2;
        rdata->co2kzero[rdata->sample_idx] = NAN;
        rdata->co2kspan[rdata->sample_idx] = NAN;
        rdata->co2kspan2[rdata->sample_idx] = NAN;
        rdata->timestamp[rdata->sample_idx] = sTime;
        rdata->sample_idx++;
        pco2.LIData = li_data;
      }
      sampleCnt++;
      while ((PCO2_GetTimeout(pco2.sampleTimer) == false))
        ;
    }
  }

  Log("Stop Data", LOG_Priority_Low);
  BSP_Hit_WD();
  if ((sampleCnt >= minLicorSamples) && (sampleCnt <= maxLicorSamples))
  {
    result = PCO2_OK;
  }

  /** Calculate Stats (Floats) */
  STATS_Compute(&pco2_li_co2);
  STATS_Compute(&pco2_li_temp);
  STATS_Compute(&pco2_li_pres);
  STATS_Compute(&pco2_o2_o2);
  STATS_Compute(&pco2_rh_rh);
  STATS_Compute(&pco2_rh_temp);

  /** Calculate Stats (Ints) */
  STATS_Compute(&pco2_li_raw1);

  STATS_Compute(&pco2_li_raw2);

  /** Create the string */
  char timestamp[24];
  AM08X5_GetTimestamp_ISO8601(timestamp);
  Log("Sample - Statistics", LOG_Priority_High);
  const char StatsHeader[] = "State,SN,Timestamp,Li_Temp_ave(C),Li_Temp_sd,Li_Pres_ave(kPa),Li_Pres_sd,CO2_ave(PPM),CO2_SD,O2_ave(%),O2_S,RH_ave(%),RH_sd,RH_T_ave(C),Rh_T_sd,Li_RawSample_ave,Li_RawSample_sd,Li_RawDetector_ave,Li_RawReference_sd";
  Stats(StatsHeader);

  memset(temp, NULL, 175);
  sprintf(temp, "%s, %s, %7.3f, %7.3f, %7.3f, %7.3f, %9.3f, %8.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %7.3f, %8ld, %8ld, %8ld, %8ld",
          sysinfo.SerialNumber, timestamp,
          pco2_li_temp.mean, pco2_li_temp.std,
          pco2_li_pres.mean, pco2_li_pres.std,
          pco2_li_co2.mean, pco2_li_co2.std,
          pco2_o2_o2.mean, pco2_o2_o2.std,
          pco2_rh_rh.mean, pco2_rh_rh.std,
          pco2_rh_temp.mean, pco2_rh_temp.std,
          (long)pco2_li_raw1.mean, (long)pco2_li_raw1.std,
          (long)pco2_li_raw2.mean, (long)pco2_li_raw2.std);
  memset(pco2_str, 0, PCO2_STR_LEN);
  strcpy(pco2_str, mode);
  strcat(pco2_str, ", ");
  strcat(pco2_str, temp);

  /** Generate the IDATA field */
  memset(temp, NULL, 175);
  strncpy(idata->timestamp, timestamp, 23);
  idata->co2 = pco2_li_co2;
  idata->temperature = pco2_li_temp;
  idata->pressure = pco2_li_pres;
  idata->o2 = pco2_o2_o2;
  idata->rh = pco2_rh_rh;
  idata->rh_temperature = pco2_rh_temp;
  idata->raw1 = pco2_li_raw1;
  idata->raw2 = pco2_li_raw2;

  /** Write the log */
  Stats(pco2_str);

  result = PCO2_OK;
  return result;
}

STATIC ePCO2Status_t PCO2_SetMode(ePCO2Mode_t mode)
{
  ePCO2Status_t result = PCO2_FAIL;
  eFLOWStatus_t fresult = FLOW_FAIL;
  BSP_Hit_WD();
  switch (mode)
  {
  case MODE_ZERO_PUMP_ON:
    Log("ZERO PUMP ON", LOG_Priority_Low);
    _delay_ms(1);
    fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_ON);
    break;
  case MODE_ZERO_PUMP_OFF:
    Log("ZERO PUMP OFF", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_OFF);
    break;
  case MODE_ZERO_VENT:
    Log("ZERO VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_ZERO_VENT);
    break;
  case MODE_ZERO_POST_CAL:
    Log("ZERO POST", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_POST);
    break;
  case MODE_SPAN_GAS_ON:
  case MODE_SPAN2_GAS_ON:
    Log("SPAN GAS ON", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_SPAN_PUMP_ON);
    break;
  case MODE_SPAN_GAS_OFF:
  case MODE_SPAN2_GAS_OFF:
    Log("SPAN GAS OFF", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_SPAN_VENT);
    break;
  case MODE_SPAN_VENT:
  case MODE_SPAN2_VENT:
    Log("SPAN VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_SPAN_VENT);
    break;
  case MODE_SPAN_POST_CAL:
  case MODE_SPAN2_POST_CAL:
    Log("SPAN POST CAL", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_SPAN_PUMP_POST);
    break;
  case MODE_EQUILIBRATE_PUMP_ON:
    Log("EQUIL PUMP ON", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_ON);
    break;
  case MODE_EQUILIBRATE_PUMP_OFF:
    Log("EQUIL PUMP OFF", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_OFF);
    break;
  case MODE_EQUILIBRATE_VENT:
    Log("EQUIL VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_EQUIL_VENT);
    break;
  case MODE_EQUILIBRATE_POST_VENT:
    Log("EQUIL POST VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_POST);
    break;
  case MODE_AIR_PUMP_ON:
    Log("AIR PUMP ON", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_AIR_PUMP_ON);
    break;
  case MODE_AIR_PUMP_OFF:
    Log("AIR PUMP OFF", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_AIR_PUMP_OFF);
    break;
  case MODE_AIR_VENT:
    Log("AIR VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_AIR_VENT);
    break;
  case MODE_AIR_POST_VENT:
    Log("AIR POST VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_AIR_POST);
    break;
  case MODE_REST:
    Log("REST", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_REST);
    break;
  case MODE_DEPLOY:
    Log("DEPLOY", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_DEPLOY);
    break;
  case MODE_VENT:
    Log("VENT", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_AIR_PUMP_OFF);
    break;
  case MODE_PRG1:
    Log("PRG1", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG1);
    break;
  case MODE_PRG2:
    Log("PRG2", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG2);
    break;
  case MODE_PRG3:
    Log("PRG3", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG3);
    break;
  case MODE_PRG4:
    Log("PRG4", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG4);
    break;
  case MODE_PRG5:
    Log("PRG5", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG5);
    break;
  case MODE_PRG6:
    Log("PRG6", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG6);
    break;
  case MODE_PRG7:
    Log("PRG7", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG7);
    break;
  case MODE_PRG8:
    Log("PRG8", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRG8);
    break;

  // case MODE_PURGE:
  //   Log("PURGE", LOG_Priority_Low);
  //   fresult = FLOW_SetMode(FLOW_STATE_PURGE);
  //   break;
  // case MODE_CLEAR:
  //   Log("CLEAR", LOG_Priority_Low);
  //   fresult = FLOW_SetMode(FLOW_STATE_CLEAR);
  //   break;

  case MODE_PRESSURIZE:
    Log("AUTO-PRESSURIZE", LOG_Priority_Low);
    fresult = FLOW_SetMode(FLOW_STATE_PRESSURIZE);
  default:
    Error(ERROR_MSG_00014000, LOG_Priority_Low);
    break;
  }
#ifdef UNIT_TESTING
  printf("%u", fresult);
#endif

  if (fresult == FLOW_OK)
  {
    result = PCO2_OK;
  }
  else
  {
    Log("PCO2 Mode: FAIL", LOG_Priority_Low);
  }

  return result;
}

void PCO2_Task_Init(void)
{
  int16_t repeats;

  if (pco2.SampleMode != PCO2_SAMPLE_MODE_DEPLOY)
  {
    /** If the scheduler is empty, add tasks */
    if (SCHEDULER_Get_TaskCount() == 0)
    {
      sTimeReg_t start;
      sTimeReg_t interval;

      /************ PCO2 TASK ************/
      /** Clear the time structs */
      AM08X5_clear_time_struct(&start);
      AM08X5_clear_time_struct(&interval);

      /** Calculate the number of repeats */
      repeats = PCO2_FastMode_CalculateRepeats();

      /** Get the start time */
      PCO2_GetNormalStartTime(&start);

      /** Get the interval */
      PCO2_GetFastInterval(&interval);

      /** Add Run(s) to Scheduler */
      SCHEDULER_create_task(PCO2_FastMode, &start, &interval, repeats, "FAST");

      /** Add O2 Calibration to Scheduler */
      PCO2_ScheduleO2Cal();
    }
  }
}

void PCO2_ScheduleO2Cal(void)
{
  sTimeReg_t start;
  sTimeReg_t interval;

  /************ O2 TASK ************/
  /** Clear the time structs */
  AM08X5_clear_time_struct(&start);
  AM08X5_clear_time_struct(&interval);

  /** Get the Interval */
  PCO2_GetO2Time(&start);
  PCO2_GetO2Interval(&interval);

  /** Add to Scheduler */
  SCHEDULER_create_task(PCO2_CalO2, &start, &interval, -1, "O2CAL");
}

void PCO2_StreamSensors(void)
{
  sLicorData_t li_data;
  float rh_temperature = 0.0;
  float rh_humidity = 0.0;
  float o2_o2 = 0.0;
  char temp[120];

  memset(pco2_str, 0, PCO2_STR_LEN);

  /** Clear the Licor UART Buffer */
  LI8x0_ClearBuffer();

  /** Read Licor */
  LI8x0_ReadSample(&li_data);

  /** Read O2 */
  o2_o2 = MAX250_Read();

  /** Read RH */
  SHT3X_GetTempAndHumidity(&rh_temperature, &rh_humidity, REPEATAB_HIGH, MODE_POLLING, 10);

  /** Prepend mode to output */
  FLOW_GetState(pco2_str);
  strcat(pco2_str, ", ");

  /** Datalogger Write Raw Vals */
  /** @todo Implement This for datalogger */
  AM08X5_GetTimestamp_ISO8601_w_hundredths(temp);
  strcat(pco2_str, temp);
  strcat(pco2_str, ",");

  sprintf(temp, " %s, %9.3f, %7.3f, %7.3f, %6ld, %6ld, %7.3f, %6.3f, %7.3f\0",
          sysinfo.SerialNumber, li_data.co2, li_data.celltemp, li_data.cellpres,
          li_data.raw.co2, li_data.raw.co2ref, rh_humidity, rh_temperature, o2_o2);

  strcat(pco2_str, temp);
  Data(pco2_str);
}

/** Calculate Band Broadening Correction
 *
 *  Calculates the band broadening correction due to water vapor in the
 *  measurement.
 *
 *  Equation:
 *
 *  P_e = P * X( x_w)
 *
 *  where:
 *  X(x_w) = 1 + (a_w - 1) * x_w
 *
 *  Definitions:
 *  P_e is the Total Effective Pressure of the gas mixture
 *  P is the Total pressure of the gas mixture
 *  a_w is broadening coefficient (specific to each Licor)
 *  x_w is the mole fraction of water vapor
 *
 * @param pressure Total Effective Pressure of the gas mixture
 * @param rh Relative Humidity of gas
 * @param T Temperature of gas (degC)
 *
 * @return Total Effective Pressure (CO2) of the gas mixture
 */
float PCO2_Calculate_BandBroadening(float pressure, float rh, float T)
{

  return 0.0;
}

/** Calculate Mole Fraction
 *
 *  Calculate the Mole Fraction of H2O from RH
 *
 *  Equation:
 *
 *
 *
 *
 *
 *  @param rh Relative Humidity of Gas (%)
 *  @param T Temperature of gas (degC)
 *
 *  @return
 */
float PCO2_Calculate_MoleFraction(float rh, float T)
{

  return 0.0;
}

/** Calculate Dry CO2 */
STATIC void PCO2_Calculate_DryCO2(sPCO2Dry_t *T, sPCO2_idata_t *idata)
{
  /** Calculate Seawater CO2 (dry) */
  float SVP = (17.502 * T->equil.rh_temperature) / (240.97 + T->equil.rh_temperature);
  SVP = exp(SVP);
  SVP *= 0.61365;

  float VP_RH = (T->equil.rh_rh - T->span.rh_rh);
  VP_RH *= SVP;
  VP_RH /= 100;

  float VP_LI = (VP_RH * T->equil.li_temperature);
  VP_LI /= T->equil.rh_temperature;

  float xco2 = (T->equil.li_pressure - VP_RH) / T->equil.li_pressure;
  xco2 = T->equil.li_co2 / xco2;

  T->sw_xco2 = xco2;
  idata->sw_xco2 = xco2;
  run_data.DRY.sw_xco2 = xco2;

  SVP = 0.0;
  VP_RH = 0;
  VP_LI = 0.0;
  xco2 = 0.0;

  /** Calculate Atmosphere CO2 (dry) */
  SVP = (17.502 * T->air.rh_temperature) / (240.97 + T->air.rh_temperature);
  SVP = exp(SVP);
  SVP *= 0.61365;

  VP_RH = (T->air.rh_rh - T->span.rh_rh);
  VP_RH *= SVP;
  VP_RH /= 100;

  VP_LI = (VP_RH * T->air.li_temperature);
  VP_LI /= T->air.rh_temperature;

  xco2 = (T->air.li_pressure - VP_RH) / T->air.li_pressure;
  xco2 = T->air.li_co2 / xco2;

  T->air_xco2 = xco2;
  idata->air_xco2 = xco2;
  run_data.DRY.air_xco2 = xco2;

  /** Get the Timestamp */
  char str[75];
  char t_str[24];
  AM08X5_GetTimestamp_ISO8601(t_str);

  /** Create the string to send */
  memset(str, 0, 75);
  Dry("TS, SW_xCO2(dry), Atm_xCO2(dry)");
  sprintf(str, "%s, %4.2f, %4.2f", t_str, T->sw_xco2, T->air_xco2);
  Dry(str);

  return;
}

/** @brief Calculate the pump settling time
 *
 * Uses the pumpoff time, vent time and stabilize time to
 * create the pump settling time (pumpwait time).
 *
 * @param None
 *
 * @return None
 */
STATIC void calculate_pumpwait(void)
{
  uint16_t sub_time = VENT_CLOSE_STABILIZE_TIME + pco2.vent;
  pco2.pump.pumpwait = pco2.pump.pumpoff;
  if (pco2.pump.pumpwait > sub_time)
  {
    pco2.pump.pumpwait -= sub_time;
  }
  else
  {
    pco2.pump.pumpwait = 2;
  }
}

/** @brief Calculate the equilibration time
 *
 * Calculates the equilibration time before the Licor warmup.
 *
 * @param None\
 *
 * @return None
 */
STATIC void calculate_prequil(void)
{
  uint16_t sub_time = pco2.LI_warmup;
  pco2.equil.prequil = pco2.equil.equil;
  if (pco2.equil.prequil > sub_time)
  {
    pco2.equil.prequil -= sub_time;
  }
  else
  {
    pco2.pump.pumpwait = 2;
  }
}

STATIC void clear_run_data(sPCO2_run_data_t *data)
{
  uint8_t i;

  memcpy(data->serialnum, 0, 12);
  for (i = 0; i < PCO2_SAMPLES_PER_MODE; i++)
  {
    data->mode[i] = 0u;
    AM08X5_clear_time_struct(&data->timestamp[i]);
    data->CO2[i] = 0.0f;
    data->Li_temp[i] = 0.0f;
    data->Li_pres[i] = 0.0f;
    data->Li_raw[i] = 0u;
    data->Li_ref[i] = 0u;
    data->RH[i] = 0.0f;
    data->RH_T[i] = 0.0f;
    data->O2[i] = 0.0f;
    data->co2kzero[i] = 0.0f;
    data->co2kspan[i] = 0.0f;
    data->co2kspan2[i] = 0.0f;
  }
  data->sample_idx = 0;
}

STATIC void clear_full_run_data(sPCO2_sdata_t *data)
{
  clear_run_data(&data->ZPOFF);
  clear_run_data(&data->ZPON);
  clear_run_data(&data->ZPPCAL);
  clear_run_data(&data->SPOFF);
  clear_run_data(&data->SPON);
  clear_run_data(&data->SPPCAL);
  clear_run_data(&data->EPOFF);
  clear_run_data(&data->EPON);
  clear_run_data(&data->APOFF);
  clear_run_data(&data->APON);
  data->DRY.air_xco2 = 0.0f;
  data->DRY.sw_xco2 = 0.0f;
  memset(data->ERRORS, 0, 40);
}

STATIC void clear_idata_mode_data(sPCO2_idata_mode_t *data)
{
  STATS_Clear(&data->temperature);
  STATS_Clear(&data->pressure);
  STATS_Clear(&data->co2);
  STATS_Clear(&data->o2);
  STATS_Clear(&data->rh);
  STATS_Clear(&data->raw1);
  STATS_Clear(&data->raw2);
}

STATIC void clear_idata_data(sPCO2_idata_t *data)
{
  clear_idata_mode_data(&data->ZPOFF);
  clear_idata_mode_data(&data->ZPON);
  clear_idata_mode_data(&data->ZPPCAL);
  clear_idata_mode_data(&data->SPOFF);
  clear_idata_mode_data(&data->SPON);
  clear_idata_mode_data(&data->SPPCAL);
  clear_idata_mode_data(&data->EPOFF);
  clear_idata_mode_data(&data->EPON);
  clear_idata_mode_data(&data->APOFF);
  clear_idata_mode_data(&data->APON);
  data->COEFF.co2kzero = 0.0f;
  data->COEFF.co2kspan = 0.0f;
  data->COEFF.co2kspan2 = 0.0f;
  data->sw_xco2 = 0.0f;
  data->air_xco2 = 0.0f;
}