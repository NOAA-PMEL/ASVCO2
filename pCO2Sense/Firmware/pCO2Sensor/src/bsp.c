/** @file BSP.c
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
 *	@note
 *
 *  @bug  No known bugs
 */
#include "bsp.h"
// #include "errors.h"
#include "logging.h"
/************************************************************************
 *			                Static Functions Prototypes
 ************************************************************************/
STATIC eBSPStatus_t CalculateUartBaudRegs(sUART_t *UART);

/************************************************************************
 *			                    Variables (Global)
 ************************************************************************/
sEUSCI_t EUSCI_A0 = {.baseAddress = EUSCI_A0_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI A0 Bus (User Console UART) */
sEUSCI_t EUSCI_A1 = {.baseAddress = EUSCI_A1_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI A1 Bus (Command Port UART) */
sEUSCI_t EUSCI_A2 = {.baseAddress = EUSCI_A2_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI A2 Bus (LICOR Port UART) */
sEUSCI_t EUSCI_A3 = {.baseAddress = EUSCI_A3_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI A3 Bus (Datalogger UART) */
sEUSCI_t EUSCI_B0 = {.baseAddress = EUSCI_B0_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B0 Bus (AMBIQ RTC I2C BUS) */
sEUSCI_t EUSCI_B1 = {.baseAddress = EUSCI_B1_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B1 Bus (RH/Temp I2C Bus) */
sEUSCI_t EUSCI_B2 = {.baseAddress = EUSCI_B2_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B2 Bus (MAX310X I2C Bus) */
sEUSCI_t EUSCI_B3 = {.baseAddress = EUSCI_B3_BASE, .timeout = 0, .direction = 0, .busy = false, .complete = false}; /**< EUSCI B3 Bus (Not Used) */

sCounters_t Timer = {.timer = {.ms = 0, .flag = true},
                     .spi = {.ms = 0, .flag = true},
                     .i2c = {.ms = 0, .flag = true}};
/************************************************************************
 *			                Variables (Static in Module)
 ************************************************************************/
/** System Clock */
const sCLK_t SysClk = {.HX = {.Freq = HFXT_FREQ, .Drive = HFXT_DRIVE, .timeout = HFXT_TIMEOUT},
                       .LX = {.Freq = LFXT_FREQ, .Drive = LFXT_DRIVE, .timeout = LFXT_TIMEOUT},
                       .ACLK = {.ClkFreq = ACLK_FREQ, .ClkSrc = ACLK_SRC, .divider = ACLK_DIV},
                       .MCLK = {.ClkFreq = MCLK_FREQ, .ClkSrc = MCLK_SRC, .divider = MCLK_DIV},
                       .SMCLK = {.ClkFreq = SMCLK_FREQ, .ClkSrc = SMCLK_SRC, .divider = SMCLK_DIV},
                       .MODOSC = {.ClkFreq = MODOSC_FREQ, .ClkSrc = MODOSC_SRC, .divider = MODOSC_DIV}};

/** System Info */
STATIC const float BAUD_N[] = {0, 0.0529, 0.0715, 0.0835, 0.1001, 0.1252, 0.143, 0.167, 0.2147, 0.2224,
                               0.2503, 0.3, 0.3335, 0.3575, 0.3753, 0.4003, 0.4286, 0.4378, 0.5002,
                               0.5715, 0.6003, 0.6254, 0.6432, 0.6667, 0.7001, 0.7147, 0.7503, 0.7861,
                               0.8004, 0.8333, 0.8464, 0.8572, 0.8751, 0.9004, 0.917, 0.9288};
STATIC const uint8_t BAUD_BRS[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x11, 0x21, 0x22, 0x44, 0x25,
                                   0x49, 0x4A, 0x52, 0x92, 0x53, 0x55, 0xAA, 0x6B, 0xAD, 0xB5, 0xB6, 0xD6,
                                   0xB7, 0xBB, 0xDD, 0xED, 0xEE, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFF};
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

  /*************************** Configure GPIO Ports **************************/

  /** Disable Interrupts */
  GPIO_disableInterrupt(GPIO_PORT_P1, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P2, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P3, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P4, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P5, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P6, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P7, (pinMask));
  GPIO_disableInterrupt(GPIO_PORT_P8, (pinMask));

  /** Clear all ports to Low Power State */
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, (pinMask & ~(GPIO_PIN4 | GPIO_PIN2)));
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P3, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P4, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P5, (pinMask & ~GPIO_PIN6));
  GPIO_setOutputLowOnPin(GPIO_PORT_P6, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P7, pinMask);
  GPIO_setOutputLowOnPin(GPIO_PORT_P8, pinMask);

  /** Configure the ADC/Reference Pins */
  GPIO_setAsInputPin(GPIO_PORT_P1, (GPIO_PIN0 | GPIO_PIN1));

  /** Configure the General Function Output Pins */
  GPIO_setAsOutputPin(SPARE_RS232_ENABLE_PORT, SPARE_RS232_ENABLE_PIN);
  GPIO_setAsOutputPin(POWER_15_ENABLE_PORT, POWER_15_ENABLE_PIN);
  GPIO_setAsOutputPin(FLOWCTRL_ENABLE_PORT, FLOWCTRL_ENABLE_PIN);
  GPIO_setAsOutputPin(LICOR_ENABLE_PORT, LICOR_ENABLE_PIN);
  GPIO_setAsOutputPin(GREEN_LED_PORT, GREEN_LED_PIN);
  GPIO_setAsOutputPin(RED_LED_PORT, RED_LED_PIN);
  GPIO_setAsOutputPin(MAX310X_I2C_SEL_PORT, MAX310X_I2C_SEL_PIN);
  GPIO_setAsOutputPin(MAX310X_NRESET_PORT, MAX310X_NRESET_PIN);
  GPIO_setAsOutputPin(MAX310X_CS_PORT, MAX310X_CS_PIN);
  GPIO_setAsOutputPin(MAX310X_NIRQ_PORT, MAX310X_NIRQ_PIN);
  GPIO_setAsOutputPin(WATCHDOG_PORT, WATCHDOG_PIN);
  GPIO_setAsOutputPin(O2_POWER_PORT, O2_POWER_PIN);

#ifdef BOARD_VER_1_3_2
  GPIO_setAsOutputPin(POWER_15_PWM_PORT, POWER_15_PWM_PIN);
#endif

  GPIO_selectInterruptEdge(AMBIQ_RTC_NIRQ2_PORT, AMBIQ_RTC_NIRQ2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(AMBIQ_RTC_NIRQ2_PORT, AMBIQ_RTC_NIRQ2_PIN);
  GPIO_enableInterrupt(AMBIQ_RTC_NIRQ2_PORT, AMBIQ_RTC_NIRQ2_PIN);

  GPIO_setAsInputPin(AMBIQ_RTC_EXT_PORT, AMBIQ_RTC_EXT_PIN);
  GPIO_setAsInputPin(MAX310X_NIRQ_PORT, MAX310X_NIRQ_PIN);

  /** Configure the Clock Power Pin To Output and On */
  GPIO_setAsOutputPin(OSC_POWER_PORT, OSC_POWER_PIN);
  GPIO_setOutputHighOnPin(OSC_POWER_PORT, OSC_POWER_PIN);

  GPIO_selectInterruptEdge(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);
  GPIO_enableInterrupt(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);

  /** Configure Input pins for HFXT */
  GPIO_setAsPeripheralModuleFunctionInputPin(HF_XTAL_PORT,
                                             HF_XTAL_PIN,
                                             HF_XTAL_FUNCTION);

  /************************* Configure I2C/SPI Ports *************************/
  /** Set I2C B0 Port Pins */
  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SDA_PORT,
                                              I2C_B0_SDA_PIN,
                                              I2C_B0_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SCL_PORT,
                                              I2C_B0_SCL_PIN,
                                              I2C_B0_FUNCTION);

  /** Set I2C B1 Port Pins */
  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B1_SDA_PORT,
                                              I2C_B1_SDA_PIN,
                                              I2C_B1_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B1_SCL_PORT,
                                              I2C_B1_SCL_PIN,
                                              I2C_B1_FUNCTION);

  /** Set SPI B2 Port Pins */
  GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_B2_MOSI_PORT,
                                              SPI_B2_MOSI_PIN,
                                              SPI_B2_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_B2_CLK_PORT,
                                              SPI_B2_CLK_PIN,
                                              SPI_B2_FUNCTION);
  GPIO_setAsPeripheralModuleFunctionInputPin(SPI_B2_MISO_PORT,
                                             SPI_B2_MISO_PIN,
                                             SPI_B2_FUNCTION);

  /************************** Configure UART Ports ***************************/
  /** Set the UART Port Pins */
  /** Set UART A0 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A0_RX_PORT,
                                             UART_A0_RX_PIN,
                                             UART_A0_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A0_TX_PORT,
                                              UART_A0_TX_PIN,
                                              UART_A0_FUNCTION);
  /** Set UART A1 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A1_RX_PORT,
                                             UART_A1_RX_PIN,
                                             UART_A1_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A1_TX_PORT,
                                              UART_A1_TX_PIN,
                                              UART_A1_FUNCTION);

  /** Set UART A2 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A2_RX_PORT,
                                             UART_A2_RX_PIN,
                                             UART_A2_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A2_TX_PORT,
                                              UART_A2_TX_PIN,
                                              UART_A2_FUNCTION);
  /** Set UART A3 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A3_RX_PORT,
                                             UART_A3_RX_PIN,
                                             UART_A3_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A3_TX_PORT,
                                              UART_A3_TX_PIN,
                                              UART_A3_FUNCTION);

  /************************** Configure Analog Ports ***************************/
  GPIO_setAsPeripheralModuleFunctionInputPin(O2_ANALOG_PORT,
                                             O2_ANALOG_PIN,
                                             O2_ANALOG_FUNCTION);

  /** Turn the 15V Power ON */
  GPIO_setOutputHighOnPin(POWER_15_ENABLE_PORT, POWER_15_ENABLE_PIN);

/*
 * Disable the GPIO power-on default high-impedance mode to activate
 * previously configured port settings
 */
#ifndef UNIT_TESTING
  PMM_unlockLPM5();
#endif

  /** Start the clocks */
  if (BSP_SYSCLK_Init() == BSP_FAIL)
  {
    Error(ERROR_MSG_01010100, LOG_Priority_High);
  }

  /** Start the timers */
  BSP_Timer_Init(NULL, NULL);

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

  /** No Low Freq crystal on this board */
  CS_turnOffLFXT();

  return result;
}

/***********************************> LPM <***********************************/
void BSP_Low_Power_Mode(void)
{
  /** Set Input Pins */
  GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
  GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN1);
  GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN6);

  /** Set Interrupt on Console Watch Pin */
  GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN7, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7);
  GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN7);

  /** Set Interrupt on Command Watch Pin */
  GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);
  GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);
  /***************** Configure GPIO Ports for Low Power Mode*******************/
  /********************* PORT 1 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P1, LPM_PORT1_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P1, LPM_PORT1_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P1, LPM_PORT1_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, LPM_PORT1_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, LPM_PORT1_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, LPM_PORT1_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P1, LPM_PORT1_INPUT_NO_RESISTORS);

  /********************* PORT 2 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P2, LPM_PORT2_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P2, LPM_PORT2_OUTPUT_HIGH_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, LPM_PORT2_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, LPM_PORT2_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P2, LPM_PORT2_INPUT_NO_RESISTORS);

  /********************* PORT 3 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P3, LPM_PORT3_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P3, LPM_PORT3_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P3, LPM_PORT3_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P3, LPM_PORT3_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, LPM_PORT3_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, LPM_PORT3_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P3, LPM_PORT3_INPUT_NO_RESISTORS);

  /********************* PORT 4 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P4, LPM_PORT4_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P4, LPM_PORT4_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P4, LPM_PORT4_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P4, LPM_PORT4_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, LPM_PORT4_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, LPM_PORT4_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P4, LPM_PORT4_INPUT_NO_RESISTORS);

  /********************* PORT 5 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P5, LPM_PORT5_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P5, LPM_PORT5_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P5, LPM_PORT5_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P5, LPM_PORT5_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, LPM_PORT5_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, LPM_PORT5_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P5, LPM_PORT5_INPUT_NO_RESISTORS);

  /********************* PORT 6 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P6, LPM_PORT6_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P6, LPM_PORT6_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P6, LPM_PORT6_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P6, LPM_PORT6_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, LPM_PORT6_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, LPM_PORT6_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P6, LPM_PORT6_INPUT_NO_RESISTORS);

  /********************* PORT 7 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P7, LPM_PORT7_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P7, LPM_PORT7_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P7, LPM_PORT7_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P7, LPM_PORT7_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P7, LPM_PORT7_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7, LPM_PORT7_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P7, LPM_PORT7_INPUT_NO_RESISTORS);

  /********************* PORT 8 **********************/
  /** Outputs Logic HIGH */
  GPIO_setAsOutputPin(GPIO_PORT_P8, LPM_PORT8_OUTPUT_HIGH_PINS);
  GPIO_setOutputHighOnPin(GPIO_PORT_P8, LPM_PORT8_OUTPUT_HIGH_PINS);

  /** Output Logic LOW */
  GPIO_setAsOutputPin(GPIO_PORT_P8, LPM_PORT8_OUTPUT_LOW_PINS);
  GPIO_setOutputLowOnPin(GPIO_PORT_P8, LPM_PORT8_OUTPUT_LOW_PINS);

  /** Input w/Pull-Ups */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P8, LPM_PORT8_INPUT_WITH_PULL_UPS);

  /** Input w/Pull-Downs */
  GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P8, LPM_PORT8_INPUT_WITH_PULL_DOWNS);

  /** Input no Pull-Ups/Downs */
  GPIO_setAsInputPin(GPIO_PORT_P8, LPM_PORT8_INPUT_NO_RESISTORS);

  /** Set Interrupts */
  /********************* PORT 1 **********************/
  GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4);
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

  GPIO_selectInterruptEdge(GPIO_PORT_P1, LPM_PORT1_HIGH_TO_LOW_PINS, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_selectInterruptEdge(GPIO_PORT_P1, LPM_PORT1_LOW_TO_HIGH_PINS, GPIO_LOW_TO_HIGH_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P1, LPM_ALL_PINS);
  GPIO_enableInterrupt(GPIO_PORT_P1, (LPM_PORT1_HIGH_TO_LOW_PINS | LPM_PORT1_LOW_TO_HIGH_PINS));

  /*******************************************************/

#ifdef BOARD_VER_1_0
  GPIO_setAsInputPinWithPullUpResistor(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);
#endif
#ifdef BOARD_VER_1_1
  GPIO_setAsInputPin(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);
  GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN4);
  GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3);
  GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN5);
#endif

  /** Added to TEST LPM with UARTS */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A0_RX_PORT,
                                             UART_A0_RX_PIN,
                                             UART_A0_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A0_TX_PORT,
                                              UART_A0_TX_PIN,
                                              UART_A0_FUNCTION);

  /** Set UART A2 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A2_RX_PORT,
                                             UART_A2_RX_PIN,
                                             UART_A2_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A2_TX_PORT,
                                              UART_A2_TX_PIN,
                                              UART_A2_FUNCTION);
  /** Set UART A3 Port Pins */
  GPIO_setAsPeripheralModuleFunctionInputPin(UART_A3_RX_PORT,
                                             UART_A3_RX_PIN,
                                             UART_A3_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(UART_A3_TX_PORT,
                                              UART_A3_TX_PIN,
                                              UART_A3_FUNCTION);

  /** Finish adding to test LPM with UARTS */

  GPIO_selectInterruptEdge(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);
  GPIO_enableInterrupt(AMBIQ_RTC_NTIRQ_PORT, AMBIQ_RTC_NTIRQ_PIN);

  GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4);
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

  GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);

  GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN5);

  /** Port J */
  GPIO_setAsOutputPin(GPIO_PORT_PJ, 0xFF);
  GPIO_setOutputLowOnPin(GPIO_PORT_PJ, 0xFF);

  PM5CTL0 &= ~LOCKLPM5;

  __delay_cycles(10);
  __delay_cycles(10);
  __delay_cycles(10);
  __delay_cycles(10);
  __delay_cycles(10);
}

void BSP_Low_Current_Mode_ON(void)
{
  GPIO_setOutputLowOnPin(POWER_15_PWM_PORT, POWER_15_PWM_PIN);
}

void BSP_Low_Current_Mode_OFF(void)
{
  GPIO_setOutputHighOnPin(POWER_15_PWM_PORT, POWER_15_PWM_PIN);
  PMM_unlockLPM5();
  BSP_Hit_WD();
  _delay_ms(150);
}
void _delay_ms(uint32_t ms)
{

  while (ms-- > 0)
  {
    __delay_cycles(3634);
  }
}

void _delay_us(uint32_t us)
{
  while (us-- > 0)
  {
    __delay_cycles(4);
  }
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
      Error(ERROR_MSG_01010020, LOG_Priority_High);
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

  /** If this is the first startup after programming, set the bauds */
  /** **NOTE** This adds a slight delay to startup only on the first run */
  UART->param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
  UART->param.parity = EUSCI_A_UART_NO_PARITY;
  UART->param.msborLsbFirst = EUSCI_A_UART_ONE_STOP_BIT;
  UART->param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
  UART->param.uartMode = EUSCI_A_UART_MODE;
  if (CalculateUartBaudRegs(UART) != BSP_OK)
  {
    Error(ERROR_MSG_01010021, LOG_Priority_High);
    result = BSP_FAIL;
    return result;
  }

  /** Clear the interrupts */
  EUSCI_A_UART_clearInterrupt(UART->baseAddress, 0xFF);

  if (EUSCI_A_UART_init(UART->baseAddress, &UART->param) == STATUS_FAIL)
  {
    UART->configured = false;
    result = BSP_FAIL;
    Error(ERROR_MSG_01010022, LOG_Priority_High);
  }
  else
  {
    UART->configured = true;
    result = BSP_OK;
  }

  return result;
}

eBSPStatus_t BSP_UART_SetBaudrate(sUART_t *UART, uint32_t baudrate)
{
  eBSPStatus_t result = BSP_FAIL;
  sUART_t *temp;
  memcpy(temp, UART, SIZE_OF(UART));
  temp->baudrate = baudrate;

  if (CalculateUartBaudRegs(temp) == BSP_OK)
  {
    memcpy(UART, temp, SIZE_OF(UART));
    if (EUSCI_A_UART_init(UART->baseAddress, &UART->param) == STATUS_FAIL)
    {
      UART->configured = false;
      result = BSP_FAIL;
      Error(ERROR_MSG_01010022, LOG_Priority_High);
    }
    else
    {
      UART->configured = true;
      result = BSP_OK;
    }
  }
  else
  {
    Error(ERROR_MSG_01010023, LOG_Priority_High);
  }

  return result;
}

uint32_t BSP_UART_GetBaudrate(sUART_t *UART) { return UART->baudrate; }

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

void BSP_UART_puts(sUART_t *UART, const char *val, uint16_t length)
{
  uint16_t i;

  for (i = 0; i < length; i++)
  {
    BSP_UART_putc(UART, val[i]);
  }

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
  /** All I2C Configured for running on SMCLK */
  I2C->param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
  I2C->param.i2cClk = SysClk.SMCLK.ClkFreq;

  /** Datarate */
  I2C->param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;

  /** Set Autostop */
  I2C->param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;

  /** Enable Master */
  EUSCI_B_I2C_initMaster(I2C->baseAddress, &I2C->param);

  /** Set Slave Address */
  BSP_I2C_SetSlaveAddress(I2C, I2C->slaveAddress);

  /** Send Reset */

  return;
}

void BSP_I2C_Enable(sI2C_t *I2C, uint8_t mode)
{
  EUSCI_B_I2C_initMaster(I2C->baseAddress, &I2C->param);
  BSP_I2C_SetSlaveAddress(I2C, I2C->slaveAddress);
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
  EUSCI_B_I2C_setSlaveAddress(I2C->baseAddress, addr >> 1);
  I2C->slaveAddress = addr;
  return;
}

void BSP_I2C_SetMode(sI2C_t *I2C, uint8_t mode)
{
  I2C->mode = mode;
  EUSCI_B_I2C_setMode(I2C->baseAddress, mode);
}

void BSP_I2C_Reset(sI2C_t *I2C)
{
  BSP_I2C_Disable(I2C);
  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SDA_PORT,
                                              I2C_B0_SDA_PIN,
                                              GPIO_PRIMARY_MODULE_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SCL_PORT,
                                              I2C_B0_SCL_PIN,
                                              GPIO_PRIMARY_MODULE_FUNCTION);
  GPIO_setAsOutputPin(I2C_B0_SDA_PORT, I2C_B0_SDA_PIN);
  GPIO_setOutputLowOnPin(I2C_B0_SDA_PORT, I2C_B0_SDA_PIN);
  GPIO_setOutputHighOnPin(I2C_B0_SDA_PORT, I2C_B0_SDA_PIN);
  GPIO_setOutputLowOnPin(I2C_B0_SDA_PORT, I2C_B0_SDA_PIN);
  _delay_ms(50);

  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SDA_PORT,
                                              I2C_B0_SDA_PIN,
                                              I2C_B0_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(I2C_B0_SCL_PORT,
                                              I2C_B0_SCL_PIN,
                                              I2C_B0_FUNCTION);
  BSP_I2C_Enable(I2C, EUSCI_B_I2C_TRANSMIT_MODE);
}

/***********************************> ??? <***********************************/
void BSP_I2C_put(sI2C_t *I2C, uint8_t val)
{

  I2C->param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;

  /** Enable the port */
  BSP_I2C_Enable(I2C, EUSCI_B_I2C_TRANSMIT_MODE);

  /** Send Single Byte */
  if (EUSCI_B_I2C_masterSendSingleByteWithTimeout(I2C->baseAddress, val, 1000000) == false)
  {
    Error(ERROR_MSG_01010030, LOG_Priority_High);
    Error("Restarting", LOG_Priority_High);
    PMMCTL0 |= PMMSWPOR;
  }

  /** Wait for Tx or timeout */
  uint8_t timeout = 5;
  while (EUSCI_B_I2C_isBusBusy(I2C->baseAddress) && timeout > 0)
  {
    _delay_ms(10);
  }

  return;
}

void BSP_I2C_puts(sI2C_t *I2C, uint8_t *val, uint16_t len)
{
  I2C->param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;

  /** Ready the number of bytes */
  I2C->param.byteCounterThreshold = len;

  /** Enable the port */
  BSP_I2C_Enable(I2C, EUSCI_B_I2C_TRANSMIT_MODE);

  /** Clear the interrupts */
  EUSCI_B_I2C_clearInterrupt(I2C->baseAddress,
                             EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                                 EUSCI_B_I2C_NAK_INTERRUPT);

  /** Enable the receive Interrupt */
  EUSCI_B_I2C_enableInterrupt(I2C->baseAddress,
                              EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                                  EUSCI_B_I2C_NAK_INTERRUPT);

  /** Wait for Sending Stop or timeout */
  uint8_t timeout = 5;
  while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE) &&
         timeout-- > 0)
  {
    _delay_ms(10);
  }

  /** Update the counter */
  I2C_B0_TxCount = len;

  /** Prep the send data */
  memcpy(I2C_B0_TxData, val, len);

  /** Start the transmission by writing to the register address */
  for (timeout = 5; timeout > 0; timeout--)
  {
    if (STATUS_FAIL == EUSCI_B_I2C_masterSendMultiByteStartWithTimeout(I2C->baseAddress,
                                                                       I2C_B0_TxData[0],
                                                                       10000))
    {
      BSP_I2C_Reset(I2C);
    }
    else
    {
      break;
    }
  }
  /** Wait for Tx or timeout */
  timeout = 5;

  while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE) &&
         timeout-- > 0)
  {
    _delay_ms(10);
  }
  timeout = 5;
  while (EUSCI_B_I2C_isBusBusy(I2C->baseAddress) && timeout-- > 0)
  {
    _delay_ms(10);
  }

  return;
}

uint8_t BSP_I2C_get(sI2C_t *I2C)
{
  uint8_t val = 0;
  BSP_I2C_gets(I2C, &val, 1);

  return val;
}

eBSPStatus_t BSP_I2C_gets(sI2C_t *I2C, uint8_t *val, uint16_t len)
{
  eBSPStatus_t result = BSP_FAIL;

  I2C->param.autoSTOPGeneration = EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD;
  I2C->param.byteCounterThreshold = len;

  /** Set to Receive Mode */
  EUSCI_B_I2C_setMode(I2C->baseAddress, EUSCI_B_I2C_RECEIVE_MODE);

  /** Enable the I2C Port */
  BSP_I2C_Enable(I2C, EUSCI_B_I2C_RECEIVE_MODE);

  /** Clear the interrupts */
  EUSCI_B_I2C_clearInterrupt(I2C->baseAddress,
                             EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
                                 EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
                                 EUSCI_B_I2C_NAK_INTERRUPT);

  /** Enable the receive Interrupt */
  EUSCI_B_I2C_enableInterrupt(I2C->baseAddress,
                              EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
                                  EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
                                  EUSCI_B_I2C_NAK_INTERRUPT);

  /** Wait for Sending Stop or timeout */
  uint8_t timeout = 5;
  while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(I2C->baseAddress) &&
         timeout-- > 0)
  {
    _delay_ms(10);
  }

  memset(I2C_B0_RxData, 0, 256);
  I2C_B0_MaxCount = len;
  __delay_cycles(10000);

  /** Start Receive */
  EUSCI_B_I2C_masterReceiveStart(I2C->baseAddress);
  while (EUSCI_B_I2C_masterIsStopSent(I2C->baseAddress) != EUSCI_B_I2C_STOP_SEND_COMPLETE)
    ;
  /** Wait for Count to go to zero, with timeout.  Restart System if timeout */
  uint16_t cnt = 50;
  while ((I2C_B0_MaxCount != 0) && (cnt > 0))
  {
    _delay_ms(1);
    cnt--;
  }

  /** If we timed out, send Error */
  if (cnt != 0)
  {
    result = BSP_OK;
    /** Copy the data */
    memcpy(val, I2C_B0_RxData, len);
  }

  return result;
}

eBSPStatus_t BSP_I2C_SendData(sI2C_t *I2C, uint8_t reg, uint8_t *data, uint16_t len)
{
  eBSPStatus_t result = BSP_FAIL;

  /** */
  I2C_B0_TxData[0] = reg;
  memcpy(&I2C_B0_TxData[1], data, len);
  /** Send Data */
  BSP_I2C_puts(I2C, I2C_B0_TxData, len + 1);

  result = BSP_OK;
  return result;
}

eBSPStatus_t BSP_I2C_GetData(sI2C_t *I2C, uint8_t reg, uint8_t *data, uint8_t len)
{

  eBSPStatus_t result = BSP_FAIL;

  /** Send Read to Register */
  BSP_I2C_put(I2C, reg);

  /** Read Data */
  BSP_I2C_gets(I2C, data, len);

  result = BSP_OK;
  return result;
}

/***********************************> SPI <***********************************/
void BSP_SPI_Init(sSPI_t *SPI)
{

  /** Initialize the SPI in Master Mode */
  EUSCI_B_SPI_initMaster(SPI->baseAddress, &SPI->param);

  /** Enable the SPI */
  EUSCI_B_SPI_enable(SPI->baseAddress);

  /** Clear the SPI Interrupts */
  EUSCI_B_SPI_clearInterrupt(SPI->baseAddress, 0xFF);

  /** Enable the SPI Interrupts */
  EUSCI_B_SPI_enableInterrupt(SPI->baseAddress, UCTXIE);
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

void BSP_SPI_put(sSPI_t *SPI, uint8_t val)
{

  EUSCI_B_SPI_transmitData(SPI->baseAddress, val);
  while (EUSCI_B_SPI_isBusy(SPI->baseAddress))
    ;
  return;
}

void BSP_SPI_puts(sSPI_t *SPI, uint8_t *val, uint16_t len)
{
  uint16_t i;
  for (i = 0; i < len; i++)
  {
    EUSCI_B_SPI_transmitData(SPI->baseAddress, val[i]);
  }
  return;
}

uint8_t BSP_SPI_get(sSPI_t *SPI, uint8_t addr)
{

  EUSCI_B_SPI_transmitData(SPI->baseAddress, addr);
  while (EUSCI_B_SPI_isBusy(SPI->baseAddress))
    ;
  EUSCI_B_SPI_receiveData(SPI->baseAddress);
  EUSCI_B_SPI_transmitData(SPI->baseAddress, 0x00);
  while (EUSCI_B_SPI_isBusy(SPI->baseAddress))
    ;

  return EUSCI_B_SPI_receiveData(SPI->baseAddress);
}

void BSP_SPI_gets(sSPI_t *SPI, uint8_t addr, uint8_t *val, uint16_t len)
{
  uint16_t i;

  /** Write the register to read from */
  EUSCI_B_SPI_transmitData(SPI->baseAddress, addr);

  /** Write 0's and read the return */
  for (i = 0; i < len; i++)
  {
    EUSCI_B_SPI_transmitData(SPI->baseAddress, 0x00);
    val[i] = EUSCI_B_SPI_receiveData(SPI->baseAddress);
  }

  return;
}

/*********************************> TIMING <*********************************/
void BSP_Timer_Init(sTimerA_t *A, sTimerB_t *B)
{
#ifndef UNIT_TESTING
  TA0CCR0 = 50000;
  TA0CTL = TASSEL__ACLK | MC__UP | ID__2;
#endif
}

/*********************************> Watchdog <*********************************/
void BSP_Init_WD(void)
{
  GPIO_setAsOutputPin(WATCHDOG_PORT, WATCHDOG_PIN);

#ifndef UNIT_TESTING
  PMM_unlockLPM5();
#endif
}

void BSP_Hit_WD(void)
{
  GPIO_setOutputHighOnPin(WATCHDOG_PORT, WATCHDOG_PIN);
  __delay_cycles(500);
  GPIO_setOutputLowOnPin(WATCHDOG_PORT, WATCHDOG_PIN);
}

/*********************************> Analog <*********************************/
eBSPStatus_t BSP_ANALOG_Init(sAnalog_t *Analog)
{
  eBSPStatus_t result = BSP_FAIL;
  /** Initialize the ADC */
  ADC12_B_init(Analog->baseAddress, &Analog->param);

  /** Enable the ADC */
  ADC12_B_enable(Analog->baseAddress);

  /** Setup the Sampling for ADC */
  ADC12_B_setupSamplingTimer(Analog->baseAddress,
                             ADC12_B_CYCLEHOLD_16_CYCLES,
                             ADC12_B_CYCLEHOLD_4_CYCLES,
                             ADC12_B_MULTIPLESAMPLESDISABLE);

  /** Configure Memory Buffer */
  ADC12_B_configureMemoryParam configureMemoryParam = {0};
  configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
  configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_A8;
  configureMemoryParam.refVoltageSourceSelect = ADC12_B_VREFPOS_EXTPOS_VREFNEG_VSS;
  configureMemoryParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
  configureMemoryParam.windowComparatorSelect = ADC12_B_WINDOW_COMPARATOR_DISABLE;
  configureMemoryParam.differentialModeSelect = ADC12_B_DIFFERENTIAL_MODE_DISABLE;
  ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

  result = BSP_OK;
  return result;
}

float BSP_ANALOG_ReadVoltage(sAnalog_t *Analog)
{
  uint16_t adcval = 0;
  float voltage;

  /** Start the conversion */
  ADC12_B_startConversion(Analog->baseAddress,
                          ADC12_B_MEMORY_0,
                          ADC12_B_SINGLECHANNEL);

  /** Wait for interrupt on memory buffer 0 */
  while (!ADC12_B_getInterruptStatus(Analog->baseAddress,
                                     0,
                                     ADC12_B_IFG0))
    ;

  /** Grab ADC value */
  adcval = ADC12_B_getResults(Analog->baseAddress, ADC12_B_MEMORY_0);

  voltage = (float)adcval;
  voltage *= Analog->refVoltage;
  voltage /= 4096;

  return voltage;
}
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
