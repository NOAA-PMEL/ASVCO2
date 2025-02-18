/** @file commandCmd.c
 *  @brief Command Commands
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date May 16, 2018
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
#include "commandCmd.h"

/************************************************************************
 *					   STATIC Functions Prototypes
 ************************************************************************/

STATIC eCommandCmdStatus_t COMMANDCMD_Ver(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_Help(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_CShow(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_gps(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_time(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_System(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_Startup(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_Who(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_SerialNum(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Span(sUART_t *pUART, const char buffer[]);
#ifdef GREEN_LED_COMMAND
STATIC eCommandCmdStatus_t COMMANDCMD_GreenLedOn(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_GreenLedOff(sUART_t *pUART, const char buffer[]);
#endif
#ifdef RED_LED_COMMAND
STATIC eCommandCmdStatus_t COMMANDCMD_RedLedOn(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_RedLedOff(sUART_t *pUART, const char buffer[]);
#endif
STATIC eCommandCmdStatus_t COMMANDCMD_SReset(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_Reset(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_IData(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_SData(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_SDataWrite(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_SLog(sUART_t *pUART, const char buffer[]);
STATIC eCommandCmdStatus_t COMMANDCMD_ELog(sUART_t *pUART, const char buffer[]);
STATIC bool is_write_cmd(const char buffer[]);
/************************************************************************
 *					        STATIC Variables
 ************************************************************************/
STATIC const sCommandCommandTable_t mCommandCommandTable[] =
    {
        {"?", &COMMANDCMD_Help, HELP("This Help menu")},
        {"ver", &COMMANDCMD_Ver, HELP("Get the version string")},
        {"cshow", &COMMANDCMD_CShow, HELP("Show COM Data")},
        {"gps", &COMMANDCMD_gps, HELP("GPS Data (-2 for Data Logger)")},
        {"time", &COMMANDCMD_time, HELP("Sensor Date & Time (yy mm dd hh mm ss)")},
        {"who", &COMMANDCMD_Who, HELP("Serial Number")},
        {"serial", &COMMANDCMD_SerialNum, HELP("Serial Number")},
        {"span", &CONSOLECMD_Span, HELP("Span Gas Concentration (ppm)")},
        {"system", &COMMANDCMD_System, HELP("Display All System Settings")},
        {"startup", &COMMANDCMD_Startup, HELP("Display System Startup Count")},
        {"sreset", &COMMANDCMD_SReset, HELP("Software Reset")},
        {"reset", &COMMANDCMD_Reset, HELP("Hardware Reset")},
        {"idata", &COMMANDCMD_IData, HELP("IData")},
        {"sdata", &COMMANDCMD_SData, HELP("SData")},

        {"@@@", &COMMANDCMD_SDataWrite, HELP("Start of SData Wrapper, see User Manual")},
        {"slog", &COMMANDCMD_SLog, HELP("System Log Level (0 - 3)")},
        {"elog", &COMMANDCMD_ELog, HELP("System Error Log Level (0 - 3)")},
#ifdef GREEN_LED_COMMAND
        {"grnon", &COMMANDCMD_GreenLedOn, HELP("Test Mode Only: Turn Licor ON")},
        {"grnoff", &COMMANDCMD_GreenLedOff, HELP("Test Mode Only: Turn Licor OFF")},
#endif
#ifdef RED_LED_COMMAND
        {"redon", &COMMANDCMD_RedLedOn, HELP("Test Mode Only: Turn Licor ON")},
        {"redoff", &COMMANDCMD_RedLedOff, HELP("Test Mode Only: Turn Licor OFF")},
#endif

        COMMAND_COMMAND_TABLE_END // must be LAST
};

const char cshowstr[] = "COMM1\r\n\
Control\r\n\
samplestart=00:00:00\r\n\
sampleinterval=00:00:00\r\n\
sampleperiod=00:00:00\r\n\
NA\r\n\
samplestart=00:00:00\r\n\
sampleinterval=00:00:00\r\n\
sampleperiod=00:00:00\r\n\
NA\r\n\
samplestart=00:00:00\r\n\
sampleinterval=00:00:00\r\n\
sampleperiod=00:00:00\r\n";

#ifdef UNIT_TESTING
uint32_t commandCommandTableSize = sizeof(mCommandCommandTable) / sizeof(mCommandCommandTable[0]);
#endif

/************************************************************************
 *					         Functions
 ************************************************************************/
const sCommandCommandTable_t *COMMANDCMD_GetTable(void)
{
    return &mCommandCommandTable[0];
}
/************************************************************************
 *					   STATIC Functions
 ************************************************************************/
STATIC eCommandCmdStatus_t COMMANDCMD_Help(sUART_t *pUART, const char buffer[])
{
    uint32_t i;
    uint32_t tableLength;
    eCommandCmdStatus_t result = COMMANDCMD_OK;

    IGNORE_UNUSED_VARIABLE(buffer);

    tableLength = sizeof(mCommandCommandTable) / sizeof(mCommandCommandTable[0]);
    for (i = 0u; i < tableLength - 1u; i++)
    {
        CONSOLEIO_SendStr(pUART, (char *)mCommandCommandTable[i].name);
#if COMMAND_COMMAND_MAX_HELP_LENGTH > 0
        CONSOLEIO_SendStr(pUART, "\r\t : ");
        CONSOLEIO_SendStr(pUART, (char *)mCommandCommandTable[i].help);
#endif // COMMAND_COMMAND_MAX_HELP_LENGTH > 0
        CONSOLEIO_SendStr(pUART, STR_ENDLINE);

#if COMMAND_COMMAND_MAX_HELP_LENGTH > 0

#endif // COMMAND_COMMAND_MAX_HELP_LENGTH > 0
    }

    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_Ver(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_OK;

    IGNORE_UNUSED_VARIABLE(buffer);

    CONSOLEIO_SendStr(pUART, (char *)sysinfo.ver);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_CShow(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    IGNORE_UNUSED_VARIABLE(buffer);
    CONSOLEIO_SendStr(pUART, (char *)cshowstr);

    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_gps(sUART_t *pUART, const char buffer[])
{
    char gps[16];
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;

    sprintf(gps, "%li", sysinfo.gps);

    CONSOLEIO_SendStr(pUART, gps);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);
    IGNORE_UNUSED_VARIABLE(buffer);

    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_time(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    char timestr[32];
    memset(timestr, 0, 32);

    /** Check for write or read command */
    if (true == is_write_cmd(buffer))
    {
        /** Write the time */
        if (AM08X5_OK != AM08X5_SetTime((char *)&buffer[2]))
        {
            return result;
        }
        CONSOLEIO_SendStr(pUART, STR_ACK);
    }

    /** Get the Time */
    AM08X5_GetTimestamp_ISO8601(timestr);

    /** Send the time */
    CONSOLEIO_SendStr(pUART, timestr);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_System(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    IGNORE_UNUSED_VARIABLE(buffer);
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_Startup(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    char startup[20];
    memset(startup, 0, 20);

    IGNORE_UNUSED_VARIABLE(buffer);

    sprintf(startup, "%lu", sysinfo.StartupCounter);
    CONSOLEIO_SendStr(pUART, startup);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_Who(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;

    CONSOLEIO_SendStr(pUART, sysinfo.SerialNumber);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_SerialNum(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;

    /** Check for write or read command */
    if (true == is_write_cmd(buffer))
    {
        char *pCRLF;
        buffer += 2;
        pCRLF = strstr(buffer, "\r\n");
        uint16_t len = (pCRLF - buffer);
        if (20 >= len)
        {
            memset(sysinfo.SerialNumber, 0, 20);
            strncpy(sysinfo.SerialNumber, buffer, len);
        }
    }

    CONSOLEIO_SendStr(pUART, sysinfo.SerialNumber);
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Span(sUART_t *pUART, const char buffer[])
{
    eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

    float span = 0.0;

    /** Check for write command */
    if (true == is_write_cmd(buffer))
    {
        span = atof(&buffer[2]);
        PCO2_SetSpan(span);
    }

    memset(sendstr, 0, 64);
    span = PCO2_GetSpan();

    sprintf(sendstr, "span= %.2f\r\n", span);

    CONSOLEIO_SendStr(pUART, sendstr);

    result = CONSOLECMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_SLog(sUART_t *pUART,
                                           const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    /** Check for write or read command */
    if (true == is_write_cmd(buffer))
    {
        /** Configure the log mode */
        uint8_t state = atoi(&buffer[1]);

        eLogPort_t port = LOG_PORT_None;
        eLogType_t type = LOGGING_None;

        /** Is it set to ALL ON? */
        if ((state == 0x01) || (state >= 0x63))
        {
            type = LOGGING_All;
        }
        else
        {
            /** Find Timestamping */
            if (state & 0x01)
            {
            }

            /** Find Logging Type */
            if (state & 0x02)
            {
                type |= LOGGING_Warnings;
            }
            if (state & 0x04)
            {
                type |= LOGGING_Critical;
            }

            /** Find Port */
            if (state & 0x08)
            {
                port |= LOG_PORT_Console;
            }
            if (state & 0x10)
            {
                port |= LOG_PORT_Command;
            }
            if (state & 0x02)
            {
                port |= LOG_PORT_Datalogger;
            }
        }
    }

    /** Print the log configuration */
    LogPrint();

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_ELog(sUART_t *pUART,
                                           const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    /** Check for write or read command */
    if (true == is_write_cmd(buffer))
    {
        /** Configure the log mode */
        uint8_t state = atoi(&buffer[1]);

        eLogPort_t port = LOG_PORT_None;
        eLogType_t type = LOGGING_None;

        /** Is it set to ALL ON? */
        if ((state == 0x01) || (state >= 0x63))
        {
            type = LOGGING_All;
        }
        else
        {
            /** Find Timestamping */
            if (state & 0x01)
            {
            }

            /** Find Logging Type */
            if (state & 0x02)
            {
                type |= LOGGING_Warnings;
            }
            if (state & 0x04)
            {
                type |= LOGGING_Critical;
            }

            /** Find Port */
            if (state & 0x08)
            {
                port |= LOG_PORT_Console;
            }
            if (state & 0x10)
            {
                port |= LOG_PORT_Command;
            }
            if (state & 0x02)
            {
                port |= LOG_PORT_Datalogger;
            }
        }
    }

    /** Print the log configuration */
    ErrorPrint();

    result = COMMANDCMD_OK;
    return result;
}

#ifdef GREEN_LED_COMMAND
STATIC eCommandCmdStatus_t COMMANDCMD_GreenLedOn(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    sTimeReg_t t1;

    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

    /** Green LED ON*/
    LED_SetState(&grnLED, LED_ON_STATE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_GreenLedOff(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;

    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

    /** Green LED OFF*/
    LED_SetState(&grnLED, LED_OFF_STATE);

    result = COMMANDCMD_OK;
    return result;
}
#endif

#ifdef RED_LED_COMMAND
STATIC eCommandCmdStatus_t COMMANDCMD_RedLedOn(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    sTimeReg_t t1;

    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

    /** Red LED ON*/
    LED_SetState(&redLED, LED_ON_STATE);

    result = COMMANDCMD_OK;
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_RedLedOff(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    sTimeReg_t t1;

    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

    /** Clear the time struct */
    AM08X5_clear_time_struct(&t1);

    /** Red LED OFF*/
    LED_SetState(&redLED, LED_OFF_STATE);

    result = COMMANDCMD_OK;
    return result;
}
#endif

STATIC eCommandCmdStatus_t COMMANDCMD_SReset(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_OK;
    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

/** Software Reset */
#ifndef UNIT_TESTING
    PMMCTL0 |= PMMSWBOR;
#endif

    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_Reset(sUART_t *pUART, const char buffer[])
{

    /** Ingore other strings in the buffer */
    IGNORE_UNUSED_VARIABLE(buffer);

/** Hardware Reset */
#ifndef UNIT_TESTING
    /** Go into infinite while loop, wait for watchdog to reset the micro */
    while (1)
        ;
#endif
}

STATIC eCommandCmdStatus_t COMMANDCMD_IData(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_OK;
    CONSOLEIO_SendStr(pUART, "\r\n");
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_SData(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_OK;
    CONSOLEIO_SendStr(pUART, "\r\n");
    return result;
}

STATIC eCommandCmdStatus_t COMMANDCMD_SDataWrite(sUART_t *pUART, const char buffer[])
{
    eCommandCmdStatus_t result = COMMANDCMD_FAIL;
    return result;
}

/** @brief Check Write Command
 *
 * Checks the buffer for a "= " (equals sign and space) to determine
 * if it is a write string or
 *
 * @param buffer Pointer to buffer
 *
 * @return result (write=true or read=false)
 */
STATIC bool is_write_cmd(const char buffer[])
{
    bool result = false;

    if (('=' == buffer[0]) && (' ' == buffer[1]))
    {
        result = true;
    }

    return result;
}