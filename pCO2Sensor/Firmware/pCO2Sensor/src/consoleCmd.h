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
 *	@note
 *
 *  @bug  No known bugs
 */
#ifndef _CONSOLECMD_H
#define _CONSOLECMD_H

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
 *                       HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "logging.h"
#include "consoleIo.h"
#include "pco2.h"
#include "AM08x5.h"
#include "LI8x0.h"
#include "flowcontrol.h"
#include "max250.h"
#include "sysinfo.h"
#include "LED.h"
#include "sht35.h"

/************************************************************************
 *                               MACROS
 ************************************************************************/
#define CONSOLE_COMMAND_MAX_COMMAND_LENGTH 10 // command only
#define CONSOLE_COMMAND_MAX_LENGTH 256        // whole command with argument
#define CONSOLE_COMMAND_MAX_HELP_LENGTH 64    // if this is zero, there will be no  help (XXXOPT: RAM reduction)
#define CONSOLE_COMMAND_TABLE_END \
    {                             \
        NULL, NULL, HELP("")      \
    }

#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
#define HELP(x) (x)
#else
#define HELP(x) 0
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH

#define IGNORE_UNUSED_VARIABLE(x) \
    if (&x == &x)                 \
    {                             \
    }

#ifndef STR_ENDLINE
#define STR_ENDLINE ("\r\n")
#endif

#define CONSOLECMD_SENDSTR_LEN (1450)
#define CONSOLECMD_INDEX_LEN  102//(100)
/************************************************************************
 *                       Structs & Enums
 ************************************************************************/
/**
 *  @enum COMMAND Status
 *  @brief COMMAND Status
 */
typedef enum
{
    CONSOLECMD_OK = 0u,  /**< ConsoleIO passes */
    CONSOLECMD_FAIL = 1u /**< ConsoleIO failure */
} eConsoleCmdStatus_t;

/**
 * @enum Command Results
 *
 */
typedef eConsoleCmdStatus_t (*ConsoleCommand_t)(sUART_t *pUART, const char buffer[]);

/**
 * @struct Command Struct
 *
 */
typedef struct sConsoleCommandStruct
{
    const char *name;
    ConsoleCommand_t execute;
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
    char help[CONSOLE_COMMAND_MAX_HELP_LENGTH];
#else
    uint8_t junk;
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH
} sConsoleCommandTable_t;

typedef struct sConsoleHotkeyStruct
{
    const char *name;
    ConsoleCommand_t execute;
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
    char help[CONSOLE_COMMAND_MAX_HELP_LENGTH];
#else
    uint8_t junk;
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH
} sConsoleHotkeyTable_t;

#ifdef UNIT_TESTING
extern uint32_t consoleCommandTableSize;
extern const sConsoleCommandTable_t mConsoleCommandTable[CONSOLECMD_INDEX_LEN];
extern const sConsoleHotkeyTable_t mConsoleHotkeyTable[10];
#endif
/************************************************************************
 *					    Function Prototypes
 ************************************************************************/
/** @brief Get Console Command Table
 *
 *  Return the Console Command Table
 *
 *  @return Pointer to Command Table
 */
const sConsoleCommandTable_t *CONSOLECMD_GetTable(void);

/** @brief Get Console Hotkey Table
 *
 * Return the Console Hotkey Table
 *
 * @return Pointer to Hotkey Table
 */
const sConsoleHotkeyTable_t *CONSOLECMD_GetHotkeys(void);

#endif // _CONSOLECMD_H
