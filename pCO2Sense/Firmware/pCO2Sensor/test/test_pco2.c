#include "unity.h"
#include "pco2.h"
#include "mock_LI8x0.h"
#include "mock_flowcontrol.h"
#include "mock_MAX310x.h"
#include "mock_max250.h"
#include "mock_statistics.h"
#include "mock_errors.h"
#include "mock_rh.h"
#include "mock_datalogger.h"
#include "mock_AM08x5.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_PCO2_SetMode_should_SetMode_ZeroPumpOn_and_FlowController_and_ReturnValid(void)
{
    /* Arrange */
    ePCO2Mode_t mode = MODE_ZERO_PUMP_ON;

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON, FLOW_OK);
    Log_Expect("PCO2 Mode: ZEON", LOG_Priority_Low);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_SetMode(mode));
    TEST_ASSERT_EQUAL_STRING("ZEON", pco2.mode);
}

void test_PCO2_SetMode_should_SetMode_Deploy_and_FlowController_and_ReturnValid(void)
{
    /* Arrange */
    ePCO2Mode_t mode = MODE_DEPLOY;

    /* Expect */
    FLOW_SetMode_ExpectAndReturn(FLOW_STATE_DEPLOY, FLOW_OK);
    Log_Expect("PCO2 Mode: DPLY", LOG_Priority_Low);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_SetMode(mode));
    TEST_ASSERT_EQUAL_STRING("DPLY", pco2.mode);
}

void test_PCO2_SetMode_should_Fail_if_NonValid_mode(void)
{
    /* Arrange */
    ePCO2Mode_t mode = 0xFF;

    /* Expect */
    Error_Expect(ERROR_MSG_0122, LOG_Priority_Low);
    Log_Expect("PCO2 Mode: FAIL", LOG_Priority_Low);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_SetMode(mode));
}

void test_PCO2_SampleConfig_should_RunState_and_Return(void)
{
    /* Arrange */
    uint16_t temp = 0;
    pco2.timeout1 = &temp;

    /* Expect */
    /** Power On */
    Log_Expect("PCO2 Config", LOG_Priority_Low);
    LI8x0_PowerON_Expect();

    /** Wait for timeout ignored in test */

    /** Configure the Licor */
    LI8x0_Config_ExpectAndReturn(LICOR_OK);

    /* Act */
    PCO2_State_Config();
    TEST_ASSERT_EQUAL(10, temp);
}

void mock_setmode(ePCO2Mode_t mode, eFLOWState_t status, char *mstr)
{
    switch (mode)
    {
    case MODE_ZERO_PUMP_ON:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "ZEON");
            Log_Expect("PCO2 Mode: ZEON", LOG_Priority_Low);
        }
        break;
    case MODE_ZERO_PUMP_OFF:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_OFF, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "ZEOFF");
            Log_Expect("PCO2 Mode: ZEOFF", LOG_Priority_Low);
        }
        break;
    case MODE_ZERO_PRE_CAL:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_POST, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "ZEOFF");
            Log_Expect("PCO2 Mode: ZEOFF", LOG_Priority_Low);
        }
        break;
    case MODE_ZERO_POST_CAL:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_POST, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "ZEPC");
            Log_Expect("PCO2 Mode: ZEPC", LOG_Priority_Low);
        }
        break;
    case MODE_SPAN_PUMP_ON:
    case MODE_SPAN2_PUMP_ON:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_SPAN_PUMP_ON, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "SPON");
            Log_Expect("PCO2 Mode: SPON", LOG_Priority_Low);
        }
        break;
    case MODE_SPAN_PUMP_OFF:
    case MODE_SPAN2_PUMP_OFF:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_SPAN_PUMP_OFF, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "SPOFF");
            Log_Expect("PCO2 Mode: SPOFF", LOG_Priority_Low);
        }
        break;
    case MODE_SPAN_PRE_CAL:
    case MODE_SPAN2_PRE_CAL:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_SPAN_PUMP_POST, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "SPOFF");
            Log_Expect("PCO2 Mode: SPOFF", LOG_Priority_Low);
        }
        break;
    case MODE_SPAN_POST_CAL:
    case MODE_SPAN2_POST_CAL:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_SPAN_PUMP_POST, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "SPPC");
            Log_Expect("PCO2 Mode: SPPC", LOG_Priority_Low);
        }
        break;
    case MODE_EQUILIBRATE_PUMP_ON:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_EQUIL_PUMP_ON, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "EQON");
            Log_Expect("PCO2 Mode: EQON", LOG_Priority_Low);
        }
        break;
    case MODE_EQUILIBRATE_PUMP_OFF:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_EQUIL_PUMP_OFF, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "EQOFF");
            Log_Expect("PCO2 Mode: EQOFF", LOG_Priority_Low);
        }
        break;
    case MODE_AIR_PUMP_ON:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_AIR_PUMP_ON, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "ARON");
            Log_Expect("PCO2 Mode: ARON", LOG_Priority_Low);
        }
        break;
    case MODE_AIR_PUMP_OFF:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_AIR_PUMP_OFF, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "AROFF");
            Log_Expect("PCO2 Mode: AROFF", LOG_Priority_Low);
        }
        break;
    case MODE_REST:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_REST, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "REST");
            Log_Expect("PCO2 Mode: REST", LOG_Priority_Low);
        }
        break;
    case MODE_DEPLOY:
        FLOW_SetMode_ExpectAndReturn(FLOW_STATE_DEPLOY, status);
        if (status == FLOW_OK)
        {
            strcpy(mstr, "DPLY");
            Log_Expect("PCO2 Mode: DPLY", LOG_Priority_Low);
        }
        break;

    default:
        Error_Expect(ERROR_MSG_0122, LOG_Priority_Low);
        break;
    }

    if (status == FLOW_FAIL)
    {
        Log_Expect("PCO2 Mode: FAIL", LOG_Priority_Low);
    }
    // char temp[24];
    // if(status == FLOW_OK){
    //     // strcat(temp,mstr);
    //     printf("%s\n",mstr);
    //     // sprintf(temp,"PCO2 Mode: %s",mstr);
    //     strcpy(temp,"PCO2 Mode: ");
    //     strcat(temp,mstr);
    //     printf("%s\n",temp);
    //     Log_Expect(temp,LOG_Priority_Low);
    // } else {
    //     sprintf(temp,"PCO2 Mode: FAIL");
    //     Log_Expect(temp,LOG_Priority_Low);
    // }
}
void test_PCO2_State_Rest_should_ShutLicorOFF_and_Sleep(void)
{
    /* Arrange */
    char mstr[8];

    /* Expect */
    Log_Expect("PCO2 Rest", LOG_Priority_Low);
    mock_setmode(MODE_REST, FLOW_OK, mstr);
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_REST,FLOW_OK);
    LI8x0_PowerOFF_Expect();

    /* Act */
    PCO2_State_Rest();
}

void test_PCO2_State_Rest_should_ReportError_but_still_shut_Licor(void)
{
    /* Arrange */
    char mstr[8];
    /* Expect */
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_REST,FLOW_FAIL);
    Log_Expect("PCO2 Rest", LOG_Priority_Low);
    mock_setmode(MODE_REST, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0120, LOG_Priority_Med);
    LI8x0_PowerOFF_Expect();

    /* Act */
    PCO2_State_Rest();
}

void test_PCO2_Calibrate_should_Fail_if_CalState_IsBad(void)
{
    /* Arrange */
    uint8_t cal = 3;

    /* Expect */
    Log_Expect("PCO2 Cal: FAIL", LOG_Priority_Low);
    Error_Expect(ERROR_MSG_0130, LOG_Priority_Low);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Calibrate_Zero_and_ReturnValid(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_ZERO;

    /* Expect */
    Log_Expect("PCO2 Cal: Zero", LOG_Priority_Low);
    LI8x0_Zero_ExpectAndReturn(LICOR_OK);
    DATALOGGER_Write_LicorCal_Expect(pco2_str);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Fail_if_Default_ZeroFails(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_ZERO;

    /* Expect */
    Log_Expect("PCO2 Cal: Zero", LOG_Priority_Low);
    LI8x0_Zero_ExpectAndReturn(LICOR_FAIL);
    Error_Expect(ERROR_MSG_0131, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Calibrate_Span_and_ReturnValid(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_SPAN;

    /* Expect */
    Log_Expect("PCO2 Cal: Span", LOG_Priority_Low);
    LI8x0_Span_ExpectAndReturn(LICOR_OK);
    DATALOGGER_Write_LicorCal_Expect(pco2_str);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Fail_if_Default_SpanFails(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_SPAN;

    /* Expect */
    Log_Expect("PCO2 Cal: Span", LOG_Priority_Low);
    LI8x0_Span_ExpectAndReturn(LICOR_FAIL);
    Error_Expect(ERROR_MSG_0132, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Calibrate_Span2_and_ReturnValid(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_SPAN2;

    /* Expect */
    Log_Expect("PCO2 Cal: Span2", LOG_Priority_Low);
    LI8x0_Span2_ExpectAndReturn(LICOR_OK);
    DATALOGGER_Write_LicorCal_Expect(pco2_str);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Calibrate(cal));
}

void test_PCO2_Calibrate_should_Fail_if_Default_Span2Fails(void)
{
    /* Arrange */
    ePCO2Status_t cal = PCO2_CAL_SPAN2;

    /* Expect */
    Log_Expect("PCO2 Cal: Span2", LOG_Priority_Low);
    LI8x0_Span2_ExpectAndReturn(LICOR_FAIL);
    Error_Expect(ERROR_MSG_0133, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Calibrate(cal));
}

void mock_sample(uint16_t o2data, sLicorData_t *li_data,
                 sRHData_t *rh_data, char *dl_temp, uint16_t runNum, char *mode)
{
    Log_Expect("PCO2 Substate: Sample", LOG_Priority_Low);
    LI8x0_ClearBuffer_Expect();

    STATS_Clear_Expect(&pco2_li_co2);
    STATS_Clear_Expect(&pco2_li_temp);
    STATS_Clear_Expect(&pco2_li_pres);
    STATS_Clear_Expect(&pco2_o2_o2);
    STATS_Clear_Expect(&pco2_rh_rh);
    STATS_Clear_Expect(&pco2_rh_temp);
    if (true == rh_data->has_pres)
    {
        STATS_Clear_Expect(&pco2_rh_pres);
    }
    uint16_t i;

    for (i = 0; i < runNum * 2; i++)
    {
        LI8x0_HasSample_ExpectAndReturn(true);

        LI8x0_ReadSample_Expect(li_data);
        LI8x0_ReadSample_IgnoreArg_data();
        LI8x0_ReadSample_ReturnThruPtr_data(li_data);

        MAX250_Read_ExpectAndReturn(o2data);

        RH_Read_Expect(rh_data);
        RH_Read_IgnoreArg_data();
        RH_Read_ReturnThruPtr_data(rh_data);

        DATALOGGER_Write_SampleRaw_Expect(mode, strlen(mode), dl_temp, strlen(dl_temp));

        STATS_Add_Expect(&pco2_li_co2, li_data->co2);
        STATS_Add_Expect(&pco2_li_temp, li_data->celltemp);
        STATS_Add_Expect(&pco2_li_pres, li_data->cellpres);
        STATS_Add_Expect(&pco2_o2_o2, o2data);
        STATS_Add_Expect(&pco2_rh_rh, rh_data->rh);
        STATS_Add_Expect(&pco2_rh_temp, rh_data->temp);
        if (true == rh_data->has_pres)
        {
            STATS_Add_Expect(&pco2_rh_pres, rh_data->pres);
        }
    }

    STATS_Compute_Expect(&pco2_li_co2);
    STATS_Compute_Expect(&pco2_li_temp);
    STATS_Compute_Expect(&pco2_li_pres);
    STATS_Compute_Expect(&pco2_o2_o2);
    STATS_Compute_Expect(&pco2_rh_rh);
    STATS_Compute_Expect(&pco2_rh_temp);
    if (true == rh_data->has_pres)
    {
        STATS_Compute_Expect(&pco2_rh_pres);
    }
    return;
}

void test_PCO2_State_Sample_Should_Sample_for_Duration_and_ReturnValid(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    pco2.sample_timeout = &temp;
    uint8_t i = 0;
    float o2data = 7.3;
    char mode[] = "Mode1";
    strcpy(pco2.mode, mode);

    sLicorData_t li_data = {
        .co2 = 1.0,
        .co2abs = 1.1,
        .h2o = 0.0,
        .h2odewpoint = 0.0,
        .h2oabs = 0.0,
        .celltemp = 2.0,
        .cellpres = 3.0,
        .ivolt = 4.0,
        .flowrate = 5.0,
        .raw = {
            .co2 = 1,
            .co2ref = 2,
            .h2o = 3,
            .h2oref = 4}

    };
    sRHData_t rh_data = {.rh = 3.2,
                         .temp = 7.1,
                         .pres = 0.5,
                         .has_pres = false};

    char dl_temp[] = "1.000,2.000,3.000,1,2,3.200,7.100";

    /* Expect */
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, mode);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Sample());
}

void mock_calibrate_zero_success(void)
{
    Log_Expect("PCO2 Cal: Zero", LOG_Priority_Low);
    LI8x0_Zero_ExpectAndReturn(LICOR_OK);
    DATALOGGER_Write_LicorCal_Expect(pco2_str);
}

void mock_calibrate_zero_fail(void)
{
    Log_Expect("PCO2 Cal: Zero", LOG_Priority_Low);
    LI8x0_Zero_ExpectAndReturn(LICOR_FAIL);
    Error_Expect(ERROR_MSG_0131, LOG_Priority_High);
}

void test_PCO2_Zero_should_RunState_and_ReturnValid(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {
        .co2 = 3.0,
        .co2abs = 1.1,
        .h2o = 0.0,
        .h2odewpoint = 0.0,
        .h2oabs = 0.0,
        .celltemp = 5.0,
        .cellpres = 9.0,
        .ivolt = 4.0,
        .flowrate = 5.0,
        .raw = {
            .co2 = 89,
            .co2ref = 45,
            .h2o = 3,
            .h2oref = 4}

    };
    sRHData_t rh_data = {.rh = 6.1,
                         .temp = 3.4,
                         .pres = 5.2,
                         .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_OK);
    Log_Expect("PCO2 Zero", LOG_Priority_Low);
    mock_setmode(MODE_ZERO_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEON");
    mock_setmode(MODE_ZERO_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_ZERO_PRE_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEOFF");
    mock_calibrate_zero_success();
    mock_setmode(MODE_ZERO_POST_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEPC");

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Zero());
}

void test_PCO2_Zero_should_RunState_and_Fail_If_BadZEON_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];

    /* Expect */
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_FAIL);
    Log_Expect("PCO2 Zero", LOG_Priority_Low);
    mock_setmode(MODE_ZERO_PUMP_ON, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0150, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Zero());
}

void test_PCO2_Zero_should_RunState_and_Fail_If_BadZEOFF_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];

    /* Expect */
    Log_Expect("PCO2 Zero", LOG_Priority_Low);
    mock_setmode(MODE_ZERO_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEON");
    mock_setmode(MODE_ZERO_PUMP_OFF, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0152, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Zero());
}

void test_PCO2_Zero_should_RunState_and_Fail_If_BadZEOFF2_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    Log_Expect("PCO2 Zero", LOG_Priority_Low);
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_OK);
    mock_setmode(MODE_ZERO_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEON");
    mock_setmode(MODE_ZERO_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_ZERO_PRE_CAL, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0153, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Zero());
}

void test_PCO2_Zero_should_RunState_and_Fail_If_BadCAL(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    Log_Expect("PCO2 Zero", LOG_Priority_Low);
    mock_setmode(MODE_ZERO_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEON");
    mock_setmode(MODE_ZERO_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_ZERO_PRE_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ZEOFF");
    mock_calibrate_zero_fail();
    Error_Expect(ERROR_MSG_0155, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Zero());
}

void mock_calibrate_span_success(void)
{
    Log_Expect("PCO2 Cal: Span", LOG_Priority_Low);
    LI8x0_Span_ExpectAndReturn(LICOR_OK);
    DATALOGGER_Write_LicorCal_Expect(pco2_str);
}

void mock_calibrate_span_fail(void)
{
    Log_Expect("PCO2 Cal: Span", LOG_Priority_Low);
    LI8x0_Span_ExpectAndReturn(LICOR_FAIL);
    Error_Expect(ERROR_MSG_0132, LOG_Priority_High);
}

void test_PCO2_Span_should_RunState_and_ReturnValid(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {
        .co2 = 3.0,
        .co2abs = 1.1,
        .h2o = 0.0,
        .h2odewpoint = 0.0,
        .h2oabs = 0.0,
        .celltemp = 5.0,
        .cellpres = 9.0,
        .ivolt = 4.0,
        .flowrate = 5.0,
        .raw = {
            .co2 = 89,
            .co2ref = 45,
            .h2o = 3,
            .h2oref = 4}

    };
    sRHData_t rh_data = {.rh = 6.1,
                         .temp = 3.4,
                         .pres = 5.2,
                         .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_OK);
    Log_Expect("PCO2 Span", LOG_Priority_Low);
    mock_setmode(MODE_SPAN_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPON");
    mock_setmode(MODE_SPAN_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_SPAN_PRE_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPOFF");
    mock_calibrate_span_success();
    mock_setmode(MODE_SPAN_POST_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPPC");

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Span());
}

void test_PCO2_Span_should_RunState_and_Fail_If_BadZEON_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];

    /* Expect */
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_FAIL);
    Log_Expect("PCO2 Span", LOG_Priority_Low);
    mock_setmode(MODE_SPAN_PUMP_ON, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0160, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Span());
}

void test_PCO2_Span_should_RunState_and_Fail_If_BadZEOFF_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];

    /* Expect */
    Log_Expect("PCO2 Span", LOG_Priority_Low);
    mock_setmode(MODE_SPAN_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPON");
    mock_setmode(MODE_SPAN_PUMP_OFF, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0162, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Span());
}

void test_PCO2_Span_should_RunState_and_Fail_If_BadZEOFF2_Set(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    Log_Expect("PCO2 Span", LOG_Priority_Low);
    // FLOW_SetMode_ExpectAndReturn(FLOW_STATE_ZERO_PUMP_ON,FLOW_OK);
    mock_setmode(MODE_SPAN_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPON");
    mock_setmode(MODE_SPAN_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_SPAN_PRE_CAL, FLOW_FAIL, mstr);
    Error_Expect(ERROR_MSG_0163, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Span());
}

void test_PCO2_Span_should_RunState_and_Fail_If_BadCAL(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    char mode[8] = "ZEON";

    float o2data = 7.3;
    sLicorData_t li_data = {.co2 = 3.0,
                            .co2abs = 1.1,
                            .h2o = 0.0,
                            .h2odewpoint = 0.0,
                            .h2oabs = 0.0,
                            .celltemp = 5.0,
                            .cellpres = 9.0,
                            .ivolt = 4.0,
                            .flowrate = 5.0,
                            .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    Log_Expect("PCO2 Span", LOG_Priority_Low);
    mock_setmode(MODE_SPAN_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPON");
    mock_setmode(MODE_SPAN_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_SPAN_PRE_CAL, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "SPOFF");
    mock_calibrate_span_fail();
    Error_Expect(ERROR_MSG_0165, LOG_Priority_High);

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_FAIL, PCO2_State_Span());
}

void test_PCO2_State_Equilibrate_should_RunState_and_ReturnValid(void)
{

    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    float o2data = 7.3;
    sLicorData_t li_data = {
        .co2 = 3.0,
        .co2abs = 1.1,
        .h2o = 0.0,
        .h2odewpoint = 0.0,
        .h2oabs = 0.0,
        .celltemp = 5.0,
        .cellpres = 9.0,
        .ivolt = 4.0,
        .flowrate = 5.0,
        .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];
    /* Expect */
    Log_Expect("PCO2 Equilibrate", LOG_Priority_Low);
    mock_setmode(MODE_EQUILIBRATE_PUMP_ON, FLOW_OK, mstr);
    LI8x0_PowerOFF_Expect();
    LI8x0_PowerON_Expect();
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "EQON");
    mock_setmode(MODE_EQUILIBRATE_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_AIR_PUMP_OFF, FLOW_OK, mstr);
    mock_setmode(MODE_EQUILIBRATE_PUMP_OFF, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "EQOFF");

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Equilibrate());
}

void test_PCO2_State_Air_should_RunState_and_ReturnValid(void)
{
    /* Arrange */
    uint16_t runNum = 5;
    pco2.sampleCO2 = runNum;
    uint16_t temp = 0;
    uint16_t temp2 = 0;
    pco2.timeout1 = &temp;
    pco2.timeout2 = &temp2;
    uint16_t samp_temp;
    pco2.sample_timeout = &samp_temp;
    float o2data = 7.3;
    sLicorData_t li_data = {
        .co2 = 3.0,
        .co2abs = 1.1,
        .h2o = 0.0,
        .h2odewpoint = 0.0,
        .h2oabs = 0.0,
        .celltemp = 5.0,
        .cellpres = 9.0,
        .ivolt = 4.0,
        .flowrate = 5.0,
        .raw = {.co2 = 89, .co2ref = 45, .h2o = 3, .h2oref = 4}};
    sRHData_t rh_data = {.rh = 6.1, .temp = 3.4, .pres = 5.2, .has_pres = false};

    char dl_temp[] = "3.000,5.000,9.000,89,45,6.100,3.400";
    char mstr[8];

    /* Expect */
    Log_Expect("PCO2 State: Air", LOG_Priority_Low);
    mock_setmode(MODE_AIR_PUMP_ON, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "ARON");
    mock_setmode(MODE_AIR_PUMP_OFF, FLOW_OK, mstr);
    mock_sample(o2data, &li_data, &rh_data, dl_temp, runNum, "AROFF");

    /* Act */
    TEST_ASSERT_EQUAL(PCO2_OK, PCO2_State_Air());
}

void test_PCO2_Init_should_InitializeAll_and_ReturnValid(void)
{
    /* Arrange */

    /* Expect */
    Log_Expect("PCO2 Init", LOG_Priority_High);
    LI8x0_Init_ExpectAndReturn(LICOR_OK);
    FLOW_Init_ExpectAndReturn(FLOW_OK);
    MAX250_Init_Expect();
    RH_Init_ExpectAndReturn(RH_OK);
    DATALOGGER_Init_ExpectAndReturn(DL_OK);

    /* Act */
    PCO2_Initialize();
}

void test_PCO2_FastMode_CalculateRepeats_should_ReturnValidValue(void)
{
    /* Arrange */
    uint16_t i;
    /* Expect */
    for (i = 0; i < 6; i++)
    {
        AM08X5_compare_time_IgnoreAndReturn(true);
        AM08X5_subtract_time_struct_IgnoreAndReturn(AM08X5_OK);
    }
    AM08X5_compare_time_IgnoreAndReturn(false);

    /* Act */
    TEST_ASSERT_EQUAL(6, PCO2_FastMode_CalculateRepeats());
}

// void test_PCO2_GetRunLength_should_returnValid_RunLength(void){
//     /* Arrange */
//     pco2.
//     /* Expect */

//     /* Act */

// }

void test_PCO2_GetNormalInterval_should_return_NormalInterval(void)
{
    /* Arrange */
    sTimeReg_t t;

    pco2.NormInterval.year = 0;
    pco2.NormInterval.month = 0;
    pco2.NormInterval.date = 0;
    pco2.NormInterval.hour = 3;
    pco2.NormInterval.minute = 0;
    pco2.NormInterval.second = 0;

    /* Expect */
    AM08X5_clear_time_struct_Expect(&t);
    AM08X5_clear_time_struct_ReturnThruPtr_t(&pco2.NormInterval);

    /* Act */
    PCO2_GetNormalInterval(&t);
    TEST_ASSERT_EQUAL(pco2.NormInterval.year, t.year);
    TEST_ASSERT_EQUAL(pco2.NormInterval.month, t.month);
    TEST_ASSERT_EQUAL(pco2.NormInterval.date, t.date);
    TEST_ASSERT_EQUAL(pco2.NormInterval.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.NormInterval.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.NormInterval.second, t.second);
}

void test_PCO2_GetNormalStartTime_should_return_StartTime(void)
{
    /* Arrange */
    sTimeReg_t t;

    pco2.NormStart.year = 18;
    pco2.NormStart.month = 5;
    pco2.NormStart.date = 3;
    pco2.NormStart.hour = 18;
    pco2.NormStart.minute = 0;
    pco2.NormStart.second = 0;

    /* Expect */
    AM08X5_clear_time_struct_Expect(&t);
    AM08X5_clear_time_struct_ReturnThruPtr_t(&pco2.NormStart);

    /* Act */
    PCO2_GetNormalStartTime(&t);
    TEST_ASSERT_EQUAL(pco2.NormStart.year, t.year);
    TEST_ASSERT_EQUAL(pco2.NormStart.month, t.month);
    TEST_ASSERT_EQUAL(pco2.NormStart.date, t.date);
    TEST_ASSERT_EQUAL(pco2.NormStart.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.NormStart.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.NormStart.second, t.second);
}

void test_PCO2_GetFastInterval_should_return_FastInterval(void)
{
    /* Arrange */
    sTimeReg_t t;

    pco2.FastInterval.year = 0;
    pco2.FastInterval.month = 0;
    pco2.FastInterval.date = 0;
    pco2.FastInterval.hour = 0;
    pco2.FastInterval.minute = 45;
    pco2.FastInterval.second = 0;

    /* Expect */
    AM08X5_clear_time_struct_Expect(&t);
    AM08X5_clear_time_struct_ReturnThruPtr_t(&pco2.FastInterval);

    /* Act */
    PCO2_GetFastInterval(&t);
    TEST_ASSERT_EQUAL(pco2.FastInterval.year, t.year);
    TEST_ASSERT_EQUAL(pco2.FastInterval.month, t.month);
    TEST_ASSERT_EQUAL(pco2.FastInterval.date, t.date);
    TEST_ASSERT_EQUAL(pco2.FastInterval.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.FastInterval.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.FastInterval.second, t.second);
}

void test_PCO2_GetChange_should_return_StartTime(void)
{
    /* Arrange */
    sTimeReg_t t;

    pco2.FastChange.year = 0;
    pco2.FastChange.month = 0;
    pco2.FastChange.date = 0;
    pco2.FastChange.hour = 20;
    pco2.FastChange.minute = 0;
    pco2.FastChange.second = 0;

    /* Expect */
    AM08X5_clear_time_struct_Expect(&t);
    AM08X5_clear_time_struct_ReturnThruPtr_t(&pco2.FastChange);

    /* Act */
    PCO2_GetChange(&t);
    TEST_ASSERT_EQUAL(pco2.FastChange.year, t.year);
    TEST_ASSERT_EQUAL(pco2.FastChange.month, t.month);
    TEST_ASSERT_EQUAL(pco2.FastChange.date, t.date);
    TEST_ASSERT_EQUAL(pco2.FastChange.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.FastChange.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.FastChange.second, t.second);
}

void test_PCO2_GetSample_should_return_samplefrequency(void)
{
    /* Arrange */
    pco2.sample = 4;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(4, PCO2_GetSample());
}

void test_PCO2_GetEquil_should_return_equil(void)
{
    /* Arrange */
    pco2.equil.equil = 458;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(458, PCO2_GetEquil());
}

void test_PCO2_GetWarmup_should_return_warmuptime(void)
{
    /* Arrange */
    pco2.LI_warmup = 300;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(300, PCO2_GetWarmup());
}

void test_PCO2_GetPumpON_should_return_pumpon_time(void)
{
    /* Arrange */
    pco2.pump.pumpon = 56;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(56, PCO2_GetPumpON());
}

void test_PCO2_GetPumpOFF_should_return_pumpoff_time(void)
{
    /* Arrange */
    pco2.pump.pumpoff = 32;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(32, PCO2_GetPumpOFF());
}

void test_PCO2_GetSampleCO2_should_return_sampleco2_time(void)
{
    /* Arrange */
    pco2.sampleCO2 = 15;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(15, PCO2_GetSampleCO2());
}

void test_PCO2_Vent_should_return_vent_time(void)
{
    /* Arrange */
    pco2.vent = 11;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(11, PCO2_GetVent());
}

void test_PCO2_Heater_should_return_heater_state(void)
{
    /* Arrange */
    pco2.LI_heater = 1;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(1, PCO2_GetHeater());
}

void test_PCO2_GetSpanDiff_should_return_spandiff(void)
{
    /* Arrange */
    pco2.spandiff = 4.37;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL_FLOAT(4.37, PCO2_GetSpanDiff());
}

void test_PCO2_GetSpan_should_return_spandiff(void)
{
    /* Arrange */
    pco2.span = 5.76;
    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL_FLOAT(5.76, PCO2_GetSpan());
}

void test_PCO2_SetNormalInterval_should_set_pco2_struct(void)
{
    /* Arrange */
    sTimeReg_t t = {.year = 0,
                    .month = 0,
                    .date = 0,
                    .hour = 2,
                    .minute = 0,
                    .second = 0};
    /* Expect */

    /* Act */
    PCO2_SetNormalInterval(&t);
    TEST_ASSERT_EQUAL(pco2.NormInterval.year, t.year);
    TEST_ASSERT_EQUAL(pco2.NormInterval.month, t.month);
    TEST_ASSERT_EQUAL(pco2.NormInterval.date, t.date);
    TEST_ASSERT_EQUAL(pco2.NormInterval.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.NormInterval.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.NormInterval.second, t.second);
}

void test_PCO2_SetChange_should_set_pco2_struct(void)
{
    /* Arrange */
    sTimeReg_t t = {.year = 0,
                    .month = 0,
                    .date = 1,
                    .hour = 0,
                    .minute = 0,
                    .second = 0};
    /* Expect */

    /* Act */
    PCO2_SetChange(&t);
    TEST_ASSERT_EQUAL(pco2.FastChange.year, t.year);
    TEST_ASSERT_EQUAL(pco2.FastChange.month, t.month);
    TEST_ASSERT_EQUAL(pco2.FastChange.date, t.date);
    TEST_ASSERT_EQUAL(pco2.FastChange.hour, t.hour);
    TEST_ASSERT_EQUAL(pco2.FastChange.minute, t.minute);
    TEST_ASSERT_EQUAL(pco2.FastChange.second, t.second);
}

void test_PCO2_SetSample_should_set_pco2_struct(void)
{
    /* Arrange */
    uint8_t sample = 4;
    /* Expect */

    /* Act */
    PCO2_SetSample(sample);
    TEST_ASSERT_EQUAL(sample, pco2.sample);
}

void test_PCO2_SetSample_invalid_entry_should_not_set_pco2_struct(void)
{
    /* Arrange */
    pco2.sample = 2;
    uint8_t sample = 5;
    /* Expect */

    /* Act */
    PCO2_SetSample(sample);
    TEST_ASSERT_EQUAL(2, pco2.sample);
}

void test_PCO2_SetSpanDiff_should_set_pco2_struct(void)
{
    /* Arrange */
    float val = 1.732;
    /* Expect */

    /* Act */
    PCO2_SetSpanDiff(val);
    TEST_ASSERT_EQUAL_FLOAT(val, pco2.spandiff);
}

void test_PCO2_SetSpan_should_set_pco2_struct(void)
{
    /* Arrange */
    float val = 499.101;
    /* Expect */

    /* Act */
    PCO2_SetSpan(val);
    TEST_ASSERT_EQUAL_FLOAT(val, pco2.span);
}

void test_PCO2_SetEquil_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 999;
    /* Expect */

    /* Act */
    PCO2_SetEquil(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.equil.equil);
}

void test_PCO2_SetWarmup_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 888;
    /* Expect */

    /* Act */
    PCO2_SetWarmup(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.LI_warmup);
}

void test_PCO2_SetPumpON_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 555;
    /* Expect */

    /* Act */
    PCO2_SetPumpON(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.pump.pumpon);
}

void test_PCO2_SetPumpOFF_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 777;
    /* Expect */

    /* Act */
    PCO2_SetPumpOFF(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.pump.pumpoff);
}

void test_PCO2_SampleCO2_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 666;
    /* Expect */

    /* Act */
    PCO2_SetSampleCO2(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.sampleCO2);
}

void test_PCO2_SetVent_should_set_pco2_struct(void)
{
    /* Arrange */
    uint32_t seconds = 444;
    /* Expect */

    /* Act */
    PCO2_SetVent(seconds);
    TEST_ASSERT_EQUAL(seconds, pco2.vent);
}

void test_PCO2_SetHeater_ON_should_set_heater_state_and_return(void)
{
    /* Arrange */
    uint8_t heaterstate = 1;

    /* Expect */
    LI8x0_Heater_ExpectAndReturn(heaterstate,LICOR_OK);

    /* Act */
    PCO2_SetHeater(heaterstate);
    TEST_ASSERT_EQUAL(heaterstate, pco2.LI_heater);
}

/* Arrange */

/* Expect */

/* Act */

/* Arrange */

/* Expect */

/* Act */