/** @file logging.c
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

#include "logging.h"
#include "command.h"
/************************************************************************
 *					        Static Variables
 ************************************************************************/
STATIC PERSISTENT sLog_t logs = {
    .port = (LOG_PORT_Console | LOG_PORT_Command),
    .console = LOG_LEVEL_Debug,
    .command = LOG_LEVEL_Error,
    .quiet = false,

};

STATIC uint16_t ErrorCodes[8];
/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
void _sendLog(const char *str);
void _sendMsg(const char *prefix, const char *str);
void _sendStr(const char *str);
static void _parseErrors(char *str);
/************************************************************************
 *					         Functions
 ************************************************************************/
void Log(const char *str, eLogPriority_t priority)
{
   
    char prefix[80] = "LOG: ";
    char time_str[30];
    AM08X5_GetTimestamp_ISO8601(time_str);
    strcat(prefix, time_str);
    strcat(prefix, " ");
    _sendLog(prefix);
    _sendLog(str);
    _sendLog("\r\n");


}

void Error(const char *str, eLogPriority_t priority)
{
  char outstr[80];
  char *pOutStr = &outstr[0];
  memset(outstr, 0, 80);
  strcpy(outstr, str);
  _sendMsg("ERR", outstr);
  char tempstr[8];
  strncpy(tempstr, str, 8);
  _parseErrors(tempstr);
}

void Error_Clear(void)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    ErrorCodes[i] = 0;
  }
}

void Error_GetFlags(char *s)
{
  char str[80] = "FLAGS:";
  char temp[9];
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    sprintf(temp, " %04x", ErrorCodes[i]);
    strcat(str, temp);
    strcat(s, temp);
  }

  strcat(str, "\r\n");

  _sendStr(str);
}

void Data(const char *str)
{
    if(logs.command >= LOG_LEVEL_None)
    {
        COMMAND_Init();
        _delay_ms(10);
    }
    _sendMsg("DATA", str);
}

void Coeff(const char *str)
{
    char *pTemp = (char*) str;
    if (strstr(pTemp, "\r\n") != NULL)
    {
      char *token = strtok(pTemp, "\r\n");
      while (token != NULL)
      {
        _sendMsg("COEFF", token);
        token = strtok(NULL, "\r\n");
      }
    }
}

void Stats(const char *str)
{
    _sendMsg("STATS", str);
}

void Dry(const char *str)
{
    _sendMsg("DRY", str);
}

void Sleep(void)
{
    if(logs.command == LOG_LEVEL_Debug)
    {
        _sendStr("SLEEP: Entering Sleep Mode\r\n");
    } else if(logs.command == LOG_LEVEL_Error){
        _sendStr("SLEEP\r\n");
    }
}

void Reset(void)
{
    if(logs.command == LOG_LEVEL_Debug)
    {
        _sendStr("RESET: System Reset Commanded\r\n");
    } else if(logs.command == LOG_LEVEL_Error) {
        _sendStr("SYSTEM_Reset\r\n");
    }
    _delay_ms(10);
}

void Message(const char *str)
{
//    _sendStr(*str);
    CONSOLE_puts(str);
    if(logs.quiet != true)
    {
      COMMAND_puts(str);
    }
}

void SetLogLevel(eLogLevel_t level)
{
    if( (level <= LOG_LEVEL_Error) )
    {
      logs.command = level;
    }
}

eLogLevel_t GetLogLevel(char *level)
{
  switch(logs.command) {
    case LOG_LEVEL_None:
    strcpy(level, "none");
    break;
    case LOG_LEVEL_Debug:
    strcpy(level, "debug");
    break;
    case LOG_LEVEL_Error:
    strcpy(level, "error");
    break;
    default:
    break;
  }
  
    return logs.command;
}


eLogPort_t GetLogPort(void)
{
  return (eLogPort_t)logs.port;
}

void SetLogPorts(eLogPort_t port)
{
  if ((port | LOG_PORT_None) ||
      (port | LOG_PORT_Console) ||
      (port | LOG_PORT_Command) )
  {
    logs.port = port;
  }
}

void SetLogQuiet(bool quiet)
{
  logs.quiet = quiet;
}

bool GetLogQuiet(void)
{
  return logs.quiet;
}

/************************************************************************
 *					   Static Functions
 ************************************************************************/
static void _sendLog(const char *str)
{
    CONSOLE_puts(str);

    if( 
      (logs.port & LOG_PORT_Command) && 
      (logs.command == LOG_LEVEL_Debug) &&
      (logs.quiet != true) 
      )
    {
        COMMAND_puts(str);
    }
}


static void _sendMsg(const char *prefix, const char *str) {
    CONSOLE_puts(prefix);
    CONSOLE_puts(": ");
    CONSOLE_puts(str);
    CONSOLE_puts("\r\n");

    if(logs.quiet != true)
      {
        COMMAND_puts(prefix);
        COMMAND_puts(": ");
        COMMAND_puts(str);
        COMMAND_puts("\r\n");
    }
}

static void _sendStr(const char *str)
{
    CONSOLE_puts(str);
    if(logs.quiet != true)
    {
      COMMAND_puts(str);
    }
}


static void _parseErrors(char *str)
{

  /** Convert last 4 to digits */
  uint16_t value = 0;
  uint8_t i;
  for (i = 0; i < 4; i++)
  {
    value |= (str[i + 4] - '0') << (4 * (3 - i));
  }

  /** Sort into correct error code flags */
  if (strncmp(str, "00", 2) == 0)
  {
    if (strncmp(&str[2], "01", 2) == 0)
    {
      ErrorCodes[0] |= value;
    }
    if (strncmp(&str[2], "02", 2) == 0)
    {
      ErrorCodes[1] |= value;
    }
    if (strncmp(&str[2], "04", 2) == 0)
    {
      ErrorCodes[2] |= value;
    }
    if (strncmp(&str[2], "08", 2) == 0)
    {
      ErrorCodes[3] |= value;
    }
    if (strncmp(&str[2], "10", 2) == 0)
    {
      ErrorCodes[4] |= value;
    }
    if (strncmp(&str[2], "20", 2) == 0)
    {
      ErrorCodes[5] |= value;
    }
    if (strncmp(&str[2], "40", 2) == 0)
    {
      ErrorCodes[6] = value;
    }
    if (strncmp(&str[2], "80", 2) == 0)
    {
      ErrorCodes[7] = value;
    }
  }
  return;
}