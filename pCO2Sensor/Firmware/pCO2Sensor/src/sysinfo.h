/** @file sysinfo.h
 *  @brief System Information
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date May 3, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _SYSINFO_H
#define _SYSINFO_H

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
#include <stdio.h>
#ifndef UNIT_TESTING

#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "sysconfig.h"
#include "version.h"

/************************************************************************
 *							Structs & Enums
 ************************************************************************/

typedef enum
{
  SYS_None = 0x0u,     /** Sleep Mode (Not actually used) */
  SYS_Console = 0x01u, /** Console Mode */
  SYS_Command = 0x02u, /** Command Mode */
  SYS_Sample = 0x04u,  /** Sample Routine */
  SYS_Watchdog = 0x08u /** Watchdog mode (Wakes every ~25 seconds to hit watchdog) */
} eSysMode_t;

typedef enum
{
  SYS_Console_None = 0x00u,     /** No Console Mode */
  SYS_Console_Terminal = 0x01u, /** Terminal Mode */
  SYS_Console_Hotkey = 0x02u    /** Hotkey Mode */
} eConsoleMode_t;

typedef enum
{
  SYS_Submode_Normal = 0x00u, /**< Normal Mode */
  SYS_Submode_Test = 0x01u,   /**< Test Mode */
} eConsoleSubmode_t;

typedef enum
{
  SYS_UART_BLOCK = 0x00u,                  /**< No Passthrough */
  SYS_UART_LICOR_PASSTHROUGH = 0x01u,      /**< Licor Passthrough mode */
  SYS_UART_FLOW_PASSTHROUGH = 0x02u,       /** Flow Controller Passthrough */
  SYS_UART_DATALOGGER_PASSTHROUGH = 0x04u, /**< Datalogger Passthrough Mode */

} sUartPassthrough_t;

typedef struct
{
  volatile int16_t tCounter;
  volatile bool tFlag;
} sSysTimer_t;

typedef struct
{
  char SerialNumber[20];     /**< System Serial Number */
  const char ver[32];        /**< Firmware version */
  uint32_t StartupCounter;   /**< Number of hard starts */
  uint32_t BORCounter;       /**< Number of brownout restarts */
  uint32_t RSTCounter;       /**< Hardware reset counter */
  uint32_t PORCounter;       /**< Poweron reset counter */
  uint32_t I2CFailures;      /**< Number of I2C Failure system restarts */
  sSysTimer_t timer1;        /**< 1 Second Timer (AM08X5 RTC) */
  sSysTimer_t timer2;        /**< 1 Second Timer (AM08X5 RTC) */
  sSysTimer_t sampleTimer;   /**< 1ms Sensor Sampling Timer (uC RTC) */
  const int32_t gps;         /**< GPS value (-2 for Datalogger) */
  eConsoleSubmode_t Submode; /**< Normal or Test Mode */
} sSystem_t;

/************************************************************************
 *				Externs
 ************************************************************************/
extern PERSISTENT sSystem_t sysinfo;

extern volatile char *pRxBufferMax;
extern volatile char UART_Rx_Flag;
extern volatile char SYS_MS_Flag;
extern volatile eSysMode_t SysMode;
extern volatile eConsoleMode_t ConsoleMode;

extern uint8_t I2C_B0_TxData[256];
extern uint8_t I2C_B0_TxCount;
extern uint8_t I2C_B0_RxData[256];
extern uint8_t I2C_B0_MaxCount;

extern uint8_t I2C_B1_TxData[16];
extern uint8_t I2C_B1_TxCount;
extern uint8_t I2C_B1_RxData[16];
extern uint8_t I2C_B1_MaxCount;

extern char TELOS_IRID_BUF[SYS_IRIDIUM_BUFFER_LENGTH];
extern char *pTELOS_IRID_BUF;
extern uint16_t TELOS_IRID_BUF_Len;

extern volatile char ConsoleRxBuffer[SYSTEM_CONSOLE_RX_BUFFER_LEN];
extern volatile char *pConsoleRxBuffer;
extern volatile uint8_t ConsoleTimeoutSuppress;

extern volatile char CommandRxBuffer[SYSTEM_COMMAND_RX_BUFFER_LEN];
extern volatile char *pCommandRxBuffer;
extern volatile uint8_t CommandTimeoutSuppress;

extern volatile char LicorRxBuffer[LICOR_BUFFER_LEN];
extern volatile char *pLicorRxBuffer;
extern volatile bool LicorRxFlag;

extern volatile sUartPassthrough_t UartPassthrough;

extern volatile uint8_t Console_CtrlC_Count;
#endif // _SYSINFO_H
