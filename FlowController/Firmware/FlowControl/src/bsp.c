/** @file BSP.c
 *  @brief Board Support Package
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 21st, 2017
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
#include "bsp.h"

/************************************************************************
*			                Static Functions Prototypes
************************************************************************/
STATIC eBSPStatus_t CalculateUartBaudRegs(sUART_t *UART);
STATIC void ClearBusBufferForTransmit(sEUSCI_t *temp);
STATIC void ClearBusBufferForReceive(sEUSCI_t *temp);
//STATIC void DisableInterrupt(uint16_t interrupt);
//STATIC void EnableInterrupt(uint16_t interrupt);
STATIC void ClearEUSCI(sEUSCI_t *port);
/************************************************************************
*			                    Variables (Global)
************************************************************************/
sEUSCI_t EUSCI_B0 = {.baseAddress = EUSCI_B0_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B0 Bus (AMBIQ RTC I2C BUS) */
sEUSCI_t EUSCI_B1 = {.baseAddress = EUSCI_B1_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B1 Bus (DRV8823 HBridge SPI BUS) */

sCounters_t Timer = {.timer = {.ms = 0, .flag = true},
                     .spi = {.ms = 0, .flag = true},
                     .i2c = {.ms = 0, .flag = true}};
/************************************************************************
*			                Variables (Static in Module)
************************************************************************/
//STATIC uint8_t UART_A0_Mask = 0;
//STATIC uint8_t UART_A1_Mask = 0;
//STATIC uint8_t UART_A2_Mask = 0;
//STATIC uint8_t UART_A3_Mask = 0;
//STATIC uint8_t I2C_B0_Mask = 0;
STATIC uint8_t SPI_B1_Mask = 0;
//STATIC uint8_t GPIO_P1_4_Mask = 0;
//STATIC uint8_t GPIO_P1_5_Mask = 0;

/** System Clock */
const sCLK_t SysClk = {.HX = {.Freq = HFXT_FREQ, .Drive = HFXT_DRIVE, .timeout = HFXT_TIMEOUT},
                       .LX = {.Freq = LFXT_FREQ, .Drive = LFXT_DRIVE, .timeout = LFXT_TIMEOUT},
                       .ACLK = {.ClkFreq = ACLK_FREQ, .ClkSrc = ACLK_SRC, .divider = ACLK_DIV},
                       .MCLK = {.ClkFreq = MCLK_FREQ, .ClkSrc = MCLK_SRC, .divider = MCLK_DIV},
                       .SMCLK = {.ClkFreq = SMCLK_FREQ, .ClkSrc = SMCLK_SRC, .divider = SMCLK_DIV},
                       .MODOSC = {.ClkFreq = MODOSC_FREQ, .ClkSrc = MODOSC_SRC, .divider = MODOSC_DIV}};

/** System Info */
PERSISTENT sBSPSystemInfo_t SysInfo = {.ver = VERSION, .ser = SERIAL};

STATIC const float BAUD_N[] = {0, 0.0529, 0.0715, 0.0835, 0.1001, 0.1252, 0.143, 0.167, 0.2147, 0.2224, 0.2503, 0.3, 0.3335, 0.3575, 0.3753, 0.4003, 0.4286, 0.4378, 0.5002, 0.5715, 0.6003, 0.6254, 0.6432, 0.6667, 0.7001, 0.7147, 0.7503, 0.7861, 0.8004, 0.8333, 0.8464, 0.8572, 0.8751, 0.9004, 0.917, 0.9288};
STATIC const uint8_t BAUD_BRS[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x11, 0x21, 0x22, 0x44, 0x25, 0x49, 0x4A, 0x52, 0x92, 0x53, 0x55, 0xAA, 0x6B, 0xAD, 0xB5, 0xB6, 0xD6, 0xB7, 0xBB, 0xDD, 0xED, 0xEE, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFF};
STATIC const uint8_t BAUD_BRS_LEN = SIZE_OF(BAUD_BRS);

/************************************************************************
*					                   Functions
************************************************************************/
/**********************************> INIT <***********************************/
void BSP_Init_Board(void)
{
  uint16_t pinMask = (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                      GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                      GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                      GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

  /** Clear all ports to Low Power State */
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P3, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P4, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P5, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P6, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P7, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P8, pinMask);

  /** Set Pins High-Z */
  GPIO_setAsInputPin(GPIO_PORT_P1, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P2, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P3, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P4, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P5, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P6, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P7, pinMask);
  GPIO_setAsInputPin(GPIO_PORT_P8, pinMask);

  /** Set the Clock Power Pin To Output and On */
  GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN7);
  GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN7);

  /** Set Power to Command Port */
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);

#ifndef UNIT_TESTING
  //  _delay_ms(SYS_BOARD_INIT_DELAY);
#endif

  /** Set Input pins for HFXT */
  GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

  /** Set the SPI Port Pins */
  GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_B1_CLK_PORT, SPI_B1_CLK_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_B1_SIMO_PORT, SPI_B1_SIMO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_B1_SOMI_PORT, SPI_B1_SOMI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

  /** Set the I2C Port Pins */

  /** Set the UART Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A1_RX_PORT, UART_A1_RX_PIN, GPIO_SECONDARY_MODULE_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A1_TX_PORT, UART_A1_TX_PIN, GPIO_SECONDARY_MODULE_FUNCTION);

  /** Set Pump Output High */
  GPIO_setOutputHighOnPin(SYS_RELAY_TOGGLE_PORT, SYS_RELAY_TOGGLE_PIN);

/*
   * Disable the GPIO power-on default high-impedance mode to activate
   * previously configured port settings
   */
#ifndef UNIT_TESTING
  PMM_unlockLPM5();
#endif

  /** Update Startup Counter */
  SysInfo.StartupCounter++;

  return;
}
/***********************************> CLK <***********************************/
eBSPStatus_t BSP_SYSCLK_Init(void)
{
  eBSPStatus_t result = BSP_FAIL;

  /** Set Clock static values */
  CS_setExternalClockSource(SysClk.LX.Freq, SysClk.HX.Freq);

  /** Initialize ACLK */
  CS_initClockSignal(CS_ACLK, SysClk.ACLK.ClkSrc, SysClk.ACLK.divider);

  /** Initialize MCLK */
  CS_initClockSignal(CS_MCLK, SysClk.MCLK.ClkSrc, SysClk.MCLK.divider);

  /** Initialize SMCLK */
  CS_initClockSignal(CS_SMCLK, SysClk.SMCLK.ClkSrc, SysClk.SMCLK.divider);

  /** Initialize MODCLK */
  CS_initClockSignal(CS_MODOSC, SysClk.MODOSC.ClkSrc, SysClk.MODOSC.divider);

  /** Turn High Frequency Oscillator On with a timeout */
  if (SysClk.HX.Freq > 0)
  {
    if (CS_turnOnHFXTWithTimeout(SysClk.HX.Drive, SysClk.HX.timeout) == STATUS_SUCCESS)
    {
      result = BSP_OK;
    }
  }
  else
  {
  }

  /** Turn Low Frequency Oscillator ON with a timeout */
  if (SysClk.LX.Freq > 0)
  {
    if (CS_turnOnLFXTWithTimeout(SysClk.LX.Drive, SysClk.LX.timeout) == STATUS_SUCCESS)
    {
      result = BSP_OK;
    }
  }
  else
  {
    CS_turnOffLFXT();
  }
  return result;
}

void _delay_ms(uint32_t ms)
{
#ifndef UNIT_TESTING
  Timer.timer.flag = false;
  Timer.timer.ms = ms;
  while (Timer.timer.flag == false)
    ;
#endif
}

/**********************************> GPIO <***********************************/
void BSP_GPIO_Init(sGPIO_t *IO)
{

  /** Is the pin an output or input? */
  if (IO->mode == 0)
  {
    GPIO_setAsOutputPin(IO->port, IO->pin);
  }
  else
  {
    /** needs pull-ups? */
    if (IO->pullups == 0)
    {
      GPIO_setAsInputPin(IO->port, IO->pin);
    }
    else if (IO->pullups == 1)
    {
      GPIO_setAsInputPinWithPullUpResistor(IO->port, IO->pin);
    }
    else if (IO->pullups == 2)
    {
      GPIO_setAsInputPinWithPullDownResistor(IO->port, IO->pin);
    }
    else
    {
      /** @todo ADD ERROR */
    }
  }

  /** Set the Interrupt Edge */
  GPIO_selectInterruptEdge(IO->port, IO->pin, IO->interruptEdge);

  IO->configured = true;
  return;
}

void BSP_GPIO_SetPin(sGPIO_t *IO)
{
  if (IO->mode == 0)
  {
    if (IO->inverted == 0)
    {
      GPIO_setOutputHighOnPin(IO->port, IO->pin);
    }
    else
    {
      GPIO_setOutputLowOnPin(IO->port, IO->pin);
    }
  }
  return;
}

void BSP_GPIO_ClearPin(sGPIO_t *IO)
{
  if (IO->mode == 0)
  {
    if (IO->inverted == 0)
    {
      GPIO_setOutputLowOnPin(IO->port, IO->pin);
    }
    else
    {
      GPIO_setOutputHighOnPin(IO->port, IO->pin);
    }
  }
}

int8_t BSP_GPIO_ReadInputPin(sGPIO_t *IO)
{
  int8_t result = 0;
  if (IO->mode == 1)
  {
    result = GPIO_getInputPinValue(IO->port, IO->pin);
    IO->inputValue = result;
  }
  else
  {
    /** Return -1 since this is an output pin */
    result = -1;
  }
  return result;
}

void BSP_GPIO_EnableInterrupt(sGPIO_t *IO)
{
  GPIO_enableInterrupt(IO->port, IO->pin);
  IO->interrupt = 1;
}

void BSP_GPIO_ClearInterrupt(sGPIO_t *IO)
{
  GPIO_clearInterrupt(IO->port, IO->pin);
  IO->interrupt = 0;
}

void BSP_GPIO_SetPeripheralOutputPin(uint8_t port, uint16_t pin, uint16_t mode)
{
  GPIO_setAsPeripheralModuleFunctionOutputPin(port, pin, mode);
}

void BSP_GPIO_SetPeripheralInputPin(uint8_t port, uint16_t pin, uint16_t mode)
{
  GPIO_setAsPeripheralModuleFunctionInputPin(port, pin, mode);
}

/**********************************> UART <***********************************/
eBSPStatus_t BSP_UART_Init(sUART_t *UART)
{
  eBSPStatus_t result = BSP_FAIL;

  UART->param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
  UART->param.parity = EUSCI_A_UART_NO_PARITY;
  UART->param.msborLsbFirst = EUSCI_A_UART_ONE_STOP_BIT;
  UART->param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
  UART->param.uartMode = EUSCI_A_UART_MODE;

  if (CalculateUartBaudRegs(UART) != BSP_OK)
  {
    /** @todo ADD Bad value error here, unable to set UART */
    result = BSP_FAIL;
  }
  else
  {
    result = BSP_OK;

    if (EUSCI_A_UART_init(UART->baseAddress, &UART->param) == STATUS_FAIL)
    {
      UART->configured = false;
      result = BSP_FAIL;
      /** @todo ADD UART Init Error */
    }
    else
    {
      UART->configured = true;
    }
  }

  return result;
}

void BSP_UART_Enable(sUART_t *UART)
{
  EUSCI_A_UART_enable(UART->baseAddress);
  return;
}

void BSP_UART_Disable(sUART_t *UART)
{
  EUSCI_A_UART_disable(UART->baseAddress);
  return;
}

void BSP_UART_EnableInterrupt(sUART_t *UART, uint8_t mask)
{
  EUSCI_A_UART_enableInterrupt(UART->baseAddress, mask);
  return;
}

void BSP_UART_DisableInterrupt(sUART_t *UART, uint8_t mask)
{
  EUSCI_A_UART_disableInterrupt(UART->baseAddress, mask);
  return;
}

void BSP_UART_ClearInterrupt(sUART_t *UART, uint8_t mask)
{
  EUSCI_A_UART_clearInterrupt(UART->baseAddress, mask);
}

void BSP_UART_putc(sUART_t *UART, char val)
{
  EUSCI_A_UART_transmitData(UART->baseAddress, (uint8_t)val);
  return;
}

void BSP_UART_puts(sUART_t *UART, char *val, uint16_t length)
{
  uint16_t i;

  for (i = 0; i < length; i++)
  {
    BSP_UART_putc(UART, val[i]);
  }

  while (EUSCI_A_UART_queryStatusFlags(UART->baseAddress, EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG) > 0)
    ;

  return;
}

uint16_t BSP_UART_gets(sUART_t *UART, char *str)
{
  uint16_t i = 0;

  if (UART->buf.size > 0)
  {
    for (i = 0; i < SYS_UART_BUFFER_LEN; i++)
    {
      BufferC_getc(&UART->buf, &str[i]);
      if (str[i] == '\n')
      {
        i++;
        break;
      }
    }
  }
  return i;
}

/***********************************> I2C <***********************************/
void BSP_I2C_Init(sI2C_t *I2C)
{
  /** All I2C configured for running on SMCLK */
  I2C->param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
  I2C->param.i2cClk = SysClk.SMCLK.ClkFreq;

  EUSCI_B_I2C_initMaster(I2C->baseAddress, &I2C->param);
  return;
}

void BSP_I2C_Enable(sI2C_t *I2C)
{
  EUSCI_B_I2C_enable(I2C->baseAddress);
  return;
}

void BSP_I2C_Disable(sI2C_t *I2C)
{
  EUSCI_B_I2C_disable(I2C->baseAddress);
  return;
}

void BSP_I2C_SetSlaveAddress(sI2C_t *I2C, uint8_t addr)
{
  I2C->slaveAddress = addr;
  EUSCI_B_I2C_setSlaveAddress(I2C->baseAddress, addr);
  return;
}

void BSP_I2C_SetMode(sI2C_t *I2C, uint8_t mode)
{
  I2C->mode = mode;
  EUSCI_B_I2C_setMode(I2C->baseAddress, mode);
}

eBSPStatus_t BSP_I2C_SendBytes(sI2C_t *I2C)
{
  eBSPStatus_t result = BSP_FAIL;
  uint16_t i;
  uint16_t len = I2C->Tx.size;
  /** check for a valid transmit length */
  if (I2C->Tx.size < 1)
  {
    result = BSP_OK;
  }
  else
  {
    switch (I2C->baseAddress)
    {
#ifdef BSP_I2C_B0
    case EUSCI_B0_BASE:
      EUSCI_B0.Tx.ByteCnt = I2C->Tx.size;

      /** Clear the Bus Transmit Buffer */
      ClearBusBufferForTransmit(&EUSCI_B0);

      /** Transfer I2C buffer to EUSCI buffer */
      for (i = 0; i < len; i++)
      {
        Buffer8_get(&I2C->Tx, &EUSCI_B0.Tx.buf[i]);
      }

      /** Set the mode */
      BSP_I2C_SetMode(I2C, EUSCI_B_I2C_TRANSMIT_MODE);

      /** Setup for send string or send single */
      if (len == 1)
      {
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0.baseAddress, EUSCI_B0.Tx.buf[0]);
      }
      else
      {
        EUSCI_B_I2C_masterSendStart(EUSCI_B0.baseAddress);
/** Wait for I2C Send to complete */
#ifndef UNIT_TESTING
        while (EUSCI_B0.complete == false)
          ; /** @todo add timeout */
#endif

        if (EUSCI_B0.success == true)
        {
          result = BSP_OK;
        }
        else
        {
          result = BSP_FAIL;
        }
      }

      break;
#endif
#ifdef BSP_I2C_B1
    case EUSCI_B1_BASE:
      EUSCI_B1.Tx.ByteCnt = I2C->Tx.size;

      /** Clear the Bus Transmit Buffer */
      ClearBusBufferForTransmit(&EUSCI_B1);

      /** Transfer I2C buffer to EUSCI buffer */
      for (i = 0; i < len; i++)
      {
        Buffer8_get(&I2C->Tx, &EUSCI_B1.Tx.buf[i]);
      }

      /** Set the mode */
      BSP_I2C_SetMode(I2C, EUSCI_B_I2C_TRANSMIT_MODE);

      /** Setup for send string or send single */
      if (len == 1)
      {
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B1.baseAddress, EUSCI_B1.Tx.buf[0]);
      }
      else
      {
        EUSCI_B_I2C_masterSendStart(EUSCI_B1.baseAddress);
/** Wait for I2C Send to complete */
#ifndef UNIT_TESTING
        while (EUSCI_B1.complete == false)
          ; /** @todo add timeout */
#endif

        if (EUSCI_B1.success == true)
        {
          result = BSP_OK;
        }
        else
        {
          result = BSP_FAIL;
        }
      }

      break;
#endif

    default:
      /** @todo Add error */
      break;
    }
  }
  /** Clear the send buffer */
  Buffer8_Clear(&I2C->Tx);

  return result;
}

eBSPStatus_t BSP_I2C_ReceiveBytes(sI2C_t *I2C, uint16_t len, uint32_t timeout)
{
  eBSPStatus_t result = BSP_FAIL;
  uint16_t i;

  /** Clear the I2C Buffer */
  Buffer8_Clear(&I2C->Rx);

  /** Set the mode */
  BSP_I2C_SetMode(I2C, EUSCI_B_I2C_RECEIVE_MODE);

  /** Clear the interrupt temp buffer, send start, and wait for completion */
  switch (I2C->baseAddress)
  {
#ifdef BSP_I2C_B0
  case EUSCI_B0_BASE:
    ClearBusBufferForTransmit(&EUSCI_B0);
    EUSCI_B_I2C_masterReceiveStart(EUSCI_B0.baseAddress);
#ifndef UNIT_TESTING
    while (EUSCI_B0.complete == false)
      ; /**@todo add timeout */
#endif
    break;
#endif
#ifdef BSP_I2C_B1
  case EUSCI_B1_BASE:
    ClearBusBufferForTransmit(&EUSCI_B1);
    EUSCI_B_I2C_masterReceiveStart(EUSCI_B1.baseAddress);
#ifndef UNIT_TESTING
    while (EUSCI_B0.complete == false)
      ; /**@todo add timeout */
#endif
    break;
#endif
#ifdef BSP_I2C_B2
  case EUSCI_B2_BASE:
    ClearBusBufferForTransmit(&EUSCI_B2);
    EUSCI_B_I2C_masterReceiveStart(EUSCI_B2.baseAddress);
#ifndef UNIT_TESTING
    while (EUSCI_B0.complete == false)
      ; /**@todo add timeout */
#endif
    break;
#endif
#ifdef BSP_I2C_B3
  case EUSCI_B3_BASE:
    ClearBusBufferForTransmit(&EUSCI_B3);
    EUSCI_B_I2C_masterReceiveStart(EUSCI_B3.baseAddress);
#ifndef UNIT_TESTING
    while (EUSCI_B0.complete == false)
      ; /**@todo add timeout */
#endif
    break;
#endif
  default:
    /** @todo Add Error */
    break;
  }

  /** If Valid receive, transfer buffer to I2C Struct buffer */
  if (EUSCI_B0.success == true)
  {
    for (i = BSP_BUS_BUFFER_LENGTH; i > 0; --i)
    {
      Buffer8_put(&I2C->Rx, EUSCI_B0.Rx.buf[i - 1]);
    }
    result = BSP_OK;
  }
  else
  {
    result = BSP_FAIL;
  }

  return result;
}

/***********************************> SPI <***********************************/
void BSP_SPI_Init(sSPI_t *SPI)
{
  /** All SPI configured for running on SMCLK */
  SPI->param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
  SPI->param.clockSourceFrequency = SysClk.SMCLK.ClkFreq;
  SPI_B1_Mask = 0;
  EUSCI_B_SPI_initMaster(SPI->baseAddress, &SPI->param);
  return;
}

void BSP_SPI_Enable(sSPI_t *SPI)
{
  EUSCI_B_SPI_enable(SPI->baseAddress);
  return;
}

void BSP_SPI_Disable(sSPI_t *SPI)
{
  EUSCI_B_SPI_disable(SPI->baseAddress);
  return;
}

eBSPStatus_t BSP_SPI_SendBytes(sSPI_t *SPI)
{
  eBSPStatus_t result = BSP_FAIL;
  uint16_t i;
  uint16_t len = SPI->Tx.size;

  /** check for a valid transmit length */
  if (SPI->Tx.size < 1)
  {
    result = BSP_OK;
  }
  else
  {
    switch (SPI->baseAddress)
    {
#ifdef BSP_SPI_B0
    case EUSCI_B0_BASE:
      /** Enable the port */
      BSP_SPI_Enable(SPI);

      /** Enable the interrupt */
      SPI_B0_Mask |= EUSCI_B_SPI_RECEIVE_INTERRUPT;

      /** Clear the EUSCI Settings */
      ClearEUSCI(&EUSCI_B0);

      /** Populate the EUSCI Buffer */
      for (i = 0; i < len; i++)
      {
        Buffer8_get(&SPI->Tx, &EUSCI_B0.Tx.buf[i]);
      }
      EUSCI_B0.Rx.ByteCnt = len - 1;

      /** Send the data */
      EUSCI_B_SPI_transmitData(EUSCI_B0.baseAddress, EUSCI_B0.Tx.buf[EUSCI_B0.Tx.ByteCnt++]);
      result = BSP_OK;

/** Wait for send to finish? */
#ifndef UNIT_TESTING
      while (EUSCI_B_SPI_isBusy(EUSCI_B0.baseAddress))
        ;
#endif
      break;
#endif
#ifdef BSP_SPI_B1
    case EUSCI_B1_BASE:
      /** Enable the port */
      BSP_SPI_Enable(SPI);

      /** Enable the interrupt */
      SPI_B1_Mask |= EUSCI_B_SPI_RECEIVE_INTERRUPT;

      /** Clear the EUSCI Settings */
      ClearEUSCI(&EUSCI_B1);

      /** Populate the EUSCI Buffer */
      for (i = 0; i < len; i++)
      {
        Buffer8_get(&SPI->Tx, &EUSCI_B1.Tx.buf[i]);
      }
      EUSCI_B1.Rx.ByteCnt = len;

      /** Send the data */
      for (i = 0; i < EUSCI_B1.Rx.ByteCnt; i++)
      {
        EUSCI_B_SPI_transmitData(EUSCI_B1.baseAddress, EUSCI_B1.Tx.buf[EUSCI_B1.Tx.ByteCnt++]);

/** Wait for send to finish? */
#ifndef UNIT_TESTING
        while (EUSCI_B_SPI_isBusy(EUSCI_B1.baseAddress))
          ;
#endif
      }

      result = BSP_OK;
      break;
#endif
    default:
      /** @todo Add error */
      break;
    }
  }

  /** Clear the send buffer */
  Buffer8_Clear(&SPI->Tx);

  switch (SPI->baseAddress)
  {
#ifdef BSP_SPI_B0
  case EUSCI_B0_BASE:
    memset(EUSCI_B0.Tx.buf, 0, len);
    break;
#endif
#ifdef BSP_SPI_B1
  case EUSCI_B1_BASE:
    memset(EUSCI_B1.Tx.buf, 0, len);
    break;
#endif
  default:
    break;
  }
  return result;
}

eBSPStatus_t BSP_SPI_ReceiveBytes(sSPI_t *SPI, uint16_t len)
{
  eBSPStatus_t result = BSP_FAIL;
  uint16_t i;

  /** Clear the SPI Buffer */
  Buffer8_Clear(&SPI->Rx);

  /** Clear the interrupt temp buffer, send start, and wait for completion */
  switch (SPI->baseAddress)
  {
#ifdef BSP_SPI_B0
  case EUSCI_B0_BASE:
    /** Clear the Bus Buffer */
    ClearBusBufferForReceive(&EUSCI_B0);

    /** Enable the SPI Interrupt */
    SPI_B0_Mask |= EUSCI_B_SPI_RECEIVE_INTERRUPT;

    /** Fill the send buffer with 0's */
    EUSCI_B0.Rx.ByteCnt = len;
    EUSCI_B_SPI_transmitData(EUSCI_B0.baseAddress, EUSCI_B0.Tx.buf[0]);
#ifndef UNIT_TESTING
    while (EUSCI_B0.complete == false)
      ;
    for (i = BSP_BUS_BUFFER_LENGTH; i > 0; i++)
    {
      Buffer8_put(&SPI->Rx, EUSCI_B0.Rx.buf[i - 1]);
    }
    result = BSP_OK;
#endif
    break;
#endif
#ifdef BSP_SPI_B1
  case EUSCI_B1_BASE:
    /** Clear the Bus Buffer */
    ClearBusBufferForReceive(&EUSCI_B1);

    /** Enable the SPI Interrupt */
    SPI_B1_Mask |= EUSCI_B_SPI_RECEIVE_INTERRUPT;

    /** Fill the send buffer with 0's */
    EUSCI_B1.Rx.ByteCnt = len;
    EUSCI_B_SPI_transmitData(EUSCI_B1.baseAddress, EUSCI_B1.Tx.buf[0]);
#ifndef UNIT_TESTING
    while (EUSCI_B_SPI_isBusy(EUSCI_B1.baseAddress))
      ;
    for (i = BSP_BUS_BUFFER_LENGTH; i > 0; i++)
    {
      Buffer8_put(&SPI->Rx, EUSCI_B1.Rx.buf[i - 1]);
    }
    result = BSP_OK;
#endif
#endif
#ifdef BSP_SPI_B2
  case EUSCI_B2_BASE:

    break;
#endif
#ifdef BSP_SPI_B3
  case EUSCI_B3_BASE:

    break;
#endif
  default:
    /** @todo Add Error */
    break;
  }

  /** If Valid receive, transfer buffer to SPI Struct buffer */
  if (EUSCI_B0.success == true)
  {
    for (i = BSP_BUS_BUFFER_LENGTH; i > 0; --i)
    {
      Buffer8_put(&SPI->Rx, EUSCI_B0.Rx.buf[i - 1]);
    }
    result = BSP_OK;
  }
  else
  {
    result = BSP_FAIL;
  }

  return result;
}

/*********************************> TIMING <*********************************/
void BSP_Timer_Init(void)
{

  /** This code works, but should be fixed to run the driverlib functions */
  /** @todo Add Timer A0 driverlib call */
#ifndef UNIT_TESTING
  TA0CCTL0 = CCIE;
  //  TA0CCR0 = 57;
  TA0CCR0 = 114;
  TA0CTL = TASSEL__SMCLK | MC__UP | ID_3;
#endif
  /** @todo Add Timer A1 driverlib call */
#ifndef UNIT_TESTING
  TA1CCTL0 = CCIE;
  TA1CCR0 = 14398;
  TA1CTL = TASSEL__SMCLK | MC__UP | ID_3;
#endif
}

void BSP_Timer_Start(void)
{
}
/*********************************> TIMING <*********************************/

/************************************************************************
*					                  Static Functions
************************************************************************/
/** @brief Calculate UART Baud Registers
 *  
 *  Calculates the Oversampling, clock prescalar and first/second mod registers
 *  for setting the baud rate. 
 *
 *  @param *UART Pointer to UART structure
 *  
 *  @return result
 */
STATIC eBSPStatus_t CalculateUartBaudRegs(sUART_t *UART)
{
  eBSPStatus_t result = BSP_FAIL;
  float freq = 0.0;
  float temp = 0.0;
  float n = 0;
  int16_t UCBRx = 0;
  uint8_t UCBRFx = 0;
  uint8_t UCBRSx = 0;
  uint8_t OS = 0;
  uint8_t i;

  if ((UART->baudrate == 2400) || (UART->baudrate == 4800) ||
      (UART->baudrate == 9600) || (UART->baudrate == 19200) ||
      (UART->baudrate == 38400) || (UART->baudrate == 57600) ||
      (UART->baudrate == 115200))
  {

    if (UART->param.selectClockSource == EUSCI_A_UART_CLOCKSOURCE_ACLK)
    {
      freq = (float)SysClk.ACLK.ClkFreq;
    }
    else
    {
      freq = (float)SysClk.SMCLK.ClkFreq;
    }

    if (freq > 0)
    {
      /** Calculate N */
      n = freq / UART->baudrate;

      /** Calculate the first */
      if (n <= 16)
      {
        OS = 0;
        UCBRx = (uint16_t)n;
        UCBRFx = 0;
      }
      else
      {
        OS = 1;
        UCBRx = (uint16_t)floor(n / 16);

        temp = n / 16;
        temp = temp - (float)UCBRx;
        temp *= 16;
        UCBRFx = (uint8_t)temp;
      }

      /** Find the BRS value */
      n = n - floor(n);
      for (i = BAUD_BRS_LEN; i > 0; --i)
      {
        if (BAUD_N[i - 1] < n)
        {
          UCBRSx = BAUD_BRS[i - 1];
          break;
        }
      }

      UART->param.overSampling = OS;
      UART->param.firstModReg = (uint8_t)UCBRFx;
      UART->param.secondModReg = (uint8_t)UCBRSx;
      UART->param.clockPrescalar = (uint16_t)UCBRx;
      result = BSP_OK;
    }
    else
    {
      result = BSP_FAIL;
    }
  }
  return result;
}

/** @brief Clear Bus Bufffer for Transmit Mode
 *
 *  Clears the Bus Buffer, complete and sets diretion to output, system busy.
 *
 *  @param temp Bus Buffer
 *
 *  @return none
 */
STATIC void ClearBusBufferForTransmit(sEUSCI_t *temp)
{
  temp->direction = 0;
  temp->complete = false;
  temp->busy = true;
  temp->success = false;
  memset(temp->Tx.buf, 0, BSP_BUS_BUFFER_LENGTH);
}

/** @brief Clear Bus Bufffer for Receive Mode
 *
 *  Clears the Bus Buffer, complete and sets diretion to input, system busy.
 *
 *  @param temp Bus Buffer
 *
 *  @return none
 */
STATIC void ClearBusBufferForReceive(sEUSCI_t *temp)
{
  temp->direction = 1;
  temp->complete = false;
  temp->busy = true;
  temp->success = false;
  memset(temp->Rx.buf, 0, BSP_BUS_BUFFER_LENGTH);
}

///** @brief Disable Interrupts selected 
// *
// *  Disables the selected inputs.
// *
// *  @param interrupt Interrupts to be selected (can be OR'ed together)
// *
// *  @return none
// */
//STATIC void DisableInterrupt(uint16_t interrupt)
//{
//
//  if (interrupt & INTERRUPT_UART_A0)
//  {
//    /** Disable UART A0 */
//    UART_A0_Mask = EUSCI_A_UART_getInterruptStatus(EUSCI_A0_BASE, 0xFFu);
//    EUSCI_A_UART_disableInterrupt(EUSCI_A0_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_UART_A1)
//  {
//    /** Disable UART A1 */
//    UART_A1_Mask = EUSCI_A_UART_getInterruptStatus(EUSCI_A1_BASE, 0xFFu);
//    EUSCI_A_UART_disableInterrupt(EUSCI_A1_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_UART_A2)
//  {
//    /** Disable UART A2 */
//    UART_A2_Mask = EUSCI_A_UART_getInterruptStatus(EUSCI_A2_BASE, 0xFFu);
//    EUSCI_A_UART_disableInterrupt(EUSCI_A2_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_UART_A3)
//  {
//    /** Disable UART A3 */
//    UART_A3_Mask = EUSCI_A_UART_getInterruptStatus(EUSCI_A3_BASE, 0xFFu);
//    EUSCI_A_UART_disableInterrupt(EUSCI_A3_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_I2C_B0)
//  {
//    /** Disable I2C B0 */
//    I2C_B0_Mask = EUSCI_B_I2C_getInterruptStatus(EUSCI_B0_BASE, 0xFFu);
//    EUSCI_B_I2C_disableInterrupt(EUSCI_B0_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_SPI_B1)
//  {
//    /** Disable SPI B1 */
//    SPI_B1_Mask = EUSCI_B_SPI_getInterruptStatus(EUSCI_B1_BASE, 0xFFu);
//    EUSCI_B_SPI_disableInterrupt(EUSCI_B1_BASE, 0xFFu);
//  }
//
//  if (interrupt & INTERRUPT_GPIO_P1_4)
//  {
//    /** Disable GPIO Port1 Pin4 */
//    GPIO_P1_4_Mask = 0xFF & GPIO_getInterruptStatus(1, 4);
//    GPIO_disableInterrupt(1, 4);
//  }
//
//  if (interrupt & INTERRUPT_GPIO_P1_5)
//  {
//    /** Disable GPIO Port1 Pin5 */
//    GPIO_P1_4_Mask = 0xFF & GPIO_getInterruptStatus(1, 5);
//    GPIO_disableInterrupt(1, 5);
//  }
//
//  return;
//}
//
///** @brief Enable Interrupts selected 
// *
// *  Enables the selected inputs with the static mask value (must be set prior to calling function).
// *
// *  @param interrupt Interrupts to be selected (CANNOT be OR'ed together)
// *
// *  @return none
// */
//STATIC void EnableInterrupt(uint16_t interrupt)
//{
//
//  if (interrupt & INTERRUPT_UART_A0)
//  {
//    /** Enable UART A0 */
//    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, UART_A0_Mask);
//  }
//
//  if (interrupt & INTERRUPT_UART_A1)
//  {
//    /** Enable UART A1 */
//    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, UART_A1_Mask);
//  }
//
//  if (interrupt & INTERRUPT_UART_A2)
//  {
//    /** Enable UART A2 */
//    EUSCI_A_UART_enableInterrupt(EUSCI_A2_BASE, UART_A2_Mask);
//  }
//
//  if (interrupt & INTERRUPT_UART_A3)
//  {
//    /** Enable UART A3 */
//    EUSCI_A_UART_enableInterrupt(EUSCI_A3_BASE, UART_A3_Mask);
//  }
//
//  if (interrupt & INTERRUPT_I2C_B0)
//  {
//    /** Enable I2C B0 */
//    EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE, I2C_B0_Mask);
//  }
//
//  if (interrupt & INTERRUPT_SPI_B1)
//  {
//    /** Enable SPI B1 */
//    EUSCI_B_SPI_clearInterrupt(EUSCI_B1_BASE, 0xFF);
//    EUSCI_B_SPI_enableInterrupt(EUSCI_B1_BASE, SPI_B1_Mask);
//  }
//
//  if (interrupt & INTERRUPT_GPIO_P1_4)
//  {
//    /** Enable GPIO Port1 Pin4 */
//    GPIO_enableInterrupt(1, 4);
//  }
//
//  if (interrupt & INTERRUPT_GPIO_P1_5)
//  {
//    /** Enable GPIO Port1 Pin5 */
//    GPIO_enableInterrupt(1, 5);
//  }
//
//#ifndef UNIT_TESTING
//  __bis_SR_register(GIE); // Enter LPM0 w/ interrupt
//#endif
//  return;
//}

/** @brief Clear the EUSCI Structure for op
 *
 *  Setup the EUSCI Structure for a new Operation
 *
 *  @param *port Pointer to the EUSCI Structure
 *
 *  @return None
 */
STATIC void ClearEUSCI(sEUSCI_t *port)
{

  port->busy = false;
  port->complete = false;
  port->success = false;

  memset(port->Tx.buf, 0, BSP_BUS_BUFFER_LENGTH);
  port->Tx.ByteCnt = 0;

  memset(port->Rx.buf, 0, BSP_BUS_BUFFER_LENGTH);
  port->Rx.ByteCnt = 0;
}