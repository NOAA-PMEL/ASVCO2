/** @file LI8x0.h
 *  @brief LIcor 800 series drivers
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 15, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note Licor LI-820/830 Drivers
 *  @note
 *
 *  @bug  No known bugs
 */
#include "LI8x0.h"

/************************************************************************
 *					        Static Variables
 ************************************************************************/

STATIC PERSISTENT char LicorBuffer[LICOR_BUFFER_LEN];
STATIC char LicorSubstr[256];

PERSISTENT sLicor_t Licor1;
PERSISTENT sLicor_t *pLicor1 = &Licor1;

STATIC PERSISTENT sLicor_t Licor1 = {
    .UART = {.param = {.selectClockSource = LICOR_UART_CLK,
                       .parity = LICOR_UART_PARITY,
                       .msborLsbFirst = LICOR_UART_MSB_FIRST,
                       .numberofStopBits = LICOR_UART_STOPBITS,
                       .uartMode = LICOR_UART_MODE},
             .baseAddress = EUSCI_A2_BASE,
             .baudrate = 9600,
             .configured = 0,
             .enabled = 0,
             .interrupt = 0,
             .msg = 0},
    .Power = {
        .pin = {.port = LICOR_ENABLE_PORT,
                .pin = LICOR_ENABLE_PIN,
                .mode = LICOR_ENABLE_DIR,
                .inverted = LICOR_ENABLE_POL},
    },
    .sysconfig = {.serialnum = "", .ver = ""},
    .type = LICOR_LI820,
};

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC eLicorStatus_t LI8x0_ValidateXML(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_ValidXML_Level2(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_ValidXML_Level3(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Validate_NoPump(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Validate_NoH2O(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Level1Str(eLicorXMLLvl1_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);
STATIC eLicorStatus_t LI8x0_Level2Str(eLicorXMLLvl2_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);
STATIC eLicorStatus_t LI8x0_Level3Str(eLicorXMLLvl3_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);
STATIC eLicorStatus_t LI8x0_Level4Str(eLicorXMLLvl4_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);
STATIC eLicorStatus_t LI8x0_IsAck(sLicorXML_t *XML,
                                  char *sstr,
                                  uint16_t slen);
STATIC eLicorStatus_t LI8x0_ParseString(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseData(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseAuxdata(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseRS232(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseCFG(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseCal(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParsePoly(char *str, uint16_t strlen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParsePump(char *str, uint16_t slen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseSource(char *str, uint16_t slen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseTech(char *str, uint16_t slen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8x0_ParseTechPump(char *str, uint16_t slen, sLicorTags_t *tags);
STATIC eLicorStatus_t LI8X0_FindSubstring(char *searchstr, char *substr, uint32_t sstrlen,
                                          uint32_t *startIndex, uint32_t *endIndex);
STATIC eLicorStatus_t LI8x0_ParseDACS(char *str, uint16_t slen, sLicorTags_t *tags);
STATIC void LI8X0_ClearTags(sLicorTags_t *tags);
STATIC void LI8x0_ReadPrep(void);
STATIC eLicorStatus_t LI8X0_Read(char *sendstr, uint16_t mstimeout);
STATIC void add_bool_to_string(char *sstr, bool val);
STATIC uint16_t add_cr_lf_and_tab_to_string(char *sstr, uint8_t numCrLf, uint8_t numTabs);
STATIC eLicorStatus_t LI8x0_WaitForACK(char *sstr);

/************************************************************************
 *					         Functions
 ************************************************************************/
eLicorStatus_t LI8x0_Init(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    Log("LICOR Initialize Start", LOG_Priority_High);
    /** Initialize the Licor UART */
    if (BSP_UART_Init(&pLicor1->UART) != BSP_FAIL)
    {
        BSP_UART_Enable(&pLicor1->UART);
        BSP_UART_EnableInterrupt(&pLicor1->UART, EUSCI_A_UART_RECEIVE_INTERRUPT);

        result = LICOR_OK;
    }
    else
    {
        __delay_cycles(500);
    }
    /** Initialize the Licor Power Relay Pin */
    BSP_GPIO_Init(&pLicor1->Power.pin);

    /** Autodiscover the type of Licor */
    LI8x0_AutoDiscover();

    Log("LICOR Initialize Complete", LOG_Priority_High);
    return result;
}

void LI8x0_PowerON(void)
{

    /** Take Board out of Low Current mode */
    BSP_Low_Current_Mode_OFF();

    _delay_ms(50);

    /** Turn Licor ON */
    BSP_GPIO_SetPin(&Licor1.Power.pin);

    /** Store State */
    Licor1.Power.state = true;
    Log("LICOR Power On", LOG_Priority_High);
}

void LI8x0_PowerOFF(void)
{
    /** Turn Pin Off */

    BSP_GPIO_ClearPin(&Licor1.Power.pin);

    /** Clear the power on time */
    AM08X5_clear_time_struct(&Licor1.Power.time);

    /** Store State */
    if (Licor1.Power.state == true)
    {
        Log("LICOR Power Off", LOG_Priority_High);
    }
    Licor1.Power.state = false;
}

bool LI8x0_GetPower(void)
{
    return Licor1.Power.state;
}

void LI8x0_CreateAndSendString(sLicorXML_t *pXML, char *setval, uint16_t svlen)
{

    char sstr[80];
    char estr[80];
    uint16_t slen = 0;
    uint16_t elen = 0;
    memset(sstr, 0, 80);
    memset(estr, 0, 80);

    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strncat(sstr, setval, svlen);
    strcat(sstr, estr);
    slen += svlen;
    slen += elen;
    LI8x0_ReadPrep();
    BSP_UART_puts(&Licor1.UART, sstr, slen);
}

eLicorStatus_t LI8x0_Config(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    eLicorStatus_t t_result = LICOR_FAIL;
    char sstr[80];

    Log("LICOR Config Start", LOG_Priority_High);
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type};
    sLicorXML_t *pXML = &XML;

    /** SHUT THE OUTRATE OFF FIRST */
    /* outrate = 0 */
    XML.LVL2 = LICOR_XML_LVL2_CFG;
    XML.LVL3 = LICOR_XML_LVL3_OUTRATE;
    LI8x0_CreateAndSendString(pXML, "0.0", 3);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* **** RS232 **** */
    XML.LVL2 = LICOR_XML_LVL2_RS232;
    XML.LVL4 = LICOR_XML_LVL4_NONE;

    /* FlowRate = Off */
    if ((LICOR_LI830 == Licor1.type) || (LICOR_LI850 == Licor1.type))
    {
        XML.LVL3 = LICOR_XML_LVL3_FLOWRATE;
        LI8x0_CreateAndSendString(pXML, "FALSE", 5);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
    }

    /* Co2 = On */
    XML.LVL3 = LICOR_XML_LVL3_CO2;
    LI8x0_CreateAndSendString(pXML, "TRUE", 4);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }
    /* celltemp = On */
    XML.LVL3 = LICOR_XML_LVL3_CELLTEMP;
    LI8x0_CreateAndSendString(pXML, "TRUE", 4);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* cellpres = On */
    XML.LVL3 = LICOR_XML_LVL3_CELLPRES;
    LI8x0_CreateAndSendString(pXML, "TRUE", 4);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* ivolt = On */
    XML.LVL3 = LICOR_XML_LVL3_IVOLT;
    LI8x0_CreateAndSendString(pXML, "TRUE", 4);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* co2abs = On */
    XML.LVL3 = LICOR_XML_LVL3_CO2ABS;
    LI8x0_CreateAndSendString(pXML, "FALSE", 5);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* echo = Off */
    XML.LVL3 = LICOR_XML_LVL3_ECHO;
    LI8x0_CreateAndSendString(pXML, "FALSE", 5);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* strip = Off */
    XML.LVL3 = LICOR_XML_LVL3_STRIP;
    LI8x0_CreateAndSendString(pXML, "FALSE", 4);
    t_result = LI8x0_WaitForACK(sstr);
    if (LICOR_FAIL == t_result)
    {
        return result;
    }

    /* h2o = Ignored if LI820 or LI830 */
    if ((LICOR_LI840 == Licor1.type) || (LICOR_LI850 == Licor1.type))
    {
        /* h2oabs = On */
        XML.LVL3 = LICOR_XML_LVL3_H2O;
        LI8x0_CreateAndSendString(pXML, "TRUE", 4);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
        /* h2oabs = Off */
        XML.LVL3 = LICOR_XML_LVL3_CO2ABS;
        LI8x0_CreateAndSendString(pXML, "FALSE", 5);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
        /* raw_h20 = On */
        XML.LVL3 = LICOR_XML_LVL3_RAW;
        XML.LVL4 = LICOR_XML_LVL4_H2O;
        LI8x0_CreateAndSendString(pXML, "TRUE", 4);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
        /* raw_h20ref = On */
        XML.LVL4 = LICOR_XML_LVL4_H2O;
        LI8x0_CreateAndSendString(pXML, "TRUE", 4);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
    }

    /* raw_co2 = On */
    XML.LVL3 = LICOR_XML_LVL3_RAW;
    XML.LVL4 = LICOR_XML_LVL4_NONE;
    if ((Licor1.type == LICOR_LI830) || (Licor1.type == LICOR_LI850))
    {
        LI8x0_CreateAndSendString(pXML, "TRUE", 4);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
    }
    else
    {
        LI8x0_CreateAndSendString(pXML, "TRUE", 4);
        t_result = LI8x0_WaitForACK(sstr);
        if (LICOR_FAIL == t_result)
        {
            return result;
        }
    }

    /* **** CFG **** */
    XML.LVL2 = LICOR_XML_LVL2_CFG;
    XML.LVL4 = LICOR_XML_LVL4_NONE;

    /* heater = Off */
    XML.LVL3 = LICOR_XML_LVL3_HEATER;
    LI8x0_CreateAndSendString(pXML, "FALSE", 5);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* pcomp = On */
    XML.LVL3 = LICOR_XML_LVL3_PCOMP;
    LI8x0_CreateAndSendString(pXML, "True", 4);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* filter = 0 */
    XML.LVL3 = LICOR_XML_LVL3_FILTER;
    LI8x0_CreateAndSendString(pXML, "0.0", 3);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* outrate = 0 */
    XML.LVL3 = LICOR_XML_LVL3_OUTRATE;
    LI8x0_CreateAndSendString(pXML, "0.0", 3);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* bench = 14 */
    XML.LVL3 = LICOR_XML_LVL3_BENCH;
    LI8x0_CreateAndSendString(pXML, "14", 2);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* span = SPAN */
    XML.LVL3 = LICOR_XML_LVL3_SPAN;
    LI8x0_CreateAndSendString(pXML, "0.0", 3);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }
    /* alarm_enabled = Off */
    XML.LVL3 = LICOR_XML_LVL3_ALARMS;
    XML.LVL4 = LICOR_XML_LVL4_ENABLED;
    LI8x0_CreateAndSendString(pXML, "FALSE", 5);
    if (LICOR_FAIL == LI8x0_WaitForACK(sstr))
    {
        return result;
    }

    Log("LICOR Config Complete", LOG_Priority_High);
    result = LICOR_OK;
    return result;
}

eLicorStatus_t LI8x0_Get_SerialNumber(char *str)
{

    eLicorStatus_t result = LICOR_FAIL;

    if (strlen(Licor1.sysconfig.serialnum) > 0)
    {
        strcpy(str, Licor1.sysconfig.serialnum);
        result = LICOR_OK;
    }
    return result;
}

eLicorStatus_t LI8x0_Get_Version(char *str)
{
    eLicorStatus_t result = LICOR_FAIL;
    strcpy(str, Licor1.sysconfig.ver);
    return result;
}

void LI8x0_Get_ccva1_1(char *str)
{
  sprintf(str, "%3.5f", Licor1.sysconfig.poly.co2.a1);
}
void LI8x0_Get_ccva1_2(char *str)
{
  sprintf(str, "%3.5f", Licor1.sysconfig.poly.co2.a2);
}

void LI8x0_Get_ccva1_3(char *str)
{
  sprintf(str, "%3.5f", Licor1.sysconfig.poly.co2.a3);
}

void LI8x0_Get_ccva1_4(char *str)
{
  sprintf(str, "%3.5f", Licor1.sysconfig.poly.co2.a4);
}


eLicorStatus_t LI8x0_Get_Zero(char *str)
{
    eLicorStatus_t result = LICOR_FAIL;
    sprintf(str, "%3.8f", Licor1.sysconfig.cal.co2kzero);
    return result;
}

eLicorStatus_t LI8x0_Get_Span(char *str)
{
    eLicorStatus_t result = LICOR_FAIL;
    sprintf(str, "%3.8f", Licor1.sysconfig.cal.co2kspan);
    return result;
}

float LI8x0_Get_KZero_float(void)
{
    return Licor1.sysconfig.cal.co2kzero;
}

float LI8x0_Get_KSpan_float(void)
{
    return Licor1.sysconfig.cal.co2kspan;
}

float LI8x0_Get_KSpan2_float(void)
{
    return Licor1.sysconfig.cal.co2kspan2;
}

eLicorStatus_t LI8x0_Get_Span2(char *str)
{
    eLicorStatus_t result = LICOR_FAIL;
    sprintf(str, "%3.8f", Licor1.sysconfig.cal.co2kspan2);
    return result;
}

eLicorStatus_t LI8x0_Set_Zero(float val)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    char temp[20];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i;
    memset(temp, 0, 20);

    /** */
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2KZERO,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    sprintf(temp, "%3.8f", val);
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, temp);
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 2000000;
    do
    {
        _delay_us(1);
    } while ((--i > 0) && (LicorRxFlag != true));

    if (LI8x0_IsAck(pXML, (char *)LicorRxBuffer, strlen((char *)LicorRxBuffer)) == LICOR_OK)
    {

        /** Read Response */
        if (LicorRxFlag == true)
        {
            if (LI8x0_ParseData((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
            {
                result = LICOR_OK;
            }
        }
    }

    return result;
}

eLicorStatus_t LI8x0_Set_Span(float val)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    char temp[20];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i;
    memset(temp, 0, 20);

    /** */
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2KSPAN,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    sprintf(temp, "%3.8f", val);
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, temp);
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 2000000;
    do
    {
        _delay_us(1);
    } while ((--i > 0) && (LicorRxFlag != true));

    if (LI8x0_IsAck(pXML, (char *)LicorRxBuffer, strlen((char *)LicorRxBuffer)) == LICOR_OK)
    {

        /** Read Response */
        if (LicorRxFlag == true)
        {
            if (LI8x0_ParseData((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
            {
                result = LICOR_OK;
            }
        }
    }

    return result;
}

eLicorStatus_t LI8x0_Set_Span2(float val)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    char temp[20];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i;
    memset(temp, 0, 20);

    /** */
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2KSPAN2,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    sprintf(temp, "%3.8f", val);
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, temp);
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 2000000;
    do
    {
        _delay_us(1);
    } while ((--i > 0) && (LicorRxFlag != true));

    if (LI8x0_IsAck(pXML, (char *)LicorRxBuffer, strlen((char *)LicorRxBuffer)) == LICOR_OK)
    {

        /** Read Response */
        if (LicorRxFlag == true)
        {
            if (LI8x0_ParseData((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
            {
                result = LICOR_OK;
            }
        }
    }

    return result;
}

eLicorStatus_t LI8x0_ReadSample(sLicorData_t *data)
{
    eLicorStatus_t result = LICOR_FAIL;

    if (LI8x0_Sample() != LICOR_FAIL)
    {
        *data = Licor1.sysconfig.data;
    }

    /** Parse the message and */

    return result;
}

eLicorStatus_t LI8x0_Sample(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i;

    /** */
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_DATA,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "?");
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 2000000;
    do
    {
        _delay_us(1);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LI8x0_IsAck(pXML, (char *)LicorRxBuffer, strlen((char *)LicorRxBuffer)) == LICOR_OK)
    {

        /** Read Response */
        if (LicorRxFlag == true)
        {
            if (LI8x0_ParseData((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
            {
                result = LICOR_OK;
            }
        }
    }
    return result;
}

eLicorStatus_t LI8x0_Sample_Query(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;

    /** */
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_DATA,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "?");
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    result = LICOR_OK;
    return result;
}

eLicorStatus_t LI8x0_Sample_Read(sLicorData_t *data)
{
    eLicorStatus_t result = LICOR_FAIL;
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_DATA,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;
    uint32_t i;

    /** Wait for response */
    i = 2000000;
    do
    {
        _delay_us(1);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LI8x0_IsAck(pXML, (char *)LicorRxBuffer, strlen((char *)LicorRxBuffer)) == LICOR_OK)
    {
        /** Read Response */
        if (LicorRxFlag == true)
        {
            if (LI8x0_ParseData((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
            {
                *data = Licor1.sysconfig.data;
                result = LICOR_OK;
            }
        }
    }
    return result;
}

void LI8x0_Clear_Data(sLicorData_t *data)
{
    data->cellpres = NAN;
    data->celltemp = NAN;
    data->co2 = NAN;
    data->co2abs = NAN;
    data->flowrate = NAN;
    data->h2o = NAN;
    data->h2oabs = NAN;
    data->h2odewpoint = NAN;
    data->ivolt = NAN;
    data->raw.co2 = 0;
    data->raw.co2ref = 0;
    data->raw.h2o = 0;
    data->raw.h2oref = 0;
}

eLicorStatus_t LI8x0_Zero(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint8_t i;

    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2ZERO,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Alert the User */
    Log("LICOR Zero Commanded", LOG_Priority_Med);

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "TRUE");
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 100;
    do
    {
        _delay_ms(10);
    } while ((i-- > 0) && (LicorRxFlag != true));

    /** Verify ACK */
    strcpy(sstr, (char *)LicorRxBuffer);
    for (i = 0; i < 80; i++)
    {
        sstr[i] = toupper(sstr[i]);
    }
    if (LI8x0_IsAck(pXML, sstr, strlen(sstr) != LICOR_OK))
    {
        return result;
    }

    /** Wait for response, up to 60 seconds */
    LI8x0_ReadPrep();

    i = 60;

    do
    {
        _delay_ms(1000);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag == true)
    {
        if (LI8x0_ParseString((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
        {
            result = LICOR_OK;
        }
    }

    return result;
}

eLicorStatus_t LI8x0_Span(float span)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint8_t i;
    char temp[16];
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2SPAN,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Alert the User */
    Log("LICOR Span Commanded", LOG_Priority_Med);

    /** What Span gas concenstration is being used */
    sprintf(temp, "%0.3f", span);

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, temp);
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 100;
    do
    {
        _delay_ms(10);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag != true)
    {
        return result;
    }

    /** Verify ACK */
    strcpy(sstr, (char *)LicorRxBuffer);
    for (i = 0; i < 80; i++)
    {
        sstr[i] = toupper(sstr[i]);
    }
    if (LI8x0_IsAck(pXML, sstr, strlen(sstr) != LICOR_OK))
    {
        return result;
    }

    /** Wait for response, up to 60 seconds */
    LI8x0_ReadPrep();

    i = 60;

    do
    {
        _delay_ms(1000);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag == true)
    {
        if (LI8x0_ParseString((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
        {
            result = LICOR_OK;
        }
    }

    Licor1.sysconfig.cal.co2span = span;

    return result;
}

eLicorStatus_t LI8x0_Span2(float span)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint8_t i;
    char temp[16];
    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)Licor1.type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_CO2SPAN2,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Alert the User */
    Log("LICOR Span2 Commanded", LOG_Priority_Med);

    /** What Span gas concenstration is being used */
    sprintf(temp, "%0.3f", span);

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, temp);
    strcat(sstr, estr);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    /** Wait for response */
    i = 100;
    do
    {
        _delay_ms(10);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag != true)
    {
        return result;
    }

    /** Verify ACK */
    strcpy(sstr, (char *)LicorRxBuffer);
    for (i = 0; i < 80; i++)
    {
        sstr[i] = toupper(sstr[i]);
    }
    if (LI8x0_IsAck(pXML, sstr, strlen(sstr) != LICOR_OK))
    {
        return result;
    }

    /** Wait for response, up to 60 seconds */
    LI8x0_ReadPrep();

    i = 60;

    do
    {
        _delay_ms(1000);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag == true)
    {
        if (LI8x0_ParseString((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->sysconfig) == LICOR_OK)
        {
            result = LICOR_OK;
        }
        else
        {
            Log("Failure to Span2", LOG_Priority_High);
        }
    }
    else
    {
        Log("Span2 timeout", LOG_Priority_High);
    }

    Licor1.sysconfig.cal.co2span2 = span;

    return result;
}

eLicorStatus_t LI8x0_CreateTags(sLicorXML_t *XML,
                                char *sstr,
                                uint16_t *slen,
                                char *estr,
                                uint16_t *elen)
{

    eLicorStatus_t result = LICOR_FAIL;

    /** Check for valid commands */
    if (LI8x0_ValidateXML(XML) == LICOR_OK)
    {

        /** Get Level 1 Strings */
        if (LI8x0_Level1Str(XML->LVL1, sstr, slen, estr, elen) != LICOR_OK)
        {
            return result;
        }

        if (LI8x0_Level2Str(XML->LVL2, sstr, slen, estr, elen) != LICOR_OK)
        {
            return result;
        }

        if (LI8x0_Level3Str(XML->LVL3, sstr, slen, estr, elen) != LICOR_OK)
        {
            return result;
        }

        if (LI8x0_Level4Str(XML->LVL4, sstr, slen, estr, elen) != LICOR_OK)
        {
            return result;
        }
    }

    result = LICOR_OK;

    return result;
}

eLicorStatus_t LI8x0_GetInfo(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i = 0;

    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)pLicor1->type,
                       .LVL2 = LICOR_XML_LVL2_NONE,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    Log("LICOR Get System Info", LOG_Priority_High);

    /** Write to UART */

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "?");
    strcat(sstr, estr);
    strcat(sstr, "\r\n");

    LI8x0_ReadPrep();

    if (LI8X0_Read(sstr, 5000) != LICOR_OK)
    {
        return result;
    }

    /** Upper case the buffer */
    for (i = 0; i < LICOR_BUFFER_LEN; i++)
    {
        if (LicorRxBuffer[i] == 0)
        {
            break;
        }
        LicorRxBuffer[i] = toupper(LicorRxBuffer[i]);
    }

    /** Parse the string */
    if (LI8x0_ParseString((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->readconfig) == LICOR_OK)
    {

        result = LICOR_OK;
        /** Copy the readstring to the system values */
        LI8X0_ClearTags(&pLicor1->sysconfig);
        pLicor1->sysconfig = pLicor1->readconfig;
    }

    return result;
}

eLicorStatus_t LI8x0_AutoDiscover(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[] = "<who>\r\n";
    uint32_t i = 0;
    Log("LICOR Autodiscover Start", LOG_Priority_High);
    /** Make sure the Licor is powered up */
    LI8x0_PowerON();
    _delay_ms(100);

    BSP_UART_puts(&Licor1.UART, "<li830><cfg><outrate>0</outrate></cfg></li830>\r\n", 35);
    _delay_ms(250);

    /** Send String */
    LI8x0_ReadPrep();
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));
    _delay_ms(250);
    /** Wait for response */
    i = 300;
    do
    {
        _delay_ms(10);
    } while ((i-- > 0) && (LicorRxFlag != true));

    /** Uppercase the buffer */
    for (i = 0; i < LICOR_BUFFER_LEN; i++)
    {
        LicorRxBuffer[i] = toupper(LicorRxBuffer[i]);
    }

    /** Figure out which type of Licor */
    result = LICOR_OK;
    if (strstr((char *)LicorRxBuffer, "LI820") != NULL)
    {
        Licor1.type = LICOR_LI820;
    }
    else if (strstr((char *)LicorRxBuffer, "LI830") != NULL)
    {
        Licor1.type = LICOR_LI830;
    }
    else if (strstr((char *)LicorRxBuffer, "LI840") != NULL)
    {
        Licor1.type = LICOR_LI840;
    }
    else if (strstr((char *)LicorRxBuffer, "LI850") != NULL)
    {
        Licor1.type = LICOR_LI850;
    }
    else
    {
        result = LICOR_FAIL;
    }

    Log("LICOR Autodiscover Complete", LOG_Priority_High);
    return result;
}

eLicorStatus_t LI8x0_GetCal(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i = 0;

    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)pLicor1->type,
                       .LVL2 = LICOR_XML_LVL2_CAL,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "?");
    strcat(sstr, estr);
    strcat(sstr, "\r\n");

    /** Prep for read */
    LI8x0_ReadPrep();

    /** Read and check results */
    if (LI8X0_Read(sstr, 5000) != LICOR_OK)
    {
        return result;
    }

    /** Upper case the buffer */
    for (i = 0; i < LICOR_BUFFER_LEN; i++)
    {
        LicorRxBuffer[i] = toupper(LicorRxBuffer[i]);
    }

    /** Parse the string */
    if (LI8x0_ParseString((char *)LicorRxBuffer, strlen((char *)LicorRxBuffer), &pLicor1->readconfig) == LICOR_OK)
    {

        result = LICOR_OK;
        /** Copy the readstring to the system values */
        LI8X0_ClearTags(&pLicor1->sysconfig);
        pLicor1->sysconfig = pLicor1->readconfig;
    }

    return result;
}

eLicorStatus_t LI8x0_SetLicor(void)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;

    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)pLicor1->type,
                       .LVL2 = LICOR_XML_LVL2_NONE,
                       .LVL3 = LICOR_XML_LVL3_NONE,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    strcat(sstr, "?");
    strcat(sstr, estr);
    return result;
}

void LI8x0_ClearBuffer(void)
{
    BufferC_Clear(&Licor1.UART.buf);
}

eLicorStatus_t LI8x0_Heater(uint8_t state)
{
    eLicorStatus_t result = LICOR_FAIL;
    char sstr[80];
    char estr[40];
    uint16_t slen = 0;
    uint16_t elen = 0;
    uint32_t i = 0;

    sLicorXML_t XML = {.LVL1 = (eLicorXMLLvl1_t)pLicor1->type,
                       .LVL2 = LICOR_XML_LVL2_CFG,
                       .LVL3 = LICOR_XML_LVL3_HEATER,
                       .LVL4 = LICOR_XML_LVL4_NONE};
    sLicorXML_t *pXML = &XML;

    /** Create String */
    LI8x0_CreateTags(pXML, sstr, &slen, estr, &elen);
    switch (state)
    {
    case 0:
        strcat(sstr, "FALSE");
        break;
    case 1:
        strcat(sstr, "TRUE");
        break;
    default:
        return result;
        break;
    }

    strcat(sstr, estr);

    /** Write to UART */
    BSP_UART_puts(&pLicor1->UART, sstr, strlen(sstr));

    memset(LicorBuffer, 0, LICOR_BUFFER_LEN);
    /** Wait for response */
    for (i = 0; i < 5; i++)
    {
        if (BufferC_gets(&pLicor1->UART.buf, LicorBuffer, LICOR_BUFFER_LEN) == BUFFER_C_OK)
        {
            break;
        }
#ifndef UNIT_TESTING
        _delay_ms(200);
#endif
    }

    /** Upper case the buffer */
    for (i = 0; i < LICOR_BUFFER_LEN; i++)
    {
        LicorBuffer[i] = toupper(LicorBuffer[i]);
    }

    /** Verify ACK */
    if (LI8x0_IsAck(pXML, LicorBuffer, strlen(LicorBuffer)) != LICOR_OK)
    {
        return result;
    }

    result = LICOR_OK;
    return result;
}

void LI8x0_CreateString_SysInfo(char *sstr, uint16_t *len)
{
    char temp[80];
    char temp2[16];

    /** Get Licor Type */
    strcpy(temp2, "LI8");

    switch (Licor1.type)
    {
    case LICOR_LI820:
        strcat(temp2, "20");
        break;
    case LICOR_LI830:
        strcat(temp2, "30");
        break;
    case LICOR_LI840:
        strcat(temp2, "40");
        break;
    case LICOR_LI850:
        strcat(temp2, "50");
        break;
    default:
        return;
        break;
    }

    *len += sprintf(temp, "%s", temp2);
    strcat(sstr, temp);

    /** Newline And Tab */
    len += add_cr_lf_and_tab_to_string(sstr, 1, 1);

    /** Get Ack Condition */
    strcat(sstr, "ACK: ");
    if (Licor1.sysconfig.ack == false)
    {
        strcat(sstr, "False");
    }
    else
    {
        strcat(sstr, "True");
    }

    /** Newline And Tab */
    add_cr_lf_and_tab_to_string(sstr, 1, 1);

    /** Get version */
    strcat(sstr, Licor1.sysconfig.ver);

    /** Create Error */
    LI8x0_CreateString_Error(sstr, 1);

    /** Create Data */
    LI8x0_CreateString_Data(sstr, 1);

    /** Create Auxdata */
    LI8x0_CreateString_Auxdata(sstr, 1);

    /** Create RS232 */
    LI8x0_CreateString_RS232(sstr, 1);

    /** Create Cfg */
    LI8x0_CreateString_Cfg(sstr, 1);

    /** Create Cal */
    LI8x0_CreateString_Cal(sstr, 1);

    /** Create Poly */
    LI8x0_CreateString_Poly(sstr, 1);

    /** Create Source */
    LI8x0_CreateString_Source(sstr, 1);

    /** Create Serial Number */
    LI8x0_CreateString_Serial(sstr, 1);
}

void LI8x0_CreateString_Error(char *sstr, uint8_t indentLevel)
{
    /** Newline And Tab */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel);

    /** Get error */
    strcat(sstr, "Error: ");
    strcat(sstr, Licor1.sysconfig.error);
}

void LI8x0_CreateString_Data(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Data */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel);
    strcat(sstr, "Data");

    /** Data -> flowrate */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.flowrate);
    strcat(sstr, "Flowrate: ");
    strcat(sstr, temp2);

    /** Data -> flowrate */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.celltemp);
    strcat(sstr, "Celltemp: ");
    strcat(sstr, temp2);

    /** Data -> cellpres */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.cellpres);
    strcat(sstr, "Cellpres: ");
    strcat(sstr, temp2);

    /** Data -> co2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.co2);
    strcat(sstr, "CO2: ");
    strcat(sstr, temp2);

    /** Data -> co2abs */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.co2abs);
    strcat(sstr, "CO2ABS: ");
    strcat(sstr, temp2);

    if ((Licor1.type == LICOR_LI840) || (Licor1.type == LICOR_LI850))
    {
        /** Data -> h20 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
        sprintf(temp2, "%3.3f", Licor1.sysconfig.data.h2o);
        strcat(sstr, "H2O: ");
        strcat(sstr, temp2);

        /** Data -> h20abs */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
        sprintf(temp2, "%3.3f", Licor1.sysconfig.data.h2oabs);
        strcat(sstr, "H2OABS: ");
        strcat(sstr, temp2);
    }

    /** Data -> ivolt */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.data.ivolt);
    strcat(sstr, "ivolt: ");
    strcat(sstr, temp2);

    /** Data -> Raw */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Raw");

    /** Data -> Raw -> co2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.data.raw.co2);
    strcat(sstr, "CO2: ");
    strcat(sstr, temp2);

    /** Data -> Raw -> co2ref */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.data.raw.co2ref);
    strcat(sstr, "CO2REF: ");
    strcat(sstr, temp2);

    if ((Licor1.type == LICOR_LI840) || (Licor1.type == LICOR_LI850))
    {
        /** Data -> Raw -> h20 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        sprintf(temp2, "%lu", Licor1.sysconfig.data.raw.h2o);
        strcat(sstr, "H2O: ");
        strcat(sstr, temp2);

        /** Data -> Raw -> h20ref */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        sprintf(temp2, "%lu", Licor1.sysconfig.data.raw.h2oref);
        strcat(sstr, "H2OREF: ");
        strcat(sstr, temp2);
    }
}

void LI8x0_CreateString_Auxdata(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Auxdata */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel);
    strcat(sstr, "Auxdata");

    /** Auxdata -> pca */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.auxdata.pca);
    strcat(sstr, "pca: ");
    strcat(sstr, temp2);

    /** Auxdata -> pha */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.auxdata.pha);
    strcat(sstr, "pha: ");
    strcat(sstr, temp2);

    /** Auxdata -> bb_eff */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.auxdata.bb_eff);
    strcat(sstr, "bb_eff: ");
    strcat(sstr, temp2);

    /** Auxdata -> psi */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3f", Licor1.sysconfig.auxdata.psi);
    strcat(sstr, "psi: ");
    strcat(sstr, temp2);
}

void LI8x0_CreateString_RS232(char *sstr, uint8_t indentLevel)
{
    /** RS232 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel);
    strcat(sstr, "RS232");

    /** Data -> flowrate */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Flowrate: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.flowrate);

    /** Data -> celltemp */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Celltemp: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.celltemp);

    /** Data -> cellpres */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Cellpres: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.cellpres);

    /** Data -> co2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "CO2: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.co2);

    /** Data -> co2abs */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "CO2ABS: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.co2abs);

    if ((Licor1.type == LICOR_LI840) || (Licor1.type == LICOR_LI850))
    {
        /** Data -> h20 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
        strcat(sstr, "H2O: ");
        add_bool_to_string(sstr, Licor1.sysconfig.rs232.h2o);

        /** Data -> h20abs */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
        strcat(sstr, "H2OABS: ");
        add_bool_to_string(sstr, Licor1.sysconfig.rs232.h2oabs);
    }

    /** Data -> ivolt */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "ivolt: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.ivolt);

    /** Data -> Raw */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Raw");

    /** Data -> Raw -> co2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "CO2: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.raw.co2);

    /** Data -> Raw -> co2ref */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "CO2REF: ");
    add_bool_to_string(sstr, Licor1.sysconfig.rs232.raw.co2ref);

    if ((Licor1.type == LICOR_LI840) || (Licor1.type == LICOR_LI850))
    {
        /** Data -> Raw -> h20 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        strcat(sstr, "H2O: ");
        add_bool_to_string(sstr, Licor1.sysconfig.rs232.raw.h2o);

        /** Data -> Raw -> h20ref */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        strcat(sstr, "H2OREF: ");
        add_bool_to_string(sstr, Licor1.sysconfig.rs232.raw.h2oref);
    }
}
void LI8x0_CreateString_Cfg(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Cfg */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
    strcat(sstr, "Cfg");

    /** Cfg -> heater */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Heater: ");
    add_bool_to_string(sstr, Licor1.sysconfig.cfg.heater);

    /** Cfg -> pcomp */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Pcomp: ");
    add_bool_to_string(sstr, Licor1.sysconfig.cfg.pcomp);

    /** Cfg -> filter */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%u", Licor1.sysconfig.cfg.filter);
    strcat(sstr, "Filter: ");
    strcat(sstr, temp2);

    /** Cfg -> outrate */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.cfg.outrate);
    strcat(sstr, "Outrate: ");
    strcat(sstr, temp2);

    /** Cfg -> bench */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%u", Licor1.sysconfig.cfg.bench);
    strcat(sstr, "Bench: ");
    strcat(sstr, temp2);

    /** Cfg -> span */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%u", Licor1.sysconfig.cfg.span);
    strcat(sstr, "Span: ");
    strcat(sstr, temp2);

    /** Cfg -> alarms */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Alarms");

    /** Cfg -> Alarms -> logic */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "Logic: ");
    if (Licor1.sysconfig.cfg.alarms.logic == TTL)
    {
        strcat(sstr, "TTL");
    }
    else
    {
        strcat(sstr, "SWG");
    }

    /** Cfg -> Alarms -> source */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "Source: ");
    if (Licor1.sysconfig.cfg.alarms.sources == CO2)
    {
        strcat(sstr, "CO2");
    }
    else
    {
        strcat(sstr, "H2O");
    }

    /** Cfg -> Alarms -> enabled */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "Enabled: ");
    add_bool_to_string(sstr, Licor1.sysconfig.cfg.alarms.enabled);

    /** Cfg -> Alarms -> high */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.high);
    strcat(sstr, "High: ");
    strcat(sstr, temp2);

    /** Cfg -> Alarms -> hdead */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.hdead);
    strcat(sstr, "Hdead: ");
    strcat(sstr, temp2);

    /** Cfg -> Alarms -> low */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.low);
    strcat(sstr, "Low: ");
    strcat(sstr, temp2);

    /** Cfg -> Alarms -> ldead */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.ldead);
    strcat(sstr, "Ldead: ");
    strcat(sstr, temp2);

    /** Cfg -> dacs */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "DACs");

    /** Cfg -> dacs -> range */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "Source: ");
    if (Licor1.sysconfig.cfg.dacs.range == R_2_5)
    {
        strcat(sstr, "2.5");
    }
    else
    {
        strcat(sstr, "5.0");
    }
    /** Crg -> dacs -> d1 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "D1: ");
    LI8x0_CreateSubstring_DAC(sstr, Licor1.sysconfig.cfg.dacs.d1, indentLevel + 2);

    /** Crg -> dacs -> d1_0 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.cfg.alarms.ldead);
    strcat(sstr, "Ldead: ");
    strcat(sstr, temp2);

    /** Crg -> dacs -> d1_f */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.ldead);
    strcat(sstr, "Ldead: ");
    strcat(sstr, temp2);

    /** Crg -> dacs -> d2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "D2: ");
    LI8x0_CreateSubstring_DAC(sstr, Licor1.sysconfig.cfg.dacs.d2, indentLevel + 2);

    /** Crg -> dacs -> d2_0 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%f#", Licor1.sysconfig.cfg.alarms.ldead);
    strcat(sstr, "Ldead: ");
    strcat(sstr, temp2);

    /** Crg -> dacs -> d2_f */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%E", Licor1.sysconfig.cfg.alarms.ldead);
    strcat(sstr, "Ldead: ");
    strcat(sstr, temp2);
}

void LI8x0_CreateSubstring_DAC(char *sstr, eLicorD_t D, uint8_t indentLevel)
{
    switch (D)
    {
    case LICOR_DAC_CO2:
        strcat(sstr, "co2");
        break;
    case LICOR_DAC_H2O:
        strcat(sstr, "h2o");
        break;
    case LICOR_DAC_H2ODEWPOINT:
        strcat(sstr, "h2odewpoint");
        break;
    case LICOR_DAC_CELLPRES:
        strcat(sstr, "cellpres");
        break;
    case LICOR_DAC_CELLTEMP:
        strcat(sstr, "celltemp");
        break;
    case LICOR_DAC_NONE:
        strcat(sstr, "none");
        break;
    default:
        break;
    }
}

void LI8x0_CreateString_Cal_NoIndent(char *sstr)
{
    char temp2[16];

    /** Cal -> co2lastzero */
    strcat(sstr, "CO2LastZero: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastzero);
    strcat(sstr, "\r\n");

    /** Cal -> co2kzero */
    sprintf(temp2, "%3.3E", Licor1.sysconfig.cal.co2kzero);
    strcat(sstr, "CO2kzero: ");
    strcat(sstr, temp2);
    strcat(sstr, "\r\n");

    /** Cal -> co2lastspan */
    strcat(sstr, "CO2LastSpan: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastspan);
    strcat(sstr, "\r\n");

    /** Cal -> co2lastspan2 */
    strcat(sstr, "CO2LastSpan2: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastspan2);
    strcat(sstr, "\r\n");

    /** Cal -> co2kspan */
    sprintf(temp2, "%3.3E", Licor1.sysconfig.cal.co2kspan);
    strcat(sstr, "CO2kspan: ");
    strcat(sstr, temp2);
    strcat(sstr, "\r\n");

    /** Cal -> co2kspan2 */
    sprintf(temp2, "%3.3E", Licor1.sysconfig.cal.co2kspan2);
    strcat(sstr, "CO2kspan2: ");
    strcat(sstr, temp2);

    return;
}

void LI8x0_CreateString_Cal(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Cal */
    if (indentLevel > 0)
    {
        add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
        strcat(sstr, "Cal");
    }

    /** Cal -> co2lastzero */

    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "CO2LastZero: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastzero);

    /** Cal -> co2kzero */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.8E", Licor1.sysconfig.cal.co2kzero);
    strcat(sstr, "CO2kzero: ");
    strcat(sstr, temp2);

    /** Cal -> co2lastspan */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "CO2LastSpan: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastspan);

    /** Cal -> co2lastspan2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "CO2LastSpan2: ");
    strcat(sstr, Licor1.sysconfig.cal.co2lastspan2);

    /** Cal -> co2kspan */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.8E", Licor1.sysconfig.cal.co2kspan);
    strcat(sstr, "CO2kspan: ");
    strcat(sstr, temp2);

    /** Cal -> co2kspan2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.8E", Licor1.sysconfig.cal.co2kspan2);
    strcat(sstr, "CO2kspan2: ");
    strcat(sstr, temp2);
}

void LI8x0_CreateString_Poly(char *sstr, uint8_t indentLevel)
{

    char temp2[16];

    /** Poly */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
    strcat(sstr, "Poly");

    /** Poly -> date */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Date: ");
    strcat(sstr, Licor1.sysconfig.poly.date);

    /** Poly -> bb */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.bb);
    strcat(sstr, "BB: ");
    strcat(sstr, temp2);

    /** Poly -> xs */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.xs);
    strcat(sstr, "XS: ");
    strcat(sstr, temp2);

    /** Poly -> reset */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Reset: ");
    add_bool_to_string(sstr, Licor1.sysconfig.poly.reset);

    /** Poly -> co2*/
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel + 1);
    strcat(sstr, "CO2");

    /** Poly -> co2 -> a1 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.co2.a1);
    strcat(sstr, "A1: ");
    strcat(sstr, temp2);

    /** Poly -> co2 -> a2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.co2.a2);
    strcat(sstr, "A2: ");
    strcat(sstr, temp2);

    /** Poly -> co2 -> a3 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.co2.a3);
    strcat(sstr, "A3: ");
    strcat(sstr, temp2);

    /** Poly -> co2 -> a4 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.co2.a4);
    strcat(sstr, "A4: ");
    strcat(sstr, temp2);

    if ((Licor1.type == LICOR_LI840) || (Licor1.type == LICOR_LI850))
    {
        /** Poly -> h2o*/
        add_cr_lf_and_tab_to_string(sstr, 2, indentLevel + 1);
        strcat(sstr, "H2O");

        /** Poly -> h2o -> a1 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.h2o.a1);
        strcat(sstr, "A1: ");
        strcat(sstr, temp2);

        /** Poly -> h2o -> a2 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.h2o.a2);
        strcat(sstr, "A2: ");
        strcat(sstr, temp2);

        /** Poly -> h2o -> a3 */
        add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
        sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.h2o.a3);
        strcat(sstr, "A3: ");
        strcat(sstr, temp2);
    }

    /** Poly -> press */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel + 1);
    strcat(sstr, "Press");

    /** Poly -> press -> a1 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.pres.a0);
    strcat(sstr, "A1: ");
    strcat(sstr, temp2);

    /** Poly -> press -> a2 */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.poly.pres.a1);
    strcat(sstr, "A2: ");
    strcat(sstr, temp2);
}

void LI8x0_CreateString_Pump(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Pump */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
    strcat(sstr, "Pump");

    /** Pump -> enabled*/
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    strcat(sstr, "Enabled: ");
    add_bool_to_string(sstr, Licor1.sysconfig.pump.enabled);

    /** Pump -> time */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%lu", Licor1.sysconfig.pump.time);
    strcat(sstr, "Time: ");
    strcat(sstr, temp2);

    /** Pump -> drive */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%u", Licor1.sysconfig.pump.drive);
    strcat(sstr, "Drive: ");
    strcat(sstr, temp2);

    /** Pump -> status */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%u", Licor1.sysconfig.pump.status);
    strcat(sstr, "Status: ");
    strcat(sstr, temp2);
}

void LI8x0_CreateString_Source(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Source */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
    strcat(sstr, "Source");

    /** Source -> time*/
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 1);
    sprintf(temp2, "%lu", Licor1.sysconfig.source.time);
    strcat(sstr, "Time: ");
    strcat(sstr, temp2);
}
void LI8x0_CreateString_Tech(char *sstr, uint8_t indentLevel)
{
    char temp2[16];

    /** Tech */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel);
    strcat(sstr, "Tech");

    /** Tech -> pump */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel + 1);
    strcat(sstr, "Pump");

    /** Tech -> pump -> model */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    strcat(sstr, "Model: ");
    strcat(sstr, Licor1.sysconfig.tech.pump.model);

    /** Tech -> pump -> ki */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.tech.pump.ki);
    strcat(sstr, "Ki: ");
    strcat(sstr, temp2);

    /** Tech -> pump -> kp */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.tech.pump.kp);
    strcat(sstr, "Kp: ");
    strcat(sstr, temp2);

    /** Tech -> pump -> kd */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.tech.pump.kd);
    strcat(sstr, "Kd: ");
    strcat(sstr, temp2);

    /** Tech -> pump -> offset */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.tech.pump.offset);
    strcat(sstr, "Offset: ");
    strcat(sstr, temp2);

    /** Tech -> pump -> gain */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%lu", Licor1.sysconfig.tech.pump.gain);
    strcat(sstr, "Gain: ");
    strcat(sstr, temp2);

    /** Tech -> pump -> flow */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.tech.pump.flow);
    strcat(sstr, "Flow: ");
    strcat(sstr, temp2);

    /** Tech -> source */
    add_cr_lf_and_tab_to_string(sstr, 2, indentLevel + 1);
    strcat(sstr, "Source");

    /** Tech -> source 0> voltage */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel + 2);
    sprintf(temp2, "%3.3E", Licor1.sysconfig.tech.source.voltage);
    strcat(sstr, "Voltage: ");
    strcat(sstr, temp2);
}
void LI8x0_CreateString_Serial(char *sstr, uint8_t indentLevel)
{
    /** SerialNum */
    add_cr_lf_and_tab_to_string(sstr, 1, indentLevel);
    strcat(sstr, "SerialNum: ");
    strcat(sstr, Licor1.sysconfig.serialnum);
}

void LI8x0_SendString(char *sstr, uint16_t strlen)
{
    /** Used to send commands during Passthrough mode */
    BSP_UART_puts(&Licor1.UART, sstr, strlen);
}

STATIC void add_bool_to_string(char *sstr, bool val)
{
    if (val == true)
    {
        strcat(sstr, "True");
    }
    else
    {
        strcat(sstr, "False");
    }
}
STATIC uint16_t add_cr_lf_and_tab_to_string(char *sstr, uint8_t numCrLf, uint8_t numTabs)
{
    uint16_t len;
    uint8_t i = 0;

    for (i = 0; i < numCrLf; i++)
    {
        strcat(sstr, "\r\n");
        len += 2;
    }

    for (i = 0; i < numTabs; i++)
    {
        strcat(sstr, "\t");
        len++;
    }
    return len;
}

/************************************************************************
 *					   Static Functions
 ************************************************************************/
/** @brief Test for valid level combos
 *
 * Tests the XML structure to see if the Lvl1-4 combo is valid
 *
 * @brief *XML Pointer to XML Structure to test
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ValidateXML(sLicorXML_t *XML)
{
    eLicorStatus_t result = LICOR_FAIL;

    if ((XML->LVL1 == LICOR_XML_LVL1_820) ||
        (XML->LVL1 == LICOR_XML_LVL1_830))
    {

        if (LI8x0_Validate_NoH2O(XML) != LICOR_OK)
        {
            Error(ERROR_MSG_00800800, LOG_Priority_Med);
            return result;
        }
    }

#ifndef LICOR_PUMP
    if (LI8x0_Validate_NoPump(XML) == LICOR_OK)
    {
    }
    else
    {
        Error(ERROR_MSG_00800080, LOG_Priority_Med);
    }
#endif

    switch (XML->LVL1)
    {
    case LICOR_XML_LVL1_820:
    case LICOR_XML_LVL1_830:
    case LICOR_XML_LVL1_840:
    case LICOR_XML_LVL1_850:
        if (LI8x0_ValidXML_Level2(XML) == LICOR_OK)
        {
            if (LI8x0_ValidXML_Level3(XML) == LICOR_OK)
            {
                result = LICOR_OK;
            }
        }

        break;
    case LICOR_XML_LVL1_NONE:
        if ((XML->LVL2 == LICOR_XML_LVL2_NONE) &&
            (XML->LVL3 == LICOR_XML_LVL3_NONE) &&
            (XML->LVL4 == LICOR_XML_LVL4_NONE))
        {
            result = LICOR_OK;
        }
        else
        {
            Error(ERROR_MSG_00800020, LOG_Priority_Med);
            result = LICOR_FAIL;
        }
        break;
    default:
        Error(ERROR_MSG_00800002, LOG_Priority_Med);
        break;
    }

    return result;
}

/** @brief Test for No Pump Command
 *
 * Tests the XML structure to see there is no Pump Commands
 *
 * @brief *XML Pointer to XML Structure to test
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Validate_NoPump(sLicorXML_t *XML)
{
    eLicorStatus_t result = LICOR_FAIL;

    if ((XML->LVL2 == LICOR_XML_LVL2_PUMP) ||
        (XML->LVL3 == LICOR_XML_LVL3_PUMP))
    {
        result = LICOR_FAIL;
    }
    else
    {
        result = LICOR_OK;
    }
    return result;
}

/** @brief Test for No H2o
 *
 * Tests the XML structure to see there is no H2O Commands
 *
 * @brief *XML Pointer to XML Structure to test
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Validate_NoH2O(sLicorXML_t *XML)
{
    eLicorStatus_t result = LICOR_FAIL;

    if (
        (XML->LVL3 == LICOR_XML_LVL3_H2O) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OABS) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2ODEW) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OZERO) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OSPAN) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OSPAN2) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OLASTZERO) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OLASTSPAN) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OLASTSPAN2) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OKSPAN) ||
        (XML->LVL3 == LICOR_XML_LVL3_H2OKSPAN2))
    {
        result = LICOR_FAIL;
    }
    else
    {
        result = LICOR_OK;
    }
    return result;
}

/** @brief Test for valid level 2 combos
 *
 * Tests the XML structure at level 2 for valid combos
 *
 * @param *XML Pointer to XML Structure to test
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ValidXML_Level2(sLicorXML_t *XML)
{
    eLicorStatus_t result = LICOR_FAIL;
    /** Check Level 3 data */
    if (LI8x0_ValidXML_Level3(XML) == LICOR_OK)
    {
        switch (XML->LVL2)
        {
        case LICOR_XML_LVL2_NONE:
        case LICOR_XML_LVL2_ACK:
        case LICOR_XML_LVL2_VER:
        case LICOR_XML_LVL2_ERROR:
        case LICOR_XML_LVL2_SER:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) &&
                (XML->LVL4 == LICOR_XML_LVL4_NONE))
            {
                result = LICOR_OK;
            }
            else
            {

                result = LICOR_FAIL;
            }
            break;
        case LICOR_XML_LVL2_AUXDATA:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_PCA) ||
                (XML->LVL3 == LICOR_XML_LVL3_PHA) ||
                (XML->LVL3 == LICOR_XML_LVL3_BB) ||
                (XML->LVL3 == LICOR_XML_LVL3_PSI))
            {
                result = LICOR_OK;
            }
            else
            {

                result = LICOR_FAIL;
            }

            break;

        /** Cases with Level 3 and 4 data */
        case LICOR_XML_LVL2_DATA:
        case LICOR_XML_LVL2_RS232:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_FLOWRATE) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2O) ||
                (XML->LVL3 == LICOR_XML_LVL3_CELLTEMP) ||
                (XML->LVL3 == LICOR_XML_LVL3_CELLPRES) ||
                (XML->LVL3 == LICOR_XML_LVL3_IVOLT) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2ABS) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OABS) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2ODEW) ||
                (XML->LVL3 == LICOR_XML_LVL3_RAW) ||
                (XML->LVL3 == LICOR_XML_LVL3_ECHO) ||
                (XML->LVL3 == LICOR_XML_LVL3_STRIP))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;
        case LICOR_XML_LVL2_CFG:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_HEATER) ||
                (XML->LVL3 == LICOR_XML_LVL3_PCOMP) ||
                (XML->LVL3 == LICOR_XML_LVL3_FILTER) ||
                (XML->LVL3 == LICOR_XML_LVL3_OUTRATE) ||
                (XML->LVL3 == LICOR_XML_LVL3_ALARMS) ||
                (XML->LVL3 == LICOR_XML_LVL3_BENCH) ||
                (XML->LVL3 == LICOR_XML_LVL3_SPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_DACS))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }

            break;
        case LICOR_XML_LVL2_CAL:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2ZERO) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2SPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2SPAN2) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2LASTZERO) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2KZERO) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2LASTSPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2LASTSPAN2) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2KSPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2KSPAN2) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OLASTZERO) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OLASTSPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OLASTSPAN2) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OKSPAN) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2OKSPAN2))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;
        case LICOR_XML_LVL2_POLY:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_DATE) ||
                (XML->LVL3 == LICOR_XML_LVL3_BB) ||
                (XML->LVL3 == LICOR_XML_LVL3_XS) ||
                (XML->LVL3 == LICOR_XML_LVL3_RESET) ||
                (XML->LVL3 == LICOR_XML_LVL3_CO2) ||
                (XML->LVL3 == LICOR_XML_LVL3_H2O) ||
                (XML->LVL3 == LICOR_XML_LVL3_PRESS))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;
        case LICOR_XML_LVL2_PUMP:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_ENABLED) ||
                (XML->LVL3 == LICOR_XML_LVL3_TIME) ||
                (XML->LVL3 == LICOR_XML_LVL3_DRIVE) ||
                (XML->LVL3 == LICOR_XML_LVL3_STATUS))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;

        case LICOR_XML_LVL2_SOURCE:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_TIME))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;
        case LICOR_XML_LVL2_TECH:
            if ((XML->LVL3 == LICOR_XML_LVL3_NONE) ||
                (XML->LVL3 == LICOR_XML_LVL3_PUMP) ||
                (XML->LVL3 == LICOR_XML_LVL3_SOURCE) ||
                (XML->LVL3 == LICOR_XML_LVL3_ENABLED))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
            break;
        default:

            result = LICOR_FAIL;
            break;
        }
    }
    else
    {
        result = LICOR_FAIL;
    }

    if (result == LICOR_FAIL)
    {
#ifdef UNIT_TESTING
#ifdef DEBUG_PRINT_ERRORS
        printf("\nLevel 2: %s", ERROR_MSG_00800080);
#endif
#endif
        Error(ERROR_MSG_00800080, LOG_Priority_Med);
    }

    return result;
}

/** @brief Test for valid level 2 combos
 *
 * Tests the XML structure at level 2 for valid combos
 *
 * @param *XML Pointer to XML Structure to test
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ValidXML_Level3(sLicorXML_t *XML)
{
    eLicorStatus_t result = LICOR_FAIL;

    switch (XML->LVL3)
    {
    case LICOR_XML_LVL3_NONE:
    case LICOR_XML_LVL3_FLOWRATE:
    case LICOR_XML_LVL3_CELLTEMP:
    case LICOR_XML_LVL3_CELLPRES:
    case LICOR_XML_LVL3_CO2ABS:
    case LICOR_XML_LVL3_CO2REF:
    case LICOR_XML_LVL3_H2OABS:
    case LICOR_XML_LVL3_H2ODEW:
    case LICOR_XML_LVL3_IVOLT:
    case LICOR_XML_LVL3_PCA:
    case LICOR_XML_LVL3_PHA:
    case LICOR_XML_LVL3_BB:
    case LICOR_XML_LVL3_PSI:
    case LICOR_XML_LVL3_ECHO:
    case LICOR_XML_LVL3_STRIP:
    case LICOR_XML_LVL3_HEATER:
    case LICOR_XML_LVL3_PCOMP:
    case LICOR_XML_LVL3_FILTER:
    case LICOR_XML_LVL3_OUTRATE:
    case LICOR_XML_LVL3_BENCH:
    case LICOR_XML_LVL3_SPAN:
    case LICOR_XML_LVL3_CO2ZERO:
    case LICOR_XML_LVL3_CO2SPAN:
    case LICOR_XML_LVL3_CO2SPAN2:
    case LICOR_XML_LVL3_CO2LASTZERO:
    case LICOR_XML_LVL3_CO2KZERO:
    case LICOR_XML_LVL3_CO2LASTSPAN:
    case LICOR_XML_LVL3_CO2LASTSPAN2:
    case LICOR_XML_LVL3_CO2KSPAN:
    case LICOR_XML_LVL3_CO2KSPAN2:
    case LICOR_XML_LVL3_H2OZERO:
    case LICOR_XML_LVL3_H2OSPAN:
    case LICOR_XML_LVL3_H2OSPAN2:
    case LICOR_XML_LVL3_H2OLASTZERO:
    case LICOR_XML_LVL3_H2OLASTSPAN:
    case LICOR_XML_LVL3_H2OLASTSPAN2:
    case LICOR_XML_LVL3_H2OKSPAN:
    case LICOR_XML_LVL3_H2OKSPAN2:
    case LICOR_XML_LVL3_DATE:
    case LICOR_XML_LVL3_XS:
    case LICOR_XML_LVL3_RESET:
    case LICOR_XML_LVL3_SERIALNUM:
    case LICOR_XML_LVL3_ENABLED:
    case LICOR_XML_LVL3_TIME:
    case LICOR_XML_LVL3_DRIVE:
    case LICOR_XML_LVL3_STATUS:
        if (XML->LVL4 == LICOR_XML_LVL4_NONE)
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;

    case LICOR_XML_LVL3_RAW:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_CO2) ||
            (XML->LVL4 == LICOR_XML_LVL4_CO2REF) ||
            (XML->LVL4 == LICOR_XML_LVL4_H2O) ||
            (XML->LVL4 == LICOR_XML_LVL4_H2OREF))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }

        break;
    case LICOR_XML_LVL3_ALARMS:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_LOGIC) ||
            (XML->LVL4 == LICOR_XML_LVL4_SOURCE) ||
            (XML->LVL4 == LICOR_XML_LVL4_ENABLED) ||
            (XML->LVL4 == LICOR_XML_LVL4_HIGH) ||
            (XML->LVL4 == LICOR_XML_LVL4_HDEAD) ||
            (XML->LVL4 == LICOR_XML_LVL4_LOW) ||
            (XML->LVL4 == LICOR_XML_LVL4_LDEAD))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    case LICOR_XML_LVL3_DACS:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_RANGE) ||
            (XML->LVL4 == LICOR_XML_LVL4_D1) ||
            (XML->LVL4 == LICOR_XML_LVL4_D1_0) ||
            (XML->LVL4 == LICOR_XML_LVL4_D1_F) ||
            (XML->LVL4 == LICOR_XML_LVL4_D2) ||
            (XML->LVL4 == LICOR_XML_LVL4_D2_0) ||
            (XML->LVL4 == LICOR_XML_LVL4_D2_F))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    case LICOR_XML_LVL3_PRESS:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_A0) ||
            (XML->LVL4 == LICOR_XML_LVL4_A1))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    case LICOR_XML_LVL3_PUMP:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_MODEL) ||
            (XML->LVL4 == LICOR_XML_LVL4_KI) ||
            (XML->LVL4 == LICOR_XML_LVL4_KP) ||
            (XML->LVL4 == LICOR_XML_LVL4_KD) ||
            (XML->LVL4 == LICOR_XML_LVL4_OFFSET) ||
            (XML->LVL4 == LICOR_XML_LVL4_GAIN) ||
            (XML->LVL4 == LICOR_XML_LVL4_FLOW))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    case LICOR_XML_LVL3_SOURCE:
        if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
            (XML->LVL4 == LICOR_XML_LVL4_VOLTAGE))
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;

    /** Special Cases */
    case LICOR_XML_LVL3_CO2:
        if ((XML->LVL2 == LICOR_XML_LVL2_DATA) || (XML->LVL2 == LICOR_XML_LVL2_RS232))
        {
            if ((XML->LVL4 == LICOR_XML_LVL4_NONE))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
        }
        else if (XML->LVL2 == LICOR_XML_LVL2_POLY)
        {
            if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
                (XML->LVL4 == LICOR_XML_LVL4_A1) ||
                (XML->LVL4 == LICOR_XML_LVL4_A2) ||
                (XML->LVL4 == LICOR_XML_LVL4_A3) ||
                (XML->LVL4 == LICOR_XML_LVL4_A4))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    case LICOR_XML_LVL3_H2O:
        if ((XML->LVL2 == LICOR_XML_LVL2_DATA) || (XML->LVL2 == LICOR_XML_LVL2_RS232))
        {
            if ((XML->LVL4 == LICOR_XML_LVL4_NONE))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
        }
        else if (XML->LVL2 == LICOR_XML_LVL2_POLY)
        {
            if ((XML->LVL4 == LICOR_XML_LVL4_NONE) ||
                (XML->LVL4 == LICOR_XML_LVL4_A1) ||
                (XML->LVL4 == LICOR_XML_LVL4_A2) ||
                (XML->LVL4 == LICOR_XML_LVL4_A3))
            {
                result = LICOR_OK;
            }
            else
            {
                result = LICOR_FAIL;
            }
        }
        else
        {
            result = LICOR_FAIL;
        }
        break;
    default:
        result = LICOR_FAIL;
        break;
    }

    if (result == LICOR_FAIL)
    {
#ifdef UNIT_TESTING
#ifdef DEBUG_PRINT_ERRORS
        printf("\nLevel 3: %s", ERROR_MSG_00800200);
#endif
#endif
        Error(ERROR_MSG_00800200, LOG_Priority_Med);
    }

    return result;
}

/** @brief Create Level 1 Tag
 *
 * Creates the Level 1 (Sensor Type) XML Tag
 *
 * @param LVL Level 1 selection
 * @param *sstr Pointer to XML Start Tag string
 * @param *sLen Pointer to XML Start Tag string length
 * @param *estr Pointer to XML End Tag string
 * @param *eLen Pointer to XML End Tag length
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Level1Str(eLicorXMLLvl1_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen)
{
    eLicorStatus_t result = LICOR_OK;
    char temp[5];
    memset(temp, 0, 5);
    /** Prep the selected tag */
    switch (LVL)
    {
    case LICOR_XML_LVL1_820:
        strcpy(temp, "820>");
        break;
    case LICOR_XML_LVL1_830:
        strcpy(temp, "830>");
        break;
    case LICOR_XML_LVL1_840:
        strcpy(temp, "840>");
        break;
    case LICOR_XML_LVL1_850:
        strcpy(temp, "850>");
        break;
    case LICOR_XML_LVL1_NONE:
        break;
    default:
        Error(ERROR_MSG_00800002, LOG_Priority_Med);
        result = LICOR_FAIL;
        break;
    }

    /** Generate The Level 4 Tag */
    if ((LVL != LICOR_XML_LVL1_NONE) && (result != LICOR_FAIL))
    {
        strcpy(sstr, "<LI");
        strcat(sstr, temp);
        strcpy(estr, "</LI");
        strcat(estr, temp);
        *slen = 7;
        *elen = 8;
    }

    return result;
}

/** @brief Create Level 2 Tag
 *
 * Creates the Level 2 (Sensor Type) XML Tag
 *
 * @param LVL Level 2 selection
 * @param *sstr Pointer to XML Start Tag string
 * @param *sLen Pointer to XML Start Tag string length
 * @param *estr Pointer to XML End Tag string
 * @param *eLen Pointer to XML End Tag length
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Level2Str(eLicorXMLLvl2_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen)
{
    eLicorStatus_t result = LICOR_OK;
    char temp[16];
    char etemp[64];
    memset(temp, 0, 16);
    memset(etemp, 0, 64);
    strcpy(etemp, estr);

    /** Prep the selected tag */
    switch (LVL)
    {

    case LICOR_XML_LVL2_NONE:
        break;
    case LICOR_XML_LVL2_DATA:
        strcpy(temp, "DATA>");
        break;
    case LICOR_XML_LVL2_AUXDATA:
        strcpy(temp, "AUXDATA>");
        break;
    case LICOR_XML_LVL2_RS232:
        strcpy(temp, "RS232>");
        break;
    case LICOR_XML_LVL2_CFG:
        strcpy(temp, "CFG>");
        break;
    case LICOR_XML_LVL2_CAL:
        strcpy(temp, "CAL>");
        break;
    case LICOR_XML_LVL2_POLY:
        strcpy(temp, "POLY>");
        break;
    case LICOR_XML_LVL2_PUMP:
        strcpy(temp, "PUMP>");
        break;
    case LICOR_XML_LVL2_SOURCE:
        strcpy(temp, "SOURCE>");
        break;
    case LICOR_XML_LVL2_TECH:
        strcpy(temp, "TECH>");
        break;
    case LICOR_XML_LVL2_SER:
        strcpy(temp, "SERIALNUM>");
        break;
    case LICOR_XML_LVL2_VER:
        strcpy(temp, "VER>");
        break;
    case LICOR_XML_LVL2_ACK:
        strcpy(temp, "ACK>");
        break;
    case LICOR_XML_LVL2_ERROR:
        strcpy(temp, "ERROR>");
        break;
    default:
        Error(ERROR_MSG_00800004, LOG_Priority_Med);
        result = LICOR_FAIL;
        break;
    }

    /** Generate The Level 4 Tag */
    if ((LVL != LICOR_XML_LVL2_NONE) && (result != LICOR_FAIL))
    {
        strcat(sstr, "<");
        strcpy(estr, "</");
        strcat(sstr, temp);
        strcat(estr, temp);
        strcat(estr, etemp);

        *slen = strlen(sstr);
        *elen = strlen(estr);
    }

    return result;
}

/** @brief Create Level 3 Tag
 *
 * Creates the Level 3 (Sensor Type) XML Tag
 *
 * @param LVL Level 3 selection
 * @param *sstr Pointer to XML Start Tag string
 * @param *sLen Pointer to XML Start Tag string length
 * @param *estr Pointer to XML End Tag string
 * @param *eLen Pointer to XML End Tag length
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Level3Str(eLicorXMLLvl3_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen)
{
    eLicorStatus_t result = LICOR_OK;
    char temp[16];
    char etemp[64];
    char dstr[11];
    memset(temp, 0, 16);
    memset(etemp, 0, 64);
    memset(dstr, 0, 11);
    strcpy(etemp, estr);

    /** Prep the selected tag */
    switch (LVL)
    {

    case LICOR_XML_LVL3_NONE:
        break;
    case LICOR_XML_LVL3_FLOWRATE:
        strcpy(temp, "FLOWRATE>");
        break;
    case LICOR_XML_LVL3_CELLTEMP:
        strcpy(temp, "CELLTEMP>");
        break;
    case LICOR_XML_LVL3_CELLPRES:
        strcpy(temp, "CELLPRES>");
        break;
    case LICOR_XML_LVL3_CO2:
        strcpy(temp, "CO2>");
        break;
    case LICOR_XML_LVL3_CO2ABS:
        strcpy(temp, "CO2ABS>");
        break;
    case LICOR_XML_LVL3_CO2REF:
        strcpy(temp, "CO2REF>");
        break;
    case LICOR_XML_LVL3_H2O:
        strcpy(temp, "H2O>");
        break;
    case LICOR_XML_LVL3_H2OABS:
        strcpy(temp, "H2OABS>");
        break;
    case LICOR_XML_LVL3_H2ODEW:
        strcpy(temp, "H2ODEWPOINT>");
        break;
    case LICOR_XML_LVL3_IVOLT:
        strcpy(temp, "IVOLT>");
        break;
    case LICOR_XML_LVL3_RAW:
        strcpy(temp, "RAW>");
        break;
    case LICOR_XML_LVL3_PCA:
        strcpy(temp, "PCA>");
        break;
    case LICOR_XML_LVL3_PHA:
        strcpy(temp, "PHA>");
        break;
    case LICOR_XML_LVL3_BB:
        strcpy(temp, "BB>");
        break;
    case LICOR_XML_LVL3_PSI:
        strcpy(temp, "PSI>");
        break;
    case LICOR_XML_LVL3_ECHO:
        strcpy(temp, "ECHO>");
        break;
    case LICOR_XML_LVL3_STRIP:
        strcpy(temp, "STRIP>");
        break;
    case LICOR_XML_LVL3_HEATER:
        strcpy(temp, "HEATER>");
        break;
    case LICOR_XML_LVL3_PCOMP:
        strcpy(temp, "PCOMP>");
        break;
    case LICOR_XML_LVL3_FILTER:
        strcpy(temp, "FILTER>");
        break;
    case LICOR_XML_LVL3_OUTRATE:
        strcpy(temp, "OUTRATE>");
        break;
    case LICOR_XML_LVL3_ALARMS:
        strcpy(temp, "ALARMS>");
        break;
    case LICOR_XML_LVL3_BENCH:
        strcpy(temp, "BENCH>");
        break;
    case LICOR_XML_LVL3_SPAN:
        strcpy(temp, "SPAN>");
        break;
    case LICOR_XML_LVL3_DACS:
        strcpy(temp, "DACS>");
        break;
    case LICOR_XML_LVL3_CO2ZERO:
        strcpy(temp, "CO2ZERO>");
        break;
    case LICOR_XML_LVL3_CO2SPAN:
        strcpy(temp, "CO2SPAN>");
        break;
    case LICOR_XML_LVL3_CO2SPAN2:
        strcpy(temp, "CO2SPAN2>");
        break;
    case LICOR_XML_LVL3_CO2LASTZERO:
        strcpy(temp, "CO2LASTZERO>");
        break;
    case LICOR_XML_LVL3_CO2KZERO:
        strcpy(temp, "CO2KZERO>");
        break;
    case LICOR_XML_LVL3_CO2LASTSPAN:
        strcpy(temp, "CO2LASTSPAN>");
        break;
    case LICOR_XML_LVL3_CO2LASTSPAN2:
        strcpy(temp, "CO2LASTSPAN2>");
        break;
    case LICOR_XML_LVL3_CO2KSPAN:
        strcpy(temp, "CO2KSPAN>");
        break;
    case LICOR_XML_LVL3_CO2KSPAN2:
        strcpy(temp, "CO2KSPAN2>");
        break;
    case LICOR_XML_LVL3_H2OZERO:
        strcpy(temp, "H2OZERO>");
        break;
    case LICOR_XML_LVL3_H2OSPAN:
        strcpy(temp, "H2OSPAN>");
        break;
    case LICOR_XML_LVL3_H2OSPAN2:
        strcpy(temp, "H2OSPAN2>");
        break;
    case LICOR_XML_LVL3_H2OLASTZERO:
        strcpy(temp, "H2OLASTZERO>");
        break;
    case LICOR_XML_LVL3_H2OLASTSPAN:
        strcpy(temp, "H2OLASTSPAN>");
        break;
    case LICOR_XML_LVL3_H2OLASTSPAN2:
        strcpy(temp, "H2OLASTSPAN2>");
        break;
    case LICOR_XML_LVL3_H2OKSPAN:
        strcpy(temp, "H2OKSPAN>");
        break;
    case LICOR_XML_LVL3_H2OKSPAN2:
        strcpy(temp, "H2OKSPAN2>");
        break;
    case LICOR_XML_LVL3_DATE:
        strcpy(temp, "DATA>");
        break;
    case LICOR_XML_LVL3_XS:
        strcpy(temp, "XS>");
        break;
    case LICOR_XML_LVL3_RESET:
        strcpy(temp, "RESET>");
        break;
    case LICOR_XML_LVL3_PRESS:
        strcpy(temp, "PRESS>");
        break;
    case LICOR_XML_LVL3_ENABLED:
        strcpy(temp, "ENABLED>");
        break;
    case LICOR_XML_LVL3_TIME:
        strcpy(temp, "TIME>");
        break;
    case LICOR_XML_LVL3_DRIVE:
        strcpy(temp, "DRIVE>");
        break;
    case LICOR_XML_LVL3_STATUS:
        strcpy(temp, "STATUS>");
        break;
    case LICOR_XML_LVL3_PUMP:
        strcpy(temp, "PUMP>");
        break;
    case LICOR_XML_LVL3_SOURCE:
        strcpy(temp, "SOURCE>");
        break;
    case LICOR_XML_LVL3_SERIALNUM:
        strcpy(temp, "SERIALNUM>");
        break;

    default:
        Error(ERROR_MSG_00800008, LOG_Priority_Med);
        result = LICOR_FAIL;
        break;
    }

    /** Generate The Level 3 Tag */
    if ((LVL != LICOR_XML_LVL3_NONE) && (result != LICOR_FAIL))
    {
        if ((LVL == LICOR_XML_LVL3_CO2ZERO) || (LVL == LICOR_XML_LVL3_CO2SPAN) ||
            (LVL == LICOR_XML_LVL3_CO2SPAN2) || (LVL == LICOR_XML_LVL3_H2OZERO) ||
            (LVL == LICOR_XML_LVL3_H2OSPAN) || (LVL == LICOR_XML_LVL3_H2OSPAN2))
        {
            /** Grab date */
            AM08X5_GetTimestamp(dstr, DS_DD_MMM_YYYY, TS_TIME_NONE);

            strcat(sstr, "<DATE>");
            strcat(sstr, dstr);
            strcat(sstr, "</DATE>");
        }

        strcat(sstr, "<");
        strcpy(estr, "</");

        strcat(sstr, temp);
        strcat(estr, temp);
        strcat(estr, etemp);

        *slen = strlen(sstr);
        *elen = strlen(estr);
    }

    return result;
}

/** @brief Create Level 4 Tag
 *
 * Creates the Level 4 (Sensor Type) XML Tag
 *
 * @param LVL Level 4 selection
 * @param *sstr Pointer to XML Start Tag string
 * @param *sLen Pointer to XML Start Tag string length
 * @param *estr Pointer to XML End Tag string
 * @param *eLen Pointer to XML End Tag length
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_Level4Str(eLicorXMLLvl4_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen)
{

    eLicorStatus_t result = LICOR_OK;
    char temp[16];
    char etemp[64];
    memset(temp, 0, 16);
    memset(etemp, 0, 64);
    strcpy(etemp, estr);

    /** Prep the selected tag */
    switch (LVL)
    {
    case LICOR_XML_LVL4_NONE:
        break;
    case LICOR_XML_LVL4_CO2:
        strcpy(temp, "CO2>");
        break;
    case LICOR_XML_LVL4_CO2REF:
        strcpy(temp, "CO2REF>");
        break;
    case LICOR_XML_LVL4_H2O:
        strcpy(temp, "H2O>");
        break;
    case LICOR_XML_LVL4_H2OREF:
        strcpy(temp, "H2OREF>");
        break;
    case LICOR_XML_LVL4_LOGIC:
        strcpy(temp, "LOGIC>");
        break;
    case LICOR_XML_LVL4_SOURCE:
        strcpy(temp, "SOURCE>");
        break;
    case LICOR_XML_LVL4_ENABLED:
        strcpy(temp, "ENABLED>");
        break;
    case LICOR_XML_LVL4_HIGH:
        strcpy(temp, "HIGH>");
        break;
    case LICOR_XML_LVL4_HDEAD:
        strcpy(temp, "HDEAD>");
        break;
    case LICOR_XML_LVL4_LOW:
        strcpy(temp, "LOW>");
        break;
    case LICOR_XML_LVL4_LDEAD:
        strcpy(temp, "LDEAD>");
        break;
    case LICOR_XML_LVL4_RANGE:
        strcpy(temp, "RANGE>");
        break;
    case LICOR_XML_LVL4_D1:
        strcpy(temp, "D1>");
        break;
    case LICOR_XML_LVL4_D1_0:
        strcpy(temp, "D1_0>");
        break;
    case LICOR_XML_LVL4_D1_F:
        strcpy(temp, "D1_F>");
        break;
    case LICOR_XML_LVL4_D2:
        strcpy(temp, "D2>");
        break;
    case LICOR_XML_LVL4_D2_0:
        strcpy(temp, "D2_0>");
        break;
    case LICOR_XML_LVL4_D2_F:
        strcpy(temp, "D2_F>");
        break;
    case LICOR_XML_LVL4_A1:
        strcpy(temp, "A1>");
        break;
    case LICOR_XML_LVL4_A2:
        strcpy(temp, "A2>");
        break;
    case LICOR_XML_LVL4_A3:
        strcpy(temp, "A3>");
        break;
    case LICOR_XML_LVL4_A4:
        strcpy(temp, "A4>");
        break;
    case LICOR_XML_LVL4_MODEL:
        strcpy(temp, "MODEL>");
        break;
    case LICOR_XML_LVL4_KI:
        strcpy(temp, "KI>");
        break;
    case LICOR_XML_LVL4_KP:
        strcpy(temp, "KP>");
        break;
    case LICOR_XML_LVL4_KD:
        strcpy(temp, "KD>");
        break;
    case LICOR_XML_LVL4_OFFSET:
        strcpy(temp, "OFFSET>");
        break;
    case LICOR_XML_LVL4_GAIN:
        strcpy(temp, "GAIN>");
        break;
    case LICOR_XML_LVL4_FLOW:
        strcpy(temp, "FLOW>");
        break;
    case LICOR_XML_LVL4_VOLTAGE:
        strcpy(temp, "VOLTAGE>");
        break;
    default:
        Error(ERROR_MSG_00800010, LOG_Priority_Med);
        result = LICOR_FAIL;
        break;
    }

    /** Generate The Level 4 Tag */
    if ((LVL != LICOR_XML_LVL4_NONE) && (result != LICOR_FAIL))
    {
        strcat(sstr, "<");
        strcpy(estr, "</");
        strcat(sstr, temp);
        strcat(estr, temp);
        strcat(estr, etemp);
        *slen = strlen(sstr);
        *elen = strlen(estr);
    }

    return result;
}

/** @brief Check response for ACK
 *
 * Checks the response string for a valid ACK
 *
 * @param *XML Pointer to XML Struct
 * @param *sstr Received String
 * @param slen Received string length
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_IsAck(sLicorXML_t *XML,
                                  char *sstr,
                                  uint16_t slen)
{
    eLicorStatus_t result = LICOR_FAIL;
    char head[18];
    char tail[18];
    char ack[36] = "<ACK>TRUE";
    if (slen > 0)
    {
        memset(head, 0, 18);
        memset(tail, 0, 18);

        /** Create a valid ACK for this system */
        uint8_t i;
        char *tstr = sstr;
        for (i = 0; i < slen; i++)
        {
            *tstr = toupper((char)*tstr);
            tstr++;
        }

        /** Compare the generated ACk to the send message */
        char *tt = strstr(sstr, ack);
        if (tt != NULL)
        {
            result = LICOR_OK;
        }
        else
        {
            result = LICOR_FAIL;
        }
    }
    return result;
}

/** @brief Parse a RX String
 *
 * Parses the RX String into a Licor Tag structure
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseString(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_ack[] = "ACK";
    const char str_ver[] = "VER";
    const char str_error[] = "ERROR";
    const char str_data[] = "DATA";
    const char str_auxdata[] = "AUXDATA";
    const char str_rs232[] = "RS232";
    const char str_cfg[] = "CFG";
    const char str_cal[] = "CAL";
    const char str_poly[] = "POLY";
    const char str_pump[] = "PUMP";
    const char str_source[] = "SOURCE";
    const char str_tech[] = "TECH";
    const char str_sernum[] = "SERIALNUM";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    uint8_t temp = 0;

    uint16_t i;
    for (i = 0; i < slen; i++)
    {
        str[i] = toupper(str[i]);
    }
    /** Search for cell temperature */
    if (LI8X0_FindSubstring(str, (char *)str_ack, strlen(str_ack), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->ack = temp;
    }

    /** Search for Version */
    if (LI8X0_FindSubstring(str, (char *)str_ver, strlen(str_ver), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->ver, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for Error */
    if (LI8X0_FindSubstring(str, (char *)str_error, strlen(str_error), &sIdx, &eIdx) == LICOR_OK)
    {
        memset(&tags->error, 0, 128);
        strncpy(tags->error, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for Data */
    if (LI8X0_FindSubstring(str, (char *)str_data, strlen(str_data), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseData(LicorSubstr, strlen(str_data), tags);
    }

    /** Search for Auxdata */
    if (LI8X0_FindSubstring(str, (char *)str_auxdata, strlen(str_auxdata), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseAuxdata(LicorSubstr, strlen(str_auxdata), tags);
    }

    /** Search for RS232 */
    if (LI8X0_FindSubstring(str, (char *)str_rs232, strlen(str_rs232), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseRS232(LicorSubstr, strlen(str_rs232), tags);
    }

    /** Search for CFG */
    if (LI8X0_FindSubstring(str, (char *)str_cfg, strlen(str_cfg), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseCFG(LicorSubstr, strlen(str_cfg), tags);
    }

    /** Search for CAL */
    if (LI8X0_FindSubstring(str, (char *)str_cal, strlen(str_cal), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseCal(LicorSubstr, strlen(str_cal), tags);
    }

    /** Search for POLY */
    if (LI8X0_FindSubstring(str, (char *)str_poly, strlen(str_poly), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParsePoly(LicorSubstr, strlen(str_poly), tags);
    }

    /** Search for PUMP */
    if (LI8X0_FindSubstring(str, (char *)str_pump, strlen(str_pump), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParsePump(LicorSubstr, strlen(str_pump), tags);
    }

    /** Search for SOURCE */
    if (LI8X0_FindSubstring(str, (char *)str_source, strlen(str_source), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseSource(LicorSubstr, strlen(str_source), tags);
    }

    /** Search for TECH */
    if (LI8X0_FindSubstring(str, (char *)str_tech, strlen(str_tech), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        LI8x0_ParseTech(LicorSubstr, strlen(str_tech), tags);
    }

    /** Search for SERIALNUM */
    if (LI8X0_FindSubstring(str, (char *)str_sernum, strlen(str_sernum), &sIdx, &eIdx) == LICOR_OK)
    {
        memset(tags->serialnum, 0, 32);
        strncpy(tags->serialnum, &str[sIdx], (eIdx - sIdx));
    }

    result = LICOR_OK;

    return result;
}

/** @brief Parse Data String
 *
 * Parses the <DATA> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseData(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_celltemp[] = "CELLTEMP";
    const char str_cellpres[] = "CELLPRES";
    const char str_co2[] = "CO2";
    const char str_co2abs[] = "CO2ABS";

    const char str_ivolt[] = "IVOLT";
    const char str_raw[] = "RAW";
    const char str_raw_co2[] = "CO2";
    const char str_raw_co2ref[] = "CO2REF";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[32];
    memset(tempVal, 0, 16);
    float temp;

    /** Search for cell temperature */
    if (LI8X0_FindSubstring(str, (char *)str_celltemp, strlen(str_celltemp), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        temp = strtof(tempVal, NULL);
        if ((-25.0 <= temp) && (temp <= 125.0))
        {
            tags->data.celltemp = temp;
        }
        else
        {
            tags->data.celltemp = NAN;
        }
        memset(tempVal, 0, 16);
    }

    /** Search for cell pressure */
    if (LI8X0_FindSubstring(str, (char *)str_cellpres, strlen(str_cellpres), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        temp = atof(tempVal);
        if ((0.0 <= temp) && (temp <= 200.0))
        {
            tags->data.cellpres = temp;
        }
        else
        {
            tags->data.cellpres = NAN;
        }
        memset(tempVal, 0, 16);
    }

    /** Search for co2 */
    if (LI8X0_FindSubstring(str, (char *)str_co2, strlen(str_co2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        temp = atof(tempVal);
        if ((-50.0 <= temp) && (temp <= 10000.0))
        {
            tags->data.co2 = temp;
        }
        else
        {
            temp = NAN;
        }
        memset(tempVal, 0, 16);
    }

    /** Search for co2abs */
    if (LI8X0_FindSubstring(str, (char *)str_co2abs, strlen(str_co2abs), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        temp = atof(tempVal);
        if ((-1000.0 <= temp) && (temp <= 1e8))
        {
            tags->data.co2abs = temp;
        }
        else
        {
            temp = NAN;
        }
        memset(tempVal, 0, 16);
    }

    /** Search for ivolt */
    if (LI8X0_FindSubstring(str, (char *)str_ivolt, strlen(str_ivolt), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        temp = atof(tempVal);
        if ((-10.0 <= temp) && (temp <= 100.0))
        {
            tags->data.ivolt = NAN;
        }
        else
        {
            tags->data.ivolt = NAN;
        }
        memset(tempVal, 0, 16);
    }

    uint32_t l_temp;

    /** Search for raw */
    if (LI8X0_FindSubstring(str, (char *)str_raw, strlen(str_raw), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        /** Search for raw co2 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_co2, strlen(str_raw_co2), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            l_temp = atol(tempVal);
            if (l_temp <= 9999999)
            {
                tags->data.raw.co2 = l_temp;
            }
            else
            {
                tags->data.raw.co2 = 999999;
            }
            memset(tempVal, 0, 16);
        }

        /** Search for raw co2 ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_co2ref, strlen(str_raw_co2ref), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            l_temp = atol(tempVal);
            if (l_temp <= 9999999)
            {
                tags->data.raw.co2ref = l_temp;
            }
            else
            {
                tags->data.raw.co2ref = 999999;
            }
            memset(tempVal, 0, 16);
        }
    }
    result = LICOR_OK;
    return result;
}

/** @brief Parse Auxdata String
 *
 * Parses the <AUXDATA> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseAuxdata(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_pca[] = "PCA";
    const char str_pha[] = "PHA";
    const char str_bbeff[] = "BB_EFF";
    const char str_psi[] = "PSI";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);

    /** Search for PCA */
    if (LI8X0_FindSubstring(str, (char *)str_pca, strlen(str_pca), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->auxdata.pca = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for PHA */
    if (LI8X0_FindSubstring(str, (char *)str_pha, strlen(str_pha), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->auxdata.pha = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for BB EFF */
    if (LI8X0_FindSubstring(str, (char *)str_bbeff, strlen(str_bbeff), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->auxdata.bb_eff = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for PSI */
    if (LI8X0_FindSubstring(str, (char *)str_psi, strlen(str_psi), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->auxdata.psi = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    result = LICOR_OK;

    return result;
}

/** @brief Parse RS232 String
 *
 * Parses the <RS232> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseRS232(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_flowrate[] = "FLOWRATE";
    const char str_celltemp[] = "CELLTEMP";
    const char str_cellpres[] = "CELLPRES";
    const char str_co2[] = "CO2";
    const char str_co2abs[] = "CO2ABS";
    const char str_h2o[] = "H2O";
    const char str_h2oabs[] = "H2OABS";
    const char str_h2odew[] = "H2ODEWPOINT";
    const char str_ivolt[] = "IVOLT";
    const char str_raw[] = "RAW";
    const char str_raw_co2[] = "CO2";
    const char str_raw_co2ref[] = "CO2REF";
    const char str_raw_h2o[] = "H2O";
    const char str_raw_h2oref[] = "H2OREF";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);
    uint8_t temp;

    /** Search for cell temperature */
    if (LI8X0_FindSubstring(str, (char *)str_celltemp, strlen(str_celltemp), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.celltemp = temp;
    }

    /** Search for cell pressure */
    if (LI8X0_FindSubstring(str, (char *)str_cellpres, strlen(str_cellpres), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.cellpres = temp;
    }

    /** Search for co2 */
    if (LI8X0_FindSubstring(str, (char *)str_co2, strlen(str_co2), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.co2 = temp;
    }

    /** Search for co2abs */
    if (LI8X0_FindSubstring(str, (char *)str_co2abs, strlen(str_co2abs), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.co2abs = temp;
    }

    /** Search for h2o */
    if (LI8X0_FindSubstring(str, (char *)str_h2o, strlen(str_h2o), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.h2o = temp;
    }

    /** Search for h20abs */
    if (LI8X0_FindSubstring(str, (char *)str_h2oabs, strlen(str_h2oabs), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.h2oabs = temp;
    }

    /** Search for h2o dewpoint */
    if (LI8X0_FindSubstring(str, (char *)str_h2odew, strlen(str_h2odew), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.h2odewpoint = temp;
    }

    /** Search for ivolt */
    if (LI8X0_FindSubstring(str, (char *)str_ivolt, strlen(str_ivolt), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.ivolt = temp;
    }

    /** Search for flowrate */
    if (LI8X0_FindSubstring(str, (char *)str_flowrate, strlen(str_flowrate), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->rs232.flowrate = temp;
    }

    /** Search for raw */
    if (LI8X0_FindSubstring(str, (char *)str_raw, strlen(str_raw), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        /** Search for raw co2 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_co2, strlen(str_raw_co2), &sIdx, &eIdx) == LICOR_OK)
        {
            if (strncmp("TRUE", &LicorSubstr[sIdx], (eIdx - sIdx)) == 0)
            {
                temp = true;
            }
            else
            {
                temp = false;
            }
            tags->rs232.raw.co2 = temp;
        }

        /** Search for raw co2 ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_co2ref, strlen(str_raw_co2), &sIdx, &eIdx) == LICOR_OK)
        {
            if (strncmp("TRUE", &LicorSubstr[sIdx], (eIdx - sIdx)) == 0)
            {
                temp = true;
            }
            else
            {
                temp = false;
            }
            tags->rs232.raw.co2ref = temp;
        }

        /** Search for raw h2o */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_h2o, strlen(str_raw_co2), &sIdx, &eIdx) == LICOR_OK)
        {
            if (strncmp("TRUE", &LicorSubstr[sIdx], (eIdx - sIdx)) == 0)
            {
                temp = true;
            }
            else
            {
                temp = false;
            }
            tags->rs232.raw.h2o = temp;
        }

        /** Search for raw h2o ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_raw_h2oref, strlen(str_raw_co2), &sIdx, &eIdx) == LICOR_OK)
        {
            if (strncmp("TRUE", &LicorSubstr[sIdx], (eIdx - sIdx)) == 0)
            {
                temp = true;
            }
            else
            {
                temp = false;
            }
            tags->rs232.raw.h2oref = temp;
        }
    }

    result = LICOR_OK;
    return result;
}

/** @brief Parse CFG String
 *
 * Parses the <CFG> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseCFG(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_heater[] = "HEATER";
    const char str_pcomp[] = "PCOMP";
    const char str_filter[] = "FILTER";
    const char str_bench[] = "BENCH";
    const char str_span[] = "SPAN";
    const char str_alarms[] = "ALARMS";
    const char str_alarms_logic[] = "LOGIC";
    const char str_alarms_source[] = "SOURCE";
    const char str_alarms_enabled[] = "ENABLED";
    const char str_alarms_high[] = "HIGH";
    const char str_alarms_hdead[] = "HDEAD";
    const char str_alarms_low[] = "LOW";
    const char str_alarms_ldead[] = "LDEAD";
    const char str_dacs[] = "DACS";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);
    uint8_t temp;

    /** Search for HEATER */
    if (LI8X0_FindSubstring(str, (char *)str_heater, strlen(str_heater), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->cfg.heater = temp;
    }

    /** Search for PCOMP */
    if (LI8X0_FindSubstring(str, (char *)str_pcomp, strlen(str_pcomp), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->cfg.pcomp = temp;
    }

    /** Search for FILTER */
    if (LI8X0_FindSubstring(str, (char *)str_filter, strlen(str_filter), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.filter = (uint16_t)atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for BENCH */
    if (LI8X0_FindSubstring(str, (char *)str_bench, strlen(str_bench), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.bench = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for SPAN */
    if (LI8X0_FindSubstring(str, (char *)str_span, strlen(str_span), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.span = atoi("123");
        memset(tempVal, 0, 16);
    }

    /** Search for ALARMS */
    if (LI8X0_FindSubstring(str, (char *)str_alarms, strlen(str_alarms), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        /** Search for raw co2 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_logic, strlen(str_alarms_logic), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            if (strncmp("TTL", tempVal, (eIdx - sIdx)) == 0)
            {
                tags->cfg.alarms.logic = TTL;
            }
            else
            {
                tags->cfg.alarms.logic = SWG;
            }
            memset(tempVal, 0, 16);
        }

        /** Search for raw ALARMS SOURCE ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_source, strlen(str_alarms_source), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            if (strncmp("CO2", tempVal, (eIdx - sIdx)) == 0)
            {
                tags->cfg.alarms.sources = CO2;
            }
            else
            {
                tags->cfg.alarms.sources = H2O;
            }
            memset(tempVal, 0, 16);
        }

        /** Search for ALARMS ENABLED */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_enabled, strlen(str_alarms_enabled), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            if (strncmp("TRUE", tempVal, (eIdx - sIdx)) == 0)
            {
                tags->cfg.alarms.enabled = true;
            }
            else
            {
                tags->cfg.alarms.enabled = false;
            }
            memset(tempVal, 0, 16);
        }

        /** Search for ALARMS HIGH */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_high, strlen(str_alarms_high), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->cfg.alarms.high = atof(tempVal);
            memset(tempVal, 0, 16);
        }

        /** Search for ALARMS HDEAD */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_hdead, strlen(str_alarms_hdead), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->cfg.alarms.hdead = atof(tempVal);
            memset(tempVal, 0, 16);
        }

        /** Search for ALARMS LOW ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_low, strlen(str_alarms_low), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->cfg.alarms.low = atof(tempVal);
            memset(tempVal, 0, 16);
        }

        /** Search for ALARMS LDEAD ref */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_alarms_ldead, strlen(str_alarms_ldead), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->cfg.alarms.ldead = atof(tempVal);
            memset(tempVal, 0, 16);
        }
    }

    /** Search for DACS */
    if (LI8X0_FindSubstring(str, (char *)str_dacs, strlen(str_dacs), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));

        if (LI8x0_ParseDACS(LicorSubstr, strlen(str), tags) == LICOR_OK)
        {
        }
    }

    result = LICOR_OK;

    return result;
}

/** @brief Parse DACS String
 *
 * Parses the <DACS> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseDACS(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_dacs_d1[] = "D1";
    const char str_dacs_d1_0[] = "D1_0";
    const char str_dacs_d1_f[] = "D1_F";
    const char str_dacs_d2[] = "D2";
    const char str_dacs_d2_0[] = "D2_0";
    const char str_dacs_d2_f[] = "D2_F";

    char tempVal[16];
    uint32_t sIdx = 0;
    uint32_t eIdx = 0;

    /** Search for DACS D1_0 ref */
    if (LI8X0_FindSubstring(str, (char *)str_dacs_d1_0, strlen(str_dacs_d1_0), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.dacs.d1_0 = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for DACS D1_F ref */
    if (LI8X0_FindSubstring(str, (char *)str_dacs_d1_f, strlen(str_dacs_d1_f), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.dacs.d1_f = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for DACS D2_0 ref */
    if (LI8X0_FindSubstring(str, (char *)str_dacs_d2_0, strlen(str_dacs_d2_0), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.dacs.d2_0 = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for DACS D2_F ref */
    if (LI8X0_FindSubstring(str, (char *)str_dacs_d2_f, strlen(str_dacs_d2_f), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cfg.dacs.d2_f = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    if (LI8X0_FindSubstring(str, (char *)str_dacs_d1, strlen(str_dacs_d1), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
        if (strncmp("CO2", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d1 = LICOR_DAC_CO2;
        }
        else if (strncmp("H2O", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d1 = LICOR_DAC_H2O;
        }
        else if (strncmp("H2ODP", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d1 = LICOR_DAC_H2ODEWPOINT;
        }
        else if (strncmp("CELLTEMP", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d1 = LICOR_DAC_CELLTEMP;
        }
        else if (strncmp("CELLPRES", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d1 = LICOR_DAC_CELLPRES;
        }
        else
        {
            tags->cfg.dacs.d1 = LICOR_DAC_NONE;
        }
        memset(tempVal, 0, 16);
    }

    if (LI8X0_FindSubstring(str, (char *)str_dacs_d2, strlen(str_dacs_d2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
        if (strncmp("CO2", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d2 = LICOR_DAC_CO2;
        }
        else if (strncmp("H2O", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d2 = LICOR_DAC_H2O;
        }
        else if (strncmp("H2ODP", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d2 = LICOR_DAC_H2ODEWPOINT;
        }
        else if (strncmp("CELLTEMP", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d2 = LICOR_DAC_CELLTEMP;
        }
        else if (strncmp("CELLPRES", tempVal, (eIdx - sIdx)) == 0)
        {
            tags->cfg.dacs.d2 = LICOR_DAC_CELLPRES;
        }
        else
        {
            tags->cfg.dacs.d2 = LICOR_DAC_NONE;
        }
        memset(tempVal, 0, 16);
    }

    result = LICOR_OK;
    return result;
}

/** @brief Parse CAL String
 *
 * Parses the <CAL> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseCal(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_FAIL;

    const char str_co2lastzero[] = "CO2LASTZERO";
    const char str_co2lastspan[] = "CO2LASTSPAN";
    const char str_co2lastspan2[] = "CO2LASTSPAN2";
    const char str_co2zero[] = "CO2ZERO";
    const char str_co2span[] = "CO2SPAN";
    const char str_co2span2[] = "CO2SPAN2";
    const char str_co2kzero[] = "CO2KZERO";
    const char str_co2kspan[] = "CO2KSPAN";
    const char str_co2kspan2[] = "CO2KSPAN2";
    const char str_h2olastzero[] = "H2OLASTZERO";
    const char str_h2olastspan[] = "H2OLASTSPAN";
    const char str_h2olastspan2[] = "H2OLASTSPAN2";
    const char str_h2ozero[] = "H2OZERO";
    const char str_h2ospan[] = "H2OSPAN";
    const char str_h2ospan2[] = "H2OSPAN2";
    const char str_h2okzero[] = "H2OKZERO";
    const char str_h2okspan[] = "H2OKSPAN";
    const char str_h2okspan2[] = "H2OKSPAN2";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);

    /** Search for CO2 LAST ZERO */
    if (LI8X0_FindSubstring(str, (char *)str_co2lastzero, strlen(str_co2lastzero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.co2lastzero, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for CO2 LAST SPAN */
    if (LI8X0_FindSubstring(str, (char *)str_co2lastspan, strlen(str_co2lastspan), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.co2lastspan, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for CO2 LAST SPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_co2lastspan2, strlen(str_co2lastspan2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.co2lastspan2, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for CO2 ZERO */
    if (LI8X0_FindSubstring(str, (char *)str_co2zero, strlen(str_co2zero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2zero = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for CO2 SPAN */
    if (LI8X0_FindSubstring(str, (char *)str_co2span, strlen(str_co2span), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2span = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for CO2 SPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_co2span2, strlen(str_co2span2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2span2 = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for CO2 KZERO */
    if (LI8X0_FindSubstring(str, (char *)str_co2kzero, strlen(str_co2kzero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2kzero = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for CO2 KSPAN */
    if (LI8X0_FindSubstring(str, (char *)str_co2kspan, strlen(str_co2kspan), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2kspan = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for CO2 KSPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_co2kspan2, strlen(str_co2kspan2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.co2kspan2 = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O LAST ZERO */
    if (LI8X0_FindSubstring(str, (char *)str_h2olastzero, strlen(str_h2olastzero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.h2olastzero, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for H2O LAST SPAN */
    if (LI8X0_FindSubstring(str, (char *)str_h2olastspan, strlen(str_h2olastspan), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.h2olastspan, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for H2O LAST SPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_h2olastspan2, strlen(str_h2olastspan2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->cal.h2olastspan2, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for H2O ZERO */
    if (LI8X0_FindSubstring(str, (char *)str_h2ozero, strlen(str_h2ozero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2ozero = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O SPAN */
    if (LI8X0_FindSubstring(str, (char *)str_h2ospan, strlen(str_h2ospan), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2ospan = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O SPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_h2ospan2, strlen(str_h2ospan2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2ospan2 = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O KZERO */
    if (LI8X0_FindSubstring(str, (char *)str_h2okzero, strlen(str_h2okzero), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2okzero = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O KSPAN */
    if (LI8X0_FindSubstring(str, (char *)str_h2okspan, strlen(str_h2okspan), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2okspan = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for H2O SPAN2 */
    if (LI8X0_FindSubstring(str, (char *)str_h2okspan2, strlen(str_h2okspan2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->cal.h2okspan2 = atof(tempVal);
        memset(tempVal, 0, 16);
    }
    return result;
}

/** @brief Parse POLY String
 *
 * Parses the <POLY> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParsePoly(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_OK;
    const char str_poly_date[] = "DATE";
    const char str_poly_bb[] = "BB";
    const char str_poly_xs[] = "XS";
    const char str_poly_reset[] = "RESET";
    const char str_poly_co2[] = "CO2";
    const char str_poly_h2o[] = "H2O";
    const char str_poly_pres[] = "PRES";
    const char str_poly_a0[] = "A0";
    const char str_poly_a1[] = "A1";
    const char str_poly_a2[] = "A2";
    const char str_poly_a3[] = "A3";
    const char str_poly_a4[] = "A4";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);
    uint8_t temp;

    /** Search for POLY DATE */
    if (LI8X0_FindSubstring(str, (char *)str_poly_date, strlen(str_poly_date), &sIdx, &eIdx) == LICOR_OK)
    {
        memset(tags->poly.date, 0, 20);
        strncpy(tags->poly.date, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for POLY RESET */
    if (LI8X0_FindSubstring(str, (char *)str_poly_reset, strlen(str_poly_reset), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->poly.reset = temp;
    }

    /** Search for POLY BB */
    if (LI8X0_FindSubstring(str, (char *)str_poly_bb, strlen(str_poly_bb), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->poly.bb = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for POLY XS */
    if (LI8X0_FindSubstring(str, (char *)str_poly_xs, strlen(str_poly_xs), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->poly.xs = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for POLY CO2 */
    if (LI8X0_FindSubstring(str, (char *)str_poly_co2, strlen(str_poly_co2), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        /** Search for A1 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a1, strlen(str_poly_a1), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.co2.a1 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        /** Search for A2 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a2, strlen(str_poly_a2), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.co2.a2 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        /** Search for A3 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a3, strlen(str_poly_a3), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.co2.a3 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        /** Search for A4 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a4, strlen(str_poly_a4), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.co2.a4 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        memset(LicorSubstr, 0, strlen(LicorSubstr));
    }

    /** Search for POLY H2O */
    if (LI8X0_FindSubstring(str, (char *)str_poly_h2o, strlen(str_poly_h2o), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));
        /** Search for A1 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a1, strlen(str_poly_a1), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.h2o.a1 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        /** Search for A2 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a2, strlen(str_poly_a2), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.h2o.a2 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        /** Search for A3 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a3, strlen(str_poly_a3), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.h2o.a3 = atof(tempVal);
            memset(tempVal, 0, 16);
        }

        memset(LicorSubstr, 0, strlen(LicorSubstr));
    }

    /** Search for POLY PRES */
    if (LI8X0_FindSubstring(str, (char *)str_poly_pres, strlen(str_poly_pres), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));

        /** Search for A0 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a0, strlen(str_poly_a0), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.pres.a0 = atof(tempVal);
            memset(tempVal, 0, 16);
        }

        /** Search for A1 */
        if (LI8X0_FindSubstring(LicorSubstr, (char *)str_poly_a1, strlen(str_poly_a1), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &LicorSubstr[sIdx], (eIdx - sIdx));
            tags->poly.pres.a1 = atof(tempVal);
            memset(tempVal, 0, 16);
        }
        memset(LicorSubstr, 0, strlen(LicorSubstr));
    }

    return result;
}

/** @brief Parse PUMP String
 *
 * Parses the <PUMP> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParsePump(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_OK;
    const char str_pump_enabled[] = "ENABLED";
    const char str_pump_time[] = "TIME";
    const char str_pump_drive[] = "DRIVE";
    const char str_pump_status[] = "STATUS";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);
    uint8_t temp;

    /** Search for PUMP ENABLED */
    if (LI8X0_FindSubstring(str, (char *)str_pump_enabled, strlen(str_pump_enabled), &sIdx, &eIdx) == LICOR_OK)
    {
        if (strncmp("TRUE", &str[sIdx], (eIdx - sIdx)) == 0)
        {
            temp = true;
        }
        else
        {
            temp = false;
        }
        tags->pump.enabled = temp;
    }

    /** Search for PUMP TIME */
    if (LI8X0_FindSubstring(str, (char *)str_pump_time, strlen(str_pump_time), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->pump.time = atol(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for PUMP DRIVE */
    if (LI8X0_FindSubstring(str, (char *)str_pump_drive, strlen(str_pump_drive), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->pump.drive = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for PUMP STATUS */
    if (LI8X0_FindSubstring(str, (char *)str_pump_status, strlen(str_pump_status), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->pump.status = (eLicorPumpStatus_t)atoi(tempVal);
        memset(tempVal, 0, 16);
    }
    return result;
}

/** @brief Parse SOURCE String
 *
 * Parses the <SOURCE> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseSource(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_OK;
    const char str_source_time[] = "TIME";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);

    /** Search for PUMP TIME */
    if (LI8X0_FindSubstring(str, (char *)str_source_time, strlen(str_source_time), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->source.time = atol(tempVal);
        memset(tempVal, 0, 16);
    }

    return result;
}

/** @brief Parse TECH String
 *
 * Parses the <TECH> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseTech(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_OK;
    const char str_tech_pump[] = "PUMP";
    const char str_tech_source[] = "SOURCE";
    const char str_tech_source_voltage[] = "VOLTAGE";

    uint32_t sIdx = 0;
    uint32_t eIdx = 0;
    char tempVal[16];
    memset(tempVal, 0, 16);

    /** Search for TECH SOURCE */
    if (LI8X0_FindSubstring(str, (char *)str_tech_source, strlen(str_tech_source), &sIdx, &eIdx) == LICOR_OK)
    {
        char tempVal2[16];
        memset(tempVal2, 0, 16);
        strncpy(tempVal2, &str[sIdx], (eIdx - sIdx));
        if (LI8X0_FindSubstring(tempVal2, (char *)str_tech_source_voltage, strlen(str_tech_source_voltage), &sIdx, &eIdx) == LICOR_OK)
        {
            strncpy(tempVal, &tempVal2[sIdx], (eIdx - sIdx));
            tags->tech.source.voltage = atof(tempVal);
        }
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump, strlen(str_tech_pump), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(LicorSubstr, &str[sIdx], (eIdx - sIdx));

        if (LI8x0_ParseTechPump(LicorSubstr, strlen(str), tags) == LICOR_OK)
        {
        }
    }

    return result;
}

/** @brief Parse PUMP String
 *
 * Parses the <PUMP> Tag
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC eLicorStatus_t LI8x0_ParseTechPump(char *str, uint16_t slen, sLicorTags_t *tags)
{
    eLicorStatus_t result = LICOR_OK;
    const char str_tech_pump_model[] = "MODEL";
    const char str_tech_pump_ki[] = "KI";
    const char str_tech_pump_kp[] = "KP";
    const char str_tech_pump_kd[] = "KD";
    const char str_tech_pump_offset[] = "OFFSET";
    const char str_tech_pump_gain[] = "GAIN";
    const char str_tech_pump_flow[] = "FLOW";

    char tempVal[16];
    uint32_t sIdx = 0;
    uint32_t eIdx = 0;

    /** Search for TECH PUMP MODEL */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_model, strlen(str_tech_pump_model), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tags->tech.pump.model, &str[sIdx], (eIdx - sIdx));
    }

    /** Search for TECH PUMP KI  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_ki, strlen(str_tech_pump_ki), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.ki = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP KP  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_kp, strlen(str_tech_pump_kp), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.kp = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP KD  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_kd, strlen(str_tech_pump_kd), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.kd = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP OFFSET  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_offset, strlen(str_tech_pump_offset), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.offset = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP GAIN  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_gain, strlen(str_tech_pump_gain), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.gain = atoi(tempVal);
        memset(tempVal, 0, 16);
    }

    /** Search for TECH PUMP FLOW  */
    if (LI8X0_FindSubstring(str, (char *)str_tech_pump_flow, strlen(str_tech_pump_flow), &sIdx, &eIdx) == LICOR_OK)
    {
        strncpy(tempVal, &str[sIdx], (eIdx - sIdx));
        tags->tech.pump.flow = atof(tempVal);
        memset(tempVal, 0, 16);
    }

    return result;
}

/** @brief Find Substring
 *
 * Finds the Substring within the message
 *
 * @param *searchstr Pointer to String to strip
 * @param *substr Pointer to substring to search for
 * @param sstlen String length
 * @param *startindex Pointer to start index
 * @param *endindex Pointer to end index
 *
 * @return result
 */
STATIC eLicorStatus_t LI8X0_FindSubstring(char *searchstr, char *substr, uint32_t sstrlen,
                                          uint32_t *startIndex, uint32_t *endIndex)
{

    eLicorStatus_t result = LICOR_FAIL;
    char *ret;
    char start[16] = "<";
    char end[16] = "</";
    uint8_t nullFlag = false;

    strncat(start, substr, sstrlen);
    strncat(end, substr, sstrlen);
    strcat(start, ">");
    strcat(end, ">");

    /** Search for the substring */
    ret = strstr(searchstr, start);

    if (ret != NULL)
    {
        *startIndex = ret - searchstr;
        *startIndex += strlen(start);
    }
    else
    {
        nullFlag = true;
    }

    if (nullFlag == false)
    {
        ret = strstr(searchstr, end);
        if (ret != NULL)
        {
            *endIndex = ret - searchstr;
        }
        else
        {
            nullFlag = true;
        }
    }

    if (nullFlag == true)
    {
        result = LICOR_FAIL;
    }
    else
    {
        result = LICOR_OK;
    }

    return result;
}

/** @brief Clear Tags Struct
 *
 * Clears the Tags structure to 0, null, "", etc.
 *
 * @param *str Pointer to Received String
 * @param slen Received String Length
 * @param *tags Pointer to Tag Structure
 *
 * @return result
 */
STATIC void LI8X0_ClearTags(sLicorTags_t *tags)
{
    /** Clear LVL1 Tags */
    memset(tags->ver, 0, 16);
    memset(tags->error, 0, 128);
    memset(tags->serialnum, 0, 32);

    /** Clear Data Tags */
    tags->data.flowrate = 0.0;
    tags->data.celltemp = 0.0;
    tags->data.cellpres = 0.0;
    tags->data.co2 = 0.0;
    tags->data.co2abs = 0.0;
    tags->data.h2o = 0.0;
    tags->data.h2oabs = 0.0;
    tags->data.h2odewpoint = 0.0;
    tags->data.ivolt = 0.0;
    tags->data.raw.co2 = 0.0;
    tags->data.raw.co2ref = 0.0;
    tags->data.raw.h2o = 0.0;
    tags->data.raw.h2oref = 0.0;

    /** Clear Auxdata Tags */
    tags->auxdata.pca = 0.0;
    tags->auxdata.pha = 0.0;
    tags->auxdata.bb_eff = 0.0;
    tags->auxdata.psi = 0.0;

    /** Clear RS232 */
    tags->rs232.flowrate = false;
    tags->rs232.co2 = false;
    tags->rs232.h2o = false;
    tags->rs232.celltemp = false;
    tags->rs232.cellpres = false;
    tags->rs232.ivolt = false;
    tags->rs232.co2abs = false;
    tags->rs232.h2oabs = false;
    tags->rs232.h2odewpoint = false;
    tags->rs232.raw.co2 = false;
    tags->rs232.raw.co2ref = false;
    tags->rs232.raw.h2o = false;
    tags->rs232.raw.h2oref = false;
    tags->rs232.echo = false;
    tags->rs232.strip = false;

    /** Clear CFG */
    tags->cfg.heater = false;
    tags->cfg.pcomp = false;
    tags->cfg.filter = 0;
    tags->cfg.outrate = 0.0;
    tags->cfg.alarms.logic = TTL;
    tags->cfg.alarms.sources = CO2;
    tags->cfg.alarms.enabled = false;
    tags->cfg.alarms.high = 0.0;
    tags->cfg.alarms.hdead = 0.0;
    tags->cfg.alarms.low = 0.0;
    tags->cfg.alarms.ldead = 0.0;
    tags->cfg.bench = 5;
    tags->cfg.span = 0;
    tags->cfg.dacs.range = R_2_5;
    tags->cfg.dacs.d1 = LICOR_DAC_NONE;
    tags->cfg.dacs.d1_0 = 0.0;
    tags->cfg.dacs.d1_f = 0.0;
    tags->cfg.dacs.d2 = LICOR_DAC_NONE;
    tags->cfg.dacs.d2_0 = 0.0;
    tags->cfg.dacs.d2_f = 0.0;

    /** Clear Cal */
    memset(tags->cal.co2lastzero, 0, 20);
    memset(tags->cal.co2lastspan, 0, 20);
    memset(tags->cal.co2lastspan2, 0, 20);
    memset(tags->cal.h2olastzero, 0, 20);
    memset(tags->cal.h2olastspan, 0, 20);
    memset(tags->cal.h2olastspan2, 0, 20);
    tags->cal.co2kzero = 0.0;
    tags->cal.co2kspan = 0.0;
    tags->cal.co2kspan2 = 0.0;
    tags->cal.h2okzero = 0.0;
    tags->cal.h2okspan = 0.0;
    tags->cal.h2okspan2 = 0.0;

    /** Clear Poly */
    memset(tags->poly.date, 0, 20);
    tags->poly.bb = 0.0;
    tags->poly.xs = 0.0;
    tags->poly.reset = false;
    tags->poly.co2.a1 = 0.0;
    tags->poly.co2.a2 = 0.0;
    tags->poly.co2.a3 = 0.0;
    tags->poly.co2.a4 = 0.0;
    tags->poly.h2o.a1 = 0.0;
    tags->poly.h2o.a2 = 0.0;
    tags->poly.h2o.a3 = 0.0;
    tags->poly.pres.a0 = 0.0;
    tags->poly.pres.a1 = 0.0;

    /** Clear Pump */
    tags->pump.enabled = false;
    tags->pump.time = 0;
    tags->pump.drive = 0;
    tags->pump.status = (eLicorPumpStatus_t)0;

    /** Clear source */
    tags->source.time = 0;

    /** Clear Tech */
    memset(tags->tech.pump.model, 0, 16);
    tags->tech.pump.ki = 0;
    tags->tech.pump.kp = 0;
    tags->tech.pump.kd = 0;
    tags->tech.pump.offset = 0;
    tags->tech.pump.gain = 0;
    tags->tech.pump.flow = 0;
    tags->tech.source.voltage = 0.0;

    return;
}

STATIC void LI8x0_ReadPrep(void)
{
    /** Prep Buffer */
    memset((char *)LicorRxBuffer, 0, 10);
    /** Reset Rx Pointer */
    pLicorRxBuffer = &LicorRxBuffer[0];

    /** Reset Rx Flag */
    LicorRxFlag = false;
}

STATIC eLicorStatus_t LI8X0_Read(char *sendstr, uint16_t mstimeout)
{
    eLicorStatus_t status = LICOR_FAIL;

    /** Send the command */
    if (sendstr != NULL)
    {
        BSP_UART_puts(&pLicor1->UART, sendstr, strlen(sendstr));
    }

    /** Wait for data or timeout */
    do
    {
        _delay_ms(1);
    } while ((LicorRxFlag != true) && (mstimeout-- > 0));

    if (LicorRxFlag == true)
    {
        status = LICOR_OK;
    }

    return status;
}

STATIC eLicorStatus_t LI8x0_WaitForACK(char *sstr)
{

    eLicorStatus_t result = LICOR_FAIL;
    sLicorXML_t XML;
    sLicorXML_t *pXML = &XML;

    /** Wait for response */
    uint8_t i = 100;
    do
    {
        _delay_ms(10);
    } while ((i-- > 0) && (LicorRxFlag != true));

    if (LicorRxFlag != true)
    {

        return result;
    }

    /** Verify ACK */
    strcpy(sstr, (char *)LicorRxBuffer);
    for (i = 0; i < 80; i++)
    {
        sstr[i] = toupper(sstr[i]);
    }
    if (LI8x0_IsAck(pXML, sstr, strlen(sstr) == LICOR_OK))
    {
        result = LICOR_OK;
    }
    LI8x0_ReadPrep();

    return result;
}