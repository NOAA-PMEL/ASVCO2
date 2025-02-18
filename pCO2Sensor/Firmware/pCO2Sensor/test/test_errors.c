#include "unity.h"
#include "errors.h"
#include "mock_bsp.h"
#include "mock_console.h"
#include "mock_command.h"
#include "mock_datalogger.h"
#include "mock_AM08x5.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_logGetType_should_return_ValidString(void){
    eLogType_t type = LOGGING_None;
    char returnedMsg[LOG_TYPE_PRINT_STR_LEN];

    char expectedMsg[LOG_TYPE_PRINT_STR_LEN] = "None\r\n";
    memset(returnedMsg,0,LOG_TYPE_PRINT_STR_LEN);
    logGetType(type,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    type = LOGGING_Warnings;
    strcpy(expectedMsg,"Warnings\r\n");
    memset(returnedMsg,0,LOG_TYPE_PRINT_STR_LEN);
    logGetType(type,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    type = LOGGING_Critical;
    strcpy(expectedMsg,"Critical\r\n");
    memset(returnedMsg,0,LOG_TYPE_PRINT_STR_LEN);
    logGetType(type,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    type = LOGGING_All;
    strcpy(expectedMsg,"All\r\n");
    memset(returnedMsg,0,LOG_TYPE_PRINT_STR_LEN);
    logGetType(type,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);
    
}

void test_logGetLevel_should_return_ValidString(void){
    eLogType_t level = LOGGING_Off;
    char returnedMsg[LOG_LEVEL_PRINT_STR_LEN];

    char expectedMsg[LOG_LEVEL_PRINT_STR_LEN] = "Off\r\n";
    memset(returnedMsg,0,LOG_LEVEL_PRINT_STR_LEN);
    logGetLevel(level,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    level = LOGGING_Minimal;
    strcpy(expectedMsg,"Minimal\r\n");
    memset(returnedMsg,0,LOG_LEVEL_PRINT_STR_LEN);
    logGetLevel(level,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    level = LOGGING_Verbose;
    strcpy(expectedMsg,"Verbose\r\n");
    memset(returnedMsg,0,LOG_LEVEL_PRINT_STR_LEN);
    logGetLevel(level,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);
    
}

void test_logGetTimestampSetting_should_return_ValidString(void){
    eLogTStampSet_t tstamp = LOGGING_TSTAMP_Off;
    char returnedMsg[LOG_TIMESTAMP_PRINT_STR_LEN];
    
    char expectedMsg[LOG_TIMESTAMP_PRINT_STR_LEN] = "Off\r\n";
    memset(returnedMsg,0,LOG_TIMESTAMP_PRINT_STR_LEN); 
    logGetTimestampSetting(tstamp,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

    tstamp = LOGGING_TSTAMP_On;
    strcpy(expectedMsg,"On\r\n");
    memset(returnedMsg,0,LOG_TIMESTAMP_PRINT_STR_LEN);
    logGetTimestampSetting(tstamp,&returnedMsg[0]);
    TEST_ASSERT_EQUAL_STRING(expectedMsg,returnedMsg);

}


void test_LogPrint_should_PrintCorrectSettings_to_Console(void) {
    errors.type = LOGGING_All;
    errors.level = LOGGING_Verbose;
    errors.port = LOG_PORT_Console;
    debug.type = LOGGING_Warnings;
    debug.level = LOGGING_Minimal;

    char type[] =  "Error Type : All\r\n";
    char level[] = "Error Level: Verbose\r\n";

    CONSOLE_puts_Expect(&type[0]);
    CONSOLE_puts_Expect(&level[0]);

    ErrorPrint();



}

void test_LogPrint_should_PrintCorrectSettings_to_Command(void){
    errors.type = LOGGING_Critical;
    errors.level = LOGGING_Minimal;
    errors.port = LOG_PORT_Command;

    char cType[] =  "Error Type : Critical\r\n";
    char cLevel[] = "Error Level: Minimal\r\n";

    COMMAND_puts_Expect(&cType[0]);
    COMMAND_puts_Expect(&cLevel[0]);
    ErrorPrint();
}

void test_LogPrint_should_PrintCorrectSettings_to_CommandAndConsole(void){
    errors.type = LOGGING_Warnings;
    errors.level = LOGGING_Off;
    errors.port = LOG_PORT_Command | LOG_PORT_Console;

    char cType[] =  "Error Type : Warnings\r\n";
    char cLevel[] = "Error Level: Off\r\n";

    CONSOLE_puts_Expect(&cType[0]);
    COMMAND_puts_Expect(&cType[0]);

    CONSOLE_puts_Expect(&cLevel[0]);
    COMMAND_puts_Expect(&cLevel[0]);

    ErrorPrint();
}

void test_LogPrint_should_PrintSettings_to_Datalogger(void){
    TEST_IGNORE();
}

void test_DebugPrint_should_PrintSettings_to_Console(void){
    debug.type = LOGGING_All;
    debug.level = LOGGING_Verbose;
    debug.port = LOG_PORT_Console;

    char type[] =  "Debug Type : All\r\n";
    char level[] = "Debug Level: Verbose\r\n";

    CONSOLE_puts_Expect(&type[0]);
    CONSOLE_puts_Expect(&level[0]);

    DebugPrint();
    
}

void test_DebugPrint_should_PrintSettings_to_Command(void){
    debug.type = LOGGING_Critical;
    debug.level = LOGGING_Minimal;
    debug.port = LOG_PORT_Command;

    char type[] =  "Debug Type : Critical\r\n";
    char level[] = "Debug Level: Minimal\r\n";

    COMMAND_puts_Expect(&type[0]);
    COMMAND_puts_Expect(&level[0]);

    DebugPrint();

}

void test_DebugPrint_should_PrintSettings_to_CommandAbdConsole(void){
    debug.type = LOGGING_Critical;
    debug.level = LOGGING_Minimal;
    debug.port = LOG_PORT_Command | LOG_PORT_Console;

    char type[] =  "Debug Type : Critical\r\n";
    char level[] = "Debug Level: Minimal\r\n";

    CONSOLE_puts_Expect(&type[0]);
    COMMAND_puts_Expect(&type[0]);

    CONSOLE_puts_Expect(&level[0]);
    COMMAND_puts_Expect(&level[0]);
    

    DebugPrint();

}

// void test_ErrorPrint_should_PrintSettings_to_Datalogger(void){
//     TEST_IGNORE();
// }

void test_ErrorPrint_should_PrintSettings_to_Console(void){
    errors.type = LOGGING_All;
    errors.level = LOGGING_Verbose;
    errors.port = LOG_PORT_Console;

    char type[] =  "Error Type : All\r\n";
    char level[] = "Error Level: Verbose\r\n";

    CONSOLE_puts_Expect(&type[0]);
    CONSOLE_puts_Expect(&level[0]);

    ErrorPrint();
    
}

void test_ErrorPrint_should_PrintSettings_to_Command(void){
    errors.type = LOGGING_Critical;
    errors.level = LOGGING_Minimal;
    errors.port = LOG_PORT_Command;

    char type[] =  "Error Type : Critical\r\n";
    char level[] = "Error Level: Minimal\r\n";

    COMMAND_puts_Expect(&type[0]);
    COMMAND_puts_Expect(&level[0]);

    ErrorPrint();

}

void test_ErrorPrint_should_PrintSettings_to_CommandAbdConsole(void){
    errors.type = LOGGING_Critical;
    errors.level = LOGGING_Minimal;
    errors.port = LOG_PORT_Command | LOG_PORT_Console;

    char type[] =  "Error Type : Critical\r\n";
    char level[] = "Error Level: Minimal\r\n";

    CONSOLE_puts_Expect(&type[0]);
    COMMAND_puts_Expect(&type[0]);

    CONSOLE_puts_Expect(&level[0]);
    COMMAND_puts_Expect(&level[0]);
    

    ErrorPrint();

}

void test_DebugPrint_should_PrintSettings_to_Datalogger(void){
    TEST_IGNORE();
}

void test_LogConfig_should_SetCorrectSettings(void){

    logs.type = LOGGING_Warnings;
    logs.level = LOGGING_Off;
    logs.timestamp = LOGGING_TSTAMP_On;
    logs.port = LOG_PORT_Command | LOG_PORT_Console;

    LogConfig(LOGGING_All,LOGGING_Verbose,LOGGING_TSTAMP_Off);

    TEST_ASSERT_EQUAL(LOGGING_All, logs.type);
    TEST_ASSERT_EQUAL(LOGGING_Verbose, logs.level);
    TEST_ASSERT_EQUAL(LOGGING_TSTAMP_Off,logs.timestamp);
}

void test_DebugConfig_should_SetCorrectSettings(void){
    
        debug.type = LOGGING_Warnings;
        debug.level = LOGGING_Off;
        debug.port = LOG_PORT_Command | LOG_PORT_Console;
        debug.timestamp = LOGGING_TSTAMP_Off;
        DebugConfig(LOGGING_All,LOGGING_Verbose,LOGGING_TSTAMP_Off);
    
        TEST_ASSERT_EQUAL(LOGGING_All, debug.type);
        TEST_ASSERT_EQUAL(LOGGING_Verbose, debug.level);
        TEST_ASSERT_EQUAL(LOGGING_TSTAMP_Off,debug.timestamp);
    
}

void test_ErrorConfig_should_SetCorrectSettings(void){
    
        errors.type = LOGGING_Warnings;
        errors.level = LOGGING_Off;
        errors.port = LOG_PORT_Command | LOG_PORT_Console;
        errors.timestamp = LOGGING_TSTAMP_On;

        ErrorConfig(LOGGING_All,LOGGING_Verbose,LOGGING_TSTAMP_Off);
    
        TEST_ASSERT_EQUAL(LOGGING_All, errors.type);
        TEST_ASSERT_EQUAL(LOGGING_Verbose, errors.level);
        TEST_ASSERT_EQUAL(LOGGING_TSTAMP_Off,errors.timestamp);
}

void test_MessageLevel_Verbose_should_ReturnCorrectString(void){

    eLogLevel_t level = LOGGING_Verbose;
    char msg[] = "0040 DEBUG TEST MESSAGE";
    char *pMsg = &msg[0];
    char expectedMsg[80] = "Debug: D";
    eLog_t logType = LOG_Debug;


    // printf("%s\n",msg);
    strcat(expectedMsg,msg);

    MessageLevel(logType,level,pMsg);
    // printf("%s\n",msg);
    
    TEST_ASSERT_EQUAL_STRING(expectedMsg,msg);

}

void test_MessageLevel_Minimal_should_ReturnCorrectString(void){

    eLogLevel_t level = LOGGING_Minimal;
    char msg[] = "0810 DEBUG TEST MESSAGE 2";
    char *pMsg = &msg[0];
    char expectedMsg[80] = "D";
    eLog_t logType = LOG_Debug;


    strncat(expectedMsg,msg,4);

    MessageLevel(logType,level,pMsg);
    
    TEST_ASSERT_EQUAL_STRING(expectedMsg,msg);
}

void test_MessageLevel_Off_should_ReturnCorrectString(void){
    
    eLogLevel_t level = LOGGING_Off;
    char msg[] = "0810 DEBUG TEST MESSAGE 2";
    char *pMsg = &msg[0];
    char expectedMsg[32] = "";
    eLog_t logType = LOG_Debug;    
    MessageLevel(logType,level,pMsg);
    
    TEST_ASSERT_EQUAL_STRING(expectedMsg,msg);
}

void test_Debug_should_Return_CorrectString_for_AllSetting(void){
    debug.type = LOGGING_All;
    debug.level = LOGGING_Verbose;
    debug.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Debug";
    char expectedMsg[80] = "Debug: D";

    strcat(expectedMsg,str);

    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

    priority = LOG_Priority_Med;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

    priority = LOG_Priority_Low;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

}

void test_Debug_should_Return_CorrectString_for_Level_Minimal(void){
    debug.type = LOGGING_All;
    debug.level = LOGGING_Minimal;
    debug.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Debug";
    char expectedMsg[80] = "D";

    strncat(expectedMsg,str,4);

    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

    priority = LOG_Priority_Med;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

    priority = LOG_Priority_Low;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&str[0],priority);

}

void test_Debug_should_Return_Nothing_for_Level_Off(void){
    debug.type = LOGGING_All;
    debug.level = LOGGING_Off;
    debug.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Debug";
    char expectedMsg[80] = "Debug: D";

    strcat(expectedMsg,str);

    Debug(&str[0],priority);

    priority = LOG_Priority_Med;
    Debug(&str[0],priority);

    priority = LOG_Priority_Low;
    Debug(&str[0],priority);
}


void test_Debug_should_Timestamp_Return_if_Selected(void){
    debug.type = LOGGING_All;
    debug.level = LOGGING_Verbose;
    debug.port = LOG_PORT_Console;
    debug.timestamp = LOGGING_TSTAMP_On;
    eLogPriority_t priority = LOG_Priority_High;
    char teststr[] = "0005 Testing Debug";

    char tstr[24] = "2018/01/12 12:41:01";
    char expectedMsg[80];
    memset(expectedMsg,0,80);
    AM08X5_Datestamp_t ds = DS_YYYYsMMsDD;
    AM08X5_Timestamp_t ts = TS_TIME_HH_MM_SS;

    // strcat(expectedMsg,tstr);
    // strcat(expectedMsg,",");
    strcat(expectedMsg,"Debug: D");
    strcat(expectedMsg,teststr);
    // printf("Test=%s\n",expectedMsg);

    AM08X5_GetTimestamp_Expect(&expectedMsg[0],ds,ts);
    AM08X5_GetTimestamp_IgnoreArg_str();
    AM08X5_GetTimestamp_ReturnArrayThruPtr_str(&tstr[0],strlen(tstr));
    strcat(tstr,",");
    CONSOLE_puts_Expect(&tstr[0]);
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Debug(&teststr[0],priority);

}


void test_Errors_should_Return_CorrectString_for_AllSetting(void){
    errors.type = LOGGING_All;
    errors.level = LOGGING_Verbose;
    errors.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Debug";
    char expectedMsg[80] = "Error: E";

    strcat(expectedMsg,str);

    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

    priority = LOG_Priority_Med;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

    priority = LOG_Priority_Low;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

}

void test_Errors_should_Return_CorrectString_for_Level_Minimal(void){
    errors.type = LOGGING_All;
    errors.level = LOGGING_Minimal;
    errors.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Debug";
    char expectedMsg[80] = "E";

    strncat(expectedMsg,str,4);

    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

    priority = LOG_Priority_Med;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

    priority = LOG_Priority_Low;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Error(&str[0],priority);

}

void test_Errors_should_Return_Nothing_for_Level_Off(void){
    errors.type = LOGGING_All;
    errors.level = LOGGING_Off;
    errors.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "0005 Testing Error";
    char expectedMsg[80] = "Error: E";

    strcat(expectedMsg,str);

    Error(&str[0],priority);

    priority = LOG_Priority_Med;
    Error(&str[0],priority);

    priority = LOG_Priority_Low;
    Error(&str[0],priority);
}


void test_Log_should_Return_CorrectString_for_AllSetting(void){
    logs.type = LOGGING_All;
    logs.level = LOGGING_Verbose;
    logs.port = LOG_PORT_Console;

    eLogPriority_t priority = LOG_Priority_High;

    char str[] = "This is the log test";
    char expectedMsg[80] = "Log: ";

    strcat(expectedMsg,str);

    CONSOLE_puts_Expect(&expectedMsg[0]);
    Log(&str[0],priority);

    priority = LOG_Priority_Med;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Log(&str[0],priority);

    priority = LOG_Priority_Low;
    CONSOLE_puts_Expect(&expectedMsg[0]);
    Log(&str[0],priority);

}






///////////////////////////////////////////////
// void test_Log_should_Return_CorrectString_for_Level_Minimal(void){
//     errors.type = LOGGING_All;
//     errors.level = LOGGING_Minimal;
//     errors.port = LOG_PORT_Console;

//     eLogPriority_t priority = LOG_Priority_High;

//     char str[] = "0005 Testing Debug";
//     char expectedMsg[80] = "D";

//     strncat(expectedMsg,str,4);

//     CONSOLE_puts_Expect(&expectedMsg[0]);
//     Error(&str[0],priority);

//     priority = LOG_Priority_Med;
//     CONSOLE_puts_Expect(&expectedMsg[0]);
//     Error(&str[0],priority);

//     priority = LOG_Priority_Low;
//     CONSOLE_puts_Expect(&expectedMsg[0]);
//     Error(&str[0],priority);

// }

// void test_Log_should_Return_Nothing_for_Level_Off(void){
//     errors.type = LOGGING_All;
//     errors.level = LOGGING_Off;
//     errors.port = LOG_PORT_Console;

//     eLogPriority_t priority = LOG_Priority_High;

//     char str[] = "0005 Testing Debug";
//     char expectedMsg[80] = "Debug: D";

//     strcat(expectedMsg,str);

//     Error(&str[0],priority);

//     priority = LOG_Priority_Med;
//     Error(&str[0],priority);

//     priority = LOG_Priority_Low;
//     Error(&str[0],priority);
// }