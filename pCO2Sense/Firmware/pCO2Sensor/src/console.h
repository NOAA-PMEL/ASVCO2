/** @file consoleCmd.h
 *  @brief Console Commands
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 25, 2018
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
#ifndef _CONSOLE_H
#define _CONSOLE_H

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

/************************************************************************
*							MACROS
************************************************************************/
#define CONSOLE_PROMPT ("> ")
#define PARAM_SEPARATER (' ')
#define STR_ENDLINE ("\r\n")
#define CHAR_CR ('\r')
#define CHAR_LF ('\n')
#define CHAR_NULL ('\0')
#define CONSOLE_BUFFER_LENGTH (64)
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
 *  @enum CONSOLE Status
 *  @brief CONSOLE Status 
 */
typedef enum
{
    CONSOLE_OK = 0u,  /**< ConsoleIO passes */
    CONSOLE_FAIL = 1u /**< ConsoleIO failure */
} eConsoleStatus_t;

/************************************************************************
*							Externs
************************************************************************/
#ifdef UNIT_TESTING
extern sUART_t Console;
extern sUART_t Command;
#endif
/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief Init Console Port.
 *
 *  Initialize the Console and Command Ports.
 *
 *  @return None
 */
void CONSOLE_Init(void);

/** @brief Close Console
 *
 * Close the Console
 *
 * @param None
 * 
 * @return None
 */
void CONSOLE_Close(void);

/** @brief Process Console
 * 
 * Process the console commands.  This function is called in a main loop.
 * 
 * @param None
 * 
 * @return None
 */
void CONSOLE_Process(void);

/** @brief Processing Hotkey Entry
 *
 * Parse and process user input hotkey
 *
 * @param None
 * 
 * @return None
 */
void CONSOLE_ProcessHotkey(void);

/** @brief Terminal Mode
 * 
 * Process Terminal Mode
 * 
 * @param None
 * 
 * @return None
 */
void CONSOLE_TerminalMode(void);

/** @brief Send String over Console
 * 
 * Send a string over the console.
 * 
 * @param *str Pointer to send string
 * 
 * @return None
 */
void CONSOLE_puts(const char *str);

/** @brief Send string of length over console
 * 
 * Send a string of predetermined length over console
 *
 * @param *str Pointer to send string
 * @len Length of String to send
 * 
 * @return 
 */
void CONSOLE_Send(char *str, uint16_t len);

/** @brief Clear Console Buffer
 * 
 * Clear the Console Buffer
 *
 * @param None
 * 
 * @return None
 */
void CONSOLE_ClearBuffer(void);

#endif // _CONSOLE_H
