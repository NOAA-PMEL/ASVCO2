#include "unity.h"
#include "console.h"
#include "mock_consoleio.h"

void setUp(void)
{

}

void tearDown(void)
{
}


void test_console_ProcessState_should_Interpret_valid_Messages(void){
  // 
  char sendstr[] = "> ";
  
  CONSOLEIO_Init_ExpectAndReturn(CONSOLEIO_OK);
  CONSOLEIO_Send_ExpectAndReturn("> ",2,CONSOLEIO_OK);
  
  CONSOLE_Init();
  
}