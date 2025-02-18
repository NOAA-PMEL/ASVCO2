/** @file command.c
 *  @brief Command
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 27, 2018
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
#include "command.h"
// #include "errors.h"
#include "logging.h"

/************************************************************************
 *					        Static Variables
 ************************************************************************/
STATIC PERSISTENT sUART_t Command = {
    .param = {.selectClockSource = SYS_COMMAND_UART_CLK,
              .parity = SYS_COMMAND_UART_PARITY,
              .msborLsbFirst = SYS_COMMAND_UART_MSB_FIRST,
              .numberofStopBits = SYS_COMMAND_UART_STOPBITS,
              .uartMode = SYS_COMMAND_UART_MODE},
    .baseAddress = SYS_COMMAND_UART_PORT,
    .baudrate = SYS_COMMAND_UART_BAUDRATE,
};
STATIC PERSISTENT sGPIO_t CMDPower = {.port = CMD_POWER_PORT,
                                      .pin = CMD_POWER_PIN,
                                      .mode = CMD_POWER_DIR,
                                      .inverted = CMD_POWER_POL};

STATIC PERSISTENT sGPIO_t CMDDetect = {
    .port = CMD_DETECT_PORT,
    .pin = CMD_DETECT_PIN,
    .mode = CMD_DETECT_DIR,
    .inverted = CMD_DETECT_POL};

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC uint8_t COMMAND_CommandMatch(const char *name, const char *buf);
/************************************************************************
 *					         Functions
 ************************************************************************/
void COMMAND_Process(void)
{
  const sConsoleCommandTable_t *commandTable;
  uint32_t cmdIdx = 0;
  int32_t found;
  eConsoleCmdStatus_t result;
  uint16_t namelen = 0;

  char *pbuf = (char *)&CommandRxBuffer[0];
  BSP_Hit_WD();
  /** Verify buffer has string in it */
  if (0u < strlen((char *)&CommandRxBuffer[0]))
  {
    found = NOT_FOUND;
    commandTable = CONSOLECMD_GetTable();

    while ((NULL != commandTable[cmdIdx].name) &&
           (NOT_FOUND == found) &&
           (cmdIdx < CONSOLECMD_INDEX_LEN))
    {
      BSP_Hit_WD();
      if (0 < COMMAND_CommandMatch(commandTable[cmdIdx].name, pbuf))
      {

        namelen = strlen(commandTable[cmdIdx].name);
        pbuf += namelen;
        result = commandTable[cmdIdx].execute(&Command, pbuf);
        if (CONSOLECMD_OK != result)
        {
          Error(ERROR_MSG_01010011, LOG_Priority_Low);
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
    CONSOLEIO_SendStr(&Command, (char *)CommandRxBuffer);
    CONSOLEIO_SendStr(&Command, "- Not Found\r\n");
  }

  COMMAND_ClearBuffer();

  CONSOLEIO_SendStr(&Command, STR_ENDLINE);
  _delay_ms(10);

  return;
}

void COMMAND_Init(void)
{
  SYS_COMMAND_PORT_CONFIG1();
  SYS_COMMAND_PORT_CONFIG0();
  PMM_unlockLPM5();

  /** Initialize the UART */
  UCA1IE = 0x00;
  UCA1IFG = 0x00;
  CONSOLEIO_Init(&Command);
}

void COMMAND_Close(void)
{
  CONSOLEIO_Close(&Command);

  /** This is a hack to deal with the USB port lock-up issue */
  /** By turning the port off and setting to output, the */
  /** USB chip does not lock up, allowing for the terminal */
  /** to remain in use.  The lock-up seems to be from the */
  /** Flow Control Module being power on and a voltage spike */
  /** on ground.  This will have to be dealt with */
  P2SEL1 &= ~(BIT5 | BIT6);
  P2SEL0 &= ~(BIT5 | BIT6);
  P2DIR |= (BIT5 | BIT6);
  P2OUT &= ~(BIT5 | BIT6);
  PMM_unlockLPM5();
}

void COMMAND_TerminalMode(void)
{
  /** Always starting in normal mode */
  sysinfo.Submode = SYS_Submode_Normal;

  /** Console mode into Terminal */
  ConsoleMode = SYS_Console_Terminal;

  /** Run the Terminal until the user requests it or there is a timeout */
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
  do
  {
    BSP_Hit_WD();
    /** Display the prompt */
    COMMAND_puts(sysinfo.SerialNumber);
    if (sysinfo.Submode == SYS_Submode_Test)
    {
      CONSOLEIO_SendStr(&Command, "[TESTMODE]");
    }
    CONSOLEIO_SendStr(&Command, CONSOLE_PROMPT);

    /** Wait for a command (or timeout) */
    uint16_t count = COMMAND_TIMEOUT_SEC;
    do
    {
      BSP_Hit_WD();
      _delay_ms(1000);
    } while ((strstr((char *)&CommandRxBuffer[0], "\r\n") == NULL) &&
             (--count > 0));
    if (count == 0)
    {
      break;
    }

    /** If timeout, return and go back to sleep */
    if (pConsoleRxBuffer == &CommandRxBuffer[0])
    {
      break;
    }

    /** Parse message */
    COMMAND_Process();

  } while (ConsoleMode == SYS_Console_Terminal);

  if (sysinfo.Submode == SYS_Submode_Test)
  {
    sysinfo.Submode = SYS_Submode_Normal;
  }

  COMMAND_puts("Exiting console\r\n");
}

void COMMAND_puts(const char *str)
{
  /** Send String over COMMAND */
  CONSOLEIO_SendStr(&Command, str);
}

void COMMAND_Send(char *str, uint16_t len)
{
  uint16_t temp;
  /** Send message of predetermined length */
  CONSOLEIO_Send(&Command, str, len, &temp);
}

eCommandStatus_t COMMAND_SetBaudrate(uint32_t baudrate)
{
  eCommandStatus_t result = COMMAND_FAIL;
  if (BSP_OK == BSP_UART_SetBaudrate(&Command, baudrate))
  {
    result = COMMAND_OK;
  }

  return result;
}

uint32_t COMMAND_GetBaudrate(void)
{
  return BSP_UART_GetBaudrate(&Command);
}

void COMMAND_ClearBuffer(void)
{
  memset((char *)&CommandRxBuffer[0], 0, SYSTEM_COMMAND_RX_BUFFER_LEN);
  pCommandRxBuffer = &CommandRxBuffer[0];
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
STATIC uint8_t COMMAND_CommandMatch(const char *name, const char *buf)
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
