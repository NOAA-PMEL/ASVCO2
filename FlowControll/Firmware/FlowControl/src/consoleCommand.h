#ifndef _CONSOLECOMMAND_H
#define _CONSOLECOMMAND_H

#include <string.h>
#include <stdint.h>
#include "bsp.h"
// #include "console.h"
#include "consoleio.h"
#include "command.h"
#include "version.h"


#define CONSOLE_COMMAND_MAX_CMD_LEN     (10)
#define CONSOLE_COMMAND_MAX_LEN         (256)
#define CONSOLE_COMMAND_HELP_MAX_LEN    (64)

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

#ifndef SIZE_OF
#define SIZE_OF(x)      (sizeof(x)/sizeof(x[0]))
#endif 

#if CONSOLE_COMMAND_HELP_MAX_LEN > 0
    #define HELP(x) (x)
#else
    #define HELP(x) (0)
#endif




/*
 * @brief Console Command result status
 */
typedef enum { 
	COMMAND_SUCCESS = 0u, 
	COMMAND_PARAMETER_ERROR = 0x10u, 
	COMMAND_PARAMETER_END   = 0x11u,
	COMMAND_ERROR =0xFFu 
} eCommandResult_t;

typedef eCommandResult_t(*ConsoleCommand_T)(const char buffer[]);

typedef struct sConsoleCommandStruct
{
  char *name;
  ConsoleCommand_T execute;
#if CONSOLE_COMMAND_HELP_MAX_LEN > 0
	char *help[CONSOLE_COMMAND_HELP_MAX_LEN];
#else
	uint8_t junk;
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH 
} sConsoleCommandTable_t;


#define CONSOLE_COMMAND_TABLE_END {"",NULL,HELP("")}

// const sConsoleCommandTable_t *ConsoleCommandGetTable(void);
#ifdef UNIT_TESTING
extern const sConsoleCommandTable_t mConsoleCommandTable[] ;
extern size_t command_count;
#endif

void SendMaddieFunction(void);
#endif // _CONSOLECOMMAND_H
