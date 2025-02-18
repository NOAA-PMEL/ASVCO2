/** @file consoleIo.c
 *  @brief Console IO
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 24, 2018
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
#include "consoleIo.h"

/************************************************************************
 *					         Functions
 ************************************************************************/
eConsoleIoStatus_t CONSOLEIO_Init(sUART_t *Console)
{
  eConsoleIoStatus_t result = CONSOLEIO_FAIL;

  if (BSP_OK != BSP_UART_Init(Console))
  {
    Error(ERROR_MSG_01010008, LOG_Priority_High);
    return result;
  }

  BSP_UART_Enable(Console);
  BSP_UART_EnableInterrupt(Console, EUSCI_A_UART_RECEIVE_INTERRUPT);
  result = CONSOLEIO_OK;
  return result;
}

void CONSOLEIO_Close(sUART_t *Console)
{
  BSP_UART_Disable(Console);
  BSP_UART_DisableInterrupt(Console, 0xff);
}

eConsoleIoStatus_t CONSOLEIO_Receive(sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *readLen)
{
  eConsoleIoStatus_t result = CONSOLEIO_FAIL;

  /** Search Buffer for string */
  if (pConsoleRxBuffer != &ConsoleRxBuffer[0])
  {
    if (strchr((const char *)ConsoleRxBuffer, '\r') > 0)
    {
      *readLen = (uint16_t)(pConsoleRxBuffer - &ConsoleRxBuffer[0] - 2);
      strncpy(buf, (const char *)ConsoleRxBuffer, *readLen);
    }
  }
  else if (pCommandRxBuffer != &CommandRxBuffer[0])
  {
    if (strchr((const char *)CommandRxBuffer, '\r') != NULL)
    {
      *readLen = (uint16_t)(pCommandRxBuffer - &CommandRxBuffer[0] - 2);
      strncpy(buf, (const char *)CommandRxBuffer, *readLen);
    }
  }
  else
  {
    *readLen = 0;
  }

  /** Return */
  result = CONSOLEIO_OK;
  return result;
}

eConsoleIoStatus_t CONSOLEIO_Send(sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *sendLen)
{
  eConsoleIoStatus_t result = CONSOLEIO_FAIL;

  /** Send data */
  BSP_UART_puts(pUART, buf, bufLen);
  *sendLen = bufLen;

  /** Return */
  result = CONSOLEIO_OK;
  return result;
}

eConsoleIoStatus_t CONSOLEIO_SendStr(sUART_t *pUART, const char *buf)
{
  BSP_UART_puts(pUART, buf, strlen(buf));
  return CONSOLEIO_OK;
}