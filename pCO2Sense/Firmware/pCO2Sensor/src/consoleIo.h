/** @file consoleIo.h
 *  @brief Console IO 
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 24, 2018
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
#ifndef _CONSOLEIO_H
#define _CONSOLEIO_H

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
#define STATIC  static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent 
#endif
#endif

/************************************************************************
*			        STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
#include <stdio.h>
#ifndef UNIT_TESTING
#include <stdlib.h>
#endif

/************************************************************************
*			        HEADER FILES
************************************************************************/
#include "bsp.h"
#include "logging.h"

/************************************************************************
*			        Structs & Enums
************************************************************************/
/** 
 *  @enum CONSOLEIO Status
 *  @brief CONSOLEIO Status 
 */
typedef enum {
    CONSOLEIO_OK  = 0u,       /**< ConsoleIO passes */
    CONSOLEIO_FAIL = 1u       /**< ConsoleIO failure */
}eConsoleIoStatus_t;

/**
 * @enum Console Port 
 * @brief Console Port
 */

typedef enum {
    PORT_NONE    = 0x00u,       /**< Display on no ports */
    PORT_CONSOLE = 0x01u,       /**< Display on Console Port Only */
    PORT_COMMAND = 0x02u,       /**< Display on Command Port Only */
    PORT_BOTH    = 0x03u        /**< Display on both */
}eConsolePort_t;

/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief Init Command Port.
 *
 *  Initialize the Console and Command Ports.
 *
 *  @return None
 */
eConsoleIoStatus_t CONSOLEIO_Init(sUART_t *Console);


void CONSOLEIO_Close(sUART_t *Console);

/** @brief Console IO Receive
 *
 * Read Command from Console/Command Port into buffer
 * 
 * @param port Command or Console Port
 * @param buf Pointer to read string
 * @param bufLen Length of buffer
 * @param readLen Length of bytes read
 *
 *  @return None
 */
eConsoleIoStatus_t CONSOLEIO_Receive( sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *readLen);
/** @brief Console IO Send
 *
 * Write the  port.
 * 
 * @param *pUART Pointer to UART Struct to send over
 * @param buf Pointer to read string
 * @param bufLen Length of buffer
 * @param sendLen Length of bytes read
 *
 *  @return None
 */
eConsoleIoStatus_t CONSOLEIO_Send(sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *sendLen);

/** @brief Console IO Send String
 * 
 * Send string over console.
 * 
 * @param *pUART Pointer to UART Struct to send over
 * @param *buf String buffer to send
 * 
 * @return result
 */
eConsoleIoStatus_t CONSOLEIO_SendStr(sUART_t *pUART, const char *buf);
#endif // _CONSOLEIO_H
