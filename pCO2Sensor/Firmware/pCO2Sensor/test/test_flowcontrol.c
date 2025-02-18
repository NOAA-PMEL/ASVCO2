#include "unity.h"
#include "flowcontrol.h"
#include "mock_errors.h"
#include "mock_MAX310x.h"
#include "mock_bsp.h"

void setUp(void)
{
}

void tearDown(void)
{
}

mock_power_on(sGPIO_t *p){
    BSP_GPIO_SetPin_Expect(p);
}

mock_power_off(sGPIO_t *p){
    BSP_GPIO_ClearPin_Expect(p);
}

void mock_flow_startup(sFLOW_t *f){
    BSP_GPIO_Init_Expect(&f->Power);
    mock_power_on(&f->Power);
    MAX310X_Wakeup_Expect(&f->UART);
    MAX310X_SetBaudrate_ExpectAndReturn(&f->UART,9600,MAX310X_OK);
}

void test_FLOW_Startup_should_Init_and_returnValid(void){
    // Arrange
    
    // Expect
    BSP_GPIO_Init_Expect(&Flow.Power);
    mock_power_on(&Flow.Power);
    MAX310X_Wakeup_Expect(&Flow.UART);
    MAX310X_SetBaudrate_ExpectAndReturn(&Flow.UART,9600,MAX310X_OK);

    // Act
    TEST_ASSERT_EQUAL(FLOW_OK,FLOW_Startup(&Flow));
}

void test_FLOW_Startup_should_Fail_forInvalidBaudSetup(void){
    // Arrange

    // Expect
    BSP_GPIO_Init_Expect(&Flow.Power);
    mock_power_on(&Flow.Power);
    MAX310X_Wakeup_Expect(&Flow.UART);
    MAX310X_SetBaudrate_ExpectAndReturn(&Flow.UART,9600,MAX310X_FAIL);
    Error_Expect(ERROR_MSG_4000,LOG_Priority_Med);

    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_Startup(&Flow));
}

void test_FLOW_Init_should_InitUart_and_PowerPins(void){
    // Arrange

    // Expect
    MAX310X_Init_ExpectAndReturn(&Flow.UART,MAX310X_OK);
    mock_flow_startup(&Flow);

    // Act
    TEST_ASSERT_EQUAL(FLOW_OK,FLOW_Init());
}

void test_FLOW_Init_should_Fail_for_BadStartup(void){
    // Arrange

    // Expect
    MAX310X_Init_ExpectAndReturn(&Flow.UART,MAX310X_OK);
    BSP_GPIO_Init_Expect(&Flow.Power);
    mock_power_on(&Flow.Power);
    MAX310X_Wakeup_Expect(&Flow.UART);
    MAX310X_SetBaudrate_ExpectAndReturn(&Flow.UART,9600,MAX310X_FAIL);
    Error_Expect(ERROR_MSG_4000,LOG_Priority_Med);
    Error_Expect(ERROR_MSG_4001,LOG_Priority_High);
    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_Init());
}


void test_FLOW_Open_should_RunStartup_and_ReturnValid(void){
    // Arrange

    // Expect

    // Act

}


void test_FLOW_Sleep_should_SleepMAX310X_and_PowerDownFlowController(void){
    // Arrange

    // Expect
    MAX310X_Sleep_Expect();
    mock_power_off(&Flow.Power);

    // Act
    FLOW_Sleep();
}

void test_FLOW_Wakeup_should_PowerFlowController_and_WakeupMAX310X(void){
    // Arrange

    // Expect 
    mock_power_on(&Flow.Power);
    MAX310X_Wakeup_Expect(&Flow.UART);

    // Act
    FLOW_Wakeup();
}

void test_FLOW_SetMode_should_AcceptValidMode_set_FlowController_and_returnValid(void){
    // Arrange
    eFLOWState_t state = FLOW_STATE_EQUIL_PUMP_OFF;
    char mode[] = "MODE=EQUIL_OFF\r\n";
    uint8_t len = strlen(mode);


    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,mode,len,MAX310X_OK);
    _delay_ms_Expect(100);
    MAX310X_Read_IgnoreAndReturn(UART_ACK);

    // Act
    TEST_ASSERT_EQUAL(FLOW_OK,FLOW_SetMode(state));
}

void test_FLOW_SetMode_should_Fail_with_Error_for_BadSend(void){
     // Arrange
    eFLOWState_t state = FLOW_STATE_EQUIL_PUMP_OFF;
    char mode[] = "MODE=EQUIL_OFF\r\n";
    uint8_t len = strlen(mode);


    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,mode,len,MAX310X_FAIL);
    Error_Expect(ERROR_MSG_4011,LOG_Priority_High);

    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_SetMode(state));
}

void test_FLOW_SetMode_should_Fail_with_Error_for_BadResponse(void){
     // Arrange
    eFLOWState_t state = FLOW_STATE_EQUIL_PUMP_OFF;
    char mode[] = "MODE=EQUIL_OFF\r\n";
    uint8_t len = strlen(mode);


    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,mode,len,MAX310X_OK);
    _delay_ms_Expect(100);
    MAX310X_Read_IgnoreAndReturn(UART_NACK);
    Error_Expect(ERROR_MSG_4012,LOG_Priority_Med);

    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_SetMode(state));
}

void test_FLOW_GetMode_should_ReportMode_and_ReturnValid(void){
    // Arrange
    char str[] = "MODE";
    uint8_t len = 4;

    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,str,strlen(str),MAX310X_OK);
    MAX310X_Read_ExpectAndReturn(&Flow.UART,str,&len,32,MAX310X_OK);
    MAX310X_Read_IgnoreArg_str();
    MAX310X_Read_IgnoreArg_len();
    MAX310X_Read_ReturnArrayThruPtr_str(str,strlen(str));

    // Act
    TEST_ASSERT_EQUAL(FLOW_OK,FLOW_GetMode());
}

void test_FLOW_GetMode_should_FailWithError_if_MsgDoesNotSend(void){
    // Arrange
    char str[] = "MODE";
    uint8_t len = 4;

    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,str,strlen(str),MAX310X_FAIL);
    Error_Expect(ERROR_MSG_4013,LOG_Priority_Med);
    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_GetMode());
}

void test_FLOW_GetMode_should_FailWithError_if_NoData_is_read(void){
    // Arrange
    char str[] = "MODE";
    uint8_t len = 4;
    uint8_t i;

    // Expect
    MAX310X_Send_ExpectAndReturn(&Flow.UART,str,strlen(str),MAX310X_OK);
    for(i=0;i<5;i++){
        MAX310X_Read_IgnoreAndReturn(MAX310X_FAIL);
    }
    
    Error_Expect(ERROR_MSG_4014,LOG_Priority_High);
    // Act
    TEST_ASSERT_EQUAL(FLOW_FAIL,FLOW_GetMode());
}