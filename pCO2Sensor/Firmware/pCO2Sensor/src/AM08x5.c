/** @file AM08x5.c
 *  @brief Ambiq AM0805 RTC Driver
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date February 27, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Controls the AM0805 and preps date/time for user
 *  @note
 *
 *
 *  @bug  No known bugs
 */

#include "AM08x5.h"

/************************************************************************
 *					        Static Variables
 ************************************************************************/
PERSISTENT sAM08X5_t RTC = {.I2C = {.param = {.selectClockSource = AMBIQ_RTC_I2C_CLK_SRC,
                                              .i2cClk = AMBIQ_RTC_I2C_CLK_FREQ,
                                              .dataRate = AMBIQ_RTC_I2C_DES_FREQ,
                                              .byteCounterThreshold = AMBIQ_RTC_I2C_BYTE_CNT_THRESHOLD,
                                              .autoSTOPGeneration = AMBIQ_RTC_I2C_AUTOSTOP},
                                    .baseAddress = AMBIQ_RTC_I2C_PORT,
                                    .mode = AMBIQ_RTC_I2C_MODE,
                                    .slaveAddress = AMBIQ_RTC_I2C_SLAVE_ADDR,
                                    .timeout = AMBIQ_RTC_I2C_TIMEOUT},
                            .timer = {
                                .range = AMBIQ_RTC_TIMER_RANGE,
                                .repeat = AMBIQ_RTC_TIMER_REPEAT,
                                .period = AMBIQ_RTC_TIMER_PERIOD},
                            .cal = {.freq = AMBIQ_RTC_CLK_FREQ}};

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC eAM08X5Status_t AM08X5_set_alarm(sTimeReg_t *t, uint8_t repeat, uint8_t intmode, uint8_t pin);
STATIC void AM08X5_WriteReg(uint8_t addr, uint8_t data);
STATIC void AM08X5_BurstWrite(uint8_t addr, uint8_t *data, uint8_t length);
STATIC void AM08X5_BurstRead(uint8_t addr, uint8_t *data, uint8_t length);
STATIC uint8_t AM08X5_ReadReg(uint8_t addr);
STATIC uint8_t AM08X5_ReadReg_BCD(uint8_t addr);
STATIC void AM08X5_ClearReg(uint8_t addr, uint8_t mask);
STATIC void AM08X5_SetReg(uint8_t addr, uint8_t mask);
STATIC eAM08X5Status_t AM08X5_osc_sel(uint8_t osc);
STATIC void AM08X5_get_time(void);
void set_battery_switch(void);
STATIC uint8_t bcd2dec(uint8_t bcdno);
STATIC uint8_t dec2bcd(uint8_t decno);
STATIC uint8_t valid_year(uint8_t year);
STATIC uint8_t valid_month(uint8_t month);
STATIC uint8_t valid_day(uint8_t date, uint8_t month, uint8_t year);
STATIC uint8_t valid_hour(uint8_t hour);
STATIC uint8_t valid_minute(uint8_t minute);
STATIC uint8_t valid_second(uint8_t sec);
STATIC eAM08X5Status_t AM08X5_ConfigSquareWave(uint8_t pin, eAM08X5_SQFS_t sqfs);
STATIC void uint8t_to_str(uint8_t val, char *str);
STATIC time_t AM08X5_calculate_epoch(sTimeReg_t *t);
/************************************************************************
 *					   Functions
 ************************************************************************/
eAM08X5Status_t AM08X5_Initialize(void)
{
  eAM08X5Status_t result = AM08X5_FAIL;

  /** Reset the I2C */
  BSP_I2C_Reset(&RTC.I2C);

  /** Initialize the I2C */
  BSP_I2C_Init(&RTC.I2C);

  /** Initialize the IRQ Lines */
  BSP_GPIO_Init(&RTC.nIRQ1.pin);
  BSP_GPIO_Init(&RTC.nIRQ2.pin);

  /** Initialize the timer line */
  BSP_GPIO_Init(&RTC.TIRQ.pin);

  /** Initialize the External Interrupt */
  BSP_GPIO_Init(&RTC.EXTI.pin);

  /** Initialize the RTC Watchdog */
  BSP_GPIO_Init(&RTC.WDI.pin);

  result = AM08X5_OK;
  return result;
}

void AM08X5_SetIRQ(void)
{

  /** Set the oscillator */
  if (AM08X5_osc_sel(0) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202000, LOG_Priority_High);
  }

  /** Apply the offset */
  if (AM08X5_OffsetCalculation(RTC.cal.freq) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202001, LOG_Priority_High);
  }

  if (AM08X5_ConfigSquareWave(2, AMBIQ_RTC_SQW_SETTING) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202002, LOG_Priority_High);
  }

  /** Set IRQ Pins */
  /** NOTE:  This function is inextensible.  It is a temporary fix for */
  /** the PCO2 project */

  uint8_t temp = 0;
  temp = (0x01 << 2) | (0x03); /** SQW on nIRQ2, nAIRQ on nIRQ1 */
  AM08X5_ClearReg(CONTROL_2_REG, 0xFF);
  AM08X5_WriteReg(CONTROL_2_REG, temp);

  /** Set the Oscillator to RC on Battery */
  set_battery_switch();
}

void AM08X5_GetRegisterValue(uint8_t reg, char *temp)
{
  uint8_t val;
  val = AM08X5_ReadReg_BCD(reg);
  sprintf(temp, "0x%02x", val);
}
void AM08X5_GetTime(sTimeReg_t *t)
{
  /** Read the time */
  AM08X5_get_time();

  /** Populate the struct */
  *t = RTC.time.time;
}

void AM08X5_GetTimestamp_ISO8601(char *str)
{
  /** Read the time */
  AM08X5_get_time();
  sprintf(str, "20%02u-%02u-%02uT%02u:%02u:%02uZ", RTC.time.time.year,
          RTC.time.time.month,
          RTC.time.time.date,
          RTC.time.time.hour,
          RTC.time.time.minute,
          RTC.time.time.second);
}

void AM08X5_GetTimestamp_ISO8601_w_hundredths(char *str)
{
  /** Read the time */
  AM08X5_get_time();

  sprintf(str, "20%02u-%02u-%02uT%02u:%02u:%02u.%01uZ", RTC.time.time.year,
          RTC.time.time.month,
          RTC.time.time.date,
          RTC.time.time.hour,
          RTC.time.time.minute,
          RTC.time.time.second,
          RTC.time.time.hundredth / 10);
}

void AM08X5_GetTimestamp(char *str, AM08X5_Datestamp_t ds, AM08X5_Timestamp_t ts)
{
  char temp[16];
  memset(temp, 0, 16);
  char temp2[4];

  /** Read the time */
  AM08X5_get_time();

  /** Format the Datestamp */
  switch (ds)
  {
  case DS_MMsDDsYYYY:
    sprintf(temp, "%02u/%02u/20%02u", RTC.time.time.month, RTC.time.time.date, RTC.time.time.year);
    break;
  case DS_MMsDDsYY:
    sprintf(temp, "%02u/%02u/%02u", RTC.time.time.month, RTC.time.time.date, RTC.time.time.year);
    break;
  case DS_MM_DD_YYYY:
    sprintf(temp, "%02u-%02u-20%02u", RTC.time.time.month, RTC.time.time.date, RTC.time.time.year);
    break;
  case DS_MM_DD_YY:
    sprintf(temp, "%02u-%02u-%02u", RTC.time.time.month, RTC.time.time.date, RTC.time.time.year);
    break;
  case DS_YYYY_MM_DD:
    sprintf(temp, "20%02u-%02u-%02u", RTC.time.time.year, RTC.time.time.month, RTC.time.time.date);
    break;
  case DS_YY_MM_DD:
    sprintf(temp, "%02u-%02u-%02u", RTC.time.time.year, RTC.time.time.month, RTC.time.time.date);
    break;
  case DS_YYYYsMMsDD:
    sprintf(temp, "20%02u/%02u/%02u", RTC.time.time.year, RTC.time.time.month, RTC.time.time.date);
    break;
  case DS_YYsMMsDD:
    sprintf(temp, "%02u/%02u/%02u", RTC.time.time.year, RTC.time.time.month, RTC.time.time.date);
    break;
  case DS_DD_MMM_YYYY:
    switch (RTC.time.time.month)
    {
    case 1:
      strcpy(temp2, "Jan");
      break;
    case 2:
      strcpy(temp2, "Feb");
      break;
    case 3:
      strcpy(temp2, "Mar");
      break;
    case 4:
      strcpy(temp2, "Apr");
      break;
    case 5:
      strcpy(temp2, "May");
      break;
    case 6:
      strcpy(temp2, "Jun");
      break;
    case 7:
      strcpy(temp2, "Jul");
      break;
    case 8:
      strcpy(temp2, "Aug");
      break;
    case 9:
      strcpy(temp2, "Sep");
      break;
    case 10:
      strcpy(temp2, "Oct");
      break;
    case 11:
      strcpy(temp2, "Nov");
      break;
    case 12:
      strcpy(temp2, "Dec");
      break;
    default:
      break;
    }
    sprintf(temp, "%02u %s %04u", RTC.time.time.date, temp2, RTC.time.time.year + 2000);
  case DS_DATE_NONE:
    break;
  default:
    break;
  }
  /** Copy the string */
  if (ds != DS_DATE_NONE)
  {
    strcpy(str, temp);
    memset(temp, 0, 16);
  }
  /** Determine if a space is necessary */
  if ((ds != DS_DATE_NONE) && (ts != TS_TIME_NONE))
  {
    strcat(str, " ");
  }

  /** Format the Timestamp */
  switch (ts)
  {
  case TS_TIME_HH_MM_SS_SS:
    sprintf(temp, "%02u:%02u:%02u.%02u", RTC.time.time.hour, RTC.time.time.minute, RTC.time.time.second, RTC.time.time.hundredth);
    break;
  case TS_TIME_HH_MM_SS:
    sprintf(temp, "%02u:%02u:%02u", RTC.time.time.hour, RTC.time.time.minute, RTC.time.time.second);
    break;
  case TS_TIME_HH_MM:
    sprintf(temp, "%02u:%02u", RTC.time.time.hour, RTC.time.time.minute);
    break;
  case TS_TIME_HH:
    sprintf(temp, "%02u", RTC.time.time.hour);
    break;
  case TS_TIME_NONE:
    break;
  default:
    break;
  }

  if (ts != TS_TIME_NONE)
  {
    strcat(str, temp);
  }

  /** Copy to the timestamp struct */
  strcpy(RTC.time.timestamp, str);

  return;
}

eAM08X5Status_t AM08X5_OffsetCalculation(float frequency)
{
  eAM08X5Status_t errors = AM08X5_FAIL;

  RTC.cal.freq = frequency;

  float Padj = 0.0;

  /* Calculate the offset (BASED ON AM08X5 Data Sheet XT Oscillator Digital Calibration */
  Padj = 32768.0f - frequency;
  Padj *= 1.0e6;
  Padj /= 32768;
  Padj /= 1.90735;

  int32_t adj = (int32_t)Padj;

  /* Determine the Offset, Crytal Cal and Command values */
  uint8_t OFFSETX = 0;
  uint8_t XTCAL = 0;
  uint8_t CMDX = 0;

  /* The following  is based on the AB08X5 Datasheet, Revision C, Page 41 */
  /* 5.9 Digital Calibration -> 5.9.1 XT Oscillator Digital Calibration instructions */
  if (adj < -320)
  {
    errors = AM08X5_FAIL;
  }
  else if (adj < -256)
  {
    XTCAL = 3;
    CMDX = 1;
    OFFSETX = (uint8_t)(adj + 192) / 2;
    errors = AM08X5_OK;
  }
  else if (adj < -192)
  {
    XTCAL = 3;
    CMDX = 0;
    OFFSETX = (uint8_t)(adj + 192);
    errors = AM08X5_OK;
  }
  else if (adj < -128)
  {
    XTCAL = 2;
    CMDX = 0;
    OFFSETX = (uint8_t)(adj + 128);
    errors = AM08X5_OK;
  }
  else if (adj < -64)
  {
    XTCAL = 1;
    CMDX = 0;
    OFFSETX = (uint8_t)(adj + 64);
    errors = AM08X5_OK;
  }
  else if (adj < 64)
  {
    XTCAL = 0;
    CMDX = 0;
    OFFSETX = (uint8_t)(adj);
    errors = AM08X5_OK;
  }
  else if (adj < 128)
  {
    XTCAL = 0;
    CMDX = 1;
    OFFSETX = (uint8_t)(adj / 2);
    errors = AM08X5_OK;
  }
  else
  {
    errors = AM08X5_FAIL;
  }

  /* Remove the 8th bit of OFFSET X */
  OFFSETX &= 0x7F;

  /* Set registers */
  uint8_t calxt = 0;
  uint8_t oscstat = 0;
  RTC.cal.offset = OFFSETX;

  /* create the cal and oscillator stat values */
  calxt = CMDX << 7;
  calxt |= OFFSETX;
  oscstat = XTCAL << 6;

  /* Save values into structure */
  RTC.cal.calxt = calxt;
  RTC.cal.oscstat = oscstat;

#ifndef UNIT_TESTING
  _delay_ms(1);
#endif

  /* Set Oscillator Selection to 1 (RC on power off) */
  if (AM08X5_osc_sel(0) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202000, LOG_Priority_High);
  }

  /* Write Oscillator Status Control Register (0x1D) */
  AM08X5_WriteReg(OSC_STATUS_REG, oscstat);

  /* Write Calibration XT Register (0x14) */
  AM08X5_WriteReg(CAL_XT_REG, calxt);

  /* Return the errors or success value */
  return errors;
}

eAM08X5Status_t AM08X5_SetTime(char *tstr)
{
  eAM08X5Status_t result = AM08X5_FAIL;
  sTimeReg_t t = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char temp[3];
  memset(temp, 0, 3);
  uint8_t buf[8];
  memset(buf, 0, 8);
  uint8_t tstrlen = strlen(tstr);

  /** Check for valid length */
  if (tstrlen >= 17)
  {
    if (tstr[0] == ' ')
    {
      tstr++;
    }
    /** Parse string */
    /** Strip Year */
    strncpy(temp, &tstr[0], 2);
    t.year = atoi(temp);

    /** Strip Month */
    strncpy(temp, &tstr[3], 2);
    t.month = atoi(temp);

    /** Strip Date */
    strncpy(temp, &tstr[6], 2);
    t.date = atoi(temp);

    /** Strip Year */
    strncpy(temp, &tstr[9], 2);
    t.hour = atoi(temp);

    /** Strip Month */
    strncpy(temp, &tstr[12], 2);
    t.minute = atoi(temp);

    /** Strip Date */
    strncpy(temp, &tstr[15], 2);
    t.second = atoi(temp);

    /** Validate entries */
    if ((true == valid_year(t.year)) &&
        (true == valid_month(t.month)) &&
        (true == valid_day(t.date, t.month, t.year)) &&
        (true == valid_hour(t.hour)) &&
        (true == valid_minute(t.minute)) &&
        (true == valid_second(t.second))

    )
    {
      /** Set the 24-hour mode */
      uint8_t temp;
      temp = AM08X5_ReadReg(CONTROL_1_REG);
      if (temp & 0x40)
      {
        if (t.hour > 12)
        {
          t.hour -= 12;
          t.hour |= 0x20; /* Set AM/PM */
        }
      }

      /** Set the WRTC bit to enable counter writes */

      /** Populate the buffer */
      buf[0] = t.hundredth;
      buf[1] = t.second;
      buf[2] = t.minute;
      buf[3] = t.hour;
      buf[4] = t.date;
      buf[5] = t.month;
      buf[6] = t.year;
      buf[7] = 0;

      /** Set the time on the IC */
      AM08X5_BurstWrite(HUNDREDTHS_REG, buf, 8);
    }

    result = AM08X5_OK;
  }

  return result;
}

void AM08X5_CrystalCal_Set(void)
{

  /** Clear the previous settings */
  AM08X5_WriteReg(CAL_XT_REG, 0x00u);
  AM08X5_WriteReg(OSC_STATUS_REG, 0x00u);

  /** Turn Square Wave off on Pin FOUT/nIRQ1 */
  if (AM08X5_ConfigSquareWave(AM08X5_SQW_OUT_OFF, SQFS_CENTURY) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202002, LOG_Priority_High);
  }

  /** Configure 32.768kHz Square Wave on Pin FOUT/nIRQ1 */
  if (AM08X5_ConfigSquareWave(AM08X5_CRYSTAL_CONFIG_PIN, SQFS_32768) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202002, LOG_Priority_High);
  }
}

void AM08X5_CrystalCal_Clear(void)
{
  if (AM08X5_ConfigSquareWave(AM08X5_SQW_OUT_OFF, SQFS_CENTURY) == AM08X5_FAIL)
  {
    Error(ERROR_MSG_00202002, LOG_Priority_High);
  }
}

void AM08X5_AlarmSet(sAM08X5Alarm_t *alarm, sTimeReg_t *t)
{
  AM08X5_clear_time_struct(&alarm->alarm);
  uint8_t alarmType = 0;
  if (t->second > 0)
  {
    alarmType = 6;
  }
  else if (t->minute > 0)
  {
    alarmType = 5;
  }
  else if (t->hour > 0)
  {
    alarmType = 4;
  }
  else if (t->date > 0)
  {
    alarmType = 2;
  }
  else if (t->month > 0)
  {
    alarmType = 2;
  }
  else if (t->year > 0)
  {
    alarmType = 1;
  }
  AM08X5_set_alarm(t, alarmType, 1, SYSTEM_RTC_ALARM_PIN);

  alarm->alarm = *t;
}
void AM08X5_AlarmGet_Str(sAM08X5Alarm_t *alarm, char *str)
{
  sTimeReg_t t = alarm->alarm;

  sprintf(str, "%04u-%02u-%02uT%02u:%02u:%02u", t.year + 2000,
          t.month,
          t.date,
          t.hour,
          t.minute,
          t.second);
  return;
}
void AM08X5_AlarmSet_with_Repeat(sAM08X5Alarm_t *alarm, sTimeReg_t *t, sTimeReg_t *rpt)
{
  AM08X5_AlarmSet(alarm, t);
  AM08X5_add_time_struct(&alarm->alarm, rpt, &alarm->nextAlarm);
  alarm->repeat = *rpt;
}

void AM08X5_AlarmSet_Next(sAM08X5Alarm_t *t)
{
  /** Is the next alarm after the current alarm ? */
  if ((0 < t->repeatCycles) || (-1 == t->repeatCycles))
  {
    if (true == AM08X5_compare_time(&t->nextAlarm, &t->alarm))
    {
      /** Is the next alarm after the current time ? */
      if (true == AM08X5_compare_time(&t->nextAlarm, &RTC.time.time))
      {
        AM08X5_AlarmSet_with_Repeat(t, &t->nextAlarm, &t->repeat);
      }
      else
      {
        Error(ERROR_MSG_00200002, LOG_Priority_Low);
      }
    }
    else
    {
      Error(ERROR_MSG_00200004, LOG_Priority_Low);
    }
    t->repeatCycles--;
  }
  return;
}

void AM08X5_AlarmClear(sAM08X5Alarm_t *alarm)
{
  AM08X5_clear_time_struct(&alarm->alarm);
  AM08X5_clear_time_struct(&alarm->nextAlarm);
  AM08X5_clear_time_struct(&alarm->repeat);
  AM08X5_set_alarm(&alarm->alarm, 0, 1, SYSTEM_RTC_ALARM_PIN);
}

void AM08X5_TimerSet(sAM08X5Timer_t *t, sTimeReg_t *delay)
{
  /** Set Timer Length Register (1 either way )*/
  AM08X5_ClearReg(TIMER_INITIAL_REG, 0xFF);
  AM08X5_WriteReg(TIMER_INITIAL_REG, 63);

  /** Set Timer Control Reg */
  uint8_t temp;
  if (t->period == 0)
  {
    /** 4.096Hz (TFS) */
    temp = 0x00;
  }
  else
  {
    /** Seconds (TFS 1/64) */
    temp = 0x01;
    AM08X5_ClearReg(TIMER_CTRL_REG, 0xe3);
    AM08X5_SetReg(TIMER_CTRL_REG, temp);
  }
  /** Set Repeating */
  /** TImer Enable, Timer Interrupt Mode, Timer Repeat */
  temp |= (0x01 << 7) | (0x01 << 5);
  AM08X5_SetReg(TIMER_CTRL_REG, temp);

  /** Set Countdown Timer */
  AM08X5_WriteReg(TIMER_REG, 1);

  /** Enable Timer IRQ */
  AM08X5_SetReg(INT_MASK_REG, 0x08);
}

void AM08X5_TimerClear(sAM08X5Timer_t *t)
{
}

uint8_t AM08X5_compare_time_non_epoch(sTimeReg_t *t1, sTimeReg_t *t2)
{
  uint8_t result = true;
  if (t1->year > t2->year)
  {
    return result;
  }

  if (t1->month > t2->month)
  {
    return result;
  }

  if (t1->date > t2->date)
  {
    return result;
  }

  if (t1->hour > t2->hour)
  {
    return result;
  }

  if (t1->minute > t2->minute)
  {
    return result;
  }

  if (t1->second > t2->second)
  {
    return result;
  }

  if (t1->hundredth > t2->hundredth)
  {
    return result;
  }

  result = false;
  return result;
}
uint8_t AM08X5_compare_time(sTimeReg_t *t1, sTimeReg_t *t2)
{
  uint8_t result = false;

  uint32_t time1;
  uint32_t time2;

  time1 = (uint32_t)AM08X5_calculate_epoch(t1);
  time2 = (uint32_t)AM08X5_calculate_epoch(t2);

  if (time1 > time2)
  {
    result = true;
  }

  return result;
}

void AM08X5_seconds_to_time_struct(sTimeReg_t *t, uint32_t seconds)
{
  AM08X5_clear_time_struct(t);

  t->second = seconds % 60;
  seconds -= t->second;

  seconds /= 60; /** seconds = minutes now */
  t->minute = seconds % 60;
  seconds -= t->minute;

  seconds /= 60; /** seconds = hours now */
  t->hour = seconds;
}

void AM08X5_clear_time_struct(sTimeReg_t *t)
{
  t->hundredth = 0;
  t->second = 0;
  t->minute = 0;
  t->hour = 0;
  t->date = 0;
  t->weekday = 0;
  t->month = 0;
  t->year = 0;
  t->century = 0;
  t->mode = 0;
}

eAM08X5Status_t AM08X5_add_time_struct(sTimeReg_t *t1, sTimeReg_t *t2, sTimeReg_t *t3)
{
  eAM08X5Status_t result = AM08X5_OK;

  uint8_t temp;
  uint8_t days;

  t3->year = t1->year + t2->year;
  t3->month = t1->month + t2->month;
  t3->date = t1->date + t2->date;
  t3->hour = t1->hour + t2->hour;
  t3->minute = t1->minute + t2->minute;
  t3->second = t1->second + t2->second;
  t3->hundredth = t1->hundredth + t2->hundredth;

  temp = t3->hundredth / 100;
  t3->hundredth %= 100;

  t3->second += temp;
  temp = t3->second / 60;
  t3->second %= 60;

  t3->minute += temp;
  temp = t3->minute / 60;
  t3->minute %= 60;

  t3->hour += temp;
  days = t3->hour / 24;
  t3->hour %= 24;

  /** Calc Month + year, then date */
  temp = t3->month / 13;
  t3->month %= 13;
  if (t3->month == 0)
  {
    t3->month += 1;
  }
  t3->year += temp;

  t3->date += days;
  days = t3->date;

  while (days > 0)
  {
    switch (t3->month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      if (days > 31)
      {
        t3->date = 1;
        t3->month += 1;
        days -= 31;
      }
      else
      {
        t3->date = days;
        days = 0;
      }
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if (days > 30)
      {
        t3->date = 1;
        t3->month += 1;
        days -= 30;
      }
      else
      {
        t3->date = days;
        days = 0;
      }
      break;
    case 2:
      if (((t3->year % 4 == 0) && (t3->year % 100 != 0)) || (t3->year % 400 == 0))
      {
        if (days > 29)
        {
          days -= 29;
          t3->date = 1;
          t3->month += 1;
        }
        else
        {
          t3->date = days;
          days = 0;
        }
      }
      else
      {
        if (days > 28)
        {
          days -= 28;
          t3->date = 0;
          t3->month += 1;
        }
        else
        {
          t3->date = days;
          days = 0;
        }
      }
      break;
    default:
      Error(ERROR_MSG_00200010, LOG_Priority_Med);
      break;
    }
    temp = t3->month / 13;
    t3->month %= 13;
    if (t3->month == 0)
    {
      t3->month += 1;
    }
    t3->year += temp;
  }

  return result;
}

eAM08X5Status_t AM08X5_convert_timestamp(char *ts, sTimeReg_t *t)
{
  eAM08X5Status_t result = AM08X5_OK;

  t->century = 0;
  t->date = 0;
  t->month = 0;
  t->year = 0;
  char temp[3];
  temp[2] = NULL;

  strncpy(temp, &ts[0], 2);
  t->hour = atoi(temp);
  strncpy(temp, &ts[3], 2);
  t->minute = atoi(temp);
  strncpy(temp, &ts[6], 2);
  t->second = atoi(temp);

  return result;
}

eAM08X5Status_t AM08X5_convert_timestamp_ISO8601(char *ts, sTimeReg_t *t)
{
  eAM08X5Status_t result = AM08X5_FAIL;
  char temp[5];
  memset(temp, 0, 5);

  ts += 2;

  /** Strip year values */
  strncpy(temp, ts, 2);
  t->year = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip month values */
  strncpy(temp, ts, 2);
  t->month = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip date values */
  strncpy(temp, ts, 2);
  t->date = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip hour values */
  strncpy(temp, ts, 2);
  t->hour = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip minute values */
  strncpy(temp, ts, 2);
  t->minute = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip second values */
  strncpy(temp, ts, 2);
  t->second = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip hundredths values */
  strncpy(temp, ts, 2);
  t->hundredth = atoi(temp);

  /** Check for valid values */
  if (valid_year(t->year) &&
      valid_month(t->month) &&
      valid_day(t->year, t->month, t->date) &&
      valid_hour(t->hour) &&
      valid_minute(t->minute) &&
      valid_second(t->second))
  {
    result = AM08X5_OK;
  }

  return result;
}

eAM08X5Status_t AM08X5_convert_timestamp_timeonly(char *ts, sTimeReg_t *t)
{
  eAM08X5Status_t result = AM08X5_FAIL;
  char temp[3];
  memset(temp, 0, 3);

  /** Strip hour values */
  strncpy(temp, ts, 2);
  t->hour = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip minute values */
  strncpy(temp, ts, 2);
  t->minute = atoi(temp);
  memset(temp, 0, 5);
  ts += 3;

  /** Strip second values */
  strncpy(temp, ts, 2);
  t->second = atoi(temp);
  memset(temp, 0, 5);

  /** Strip Hundredths (if they exist) */
  ts += 2;
  if (*ts == '.')
  {
    ts++;
    strncpy(temp, ts, 2);
    t->hundredth = atoi(temp);
  }

  /** Check for valid values */
  if (valid_hour(t->hour) &&
      valid_minute(t->minute) &&
      valid_second(t->second))
  {
    result = AM08X5_OK;
  }

  return result;
}

eAM08X5Status_t AM08X5_get_timestamp(char *ts, sTimeReg_t *t)
{
  eAM08X5Status_t result = AM08X5_FAIL;
  char year[3] = "00";
  char month[3] = "00";
  char date[3] = "00";
  char hour[3] = "00";
  char minute[3] = "00";
  char second[3] = "00";
  char hundredth[3] = "00";

  uint8t_to_str(t->year, year);
  uint8t_to_str(t->month, month);
  uint8t_to_str(t->date, date);
  uint8t_to_str(t->hour, hour);
  uint8t_to_str(t->minute, minute);
  uint8t_to_str(t->second, second);
  uint8t_to_str(t->hundredth, hundredth);

  /** Most recent working */
  *ts++ = '2';
  *ts++ = '0';
  *ts++ = year[0];
  *ts++ = year[1];
  *ts++ = '-';
  *ts++ = month[0];
  *ts++ = month[1];
  *ts++ = '-';
  *ts++ = date[0];
  *ts++ = date[1];
  *ts++ = 'T';
  *ts++ = hour[0];
  *ts++ = hour[1];
  *ts++ = ':';
  *ts++ = minute[0];
  *ts++ = minute[1];
  *ts++ = ':';
  *ts++ = second[0];
  *ts++ = second[1];

  *ts++ = '.';
  *ts++ = hundredth[0];
  *ts++ = hundredth[1];

  *ts++ = 'Z';
  *ts = NULL;

  result = AM08X5_OK;
  return result;
}

STATIC void uint8t_to_str(uint8_t val, char *str)
{
  uint8_t cnt_tens = 0;
  uint8_t cnt_hundreds = 0;

  while (val >= 10)
  {
    cnt_tens++;
    val -= 10;
  }

  while (cnt_tens >= 10)
  {
    cnt_hundreds++;
    cnt_tens -= 10;
  }

  str[0] = cnt_tens += '0';
  str[1] = val += '0';
  str[2] = NULL;
}

void AM08X5_BCD_to_Dec(sTimeReg_t *t)
{
}

void AM08X5_DEC_to_BCD(sTimeReg_t *t)
{
}

void AM08X5_Charge_ON(void)
{
  AM08X5_SetReg(CONFIG_KEY_REG, 0x9D);
  AM08X5_WriteReg(TRICKLE_REG, 0xA9);
}

void AM08X5_Charge_OFF(void)
{
  AM08X5_SetReg(CONFIG_KEY_REG, 0x9D);
  AM08X5_WriteReg(TRICKLE_REG, 0x00);
}

void AM08X5_Autocal_RC_Set(void)
{
  /** Clear the existing autocal setting */
  AM08X5_Autocal_RC_Clear();

  /** Set the new autocal setting */
  AM08X5_WriteReg(CONFIG_KEY_REG, 0xA1);
  AM08X5_SetReg(OSC_CONTROL_REG, 0x40);
}

void AM08X5_Autocal_RC_Clear(void)
{
  /** Clear the existing Autocal setting */
  AM08X5_WriteReg(CONFIG_KEY_REG, 0xA1);
  AM08X5_ClearReg(OSC_CONTROL_REG, 0x60);
}

/************************************************************************
 *					   Static Functions
 ************************************************************************/
/** @brief Configure a Square Wave Output
 *
 *  Sets a Square Wave Output on Pin
 *
 *  @param pin Pin to set Square Wave on
 *  @param sqfs Square Wave Signal Structure
 *
 *  @return result
 */
STATIC eAM08X5Status_t AM08X5_ConfigSquareWave(uint8_t pin, eAM08X5_SQFS_t sqfs)
{
  eAM08X5Status_t result = AM08X5_FAIL;
  uint8_t temp = 0x00;
  /** Setup the multiplexor */
  temp = AM08X5_ReadReg(SQW_REG);
  temp = (temp & 0x70) | sqfs | 0x80;

  /** Set Selected Pin */
  if (pin == 0x00)
  {
    /** Clear the Square Wave */
    temp &= 0x7F;
    result = AM08X5_OK;
  }
  else if (pin == AM08X5_OUTPUT_NIRQ1)
  {
    AM08X5_ClearReg(CONTROL_2_REG, 0x03);
    AM08X5_SetReg(CONTROL_2_REG, 0x01);
    result = AM08X5_OK;
  }
  else if (pin == AM08X5_OUTPUT_NIRQ2)
  {
    AM08X5_ClearReg(CONTROL_2_REG, 0x1C);
    AM08X5_SetReg(CONTROL_2_REG, 0x04);
    result = AM08X5_OK;
  }
  else
  {
    /** Error */
    Error(ERROR_MSG_00200020, LOG_Priority_Med);
  }

  /** Write the SQW Register */
  AM08X5_WriteReg(SQW_REG, temp);

  return result;
}

/** @brief Clear Register
 *
 *  Clear RTC Register of current value (uses mask)
 *
 *  @param addr Address of register
 *  @param mask Mask to use on register
 *
 *  @return None
 */
STATIC void AM08X5_ClearReg(uint8_t addr, uint8_t mask)
{
  uint8_t temp;

  temp = AM08X5_ReadReg(addr);
  temp &= ~mask;
  AM08X5_WriteReg(addr, temp);
}

/** @brief Set Register
 *
 *  Set RTC Register using mask
 *
 *  @param addr Address of Register
 *  @param mask Mask to use to set
 */
STATIC void AM08X5_SetReg(uint8_t addr, uint8_t mask)
{
  uint8_t temp;
  /** Read */
  BSP_I2C_GetData(&RTC.I2C, addr, &temp, 1);
  /** Modify */
  temp |= mask;
  /** Write */
  BSP_I2C_SendData(&RTC.I2C, addr, &temp, 1);
}

/** @brief Write Register
 *
 *  Write value to register
 *
 *  @param addr Address of Register
 *  @param data Value to write to register
 *
 *  @return None
 */
STATIC void AM08X5_WriteReg(uint8_t addr, uint8_t data)
{

  /** If it's a time value, convert it to BCD */
  if (addr < 0x0F)
  {
    data = dec2bcd(data);
  }

  /** Send data */
  BSP_I2C_SendData(&RTC.I2C, addr, &data, 1);

  return;
}

/** @brief Burst Register Write
 *
 *  Burst write to multiple registers
 *
 *  @param addr Address of first register
 *  @param *data Pointer to data array
 *  @param length Number of bytes to write
 *
 *  @return None
 */
STATIC void AM08X5_BurstWrite(uint8_t addr, uint8_t *data, uint8_t length)
{

  uint8_t i;
  uint8_t len = (length > 0x0E) ? 0x0E : length;
  /** If it's a time value, convert it to BCD */
  for (i = 0; i < len; i++)
  {
    data[i] = dec2bcd(data[i]);
  }

  /** Check length isn't too long */
  if (BUFFER_8_SIZE < length)
  {
    /** Report size error */
    Error(ERROR_MSG_00200080, LOG_Priority_Med);
  }
  /** Transfer address and data to structure */
  if (BSP_I2C_SendData(&RTC.I2C, addr, data, length) != BSP_OK)
  {
    /** Report send error */
    Error(ERROR_MSG_00200400, LOG_Priority_Med);
  }
}

/** @brief Read Register
 *
 *  Read the value from a register
 *
 *  @param addr Address of register
 *
 *  @return Value
 */
STATIC uint8_t AM08X5_ReadReg(uint8_t addr)
{
  uint8_t data = 0;
  /** Transfer address and data to structure */
  BSP_I2C_put(&RTC.I2C, addr);
  /** Read the register */
  data = BSP_I2C_get(&RTC.I2C);

  /** If it's a time value, convert it to DEC */
  if (addr < 0x0F)
  {
    data = bcd2dec(data);
  }

  return data;
}

/** @brief Read Register as BCD
 *
 * @param addr Address to read
 *
 * @return value read
 */
STATIC uint8_t AM08X5_ReadReg_BCD(uint8_t addr)
{
  uint8_t data = 0;
  /** Transfer address and data to structure */
  BSP_I2C_put(&RTC.I2C, addr);
  /** Read the register */
  data = BSP_I2C_get(&RTC.I2C);

  return data;
}

/** @brief Burst Register Read
 *
 *  Burst read from multiple registers
 *
 *  @param addr Address of first read
 *  @param *data Pointer to data array
 *  @param length Number of bytes to read
 *
 *  @return None
 */
STATIC void AM08X5_BurstRead(uint8_t addr, uint8_t *data, uint8_t length)
{
  uint8_t i;
  uint8_t len = (length > 0x0E) ? 0x0E : length;

  BSP_I2C_put(&RTC.I2C, addr);
  BSP_I2C_gets(&RTC.I2C, data, length);

  for (i = 0; i < len; i++)
  {
    data[i] = bcd2dec(data[i]);
  }

  return;
}

/** @brief Oscillator Select
 *
 *  Oscillator Selection
 *
 *  @param osc Oscillator to select.  0 = XT, no switching, 1=XT w/switching, 2=RC
 *
 *  @return result
 */
eAM08X5Status_t AM08X5_osc_sel(uint8_t osc)
{
  eAM08X5Status_t result = AM08X5_FAIL;

  uint8_t i;
  uint8_t temp;

  /** Read the osciallator control register and mask bits */
  temp = AM08X5_ReadReg(OSC_CONTROL_REG);
  temp &= 0x67;

  /** Enable Oscillator Register writes */
  AM08X5_WriteReg(CONFIG_KEY_REG, 0xA1);

  switch (osc)
  {
  case 0x00:
    AM08X5_WriteReg(OSC_CONTROL_REG, temp);
    break;
  case 0x01:
    temp |= 0x10; /** Set the AOS */
    AM08X5_WriteReg(OSC_CONTROL_REG, temp);
    break;
  default:
    temp |= 0x80; /** Set the OSEL */
    AM08X5_WriteReg(OSC_CONTROL_REG, temp);
    break;
  }

  /** Make sure the switch occured by testing OMODE */
  for (i = 0; i < 100; i++)
  {
    /** Delay between reads */
#ifndef UNIT_TESTING
    _delay_ms(1);
#endif

    temp = AM08X5_ReadReg(OSC_STATUS_REG);
    temp = (temp & 0x10) >> 4;
    if (temp == (osc >> 1))
    {
      break;
    }
  }

  if (temp == (osc >> 1))
  {
    result = AM08X5_OK;
  }

  return result;
}

/** @brief Get Current time
 *
 *  Retreive time and populate RTC struct
 *
 *  @param None
 *
 *  @return None
 */
void AM08X5_get_time(void)
{
  uint8_t temp_buff[8];
  uint8_t addr = HUNDREDTHS_REG;
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    temp_buff[i] = AM08X5_ReadReg(addr++);
  }

  /** Convert the time registers */
  RTC.time.time.hundredth = temp_buff[0];
  RTC.time.time.second = temp_buff[1];
  RTC.time.time.minute = temp_buff[2];
  RTC.time.time.hour = temp_buff[3];
  RTC.time.time.date = temp_buff[4];
  RTC.time.time.month = temp_buff[5];
  RTC.time.time.year = temp_buff[6];
  RTC.time.time.weekday = temp_buff[7];

  /** Get the current hours format mode 12:24. */
  temp_buff[0] = AM08X5_ReadReg(CONTROL_1_REG);
  if ((temp_buff[0] & 0x40) == 0)
  {
    /* 24-hour mode */
    RTC.time.time.mode = 2;
    RTC.time.time.hour = RTC.time.time.hour & 0x3F; /* Get tens:ones */
  }
  else
  {
    /* 12-hour mode.  Get PM:AM. */
    RTC.time.time.mode = (RTC.time.time.hour & 0x20) ? 1 : 0; /* PM : AM */
    RTC.time.time.hour &= 0x1F;                               /* Get tens:ones */

    if (RTC.time.time.mode == 1)
    {
      RTC.time.time.hour += 12;
    }
  }

  /* Get the century bit. */
  temp_buff[0] = AM08X5_ReadReg(STATUS_REG);
  RTC.time.time.century = (temp_buff[0] & 0x80) ? 1 : 0;

  return;
}

/** @brief Set the Battery Switch configuration
 *
 * Set the oscillator configuration to switch to RC oscillator
 * when the Battery Mode is enabled.
 *
 * @param None
 *
 * @return None
 */
void set_battery_switch(void)
{
  /** Set the battery switch reference voltage */
  AM08X5_SetReg(CONFIG_KEY_REG, 0x9D);
  AM08X5_SetReg(BREF_REG, 0x07);

  /** Set the Oscillator Control to switch to RC when battery powered */
  AM08X5_SetReg(CONFIG_KEY_REG, 0xA1);
  AM08X5_SetReg(OSC_CONTROL_REG, 0x10);
}

/** @brief BCD to Decimal
 *
 *  Convert BCD to Decimal value
 *
 *  @param bcdno BCD Number
 *
 *  @return Decimal value
 */
STATIC uint8_t bcd2dec(uint8_t bcdno)
{
  return ((bcdno >> 4) * 10) + (bcdno & 0x0F);
}

/** @brief Decimal to BCD
 *
 *  Convert Decimal to BCD
 *
 *  @param decno Decimal Number
 *
 *  @return BCD value
 */
STATIC uint8_t dec2bcd(uint8_t decno)
{
  return (((decno / 10) << 4) | (decno % 10));
}

/** @brief Valid year
 *
 *  Checks for a valid year value
 *
 *  @param year
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_year(uint8_t year)
{
  uint8_t result = false;
  if ((year >= 18) && (year <= 99))
  {
    result = true;
  }
  return result;
}

/** @brief Valid month
 *
 *  Checks for a valid year month
 *
 *  @param month
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_month(uint8_t month)
{
  uint8_t result = false;
  if ((month >= 1) && (month <= 12))
  {
    result = true;
  }
  return result;
}

/** @brief Valid day
 *
 *  Checks for a valid day value
 *
 *  @param day
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_day(uint8_t date, uint8_t month, uint8_t year)
{
  uint8_t result = false;
  uint16_t yr = 2000 + (uint16_t)year;

  if (date > 0)
  {
    switch (month)
    {
    case 1:  /** January */
    case 3:  /** March */
    case 5:  /** May */
    case 7:  /** July */
    case 8:  /** August */
    case 10: /** October */
    case 12: /** December */
      if (date < 32)
      {
        result = true;
      }
      break;
    case 9:  /** September */
    case 4:  /** April */
    case 6:  /** June */
    case 11: /** November */
      if (date < 31)
      {
        result = true;
      }
      break;
    case 2: /** February */
      if (((yr % 4 == 0) && (yr % 100 != 0)) || (yr % 400 == 0))
      {
        if (date < 30)
        {
          result = true;
        }
      }
      else
      {
        if (date < 29)
        {
          result = true;
        }
      }
    }
  }
  return result;
}

/** @brief Valid hour
 *
 *  Checks for a valid hour value
 *
 *  @param hour
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_hour(uint8_t hour)
{
  uint8_t result = false;
  if (hour < 24)
  {
    result = true;
  }
  return result;
}

/** @brief Valid minute
 *
 *  Checks for a valid year minute
 *
 *  @param minute
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_minute(uint8_t minute)
{
  uint8_t result = false;
  if (minute < 60)
  {
    result = true;
  }
  return result;
}

/** @brief Valid second
 *
 *  Checks for a valid second value
 *
 *  @param second
 *
 *  @return Valid=True, Invalid=False
 */
STATIC uint8_t valid_second(uint8_t sec)
{
  uint8_t result = false;
  if (sec < 60)
  {
    result = true;
  }
  return result;
}

/** @brief Set alarm registers
 *
 *  Sets the appropriate alarm registers
 *
 *  @param *t Pointer to Time structure
 *  @param repeat Repeat mode
 *  @param intmode Interrupt mode
 *  @param pin Pin to interrupt on with Alarm
 *
 *  @return result
 */
STATIC eAM08X5Status_t AM08X5_set_alarm(sTimeReg_t *t, uint8_t repeat, uint8_t intmode, uint8_t pin)
{
  eAM08X5Status_t result = AM08X5_FAIL;

  uint8_t temp;
  uint8_t buff[8];

  /** Populate the buffer */
  temp = AM08X5_ReadReg(CONTROL_1_REG);

  if (temp & 0x40)
  {

    t->hour |= 0x20;
  }

  buff[0] = t->hundredth;
  buff[1] = t->second;
  buff[2] = t->minute;
  buff[3] = t->hour;
  buff[4] = t->date;
  buff[5] = t->month;
  buff[6] = 0;

  /** Clear the RPT Field */
  AM08X5_ClearReg(TIMER_CTRL_REG, 0x1C);

  /** Clear the AIE bit and IM Field */
  AM08X5_ClearReg(INT_MASK_REG, 0x64);

  /** Clear the ALM Flag */
  AM08X5_ClearReg(STATUS_REG, 0x04);

  /** Set Control Register based on Pin */
  if (1 == pin)
  {
    /** Interrupt on nIRQ */
    temp = AM08X5_ReadReg(CONTROL_2_REG);
    temp |= 0x03;
    if (0 != temp)
    {
      AM08X5_WriteReg(CONTROL_2_REG, temp);
    }
  }
  else if (2 == pin)
  {
    /** Interrupt on nIRQ2 */
    temp = AM08X5_ReadReg(CONTROL_2_REG);
    temp |= 0x1C;
    if (0 != temp)
    {
      AM08X5_SetReg(STATUS_REG, 0x04);
      AM08X5_ClearReg(CONTROL_2_REG, 0x1C);
      AM08X5_SetReg(CONTROL_2_REG, 0x0C);
    }
  }
  else
  {
    Error(ERROR_MSG_00200040, LOG_Priority_Med);
  }

  /** Set the Alarm Registers */

  if (0 != repeat)
  {
    temp = (repeat << 2);
    AM08X5_SetReg(TIMER_CTRL_REG, temp);
    AM08X5_BurstWrite(ALARM_HUNDRS_REG, buff, 7);
    temp = (intmode << 5);
    temp |= 0x04;
    AM08X5_SetReg(INT_MASK_REG, temp);
  }
  else
  {
    AM08X5_SetReg(INT_MASK_REG, 0x60);
  }

  /** for testing, read alarm regs */
  uint8_t read[48];
  AM08X5_BurstRead(ALARM_HUNDRS_REG, read, 7);
  read[0] = read[1];

  /** For testing, read time regs */
  AM08X5_BurstRead(0x00, read, 48);
  read[0] = read[1];

  RTC.nIRQ1.alarm = *t;

  result = AM08X5_OK;
  return result;
}

/** Calculate Epoch
 *
 * Calculates Epoch time (in seconds) from time struct
 *
 * @param *t Pointer to time struct to convert
 *
 * @return time_t Epoch time
 */
STATIC time_t AM08X5_calculate_epoch(sTimeReg_t *t)
{
  struct tm te;
  time_t epoch;

  te.tm_year = t->year + 2000;
  te.tm_year -= 1900;
  te.tm_mon = t->month;
  te.tm_mday = t->date;
  te.tm_hour = t->hour;
  te.tm_min = t->minute;
  te.tm_sec = t->second;
  te.tm_isdst = -1;
  te.tm_wday = -1;
  te.tm_yday = -1;

  epoch = mktime(&te);

  return epoch;
}