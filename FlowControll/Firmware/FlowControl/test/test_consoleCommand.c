#include "unity.h"
#include "consoleCommand.h"
#include "mock_consoleio.h"
#include "mock_bsp.h"
#include "mock_command.h"
#include "mock_console.h"
#include "version.h"


#define SERIALNUM   ("SERNUM01")

char mReceiveBuffer[32];

sUART_t testconsole = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
    .parity = SYS_CONSOLE_UART_PARITY,
    .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
    .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
    .uartMode = SYS_CONSOLE_UART_MODE
  },
.baseAddress = SYS_CONSOLE_UART_PORT,
.baudrate = SYS_CONSOLE_UART_BAUDRATE,
};
sUART_t testcommand = { .param = {.selectClockSource = SYS_CONSOLE_UART_CLK,
    .parity = SYS_CONSOLE_UART_PARITY,
    .msborLsbFirst = SYS_CONSOLE_UART_MSB_FIRST,
    .numberofStopBits = SYS_CONSOLE_UART_STOPBITS,
    .uartMode = SYS_CONSOLE_UART_MODE
    },
.baseAddress = SYS_COMMAND_UART_PORT,
.baudrate = SYS_COMMAND_UART_BAUDRATE,
};

sBSPSystemInfo_t SysInfo = { .ver = VERSION, .ser= SERIALNUM };


void setUp(void)
{
}

void tearDown(void)
{
}


void test_ConsoleCommandHelp_should_Send_HelpTable(void){

    uint8_t i;

    // uint32_t tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
    for(i=0;i<command_count;i++){
        CONSOLEIO_SendString_Expect(&testconsole,mConsoleCommandTable[i].name);
        CONSOLEIO_SendString_Expect(&testconsole," : ");
        CONSOLEIO_SendString_Expect(&testconsole,mConsoleCommandTable[i].help[0]);
        CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);
    }
    
    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandHelp(mReceiveBuffer));
}





void test_ConsoleCommandVer_should_send_SystemVersion(void){

    CONSOLEIO_SendString_Expect(&testconsole,"Version : ");
    CONSOLEIO_SendString_Expect(&testconsole,&SysInfo.ver[0]);
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetVer(mReceiveBuffer));
}


void test_ConsoleCommandWho_should_send_SerialNumber(void){
    CONSOLEIO_SendString_Expect(&testconsole,"Serial# : ");
    CONSOLEIO_SendString_Expect(&testconsole,&SysInfo.ser[0]);
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetWho(mReceiveBuffer));
}



void test_ConsoleCommandGetCurrent_should_GetCurrent(void){
    
    SysInfo.current = 4;
    COMMAND_GetCurrent_ExpectAndReturn(SysInfo.current);
    CONSOLEIO_SendString_Expect(&testconsole,"Current : ");
    CONSOLEIO_SendString_Expect(&testconsole,"4");
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetCurrent(mReceiveBuffer));

}


void test_ConsoleCommandGetPulse_should_GetPulses(void){
    SysInfo.pulse = 567;

    COMMAND_GetPulse_ExpectAndReturn(SysInfo.pulse);
    CONSOLEIO_SendString_Expect(&testconsole,"Pulse : ");
    CONSOLEIO_SendString_Expect(&testconsole,"567 (ms)");
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetPulse(mReceiveBuffer));


}

void test_ConsoleCommandGetCount_should_GetCount(void){
    SysInfo.StartupCounter = 17;

    COMMAND_GetCount_ExpectAndReturn(SysInfo.StartupCounter);
    CONSOLEIO_SendString_Expect(&testconsole,"Startup Count : ");
    CONSOLEIO_SendString_Expect(&testconsole,"17");
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetCount(mReceiveBuffer));
}


void test_ConsoleCommandGetBaud1_should_Get_CommandBaudrate(void){

    SysInfo.commandBaudrate = 9600;

    CONSOLEIO_SendString_Expect(&testconsole,"Command Baud : ");
    CONSOLEIO_SendString_Expect(&testconsole,"9600");
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetBaud1(mReceiveBuffer));

}

void test_ConsoleCommandGetBaud2_should_Get_ConsoleBaudrate(void){
    
        SysInfo.commandBaudrate = 115200;
    
        CONSOLEIO_SendString_Expect(&testconsole,"Console Baud : ");
        CONSOLEIO_SendString_Expect(&testconsole,"115200");
        CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);
    
        TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetBaud2(mReceiveBuffer));
    
}


void test_ConsoleCommandGetMode_should_GetMode(void){

    strcpy(SysInfo.mode,"SPAN_ON");



    CONSOLEIO_SendString_Expect(&testconsole,"PCO2 Mode : ");
    COMMAND_GetMode_ExpectAndReturn(&SysInfo.mode[0]);
    CONSOLEIO_SendString_Expect(&testconsole,"SPAN_ON");
    CONSOLEIO_SendString_Expect(&testconsole,STR_ENDLINE);

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandGetMode(mReceiveBuffer));


}


void test_ConsoleCommandSet_should_SetCorrectTest(void){

    char temp1 = "ser";
    char temp2 = "ABCDEF1";

    ConsoleReceiveParamString_IgnoreAndReturn(COMMAND_SUCCESS);
    

    TEST_ASSERT_EQUAL(COMMAND_SUCCESS,ConsoleCommandSet(mReceiveBuffer));


    // char 

}

// void test_ConsoleCommandSetSer_should_SetSerialNumber(void){

//     char temp = "17ABCD\r\n";
//     strcpy(mReceiveBuffer,"ser= 17ABCD\r\n");


//     ConsoleReceiveParamString_ExpectAndReturn(&mReceiveBuffer[0], 1, &temp[0],COMMAND_SUCCESS);


// }