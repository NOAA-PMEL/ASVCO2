/** @file commandCmd.h
 *  @brief Command Commands
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date May 16, 2016
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
#ifndef _COMMANDCMD_H
#define _COMMANDCMD_H

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
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "logging.h"
#include "consoleIo.h"
#include "pco2.h"
#include "datalogger.h"
#include "sysinfo.h"
#include "AM08x5.h"

/************************************************************************
 *							MACROS
 ************************************************************************/
#define COMMAND_COMMAND_MAX_COMMAND_LENGTH 10 // command only
#define COMMAND_COMMAND_MAX_LENGTH 256        // whole command with argument
#define COMMAND_COMMAND_MAX_HELP_LENGTH 64    // if this is zero, there will be no  help (XXXOPT: RAM reduction)
#define COMMAND_COMMAND_TABLE_END \
    {                             \
        NULL, NULL, HELP("")      \
    }
#if COMMAND_COMMAND_MAX_HELP_LENGTH > 0
#define HELP(x) (x)
#else
#define HELP(x) 0
#endif // COMMAND_COMMAND_MAX_HELP_LENGTH
#define IGNORE_UNUSED_VARIABLE(x) \
    if (&x == &x)                 \
    {                             \
    }

#ifndef STR_ENDLINE
#define STR_ENDLINE ("\r\n")
#endif

#ifndef STR_ACK
#define STR_ACK ("\x6") //( (char *) 0x06 )
#endif

#ifndef STR_NACK
#define STR_NACK ("\x15") //( (char *) 0x15 )
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
    COMMANDCMD_OK = 0u,  /**< CommandIO passes */
    COMMANDCMD_FAIL = 1u /**< CommandIO failure */
} eCommandCmdStatus_t;

/**
 * @enum Command Results
 *
 */
typedef eCommandCmdStatus_t (*CommandCommand_t)(sUART_t *pUART, const char buffer[]);

/**
 * @struct Command Struct
 *
 */
typedef struct sCommandCommandStruct
{
    const char *name;
    CommandCommand_t execute;
#if COMMAND_COMMAND_MAX_HELP_LENGTH > 0
    char help[COMMAND_COMMAND_MAX_HELP_LENGTH];
#else
    uint8_t junk;
#endif // COMMAND_COMMAND_MAX_HELP_LENGTH
} sCommandCommandTable_t;

#ifdef UNIT_TESTING
extern uint32_t commandCommandTableSize;
extern const sCommandCommandTable_t mCommandCommandTable[50];
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
const sCommandCommandTable_t *COMMANDCMD_GetTable(void);

#endif // _COMMANDCMD_H