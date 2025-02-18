/** @file command.c
 *  @brief Command function file
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date December 18th, 2017
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

#include "command.h"

/************************************************************************
*			                Static Functions Prototypes
************************************************************************/
STATIC void write_current_state(void);

/************************************************************************
*			                    Variables (Global)
************************************************************************/
PERSISTENT sUART_t command = {
    .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
              .parity = SYS_CONSOLE_UART_PARITY,
              .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
              .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
              .uartMode = SYS_CONSOLE_UART_MODE},
    .baseAddress = SYS_COMMAND_UART_PORT,
    .baudrate = SYS_COMMAND_UART_BAUDRATE,
};

/************************************************************************
*					             Functions
************************************************************************/
void COMMAND_Init(void)
{
    BSP_UART_Init(&command);
    BSP_UART_Enable(&command);
    BSP_UART_EnableInterrupt(&command, EUSCI_A_UART_RECEIVE_INTERRUPT);
}

void COMMAND_Start(void)
{
    RELAY_Off();
    _delay_ms(100);
}

eCommandStatus_t COMMAND_ParseMsg(void)
{
    char buf[80];
    char temp[80];
    char num[16];
    uint8_t i;
    uint8_t sendStrFlag = false;
    eBufferCStatus_t bufStat;
    eCommandStatus_t result = COMMAND_FAIL;

    /** Clear the buffer */
    memset(buf, 0, 80);
    memset(temp, 0, 80);
    memset(num, 0, 16);

    /** Copy String to working buffer */
    bufStat = BufferC_gets(&command.buf, &buf[0], 80);

    if (bufStat == BUFFER_C_OK)
    {
        if (strncmp("mode=", buf, 5) == 0)
        {
            if (COMMAND_SetMode(&buf[5]) == COMMAND_OK)
            {
                result = COMMAND_OK;
            }
        }
        else if (strncmp("vac=", buf, 4) == 0) 
        {
            strncpy(temp, &buf[4], 1);
            MCONTROL_SetValve(atoi(temp), 1);
            result = COMMAND_OK;
        }
        else if (strncmp("vbc=", buf, 4) == 0) {
            strncpy(temp, &buf[4], 1);
            MCONTROL_SetValve(atoi(temp), 0);
            result = COMMAND_OK;
        }
        else if (strncmp("pulse=", buf, 6) == 0)
        {
            /** Strip the number to convert */
            for (i = 0; i < 16; i++)
            {
                if ((buf[i + 6] == '\r') || (buf[i + 6] == '\n'))
                {
                    break;
                }
            }
            strncpy(temp, &buf[6], i);

            if (COMMAND_SetPulse(atoi(temp)) == COMMAND_OK)
            {
                result = COMMAND_OK;
            }
        }
        else if (strncmp("current=", buf, 8) == 0)
        {
            strncpy(temp, &buf[8], 1);
            if (COMMAND_SetCurrent(atoi(temp)) == COMMAND_OK)
            {
                result = COMMAND_OK;
            }
        } else if (strncmp("panic", buf, 5) == 0){
            MCONTROL_Panic();
            result = COMMAND_OK;
        }
        else if (strncmp("mode", buf, 4) == 0)
        {
            strcat(temp, "MODE: ");
            strcat(temp, COMMAND_GetMode());
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
        else if (strncmp("current", buf, 7) == 0)
        {
            strcat(temp, "CURRENT: ");
            sprintf(num, "%1u", COMMAND_GetCurrent());
            strcat(temp, num);
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
        else if (strncmp("pulse", buf, 5) == 0)
        {
            strcat(temp, "PULSE: ");
            sprintf(num, "%lu", COMMAND_GetPulse());
            strcat(temp, num);
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
        else if (strncmp("ver", buf, 3) == 0)
        {
            strcat(temp, "VERSION: ");
            strcat(temp, COMMAND_GetVersion());
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
        else if (strncmp("ser", buf, 3) == 0)
        {
            strcat(temp, "SERIAL: ");
            strcat(temp, COMMAND_GetSerial());
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
        else if (strncmp("count", buf, 3) == 0)
        {
            strcat(temp, "COUNT: ");
            sprintf(num, "%lu", COMMAND_GetCount());
            strcat(temp, num);
            strcat(temp, UART_ENDLINE);
            sendStrFlag = true;
            result = COMMAND_OK;
        }
       
    }

    if (result == COMMAND_OK)
    {
        /** Send ACK */
        BSP_UART_putc(&command, UART_ACK);
    }
    else
    {
        /** Send NACK */
        BSP_UART_putc(&command, UART_NACK);
    }

    if (sendStrFlag == true)
    {
        BSP_UART_puts(&command, &temp[0], strlen(temp));
    }
    write_current_state();

    BufferC_Clear(&command.buf);
    return result;
}

eCommandStatus_t COMMAND_SetMode(char *mode)
{
    eCommandStatus_t result = COMMAND_FAIL;

    ePCO2State_t state; 

    if (strncmp("ZPON", mode, 4) == 0)
    {
        state = PCO2_ZERO_PUMP_ON;
    }
    else if (strncmp("ZPOFF", mode, 5) == 0)
    {
        state = PCO2_ZERO_PUMP_OFF;
    }
    else if (strncmp("ZPPCAL", mode, 6) == 0)
    {
        state = PCO2_ZERO_CAL;
    }
    else if (strncmp("ZPVENT", mode, 6) == 0)
    {
        state = PCO2_ZERO_VENT;
    }
    else if (strncmp("SPON", mode, 4) == 0)
    {
        state = PCO2_SPAN_PUMP_ON;
    }
    else if (strncmp("SPOFF", mode, 5) == 0)
    {
        state = PCO2_SPAN_PUMP_OFF;
    }
    else if (strncmp("SPVENT", mode, 6) == 0)
    {
        state = PCO2_SPAN_VENT;
    }
    else if (strncmp("SPPC", mode, 4) == 0)
    {
        state = PCO2_SPAN_CAL;
    }
    else if (strncmp("EPON", mode, 4) == 0)
    {
        state = PCO2_EQUIL_PUMP_ON;
    }
    else if (strncmp("EPOFF", mode, 5) == 0)
    {
        state = PCO2_EQUIL_PUMP_OFF;
    }
    else if (strncmp("EPVENT", mode, 6) == 0)
    {
        state = PCO2_EQUIL_VENT;
    }
    else if (strncmp("EPPOST", mode, 6) == 0)
    {
        state = PCO2_EQUIL_POST;
    }
    else if (strncmp("APON", mode, 4) == 0)
    {
        state = PCO2_AIR_PUMP_ON;
    }
    else if (strncmp("APOFF", mode, 5) == 0)
    {
        state = PCO2_AIR_PUMP_OFF;
    }
    else if (strncmp("APVENT", mode, 6) == 0)
    {
        state = PCO2_AIR_VENT;
    }
    else if (strncmp("APPOST", mode, 6) == 0)
    {
        state = PCO2_AIR_POST;
    }
    else if (strncmp("REST", mode, 4) == 0)
    {
        state = PCO2_REST;
    }
    else if (strncmp("DEPLOY", mode, 6) == 0)
    {
        state = PCO2_DEPLOYMENT;
    }
    else if (strncmp("PRES", mode, 4) == 0)
    {
        state = PCO2_PRESSURIZE;
    }
    else if (strncmp("PRG1", mode, 4) == 0)
    {
        state = PCO2_PRG1;
    }
    
    else if (strncmp("PRG2", mode, 4) == 0)
    {
        state = PCO2_PRG2;
    }
    
    else if (strncmp("PRG3", mode, 4) == 0)
    {
        state = PCO2_PRG3;
    }
    
    else if (strncmp("PRG4", mode, 4) == 0)
    {
        state = PCO2_PRG4;
    }
    
    else if (strncmp("PRG5", mode, 4) == 0)
    {
        state = PCO2_PRG5;
    }
    
    else if (strncmp("PRG6", mode, 4) == 0)
    {
        state = PCO2_PRG6;
    }
    
    else if (strncmp("PRG7", mode, 4) == 0)
    {
        state = PCO2_PRG7;
    }
    
    else if (strncmp("PRG8", mode, 4) == 0)
    {
        state = PCO2_PRG8;
    }

    else if (strncmp("PUMP_TOGGLE", mode, 11) == 0)
    {
        state = MCONTROL_PCO2_GetMode();
        state ^= 0x01;
    }

    if (MCONTROL_PCO2_SetMode(state) == MCONTROL_OK)
    {
        strcpy(SysInfo.mode, mode);
        result = COMMAND_OK;
    }

    return result;
}

eCommandStatus_t COMMAND_SetCurrent(uint8_t current)
{
    eCommandStatus_t result = COMMAND_FAIL;
    if (MCONTROL_PCO2_SetCurrent(current) == MCONTROL_OK)
    {
        result = COMMAND_OK;
    }
    return result;
}

eCommandStatus_t COMMAND_SetChannel(uint8_t channel)
{
    eCommandStatus_t result = COMMAND_FAIL;
    /** @todo Implement this function.  Need to add set channel to MCONTROL */
    return result;
}

eCommandStatus_t COMMAND_SetPulse(uint16_t ms)
{
    eCommandStatus_t result = COMMAND_FAIL;
    if (MCONTROL_PCO2_SetPulse(ms) == MCONTROL_OK)
    {
        result = COMMAND_OK;
    }
    return result;
}

eCommandStatus_t COMMAND_SetBaud(uint32_t baud)
{
    eCommandStatus_t result = COMMAND_FAIL;

    uint32_t previous_baud = command.baudrate;
    command.baudrate = baud;
    if (BSP_UART_Init(&command) == BSP_OK)
    {
        result = COMMAND_OK;
    }
    else
    {
        command.baudrate = previous_baud;
        BSP_UART_Init(&command);
    }

    return result;
}

char *COMMAND_GetMode(void)
{

    char result[32];
    char *pResult = &result[0];

    memset(pResult, 0, 32);

    switch (MCONTROL_PCO2_GetMode())
    {

    case PCO2_ZERO_PUMP_ON:
        strcpy(result, "ZERO_ON");
        break;
    case PCO2_ZERO_PUMP_OFF:
        strcpy(result, "ZERO_OFF");
        break;
    case PCO2_ZERO_CAL:
        strcpy(result, "ZERO_POST");
        break;
    case PCO2_SPAN_PUMP_ON:
        strcpy(result, "SPAN_ON");
        break;
    case PCO2_SPAN_PUMP_OFF:
        strcpy(result, "SPAN_OFF");
        break;
    case PCO2_SPAN_CAL:
        strcpy(result, "SPAN_POST");
        break;
    case PCO2_EQUIL_PUMP_ON:
        strcpy(result, "EQUIL_ON");
        break;
    case PCO2_EQUIL_PUMP_OFF:
        strcpy(result, "EQUIL_OFF");
        break;
    case PCO2_AIR_PUMP_ON:
        strcpy(result, "AIR_ON");
        break;
    case PCO2_AIR_PUMP_OFF:
        strcpy(result, "AIR_OFF");
        break;
    case PCO2_REST:
        strcpy(result, "REST");
        break;
    case PCO2_DEPLOYMENT:
        strcpy(result, "DEPLOY");
        break;
    case PCO2_PRG1:
        strcpy(result, "PRG1");
        break;
    case PCO2_PRG2:
        strcpy(result, "PRG2");
        break;
    case PCO2_PRG3:
        strcpy(result, "PRG3");
        break;
    case PCO2_PRG4:
        strcpy(result, "PRG4");
        break;
    case PCO2_PRG5:
        strcpy(result, "PRG5");
        break;
    case PCO2_PRG6:
        strcpy(result, "PRG6");
        break;
    case PCO2_PRG7:
        strcpy(result, "PRG7");
        break;
    case PCO2_PRG8:
        strcpy(result, "PRG8");
        break;
    default:
        strcpy(result, "");
        break;
    }

    return pResult;
}

uint8_t COMMAND_GetCurrent(void)
{
    return MCONTROL_PCO2_GetCurrent();
}

uint8_t COMMAND_GetChannel(void)
{
    /** @todo Implement this channel function when necessary */
  return 0;
}

uint32_t COMMAND_GetPulse(void)
{
    return MCONTROL_PCO2_GetPulse();
}
char *COMMAND_GetVersion(void)
{
    return &SysInfo.ver[0];
}
char *COMMAND_GetSerial(void)
{
    return &SysInfo.ser[0];
}
uint32_t COMMAND_GetCount(void)
{
    return SysInfo.StartupCounter;
}

uint32_t COMMAND_GetBaud(void)
{
    return command.baudrate;
}

/************************************************************************
*					         Static Functions
************************************************************************/
STATIC void write_current_state(void)
{
    uint8_t dir;
    uint8_t chan;
    uint8_t pump;
    for (chan = 0; chan < 6; chan++)
    {
        dir = MCONTROL_PCO2_GetDir(chan);
        if (dir == MOTION_VALVE_CA)
        {
            BSP_UART_putc(&command, 'A');
        }
        else if (dir == MOTION_VALVE_CB)
        {
            BSP_UART_putc(&command, 'B');
        }
        else
        {
            BSP_UART_putc(&command, 'X');
        }
    }
    pump = MCONTROL_PCO2_GetPump();
    if (pump == PUMP_OFF)
    {
        BSP_UART_putc(&command, '0');
    }
    else if (pump == PUMP_ON)
    {
        BSP_UART_putc(&command, '1');
    }
    else
    {
        BSP_UART_putc(&command, 'X');
    }
}