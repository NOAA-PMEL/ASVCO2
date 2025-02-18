#include "unity.h"
#include "max250.h"
#include "mock_bsp.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_MAX250_SetCal_should_copy_Cal_toStructure(void){
    float cal = 2.0;
    MAX250.cal = 0.0;
    MAX250_SetCal(cal);
    TEST_ASSERT_EQUAL(cal,MAX250.cal);
} 

void test_MAX250_Calculate_should_CalculateO2_and_ReturnValue(void){
    float v = 0.0115;
    float result=0.0;
    float expected = 12.0175;
    MAX250.cal = 2.0;
    MAX250_CalculateO2(&v,&result);
    TEST_ASSERT_EQUAL_FLOAT(expected,result);
}




void test_MAX250_Read_should_return_ValidValue(void){
    uint8_t i;
    float voltage = 0.011;
    float expected = 11.495;
    for(i=0;i<10;i++){
        BSP_ANALOG_ReadVoltage_ExpectAndReturn(&MAX250.Analog,voltage);
    }
    TEST_ASSERT_EQUAL_FLOAT(expected,MAX250_Read());

}



void test_MAX250_PowerON_should_SetRelayPin(void){
    /* Arrange */

    /* Expect */
    BSP_GPIO_SetPin_Expect(&MAX250.Power);

    /* Act */
    MAX250_PowerON();

}

void test_MAX250_PowerOFF_should_SetRelayPin(void){
    /* Arrange */

    /* Expect */
    BSP_GPIO_ClearPin_Expect(&MAX250.Power);

    /* Act */
    MAX250_PowerOFF();

}

void test_MAX250_Init_should_InitPorts(void){
    /* Arrange */

    /* Expect */
    BSP_GPIO_Init_Expect(&MAX250.Power);
    BSP_GPIO_SetPin_Expect(&MAX250.Power);
    BSP_GPIO_Init_Expect(&MAX250.Pin);
    BSP_ANALOG_Init_ExpectAndReturn(&MAX250.Analog,MAX250_OK);

    /* Act */
    MAX250_Init();
}