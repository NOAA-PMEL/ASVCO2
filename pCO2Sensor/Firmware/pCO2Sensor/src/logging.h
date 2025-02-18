/** @file logging.h
 *  @brief Logging Library
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date December 16, 2022
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note Logging library
 *
 *  @bug  No known bugs
 */
#ifndef _ERRORS_H
#define _ERRORS_H

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
#include <string.h>
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "errorlist.h"
#include "AM08x5.h"

/************************************************************************
 *							MACROS
 ************************************************************************/
#define INCLUDE_USB_IN_COMPILE  (true)

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum Log Priorities
 *  Log Priority to Descriminate Against
 */
typedef enum
{
  LOG_Priority_Low = 0x01, /**< Lowest Priority */
  LOG_Priority_Med = 0x02, /**< Medium Priority */
  LOG_Priority_High = 0x03 /**< Highest Priority */
} eLogPriority_t;

/** @enum Log Levels
 *  Logging levels (None, Debug, Error)
*/
typedef enum 
{
    LOG_LEVEL_None = 0x00, /**< No Logging (Quiet mode) */
    LOG_LEVEL_Debug = 0x01, /**< All Logging messages */
    LOG_LEVEL_Error = 0x08, /**< Log Errors only */
} eLogLevel_t;

/**
 *  @enum Log Port
 *  Port to Log To
 */
typedef enum
{
  LOG_PORT_None = 0x01,      /**< Don't write out any port */
  LOG_PORT_Console = 0x02,   /**< Console Port */
  LOG_PORT_Command = 0x04,   /**< Command Port */
//   LOG_PORT_Datalogger = 0x08 /**< Datalogger */
} eLogPort_t;

/**
 *  @struct Log Structure
 *  Structure containing configurations for log types
 */
typedef struct
{
  uint8_t port;
  eLogLevel_t console;
  eLogLevel_t command;
  bool quiet;
} sLog_t;

/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
void Log(const char *str, eLogPriority_t priority);
void Error(const char *str, eLogPriority_t priority);
void Error_Clear(void);
void Error_GetFlags(char *s);
void SetLogLevel(eLogLevel_t level);
eLogLevel_t GetLogLevel(char *level);
void SetLogQuiet(bool quiet);
bool GetLogQuiet(void);

void Data(const char *str);
void Coeff(const char *str);
void Stats(const char *str);
void Dry(const char *str);
void Sleep(void);
void Reset(void);
void Message(const char *str);


eLogPort_t GetLogPort(void);
void SetLogPorts(eLogPort_t port);



#endif // _ERRORS_H