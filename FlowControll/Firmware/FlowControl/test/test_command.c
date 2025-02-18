#include "unity.h"
#include "command.h"
#include "mock_bsp.h"
#include "mock_motioncontrol.h"
#include "buffer_c.h"

#define SERIALNUM   ("SERNUM01")

sBSPSystemInfo_t SysInfo = {    .ver = VERSION, 
                                .ser= SERIALNUM,
                                .mode = "",
                                .current = 5,
                                .pulse = 15,
                                .commandBaudrate = 9600,
                                .consoleBaudrate = 115200,
                                .StartupCounter = 78 
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

void setUp(void)
{
}

void tearDown(void)
{
}

void test_COMMAND_SetMode_should_AcceptValidString_and_SetValveMode_and_UpdateInfo(void){
    char mode[] = "ZERO_ON";

    MCONTROL_PCO2_SetMode_ExpectAndReturn(PCO2_ZERO_PUMP_ON,MCONTROL_OK);
    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_SetMode(&mode[0]));

    TEST_ASSERT_EQUAL_STRING(mode,SysInfo.mode);

}


void test_COMMAND_SetCurrent_should_AcceptValidNumber_and_SetCurrent(void){

    uint8_t i;

    for(i=0;i<8;i++){
        MCONTROL_PCO2_SetCurrent_ExpectAndReturn(i,MCONTROL_OK);
        TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_SetCurrent(i));
    }
}

void test_COMMAND_SetCurrent_should_Fail_for_InvalidValues(void){
    MCONTROL_PCO2_SetCurrent_ExpectAndReturn(8,MCONTROL_FAIL);
    TEST_ASSERT_EQUAL(COMMAND_FAIL,COMMAND_SetCurrent(8));
}


void test_COMMAND_SetPulse_should_AcceptValidNumbers_and_SetPulse(void){

    uint32_t pulse = 64;
    MCONTROL_PCO2_SetPulse_ExpectAndReturn(pulse,MCONTROL_OK);
    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_SetPulse(pulse));
}

void test_COMMAND_SetPulse_should_Fail_for_InvalidNumbers(void){

    uint32_t pulse = 4;
    MCONTROL_PCO2_SetPulse_ExpectAndReturn(pulse,MCONTROL_FAIL);
    TEST_ASSERT_EQUAL(COMMAND_FAIL,COMMAND_SetPulse(pulse));

}


void test_COMMAND_SetBaud_should_SetValidBaud_and_ReturnValid(void){
    
    uint32_t baud = 9600;

    BSP_UART_Init_ExpectAndReturn(&command,BSP_OK);

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_SetBaud(baud));
}


void test_COMMAND_SetBaud_should_Fail_for_InvalidBaud_and_ReturnOriginal(void){
    uint32_t baud = 6829;


    BSP_UART_Init_ExpectAndReturn(&command,BSP_FAIL);
    BSP_UART_Init_ExpectAndReturn(&command,BSP_OK);
    TEST_ASSERT_EQUAL(COMMAND_FAIL,COMMAND_SetBaud(baud));
    TEST_ASSERT_EQUAL(9600,command.baudrate);

}


void test_COMMAND_GetMode_should_ReturnValidString(void){
    
    
    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_ZERO_PUMP_ON);
    TEST_ASSERT_EQUAL_STRING("ZERO_ON",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_ZERO_PUMP_OFF);
    TEST_ASSERT_EQUAL_STRING("ZERO_OFF",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_ZERO_CAL);
    TEST_ASSERT_EQUAL_STRING("ZERO_POST",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_SPAN_PUMP_ON);
    TEST_ASSERT_EQUAL_STRING("SPAN_ON",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_SPAN_PUMP_OFF);
    TEST_ASSERT_EQUAL_STRING("SPAN_OFF",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_SPAN_CAL);
    TEST_ASSERT_EQUAL_STRING("SPAN_POST",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_EQUIL_PUMP_ON);
    TEST_ASSERT_EQUAL_STRING("EQUIL_ON",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_AIR_PUMP_OFF);
    TEST_ASSERT_EQUAL_STRING("AIR_OFF",COMMAND_GetMode());

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_DEPLOYMENT);
    TEST_ASSERT_EQUAL_STRING("DEPLOY",COMMAND_GetMode());


}


void test_COMMAND_GetCurrent_should_RetreiveCurrentCurrentSettings(void){

    MCONTROL_PCO2_GetCurrent_ExpectAndReturn(3);
    TEST_ASSERT_EQUAL(3,COMMAND_GetCurrent());

}

void test_COMMAND_GetPulse_should_RetreiveCurrentPulseDelayLength(void){

    MCONTROL_PCO2_GetPulse_ExpectAndReturn(332);
    TEST_ASSERT_EQUAL(332,COMMAND_GetPulse());
}

void test_COMMAND_GetVersion_should_Return_VersionNumberString(void){
    strcpy(SysInfo.ver,"v1.0.1");
    TEST_ASSERT_EQUAL_STRING(SysInfo.ver,COMMAND_GetVersion());
}

void test_COMMAND_GetSerial_should_Return_VersionNumberString(void){
    strcpy(SysInfo.ser,"TEST17042");
    TEST_ASSERT_EQUAL_STRING(SysInfo.ser,COMMAND_GetSerial());
}

void test_COMMAND_GetCount_should_Return_StartupCount(void){
    SysInfo.StartupCounter = 3928;
    TEST_ASSERT_EQUAL(SysInfo.StartupCounter,COMMAND_GetCount());
}

void test_COMMAND_GetBaud_should_ReturnBaudrate(void){
    command.baudrate = 115200;
    TEST_ASSERT_EQUAL(command.baudrate,COMMAND_GetBaud());
}





// void test_COMMAND_ParseMode_should_Parse_and_set_CorrectMode_and_ReturnValid(void){

//     char buf[] = ""

// }




void test_COMMAND_ParseMsg_should_Parse_SetModeMessage_and_SetMode_and_ReturnValid(void){
    char msg[] = "mode=ZERO_ON\r\n";
    
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    MCONTROL_PCO2_SetMode_ExpectAndReturn(PCO2_ZERO_PUMP_ON,MCONTROL_OK);
    BSP_UART_putc_Expect(&command,UART_ACK);
    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());
}


void test_COMMAND_ParseMsg_should_Parse_SetPulseMessage_and_SetPulse_and_ReturnValid(void){
    char msg[] = "pulse=1000\r\n";

    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    MCONTROL_PCO2_SetPulse_ExpectAndReturn(1000,MCONTROL_OK);
    BSP_UART_putc_Expect(&command,UART_ACK);
    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());
}


void test_COMMAND_ParseMsg_should_Parse_SetCurrent_and_SetCurrent_and_ReturnValid(void){

    char msg[] = "current=5\r\n";
    
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    MCONTROL_PCO2_SetCurrent_ExpectAndReturn(5,MCONTROL_OK);
    BSP_UART_putc_Expect(&command,UART_ACK);
    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());

}



void test_COMMAND_ParseMsg_should_SendMode_for_ModeRequest(void){
    char msg[] = "mode\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "MODE: ZERO_ON\r\n";

    MCONTROL_PCO2_GetMode_ExpectAndReturn(PCO2_ZERO_PUMP_ON);
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());
}


void test_COMMAND_ParseMsg_should_GetCurrent_for_CurrentRequest(void){
    char msg[] = "current\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "CURRENT: 3\r\n";

    MCONTROL_PCO2_GetCurrent_ExpectAndReturn(3);
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());
}


void test_COMMAND_ParseMsg_should_GetPulse_for_PulseRequest(void){
    char msg[] = "pulse\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "PULSE: 777\r\n";

    MCONTROL_PCO2_GetPulse_ExpectAndReturn(777);
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());

}

void test_COMMAND_ParseMsg_should_GetVersion_for_VersionRequest(void){
    char msg[] = "ver\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "VERSION: v1.7.32\r\n";

    strcpy(SysInfo.ver,"v1.7.32");
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());

}

void test_COMMAND_ParseMsg_should_GetSerial_for_SerialRequest(void){
    char msg[] = "ser\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "SERIAL: SEN170084\r\n";

    strcpy(SysInfo.ser,"SEN170084");
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());

}

void test_COMMAND_ParseMsg_should_GetCount_for_CountRequest(void){
    char msg[] = "count\r\n";
    BufferC_puts(&command.buf,&msg[0],strlen(msg));
    char rxStr[] = "COUNT: 3982\r\n";

    SysInfo.StartupCounter = 3982;
    BSP_UART_putc_Expect(&command,UART_ACK);
    BSP_UART_puts_Expect(&command,&rxStr[0],strlen(rxStr));

    TEST_ASSERT_EQUAL(COMMAND_OK,COMMAND_ParseMsg());

}