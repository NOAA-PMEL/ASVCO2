#include <string.h>

#include "unity.h"
#include "consoleio.h"
#include "buffer_c.h"
#include "mock_bsp.h"


#define SYS_CONSOLEIO_PORT    (1)

sUART_t console = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
                              .parity = SYS_CONSOLE_UART_PARITY,
                              .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
                              .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
                              .uartMode = SYS_CONSOLE_UART_MODE
                            },
                    .baseAddress = SYS_CONSOLE_UART_PORT,
                    .baudrate = SYS_CONSOLE_UART_BAUDRATE,
                  };
sUART_t command = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
                              .parity = SYS_CONSOLE_UART_PARITY,
                              .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
                              .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
                              .uartMode = SYS_CONSOLE_UART_MODE
                              },
                    .baseAddress = SYS_COMMAND_UART_PORT,
                    .baudrate = SYS_COMMAND_UART_BAUDRATE,
                  };

void setUp(void)
{
}

void tearDown(void)
{
}


void test_consoleio_Init_should_ReturnValid(void)
{
    BSP_UART_Init_Expect(&console);
    
    CONSOLEIO_Init(&console);
}


void test_consoleio_Send_should_SendString(void)
{
  char str[] = "The Moon is Disgusting, it's made of cheese\r\n";
  uint16_t strLen = strlen(str);
  
  // when
  BSP_UART_puts_Expect(&console, &str[0],strLen);
  
  // then
  CONSOLEIO_Send(&console,&str[0],strLen);
  
}

void test_consoleio_Read_should_Fail_ifBadUart(void){
  char str[] = "SEND THIS STRING\r\n";
  uint16_t strLen = strlen(str);
  uint8_t i;

  char rtnstr[SYS_UART_BUFFER_LEN];
  uint16_t readLen;

  BufferC_puts(&console.buf,&str[0],strlen(str));
  
  BSP_UART_gets_ExpectAndReturn(&console,&rtnstr[0],strLen);
  
  // then
  TEST_ASSERT_EQUAL( strLen,CONSOLEIO_Read(&console, &rtnstr[0]) );
  

}


void test_consoleio_SendString_should_SendString_of_ValidLength(void)
{
  char str[] = "The Moon is Disgusting, it's made of cheese\r\n";
  uint16_t strLen = strlen(str);
  
  // when
  BSP_UART_puts_Expect(&console, &str[0],strLen);
  
  // then
  CONSOLEIO_SendString(&console,&str[0]);
  
}