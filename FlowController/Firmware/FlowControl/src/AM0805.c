/** @file AM0805.c
 *  @brief Ambiq AM0805 RTC Driver
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date October 2nd, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */
#include "AM0805.h"

/************************************************************************
 *			                Static Functions Prototypes
 ************************************************************************/
STATIC eAM0805Status_t am_get_time(sAM0805_t *p);
STATIC eAM0805Status_t am_osc_sel(eAM0808osc_t osc);
STATIC eAM0805Status_t am_config_sqw(uint8_t Freq, eAM0805OutPins_t Pin);
STATIC eAM0805Status_t am_set_alarm_on_zeros(eAMBIQ_Alarm_t alarm, uint8_t intmode, uint8_t pin);
STATIC uint8_t bcd2dec(uint8_t bcdno);
STATIC uint8_t dec2bcd(uint8_t decno);
STATIC eAM0805Status_t readreg(uint8_t reg, uint8_t *val);
STATIC eAM0805Status_t writereg(uint8_t reg, uint8_t val);
STATIC eAM0805Status_t clrreg(uint8_t reg, uint8_t mask);
STATIC eAM0805Status_t setreg(uint8_t addr, uint8_t mask);

/************************************************************************
 *			                Variables (Static in Module)
 ************************************************************************/
STATIC PERSISTENT sAM0805_t AM0805;

/************************************************************************
 *					                   Functions
 ************************************************************************/
eAM0805Status_t AM0805_Init(void)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t setting = 0x00u;

    /** Initialize the I2C Port */
    if (BSP_I2C_Init() == BSP_OK)
    {
        /** Reset the trickle charger */
        if (writereg(CONFIG_KEY_REG, UNLOCK_KEY) == AM0805_OK)
        {
            /** Set the trickle charger ON, Schottky Diode, ROUT=3K */
            setting = (TRICKLE_ENABLE | TRICKLE_DIODE_SCHOTTKY | TRICKLE_RES_3K);
            if (writereg(TRICKLE_CHARGE_REG, setting) == AM0805_OK)
            {

                /** Set the Oscillator to Auto-Switch */
                if (am_osc_sel(AM0805_OSC_32768Hz_AutoSwitch) == AM0805_OK)
                {
                    result = AM0805_OK;
                }
            }
        }
    }
    return result;
}

eAM0805Status_t AM0805_GetTime(void)
{
    eAM0805Status_t result = AM0805_FAIL;

    if (am_get_time(&AM0805) == AM0805_OK)
    {
        strftime(&AM0805.timestr[0], sizeof(AM0805.timestr), "%2Y%2m%2d %2H:%2M:%2S", &AM0805.time);
        result = AM0805_OK;
    }

    return result;
}

eAM0805Status_t AM0805_SetCalFreq(eAM0805OutPins_t Pin)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t cResult = 0;

    /* Clear the previous settings */
    cResult += writereg(CAL_XT_REG, CALXT_CLEAR);
    cResult += writereg(OSC_STATUS_REG, OSCSTAT_CLEAR);

    /* Turn Square Wave off on Pin FOUT */
    cResult += am_config_sqw(SQW_DISABLED, Pin);

    /* Configure a 32.768kHz Square Wave on Pin FOUT */
    cResult += am_config_sqw(SQW_T_32768HZ, Pin);
    ;

    if (cResult == 0)
    {
        result = AM0805_OK;
    }

    return result;
}

eAM0805Status_t AM0805_SetAlarm(eAMBIQ_Alarm_t alarm)
{
    eAM0805Status_t result = AM0805_FAIL;

    result = am_set_alarm_on_zeros(alarm, 1, 2);

    return result;
}

eAM0805Status_t AM0805_RTC_OffsetCalculation(float frequency)
{
    eAM0805Status_t result = AM0805_FAIL;

    float Padj = 0.0;

    /** Calculate the offset (Based on the AMx8x5 Data Sheet XT Osc. Dig Cal) */
    Padj = 32768.0f - frequency;
    Padj *= 1.0e6;
    Padj /= 32769;
    Padj /= 1.90735;

    int16_t adj = (int16_t)Padj;

    /* Determine the Offset, Crytal Cal and Command values */
    uint8_t OFFSETX = 0;
    uint8_t XTCAL = 0;
    uint8_t CMDX = 0;

    /* The following  is based on the AB08X5 Datasheet, Revision C, Page 41 */
    /* 5.9 Digital Calibration -> 5.9.1 XT Oscillator Digital Calibration instructions */
    if (adj < -320)
    {
        result = AM0805_FAIL;
    }
    else if (adj < -256)
    {
        XTCAL = 3;
        CMDX = 1;
        OFFSETX = (adj + 192) / 2;
        result = AM0805_OK;
    }
    else if (adj < -192)
    {
        XTCAL = 3;
        CMDX = 0;
        OFFSETX = adj + 192;
        result = AM0805_OK;
    }
    else if (adj < -128)
    {
        XTCAL = 2;
        CMDX = 0;
        OFFSETX = adj + 128;
        result = AM0805_OK;
    }
    else if (adj < -64)
    {
        XTCAL = 1;
        CMDX = 0;
        OFFSETX = adj + 64;
        result = AM0805_OK;
    }
    else if (adj < 64)
    {
        XTCAL = 0;
        CMDX = 0;
        OFFSETX = adj;
        result = AM0805_OK;
    }
    else if (adj < 128)
    {
        XTCAL = 0;
        CMDX = 1;
        OFFSETX = adj / 2;
        result = AM0805_OK;
    }
    else
    {
        result = AM0805_FAIL;
    }

    return result;
}

/************************************************************************
*				        STATIC FUNCTIONS
************************************************************************/
/** @brief Configure Square Wave Output
 *
 *  Configure a Square Wave output on selected pin
 *
 *  @param Freq Frequency to output 
 *  @param Pin Pin to output signal on
 *
 *  @return Result
 */
STATIC eAM0805Status_t am_config_sqw(uint8_t Freq, eAM0805OutPins_t Pin)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t temp = 0;
    uint8_t cResult = 0;

    /** Read the current SQW Register */
    cResult += readreg(SQW_REG, &temp);
    temp = temp & 0x70; /** Retain Reserved Bits, but clear all else */
    temp = temp | SQW_ENABLED;
    temp = temp | Freq;

    if (Freq == SQW_DISABLED)
    {
        temp = temp & 0x3F; /** Clear SQW Enable */
    }

    /** Set up the approriate control register for selected Pin */
    switch (Pin)
    {
    case AM0805_nIRQ:
        /** Setup the nIRQ Pin */
        cResult += clrreg(CONTROL_2_REG, OUT1S_MASK);
        cResult += writereg(CONTROL_2_REG, OUT1S_SQW);

        break;
    case AM0805_nIRQ2:
        /** Setup the nIRQ2 Pin */
        cResult += clrreg(CONTROL_2_REG, OUT2S_MASK);
        cResult += writereg(CONTROL_2_REG, OUT2S_SQW);

        break;
    default:
        cResult++;
        break;
    }

    /** Write the Square Wave Register */
    cResult += writereg(SQW_REG, temp);

    if (cResult == 0)
    {
        result = AM0805_OK;
    }

    return result;
}

/** @brief Set Alarm on Zero values
 *
 *  Set selected alarm on the zero seconds, minutes, etc.
 *
 *  @param alarm Alarm to set
 *  @param intmode Type of interrupt (duration)
 *  @param Pin Pin to output alarm
 *
 *  @return Result
 */
STATIC eAM0805Status_t am_set_alarm_on_zeros(eAMBIQ_Alarm_t alarm, uint8_t intmode, uint8_t pin)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t temp_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i, temp;
    uint8_t cResult = 0;

    temp_buf[0] = dec2bcd(0);
    temp_buf[1] = dec2bcd(0);
    temp_buf[2] = dec2bcd(0);
    temp_buf[3] = dec2bcd(0);
    temp_buf[4] = dec2bcd(0);
    temp_buf[5] = dec2bcd(0);
    temp_buf[6] = dec2bcd(0);
    temp_buf[7] = dec2bcd(0);

    /** Clear Timer Control Register */
    cResult += clrreg(TIMER_CTRL_REG, 0x1C);

    /** Clear Interrupt Mask Register */
    cResult += clrreg(INT_MASK_REG, 0x64);

    /** Clear Status Register */
    cResult += clrreg(STATUS_REG, 0x04);

    /** Set Control Register for Pin */
    if (pin == 1)
    {
        /** Interrupt on nIRQ */
        cResult += readreg(CONTROL_2_REG, &temp);
        temp = (temp & 0x03);
        if (temp != 0)
        {
            cResult += setreg(CONTROL_2_REG, 0x03);
        }
    }
    else if (pin == 2)
    {
        /** Interrupt on nIRQ2 */
        cResult += readreg(CONTROL_2_REG, &temp);
        temp = (temp & 0x1C);
        if (temp != 0)
        {
            cResult += clrreg(CONTROL_2_REG, 0x1C);
            cResult += setreg(CONTROL_2_REG, 0x0C);
        }
    }

    /** Set the repeat for <1Second Alarms*/
    switch (alarm)
    {
    case AM0805_Alarm_10Hz:
        temp_buf[0] |= 0xF0;
        alarm = AM0805_Alarm_1Sec;
        break;
    case AM0805_Alarm_100Hz:
        temp_buf[0] = 0xFF;
        alarm = AM0805_Alarm_1Sec;
        break;
    default:
        break;
    }

    if (alarm != AM0805_Alarm_Disable)
    {
        temp = (alarm << 2);
        cResult += setreg(TIMER_CTRL_REG, temp);
        cResult += setreg(INT_MASK_REG, (intmode << 5));
        for (i = 0; i < 8; i++)
        {
            cResult += BSP_I2C_putc(BSP_I2C_AMBIQ, ALARM_HUNDRS_REG, temp_buf[i]);
        }
        cResult += setreg(INT_MASK_REG, 0x04);
    }
    else
    {
        /*** Set IM field to 0x30 to minize current draw */
        cResult += setreg(INT_MASK_REG, 0x60);
    }

    if (cResult == 0)
    {
        result = AM0805_OK;
    }

    return result;
}

/** @brief Get Time
 *
 *  Retreive current time 
 *
 *  @param *p Pointer to time structure
 *
 *  @return Result
 */
STATIC eAM0805Status_t am_get_time(sAM0805_t *p)
{
    eAM0805Status_t result;
    uint8_t timebuf[8];
    struct tm temp;
    uint8_t mode = 0;

    /** Clear the temporary time buffer */
    memset(timebuf, 0, 8);
    // memset(&timebuf[0],NULL,8);

    /** Read the I2C Registers for time */
    if (BSP_I2C_gets(BSP_I2C_AMBIQ, HUNDREDTHS_REG, 8, &timebuf[0]) == BSP_OK)
    {
        /** Populate time structure */
        temp.tm_sec = bcd2dec(timebuf[1]);
        temp.tm_min = bcd2dec(timebuf[2]);
        temp.tm_hour = bcd2dec(timebuf[3]);
        temp.tm_mday = bcd2dec(timebuf[4]);
        temp.tm_mon = bcd2dec(timebuf[5]);
        temp.tm_year = bcd2dec(timebuf[6]);
        temp.tm_wday = bcd2dec(timebuf[7]);
        result = AM0805_OK;
    }

    /** If valid I2C response, convert hours if necessary */
    if (result == AM0805_OK)
    {
        if (BSP_I2C_getc(BSP_I2C_AMBIQ, CONTROL_1_REG, &timebuf[0]) == BSP_OK)
        {
            /** 12 hour mode */
            if ((timebuf[0] & 0x40) != 0)
            {
                mode = (AM0805.time.tm_hour & 0x20) ? 1 : 0;
                temp.tm_hour += 12;
            }
            result = AM0805_OK;
        }
        else
        {
            result = AM0805_FAIL;
        }

        /** Update AM0805 Struct */
        AM0805.time = temp;
    }

    return result;
}

/** @brief Oscillator Select
 *
 *  Select Oscillator to use
 *
 *  @param osc Oscillator to select
 *
 *  @return result
 */
STATIC eAM0805Status_t am_osc_sel(eAM0808osc_t osc)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t i;
    uint8_t temp;

    /** Read oscillator control register */
    if (BSP_I2C_getc(BSP_I2C_AMBIQ, OSC_CONTROL_REG, &temp) == BSP_OK)
    {

        temp = temp & 0x67; /** Clear OSEL, FOS, AOS */

        switch (osc)
        {
        case AM0805_OSC_32768Hz_NoSwitch:
            if (BSP_I2C_putc(BSP_I2C_AMBIQ, OSC_CONTROL_REG, temp) == BSP_OK)
            {
                result = AM0805_OK;
            }
            break;
        case AM0805_OSC_32768Hz_AutoSwitch:
            temp = temp | 0x10;
            if (BSP_I2C_putc(BSP_I2C_AMBIQ, OSC_CONTROL_REG, temp) == BSP_OK)
            {
                result = AM0805_OK;
            }
            break;
        case AM0805_OSC_128Hz:
        default:
            temp = temp | 0x80;
            if (BSP_I2C_putc(BSP_I2C_AMBIQ, OSC_CONTROL_REG, temp) == BSP_OK)
            {
                result = AM0805_OK;
            }
            break;
        }
    }

    return result;
}

/** @brief BCD to DEC
 *
 *  Convert BCD to Decimal Value
 *
 *  @param bcd value
 *
 *  @return decimal value
 */
STATIC uint8_t bcd2dec(uint8_t bcdno)
{
    return ((bcdno >> 4) * 10) + (bcdno & 0x0F);
}

/** @brief DEC to BCD
 *
 *  Convert Decimal to BCD value
 *
 *  @param decno Decimal Value
 *
 *  @return bcd value
 */
STATIC uint8_t dec2bcd(uint8_t decno)
{
    return (((decno / 10) << 4) | (decno % 10));
}

/** @brief Read Register
 *
 *  Read current value in register
 *
 *  @param reg Register to read
 *  @param *val Pointer to value (for return)
 *
 *  @return Result
 */
STATIC eAM0805Status_t readreg(uint8_t reg, uint8_t *val)
{
    eAM0805Status_t result = AM0805_FAIL;

    if (BSP_I2C_getc(BSP_I2C_AMBIQ, reg, val) == BSP_OK)
    {
        result = AM0805_OK;
    }

    return result;
}

/** @brief Write Register
 *
 *  Write value to register
 *
 *  @param reg Register to write to
 *  @param val Value to write
 *
 *  @return Result
 */
STATIC eAM0805Status_t writereg(uint8_t reg, uint8_t val)
{
    eAM0805Status_t result = AM0805_FAIL;

    if (BSP_I2C_putc(BSP_I2C_AMBIQ, reg, val) == BSP_OK)
    {
        result = AM0805_OK;
    }
    return result;
}

/** @brief Clear Register
 *
 *  Mask (protect) bits and otherwise clear register
 *
 *  @param reg Register to clear
 *  @param mask Bits to mask
 *
 *  @return result
 */
STATIC eAM0805Status_t clrreg(uint8_t reg, uint8_t mask)
{
    eAM0805Status_t result = AM0805_FAIL;
    uint8_t cResult = 0;
    uint8_t temp = 0;

    cResult += readreg(reg, &temp);
    temp = temp & (~mask);
    cResult += writereg(reg, temp);

    if (cResult == 0)
    {
        result = AM0805_OK;
    }

    return result;
}

/** @brief Set Register
 *
 *  Mask bits and set register
 *
 *  @param addr Address to set
 *  @param mask Bits to mask
 *
 *  @return Result
 */
STATIC eAM0805Status_t setreg(uint8_t addr, uint8_t mask)
{
    eAM0805Status_t result = AM0805_FAIL;

    uint8_t temp;

    BSP_I2C_getc(BSP_I2C_AMBIQ, addr, &temp);
    temp |= mask;
    BSP_I2C_putc(BSP_I2C_AMBIQ, addr, temp);

    return result;
}