/** @file commandIo.h
 *  @brief Command IO
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
#ifndef _COMMANDIO_H
#define _COMMANDIO_H

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
#include <stdlib.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "logging.h"
/************************************************************************
 *							MACROS
 ************************************************************************/

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum COMMAND Status
 *  @brief COMMAND Status
 */
typedef enum
{
    COMMANDIO_OK = 0u,  /**< CommandIO passes */
    COMMANDIO_FAIL = 1u /**< CommandIO failure */
} eCommandIoStatus_t;

/**
 * @enum Command Port
 *
 */
// typedef enum {
//     PORT_NONE    = 0x00u,
//     PORT_CONSOLE = 0x01u,
//     PORT_COMMAND = 0x02u,
//     PORT_BOTH    = 0x03u
// }eCommandPort_t;

/************************************************************************
 *					    Externs
 ************************************************************************/
#ifdef UNIT_TESTING

#endif
/************************************************************************
 *					    Function Prototypes
 ************************************************************************/
/** @brief Init Command Port.
 *
 *  Initialize the Command and Command Ports.
 *
 *  @return None
 */
eCommandIoStatus_t COMMANDIO_Init(sUART_t *Command);

/** @brief Disable Command Port
 *
 * Disable the Command and/or Console port for low power mode
 *
 * @param *Command Port to disable
 *
 * @return result
 */
eCommandIoStatus_t COMMANDIO_Disable(sUART_t *Command);

/** @brief Enable Command Port
 *
 * Enable the Command and/or Console port and interrupts
 *
 * @param *Command Port to disable
 *
 * @return result
 */
eCommandIoStatus_t COMMANDIO_Enable(sUART_t *Command);

/** @brief Command IO Receive
 *
 * Read Command from Command/Command Port into buffer
 *
 * @param port Command or Command Port
 * @param buf Pointer to read string
 * @param bufLen Length of buffer
 * @param readLen Length of bytes read
 *
 *  @return None
 */
eCommandIoStatus_t COMMANDIO_Receive(sCircularBufferC_t *pBuf, eCommandPort_t port, char *buf, uint16_t bufLen, uint16_t *readLen);

/** @brief Command IO Send
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
eCommandIoStatus_t COMMANDIO_Send(sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *sendLen);

/** @brief Command IO Send String
 *
 * Send string over command.
 *
 * @param *pUART Pointer to UART Struct to send over
 * @param *buf String buffer to send
 *
 * @return result
 */
eCommandIoStatus_t COMMANDIO_SendStr(sUART_t *pUART, char *buf);
#endif // _COMMANDIO_H