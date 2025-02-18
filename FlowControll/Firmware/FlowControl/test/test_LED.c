#include "unity.h"
#include "LED.h"
#include "mock_bsp.h"
extern struct sLED grnLED;
extern struct sLED redLED;

sGPIO_t green = {.port=GPIO_PORT_P8,.pin=GPIO_PIN2, .mode=0,
    .configured=0, .inputValue=0,.pullups=1, .interrupt = 0,
    .interruptEdge=0, .inverted=0};
sGPIO_t red =  {.port=GPIO_PORT_P8,.pin=GPIO_PIN3, .mode=0,
  .configured=0, .inputValue=0,.pullups=1, .interrupt = 0,
  .interruptEdge=0, .inverted=0};

void setUp(void)
{
    
}

void tearDown(void)
{
}



void test_LED_Init_should_SetBothLEDs_to_On_Delay_Off(void){


    BSP_GPIO_Init_Expect(&red);;
    BSP_GPIO_Init_Expect(&green);

    LED_Init();
}


void test_LED_State_Off_should_ClearGpioPin_and_State(void){

    /** Configure the LED Structs */
    grnLED.Out.Status = LED_ON;
    redLED.Out.Status = LED_ON;

    BSP_GPIO_ClearPin_Expect(&green);
    LED_State_Off(&grnLED);
    TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_OFF_STATE,grnLED.State);

    BSP_GPIO_ClearPin_Expect(&red);
    LED_State_Off(&redLED);
    TEST_ASSERT_EQUAL(LED_OFF,redLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_OFF,redLED.State);

}


void test_LED_State_On_should_SetGpioPin_and_State(void){
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};

    /** Configure the LED Structs */
    grnLED.Out.Status = LED_OFF;
    redLED.Out.Status = LED_OFF;

    BSP_GPIO_SetPin_Expect(&green);
    LED_State_On(&grnLED);
    TEST_ASSERT_EQUAL(LED_ON,grnLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_STEADY,grnLED.State);

    BSP_GPIO_SetPin_Expect(&red);
    LED_State_On(&redLED);
    TEST_ASSERT_EQUAL(LED_ON,redLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_STEADY,redLED.State);
}


void test_LED_State_Flash_1Hz_should_ClearGpioPin_and_SetState(void){
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};

    /** Configure the LED Structs */
    grnLED.Out.Status = LED_ON;
    redLED.Out.Status = LED_ON;

    BSP_GPIO_ClearPin_Expect(&green);
    LED_State_Flash_1Hz(&grnLED);
    TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_1HZ,grnLED.State);
    TEST_ASSERT_EQUAL(0x0F0F0F0F,grnLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,grnLED.Out.idx);
    TEST_ASSERT_EQUAL(32,grnLED.Out.max);

    BSP_GPIO_ClearPin_Expect(&red);
    LED_State_Flash_1Hz(&redLED);
    TEST_ASSERT_EQUAL(LED_OFF,redLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_1HZ,redLED.State);
    TEST_ASSERT_EQUAL(0x0F0F0F0F,redLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,redLED.Out.idx);
    TEST_ASSERT_EQUAL(32,redLED.Out.max);
}

void test_LED_State_Flash_4Hz_should_ClearGpioPin_and_SetState(void){
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};

    /** Configure the LED Structs */
    grnLED.Out.Status = LED_ON;
    redLED.Out.Status = LED_ON;

    BSP_GPIO_ClearPin_Expect(&green);
    LED_State_Flash_4Hz(&grnLED);
    TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_4HZ,grnLED.State);
    TEST_ASSERT_EQUAL(0x55555555,grnLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,grnLED.Out.idx);
    TEST_ASSERT_EQUAL(32,grnLED.Out.max);

    BSP_GPIO_ClearPin_Expect(&red);
    LED_State_Flash_4Hz(&redLED);
    TEST_ASSERT_EQUAL(LED_OFF,redLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_4HZ,redLED.State);
    TEST_ASSERT_EQUAL(0x55555555,redLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,redLED.Out.idx);
    TEST_ASSERT_EQUAL(32,redLED.Out.max);
}

void test_LED_State_Flash_SOS_should_ClearGpioPin_and_SetState(void){
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};

    /** Configure the LED Structs */
    grnLED.Out.Status = LED_ON;
    redLED.Out.Status = LED_ON;

    BSP_GPIO_ClearPin_Expect(&green);
    LED_State_Flash_SOS(&grnLED);
    TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,grnLED.State);
    TEST_ASSERT_EQUAL(0x0055DDD5,grnLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,grnLED.Out.idx);
    TEST_ASSERT_EQUAL(32,grnLED.Out.max);

    BSP_GPIO_ClearPin_Expect(&red);
    LED_State_Flash_SOS(&redLED);
    TEST_ASSERT_EQUAL(LED_OFF,redLED.Out.Status);
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,redLED.State);
    TEST_ASSERT_EQUAL(0x0055DDD5,redLED.Out.Pattern);
    TEST_ASSERT_EQUAL(0,redLED.Out.idx);
    TEST_ASSERT_EQUAL(32,redLED.Out.max);
}



void test_LED_Process_SimpleCases_should_SetCorrectBit_depending_on_Index(void){
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};

    /** Test State = OFF */
    grnLED.Out.Status = LED_ON;
    BSP_GPIO_ClearPin_Expect(&green);//(&red);
    LED_State_Off(&grnLED);

    printf("Test OFF = ");
    printf("(0x%08x) ",grnLED.Out.Pattern);
        
    uint8_t i = 0;
    for(i=0;i<32;i++){
        BSP_GPIO_ClearPin_Expect(&green);(&red);
    }

    for(i=0;i<32;i++){
        printf("%d",(grnLED.Out.Pattern && (0x00000001u << grnLED.Out.idx)));
        LED_Process(&grnLED);
        TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);
    }


    /** Test State = ON */
    redLED.Out.Status = LED_OFF;
    BSP_GPIO_SetPin_Expect(&red);
    LED_State_On(&redLED);
    printf("\nTest ON =  ");
    printf("(0x%08x) ",redLED.Out.Pattern);

    for(i=0;i<32;i++){
        BSP_GPIO_SetPin_Expect(&red);
    }

    for(i=0;i<32;i++){
        printf("%d",(redLED.Out.Pattern && (0x00000001u << redLED.Out.idx)));
        LED_Process(&redLED);
        TEST_ASSERT_EQUAL(LED_ON,redLED.Out.Status);
    }
}



void test_LED_Process_for_4HzCase_should_SetPins_Correctly(void){

    uint8_t i;
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};


    /** Set the Test State */
    grnLED.State = LED_OFF_STATE;
    grnLED.Out.Status = LED_OFF;
    LED_State_Flash_4Hz(&grnLED);

    TEST_ASSERT_EQUAL(LED_ON_FLASHING_4HZ,grnLED.State);
    TEST_ASSERT_EQUAL(0x55555555,grnLED.Out.Pattern);

    printf("Test 4Hz = (0x%08x) ",grnLED.Out.Pattern);
    /** Set up the Mocks */
    for(i=0;i<32;i++){
        if(((0x55555555u>>i)&0x00000001u) == 0){
            printf("0");
            BSP_GPIO_ClearPin_Expect(&green);(&red);
        } else {
            BSP_GPIO_SetPin_Expect(&green);
            printf("1");
        }

        
    }

    for(i=0;i<32;i++){
        // LED_Process(&grnLED);
        grnLED.Run(&grnLED);
    }


}

void test_LED_Process_for_1HzCase_should_SetPins_Correctly(void){
    
    uint8_t i;
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};


    /** Set the Test State */
    grnLED.State = LED_OFF_STATE;
    grnLED.Out.Status = LED_OFF;
    LED_State_Flash_1Hz(&grnLED);

    TEST_ASSERT_EQUAL(LED_ON_FLASHING_1HZ,grnLED.State);
    TEST_ASSERT_EQUAL(0x0F0F0F0F,grnLED.Out.Pattern);

    printf("Test 1Hz = (0x%08x) ",grnLED.Out.Pattern);
    /** Set up the Mocks */
    for(i=0;i<32;i++){
        if(((0x0F0F0F0F>>i)&0x00000001u) == 0){
            printf("0");
            BSP_GPIO_ClearPin_Expect(&green);//(&red);
        } else {
            BSP_GPIO_SetPin_Expect(&green);
            printf("1");
        }

        
    }

    for(i=0;i<32;i++){
        // LED_Process(&grnLED);
        grnLED.Run(&grnLED);
    }
    
    
}


void test_LED_Process_for_SOS_Case_should_SetPins_Correctly(void){
    
    uint8_t i;
    // sGPIO_t green = {.port=8,.pin=2};
    // sGPIO_t red = {.port=8,.pin=3};


    /** Set the Test State */
    grnLED.State = LED_OFF_STATE;
    grnLED.Out.Status = LED_OFF;
    LED_State_Flash_SOS(&grnLED);

    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,grnLED.State);
    TEST_ASSERT_EQUAL(0x0055DDD5,grnLED.Out.Pattern);

    printf("Test SOS = (0x%08x) ",grnLED.Out.Pattern);
    /** Set up the Mocks */
    for(i=0;i<32;i++){
        if(((0x0055DDD5>>i)&0x00000001u) == 0){
            printf("0");
            BSP_GPIO_ClearPin_Expect(&green);
        } else {
            BSP_GPIO_SetPin_Expect(&green);
            printf("1");
        }

        
    }

    for(i=0;i<32;i++){
        // LED_Process(&grnLED);
        grnLED.Run(&grnLED);
    }
    
    
}


void test_LED_Command_should_ReturnValid_for_ValidCommands(void){

    
    char command1[] = "GREEN OFF";
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_Command(&command1[0]));
    TEST_ASSERT_EQUAL(LED_OFF_STATE,grnLED.State);
    TEST_ASSERT_EQUAL(LED_OFF,grnLED.Out.Status);

    char command2[] = "RED ON";
    BSP_GPIO_SetPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_Command(&command2[0]));
    TEST_ASSERT_EQUAL(LED_ON_STEADY,redLED.State);
    TEST_ASSERT_EQUAL(LED_ON,redLED.Out.Status);

    char command3[] = "GREEN FLASH1";
    grnLED.Out.Status = LED_ON;
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_Command(&command3[0]));
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_1HZ,grnLED.State);
    TEST_ASSERT_EQUAL(LED_ON,grnLED.Out.Status);

    char command4[] = "GREEN FLASH4";
    grnLED.Out.Status = LED_ON;
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_Command(&command4[0]));
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_4HZ,grnLED.State);
    TEST_ASSERT_EQUAL(LED_ON,grnLED.Out.Status);

    char command5[] = "GREEN SOS";
    grnLED.Out.Status = LED_ON;
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_Command(&command5[0]));
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,grnLED.State);
    TEST_ASSERT_EQUAL(LED_ON,grnLED.Out.Status);
   
}


void test_LED_Command_should_ReturnInvalid_for_InvalidCommands(void){

    char command1[] = "GREEN";
    TEST_ASSERT_EQUAL(LED_FAIL,LED_Command(&command1[0]));


    char command2[] = "GREEN 2";
    TEST_ASSERT_EQUAL(LED_FAIL,LED_Command(&command2[0]));
    
    char command3[] = "GREEN OFF ON";
    TEST_ASSERT_EQUAL(LED_FAIL,LED_Command(&command3[0]));

    char command4[] = "GREEN REPEAT";
    TEST_ASSERT_EQUAL(LED_FAIL,LED_Command(&command4[0]));

    char command5[] = "GREEN FLASH3";
    TEST_ASSERT_EQUAL(LED_FAIL,LED_Command(&command5[0]));
}


void test_LED_SetState_should_ReturnValid_if_Value_is_Valid(void){

    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&grnLED,LED_OFF_STATE));

    BSP_GPIO_SetPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&redLED,LED_ON_STEADY));

    BSP_GPIO_SetPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&grnLED,LED_ON_FLASHING_1HZ));

    BSP_GPIO_SetPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&redLED,LED_ON_FLASHING_4HZ));

    BSP_GPIO_SetPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&grnLED,LED_ON_FLASHING_SOS));
}


void test_LED_SetState_should_ReturnInvalid_if_Value_is_Invalid(void){

    /** Set a valid state */
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&grnLED,LED_OFF_STATE));

    /** Set an invalid state */
    TEST_ASSERT_EQUAL(LED_FAIL,LED_SetState(&grnLED,6));
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,grnLED.State);

    /** Set a valid state */
    BSP_GPIO_ClearPin_Ignore();
    TEST_ASSERT_EQUAL(LED_OK,LED_SetState(&redLED,LED_OFF_STATE));

    TEST_ASSERT_EQUAL(LED_FAIL,LED_SetState(&redLED,255));
    TEST_ASSERT_EQUAL(LED_ON_FLASHING_SOS,redLED.State);
}