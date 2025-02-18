#include "unity.h"
#include "console.h"
#include "mock_buffer_c.h"
#include "mock_consoleCmd.h"
#include "mock_consoleIo.h"

void setUp(void)
{
}

void tearDown(void)
{
}

// void test_TEMPLATE(void){
//     /* Arrange */

//     /* Expect */

//     /* Act */

// }





void test_CONSOLE_CommandMatch_should_MatchCommand_and_ReturnTrue(void){
    /* Arrange */
    char cmd[] = "TEST";
    char buf[] = "TEST 1 2 3";
    uint32_t result;
    /* Expect */


    /* Act */
    TEST_ASSERT_EQUAL_UINT32(1,CONSOLE_CommandMatch(cmd,buf));

}

void test_CONSOLE_CommandMatch_should_ReturnFail_for_UnmatchedCommand_NotSimilar(void){
    /* Arrange */
    char cmd[] = "TEST";
    char buf[] = "RUN";
    uint32_t result;

    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL_UINT32(0,CONSOLE_CommandMatch(cmd,buf));

}

void test_CONSOLE_CommandMatch_should_ReturnFail_for_UnmatchedCommand_StartWithSameLetter(void){
    /* Arrange */
    char cmd[] = "TEST";
    char buf[] = "TUBER";
    uint32_t result;

    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL_UINT32(0,CONSOLE_CommandMatch(cmd,buf));

}


eConsoleCmdStatus_t mock_function(sUART_t *pUART, const char buffer[]){
    printf("Mock Function");
}

const sConsoleCommandTable_t testConsoleCommandTable[] =
{
    {"?", &mock_function, HELP("List available commands")},
    {"ver", &mock_function, HELP("Get the version string")},
    {"cshow", &mock_function, HELP("Show COM Data")},
    {"gps", &mock_function, HELP("GPS Data (-2 for PCO2 Sensor)")},
    {"time", &mock_function, HELP("Sensor Date & Time (yy mm dd hh mm ss")},
    {"idata", &mock_function, HELP("Iridium Data")},
    {"sdata", &mock_function, HELP("Sensor Raw Data")},
    {"who", &mock_function, HELP("Serial Number")},
    {"serial", &mock_function, HELP("Serial Number")},
    {"sample", &mock_function, HELP("Data Sampling Interval (Hz)")},
    {"span", &mock_function, HELP("Span Gas Concentration (ppm)")},
    {"spandiff", &mock_function, HELP("Span Pressure Difference (kPa)")},
    {"interval",&mock_function, HELP("Normal Mode Cycle Interval (hours)")},
    {"change", &mock_function, HELP("Length of Fast Mode Cycle (sec)")},
    {"equil", &mock_function, HELP("Length of Equilibration Pump (sec)")},
    {"warmup", &mock_function, HELP("Length of LICOR Warmup (sec)")},
    {"pumpon", &mock_function, HELP("Length of pump on during cycle (sec)")},
    {"pumpoff",&mock_function,HELP("Length of pump off during a cycle (sec)")},
    {"sampleco2", &mock_function, HELP("Length of data sample in cycle (sec)")},
    {"vent", &mock_function, HELP("Legth of delay between open/close vent (sec)")},
    {"heater",&mock_function, HELP("Licor Heater OFF or ON")},
	CONSOLE_COMMAND_TABLE_END // must be LAST
};



void test_CONSOLE_Process_should_process_the_command_and_return(void){
    /* Arrange */
    char cmd[] = "ver";
    char buf[64];
    uint32_t i;

    /* Expect */
    /** Clear the buffer */
    BufferC_gets_ExpectAndReturn(&Console.buf,buf,CONSOLE_BUFFER_LENGTH, BUFFER_C_OK);
    BufferC_gets_IgnoreArg_str();
    BufferC_gets_ReturnThruPtr_str(cmd);
    
    /** Get the Table */
    CONSOLECMD_GetTable_ExpectAndReturn(testConsoleCommandTable);
    CONSOLEIO_SendStr_ExpectAndReturn(&Console,CONSOLE_PROMPT,CONSOLEIO_OK);


    /* Act */
    CONSOLE_Process();

}


void test_CONSOLE_Init_should_InitializeSerialPorts_and_SendPrompt(void){
    /* Arrange */

    /* Expect */
    CONSOLEIO_Init_ExpectAndReturn(&Console,CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&Console,"> ", CONSOLEIO_OK);

    /* Act */
    CONSOLE_Init();
}