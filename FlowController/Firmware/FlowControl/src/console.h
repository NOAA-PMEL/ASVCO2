#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <string.h>
#include <stdint.h>
#include "consoleio.h"

#define CONSOLE_PROMPT          ("> ")
#define PARAMETER_SEPARATER     (" ")
#define STR_ENDLINE             ("\r\n")

/*
 * @brief Console Pass/Fail error codes
 */
typedef enum {
  CONSOLE_OK,       /**< Console Pass */
  CONSOLE_FAIL      /**< Console Fail */
}eConsole_t;


/* @brief Console Initialization 
 * 
 * Initialize all functions relating to the console.
 *
 * @param None
 * 
 * @return None
 */
void CONSOLE_Init(void);

// called from lower down areas of the code (consoleCommands)
typedef enum { 
  CONSOLE_SUCCESS = 0u, 
	CONSOLE_PARAMETER_ERROR = 0x10u, 
	CONSOLE_PARAMETER_END   = 0x11u,
	CONSOLE_ERROR =0xFFu 
} eConsoleResult_T;


  
eConsoleResult_T ConsoleReceiveParamInt16(const char * buffer, const uint8_t parameterNumber, int16_t* parameterInt16);
eConsoleResult_T ConsoleReceiveParamUInt32(const char *buffer, const uint8_t parameterNumber, uint32_t *parameterUInt32);
eConsoleResult_T ConsoleReceiveParamUInt16(const char *buffer, const uint8_t parameterNumber, uint16_t *parameterUInt16);
eConsoleResult_T ConsoleReceiveParamString(const char *buffer, const uint8_t parameterNumber, char *parmeterStr);
eConsoleResult_T ConsoleSendParamInt16(int16_t parameterInt);
eConsoleResult_T ConsoleSendParamInt32(int32_t parameterInt);
eConsoleResult_T ConsoleReceiveParamHexUint16(const char * buffer, const uint8_t parameterNumber, uint16_t* parameterUint16);
eConsoleResult_T ConsoleSendParamHexUint16(uint16_t parameterUint16);
eConsoleResult_T ConsoleSendParamHexUint8(uint8_t parameterUint8);


#endif // _CONSOLE_H
