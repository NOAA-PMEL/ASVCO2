#include "unity.h"
#include "AM0805.h"
#include "mock_bsp.h"
extern sAM0805_t AM0805;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_AM0805_Init_should_ReturnValid_if_ValidRequested(void){
    BSP_I2C_Init_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,AM0805_Init());
}


void test_AM0805_Init_should_Fail_if_I2CFails(void){
    BSP_I2C_Init_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,AM0805_Init());

    BSP_I2C_Init_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,AM0805_Init());

    BSP_I2C_Init_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,AM0805_Init());

    BSP_I2C_Init_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,AM0805_Init());

    BSP_I2C_Init_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,AM0805_Init());
}


void test_readreg_should_ReturnValid_if_ValidResult(void){
    uint8_t reg;
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,readreg(OSC_CONTROL_REG,&reg));
}

void test_readreg_should_Fail_if_InvalidResult(void){
    uint8_t reg;
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,readreg(OSC_CONTROL_REG,&reg));
}

void test_writereg_should_ReturnValid_if_ValidResult(void){
    uint8_t reg;
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,writereg(OSC_CONTROL_REG,reg));
}

void test_writereg_should_Fail_if_InvalidResult(void){
    uint8_t reg;

    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,writereg(OSC_CONTROL_REG,reg));
}

void test_clrreg_should_ReturnValid_if_ValidResult(void){
    uint8_t reg;
    uint8_t mask;

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,clrreg(reg,mask));
}

void test_clrreg_should_Fail_if_InvalidResult(void){
    uint8_t reg;
    uint8_t mask;

    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_FAIL,clrreg(reg,mask));

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,clrreg(reg,mask));

    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,clrreg(reg,mask));
}

// void test_clrreg_should_MaskCorrect_for_ValidEntries(void){

//     uint8_t reg = 0x01;
//     uint8_t temp = 0xff;
//     uint8_t mask = 0x02;

//     // BSP_I2C_getc_ExpectWithArrayAndReturn(BSP_I2C_AMBIQ,reg,&temp,1,BSP_OK);
//     // BSP_I2C_getc_ExpectWithArrayAndReturn(BSP_I2C_AMBIQ,reg,&temp,0,BSP_OK);
//     BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
//     // BSP_I2C_getc_ExpectWithArray(BSP_I2C_AMBIQ,reg,&temp);
//     BSP_I2C_putc_ExpectAndReturn(BSP_I2C_AMBIQ,reg,0xFD, BSP_OK);
//     TEST_ASSERT_EQUAL(AM0805_OK,clrreg(reg,mask));

// }

void test_am_osc_sel_should_ReturnValid_for_ValidResult(void){

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,am_osc_sel(AM0805_OSC_32768Hz_NoSwitch));
}


void test_am_osc_sel_should_Fail_for_InvalidResult(void){
    
        BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
        BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
        TEST_ASSERT_EQUAL(AM0805_FAIL,am_osc_sel(AM0805_OSC_32768Hz_NoSwitch));
   
        BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
        TEST_ASSERT_EQUAL(AM0805_FAIL,am_osc_sel(AM0805_OSC_32768Hz_NoSwitch));
   
}


void test_am_get_time_should_ReturnValid_for_ValidResult(void){
    sAM0805_t p;

    BSP_I2C_gets_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(AM0805_OK,am_get_time(&p));

}

void test_am_get_time_should_Fail_for_InvalidResult(void){
    sAM0805_t p;

    BSP_I2C_gets_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,am_get_time(&p));

    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(AM0805_FAIL,am_get_time(&p));

}


void test_AM0805_SetCalFreq_should_ReturnValid_for_ValidRequest(void){

    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    TEST_ASSERT_EQUAL(BSP_OK,AM0805_SetCalFreq(AM0805_nIRQ));

}

void test_AM0805_SetCalFreq_should_Fail_for_InvalidRequest(void){
    
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));

    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));

    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));

    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));

    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));


    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));

    BSP_I2C_putc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_SetCalFreq(AM0805_nIRQ));
    
}


void test_AM0805_GetTime_should_ReturnValid_for_ValidRequests(void){
    
    BSP_I2C_gets_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);

    TEST_ASSERT_EQUAL(BSP_OK,AM0805_GetTime());
}

void test_AM0805_GetTime_should_Fail_for_InvalidRequest(void){

    BSP_I2C_gets_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_GetTime());

    BSP_I2C_gets_IgnoreAndReturn(BSP_OK);
    BSP_I2C_getc_IgnoreAndReturn(BSP_FAIL);
    TEST_ASSERT_EQUAL(BSP_FAIL,AM0805_GetTime());
}


// void test_AM0805_GetTime_should_ReturnValid_for_ValidStringInputs(void){
//     uint8_t str[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    
//     void eBSPStatus_t BSP_I2C_gets(BSPI2CChan_t Chan, uint8_t Addr, uint8_t NumBytes, uint8_t *Data)
//     {
//         strcpy(Data,str);
//     }

// }


void test_am_set_alarm_on_zeros_should_ReturnValid_for_ValidInput(void){

    /******** PIN 1 *********/
    /** clrreg */
    /** clrreg */
    /** clrreg */
    /** readreg */
    /** setreg */

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);

    BSP_I2C_getc_IgnoreAndReturn(BSP_OK);
    BSP_I2C_putc_IgnoreAndReturn(BSP_OK);

    TEST_ASSERT_EQUAL(AM0805_OK,am_set_alarm_on_zeros(AM0805_Alarm_1Hour,1,1));
}