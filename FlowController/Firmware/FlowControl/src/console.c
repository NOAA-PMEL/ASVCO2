#include "console.h"



void CONSOLE_Init(void){
  
  const char prompt[] = "> ";
  
  CONSOLEIO_Init(&console);
  CONSOLEIO_Send(&console,(char*) &prompt[0],2);
  SendMaddieFunction();
  // 
  return;
}


void CONSOLE_Process(void) {
  
  
}
  
  