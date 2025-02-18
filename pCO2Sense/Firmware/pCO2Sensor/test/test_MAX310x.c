#include "unity.h"
#include "MAX310x.h"
#include "mock_bsp.h"
#include "mock_buffer8.h"

sMAX310X_t MAX310X = {  .cs = { .port = MAX310X_CS_PORT,
                                .pin = MAX310X_CS_PIN,
                                .mode = MAX310X_CS_DIR,
                                .inverted = MAX310X_CS_POL },
                        .nReset = { .port = MAX310X_NRESET_PORT,
                                    .pin = MAX310X_NRESET_PIN,
                                    .mode = MAX310X_NRESET_DIR,
                                    .inverted = MAX310X_NRESET_POL },
                        .nIRQ = {   .port = MAX310X_NIRQ_PORT,
                                    .pin = MAX310X_NIRQ_PIN,
                                    .mode = MAX310X_NIRQ_DIR,
                                    .inverted = MAX310X_NIRQ_POL
                                },
                        .SPI = {.param = {  .selectClockSource = MAX310X_SPI_CLK_SRC,
                                            .clockSourceFrequency = MAX310X_SPI_CLK_FREQ,
                                            .desiredSpiClock = MAX310X_SPI_CLK_DES_FREQ,
                                            .msbFirst = MAX310X_SPI_MSB_FIRST,
                                            .clockPhase = MAX310X_SPI_CLK_PHASE,
                                            .clockPolarity = MAX310X_SPI_CLK_POL,
                                            .spiMode = MAX310X_SPI_CLK_MODE
                                        }


                                },
                        .Baud = {   .fref = MAX310X_CRYSTAL_FREQUENCY,
                                    .rateMode = MAX310X_DEFAULT_RATEMODE,
                                    .DIVMSB = 0,
                                    .DIVLSB = 0,
                                    .BRGConfig = 0,
                                    .Baudrate = MAX310X_DEFAULT_BAUDRATE
                                },
                        .Port = 1
};

void setUp(void)
{
}

void tearDown(void)
{
}

void mock_max_write(sMAX310X_t *MAX, uint8_t reg, uint8_t val){
    uint8_t reg_with_writebit = (0x80 | reg) ;
    reg_with_writebit |= (MAX->Port << 5);
    
    Buffer8_Clear_Expect(&MAX->SPI.Tx);  
    BSP_GPIO_SetPin_Expect((sGPIO_t *)&MAX->cs);
    BSP_SPI_put_Expect(&MAX->SPI,val);

    BSP_GPIO_ClearPin_Expect((sGPIO_t*) &MAX->cs);
}

void mock_set_interrupts(sMAX310X_t *MAX){
    mock_max_write(MAX,0x01,0x28);
    mock_max_write(MAX,0x03,0x01);
    mock_max_write(MAX,MAX310XR_RXTIMEOUT,MAX310X_RECEIVE_INTERRUPT_LEN);
    mock_max_write(MAX,MAX310XR_MODE1,MAXMODE1_AUTOSLEEP);
}

void test_MAX310X_Init_should_Initialize_I2C_and_Setup(void){
    // Arrange

    // Expect
    BSP_GPIO_Init_Expect((sGPIO_t *)&MAX310X.nReset);
    BSP_GPIO_Init_Expect((sGPIO_t *)&MAX310X.cs);
    BSP_GPIO_Init_Expect((sGPIO_t *)&MAX310X.nIRQ);

    /** Configure I2C */
    BSP_GPIO_ReadInputPin_ExpectAndReturn((sGPIO_t *)&MAX310X.nIRQ,1);
    BSP_SPI_Init_Expect(&MAX310X.SPI);
    mock_set_interrupts(&MAX310X);

    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK,MAX310X_Init(&MAX310X));
}

void test_MAX310X_WriteByte_should_SetCS_WriteSPI_x2_and_Return(void){
    // Arrange
    uint8_t reg = 0x03;
    uint8_t val = 0xAA;
    uint8_t reg_with_writebit = (0x80 | reg);
    
    // Expect
    mock_max_write(&MAX310X,reg,val);


    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK,MAX310X_WriteByte(&MAX310X,reg,val));
}

void mock_max_puts(sMAX310X_t *MAX, uint8_t reg, uint8_t *val, uint8_t len){
    uint8_t reg_with_writebit = (0x80 | reg) | (MAX->Port << 5);

    // Expect
    BSP_GPIO_SetPin_Expect((sGPIO_t *)&MAX->cs);
    BSP_SPI_puts_Expect(&MAX->SPI,val,len);
    BSP_GPIO_ClearPin_Expect((sGPIO_t*) &MAX->cs);
}
void test_MAX310X_puts_should_SetCS_WriteSPIAll_and_ReturnSuccessful(void){

    // Arrange
    uint8_t reg = 0x0B;
    uint8_t val[128];
    uint8_t len = SIZE_OF(val);
    
    uint8_t i;


    for(i=0;i<len;i++){
        val[i] = i;
    }

    // Expect
    mock_max_puts(&MAX310X,reg,val,len);

    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_puts(&MAX310X,reg,val,len));
}

void mock_max_read(sMAX310X_t *MAX, uint8_t reg, uint8_t *val){
    uint8_t reg_with_writebit =  reg | (MAX->Port<<5);

    BSP_GPIO_SetPin_Expect((sGPIO_t *)&MAX->cs);
    BSP_SPI_get_ExpectAndReturn(&MAX->SPI,reg_with_writebit,*val);
    BSP_GPIO_ClearPin_Expect((sGPIO_t*) &MAX->cs);

}
void test_MAX310X_ReadByte_should_SetCS_ReadByte_andReturnSuccess(void){
    // Arrange
    uint8_t reg = 0x07;
    uint8_t val = 0xFE;
    uint8_t *pVal = &val;
    
    // Expect
    mock_max_read(&MAX310X,reg,pVal);
 
    // Act
    TEST_ASSERT_EQUAL(val, MAX310X_ReadByte(&MAX310X,reg));

}

void mock_max_gets(sMAX310X_t *MAX, uint8_t reg, uint8_t *retval, uint8_t *expectedval,uint8_t len){
    uint8_t reg_with_writebit = ( reg  | (MAX310X.Port << 5));

    BSP_GPIO_SetPin_Expect((sGPIO_t *)&MAX->cs);
    BSP_SPI_gets_Expect(&MAX->SPI,reg_with_writebit,retval,len);
    BSP_SPI_gets_ReturnArrayThruPtr_val(expectedval,len);
    
    BSP_GPIO_ClearPin_Expect((sGPIO_t*) &MAX->cs);

}


void test_MAX310X_gets_should_SetCS_ReadString_and_ReturnSuccess(void){

    // Arrange
    uint8_t reg = 0x07;
    uint8_t val[128];
    uint8_t data[128];
    uint8_t *pData = &data[0];
    uint8_t *pVal = &val[0];
    uint8_t len = SIZE_OF(data);
    uint8_t i;
    for(i=0;i<len;i++){
        val[i] = i;
        data[i] = 0;
    }

    // Expect
    mock_max_gets(&MAX310X,reg,pData,pVal,len);


    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_gets(&MAX310X,reg,pData,len));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(val,data,len);
}

void test_MAX310X_HW_Reset_should_ToggleResetPin_and_Return(void){
    // Arrange

    // Expect
    BSP_GPIO_SetPin_Expect((sGPIO_t*)&MAX310X.nReset);
    _delay_ms_Expect(1);
    BSP_GPIO_ClearPin_Expect((sGPIO_t*)&MAX310X.nReset);

    // Act
    MAX310X_HW_Reset(&MAX310X);
}


void test_MAX310X_SW_Reset_should_SendCommands_and_Return(void){
    // Arrange
    uint8_t reg1 = MAX310XR_MODE2;
    uint8_t val1 = 0x01;
    uint8_t reg2 = MAX310XR_MODE2;
    uint8_t val2 = 0x00;

    // Expect
    mock_max_write(&MAX310X,reg1,val1);
    mock_max_write(&MAX310X,reg2,val2);

    // Act
    MAX310X_SW_Reset(&MAX310X);
}

void test_MAX310X_CalculateBRG_should_CalculateD_and_PopulateStruct_115200Baud(void){
    // Arrange
    sMAX310XBaud_t B = {    .fref = MAX310X_CRYSTAL_FREQUENCY,
                            .rateMode = MAX310X_DEFAULT_RATEMODE,
                            .DIVMSB = 0,
                            .DIVLSB = 0,
                            .BRGConfig = 0,
                            .Baudrate = MAX310X_DEFAULT_BAUDRATE
                        };

    uint32_t Baudrate = 115200;

    // Expect


    // Act
    MAX310X_CalculateBRG(&B,Baudrate);

    TEST_ASSERT_EQUAL(Baudrate, B.Baudrate);
    TEST_ASSERT_EQUAL(0x00,B.DIVMSB);
    TEST_ASSERT_EQUAL(0x02,B.DIVLSB);
    TEST_ASSERT_EQUAL(0,B.BRGConfig);
}

void test_MAX310X_CalculateBRG_should_CalculateD_and_PopulateStruct_38400Baud(void){
    // Arrange
    sMAX310XBaud_t B = {    .fref = MAX310X_CRYSTAL_FREQUENCY,
                            .rateMode = MAX310X_DEFAULT_RATEMODE,
                            .DIVMSB = 0,
                            .DIVLSB = 0,
                            .BRGConfig = 0,
                            .Baudrate = MAX310X_DEFAULT_BAUDRATE
                        };
                        
    uint32_t Baudrate = 38400;

    // Expect


    // Act
    MAX310X_CalculateBRG(&B,Baudrate);

    TEST_ASSERT_EQUAL(Baudrate, B.Baudrate);
    TEST_ASSERT_EQUAL(0x00,B.DIVMSB);
    TEST_ASSERT_EQUAL(0x06,B.DIVLSB);
    TEST_ASSERT_EQUAL(0x00,B.BRGConfig);
}

void test_MAX310X_CalculateBRG_should_CalculateD_and_PopulateStruct_9600Baud(void){
    // Arrange
    sMAX310XBaud_t B = {    .fref = MAX310X_CRYSTAL_FREQUENCY,
                            .rateMode = MAX310X_DEFAULT_RATEMODE,
                            .DIVMSB = 0,
                            .DIVLSB = 0,
                            .BRGConfig = 0,
                            .Baudrate = MAX310X_DEFAULT_BAUDRATE
                        };
                        
    uint32_t Baudrate = 9600;

    // Expect


    // Act
    MAX310X_CalculateBRG(&B,Baudrate);

    TEST_ASSERT_EQUAL(Baudrate, B.Baudrate);
    TEST_ASSERT_EQUAL(0x00,B.DIVMSB);
    TEST_ASSERT_EQUAL(24,B.DIVLSB);
    TEST_ASSERT_EQUAL(0x00,B.BRGConfig);
}

void test_MAX310X_CalculateBRG_should_CalculateD_and_PopulateStruct_460800Baud(void){
    // Arrange
    sMAX310XBaud_t B = {    .fref = MAX310X_CRYSTAL_FREQUENCY,
                            .rateMode = MAX310X_DEFAULT_RATEMODE,
                            .DIVMSB = 0,
                            .DIVLSB = 0,
                            .BRGConfig = 0,
                            .Baudrate = MAX310X_DEFAULT_BAUDRATE
                        };
                        
    uint32_t Baudrate = 460800;

    // Expect


    // Act
    MAX310X_CalculateBRG(&B,Baudrate);

    TEST_ASSERT_EQUAL(Baudrate, B.Baudrate);
    TEST_ASSERT_EQUAL(0x00,B.DIVMSB);
    TEST_ASSERT_EQUAL(0x00,B.DIVLSB);
    TEST_ASSERT_EQUAL(8,B.BRGConfig);
}

void test_MAX310X_SetBaudrate_should_CalculateBaud_and_SetRegisters(void){
    // Arrange
    uint32_t baudrate = 9600;

    // Expect
    mock_max_write(&MAX310X,MAX310XR_PLLCONFIG,0x01);
    mock_max_write(&MAX310X,MAX310XR_BRGCONFIG, 0x00);
    mock_max_write(&MAX310X,MAX310XR_DIVMSB, 0x00);
    mock_max_write(&MAX310X,MAX310XR_DIVLSB, 24);
    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_SetBaudrate(&MAX310X,baudrate));
}


void test_MAX310X_GetBaudrate_should_ReturnBaudrate(void){
    // Arrange
    uint32_t baudrate = 19200;
    MAX310X.Baud.Baudrate = baudrate;

    // Expect

    // Act
    TEST_ASSERT_EQUAL(baudrate,MAX310X_GetBaudrate(&MAX310X));
}


void test_MAX310X_Send_should_SetTxReg_for_Send(void){
    // Arrange
    char str[] = "THIS IS A TEST\r\n";
    uint8_t len = strlen(str);
    uint8_t val = 0x20;

    // Expect
    mock_max_read(&MAX310X,MAX310XR_ISR,&val);
    mock_max_puts(&MAX310X,MAX310XR_THR,str,len);

    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_Send(&MAX310X,str,len));
}

void mock_max_send(sMAX310X_t *MAX, char *str, uint8_t len, uint8_t *val){
    uint8_t count = 3;
    
    if(0 < (*val & 0x20)){
        while(0<len){
            mock_max_read(&MAX310X,MAX310XR_ISR,val);
            if(128 <= len){
                mock_max_puts(&MAX310X,MAX310XR_THR,str,128);
                len -= 128;
            } else {
                mock_max_puts(&MAX310X,MAX310XR_THR,str,len);
                len=0;
            }
        }
    } else {
        while(count > 0){
            mock_max_read(&MAX310X,MAX310XR_ISR,val);
            count--;
        }


    }
}


void test_MAX310X_Send_should_SetTxReg_for_Send_strings_longer_than_128bytes(void){
    // Arrange
    char str[164] = "THIS IS A MUCH LONGER TEST, IT IS MEANT TO EXCEED 128 CHARACTERS AND WILL TEST MULTIPLE TRANSMITS.  IT IS DIFficult to write such long texts at times\r\n";
    char *pStr = str;
    uint8_t len = strlen(str);
    uint8_t val = 0x20;

    // Expect
    mock_max_send(&MAX310X,str,len,&val);

    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_Send(&MAX310X,str,len));
}


void test_MAX310X_Send_should_Timeout_if_BufferDoesnt_Empty(void){

    // Arrange
    char str[164] = "THIS IS A MUCH LONGER TEST, IT IS MEANT TO EXCEED 128 CHARACTERS AND WILL TEST MULTIPLE TRANSMITS.  IT IS DIFficult to write such long texts at times\r\n";
    char *pStr = str;
    uint8_t len = strlen(str);
    uint8_t val = 0x00;

    // Expect
    mock_max_send(&MAX310X,str,len,&val);

    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_Send(&MAX310X,str,len));

}

void test_MAX310X_Read_should_CheckForEmptyBuffer_and_read(void){
    // Arrange
    char str[128];
    uint8_t len = 0;
    uint8_t mLen = 128;
    uint8_t readval = 0x01;
    
    char retStr[] = "This is a return string that is being tested";
    uint8_t rLen = strlen(retStr);

    // Expect
    mock_max_read(&MAX310X,MAX310XR_LSR,&readval);
    mock_max_read(&MAX310X,MAX310XR_RXFIFOLVL,&rLen);
    mock_max_gets(&MAX310X,MAX310XR_RHR,str,retStr,rLen);
    
    // Act
    TEST_ASSERT_EQUAL(MAX310X_OK, MAX310X_Read(&MAX310X, str, &len, mLen));
    TEST_ASSERT_EQUAL_STRING(retStr,str);
}


void test_MAX310X_Sleep_should_ShutdownModule_PutPinsinLPMode_and_Return(void){
    // Arrange

    // Expect
    BSP_GPIO_SetPin_Expect((sGPIO_t *)&MAX310X.nReset);
    BSP_GPIO_ClearPin_Expect((sGPIO_t *)&MAX310X.cs);
    BSP_SPI_Disable_Expect(&MAX310X.SPI);

    // Act
    // MAX310X_Sleep(&MAX310X);
    MAX310X_Sleep();
    TEST_ASSERT_EQUAL(MAX310X_Power_OFF, powerState);
}

void mock_clear_buffer(sMAX310X_t *MAX, uint8_t *temp){
    mock_max_read(MAX,MAX310XR_MODE2,temp);
    mock_max_write(MAX,MAX310XR_MODE2,*temp | 0x02);   /**< Set the FIFORst Bit */
    mock_max_write(MAX,MAX310XR_MODE2,*temp);           /**< Clear the FIFORst Bit */
}


void test_MAX310X_Wakeup_should_TakeOut_of_Reset_ApplyPins_and_Return(void){
    // Arrange
    powerState = MAX310X_Power_OFF;
    uint8_t mode2reg = 0xC0;
    // Expect
    BSP_SPI_Enable_Expect(&MAX310X.SPI);
    BSP_GPIO_ClearPin_Expect((sGPIO_t *)&MAX310X.nReset);
    BSP_GPIO_ReadInputPin_ExpectAndReturn((sGPIO_t *)&MAX310X.nIRQ,1);
    mock_set_interrupts(&MAX310X);
    mock_clear_buffer(&MAX310X,&mode2reg);

    // Act
    MAX310X_Wakeup(&MAX310X);
    TEST_ASSERT_EQUAL(MAX310X_Power_ON,powerState);
}

void test_MAX310X_ClearBuffer_should_ReadMode2_SetMoode2_and_ClearFifoRstBit(void){
    // Arrange
    uint8_t mode2reg = 0xA0;

    // Expect 
    mock_clear_buffer(&MAX310X, &mode2reg);

    // Act
    MAX310X_ClearBuffer(&MAX310X);
}