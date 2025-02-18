/** @file commandIo.c
 * @brief Command IO
 *
 * @author Matt Casari, matthew.casari@noaa.gov
 * @date April 24, 2018
 * @version 1.0.0
 *
 * @copyright National Oceanic and Atmospheric Administration
 * @copyright Pacific Marine Environmental Lab
 * @copyright Engineering Development Division
 *
 *@note
 *
 * @bug  No known bugs
 */
#include "commandIo.h"

/************************************************************************
 *					         Functions
 ************************************************************************/
eCommandIoStatus_t COMMANDIO_Init(sUART_t *Command)
{
    eCommandIoStatus_t result = COMMANDIO_FAIL;

    if (BSP_OK != BSP_UART_Init(Command))
    {
        Error(ERROR_MSG_0010, LOG_Priority_High);
        return result;
    }

    BSP_UART_Enable(Command);
    BSP_UART_EnableInterrupt(Command, EUSCI_A_UART_RECEIVE_INTERRUPT);

    result = COMMANDIO_OK;
    return result;
}

eCommandIoStatus_t COMMANDIO_Disable(sUART_t *Command)
{
    eCommandIoStatus_t result = COMMANDIO_FAIL;

    BSP_UART_Disable(Command);

    BSP_UART_DisableInterrupt(Command, EUSCI_A_UART_RECEIVE_INTERRUPT);

    result = COMMANDIO_OK;
    return result;
}

eCommandIoStatus_t COMMANDIO_Enable(sUART_t *Command)
{
    eCommandIoStatus_t result = COMMANDIO_FAIL;
    BSP_UART_Enable(Command);
    BSP_UART_EnableInterrupt(Command, EUSCI_A_UART_RECEIVE_INTERRUPT);
    result = COMMANDIO_OK;
    return result;
}

eCommandIoStatus_t COMMANDIO_Receive(sCircularBufferC_t *pBuf, eCommandPort_t port, char *buf, uint16_t bufLen, uint16_t *readLen)
{
    eCommandIoStatus_t result = COMMANDIO_FAIL;

    /** Retrieve data */
    if (BUFFER_C_OK != BufferC_gets(pBuf, buf, bufLen))
    {
        if (PORT_COMMAND == port)
        {
            Log("Command Buffer Warning", LOG_Priority_Low);
        }
        else
        {
            Log("Console Buffer Warning", LOG_Priority_Low);
        }
        return result;
    }

    /** Return */
    result = COMMANDIO_OK;
    return result;
}

eCommandIoStatus_t COMMANDIO_Send(sUART_t *pUART, char *buf, uint16_t bufLen, uint16_t *sendLen)
{
    eCommandIoStatus_t result = COMMANDIO_FAIL;

    /** Send data */
    BSP_UART_puts(pUART, buf, bufLen);
    *sendLen = bufLen;

    /** Return */
    result = COMMANDIO_OK;
    return result;
}

eCommandIoStatus_t COMMANDIO_SendStr(sUART_t *pUART, char *buf)
{
    BSP_UART_puts(pUART, buf, strlen(buf));
    return COMMANDIO_OK;
}
