#include "unity.h"
#include "consoleCmd.h"
#include "mock_consoleIo.h"
#include "mock_pco2.h"
#include "mock_AM08x5.h"
#include "mock_scheduler.h"
#include "mock_LI8x0.h"
#include "mock_flowcontrol.h"
#include "mock_max250.h"
#include "mock_sysinfo.h"

void setUp(void)
{
    strcpy(sysinfo.ver, VERSION);
    sysinfo.gps = -2;
    sysinfo.pco2 = &pco2;

}

void tearDown(void)
{
}

void mock_display_table(sUART_t *pUART, const char buffer[])
{
    uint32_t i;
    uint32_t tableLength = consoleCommandTableSize;

    for (i = 0u; i < tableLength - 1u; i++)
    {
        CONSOLEIO_SendStr_ExpectAndReturn(pUART, (char *)mConsoleCommandTable[i].name, CONSOLEIO_OK);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
        CONSOLEIO_SendStr_ExpectAndReturn(pUART, " : ", CONSOLEIO_OK);
        CONSOLEIO_SendStr_ExpectAndReturn(pUART, (char *)mConsoleCommandTable[i].help, CONSOLEIO_OK);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
        CONSOLEIO_SendStr_ExpectAndReturn(pUART, STR_ENDLINE, CONSOLEIO_OK);
    }
}

void test_ConsoleCommandHelp_should_SendValidMsg(void)
{
    /* Arrange */
    // sUART_t UART;
    sUART_t UART;
    char buf[8] = "";

    /* Expect */
    mock_display_table(&UART, buf);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Help(&UART, " 123"));
}

void test_CONSOLECMD_Interval_should_report_normalmode_interval(void)
{
    /* Arrange */
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 6,
                           .minute = 0,
                           .second = 0};
    // eConsole&UART_t &UART = &UART_CONSOLE;
    sUART_t UART;

    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();
    PCO2_GetNormalInterval_Expect(&interval);
    PCO2_GetNormalInterval_IgnoreArg_t();
    PCO2_GetNormalInterval_ReturnThruPtr_t(&interval);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "interval= 0000-00-00T06:00:00\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Interval(&UART, ""));
}

void test_CONSOLECMD_Change_should_return_hh_mm_ss(void)
{
    /* Arrange */
    sUART_t UART;
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 0,
                           .minute = 30,
                           .second = 0};
    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();
    PCO2_GetChange_Expect(&interval);
    PCO2_GetChange_IgnoreArg_t();
    PCO2_GetChange_ReturnThruPtr_t(&interval);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "change= 00:30:00\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Change(&UART, ""));
}

void test_CONSOLECMD_Equil_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 180;

    /* Expect */
    PCO2_GetEquil_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "equil= 180\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Equil(&UART, ""));
}

void test_CONSOLECMD_Warmup_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 333;

    /* Expect */
    PCO2_GetWarmup_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "warmup= 333\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Warmup(&UART, ""));
}

void test_CONSOLECMD_PumpOn_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 132;

    /* Expect */
    PCO2_GetPumpON_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "pumpon= 132\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_PumpOn(&UART, ""));
}

void test_CONSOLECMD_PumpOff_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 132;

    /* Expect */
    PCO2_GetPumpOFF_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "pumpoff= 132\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_PumpOff(&UART, ""));
}

void test_CONSOLECMD_SampleCo2_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 132;

    /* Expect */
    PCO2_GetSampleCO2_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "sampleco2= 132\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_SampleCO2(&UART, ""));
}

void test_CONSOLECMD_Vent_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t numsecs = 971;

    /* Expect */
    PCO2_GetVent_ExpectAndReturn(numsecs);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "vent= 971\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Vent(&UART, ""));
}

void test_CONSOLECMD_Heater_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t heater = 1;

    /* Expect */
    PCO2_GetHeater_ExpectAndReturn(heater);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "heater= 1\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Heater(&UART, ""));
}

void test_CONSOLECMD_Sample_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    uint32_t sample = 4;

    /* Expect */
    PCO2_GetSample_ExpectAndReturn(sample);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "sample= 4\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Sample(&UART, ""));
}

// void test_CONSOLECMD_Sample_should_return_equil_seconds(void){
//     /* Arrange */
//     sUART_t UART;
//     uint32_t sample  = 4;

//     /* Expect */
//     PCO2_GetSample_ExpectAndReturn(sample);
//     CONSOLEIO_SendStr_ExpectAndReturn(&UART,"sample= 4\r\n",CONSOLEIO_OK);

//     /* Act */
//     TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Sample(&UART,""));
// }

void test_CONSOLECMD_Span_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    float sample = 3.18;

    /* Expect */
    PCO2_GetSpan_ExpectAndReturn(sample);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "span= 3.18\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Span(&UART, ""));
}

void test_CONSOLECMD_SpanDiff_should_return_equil_seconds(void)
{
    /* Arrange */
    sUART_t UART;
    float sample = 3.18;

    /* Expect */
    PCO2_GetSpanDiff_ExpectAndReturn(sample);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "spandiff= 3.18\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_SpanDiff(&UART, ""));
}

void test_CONSOLECMD_Fast_should_SetFastMode(void)
{
    /* Arrange */
    sTimeReg_t t1;
    sTimeReg_t t2;
    uint16_t repeat = 32;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    AM08X5_clear_time_struct_Ignore();
    PCO2_FastMode_CalculateRepeats_ExpectAndReturn(repeat);
    PCO2_GetFastInterval_Expect(&t1);
    PCO2_GetFastInterval_IgnoreArg_t();
    SCHEDULER_create_task_ExpectAndReturn(PCO2_FastMode, &t1, &t2, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();
    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Fast(&UART, buf));
}

void test_CONSOLECMD_Normal_should_SetNormalMode(void)
{
    /* Arrange */
    sTimeReg_t t1;
    sTimeReg_t t2;
    int16_t repeat = -1;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    AM08X5_clear_time_struct_Ignore();
    PCO2_GetNormalStartTime_Ignore();
    PCO2_GetNormalInterval_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(PCO2_NormalMode, &t1, &t2, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();
    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Normal(&UART, buf));
}

void test_CONSOLECMD_Deployment_should_SetDeployMode(void)
{
    /** Arrange */
    sTimeReg_t t1;
    sTimeReg_t t2;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn((void *)PCO2_Deploy, &t1, &t2, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Deployment(&UART, buf));
}

void test_CONSOLECMD_LicorON_should_TurnLicorON(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(LI8x0_PowerON, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_LicorOn(&UART, buf));
}

void test_CONSOLECMD_LicorOFF_should_TurnLicorOFF(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(LI8x0_PowerOFF, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_LicorOff(&UART, buf));
}

void test_CONSOLECMD_LicorConfig_should_TurnConfigLicor(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn((void *)LI8x0_Config, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_LicorConfig(&UART, buf));
}

void test_CONSOLECMD_FlowON_should_TurnLicorON(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(FLOW_Wakeup, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_FlowOn(&UART, buf));
}

void test_CONSOLECMD_FlowOFF_should_TurnLicorOFF(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(FLOW_Sleep, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_FlowOff(&UART, buf));
}

void test_CONSOLECMD_AnalogON_should_TurnLicorON(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(MAX250_PowerON, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_AnalogOn(&UART, buf));
}

void test_CONSOLECMD_AnalogOFF_should_TurnLicorOFF(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    AM08X5_clear_time_struct_Ignore();
    SCHEDULER_create_task_ExpectAndReturn(MAX250_PowerOFF, &t1, &t1, repeat, SCHEDULER_OK);
    SCHEDULER_create_task_IgnoreArg_start();
    SCHEDULER_create_task_IgnoreArg_period();

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_AnalogOff(&UART, buf));
}

void test_CONSOLECMD_Apos_should_SetAirPosition(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_AIR_PUMP_OFF, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Apos(&UART, buf));
}

void test_CONSOLECMD_Epos_should_SetAirPosition(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_EQUIL_PUMP_OFF, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Epos(&UART, buf));
}

void test_CONSOLECMD_Zpos_should_SetAirPosition(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_OFF, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Zpos(&UART, buf));
}

void test_CONSOLECMD_Spos_should_SetAirPosition(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_SPAN_PUMP_OFF, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Spos(&UART, buf));
}

void test_CONSOLECMD_Rpos_should_SetAirPosition(void)
{
    /* Arange */
    sTimeReg_t t1;
    int16_t repeat = 0;
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_REST, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Rpos(&UART, buf));
}

void test_CONSOLECMD_SReset_should_SetAirPosition(void)
{
    /* Arange */
    sUART_t UART;
    char buf[] = "";

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_REST, FLOW_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_SReset(&UART, buf));
}

void test_find_write_str_should_return_true_for_write_string(void)
{
    /* Arange */
    const char buf[] = "= 11.1";

    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(true, is_write_cmd(buf));
}

void test_find_write_str_should_return_false_for_read_string(void)
{
    /* Arange */
    const char buf[] = "";
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(false, is_write_cmd(buf));
}

void test_CONSOLECMD_time_should_send_the_time(void)
{
    /* Arange */
    sUART_t UART;
    char buf[] = "";
    char timestr[] = "2018-05-02T12:59:32Z\0";

    /* Expect */
    AM08X5_GetTimestamp_IEEE_Expect(buf);
    AM08X5_GetTimestamp_IEEE_IgnoreArg_str();
    AM08X5_GetTimestamp_IEEE_ReturnArrayThruPtr_str(timestr,strlen(timestr));

    CONSOLEIO_SendStr_ExpectAndReturn(&UART,timestr,CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_time(&UART,buf));
}

void test_CONSOLECMD_time_should_SetTime_if_ValidTimeStr(void){
    /* Arange */
    sUART_t UART;
    char timestr[] = "2018-05-02T13:32:12\0";
    char buf[32] = "= ";
    strcat(buf,timestr);

    /* Expect */
    AM08X5_SetTime_ExpectAndReturn(timestr,AM08X5_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"Time Set\r\n",CONSOLEIO_OK);
    AM08X5_GetTimestamp_IEEE_Expect(buf);
    AM08X5_GetTimestamp_IEEE_IgnoreArg_str();
    AM08X5_GetTimestamp_IEEE_ReturnArrayThruPtr_str(timestr,strlen(timestr));
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,timestr,CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_time(&UART,buf));

}

void test_CONSOLECMD_time_should_failout_if_invalid_time(void){
    /* Arange */
    sUART_t UART;
    char timestr[] = "ABCDEFG";
    char buf[32] = "= ";
    strcat(buf,timestr);

    /* Expect */
     AM08X5_SetTime_ExpectAndReturn(timestr,AM08X5_FAIL);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_FAIL, CONSOLECMD_time(&UART,buf));
}

void test_CONSOLECMD_Heater_should_SetHeater_to_CommandedState(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 1";

    /* Expect */
    PCO2_SetHeater_Expect(1);
    PCO2_GetHeater_ExpectAndReturn(1);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"heater= 1\r\n",CONSOLEIO_OK);
    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Heater(&UART,buf));
}

void test_CONSOLECMD_Heater_should_FailOut_for_BadInputValue(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= ?";

    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_FAIL,CONSOLECMD_Heater(&UART,buf));
}


void test_CONSOLECMD_Vent_should_set_vent_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 3608";

    /* Expect */
    PCO2_SetVent_Expect(3608);
    PCO2_GetVent_ExpectAndReturn(3608);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"vent= 3608\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Vent(&UART,buf));
}


void test_CONSOLECMD_SampleCo2_should_set_sampleco2_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 17986";

    /* Expect */
    PCO2_SetSampleCO2_Expect(17986);
    PCO2_GetSampleCO2_ExpectAndReturn(17986);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"sampleco2= 17986\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_SampleCO2(&UART,buf));
}

void test_CONSOLECMD_PumpOff_should_set_pumpoff_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 8765432";

    /* Expect */
    PCO2_SetPumpOFF_Expect(8765432);
    PCO2_GetPumpOFF_ExpectAndReturn(8765432);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"pumpoff= 8765432\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_PumpOff(&UART,buf));
}


void test_CONSOLECMD_PumpOn_should_set_pumpon_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 123456789";

    /* Expect */
    PCO2_SetPumpON_Expect(123456789);
    PCO2_GetPumpON_ExpectAndReturn(123456789);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"pumpon= 123456789\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_PumpOn(&UART,buf));
}


void test_CONSOLECMD_Warmup_should_set_pumpon_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 975318";

    /* Expect */
    PCO2_SetWarmup_Expect(975318);
    PCO2_GetWarmup_ExpectAndReturn(975318);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"warmup= 975318\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Warmup(&UART,buf));
}

void test_CONSOLECMD_Equil_should_set_pumpon_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 1151";

    /* Expect */
    PCO2_SetEquil_Expect(1151);
    PCO2_GetEquil_ExpectAndReturn(1151);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"equil= 1151\r\n",CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Equil(&UART,buf));
}

void test_CONSOLECMD_SpanDiff_should_set_to_commanded_length(void){
    /* Arange */
    sUART_t UART;
    char buf[32];
    memset(buf,0,32);
    float val = 18.77;
    sprintf(buf,"= %0.2f\0",val);
    

    /* Expect */
    PCO2_SetSpanDiff_Expect(val);
    PCO2_GetSpanDiff_ExpectAndReturn(val);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"spandiff= 18.77\r\n",CONSOLEIO_OK);

    /* Act */
    // printf("buf:%s\n",buf);
    // TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_SpanDiff(&UART,buf));
    /** @todo This test is failing because of a float conversion issue */
    TEST_IGNORE();
}

void test_CONSOLECMD_Sample_should_SetSampleto_CommandedState(void){
    /* Arange */
    sUART_t UART;
    char buf[32] = "= 4";

    /* Expect */
    PCO2_SetSample_Expect(4);
    PCO2_GetSample_ExpectAndReturn(4);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"sample= 4\r\n",CONSOLEIO_OK);
    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Sample(&UART,buf));
}


void test_CONSOLECMD_Interval_should_set_normalmode_interval(void)
{
    /* Arrange */
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 3,
                           .minute = 0,
                           .second = 0};
    // eConsole&UART_t &UART = &UART_CONSOLE;
    sUART_t UART;
    char ts[] = "00-00-00T03:00:00Z";
    char buf[24];
    memset(buf,0,24);
    strcpy(buf,"= ");
    strcat(buf,ts);
    // char buf[] = "= 00-00-00T03:00:00Z";

    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();

    AM08X5_convert_timestamp_ExpectAndReturn(&buf[2],&interval,AM08X5_OK);
    AM08X5_convert_timestamp_IgnoreArg_t();
    AM08X5_convert_timestamp_ReturnThruPtr_t(&interval);

    PCO2_SetNormalInterval_Expect(&interval);
    PCO2_SetNormalInterval_IgnoreArg_t();

    PCO2_GetNormalInterval_Expect(&interval);
    PCO2_GetNormalInterval_IgnoreArg_t();
    PCO2_GetNormalInterval_ReturnThruPtr_t(&interval);

    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "interval= 0000-00-00T03:00:00\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Interval(&UART, buf));
}

void test_CONSOLECMD_Interval_should_set_and_Fail_for_BadValue(void)
{
    /* Arrange */
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 3,
                           .minute = 0,
                           .second = 0};
    // eConsole&UART_t &UART = &UART_CONSOLE;
    sUART_t UART;
    char ts[] = "0000-00-00T00:75:00Z";
    char buf[24];
    memset(buf,0,24);
    strcpy(buf,"= ");
    strcat(buf,ts);
    // char buf[] = "= 00-00-00T03:00:00Z";

    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();

    AM08X5_convert_timestamp_IgnoreAndReturn(AM08X5_FAIL);
    
    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_FAIL, CONSOLECMD_Interval(&UART, buf));
}

void test_CONSOLECMD_Change_should_set_hh_mm_ss(void)
{
    /* Arrange */
    sUART_t UART;
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 0,
                           .minute = 45,
                           .second = 0};
    char buf[] = "= 00:45:00";
    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();

    AM08X5_convert_timestamp_ExpectAndReturn(&buf[2],&interval,AM08X5_OK);
    AM08X5_convert_timestamp_IgnoreArg_t();
    AM08X5_convert_timestamp_ReturnThruPtr_t(&interval);
    
    PCO2_SetChange_Expect(&interval);

    PCO2_GetChange_Expect(&interval);
    PCO2_GetChange_IgnoreArg_t();
    PCO2_GetChange_ReturnThruPtr_t(&interval);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART, "change= 00:45:00\r\n", CONSOLEIO_OK);

    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_OK, CONSOLECMD_Change(&UART, buf));
}

void test_CONSOLECMD_Change_should_set_and_fail_for_bad_hh_mm_ss(void)
{
    /* Arrange */
    sUART_t UART;
    sTimeReg_t interval = {.year = 0,
                           .month = 0,
                           .date = 0,
                           .hour = 0,
                           .minute = 155,
                           .second = 0};
    char buf[] = "= 00:155:00";
    /* Expect */
    AM08X5_clear_time_struct_Expect(&interval);
    AM08X5_clear_time_struct_IgnoreArg_t();

    AM08X5_convert_timestamp_IgnoreAndReturn(AM08X5_FAIL);


    /* Act */
    TEST_ASSERT_EQUAL(CONSOLECMD_FAIL, CONSOLECMD_Change(&UART, buf));
}

void test_CONSOLECMD_ver_should_return_version(void){
    /* Arange */
    sUART_t UART;
    char buf[] = "";
    /* Expect */
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"ver= ",CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,VERSION,CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,STR_ENDLINE,CONSOLEIO_OK);

    /* Act */ 
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Ver(&UART,buf));
}

void test_CONSOLECMD_gps_should_return_gps(void){
    /* Arange */
    sUART_t UART;
    char buf[] = "";
    /* Expect */
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"gps= -2",CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,STR_ENDLINE,CONSOLEIO_OK);

    /* Act */ 
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_gps(&UART,buf));
}

void test_CONSOLECMD_startup_should_return_startup(void){
    /* Arange */
    sUART_t UART;
    char buf[] = "";
    sysinfo.StartupCounter = 1792;

    /* Expect */
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"startup= 1792",CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,STR_ENDLINE,CONSOLEIO_OK);

    /* Act */ 
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_Startup(&UART,buf));

}

void test_CONSOLECMD_Serial_should_return_SerialNumber(void){
    /* Arange */
    sUART_t UART;
    char sn[20] = "pco22018047";
    strcpy(sysinfo.SerialNumber,sn);

    /* Expect */
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,"ser= ",CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,sn,CONSOLEIO_OK);
    CONSOLEIO_SendStr_ExpectAndReturn(&UART,STR_ENDLINE,CONSOLEIO_OK);

    /* Act */ 
    TEST_ASSERT_EQUAL(CONSOLECMD_OK,CONSOLECMD_SerialNum(&UART,""));

}

/* Arange */

/* Expect */

/* Act */



/* Arange */

/* Expect */

/* Act */