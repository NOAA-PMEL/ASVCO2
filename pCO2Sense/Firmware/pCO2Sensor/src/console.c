/** @file console.c
 *  @brief Console
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 27, 2018
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
#include "console.h"
// #include "errors.h"
#include "logging.h"
/************************************************************************
 *					        Static Variables
 ************************************************************************/
STATIC PERSISTENT sUART_t Console = {
    .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
              .parity = SYS_CONSOLE_UART_PARITY,
              .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
              .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
              .uartMode = SYS_CONSOLE_UART_MODE},
    .baseAddress = SYS_CONSOLE_UART_PORT,
    .baudrate = SYS_CONSOLE_UART_BAUDRATE,
};

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC uint8_t CONSOLE_CommandMatch(const char *name, const char *buf);
/************************************************************************
 *					         Functions
 ************************************************************************/
void CONSOLE_Process(void)
{
  const sConsoleCommandTable_t *commandTable;
  uint32_t cmdIdx = 0;
  int32_t found;
  uint16_t namelen = 0;
  eConsoleCmdStatus_t result;

  char *pbuf = (char *)&ConsoleRxBuffer[0];

  /** Verify buffer has string in it */
  if (0u < strlen((char *)&ConsoleRxBuffer[0]))
  {
    found = NOT_FOUND;
    commandTable = CONSOLECMD_GetTable();

    while ((NULL != commandTable[cmdIdx].name) &&
           (NOT_FOUND == found) &&
           (cmdIdx < CONSOLECMD_INDEX_LEN))
    {
      BSP_Hit_WD();
      if (0 < CONSOLE_CommandMatch(commandTable[cmdIdx].name, pbuf))
      {
        namelen = strlen(commandTable[cmdIdx].name);
        pbuf += namelen;
        result = commandTable[cmdIdx].execute(&Console, pbuf);
        if (CONSOLECMD_OK != result)
        {
          Error(ERROR_MSG_01010011, LOG_Priority_Low);
          __delay_cycles(500);
        }
        found = IS_FOUND;
      }
      else
      {
        cmdIdx++;
      }
    }
  }

  if (found != IS_FOUND)
  {
    CONSOLEIO_SendStr(&Console, (char *)ConsoleRxBuffer);
    CONSOLEIO_SendStr(&Console, "- Not Found\r\n");
  }

  CONSOLE_ClearBuffer();

  CONSOLEIO_SendStr(&Console, STR_ENDLINE);
  CONSOLEIO_SendStr(&Console, STR_ENDLINE);

  return;
}

void CONSOLE_ProcessHotkey(void)
{
  const sConsoleHotkeyTable_t *commandTable;
  uint32_t cmdIdx = 0;
  int32_t found;
  eConsoleCmdStatus_t result;

  char *pbuf = (char *)&ConsoleRxBuffer[0];

  /** Verify buffer has string in it */
  if (0u < strlen((char *)&ConsoleRxBuffer[0]))
  {
    found = NOT_FOUND;
    commandTable = CONSOLECMD_GetHotkeys();

    while ((NULL != commandTable[cmdIdx].name) && (NOT_FOUND == found))
    {
      if (0 < CONSOLE_CommandMatch(commandTable[cmdIdx].name, pbuf))
      {
        result = commandTable[cmdIdx].execute(&Console, pbuf);
        if (CONSOLECMD_OK != result)
        {
          Error(ERROR_MSG_01010012, LOG_Priority_Low);
        }
        found = IS_FOUND;
      }
      else
      {
        cmdIdx++;
      }
    }
  }

  return;
}

void CONSOLE_Init(void)
{
  /** This is a hack to deal with the USB port lock-up issue */
  /** The Port is reset each time it is repowered */
  SYS_CONSOLE_PORT_CONFIG1();
  SYS_CONSOLE_PORT_CONFIG0();
  PMM_unlockLPM5();

  /** Initialize the UART */
  CONSOLEIO_Init(&Console);
}

void CONSOLE_Close(void)
{
  CONSOLEIO_Close(&Console);

  /** This is a hack to deal with the USB port lock-up issue */
  /** By turning the port off and setting to output, the */
  /** USB chip does not lock up, allowing for the terminal */
  /** to remain in use.  The lock-up seems to be from the */
  /** Flow Control Module being power on and a voltage spike */
  /** on ground.  This will have to be dealt with */
  P2SEL1 &= ~(BIT0 | BIT1);
  P2SEL0 &= ~(BIT0 | BIT1);
  P2DIR |= (BIT0 | BIT1);
  P2OUT &= ~(BIT0 | BIT1);
  PMM_unlockLPM5();
}

void CONSOLE_TerminalMode(void)
{
  /** Always starting in normal mode */
  sysinfo.Submode = SYS_Submode_Normal;

  /** Run the Terminal until the user requests it or there is a timeout */
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
  do
  {
    BSP_Hit_WD();
    /** Display the prompt */
    CONSOLE_puts(sysinfo.SerialNumber);
    if (sysinfo.Submode == SYS_Submode_Test)
    {
      CONSOLEIO_SendStr(&Console, "[TESTMODE]");
    }
    CONSOLEIO_SendStr(&Console, CONSOLE_PROMPT);

    /** Wait for a command (or timeout) */
    uint16_t count = 3000;
    do
    {
      BSP_Hit_WD();
      _delay_ms(100);
    } while ((strstr((char *)&ConsoleRxBuffer[0], "\r\n") == NULL) &&
             (count-- > 0));

    /** If timeout, return and go back to sleep */
    if (pConsoleRxBuffer == &ConsoleRxBuffer[0])
    {
      break;
    }

    /** Parse message */
    CONSOLE_Process();

  } while (ConsoleMode == SYS_Console_Terminal);

  if (sysinfo.Submode == SYS_Submode_Test)
  {
    sysinfo.Submode = SYS_Submode_Normal;
  }

  CONSOLEIO_SendStr(&Console, "Exiting console\r\n");
}

void CONSOLE_puts(const char *str)
{
  /** Send String over Console */
  CONSOLEIO_SendStr(&Console, str);
}

void CONSOLE_Send(char *str, uint16_t len)
{
  uint16_t temp;
  /** Send message of predetermined length */
  CONSOLEIO_Send(&Console, str, len, &temp);
}

void CONSOLE_ClearBuffer(void)
{
  memset((char *)&ConsoleRxBuffer[0], 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
}
/************************************************************************
 *					   Static Functions
 ************************************************************************/
/** @brief Command Match
 *
 * Run command through command table to match command with function
 *
 * @param *name Command Table Function Names
 * @param *buf Receive buffer
 *
 * @return result (False if No match, True if match)
 */
STATIC uint8_t CONSOLE_CommandMatch(const char *name, const char *buf)
{
  uint32_t i = 0;
  uint32_t result = false;

  if (buf[i] == name[i])
  {
    result = true;
    i++;
  }

  while ((1u == result) &&
         (i < CONSOLE_COMMAND_MAX_COMMAND_LENGTH) &&
         (buf[i] != PARAM_SEPARATER) &&
         (buf[i] != CHAR_CR) &&
         (buf[i] != CHAR_LF) &&
         (buf[i] != CHAR_NULL) &&
         (buf[i] != '='))
  {
    if (buf[i] != name[i])
    {
      result = false;
    }
    i++;
  }

  if ((i) != strlen(name))
  {
    result = false;
  }

  buf = &buf[i];

  return result;
}
