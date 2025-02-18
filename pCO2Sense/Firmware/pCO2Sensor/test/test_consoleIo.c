#include "unity.h"
#include "consoleIo.h"
#include "mock_bsp.h"
#include "mock_errors.h"
#include "mock_buffer_c.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_CONSOLEIO_Init_should_Init_command_and_console_and_return_valid(void){
    /* Arrange */
    sUART_t Console;
    /* Expect */
    BSP_UART_Init_ExpectAndReturn(&Console,BSP_OK);
    // BSP_UART_Init_ExpectAndReturn(&Command,BSP_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLEIO_OK, CONSOLEIO_Init(&Console));

}

void test_CONSOLEIO_Init_should_Fail_if_Console_doesnt_Init(void){
    /* Arrange */
    sUART_t Console;

    /* Expect */
    BSP_UART_Init_ExpectAndReturn(&Console,BSP_FAIL);
    Error_Expect(ERROR_MSG_0010,LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLEIO_FAIL, CONSOLEIO_Init(&Console));
}

void test_CONSOLEIO_Receive_should_Receive_and_ReturnValue(void){
    /* Arrange */
    char buf[32];
    uint16_t bufLen = 32;
    uint16_t readLen = 0;
    char returnBuf[16] = "THIS IS A TEST\r\n";
    uint16_t retLen = strlen(returnBuf);
    eConsolePort_t port = PORT_CONSOLE;
    sCircularBufferC_t cbuf;

    /* Expect */
    BufferC_gets_ExpectAndReturn(&cbuf,buf,bufLen,BUFFER_C_OK);
    BufferC_gets_IgnoreArg_str();
    BufferC_gets_ReturnThruPtr_str(returnBuf);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLEIO_OK, CONSOLEIO_Receive(&cbuf,port,buf,bufLen,&readLen));


}

void test_CONSOLEIO_Receive_should_Fail_if_Command_is_Empty(void){
    /* Arrange */
    char buf[32];
    uint16_t bufLen = 32;
    uint16_t readLen = 0;
    char returnBuf[16] = "THIS IS A TEST\r\n";
    uint16_t retLen = strlen(returnBuf);
    eConsolePort_t port = PORT_COMMAND;
    sCircularBufferC_t cbuf;

    /* Expect */
    BufferC_gets_ExpectAndReturn(&cbuf,buf,bufLen,BUFFER_C_EMPTY);
    BufferC_gets_IgnoreArg_str();
    Log_Expect("Command Buffer Warning", LOG_Priority_Low);


    /* Act */
    TEST_ASSERT_EQUAL(CONSOLEIO_FAIL, CONSOLEIO_Receive(&cbuf,port,buf,bufLen,&readLen));

}

void test_CONSOLEIO_Send_should_Send_and_ReturnValid(void){
    /* Arrange */
    char buf[32] = "Do the D.A.N.C.E 1 2 3 4 5";
    uint16_t bufLen = strlen(buf);
    uint16_t sentLen = 0;
    sUART_t Console;

    /* Expect */
    BSP_UART_puts_Expect(&Console,buf,bufLen);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLEIO_OK, CONSOLEIO_Send(&Console,  buf, bufLen, &sentLen));
    TEST_ASSERT_EQUAL(bufLen,sentLen);
}