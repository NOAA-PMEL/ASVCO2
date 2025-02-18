#include "consoleCommand.h"


//STATIC sUART_t console = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
//    .parity = SYS_CONSOLE_UART_PARITY,
//    .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
//    .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
//    .uartMode = SYS_CONSOLE_UART_MODE
//  },
//.baseAddress = SYS_CONSOLE_UART_PORT,
//.baudrate = SYS_CONSOLE_UART_BAUDRATE,
//};


#define IGNORE_UNUSED_VARIABLE(x)   if(&x==&x) {}

STATIC eCommandResult_t ConsoleCommandHelp(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetVer(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetWho(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetMode(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetCurrent(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetPulse(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetCount(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetBaud1(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetBaud2(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandGetLog(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandReset(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandSet(const char buffer[]);
STATIC eCommandResult_t ConsoleCommandSetSer(const char buffer[]);

STATIC const sConsoleCommandTable_t mConsoleCommandTable[] =
{
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ser",&ConsoleCommandGetWho,HELP("Get the serial number of the device")},
    {"ver", &ConsoleCommandGetVer, HELP("Get the version string")},
    {"mode",&ConsoleCommandGetMode,HELP("Get the current mode")},
    {"current",&ConsoleCommandGetCurrent,HELP("Get the valve current setting")},
    {"pulse",&ConsoleCommandGetPulse,HELP("Get the valve pulse time (ms)")},
    {"count",&ConsoleCommandGetCount,HELP("Get the number of startup counts")},
    {"baud1",&ConsoleCommandGetBaud1,HELP("Get the Command Port Baudrate")},
    {"baud2",&ConsoleCommandGetBaud2, HELP("Get the Console Port Baudrate")},
    {"log",&ConsoleCommandGetLog,HELP("Get the Verbosity of Error Logging")},
    {"set ser ",&ConsoleCommandSet,HELP("Set the Device Serial Number (8 Char ASCII)")},
    // {"set mode", &ConsoleCommandSet, HELP("Set the PCO2 Mode")},
    // {"set current", &ConsoleCommandSet, HELP("Set the Drive Current (0 - 7)")},
    // {"set pulse", &ConsoleCommandSet, HELP("Set the Valve Pulse Time (0 - 1000) (ms)")},
    // {"set baud1", &ConsoleCommandSet, HELP("Set the Command Baud rate")},
    // {"set baud2", &ConsoleCommandSet, HELP("Set the Console Baud rate")},
    // {"set log", &ConsoleCommandSet, HELP("Set the Log Mode (0 - 3)")},



    
    // {"reset", &ConsoleCommandReset, HELP("Factory Reset Device")},
	CONSOLE_COMMAND_TABLE_END // must be LAST
};

size_t command_count = sizeof(mConsoleCommandTable)/sizeof(mConsoleCommandTable[0]);


/** @brief Command HELP
 * 
 *  Writes all available commands to the console.
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
STATIC eCommandResult_t  ConsoleCommandHelp(const char buffer[])
{
    eCommandResult_t result = COMMAND_SUCCESS;
 
    uint32_t tableLength = SIZE_OF(mConsoleCommandTable);
    uint32_t i;

    IGNORE_UNUSED_VARIABLE(buffer);

    for(i=0;i<tableLength;i++){
        CONSOLEIO_SendString(&console,mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_HELP_MAX_LEN > 0
        CONSOLEIO_SendString(&console," : ");
        CONSOLEIO_SendString(&console,mConsoleCommandTable[i].help[0]);
#endif
        CONSOLEIO_SendString(&console,STR_ENDLINE);
    }
    return result;
}

/** @brief Console VERSION
 *
 *  Writes firmware version to the console.
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
STATIC eCommandResult_t ConsoleCommandGetVer(const char buffer[])
{
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    uint8_t cResult = 0;
    CONSOLEIO_SendString(&console,"Version : ");
    CONSOLEIO_SendString(&console,&SysInfo.ver[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);

    return result;
} 

/** @brief Return Name + Serial Number
 *
 *  Writes serial number to the console 
 *
 *  @param buffer Console buffer
 * 
 *  @return result
 */
STATIC eCommandResult_t ConsoleCommandGetWho(const char buffer[])
{
  eCommandResult_t result = COMMAND_SUCCESS;
  IGNORE_UNUSED_VARIABLE(buffer);
  uint8_t cResult = 0;

  /* Send the serial number to the console */
  CONSOLEIO_SendString(&console,"Serial# : ");
  CONSOLEIO_SendString(&console,&SysInfo.ser[0]);
  CONSOLEIO_SendString(&console,STR_ENDLINE);

  return result;
}


/** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
STATIC eCommandResult_t ConsoleCommandGetMode(const char buffer[])
{
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);    
  
    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"PCO2 Mode : ");
    CONSOLEIO_SendString(&console,COMMAND_GetMode());
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;
}

/** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandGetCurrent(const char buffer[])
 {
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    char str[4];

    sprintf(str,"%d",COMMAND_GetCurrent());

    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"Current : ");
    CONSOLEIO_SendString(&console,&str[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;
 }

 /** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
STATIC eCommandResult_t ConsoleCommandGetPulse(const char buffer[])
{
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    char str[16];

    sprintf(str,"%d (ms)",COMMAND_GetPulse());

    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"Pulse : ");
    CONSOLEIO_SendString(&console,&str[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;

}

/** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandGetCount(const char buffer[])
 {
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    char str[16];

    sprintf(str,"%d",COMMAND_GetCount());

    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"Startup Count : ");
    CONSOLEIO_SendString(&console,&str[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;
 }


 /** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandGetBaud1(const char buffer[])
 {
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    char str[8];

    sprintf(str,"%u",SysInfo.commandBaudrate);


    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"Command Baud : ");
    CONSOLEIO_SendString(&console,&str[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;
 }



  /** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandGetBaud2(const char buffer[])
 {
    eCommandResult_t result = COMMAND_SUCCESS;
    IGNORE_UNUSED_VARIABLE(buffer);
    char str[8];

    sprintf(str,"%u",SysInfo.commandBaudrate);


    /* Send the serial number to the console */
    CONSOLEIO_SendString(&console,"Console Baud : ");
    CONSOLEIO_SendString(&console,&str[0]);
    CONSOLEIO_SendString(&console,STR_ENDLINE);
  
    return result;
 }

 /** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandGetLog(const char buffer[])
 {
 
 }


 /** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandSet(const char buffer[])
 {
    char temp1[16];
    char *pTemp1;
    uint16_t i;
    eCommandResult_t result = COMMAND_ERROR;

    /** grab the first parameter passed */
    result = ConsoleReceiveParamString(buffer,1,pTemp1);

    if(COMMAND_SUCCESS == result){
//        for(i=0;i<16;i++){
//            if(' ' == pTemp1)
//            {
//                break;
//            } else {
//                temp1[i] = pTemp1++;
//            }
//        }

    }

    return result;
 }

/** @brief Return 
 *
 *  
 *
 *  @param buffer Console buffer
 *
 *  @return result
 */
 STATIC eCommandResult_t ConsoleCommandSetSer(const char buffer[])
 {

    char temp[16];
    char *pTemp;
    eCommandResult_t result = COMMAND_ERROR;

    result = ConsoleReceiveParamString(buffer,1,pTemp);

    if( COMMAND_SUCCESS == result ){
        CONSOLEIO_SendString(&console,"Serial : ");
        CONSOLEIO_SendString(&console,pTemp);
    }
 
 }


void SendMaddieFunction(void){
  
  char str[] = "Hello Whale!\r\n";
  BSP_UART_puts(&command,&str[0],strlen(str));
  
}

// /** @brief Reset System
//  *
//  *  Console command to reset system to factory defaults
//  *
//  *  @param buffer Console buffer
//  *
//  *  @return result
//  */
// STATIC eCommandResult_t ConsoleCommandReset(const char buffer[]) {
//   eCommandResult_t result = COMMAND_ERROR;
//   IGNORE_UNUSED_VARIABLE(buffer);

//   uint8_t cResult = 0;

//   /* Clear the variables */
// //   memset(SysInfo.ser,0,BSP_SYSTEM_SERIAL_LEN);
  
//   /* Send the serial number to the console */
//   CONSOLEIO_SendString("System Reset");
//   CONSOLEIO_SendString(STR_ENDLINE);
  
//   /* Check return status to ensure string was sent */
//   if(cResult == 0){
//     result = COMMAND_SUCCESS;
//   }
//   return result;  
// }