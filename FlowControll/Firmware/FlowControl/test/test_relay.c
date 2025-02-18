#include "unity.h"
#include "relay.h"
#include "mock_bsp.h"

extern sRelay_t Relay;

void setUp(void)
{
}

void tearDown(void)
{
}



void test_RELAY_Init_should_Initialize_and_TurnOutput_OFF(void){
    Relay.Toggle.port = 1;
    Relay.Toggle.pin = 1;

    BSP_GPIO_Init_Expect(&Relay.Toggle);
    
    RELAY_Init();
    TEST_ASSERT_EQUAL(1,Relay.Toggle.port);
    TEST_ASSERT_EQUAL(1,Relay.Toggle.pin);

}


void test_RELAY_On_should_Turnpin_ON_and_ReturnValid(void){
    Relay.Toggle = (sGPIO_t) {.port=3,.pin=2};
    sGPIO_t Test = {3,30};

    Relay.Toggle.port = 3;
    Relay.Toggle.pin = 2;

    BSP_GPIO_SetPin_Expect(&Relay.Toggle);
    RELAY_On();

}



void test_RELAY_Off_should_Turnpin_OFF_and_ReturnValid(void){
    Relay.Toggle = (sGPIO_t) {.port=3,.pin=2};


    BSP_GPIO_ClearPin_Expect(&Relay.Toggle);
    RELAY_Off();
}

