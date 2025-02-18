/** @file MAX310X.h
 *  @brief Maxim MAX310X Driver
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date February 15, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note MAX310X Dual Serial UART with 128-Word Fifo Driver
 *  @note (2x) UART to I2C converter
 *
 *  @bug  No known bugs
 */
#ifndef _MAX310X_H
#define _MAX310X_H

/** Remove STATIC and PERSISTENT values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
#endif
#ifndef PERSISTENT
#define PERSISTENT
#endif
#else
#ifndef STATIC
#define STATIC static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent
#endif
#endif

/************************************************************************
 *						STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <string.h>
#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
#include <math.h>

#ifdef UNIT_TESTING
#include <stdio.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "buffer8.h"
#include "logging.h"
#include "errorlist.h"

/************************************************************************
 *							MACROS
 ************************************************************************/

/** MAX310X Registers */
#define MAX310XR_RHR (0x00)
#define MAX310XR_THR (0x00)
#define MAX310XR_IRQEN (0x01)
#define MAX310XR_ISR (0x02)
#define MAX310XR_LSRINTEN (0x03)
#define MAX310XR_LSR (0x04)
#define MAX310XR_SPCLCHRINTEN (0x05)
#define MAX310XR_SPCLCHARINT (0x06)
#define MAX310XR_STSINTEN (0x07)
#define MAX310XR_STSINT (0x08)
#define MAX310XR_MODE1 (0x09)
#define MAX310XR_MODE2 (0x0A)
#define MAX310XR_LCR (0x0B)
#define MAX310XR_RXTIMEOUT (0x0C)
#define MAX310XR_HDPLXDELAY (0x0D)
#define MAX310XR_IRDA (0x0E)
#define MAX310XR_FLOWLVL (0x0F)
#define MAX310XR_FIFOTRGLVL (0x10)
#define MAX310XR_TXFIFOLVL (0x11)
#define MAX310XR_RXFIFOLVL (0x12)
#define MAX310XR_FLOWCTRL (0x13)
#define MAX310XR_XON1 (0x14)
#define MAX310XR_XON2 (0x15)
#define MAX310XR_XOFF1 (0x16)
#define MAX310XR_XOFF2 (0x17)
#define MAX310XR_GPIOCONFG (0x18)
#define MAX310XR_GPIODATA (0x19)
#define MAX310XR_PLLCONFIG (0x1A)
#define MAX310XR_BRGCONFIG (0x1B)
#define MAX310XR_DIVLSB (0x1C)
#define MAX310XR_DIVMSB (0x1D)
#define MAX310XR_CLKSOURCE (0x1E)
#define MAX310XR_GLOBALIRQ (0x1F)
#define MAX310XR_CLOBLCOMND (0x1F)
#define MAX310XR_TXSYNC (0x20)
#define MAX310XR_SYNCDELAY1 (0x21)
#define MAX310XR_SYNCDELAY2 (0x22)
#define MAX310XR_TIMER1 (0x23)
#define MAX310XR_TIMER2 (0x24)
#define MAX310XR_REVID (0x25)

#define MAX310X_CS_PIN_LOW(x) (BSP_GPIO_SetPin((sGPIO_t *)x))
#define MAX310X_CS_PIN_HIGH(x) (BSP_GPIO_ClearPin((sGPIO_t *)x))

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 * @enum MAX310X Status
 * MAX310X Status State
 */
typedef enum
{
    MAX310X_OK = 0x00u,
    MAX310X_FAIL = 0x01u
} eMAX310XStatus_t;

/**
 * @enum MAX310X Power
 * MAX310X Power State
 */
typedef enum
{
    MAX310X_Power_OFF = 0x00u,
    MAX310X_Power_ON = 0x01u
} eMAX310XPower_t;

/**
 * @enum MAX310X IRQEn
 * MAX310X IRQEn Register Settings.  Can be combined.
 */
typedef enum
{
    MAXIRQ_CTSIE = 0x80u,
    MAXIRQ_RXEMTYIEN = 0x40u,
    MAXIRQ_TFIFOEMTYIEN = 0x20u,
    MAXIRQ_TXTRGIEN = 0x10u,
    MAXIRQ_RXTRGIEN = 0x08u,
    MAXIRQ_STSIEN = 0x04u,
    MAXIRQ_SPCHRLEN = 0x02u,
    MAXIRQ_LSRERRLEN = 0x01u
} eMAX310XIRQEN_t;

/**
 * @enum MAX310X IntEn Settings
 * MAX310X IntEn Register Settings.  Can be combined.
 */
typedef enum
{
    MAXLSR_CTSBIT = 0x80u, /**< Read Only */
    MAXLSR_NOISEINTEN = 0x20u,
    MAXLSR_RBREAKIEN = 0x10u,
    MAXLSR_FRAMEERRIEN = 0x08u,
    MAXLSR_PARITYIEN = 0x04u,
    MAXLSR_ROVERRIEN = 0x02u,
    MAXLSR_RTIMEOTIEN = 0x01u
} eMAX310XLSRIntEn_t;

/**
 * @enum MAX310X MODE 1
 * MAX310X Mode 1 Register Settings.  Can be combined.
 */
typedef enum
{
    MAXMODE1_AUTOSLEEP = 0x40u,
    MAXMODE1_FORCESLEEP = 0x20u,
    MAXMODE1_TRNSCVCTRL = 0x10u,
    MAXMODE1_RTSHIZ = 0x08u,
    MAXMODE1_TXHIZ = 0x04u,
    MAXMODE1_TXDISABL = 0x02u,
    MAXMODE1_RXDISABL = 0x01u
} eMAX310XMODE1_t;

/**
 * @enum MAX310X MODE 2
 * MAX310X Mode 2 Register Settings.  Can be combined.
 *
 */
typedef enum
{
    MAXMODE2_ECHOSUPRS = 0x80u,
    MAXMODE2_MULTIDROP = 0x40u,
    MAXMODE2_LOOPBACK = 0x20u,
    MAXMODE2_SPECIALCHR = 0x10u,
    MAXMODE2_RFIFOEMTYINV = 0x08u,
    MAXMODE2_RXTRIGINV = 0x04u,
    MAXMODE2_FIFORST = 0x02u,
    MAXMODE2_RST = 0x01u
} eMAX310XMODE2_t;

/**
 *
 *
 */
typedef enum
{
    MAXLCR_NRTSBIT = 0x80u,
    MAXLCR_TXBREAK = 0x40u,
    MAXLCR_FORCEPARITY = 0x20u,
    MAXLCR_EVENPARITY = 0x10u,
    MAXLCR_PARITYEN = 0x08u,
    MAXLCR_STOPBITS = 0x04u,
    MAXLCR_LEN_8 = 0x03u,
    MAXLCR_LEN_7 = 0x02u,
    MAXLCR_LEN_6 = 0x01u,
    MAXLCR_LEN_5 = 0x00u
} eMAX310XLCR_t;

/**
 * @struct Fractional Baudrate Generator values
 * All values needed for calculating baud rate and freqs in MAX310X
 * SEE MAX310X DATASHEET FOR DESCRIPTIONS
 */
typedef struct
{
    const float fref;       /**< External Crystal Frequency */
    const uint8_t rateMode; /**< Rate Mode, See MAX310X Data Sheet */
    uint8_t DIVMSB;         /**< Integer Divisor Most Significant Bit */
    uint8_t DIVLSB;         /**< Integer Divisor Least Significant Bit */
    uint32_t BRGConfig;     /**< Fractional value BRGConfig */
    uint32_t Baudrate;
} sMAX310XBaud_t;

/**
 * @struct MAX310X Structure
 * Structure of MAX310X peripherals and settings.
 */
typedef struct
{

    const sGPIO_t nReset; /**< Reset Line, negative logic */
    const sGPIO_t cs;     /**< Chip Select */
    const sGPIO_t nIRQ;   /**< Interrupt Line, negative logic */
    const sGPIO_t nI2C;   /**< SPI/I2C Select (0 = I2C, 1 = SPI) */
    uint8_t Port;         /**< MAX310X Port (0 or 1 ) */
    sSPI_t SPI;           /**< SPI Channel */
    sMAX310XBaud_t Baud;  /**< Baudrate Structure */
} sMAX310X_t;

#ifdef UNIT_TESTING
extern sMAX310X_t MAX310X;
extern eMAX310XPower_t powerState;
#endif
/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Initialize MAX310X
 *
 * Initialize the Power Pin and UART to LiCor LI-8x0
 *
 *  @param None
 *
 *  @return None
 */
eMAX310XStatus_t MAX310X_Init(sMAX310X_t *Max);

/** @brief MAX310X Send
 *
 * Send the user string on the MAX310X UART
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param *str User string to send
 * @param len Length of user string
 *
 * @return result
 */
eMAX310XStatus_t MAX310X_Send(sMAX310X_t *Max, char *str, uint8_t len);

/** @brief MAX310X Read
 *
 * Read the string in the MAX310X Rx Buffer
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param *str Pointer to return string
 * @param *len Length of string returned
 * @param maxLen Maximum length of string to return
 *
 *  @return result
 */
eMAX310XStatus_t MAX310X_Read(sMAX310X_t *Max, char *str, uint8_t *len, uint8_t maxLen);

/** @brief Set Baudrate
 *
 * Set the baudrate to users request.
 *
 * @param *Max Pointer to the MAX310X Struct
 * @param Baudrate to set
 *
 *  @return result
 */
eMAX310XStatus_t MAX310X_SetBaudrate(sMAX310X_t *Max, uint32_t baudrate);

/** @brief Gets Baudrate
 *
 * Returns the baudrate in the MAX310X structure
 *
 * @param *Max Pointer to the MAX310X Struct
 *
 * @return baudrate
 */
uint32_t MAX310X_GetBaudrate(sMAX310X_t *Max);

/** @brief MAX310X Sleep Mode
 *
 * Puts the MAX310X IC in Sleep mode.  Sets Reset Pin, clears the CS, disables the SPI.
 *
 *  @param None
 *
 *  @return None
 */
void MAX310X_Sleep(void);

/** @brief MAX310X Wakeup
 *
 * Wakeup the MAX310X IC.  Enables SPI, Removes IC from Reset, waits for IC wakeup,
 * sets the interrupts and clears the Rx/Tx Buffers
 *
 *  @param *Max Pointer to the MAX310X Struct
 *
 *  @return None
 */
void MAX310X_Wakeup(sMAX310X_t *Max);

#endif // _MAX310X_H
