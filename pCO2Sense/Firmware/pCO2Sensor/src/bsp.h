/** @file bsp.h
 *  @brief Board Support Package for pCO2 Sensor
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 16, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note  Board Support Package for MSP430FR59xx Devices and pCO2 Sensor.
 *  @note Interfaces with lower level drivers to
 *
 *  @bug  No known bugs
 */
#ifndef _BSP_H
#define _BSP_H

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
#include <math.h>
#ifdef UNIT_TESTING
#include <stdio.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "sysconfig.h"
#include "buffer_c.h"
#include "buffer8.h"
#include "driverlib/driverlib.h"
#include "version.h"
#include "sysinfo.h"
/************************************************************************
 *							MACROS
 ************************************************************************/

#define SIZE_OF(x) (sizeof(x) / sizeof(x[0]))

#define BSP_LF_FREQ (32768)
#define BSP_HF_FREQ (8000000)

#define BSP_CONSOLE_UART_BASE_ADDR (EUSCI_A1_BASE)
#define BSP_COMMAND_UART_BASE_ADDR (EUSCI_A0_BASE)

#define BSP_BUS_BUFFER_LENGTH (128)

#define BSP_SYSTEM_SERIAL_LEN (32)
#define BSP_SYSTEM_VERSION_LEN (32)
#define BSP_SYSTEM_MODE_LEN (16)

/** I2C Ports */
#define BSP_I2C_B0

/** SPI Ports */
#define BSP_SPI_B1

/** Interrupts */
#define INTERRUPT_UART_A0 (0x0001u)
#define INTERRUPT_UART_A1 (0x0002u)
#define INTERRUPT_UART_A2 (0x0004u)
#define INTERRUPT_UART_A3 (0x0008u)
#define INTERRUPT_I2C_B0 (0x0010u)
#define INTERRUPT_SPI_B1 (0x0020u)

#define INTERRUPT_GPIO_P1_4 (0x0100u)
#define INTERRUPT_GPIO_P1_5 (0x0200u)

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum BSP Status
 *  @brief BSP Status
 */
typedef enum
{
  BSP_OK = 0u,
  BSP_FAIL = 1u
} eBSPStatus_t;

/**
 *  @struct BSP Clock Peripheral Struct
 *  SYSCLK Perihperal clock sources and settings
 */
typedef struct
{
  const uint32_t ClkFreq; /**< Clock Signal Frequency */
  const uint8_t ClkSrc;   /**< Clock Signal Source */
  const uint16_t divider; /**< Clock signal Divider */
  const uint16_t drive;   /**< Clock Drive value */
} sPeripheralClk_t;

/**
 *  @struct BSP SYSCLK Oscillator Struct
 *  Oscillator settings
 */
typedef struct
{
  const uint32_t Freq;    /**< Oscillator Frequency */
  const uint16_t Drive;   /**< Oscillator Drive Settings */
  const uint32_t timeout; /**< Startup Timeout (#cycles) */
} sOSC_t;

/**
 *  @struct BSP SYSCLK Structure
 *  SYSCLK Variable structure.  Used to configure system clocks
 */
typedef struct
{
  const sOSC_t HX;               /**< High Frequency Crystal Settings */
  const sOSC_t LX;               /**< Low Frequency Crystal Settings */
  const sPeripheralClk_t ACLK;   /**< ACLK settings */
  const sPeripheralClk_t MCLK;   /**< MCLK Settings */
  const sPeripheralClk_t SMCLK;  /**< SMCLK Settings */
  const sPeripheralClk_t MODOSC; /**< ModOsc Settings */
} sCLK_t;

/**
 *  @struct BSP UART Structure
 *  UART Variable and Data Structure.  Used for each instance of UART.
 */
typedef struct
{
  EUSCI_A_UART_initParam param;
  sCircularBufferC_t buf;          /**< EUSCI UART A Structure of settings */
  uint16_t baseAddress;            /**< UART Ax Base Address */
  uint32_t baudrate;               /**< Selected baudrate */
  volatile uint8_t configured : 1; /**< Is the UART configured? 0=No, 1=Yes */
  volatile uint8_t enabled : 1;    /**< Is the UART enabled?  0=No, 1=Yes */
  volatile uint8_t interrupt : 1;  /**< Is the Interrupt Enabled?  0=No, 1=Yes */
  volatile uint8_t msg : 1;        /**< Message waiting? */
} sUART_t;

/**
 *
 *  @struct BSP Bus Structure Buffers
 *  Tx and Rx Buffer for BSP EUSCI Bus structure
 */
typedef struct
{
  uint8_t buf[BSP_BUS_BUFFER_LENGTH]; /** Data Buffer */
  uint8_t ByteCnt;                    /** Byte Count remaining */
} sEUSCIData_t;

/**
 *  @struct BSP I2C/SPI Bus Structure
 *  I2C/SPI Bus structure for EUSCI bus port.
 */
typedef struct
{
  const uint16_t baseAddress;     /**< Base address for the port */
  const uint32_t timeout;         /**< Timeout for buffer */
  sEUSCIData_t Tx;                /**< Transmit Buffer */
  sEUSCIData_t Rx;                /**< Receive Buffer */
  volatile uint8_t direction : 1; /**< Set bus direction 0=Transmit, 1=Receive */
  volatile uint8_t busy : 1;      /**< Busy flag 0=Available, 1=Busy */
  volatile uint8_t complete : 1;  /**< Complete flag 0=Not Complete, 1=Complete */
  volatile uint8_t success : 1;   /**< Successful Tx/Rs 0=No, 1=Yes */
} sEUSCI_t;

/**
 *  @struct BSP I2C Structure
 *  I2C Variable and Data Structure for subsystems.  Used for each instance of I2C
 */
typedef struct
{
  EUSCI_B_I2C_initMasterParam param; /** EUSCI B I2C Structure of settings */
  uint16_t baseAddress;              /**< I2C Bx Base Address */
  uint8_t slaveAddress;              /**< Slave Address communicating with */
  uint8_t mode;                      /**< Mode 0=Receive, >0=Transmit*/
  uint32_t timeout;                  /**< Tx & Rx Timeout */
  volatile uint8_t configured : 1;   /**< Is the I2C Configured? 0=No, 1=Yes */
  volatile uint8_t enabled : 1;      /**< Is the I2C Enabled? 0=No, 1=Yes */
  volatile uint8_t interrupt : 1;    /**< Is the I2C Interrupt Enabled? 0=N0,1=Yes */
  sCircularBuffer8_t Rx;             /**< Receive Data circular buffer */
  sCircularBuffer8_t Tx;             /**< Transmit Data circular buffer */
} sI2C_t;

/**
 *  @struct BSP SPI Structure
 *  SPI Variable and Data structure for subsystem use.  Used for each instance of SPI
 */
typedef struct
{
  EUSCI_B_SPI_initMasterParam param; /** EUSCI B I2C Structure of settings */
  uint16_t baseAddress;              /**< SPI Bx Base Address */
  uint8_t slaveAddress;              /**< Slave Address communicating with */
  volatile uint8_t mode;             /**< Mode 0=Receive, 1=Tx With Rx >0=Transmit*/
  volatile uint8_t configured : 1;   /**< Is the SPI Configured? 0=No, 1=Yes */
  volatile uint8_t enabled : 1;      /**< Is the SPI Enabled? 0=No, 1=Yes */
  volatile uint8_t interrupt : 1;    /**< Is the SPI InterruptEnabled? 0=N0,1=Yes */
  sCircularBuffer8_t Rx;             /**< Receive Data circular buffer */
  sCircularBuffer8_t Tx;             /**< Transmit Data circular buffer */
} sSPI_t;

/**
 *  @struct BSP Analog Structure
 *  Analog Variable and Data Structure for subsystem use.  Used for each instance of Analog
 */
typedef struct
{
  ADC12_B_initParam param;
  ADC12_B_configureMemoryParam config;
  uint16_t baseAddress;
  volatile uint8_t configured : 1;
  volatile uint8_t enabled : 1;
  volatile uint8_t interrupt : 1;
  float refVoltage; /**< Reference voltage of ADC */

} sAnalog_t;

/** @brief BSP GPIO Structure
 *  GPIO Configuration Structure.  Used for each pin instance.
 */
typedef struct
{
  uint8_t port;                    /**< Port number */
  uint16_t pin;                    /**< Pin number */
  uint8_t mode : 1;                /*<< Pin Mode 0=Output, 1=Input */
  uint8_t configured : 1;          /**< Is the port configured 0=NO, 1=YES */
  volatile uint8_t inputValue : 1; /**< Value of Input */
  uint8_t pullups : 2;             /**< Needs resistors 0=No, 1=Pull-ups, 2=Pull-Downs */
  uint8_t interrupt : 1;           /**< Is there an interrupt attached? 0=No, 1=Yes */
  uint8_t interruptEdge : 1;       /**< Interrupt Edge 0=Low To High, 1=High to Low */
  uint8_t inverted : 1;            /**< Invert the output */
} sGPIO_t;

/** @brief Timer Count Structures
 * @struct Timer Count Structure
 */
typedef struct
{
  volatile uint32_t ms;  /**< millisecond countdown */
  volatile uint8_t flag; /**< Complete flag */
} sCount_t;

/** @brief Counter Structure
 * @struct Counter Type Structure
 */
typedef struct
{
  sCount_t timer; /**< Generic ms timer */
  sCount_t spi;   /**< SPI ms timer */
  sCount_t i2c;   /**< I2C ms timer */
} sCounters_t;

/** @brief BSP Timer A Structure
 *  @struct Timer A Configuration Structure
 */
typedef struct
{
  uint16_t baseAddress;
  Timer_A_initUpModeParam param; /**< Timer A Parameter Structure */
  uint32_t frequency;
  uint8_t configured : 1; /**< Is Timer A configured?  0=N0, 1=YES */
} sTimerA_t;

/** @brief BSP Timer B Structure
 *  @struct Timer B Configuration Structure
 */
typedef struct
{
  uint16_t baseAddress;
  Timer_B_initUpModeParam param; /**< Timer B Parameter Structure */
  uint32_t frequency;
  uint8_t configured : 1; /**< Is Timer A configured?  0=N0, 1=YES */
} sTimerB_t;

/**
 *  @struct BSP System Struct
 *  @brief BSP System Information
 */
typedef struct
{
  char ver[BSP_SYSTEM_VERSION_LEN]; /**< Firmware Version */
  char ser[BSP_SYSTEM_SERIAL_LEN];  /**< Serial Number */
  uint32_t StartupCounter;          /**< Number of times System has started */
} sBSPSystemInfo_t;

/** Externs */
extern sEUSCI_t EUSCI_A0;
extern sEUSCI_t EUSCI_A1;
extern sEUSCI_t EUSCI_A2;
extern sEUSCI_t EUSCI_A3;
extern sEUSCI_t EUSCI_B0;
extern sEUSCI_t EUSCI_B1;
extern sEUSCI_t EUSCI_B2;
extern sEUSCI_t EUSCI_B3;
extern sCounters_t Timer;

/************************************************************************
 *					    Function Prototypes
 ************************************************************************/

/** @brief Initialize the Board
 *
 *  Initialize all peripherals and pins
 *
 *  @param None
 *
 *  @return None
 */
void BSP_Init_Board(void);

/***********************************> CLK <***********************************/
/** @brief Initialize the System Clock
 *
 *  Initialize the system clock and all clock signals
 *
 *  @param None
 *
 *  @return result
 */
eBSPStatus_t BSP_SYSCLK_Init(void);

/** @brief Put Board in Low Power Mode
 *
 * Puts the board in lowest power mode until next command
 *
 * @param None
 *
 * @return None
 */

void BSP_Low_Power_Mode(void);

/** @brief Puts the Power Supply in Burst Mode
 *
 */
void BSP_Low_Current_Mode_ON(void);

/** @brief Puts the Power Supply in PWM Mode
 *
 */
void BSP_Low_Current_Mode_OFF(void);

/** @brief Get the date string
 *
 * Get the current date string
 *
 * @param *datestr Pointer to date string
 *
 * @return None
 */
void _get_date(char *datestr);

/** @brief Delay milli-seconds
 *
 * Delays the device # of milli-second
 *
 * @param ms Number of milli-seconds to delay
 *
 * @return None
 */
void _delay_ms(uint32_t ms);

/** @brief Delay micro-seconds
 *
 * Delays the device # of micro-seconds
 *
 * @param ms Number of micro-seconds to delay
 *
 * @return None
 */
void _delay_us(uint32_t us);

/** @brief Delay 1 second
 *
 * Delays the device # of second
 *
 * @param ms Number of seconds to delay
 *
 * @return None
 */
void _delay_sec(uint32_t sec);

/**********************************> GPIO <***********************************/
/** @brief BSP GPIO Initialize
 *
 *  BSP GPIO Initialize Pin function.  Sets up pin based on what is populated in
 *  the structure
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return result
 */
void BSP_GPIO_Init(sGPIO_t *IO);

/** @brief BSP GPIO Set Pin
 *
 *  BSP GPIO Set Pin.  Set's output high if not inverted, low if inverted.
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return None
 */
void BSP_GPIO_SetPin(sGPIO_t *IO);
/** @brief BSP GPIO Clear Pin
 *
 *  BSP GPIO Clear Pin.  Set's output low if not inverted, high if inverted.
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return None
 */
void BSP_GPIO_ClearPin(sGPIO_t *IO);

/** @brief BSP GPIO Read Pin
 *
 *  BSP GPIO Read Pin.  Read the pin (if it is configured to be an input).
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return Pin value
 */
int8_t BSP_GPIO_ReadInputPin(sGPIO_t *IO);

/** @brief BSP GPIO Enable Interrupt
 *
 *  BSP GPIO Enable Interrupt Pin.  Sets Pin on Selected Pin
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return None
 */
void BSP_GPIO_EnableInterrupt(sGPIO_t *IO);

/** @brief BSP GPIO Disable Interrupt
 *
 * BSP GPIO Disable Interrupt Pin.  Clears interrupt on selected pin.
 *
 * @param *IO Pointer to the Pin Structure
 *
 * @return None
 */
void BSP_GPIO_DisableInterrupt(sGPIO_t *IO);

/** @brief BSP GPIO Clear Interrupt
 *
 *  BSP GPIO Clear Interrupt Pin.  Clears Pin on Selected Pin
 *
 *  @param *IO Pointer to Pin Structure
 *
 *  @return None
 */
void BSP_GPIO_ClearInterrupt(sGPIO_t *IO);

/** @brief BSP GPIO Peripheral Output Setup
 *
 *  Set up BSP GPIO Peripheral to output function
 *
 *  @param port Port of peripheral
 *  @param pin Pin of peripheral
 *  @param mode Peripheral Mode
 *
 *  @return None
 */
void BSP_GPIO_SetPeripheralOutputPin(uint8_t port, uint16_t pin, uint16_t mode);

/** @brief BSP GPIO Peripheral Input Setup
 *
 *  Set up BSP GPIO Peripheral to Input function
 *
 *  @param port Port of peripheral
 *  @param pin Pin of peripheral
 *  @param mode Peripheral Mode
 *
 *  @return None
 */
void BSP_GPIO_SetPeripheralInputPin(uint8_t port, uint16_t pin, uint16_t mode);

/**********************************> UART <***********************************/
/** @brief Initialize the UART
 *
 *  Initialize the UART specified in the struct.
 *
 *  @param *UART pointer to UART struct to initialize
 *
 *  @return result
 */
eBSPStatus_t BSP_UART_Init(sUART_t *UART);

/** @brief Change the baudrate
 *
 * Change the selected baudrate
 * @param *UART pointer to UART struct to change baudrate of
 * @param baudrate Baudrate to set selected UART to.
 *
 * @return result
 */
eBSPStatus_t BSP_UART_SetBaudrate(sUART_t *UART, uint32_t baudrate);

/** @brief Get the baudrate
 *
 * Get the UART baudrate currently set
 *
 * @return baudrate
 */
uint32_t BSP_UART_GetBaudrate(sUART_t *UART);

/** @brief Enable the UART
 *
 *  Enable the UART specified in the struct.
 *
 *  @param *UART pointer to UART struct to enable
 *
 *  @return None
 */
void BSP_UART_Enable(sUART_t *UART);

/** @brief Initialize the UART
 *
 *  Disable the UART specified in the struct.
 *
 *  @param *UART pointer to UART struct to disable
 *
 *  @return None
 */
void BSP_UART_Disable(sUART_t *UART);

/** @brief Enable the UART Interrupt
 *
 *  Enable the UART interrupt specified.
 *
 *  @param *UART pointer to UART struct to enable
 *  @param mask Interrupt enable mask
 *
 *  @return None
 */
void BSP_UART_EnableInterrupt(sUART_t *UART, uint8_t mask);

/** @brief Disable the UART Interrupt
 *
 *  Disable the UART interrupt specified.
 *
 *  @param *UART pointer to UART struct to disable
 *  @param mask Interrupt disable mask
 *
 *  @return None
 */
void BSP_UART_DisableInterrupt(sUART_t *UART, uint8_t mask);

/** @brief BSP UART Putc
 *
 *  Send character on UART
 *
 *  @param *UART pointer to UART struct to send on
 *  @param val Character value to send
 *
 *  @return None
 */
void BSP_UART_putc(sUART_t *UART, char val);

/** @brief BSP UART Putc
 *
 *  Send string on UART
 *
 *  @param *UART pointer to UART Struct to send on
 *  @param *val String to send
 *  @param length Length of string to send
 *
 *  @return None
 */
void BSP_UART_puts(sUART_t *UART, const char *val, uint16_t length);

/** @brief BSP UART gets
 *
 *  Get string from UART.  Retrieves a line from the UART circular buffer.
 *
 *  @param *UART Pointer to UART struct to read string(line) from
 *  @param *str Pointer to Char array to put result in
 *
 *  @return length of String being returned
 */
uint16_t BSP_UART_gets(sUART_t *UART, char *str);

/***********************************> I2C <***********************************/
/** @brief BSP I2C Init
 *
 *  Initialize the I2C specified in the struct.
 *
 *  @param *I2C pointer to I2C struct to initialize
 *
 *  @return None
 */
void BSP_I2C_Init(sI2C_t *I2C);

/** @brief BSP I2C Reset
 *
 * Reset the I2C port
 *
 * @param *I2C pointer to the I2c struct to reset
 *
 * @return None
 */
void BSP_I2C_Reset(sI2C_t *I2C);

/** @brief BSP I2C Enable
 *
 *  Enable the I2C specified in the struct
 *
 *  @param *I2C pointer to I2C struct to enable
 *
 *  @return
 */
void BSP_I2C_Enable(sI2C_t *I2C, uint8_t mode);

/** @brief BSP I2C Set Slave Address
 *
 *  Set the slave address the Master is communicating with
 *
 *  @param *I2C pointer to the I2C Struct
 *  @param addr Slave address to send to
 *
 *  @return None
 */
void BSP_I2C_SetSlaveAddress(sI2C_t *I2C, uint8_t addr);

/** @brief BSP I2C Set Mode
 *
 *  Set the I2C mode to transmit or receive
 *
 *  @param *I2C pointer to the I2C struct
 *  @param mode Mode to set.  0=Receive, UCTR = Transmit
 *
 *  @return None
 */
void BSP_I2C_SetMode(sI2C_t *I2C, uint8_t mode);

/** @brief BSP I2C Get Data
 *
 * Get Data from I2C
 *
 * @param *I2C Pointer to the I2C Struct
 * @param reg Register to read from
 * @param *data Pointer to data array to read
 * @param len Maximum length of array
 *
 * @return Result
 */
eBSPStatus_t BSP_I2C_GetData(sI2C_t *I2C, uint8_t reg, uint8_t *data, uint8_t len);

/** @brief BSP I2C Send Data
 *
 * Send Data over I2C
 *
 * @param *I2C Pointer to the I2C Struct
 * @param reg Register to read from
 * @param *val Pointer to data array to write
 * @param len Maximum length of array
 *
 * @return Result
 */
eBSPStatus_t BSP_I2C_SendData(sI2C_t *I2C, uint8_t reg, uint8_t *val, uint16_t len);

/** @brief Write single value to I2C
 *
 * Send a single integer over I2C.
 *
 * @param *I2C Pointer to I2C Struct
 * @param val Value to write
 *
 * @return None
 */
void BSP_I2C_put(sI2C_t *I2C, uint8_t val);

/** @brief Write value array to I2C
 *
 * Send a multiple integers over I2C.
 *
 * @param *I2C Pointer to I2C Struct
 * @param *val Value array to write
 *
 * @return None
 */
void BSP_I2C_puts(sI2C_t *I2C, uint8_t *val, uint16_t len);

/** @brief Read single value to I2C
 *
 * Read a single integer over I2C.
 *
 * @param *I2C Pointer to I2C Struct
 *
 * @return Value read
 */
uint8_t BSP_I2C_get(sI2C_t *I2C);

/** @brief Read value array to I2C
 *
 * Read multiple integers over I2C.
 *
 * @param *I2C Pointer to I2C Struct
 * @param *val Value array to read to
 * @param len Maximum length of array
 *
 * @return result
 */
eBSPStatus_t BSP_I2C_gets(sI2C_t *I2C, uint8_t *val, uint16_t len);

/***********************************> SPI <***********************************/
/** @brief BSP SPI Init
 *
 *  Initialize the SPI specified in the struct.
 *
 *  @param *SPI pointer to SPI struct to initialize
 *
 *  @return None
 */
void BSP_SPI_Init(sSPI_t *SPI);

/** @brief BSP SPI Enable
 *
 *  Enable the SPI specified in the struct
 *
 *  @param *SPI pointer to SPI struct to enable
 *
 *  @return
 */
void BSP_SPI_Enable(sSPI_t *SPI);

/** @brief BSP SPI Disable
 *
 *  Disable the SPI specified in the struct
 *
 *  @param *SPI pointer to SPI struct to diusable
 *
 *  @return
 */
void BSP_SPI_Disable(sSPI_t *SPI);

/** @brief SPI Put Single Byte
 *
 * Transmit single byte to SPI Device
 *
 * @param *SPI Pointer to SPI Struct
 * @param val Value to send
 *
 * @return None
 */
void BSP_SPI_put(sSPI_t *SPI, uint8_t val);

/** @brief SPI Put Mulitple Bytes
 *
 * Transmit multiple bytes to SPI Device.
 *
 * @param *SPI Pointer to SPI Struct
 * @param *val Pointer to array of values to send
 * @param len Length of data array to send
 *
 * @return None
 */
void BSP_SPI_puts(sSPI_t *SPI, uint8_t *val, uint16_t len);

/** @brief SPI Get Byte
 *
 * Receive single byte from SPI device.
 *
 * @param *SPI Pointer to SPI Struct
 * @param addr Register address to receive data from
 *
 * @return Received Byte
 */
uint8_t BSP_SPI_get(sSPI_t *SPI, uint8_t addr);

/** @brief SPI Get Multiple Bytes
 *
 * Recieve multiple bytes from SPI device.
 *
 * @param *SPI Pointer to SPI Structure
 * @param addr Register Address to receive data from
 * @param *val Pointer to data array to save to
 * @param len Length of data to retreive
 *
 * @return None
 */
void BSP_SPI_gets(sSPI_t *SPI, uint8_t addr, uint8_t *val, uint16_t len);

/** @brief SPI Set Clock Speed
 *
 * Change clock speed of SPI to selected
 *
 * @param *SPI Pointer to SPI Structure
 * @param speed Clock speed desired
 *
 * @return none
 */
void BSP_SPI_Set_ClkSpeed(sSPI_t *SPI, uint32_t speed);

/*********************************> ANALOG <***********************************/
/** @brief Initialize Analog Pin
 *
 * Initialize the Selected Analog Pin
 *
 *  @param *Analog pointer to Analog Struct
 *  @param
 *  @param
 *
 *  @return result
 */
eBSPStatus_t BSP_ANALOG_Init(sAnalog_t *Analog);

/** @brief Read Analog Pin
 *
 * Read the Analog pin
 *
 * @param *Analog pointer to the Analog Struct
 * @param
 *
 * @return voltage on Pin
 */
float BSP_ANALOG_ReadVoltage(sAnalog_t *Analog);

/*********************************> TIMERS <**********************************/
/** @brief Initialize Both Timers
 *
 *  Initializes both Timer A and Timer B.  Sets Timer A to
 *
 *  @param None
 *
 *  @return None
 */
void BSP_Timer_Init(sTimerA_t *A, sTimerB_t *B);

/** @brief Initialize the Watchdog
 *
 * Initialize the watchdog pin
 *
 * @param None
 *
 * @return None
 */
void BSP_Init_WD(void);

/** @brief Hit the watchdog
 *
 * Toggle the watchdog pin
 *
 * @param None
 *
 * @return None
 */
void BSP_Hit_WD(void);
#endif // _BSP_H
