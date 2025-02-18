#include "consoleio.h"

sUART_t console = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
    .parity = SYS_CONSOLE_UART_PARITY,
    .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
    .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
    .uartMode = SYS_CONSOLE_UART_MODE
  },
.baseAddress = SYS_CONSOLE_UART_PORT,
.baudrate = SYS_CONSOLE_UART_BAUDRATE,
};


void CONSOLEIO_Init(sUART_t *uart){

  BSP_UART_Init(uart);

}


void CONSOLEIO_Send(sUART_t *uart,char *str,uint16_t strLen){
  
  BSP_UART_puts(uart,str,strLen);
  
  return;
}

void CONSOLEIO_SendString(sUART_t *uart,char *str){
  uint16_t len = strlen(str);

  CONSOLEIO_Send(uart,str,len);

}
  

uint16_t CONSOLEIO_Read(sUART_t *uart,char *str){
  
  /* Retreive the string from the UART Port buffer */
  return BSP_UART_gets(uart,str);

}