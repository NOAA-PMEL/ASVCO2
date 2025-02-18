#include "unity.h"
#include "bsp.h"
#include "msp430fr5994.h"
#include "buffer_c.h"
#include "mock_gpio.h"
#include "mock_cs.h"
#include "mock_eusci_a_uart.h"
#include "mock_eusci_b_i2c.h"
#include "mock_eusci_b_spi.h"
#include "mock_timer_a.h"
#include "mock_timer_b.h"

#include "buffer8.h"
#include "sysconfig.h"


void setUp(void)
{
}

void tearDown(void)
{
}


/*********************************** GPIO TESTS ***********************************/
void configure_GPIO_Output(sGPIO_t *Toggle){
    GPIO_setAsOutputPin_Expect(Toggle->port,Toggle->pin);
    GPIO_selectInterruptEdge_Expect(Toggle->port,Toggle->pin,Toggle->interruptEdge);
    BSP_GPIO_Init(Toggle);
    TEST_ASSERT_EQUAL(true,Toggle->configured);
}

void configure_GPIO_Input(sGPIO_t *Toggle){
    GPIO_setAsInputPin_Expect(Toggle->port,Toggle->pin);
    GPIO_selectInterruptEdge_Expect(Toggle->port,Toggle->pin,Toggle->interruptEdge);
    BSP_GPIO_Init(Toggle);
    TEST_ASSERT_EQUAL(true,Toggle->configured);
}


void test_BSP_GPIO_Init_should_ConfigureOutput_when_OutputRequested(void){
    uint8_t port = 1;
    uint16_t pin = 2;
    uint8_t mode = 0;
    uint8_t res = 0;
    uint8_t edge = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.pullups=res,.interrupt=0,.interruptEdge=edge};
    GPIO_setAsOutputPin_Expect(port,pin);
    GPIO_selectInterruptEdge_Expect(port,pin,edge);
    BSP_GPIO_Init(&Toggle);
    TEST_ASSERT_EQUAL(true,Toggle.configured);
}


void test_BSP_GPIO_Init_should_ConfigureInput_when_InputRequested(void){
    uint8_t port = 4;
    uint16_t pin = 3;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.pullups=res,.interrupt=0,.interruptEdge=edge};
    GPIO_setAsInputPin_Expect(port,pin);
    GPIO_selectInterruptEdge_Expect(port,pin,edge);    
    BSP_GPIO_Init(&Toggle);
    TEST_ASSERT_EQUAL(true,Toggle.configured);
}

void test_BSP_GPIO_Init_should_ConfigureInputWithPullups_when_InputWithPullupsRequested(void){
    uint8_t port = 8;
    uint16_t pin = 0;
    uint8_t mode = 1;
    uint8_t res = 1;
    uint8_t edge = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.pullups=res,.interrupt=0,.interruptEdge=edge};
    GPIO_setAsInputPinWithPullUpResistor_Expect(port,pin);
    GPIO_selectInterruptEdge_Expect(port,pin,edge);        
    BSP_GPIO_Init(&Toggle);
    TEST_ASSERT_EQUAL(true,Toggle.configured);
}

void test_BSP_GPIO_Init_should_ConfigureInputWithPulldowns_when_InputWithPulldownsRequested(void){
    uint8_t port = 3;
    uint16_t pin = 6;
    uint8_t mode = 1;
    uint8_t res = 2;
    uint8_t edge = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.pullups=res,.interrupt=0,.interruptEdge=edge};
    GPIO_setAsInputPinWithPullDownResistor_Expect(port,pin);
    GPIO_selectInterruptEdge_Expect(port,pin,edge);            
    BSP_GPIO_Init(&Toggle);
    TEST_ASSERT_EQUAL(true,Toggle.configured);
}

void test_BSP_GPIO_SetPin_should_SetPinHigh(void){
    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 0;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Output(&Toggle);
    
    GPIO_setOutputHighOnPin_Expect(port,pin);
    BSP_GPIO_SetPin(&Toggle);

    Toggle.inverted = 1;
    GPIO_setOutputLowOnPin_Expect(port,pin);
    BSP_GPIO_SetPin(&Toggle);
    
}

void test_BSP_GPIO_SetPin_should_NotSetPin_if_PinIsInput(void){
    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Input(&Toggle);
    BSP_GPIO_SetPin(&Toggle);
}

void test_BSP_GPIO_ClearPin_should_NotClearPin_if_PinIsInput(void){
    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 1;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Input(&Toggle);
    BSP_GPIO_SetPin(&Toggle);
}

void test_BSP_GPIO_ClearPin_should_ClearPinLow(void){
    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 0;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 0;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Output(&Toggle);
    
    GPIO_setOutputLowOnPin_Expect(port,pin);
    BSP_GPIO_ClearPin(&Toggle);

    Toggle.inverted = 1;
    GPIO_setOutputHighOnPin_Expect(port,pin);
    BSP_GPIO_ClearPin(&Toggle);
}


void test_BSP_GPIO_ReadInputPin_should_ReadValue(void){

    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 1;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Input(&Toggle);

    GPIO_getInputPinValue_ExpectAndReturn(port,pin,1);
    TEST_ASSERT_EQUAL(1,BSP_GPIO_ReadInputPin(&Toggle));

    GPIO_getInputPinValue_ExpectAndReturn(port,pin,0);
    TEST_ASSERT_EQUAL(0,BSP_GPIO_ReadInputPin(&Toggle));

}

void test_BSP_GPIO_ReadInputPin_should_Fail_if_ReadingOutput(void){
    uint8_t port = 1;
    uint16_t pin = 0;
    uint8_t mode = 0;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t inverted = 1;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.inverted=inverted};
    
    configure_GPIO_Output(&Toggle);

    TEST_ASSERT_EQUAL(-1,BSP_GPIO_ReadInputPin(&Toggle));
}

void test_BSP_GPIO_EnableInterrupt_should_EnableInterruptOnPin(void){
    uint8_t port = 5;
    uint16_t pin = 6;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t interrupt = 0;
    uint8_t inverted = 1;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.interrupt=interrupt};

    configure_GPIO_Input(&Toggle);

    GPIO_enableInterrupt_Expect(port,pin);
    BSP_GPIO_EnableInterrupt(&Toggle);
    TEST_ASSERT_EQUAL(1,Toggle.interrupt);
}

void test_GPIO_clearInterrupt_should_ClearInterruptOnPin(void){
    uint8_t port = 3;
    uint16_t pin = 1;
    uint8_t mode = 1;
    uint8_t res = 0;
    uint8_t edge = 0;
    uint8_t interrupt = 1;
    uint8_t inverted = 1;
    sGPIO_t Toggle = {.port=port,.pin=pin,.mode=mode,.configured=0,.interruptEdge=edge,.interrupt=interrupt};

    configure_GPIO_Input(&Toggle);


    GPIO_clearInterrupt_Expect(port,pin);
    BSP_GPIO_ClearInterrupt(&Toggle);
    TEST_ASSERT_EQUAL(0,Toggle.interrupt);
}


void test_GPIO_SetPeripheralOutputPin_should_SetPeripheralOut(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin_Expect(1,2,3);
    BSP_GPIO_SetPeripheralOutputPin(1,2,3);
}

void test_GPIO_SetPeripheralInputPin_should_SetPeripheralOut(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin_Expect(9,8,7);
    BSP_GPIO_SetPeripheralOutputPin(9,8,7);
}



/********************************** SYSCLK TESTS **********************************/
/**       !!This test specifically written for this BSP (FlowController).!!       */
void test_BSP_SYSCLK_Init_should_SetClks_and_ReturnValid(void){

    CS_setExternalClockSource_Expect(LFXT_FREQ,HFXT_FREQ);
    CS_initClockSignal_Expect(ACLKREQEN,ACLK_SRC,ACLK_DIV);
    CS_initClockSignal_Expect(MCLKREQEN,MCLK_SRC,MCLK_DIV);
    CS_initClockSignal_Expect(SMCLKREQEN,SMCLK_SRC,SMCLK_DIV);
    CS_initClockSignal_Expect(MODCLKREQEN,MODOSC_SRC,MODOSC_DIV);
    // CS_turnOffVLO_Expect();
    CS_turnOnHFXTWithTimeout_ExpectAndReturn(HFXT_DRIVE,HFXT_TIMEOUT,0x01);
    CS_turnOffLFXT_Expect();
    TEST_ASSERT_EQUAL(BSP_OK,BSP_SYSCLK_Init());
}

void ConfigureClocks(void){
    CS_setExternalClockSource_Expect(LFXT_FREQ,HFXT_FREQ);
    CS_initClockSignal_Expect(ACLKREQEN,ACLK_SRC,ACLK_DIV);
    CS_initClockSignal_Expect(MCLKREQEN,MCLK_SRC,MCLK_DIV);
    CS_initClockSignal_Expect(SMCLKREQEN,SMCLK_SRC,SMCLK_DIV);
    CS_initClockSignal_Expect(MODCLKREQEN,MODOSC_SRC,MODOSC_DIV);
    // CS_turnOffVLO_Expect();
    CS_turnOnHFXTWithTimeout_ExpectAndReturn(HFXT_DRIVE,HFXT_TIMEOUT,0x01);
    CS_turnOffLFXT_Expect();
    TEST_ASSERT_EQUAL(BSP_OK,BSP_SYSCLK_Init());
}


/*********************************** UART TESTS ***********************************/
void test_CalculateUartBaudRegs_should_ProvideValidRegs_and_ReturnSuccess(void){
    sUART_t Test;
    Test.param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;

    /** Configure the SyClk */
    ConfigureClocks();

    /** Calculate the Baud Regs */
    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 2400 */
    /** clockPrescalar = 23 */
    /** firstModReg = 13 */
    /** secondModreg = 182 */
    /** overSampling = 1 */
    Test.baudrate = 2400;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(23,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(13,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(182,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(1,Test.param.overSampling);

    /** Calculate the Baud Regs */
    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 4800 */
    /** clockPrescalar = 11 */
    /** firstModReg = 14 */
    /** secondModreg = 238 */
    /** overSampling = 1 */
    Test.baudrate = 4800;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(11,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(14,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(238,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(1,Test.param.overSampling);

    /** Calculate the Baud Regs */
    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 9600 */
    /** clockPrescalar = 5 */
    /** firstModReg = 15 */
    /** secondModreg = 146 */
    /** overSampling = 1 */
    Test.baudrate = 9600;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(5,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(15,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(146,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(1,Test.param.overSampling);

    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 19200 */
    /** clockPrescalar = 2 */
    /** firstModReg = 15 */
    /** secondModreg = 183 */
    /** overSampling = 1 */
    Test.baudrate =19200;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(2,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(15,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(183,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(1,Test.param.overSampling);

    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 38400 */
    /** clockPrescalar = 1 */
    /** firstModReg = 7 */
    /** secondModreg = 223 */
    /** overSampling = 1 */
    Test.baudrate =38400;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(1,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(7,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(223,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(1,Test.param.overSampling);


    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 57600 */
    /** clockPrescalar = 15 */
    /** firstModReg = 0 */
    /** secondModreg = 251 */
    /** overSampling = 1 */
    Test.baudrate =57600;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(15,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(0,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(251,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(0,Test.param.overSampling);

    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 915975 Hz */
    /** Baudrate of 115200 */
    /** clockPrescalar = 7 */
    /** firstModReg = 0 */
    /** secondModreg = 255 */
    /** overSampling = 0 */
    Test.baudrate =115200;
    TEST_ASSERT_EQUAL(BSP_OK, CalculateUartBaudRegs(&Test));
    TEST_ASSERT_EQUAL(7,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(0,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(255,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(0,Test.param.overSampling);
    
}


void test_CalculateUartBaudRegs_should_Fail_for_InvalidBaud(void){
    sUART_t Test;
    Test.param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;

    /** Configure the SyClk */
    ConfigureClocks();

    Test.baudrate = 2399;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 2401;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 4799;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 4801;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));
    
    Test.baudrate = 9599;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 9601;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 19199;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 19201;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 38399;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 38401;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));
    
    Test.baudrate = 57599;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 57601;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 115199;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));

    Test.baudrate = 115201;
    TEST_ASSERT_EQUAL(BSP_FAIL, CalculateUartBaudRegs(&Test));
}



void test_BSP_UART_Init_should_InitializeRequestedUart_and_ReturnSuccess(void){
    /** From online calcutor located at */
    /** http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html */
    /** SMCLK Freq = 1,831,820 Hz */
    /** Baudrate of 115200 */
    /** clockPrescalar = 7 */
    /** firstModReg = 0 */
    /** secondModreg = 255 */
    /** overSampling = 0 */
    uint16_t baseAddr = 0x0846;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};
    EUSCI_A_UART_init_ExpectAndReturn(baseAddr,&Test.param,STATUS_SUCCESS);
    TEST_ASSERT_EQUAL(BSP_OK,BSP_UART_Init(&Test));
    TEST_ASSERT_EQUAL(7,Test.param.clockPrescalar);
    TEST_ASSERT_EQUAL(0,Test.param.firstModReg);
    TEST_ASSERT_EQUAL(255,Test.param.secondModReg);
    TEST_ASSERT_EQUAL(0,Test.param.overSampling);
    TEST_ASSERT_EQUAL(1,Test.configured);
    TEST_ASSERT_EQUAL(EUSCI_A_UART_CLOCKSOURCE_SMCLK,Test.param.selectClockSource);
    TEST_ASSERT_EQUAL(EUSCI_A_UART_NO_PARITY,Test.param.parity);
    TEST_ASSERT_EQUAL(EUSCI_A_UART_ONE_STOP_BIT,Test.param.msborLsbFirst);
    TEST_ASSERT_EQUAL(EUSCI_A_UART_ONE_STOP_BIT,Test.param.numberofStopBits);
    TEST_ASSERT_EQUAL(EUSCI_A_UART_MODE,Test.param.uartMode );
}


void test_BSP_UART_Enable_should_EnableTheUartSelected(void){
    uint16_t baseAddr = 0x0846;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_enable_Expect(baseAddr);
    BSP_UART_Enable(&Test);
}


void test_BSP_UART_Disable_should_DisableTheUartSelected(void){
    uint16_t baseAddr = 0x0846;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_disable_Expect(baseAddr);
    BSP_UART_Disable(&Test); 
}


void test_BSP_UART_EnableInterrupt_should_SetTheInterrupt(void){
    uint16_t baseAddr = 0x1A75;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_enableInterrupt_Expect(baseAddr,UCRXIE);
    BSP_UART_EnableInterrupt(&Test,EUSCI_A_UART_RECEIVE_INTERRUPT);
}

void test_BSP_UART_DisableInterrupt_should_DisableTheInterrupt(void){
    uint16_t baseAddr = 0x3A00;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_disableInterrupt_Expect(baseAddr,UCRXIE);
    BSP_UART_DisableInterrupt(&Test,EUSCI_A_UART_RECEIVE_INTERRUPT);
}

void test_BSP_UART_ClearInterrupt_should_ClearTheInterrupt(void){
    uint16_t baseAddr = 0xA090;
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_clearInterrupt_Expect(baseAddr,UCRXIE);
    BSP_UART_ClearInterrupt(&Test,EUSCI_A_UART_RECEIVE_INTERRUPT);
}

void test_BSP_UART_putc_should_SendChar_on_SelectedUart(void){
    uint16_t baseAddr = 0x3A00;
    char val = 'A';
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    EUSCI_A_UART_transmitData_Expect(baseAddr,val);
    BSP_UART_putc(&Test,val);

}

void puts_send_str(uint16_t baseAddr,char *val,uint16_t len){
    uint16_t i;
    
    for(i=0;i<len;i++){
        EUSCI_A_UART_transmitData_Expect(baseAddr,val[i]);
    }
}
void test_BPS_UART_puts_should_SendString_on_SelectedUart(void){
    uint16_t baseAddr = 0x3A00;
    char *pVal = "ABCDEDF";
    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    puts_send_str(baseAddr,pVal,sizeof(pVal)/sizeof(pVal[0]));
    EUSCI_A_UART_queryStatusFlags_ExpectAndReturn(baseAddr,EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG,0);
    BSP_UART_puts(&Test,pVal,sizeof(pVal)/sizeof(pVal[0]));
    

}

void test_BSP_UART_gets_should_ReturnString_and_Length(void){
    uint16_t baseAddr = 0x3A00;
    char str[] = "I only bought it this time cause the mustache was rad\r\n";
    char rtnstr[SYS_UART_BUFFER_LEN];
    uint8_t i;

    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};
    for(i=0;i<strlen(str);i++){
        BufferC_putc(&Test.buf,str[i]);
    }
    

    TEST_ASSERT_EQUAL(strlen(str),BSP_UART_gets(&Test,&rtnstr[0]));
    TEST_ASSERT_EQUAL_STRING(str,rtnstr);


}

void test_BSP_UART_gets_should_ReturnZero_for_NullString(void){
    uint16_t baseAddr = 0x3A00;
    char str[] = "I only bought it this time cause the mustache was rad\r\n";
    char rtnstr[SYS_UART_BUFFER_LEN];
    uint8_t i;

    sUART_t Test = {.baseAddress = baseAddr,.baudrate = 115200};

    BufferC_Clear(&Test.buf);

    TEST_ASSERT_EQUAL(0,BSP_UART_gets(&Test,&rtnstr[0]));
    
}

/************************************ I2C TESTS ***********************************/
void test_BSP_I2C_Init_should_IniitalizeTheCorrectI2C(void){
    uint16_t baseAddr = 0xB0B0;
    sI2C_t Test = {.baseAddress = baseAddr};
    EUSCI_B_I2C_initMaster_Expect(baseAddr,&Test.param);
    BSP_I2C_Init(&Test);
    TEST_ASSERT_EQUAL(EUSCI_B_I2C_CLOCKSOURCE_SMCLK,Test.param.selectClockSource);

}

void test_BSP_I2C_Enable_should_EnableTheSelectedI2C(void){
    uint16_t baseAddr = 0x1C0A;
    sI2C_t Test = {.baseAddress = baseAddr};
    EUSCI_B_I2C_enable_Expect(baseAddr);
    BSP_I2C_Enable(&Test);
}


void test_BSP_I2C_Disable_should_DisableTheSelectedI2C(void){
    uint16_t baseAddr = 0x1C0A;
    sI2C_t Test = {.baseAddress = baseAddr};
    EUSCI_B_I2C_disable_Expect(baseAddr);
    BSP_I2C_Disable(&Test);
}

void test_BSP_SetSlaveAddress_should_SetTheAddress_and_StoreInStruct(void){
    uint16_t baseAddr = 0x00A0;
    sI2C_t Test = {.baseAddress = baseAddr};
    uint8_t addr = 0xA1;

    EUSCI_B_I2C_setSlaveAddress_Expect(baseAddr,addr);
    BSP_I2C_SetSlaveAddress(&Test,addr);
    TEST_ASSERT_EQUAL(addr,Test.slaveAddress);

}




void DisableInterruptCalled(uint16_t interrupt, uint8_t mask,uint8_t result){

    if(interrupt & INTERRUPT_UART_A0){
        /** Disable UART A0 */
        EUSCI_A_UART_getInterruptStatus_ExpectAndReturn(EUSCI_A0_BASE,0xFFu,result);
        EUSCI_A_UART_disableInterrupt_Expect(EUSCI_A0_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_UART_A1){
        /** Disable UART A1 */
        EUSCI_A_UART_getInterruptStatus_ExpectAndReturn(EUSCI_A1_BASE,0xFFu,result);
        EUSCI_A_UART_disableInterrupt_Expect(EUSCI_A1_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_UART_A2){
        /** Disable UART A2 */
        EUSCI_A_UART_getInterruptStatus_ExpectAndReturn(EUSCI_A2_BASE,0xFFu,result);
        EUSCI_A_UART_disableInterrupt_Expect(EUSCI_A2_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_UART_A3){
        /** Disable UART A3 */
        EUSCI_A_UART_getInterruptStatus_ExpectAndReturn(EUSCI_A3_BASE,0xFFu,result);
        EUSCI_A_UART_disableInterrupt_Expect(EUSCI_A3_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_I2C_B0){
        /** Disable I2C B0 */
        EUSCI_B_I2C_getInterruptStatus_ExpectAndReturn(EUSCI_B0_BASE,0xFFu,result);
        EUSCI_B_I2C_disableInterrupt_Expect(EUSCI_B0_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_SPI_B1){
        /** Disable SPI B1 */
        EUSCI_B_SPI_getInterruptStatus_ExpectAndReturn(EUSCI_B1_BASE,0xFFu,result);
        EUSCI_B_SPI_disableInterrupt_Expect(EUSCI_B1_BASE,0xFFu);
      }
    
      if(interrupt & INTERRUPT_GPIO_P1_4){
        /** Disable GPIO Port1 Pin4 */
        GPIO_getInterruptStatus_ExpectAndReturn(1,4,result);
        GPIO_disableInterrupt_Expect(1,4);
      }
    
      if(interrupt  & INTERRUPT_GPIO_P1_5){
        /** Disable GPIO Port1 Pin5 */
        GPIO_getInterruptStatus_ExpectAndReturn(1,5,result);
        GPIO_disableInterrupt_Expect(1,5);
      }
}

void EnableInterruptCalled(uint16_t interrupt, uint8_t mask){
    
        if(interrupt & INTERRUPT_UART_A0){
            /** Disable UART A0 */
            EUSCI_A_UART_enableInterrupt_Expect(EUSCI_A0_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_UART_A1){
            /** Disable UART A1 */
            EUSCI_A_UART_enableInterrupt_Expect(EUSCI_A1_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_UART_A2){
            /** Disable UART A2 */
            EUSCI_A_UART_enableInterrupt_Expect(EUSCI_A2_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_UART_A3){
            /** Disable UART A3 */
            EUSCI_A_UART_enableInterrupt_Expect(EUSCI_A3_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_I2C_B0){
            /** Disable I2C B0 */
            EUSCI_B_I2C_enableInterrupt_Expect(EUSCI_B0_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_SPI_B1){
            /** Disable SPI B1 */
            EUSCI_B_SPI_clearInterrupt_Expect(EUSCI_B1_BASE,0xFFu);
            EUSCI_B_SPI_enableInterrupt_Expect(EUSCI_B1_BASE,mask);
          }
        
          if(interrupt & INTERRUPT_GPIO_P1_4){
            /** Disable GPIO Port1 Pin4 */
            GPIO_enableInterrupt_Expect(1,4);
          }
        
          if(interrupt  & INTERRUPT_GPIO_P1_5){
            /** Disable GPIO Port1 Pin5 */
            GPIO_enableInterrupt_Expect(1,5);
          }
    }

void test_BSP_I2C_ReceiveBytes_should_Setup_and_WaitFor_Data(void){
    sI2C_t Test;
    uint16_t len = 20;  
    uint32_t timeout = 30;
    uint16_t interrupt = INTERRUPT_UART_A0 | INTERRUPT_UART_A1 | INTERRUPT_UART_A2 | INTERRUPT_UART_A3 |
                         INTERRUPT_SPI_B1 | INTERRUPT_GPIO_P1_4 | INTERRUPT_GPIO_P1_5;
    uint8_t mask = 0xff;

    Test.baseAddress = EUSCI_B0_BASE;
    // DisableInterruptCalled(interrupt, mask,5);
    EUSCI_B_I2C_setMode_Expect(EUSCI_B0_BASE,EUSCI_B_I2C_RECEIVE_MODE);
    EUSCI_B_I2C_masterReceiveStart_Expect(EUSCI_B0_BASE);
    
    // EnableInterruptCalled(interrupt,5);
    EUSCI_B0.success = true;
    BSP_I2C_ReceiveBytes(&Test,len,timeout);
    

}



void test_BSP_I2C_SendMultiBytes_should_StartMultipleByteTransfer(void){
    uint16_t baseAddr = 0x1C0A;
    uint8_t values[16];
    uint8_t i;
    sI2C_t Test = {.baseAddress = EUSCI_B0_BASE};
    /** Populate the tx buffer */
    for(i=0;i<16;i++){
        Buffer8_put(&Test.Tx,i*2);
    }

    EUSCI_B_I2C_setMode_Expect(EUSCI_B0_BASE,EUSCI_B_I2C_TRANSMIT_MODE);
    EUSCI_B_I2C_masterSendStart_Expect(EUSCI_B0_BASE);

    BSP_I2C_SendBytes(&Test);

    for(i=0;i<16;i++){
        TEST_ASSERT_EQUAL(i*2,EUSCI_B0.Tx.buf[i]);
    }
}


void test_BSP_SPI_Init_should_SetParams_and_Init(void){
    uint16_t baseAddr = EUSCI_B1_BASE;
    sSPI_t Test = {.baseAddress = baseAddr };
    uint8_t i;

    EUSCI_B_SPI_initMaster_Expect(baseAddr,&Test.param);
    BSP_SPI_Init(&Test);
}


void test_BSP_Enable_should_EnableTheSPI(void){
    uint16_t baseAddr = EUSCI_B1_BASE;
    sSPI_t Test = {.baseAddress = baseAddr };
    uint8_t i;

    EUSCI_B_SPI_enable_Expect(baseAddr);
    BSP_SPI_Enable(&Test);
}


void test_BSP_Disable_should_EnableTheSPI(void){
    uint16_t baseAddr = EUSCI_B1_BASE;
    sSPI_t Test = {.baseAddress = baseAddr };
    uint8_t i;

    EUSCI_B_SPI_disable_Expect(baseAddr);
    BSP_SPI_Disable(&Test);
}

void spi_init(sSPI_t *T){
    EUSCI_B_SPI_initMaster_Expect(T->baseAddress,&T->param);
    BSP_SPI_Init(T);
}
void test_BSP_SendBytes_should_PrepAndSend_Data(void){
    uint16_t baseAddr = EUSCI_B1_BASE;
    uint16_t interruptMask = (INTERRUPT_UART_A0 | 
        INTERRUPT_UART_A1 |
        INTERRUPT_UART_A2 |
        INTERRUPT_UART_A3 |
        INTERRUPT_I2C_B0 |
        INTERRUPT_GPIO_P1_4|
        INTERRUPT_GPIO_P1_5); 
    uint8_t mask = 0xff;
    uint8_t mask_SPI = EUSCI_B_SPI_RECEIVE_INTERRUPT;
    uint8_t result = 0xAA;
    sSPI_t Test = {.baseAddress = baseAddr };
    spi_init(&Test);
    uint8_t i;

    for(i=0;i<128;i++){
        Buffer8_put(&Test.Tx,i);
    }
    // DisableInterruptCalled(interruptMask,mask,result);
    EUSCI_B_SPI_enable_Expect(baseAddr);
    interruptMask = INTERRUPT_SPI_B1;
    // EnableInterruptCalled(interruptMask,mask_SPI);
    EUSCI_B_SPI_transmitData_Expect(baseAddr,0);

    
    // DisableInterruptCalled(interruptMask,mask,result);
    TEST_ASSERT_EQUAL(BSP_OK,BSP_SPI_SendBytes(&Test));
}



void test_BSP_ReceiveBytes_should_PrepSendAndReceive_Data(void){
    uint16_t baseAddr = EUSCI_B1_BASE;
    uint16_t interruptMask = (INTERRUPT_UART_A0 | 
        INTERRUPT_UART_A1 |
        INTERRUPT_UART_A2 |
        INTERRUPT_UART_A3 |
        INTERRUPT_I2C_B0 |
        INTERRUPT_GPIO_P1_4|
        INTERRUPT_GPIO_P1_5); 
    uint8_t mask = 0xff;
    uint8_t mask_SPI = EUSCI_B_SPI_RECEIVE_INTERRUPT;
    uint8_t result = 0xAA;
    sSPI_t Test = {.baseAddress = baseAddr };
    spi_init(&Test);
    uint8_t i;

    for(i=0;i<128;i++){
        Buffer8_put(&Test.Tx,i);
    }

    // DisableInterruptCalled(interruptMask,mask,result);
    // interruptMask = INTERRUPT_SPI_B1;
    // EnableInterruptCalled(INTERRUPT_SPI_B1,EUSCI_B_SPI_RECEIVE_INTERRUPT);
    // EUSCI_B_SPI_enableInterrupt_Expect(EUSCI_B1_BASE,EUSCI_B_SPI_RECEIVE_INTERRUPT);
    EUSCI_B_SPI_transmitData_Expect(baseAddr,0);
    // EnableInterruptCalled(interruptMask,result);
    EUSCI_B0.success = true;
    // EnableInterruptCalled(interruptMask,result);
    TEST_ASSERT_EQUAL(BSP_OK,BSP_SPI_ReceiveBytes(&Test,128));
    // for(i=0;i<128;i++){
    //     TEST_ASSERT_EQUAL(i,EUSCI_B1.Tx.buf[i]);
    // }

}