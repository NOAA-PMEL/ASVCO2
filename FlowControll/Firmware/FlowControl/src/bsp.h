/** @file bsp.h
 *  @brief Board Support Package
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 21, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note  Board Support Package for MSP430FR59xx Devices.
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
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "sysconfig.h"
#include "buffer_c.h"
#include "buffer8.h"
#include "buffer16.h"
#include "driverlib/driverlib.h"
#include "version.h"
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

typedef enum
{
  BSP_WATCHDOG = 0x00u,
  BSP_HBRIDGE_A_CS = 0x01u,
  BSP_HBRIDGE_B_CS = 0x02u,
  BSP_HBRIDGE_STRB = 0x04u,
  BSP_HBRIDGE_RESET = 0x08u,
  BSP_HBRIDGE_SLEEP = 0x10u,
  BSP_RELAY = 0x20u,
} eBSPGpioPins_t;

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
  volatile uint8_t configured : 1;   /**< Is the I2C Configured? 0=No, 1=Yes */
  volatile uint8_t enabled : 1;      /**< Is the I2C Enabled? 0=No, 1=Yes */
  volatile uint8_t interrupt : 1;    /**< Is the I2C Interrupt Enabled? 0=N0,1=Yes */
  sCircularBuffer8_t Rx;             /**< Receive Data circular buffer */
  sCircularBuffer8_t Tx;             /**< Transmit Data circular buffer */
} sI2C_t;

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

/** @brief BSP GPIO Structure 
 *  GPIO Configuration Structure.  Used for each pin instance.
 */
typedef struct
{
  uint8_t port;     /**< Port number */
  uint16_t pin;     /**< Pin number */
  uint8_t mode : 1; /*<< Pin Mode 0=Output, 1=Input */
  uint8_t configured : 1;          /**< Is the port configured 0=NO, 1=YES */
  volatile uint8_t inputValue : 1; /**< Value of Input */
  uint8_t pullups : 2;             /**< Needs resistors 0=No, 1=Pull-ups, 2=Pull-Downs */
  uint8_t interrupt : 1;           /**< Is there an interrupt attached? 0=No, 1=Yes */
  uint8_t interruptEdge : 1;       /**< Interrupt Edge 0=Low To High, 1=High to Low */
  uint8_t inverted : 1;            /**< Invert the output */
} sGPIO_t;

typedef struct
{
  volatile uint32_t ms;
  volatile uint8_t flag;
} sCount_t;
typedef struct
{
  sCount_t timer; /**< Generic ms timer */
  sCount_t spi;   /**< SPI ms timer */
  sCount_t i2c;   /**< I2C ms timer */
} sCounters_t;

/** @brief BSP Timer Structure
 *  @struct Timer Configuration Structure 
 */
typedef struct
{
  struct
  {
    uint16_t baseAddress;
    Timer_A_initUpModeParam param; /**< Timer A Parameter Structure */
    uint32_t frequency;
  } TimerA;
  struct
  {
    uint16_t baseAddress;
    Timer_B_initUpModeParam param; /**< Timer B Parameter Structure */
    uint32_t frequency;
  } TimerB;
  uint32_t frequency;
  uint8_t configured : 1; /**< Is Timer A configured?  0=N0, 1=YES */
} sTimer_t;

/**
 *  @struct BSP System Struct
 *  @brief BSP System Information
 */
typedef struct
{
  char ver[BSP_SYSTEM_VERSION_LEN]; /**< Firmware Version */
  char ser[BSP_SYSTEM_SERIAL_LEN];  /**< Serial Number */
  char mode[BSP_SYSTEM_MODE_LEN];   /**< Valve Mode Setting */
  uint8_t current;                  /**< HBridge Output Valve current setting */
  uint16_t pulse;                   /**< HBridge Time between set and clear pulse (ms) */
  uint32_t commandBaudrate; /**< Command Console Baudrate */
  uint32_t consoleBaudrate; /**< User Console Baudrate */
  uint32_t StartupCounter;  /**< Number of times System has started */
} sBSPSystemInfo_t;

/** Externs */
extern sEUSCI_t EUSCI_B0;
extern sEUSCI_t EUSCI_B1;
extern sCounters_t Timer;
extern sBSPSystemInfo_t SysInfo;

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

/** @brief Delay (milliseconds)
 * 
 * Delay the system for specified number of milliseconds.
 * 
 * @param ms Number of milliseconds to delay for
 * 
 * @return None
 */
void _delay_ms(uint32_t ms);

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
void BSP_UART_puts(sUART_t *UART, char *val, uint16_t length);

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

/** @brief BSP I2C Enable
 *  
 *  Enable the I2C specified in the struct
 * 
 *  @param *I2C pointer to I2C struct to enable
 *
 *  @return
 */
void BSP_I2C_Enable(sI2C_t *I2C);

/** @brief BSP I2C Disable
 *  
 *  Disable the I2C specified in the struct
 * 
 *  @param *I2C pointer to I2C struct to diusable
 *
 *  @return
 */
void BSP_I2C_Disable(sI2C_t *I2C);

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

/** @brief Send Multiple Bytes over I2C
 *
 *  Send multiple bytes (from circular buffer).  
 *
 *  @param *I2C pointer to I2C struct
 * 
 *  @return result
 */
eBSPStatus_t BSP_I2C_SendBytes(sI2C_t *I2C);

/** @brief Receive bytes over I2C
 *
 *  Receive multiple (or single) byte(s) over I2C Bus.  Saves to circular buffer.
 *
 *  @param *I2C pointer to I2C Struct
 *  @param len Number of bytes to receive 
 *  @param timeoute Number of cycles to wait for receive bytes
 *
 *  @return result
 */
eBSPStatus_t BSP_I2C_ReceiveBytes(sI2C_t *I2C, uint16_t len, uint32_t timeout);

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

/** @brief Send Multiple Bytes over SPI
  *
  *  Send multiple bytes (from circular buffer).  
  *
  *  @param *SPI pointer to SPI struct
  * 
  *  @return result
  */
eBSPStatus_t BSP_SPI_SendBytes(sSPI_t *SPI);

/** @brief Receive bytes over SPI
  *
  *  Receive multiple (or single) byte(s) over SPI Bus.  Saves to circular buffer.
  *
  *  @param *SPI pointer to SPI Struct
  *  @param len Number of bytes to receive 
  *  @param timeoute Number of cycles to wait for receive bytes
  *
  *  @return result
  */
eBSPStatus_t BSP_SPI_ReceiveBytes(sSPI_t *SPI, uint16_t len);

/*********************************> TIMERS <**********************************/
/** @brief Initialize Both Timers
  *
  *  Initializes both Timer A and Timer B.  Sets Timer A to 
  */
void BSP_Timer_Init(void);

/** @brief Start the timer
 * 
 * Starts the Timer
 * 
 */
void BSP_Timer_Start(void);
#endif // _BSP_H
