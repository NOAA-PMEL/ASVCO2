#include "unity.h"
#include "msp430fr5994.h"
#include "uart.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_UART_Init_should_ReturnValid_for_ValidEntries(void)
{
    sUART_t UART;

    TEST_ASSERT_EQUAL(UART_OK,UART_Init(&UART,UART_A0,UART_9600,1,4000000));
    TEST_ASSERT_EQUAL(UART_OK,UART_Init(&UART,UART_A0,UART_115200,1,4000000));

}

void test_UART_Init_should_Fail_for_InvalidBaud(void){
    sUART_t UART;

    TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(&UART,UART_A3,7500,1,4000000));
    TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(&UART,UART_A3,115201,1,4000000));

}


void test_UART_putc_should_ReturnValid_for_ValidEntried(void)
{
    sUART_t UART;
    char c = 't';
    TEST_ASSERT_EQUAL(UART_OK,UART_Init(&UART,UART_A0,UART_9600,1,4000000));
    TEST_ASSERT_EQUAL(UART_OK,UART_putc(c,&UART));
    TEST_ASSERT_EQUAL(c,UCA0TXBUF);

}

void test_UART_putc_should_Fail_for_InvalidEntries(void)
{
    sUART_t UART;
    char c = 't';
    TEST_ASSERT_EQUAL(UART_OK,UART_Init(&UART,UART_A0,UART_9600,1,4000000));
    UART.Port = 5;
    TEST_ASSERT_EQUAL(UART_FAIL,UART_putc(c,&UART));
    
    // TEST_ASSERT_EQUAL('f',UCA0TXBUF);
}