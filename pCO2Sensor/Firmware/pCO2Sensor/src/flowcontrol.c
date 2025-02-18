/** @file flowcontrol.c
 *  @brief Flow control driver
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date March 23, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *  @note Command interface between micro and PMEL EDD Flow Control Board
 *  @note
 *
 *  @bug  No known bugs
 */
#include "flowcontrol.h"

/************************************************************************
 *					        Static Variables
 ************************************************************************/
STATIC PERSISTENT sFLOW_t Flow = {.currentState = FLOW_STATE_DEPLOY,
                                  .nextState = FLOW_STATE_DEPLOY,
                                  .Power = {
                                      .pin = {.port = FLOW_POWER_PORT,
                                              .pin = FLOW_POWER_PIN,
                                              .mode = FLOW_POWER_DIR,
                                              .inverted = FLOW_POWER_POL},
                                      .state = 0,

                                  },
                                  .UART = {.nReset = {.port = MAX310X_NRESET_PORT, .pin = MAX310X_NRESET_PIN, .mode = MAX310X_NRESET_DIR, .inverted = MAX310X_NRESET_POL}, .cs = {.port = MAX310X_CS_PORT, .pin = MAX310X_CS_PIN, .mode = MAX310X_CS_DIR, .inverted = MAX310X_CS_POL}, .nIRQ = {.port = MAX310X_NIRQ_PORT, .pin = MAX310X_NIRQ_PIN, .mode = MAX310X_NIRQ_DIR, .inverted = MAX310X_NIRQ_POL}, .nI2C = {.port = MAX310X_I2C_SEL_PORT, .pin = MAX310X_I2C_SEL_PIN, .mode = MAX310X_I2C_SEL_DIR, .inverted = MAX310X_I2C_SEL_POL}, .SPI = {.param = {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .selectClockSource = MAX310X_SPI_CLK_SRC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .clockSourceFrequency = MAX310X_SPI_CLK_FREQ,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .desiredSpiClock = MAX310X_SPI_CLK_DES_FREQ,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .msbFirst = MAX310X_SPI_MSB_FIRST,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .clockPhase = MAX310X_SPI_CLK_PHASE,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .clockPolarity = MAX310X_SPI_CLK_POL,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           .spiMode = MAX310X_SPI_CLK_MODE,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       },
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       .baseAddress = MAX310X_SPI_PORT

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               },
                                           .Baud = {.fref = MAX310X_CRYSTAL_FREQUENCY, .rateMode = MAX310X_DEFAULT_RATEMODE, .Baudrate = MAX310X_DEFAULT_BAUDRATE}}};

const char mode[] = "MODE";

const char mode_deploy[] = "DEPLOY";
const char mode_zero[] = "ZERO";
const char mode_span[] = "SPAN";
const char mode_equil[] = "EQUIL";
const char mode_air[] = "AIR";
const char mode_rest[] = "REST";

const char mode_pump_on[] = "_ON";
const char mode_pump_off[] = "_OFF";
const char mode_pump_post[] = "_POST";

static PERSISTENT char readStr[128];

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC eFLOWStatus_t FLOW_Startup(sFLOW_t *f);
STATIC void FLOW_PowerON(sFlowPowerState_t *p);
STATIC void FLOW_PowerOFF(sFlowPowerState_t *p);
STATIC eFLOWState_t FLOW_PumpStateChange(eFLOWState_t state, bool pumpon);
STATIC eFLOWStatus_t FLOW_SetMode_Command(sFLOW_t *Flow,
                                          char *str,
                                          eFLOWState_t state);
/************************************************************************
 *					         Functions
 ************************************************************************/
eFLOWStatus_t FLOW_Init(void)
{
  eFLOWStatus_t result = FLOW_FAIL;

  CONSOLE_Close(); /** <---- THIS IS TO PREVENT USB FROM HANGING */

  /** Initialize the UART */
  MAX310X_Init(&Flow.UART);

  FLOW_Wakeup();

  /** Run the Startup Routine */
  if (FLOW_OK != FLOW_Startup(&Flow))
  {
    Error(ERROR_MSG_00400002, LOG_Priority_High);
    return result;
  }

  CONSOLE_Init(); /** <---- THIS IS TO PREVENT USB FROM HANGING */

  result = FLOW_OK;
  return result;
}

eFLOWStatus_t FLOW_SetMode(eFLOWState_t state)
{
  eFLOWStatus_t result = FLOW_FAIL;
  char str[32];
  memset(str, 0, 32);
  strcpy(str, "mode=");

  /** Turn the Flow Control Module ON */
  if (Flow.Power.state == false)
  {
    FLOW_PowerON(&Flow.Power);
  }

  /** Create the sendstring */
  switch (state)
  {
  case FLOW_STATE_ZERO_PUMP_ON:
    strcat(str, "ZPON");
    break;
  case FLOW_STATE_ZERO_PUMP_OFF:
    strcat(str, "ZPOFF");
    break;
  case FLOW_STATE_ZERO_VENT:
    strcat(str, "ZPVENT");
    break;
  case FLOW_STATE_ZERO_PUMP_POST:
    strcat(str, "ZPPCAL");
    break;
  case FLOW_STATE_SPAN_PUMP_ON:
    strcat(str, "SPON");
    break;
  case FLOW_STATE_SPAN_PUMP_OFF:
    strcat(str, "SPOFF");
    break;
  case FLOW_STATE_SPAN_VENT:
    strcat(str, "SPVENT");
    break;
  case FLOW_STATE_SPAN_PUMP_POST:
    strcat(str, "SPPCAL");
    break;
  case FLOW_STATE_EQUIL_PUMP_ON:
    strcat(str, "EPON");
    break;
  case FLOW_STATE_EQUIL_PUMP_OFF:
    strcat(str, "EPOFF");
    break;
  case FLOW_STATE_EQUIL_VENT:
    strcat(str, "EPVENT");
    break;
  case FLOW_STATE_EQUIL_PUMP_POST:
    strcat(str, "EPPOST");
    break;
  case FLOW_STATE_AIR_PUMP_ON:
    strcat(str, "APON");
    break;
  case FLOW_STATE_AIR_PUMP_OFF:
    strcat(str, "APOFF");
    break;
  case FLOW_STATE_AIR_VENT:
    strcat(str, "APVENT");
    break;
  case FLOW_STATE_AIR_POST:
    strcat(str, "APPOST");
    break;
  case FLOW_STATE_REST:
    strcat(str, "REST");
    break;
  case FLOW_STATE_DEPLOY:
    strcat(str, "DEPLOY");
    break;
  case FLOW_STATE_VENT:
    strcat(str, "VENT");
    break;
  case FLOW_STATE_PRESSURIZE:
    strcat(str, "PRES");
    break;
  case FLOW_STATE_TEST_ZERO_0:
    strcat(str, "ZERO_0");
    break;
  case FLOW_STATE_TEST_ZERO_1:
    strcat(str, "ZERO_1");
    break;
  case FLOW_STATE_TEST_ZERO_2:
    strcat(str, "ZERO_2");
    break;
  case FLOW_STATE_TEST_SPAN_0:
    strcat(str, "SPAN_0");
    break;
  case FLOW_STATE_TEST_SPAN_1:
    strcat(str, "SPAN_1");
    break;
  case FLOW_STATE_TEST_SPAN_2:
    strcat(str, "SPAN_2");
    break;
  case FLOW_STATE_TEST_EQUIL_0:
    strcat(str, "EQUIL_0");
    break;
  case FLOW_STATE_TEST_EQUIL_1:
    strcat(str, "EQUIL_1");
    break;
  case FLOW_STATE_TEST_AIR_0:
    strcat(str, "AIR_0");
    break;
  case FLOW_STATE_TEST_AIR_1:
    strcat(str, "AIR_1");
    break;
  case FLOW_STATE_TEST_PUMP:
    strcat(str, "PUMP_TOGGLE");
    break;
  case FLOW_STATE_PRG1:
    strcat(str, "PRG1");
    break;
  case FLOW_STATE_PRG2:
    strcat(str, "PRG2");
    break;
  case FLOW_STATE_PRG3:
    strcat(str, "PRG3");
    break;
  case FLOW_STATE_PRG4:
    strcat(str, "PRG4");
    break;  
  case FLOW_STATE_PRG5:
    strcat(str, "PRG5");
    break;
  case FLOW_STATE_PRG6:
    strcat(str, "PRG6");
    break;
  case FLOW_STATE_PRG7:
    strcat(str, "PRG7");
    break;
  case FLOW_STATE_PRG8:
    strcat(str, "PRG8");
    break;
//  case FLOW_STATE_PURGE:
//    strcat(str, "PURGE");
//    break;
//  case FLOW_STATE_CLEAR:
//    strcat(str, "CLEAR");
//    break;
  default:
    Error(ERROR_MSG_00400004, LOG_Priority_Med);
    return result;
    break;
  }
  strcat(str, "\r\n");

  uint8_t failCnt = 8;

  while ((failCnt-- > 0) && (result != FLOW_OK))
  {
    result = FLOW_SetMode_Command(&Flow, str, state);
    if ((result != FLOW_OK) && (failCnt > 0))
    {
      Log("Flow Controller FAIL", LOG_Priority_High);
      Log("Cycling Flow Control Power", LOG_Priority_High);
      FLOW_PowerOFF(&Flow.Power);
      FLOW_PowerON(&Flow.Power);
      Log("Retrying Flow Command", LOG_Priority_High);
    }
    else
    {
      Log(str, LOG_Priority_High);
    }
  }

  return result;
}

STATIC eFLOWStatus_t FLOW_SetMode_Command(sFLOW_t *Flow,
                                          char *str,
                                          eFLOWState_t state)
{
  eFLOWStatus_t result = FLOW_FAIL;
  uint8_t counter = 200;
  uint8_t len = 0;

  memset(readStr, 0, 128);

  /** Send the Command */
  if (MAX310X_OK != MAX310X_Send(&Flow->UART, str, strlen(str)))
  {
    Error(ERROR_MSG_00400008, LOG_Priority_High);
    return result;
  }

  if (state != FLOW_STATE_TEST_PUMP)
  {
    Flow->currentState = state;
  }

  _delay_ms(250);

  do
  {
    MAX310X_Read(&Flow->UART, readStr, &len, 128);
    if ((UART_NACK == readStr[len]) || (UART_ACK == readStr[len]))
    {
      counter = 0;
    }
  } while (counter-- > 0);

  if (readStr[len] == UART_ACK)
  {
    strcpy(str, "valves=");
    strncat(str, &readStr[1], 7);
    result = FLOW_OK;
  }
  else
  {
    Error(ERROR_MSG_00400010, LOG_Priority_High);
    char temp[32];
    sprintf(temp, "Retry attempted %u times", 200 - counter);
  }
  return result;
}

eFLOWStatus_t FLOW_GetMode(void)
{
  eFLOWStatus_t result = FLOW_FAIL;
  char str[32];
  memset(str, 0, 32);
  char sendstr[] = "MODE";
  uint8_t len = strlen(sendstr);

  if (MAX310X_OK != MAX310X_Send(&Flow.UART, sendstr, len))
  {
    Error(ERROR_MSG_00400020, LOG_Priority_Med);
    return result;
  }

  uint8_t counter = 5;
  do
  {
    if (MAX310X_OK == MAX310X_Read(&Flow.UART, str, &len, 32))
    {
      break;
    }
  } while (counter-- > 0);

  if (0 == strlen(str))
  {
    Error(ERROR_MSG_00400040, LOG_Priority_High);
    return result;
  }

  result = FLOW_OK;
  return result;
}

eFLOWStatus_t FLOW_SetPulse(uint16_t pulse)
{
  eFLOWStatus_t result = FLOW_FAIL;
  char sendstr[32];

  if (pulse > 1000)
  {

    return result;
  }

  uint8_t len = sprintf(sendstr, "pulse=%u\r\n", pulse);
  FLOW_Write(sendstr, len);

  return result;
}

int16_t FLOW_GetPulse(void)
{
  int16_t result = -1;
  char str[32];
  memset(str, 0, 32);
  char sendstr[] = "pulse\r\n";
  uint8_t len = strlen(sendstr);

  FLOW_Write(sendstr, 7);
  _delay_ms(30);
  FLOW_Read(str, &len);

  if (0 == strlen(str))
  {
    Error(ERROR_MSG_00400040, LOG_Priority_High);
    return result;
  }
  else
  {
    char *pStart;
    char *pEnd;
    pStart = strstr(str, "PULSE");

    if (pStart != NULL)
    {
      pEnd = strstr(pStart, "\r\n");
      if (pEnd != NULL)
      {
        char temp[10] = {0};
        strncpy(temp, pStart + 7, pEnd - pStart - 7);
        result = atoi(temp);
      }
    }
  }

  return result;
}

eFLOWStatus_t FLOW_SetCurrent(uint16_t current)
{
  eFLOWStatus_t result = FLOW_FAIL;
  char sendstr[32];

  if (current > 1000)
  {

    return result;
  }

  uint8_t len = sprintf(sendstr, "current=%u\r\n", current);
  FLOW_Write(sendstr, len);

  return result;
}

int16_t FLOW_GetCurrent(void)
{
  int16_t result = -1;
  char str[32];
  memset(str, 0, 32);
  char sendstr[] = "current\r\n";
  uint8_t len = strlen(sendstr);

  FLOW_Write(sendstr, strlen(sendstr));
  _delay_ms(30);
  FLOW_Read(str, &len);

  if (0 == strlen(str))
  {
    Error(ERROR_MSG_00400040, LOG_Priority_High);
    return result;
  }
  else
  {
    char *pStart;
    char *pEnd;
    pStart = strstr(str, "CURRENT");

    if (pStart != NULL)
    {
      pEnd = strstr(pStart, "\r\n");
      if (pEnd != NULL)
      {
        char temp[10] = {0};
        strncpy(temp, pStart + 9, pEnd - pStart - 9);
        result = atoi(temp);
      }
    }
  }

  return result;
}

void FLOW_Write(char *val, uint8_t len)
{

  if (MAX310X_OK != MAX310X_Send(&Flow.UART, val, len))
  {
    Error(ERROR_MSG_00400020, LOG_Priority_Med);
  }
}

void FLOW_Read(char *val, uint8_t *len)
{

  uint8_t counter = 5;
  do
  {
    if (MAX310X_OK == MAX310X_Read(&Flow.UART, val, len, 32))
    {
      break;
    }
  } while (counter-- > 0);
}

void FLOW_GetState(char *str)
{
  switch (Flow.currentState)
  {
  case FLOW_STATE_ZERO_PUMP_ON:
    strcpy(str, "ZPON");
    break;
  case FLOW_STATE_ZERO_PUMP_OFF:
    strcpy(str, "ZPOFF");
    break;
  case FLOW_STATE_ZERO_VENT:
    strcpy(str, "ZPVENT");
    break;
  case FLOW_STATE_ZERO_PUMP_POST:
    strcpy(str, "ZPPCAL");
    break;
  case FLOW_STATE_SPAN_PUMP_ON:
    strcpy(str, "SPON");
    break;
  case FLOW_STATE_SPAN_PUMP_OFF:
    strcpy(str, "SPOFF");
    break;
  case FLOW_STATE_SPAN_VENT:
    strcpy(str, "SPVENT");
    break;
  case FLOW_STATE_SPAN_PUMP_POST:
    strcpy(str, "SPPCAL");
    break;
  case FLOW_STATE_EQUIL_PUMP_ON:
    strcpy(str, "EPON");
    break;
  case FLOW_STATE_EQUIL_PUMP_OFF:
    strcpy(str, "EPOFF");
    break;
  case FLOW_STATE_EQUIL_VENT:
    strcpy(str, "EPVENT");
    break;
  case FLOW_STATE_EQUIL_PUMP_POST:
    strcpy(str, "EPPC");
    break;
  case FLOW_STATE_AIR_PUMP_ON:
    strcpy(str, "APON");
    break;
  case FLOW_STATE_AIR_PUMP_OFF:
    strcpy(str, "APOFF");
    break;
  case FLOW_STATE_AIR_VENT:
    strcpy(str, "APVENT");
    break;
  case FLOW_STATE_AIR_POST:
    strcpy(str, "APPOST");
    break;
  case FLOW_STATE_REST:
    strcpy(str, "REST");
    break;
  case FLOW_STATE_DEPLOY:
    strcpy(str, "DPLY");
    break;
  case FLOW_STATE_PRESSURIZE:
    strcpy(str, "PRES");
    break;
  case FLOW_STATE_PRG1:
    strcat(str, "PRG1");
    break;
  case FLOW_STATE_PRG2:
    strcat(str, "PRG2");
    break;
  case FLOW_STATE_PRG3:
    strcat(str, "PRG3");
    break;
  case FLOW_STATE_PRG4:
    strcat(str, "PRG4");
    break;
  case FLOW_STATE_PRG5:
    strcat(str, "PRG5");
    break;
  case FLOW_STATE_PRG6:
    strcat(str, "PRG6");
    break;
  case FLOW_STATE_PRG7:
    strcat(str, "PRG7");
    break;
  case FLOW_STATE_PRG8:
    strcat(str, "PRG8");
    break;
//  case FLOW_STATE_PURGE:
//    strcpy(str, "PURGE");
//    break;
//  case FLOW_STATE_CLEAR:
//    strcpy(str, "CLEAR");
//    break;
  default:
    break;
  }
  return;
}

void FLOW_Sleep(void)
{
  /** Power off the Flow Controller */
  FLOW_PowerOFF(&Flow.Power);
}

void FLOW_Wakeup(void)
{

  if (Flow.Power.state == false)
  {
    /** Power on the Flow Controller */
    Log("FLOW Power On", LOG_Priority_High);
    FLOW_PowerON(&Flow.Power);

    /** Wakeup the UART */
    MAX310X_Wakeup(&Flow.UART);
    _delay_ms(100);
  }
}

void FLOW_PumpToggle(void)
{
  FLOW_Wakeup();
  FLOW_SetMode(FLOW_STATE_TEST_PUMP);
}

void FLOW_PumpON(void)
{
  eFLOWState_t state;
  FLOW_Wakeup();
  state = FLOW_PumpStateChange(Flow.currentState, true);
  FLOW_SetMode(state);
}

void FLOW_PumpOFF(void)
{
  eFLOWState_t state;
  FLOW_Wakeup();
  state = FLOW_PumpStateChange(Flow.currentState, false);
  FLOW_SetMode(state);
}
/************************************************************************
 *					           Static Functions
 ************************************************************************/
/** @brief Flow Controller Startup
 *
 * Settings and initializations for the Flow Control Startup routine.
 * Sets the GPIO Pin, powers the Flow Controller, runs the Wakeup routine
 * and sets the baudrate.
 *
 *  @param *f Pointer to Flow Structure
 *
 *  @return result
 */
STATIC eFLOWStatus_t FLOW_Startup(sFLOW_t *f)
{
  eFLOWStatus_t result = FLOW_FAIL;

  /** Initialize the GPIO */
  BSP_GPIO_Init(&f->Power.pin);

  /** Wakeup the MAX310X (Acts like Open in this case) */
  MAX310X_Wakeup(&f->UART);

  /** Set Baudrate */
  if (MAX310X_OK != MAX310X_SetBaudrate(&f->UART, FLOW_UART_BAUDRATE))
  {
    Error(ERROR_MSG_00400001, LOG_Priority_Med);
    return result;
  }

  result = FLOW_OK;
  return result;
}

/** @brief Power ON
 *
 * Power the Flow Controller ON
 *
 *  @param *p Pointer to Power Pin
 *
 *  @return None
 */
STATIC void FLOW_PowerON(sFlowPowerState_t *p)
{
  BSP_Low_Current_Mode_OFF();
  BSP_GPIO_SetPin(&p->pin);
  BSP_Low_Current_Mode_OFF();
  p->state = true;
  AM08X5_GetTime(&p->time);
  _delay_ms(1000);
}

/** @brief Power OFF
 *
 * Power the Flow Controller OFF
 *
 *  @param *p Pointer to Power Pin
 *
 *  @return None
 */
STATIC void FLOW_PowerOFF(sFlowPowerState_t *p)
{
  BSP_GPIO_ClearPin(&p->pin);

  if (p->state == true)
  {
    Log("FLOW Power Off", LOG_Priority_High);
  }
  p->state = false;
  AM08X5_clear_time_struct(&p->time);
}

STATIC eFLOWState_t FLOW_PumpStateChange(eFLOWState_t state, bool pumpon)
{
  if (pumpon == true)
  {
    switch (state)
    {
    case FLOW_STATE_ZERO_PUMP_ON:
    case FLOW_STATE_ZERO_PUMP_OFF:
    case FLOW_STATE_ZERO_PUMP_POST:
      state = FLOW_STATE_ZERO_PUMP_ON;
      break;
    case FLOW_STATE_SPAN_PUMP_ON:
    case FLOW_STATE_SPAN_PUMP_OFF:
    case FLOW_STATE_SPAN_PUMP_POST:
      state = FLOW_STATE_SPAN_PUMP_ON;
      break;
    case FLOW_STATE_EQUIL_PUMP_ON:
    case FLOW_STATE_EQUIL_PUMP_OFF:
    case FLOW_STATE_EQUIL_PUMP_POST:
      state = FLOW_STATE_EQUIL_PUMP_ON;
      break;
    case FLOW_STATE_AIR_PUMP_ON:
    case FLOW_STATE_AIR_PUMP_OFF:
      state = FLOW_STATE_AIR_PUMP_ON;
      break;
    case FLOW_STATE_REST:
      state = FLOW_STATE_REST;
      break;
    case FLOW_STATE_DEPLOY:
      state = FLOW_STATE_DEPLOY;
      break;
    default:
      break;
    }
  }
  else
  {
    switch (state)
    {
    case FLOW_STATE_ZERO_PUMP_ON:
    case FLOW_STATE_ZERO_PUMP_OFF:
    case FLOW_STATE_ZERO_PUMP_POST:
      state = FLOW_STATE_ZERO_PUMP_OFF;
      break;
    case FLOW_STATE_SPAN_PUMP_ON:
    case FLOW_STATE_SPAN_PUMP_OFF:
    case FLOW_STATE_SPAN_PUMP_POST:
      state = FLOW_STATE_SPAN_PUMP_OFF;
      break;
    case FLOW_STATE_EQUIL_PUMP_ON:
    case FLOW_STATE_EQUIL_PUMP_OFF:
    case FLOW_STATE_EQUIL_PUMP_POST:
      state = FLOW_STATE_EQUIL_PUMP_OFF;
      break;
    case FLOW_STATE_AIR_PUMP_ON:
    case FLOW_STATE_AIR_PUMP_OFF:
      state = FLOW_STATE_AIR_PUMP_OFF;
      break;
    case FLOW_STATE_REST:
      state = FLOW_STATE_REST;
      break;
    case FLOW_STATE_DEPLOY:
      state = FLOW_STATE_DEPLOY;
      break;
    default:
      break;
    }
  }
  return state;
}
