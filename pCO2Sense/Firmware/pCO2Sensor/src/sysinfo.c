/** @file sysinfo.c
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
#include "sysinfo.h"

/************************************************************************
 *					   STATIC Functions Prototypes
 ************************************************************************/

/************************************************************************
 *					        STATIC Variables
 ************************************************************************/
PERSISTENT sSystem_t sysinfo = {
    .ver = VERSION,
    .StartupCounter = 0,
    .BORCounter = 0,
    .PORCounter = 0,
    .RSTCounter = 0,
    .I2CFailures = 0,
    .gps = -2,
    .timer1 = {.tCounter = 0,
               .tFlag = false},
    .timer2 = {.tCounter = 0,
               .tFlag = false},
    .sampleTimer = {.tCounter = 0,
                    .tFlag = false},

};

PERSISTENT volatile char UART_Rx_Flag;
PERSISTENT volatile char SYS_MS_Flag;
PERSISTENT volatile eSysMode_t SysMode = SYS_Watchdog;
PERSISTENT volatile eConsoleMode_t ConsoleMode = SYS_Console_None;

/** Console Buffer */
PERSISTENT volatile char ConsoleRxBuffer[SYSTEM_CONSOLE_RX_BUFFER_LEN];
PERSISTENT volatile char *pConsoleRxBuffer = &ConsoleRxBuffer[0];
volatile uint8_t ConsoleTimeoutSuppress = false;

/** Command Buffer */
PERSISTENT volatile char CommandRxBuffer[SYSTEM_COMMAND_RX_BUFFER_LEN];
PERSISTENT volatile char *pCommandRxBuffer = &CommandRxBuffer[0];
volatile uint8_t CommandTimeoutSuppress = false;

/** Licor Buffer */
PERSISTENT volatile char LicorRxBuffer[LICOR_BUFFER_LEN];
PERSISTENT volatile char *pLicorRxBuffer = &LicorRxBuffer[0];
volatile bool LicorRxFlag = false;
volatile sUartPassthrough_t UartPassthrough = SYS_UART_BLOCK;

/** I2C B0 Transfer Data */
uint8_t I2C_B0_TxData[256];
uint8_t I2C_B0_TxCount;
uint8_t I2C_B0_RxData[256];
uint8_t I2C_B0_MaxCount;

/** I2C B1 Transfer Data */
uint8_t I2C_B1_TxData[16];
uint8_t I2C_B1_TxCount;
uint8_t I2C_B1_RxData[16];
uint8_t I2C_B1_MaxCount;

/** TELOS Iridium Buffer */
PERSISTENT char TELOS_IRID_BUF[SYS_IRIDIUM_BUFFER_LENGTH];
char *pTELOS_IRID_BUF;
uint16_t TELOS_IRID_BUF_Len;

/** Console Ctrl-C Coutner */
volatile uint8_t Console_CtrlC_Count = 0;