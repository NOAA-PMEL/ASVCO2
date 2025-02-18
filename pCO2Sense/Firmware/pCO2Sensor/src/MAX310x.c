/** @file MAX310X.h
 * @brief Maxim MAX310X Driver
 *
 * @author Matt Casari, matthew.casari@noaa.gov
 * @date February 15, 2018
 * @version 1.0.0
 *
 * @copyright National Oceanic and Atmospheric Administration
 * @copyright Pacific Marine Environmental Lab
 * @copyright Environmental Development Division
 *
 *	@note MAX310X Dual Serial UART with 128-Word Fifo Driver
 * @note (2x) UART to I2C converter
 *
 * @bug  No known bugs
 */
#include "MAX310x.h"
// #include "errors.h"
#include "logging.h"
/************************************************************************
 *					        Static Variables
 ************************************************************************/
STATIC eMAX310XPower_t powerState;
STATIC sGPIO_t *nReset;
STATIC sGPIO_t *cs;
STATIC sGPIO_t *nIRQ;
STATIC sGPIO_t *nI2C;
STATIC sSPI_t *SPI;
/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC eMAX310XStatus_t MAX310X_WriteByte(sMAX310X_t *Max, uint8_t reg, uint8_t val);
STATIC uint8_t MAX310X_ReadByte(sMAX310X_t *Max, uint8_t reg);
STATIC eMAX310XStatus_t MAX310X_puts(sMAX310X_t *Max, uint8_t reg, uint8_t *val, uint8_t len);
STATIC void MAX310X_CalculateBRG(sMAX310XBaud_t *b, uint32_t baudrate);
STATIC void MAX310X_SetInterrupts(sMAX310X_t *Max);
STATIC void MAX310X_ClearBuffer(sMAX310X_t *Max);
STATIC void MAX310X_SetUART(sMAX310X_t *Max);
/************************************************************************
 *					         Functions
 ************************************************************************/
eMAX310XStatus_t MAX310X_Init(sMAX310X_t *Max)
{
    eMAX310XStatus_t result = MAX310X_FAIL;
    nReset = (sGPIO_t *)&Max->nReset;
    cs = (sGPIO_t *)&Max->cs;
    nIRQ = (sGPIO_t *)&Max->nIRQ;
    nI2C = (sGPIO_t *)&Max->nI2C;
    SPI = &Max->SPI;

    /** Setup RESET GPIO */
    BSP_GPIO_Init(nReset);

    /** Setup CS GPIO */
    BSP_GPIO_Init(cs);
    MAX310X_CS_PIN_HIGH(&Max->cs);

    /** Setup nIRQ */
    BSP_GPIO_Init(nIRQ);

    /** Setup nI2C */
    BSP_GPIO_Init(nI2C);

    /** Set SPI */
    BSP_GPIO_SetPin(nI2C);

    /** Clear the RESET */
    BSP_GPIO_ClearPin(nReset);

    /** Wait for MAX310X To come out of reset */
    uint16_t timeout = 1000;
    while (
        (0 == BSP_GPIO_ReadInputPin((sGPIO_t *)&Max->nIRQ)) &&
        (timeout-- > 0))
    {
        if (timeout == 0)
        {
            Error(ERROR_MSG_01010040, LOG_Priority_High);
            return result;
        }
        _delay_ms(1);
    }

    /** Configure SPI */
    BSP_SPI_Init(SPI);

    /** Set the IC state */
    powerState = MAX310X_Power_ON;

    /** Clear the CS */
    MAX310X_CS_PIN_HIGH(&Max->cs);

    result = MAX310X_OK;
    return result;
}

eMAX310XStatus_t MAX310X_Send(sMAX310X_t *Max, char *str, uint8_t len)
{
    eMAX310XStatus_t result = MAX310X_FAIL;
    uint8_t temp = 0;
    uint8_t tempLen = 0;
    uint8_t count = 3;

    /** Clear the Buffer */
    MAX310X_ClearBuffer(Max);

    /** Check Message Length */
    while ((0 < len) && (0 < count))
    {
        /** Is Tx Buffer Empty? */
        temp = MAX310X_ReadByte(Max, MAX310XR_ISR);

        if (0 < (temp & 0x20))
        {
            if (len > 128)
            {
                tempLen = 128;
                len -= 128;
            }
            else
            {
                tempLen = len;
                len -= tempLen;
            }
            MAX310X_puts(Max, MAX310XR_THR, (uint8_t *)str, tempLen);
            count = 0;
        }
        else
        {
#ifndef UNIT_TESTING
            _delay_ms(50);
#endif
            count--;
        }
    }

    result = MAX310X_OK;
    return result;
}

eMAX310XStatus_t MAX310X_Read(sMAX310X_t *Max, char *str, uint8_t *len, uint8_t maxLen)
{
    eMAX310XStatus_t result = MAX310X_FAIL;
    uint8_t temp = 0;
    uint8_t readLen = 0;
    char *pStr = str;

    /** Read the number of bytes waiting */
    temp = MAX310X_ReadByte(Max, MAX310XR_RXFIFOLVL);

    /** Grab as many bytes as there, or as many as we can handle */
    if (maxLen < temp)
    {
        readLen = maxLen;
    }
    else
    {
        readLen = temp;
    }

    uint8_t i;
    for (i = 0; i < readLen; i++)
    {
        *pStr = MAX310X_ReadByte(Max, MAX310XR_RHR);
        pStr++;
    }

    *len = readLen;

    result = MAX310X_OK;
    return result;
}

eMAX310XStatus_t MAX310X_SetBaudrate(sMAX310X_t *Max, uint32_t baudrate)
{
    eMAX310XStatus_t result = MAX310X_FAIL;

    /** Verify valid baudrate */
    switch (baudrate)
    {
    case 2400:
    case 4800:
    case 9600:
    case 19200:
    case 28800:
    case 38400:
    case 57600:
    case 115200:
    case 230400:
    case 460800:
        break;
    default:
#ifndef UNIT_TESTING
        Error(ERROR_MSG_1020, LOG_Priority_Low);
#endif
        return result;
        break;
    }

    /** Configure the Clock Source for external oscillator */
    MAX310X_WriteByte(Max, MAX310XR_CLKSOURCE, 0x0A);

    /** Calculate the Register Values */
    MAX310X_CalculateBRG(&Max->Baud, baudrate);

    /** Set the BRGCONFIG Register */
    MAX310X_WriteByte(Max, MAX310XR_BRGCONFIG, Max->Baud.BRGConfig);

    /** Set the MSB and LSB Baud Rate Registers */
    MAX310X_WriteByte(Max, MAX310XR_DIVLSB, Max->Baud.DIVLSB);
    MAX310X_WriteByte(Max, MAX310XR_DIVMSB, Max->Baud.DIVMSB);

    uint8_t temp;
    uint8_t tryCount = 5;
    do
    {
        temp = MAX310X_ReadByte(Max, MAX310XR_STSINT);
    } while ((temp & BIT5) && (tryCount-- > 0));

    if ((tryCount == 0) && ((temp & BIT5) == 0))
    {
        return result;
    }
    /** Readback registers */
    temp = MAX310X_ReadByte(Max, MAX310XR_CLKSOURCE);
    temp = MAX310X_ReadByte(Max, MAX310XR_BRGCONFIG);
    temp = MAX310X_ReadByte(Max, MAX310XR_DIVLSB);
    temp = MAX310X_ReadByte(Max, MAX310XR_DIVMSB);

    result = MAX310X_OK;
    return result;
}

uint32_t MAX310X_GetBaudrate(sMAX310X_t *Max)
{
    return Max->Baud.Baudrate;
}

void MAX310X_Sleep(void)
{

    /** Put MAX310X in Reset for Low Power */
    BSP_GPIO_SetPin(nReset);

    /** Put MAX310X Chip Select Low to save power */
    BSP_GPIO_ClearPin(cs);

    /** Close SPI */
    BSP_SPI_Disable(SPI);

    /** Set the IC state */
    powerState = MAX310X_Power_OFF;
}

void MAX310X_Wakeup(sMAX310X_t *Max)
{
    if (MAX310X_Power_ON != powerState)
    {
        /** Close SPI */
        BSP_SPI_Enable(SPI);

        /** Remove MAX310X from Reset for Run Mode */
        BSP_GPIO_ClearPin(nReset);

        /** Test for reset */
        uint16_t timeout = 1000;
        while (
            (0 == BSP_GPIO_ReadInputPin(nIRQ)) &&
            (timeout-- > 0))
        {
            if (timeout == 0)
            {
                Error(ERROR_MSG_01010041, LOG_Priority_High);
                return;
            }
            _delay_ms(1);
        }

        /** Set the IC state */
        powerState = MAX310X_Power_ON;
    }

    /** Configure Interrupts */
    MAX310X_SetInterrupts(Max);

    /** Configure the UART */
    MAX310X_SetUART(Max);

    /** Clear buffer */
    MAX310X_ClearBuffer(Max);
}

/************************************************************************
 *					           Static Functions
 ************************************************************************/

/** @brief MAX310X Write Byte
 *
 * Write byte to UART.
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param reg Register to write to.
 * @param val Value to write to register
 *
 * @return result
 */
STATIC eMAX310XStatus_t MAX310X_WriteByte(sMAX310X_t *Max, uint8_t port, uint8_t val)
{
    eMAX310XStatus_t result = MAX310X_FAIL;
    uint8_t sendreg = port;

    /** Prep which port to write to */
    if (Max->Port > 1)
    {
        return result;
    }
    sendreg |= (0x80);

    /** Set the Chip Select */
    MAX310X_CS_PIN_LOW(&Max->cs);

    /** Write the address */
    BSP_SPI_put(&Max->SPI, sendreg);

    /** Write the byte */
    BSP_SPI_put(&Max->SPI, val);

    /** Set the Chip Select */
    MAX310X_CS_PIN_HIGH(&Max->cs);
    result = MAX310X_OK;
    return result;
}
/** @brief MAX310X Puts
 *
 * Put String to UART.
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param reg Register to write to
 * @param *val Pointer to send string
 * @param len Length of string
 *
 * @return result
 */
STATIC eMAX310XStatus_t MAX310X_puts(sMAX310X_t *Max, uint8_t reg, uint8_t *val, uint8_t len)
{
    eMAX310XStatus_t result = MAX310X_FAIL;
    uint8_t sendreg = reg;

    /** Prep which port to write to */
    if (Max->Port > 1)
    {
        return result;
    }
    sendreg |= (0x80);

    /** Set the Chip Select */
    MAX310X_CS_PIN_LOW(&Max->cs);

    /** Write the address */
    BSP_SPI_put(&Max->SPI, sendreg);
    do
    {
        /** Write the byte */
        BSP_SPI_put(&Max->SPI, *val++);
        len--;
    } while (len > 0);
    /** Set the Chip Select */
    MAX310X_CS_PIN_HIGH(&Max->cs);
    result = MAX310X_OK;
    return result;
}

/** @brief MAX310X Read Byte
 *
 * Read Byte from UART.
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param reg Register to read from
 *
 * @return Byte read
 */
STATIC uint8_t MAX310X_ReadByte(sMAX310X_t *Max,
                                uint8_t port)
{

    uint8_t val;

    /** Set the Chip Select */
    MAX310X_CS_PIN_LOW(&Max->cs);

    /** Setup read for read from port selected */
    val = BSP_SPI_get(&Max->SPI, (port & 0x7F));

    /** Clear the Chip Select */
    MAX310X_CS_PIN_HIGH(&Max->cs);

    return val;
}

/** @brief Fractional Baudrate Calculator
 *
 * Fractional Buadrate Calculator take system frequency values and requested
 * baudrate to calculate register values for MAX310X.
 *
 *  ************** Equations ***************
 *
 *  --EQ1--
 *  D = (fRef - RateMode) / (16 x BaudRate )
 * where:
 *  D = Divisor
 *  fRef = Crystal Frequency
 *  RateMode = 1x, 2x, or 4x  (used to create faster frequencies via PLL)
 *  BaudRate = Requested baudrate
 *
 * --EQ2--
 *  Div = TRUNC(D)
 *  where:
 *  Div = Integer value of D
 *
 * --EQ3--
 *  FRACT = ROUND(16 x (D - Div))
 *  where:
 *  FRACT = Fractional Portion of D
 *
 * @param *b Pointer to the Baudrate Structure
 * @param baudrate Selected Baudrate
 *
 * @return none
 */
STATIC void MAX310X_CalculateBRG(sMAX310XBaud_t *b, uint32_t baudrate)
{
    float D;
    uint16_t Div;

    D = b->fref * (float)b->rateMode;
    D /= (float)baudrate;
    D /= 16;

    Div = (uint16_t)D;

    b->DIVLSB = (uint8_t)Div & 0x00FF;
    b->DIVMSB = (uint8_t)(Div & 0xFF00) >> 8;

    D -= (float)Div;
    D *= 16.0;

    b->BRGConfig = (uint32_t)round(D);

    b->Baudrate = baudrate;
}

/** @brief MAX310X Set Interrupts
 *
 * Set interrupts on MAX310X.  Currently sets:
 * 1a. Receive Trigger Enable Interrupt
 * 1b. Transmit FIFO Empty Interrupt
 * 2a. Receive Timeout Interrupt
 * 3a. Auto-Sleep Mode
 *
 * @param *Max Pointer to the MAX310X Struct
 *
 * @return return None
 */
STATIC void MAX310X_SetInterrupts(sMAX310X_t *Max)
{
    /** Set the MAX310X IRQ Enables */
    uint8_t irq = (MAXIRQ_RXTRGIEN | MAXIRQ_TFIFOEMTYIEN);
    MAX310X_WriteByte(Max, MAX310XR_IRQEN, irq);

    /** Set the MAX310X Line Status Interrupts */
    irq = (MAXLSR_RTIMEOTIEN);
    MAX310X_WriteByte(Max, MAX310XR_LSRINTEN, irq);

    /** Set the MAX310X Receive Timeout Interrupt */
    irq = MAX310X_RECEIVE_INTERRUPT_LEN;
    MAX310X_WriteByte(Max, MAX310XR_RXTIMEOUT, irq);

    /** Set Auto-Sleep Mode */
    irq = MAXMODE1_AUTOSLEEP;
    MAX310X_WriteByte(Max, MAX310XR_MODE1, irq);

    return;
}

/** @brief MAX310X Configure UART
 *
 * Configure the UART settings
 * Sets:
 * 1. 8-Bit
 * 2. Stop-Bit
 * 3. No Parity
 * 4. No Flow Control
 *
 * @param *Max Pointer to MAX310X Struct
 *
 * @return None
 */
STATIC void MAX310X_SetUART(sMAX310X_t *Max)
{

    uint8_t uart = (MAXLCR_LEN_8);
    MAX310X_WriteByte(Max, MAX310XR_LCR, uart);
}

/** @brief MAX310X Clear Buffers
 *
 * Clear Rx/Tx Buffer.
 *
 * @param *Max Pointer to the MAX310X Struct
 *
 * @return None
 */
STATIC void MAX310X_ClearBuffer(sMAX310X_t *Max)
{
    /** Read the Mode2 Register */
    uint8_t temp = MAX310X_ReadByte(Max, MAX310XR_MODE2);

    do
    {
        /** Clear the Rx/Tx buffers by setting FIFORst Bit */
        temp |= MAXMODE2_FIFORST;
        MAX310X_WriteByte(Max, MAX310XR_MODE2, temp);

        temp &= ~MAXMODE2_FIFORST;
        /** Finish by clearing the FIFORst Bit */
        MAX310X_WriteByte(Max, MAX310XR_MODE2, temp);

        temp = MAX310X_ReadByte(Max, MAX310XR_MODE2);

    } while (temp != 0);
}