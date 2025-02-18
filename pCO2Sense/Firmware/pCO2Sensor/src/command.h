/** @file commandCmd.h
 *  @brief Command Commands
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 25, 2018
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
#ifndef _COMMAND_H
#define _COMMAND_H

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
#include <string.h>

#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "consoleIo.h"
#include "consoleCmd.h"
#include "bsp.h"
#include "buffer_c.h"
#include "datalogger.h"
/************************************************************************
 *							MACROS
 ************************************************************************/
#define COMMAND_TIMEOUT_SEC (180)
#define COMMAND_PROMPT ("> ")
#define PARAM_SEPARATER (' ')
#define STR_ENDLINE ("\r\n")
#ifndef STR_ACK
#define STR_ACK ("\x6")
#endif

#ifndef STR_NACK
#define STR_NACK ("\x15")
#endif

#define CHAR_CR ('\r')
#define CHAR_LF ('\n')
#define CHAR_NULL ('\0')
#define COMMAND_BUFFER_LENGTH (64)

#ifndef NOT_FOUND
#define NOT_FOUND (-1)
#endif
#ifndef IS_FOUND
#define IS_FOUND (1)
#endif

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum COMMAND Status
 *  @brief COMMAND Status
 */
typedef enum
{
    COMMAND_OK = 0u,  /**< CommandIO passes */
    COMMAND_FAIL = 1u /**< CommandIO failure */
} eCommandStatus_t;

/************************************************************************
 *							Externs
 ************************************************************************/
#ifdef UNIT_TESTING
extern sUART_t Command;
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
void COMMAND_Init(void);

void COMMAND_Close(void);

void COMMAND_TerminalMode(void);

/** @brief Process Command
 *
 * Process the command commands.  This function is called in a main loop.
 *
 * @param None
 *
 * @return None
 */
void COMMAND_Process(void);

/** @brief Send String over Command
 *
 * Send a string over the command.
 *
 * @param *str Pointer to send string
 *
 * @return None
 */
void COMMAND_puts(const char *str);

/** @brief Send String of specific length
 *
 * Sends a string of specified length over the command port
 *
 * @param *str Pointer to send string
 * @param len Length of string
 *
 * @return None
 */
void COMMAND_Send(char *str, uint16_t len);

/** @brief Send string of length over console
 *
 * Send a string of predetermined length over console
 *
 * @param *str Pointer to send string
 * @len Length of String to send
 *
 * @return
 */
void COMMAND_Send(char *str, uint16_t len);
uint32_t COMMAND_GetBaudrate(void);
eCommandStatus_t COMMAND_SetBaudrate(uint32_t baudrate);

/** @brief Put command port to sleep
 *
 * Puts the command port in low power mode
 *
 * @param None
 *
 * @return None
 */
void COMMAND_Sleep(void);

/** @brief Clear the command buffer
 *
 * Clears the command buffer
 *
 * @param None
 *
 * @return None
 */
void COMMAND_ClearBuffer(void);

#endif // _COMMAND_H