#include "unity.h"
#include "motioncontrol.h"
// #include "mock_motioncontrol.h"
#include "mock_DRV8823.h"
#include "mock_relay.h"
// #include "DRV8823.h"
// extern sMotionSetting_t ZeroPumpOn_State_Default;

STATIC PERSISTENT sMotionControl_t Test = {
    .DRV_A = {
        .Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode=0},
        .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode=0},
        .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode=0},
        .cs = {.port = SYS_MCONTROL_BRIDGE_A_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_A_CS_PIN, .mode=0},
        .SpiChan = { .param = {
                        .selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
                        .clockSourceFrequency = SYS_MCONTROL_SPI_CLK_FREQ,
                        .desiredSpiClock = SYS_MCONTROL_SPI_BAUD,
                        .msbFirst = SYS_MCONTROL_SPI_MSBFIRST,
                        .clockPhase = SYS_MCONTROL_SPI_CLK_PHASE,
                        .clockPolarity = SYS_MCONTROL_SPI_CLK_POL,
                        .spiMode = SYS_MCONTROL_SPI_MODE
                        },
                    .baseAddress = SYS_MCONTROL_BRIDGE_A_SPI_BASE,
                    .slaveAddress = 0,
                    .mode = 0,
                    .configured = 0,
                    .enabled = 0,
                    .interrupt = 0}
    },
    .DRV_B = {
        .Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode=0},
        .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode=0},
        .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode=0},
        .cs = {.port = SYS_MCONTROL_BRIDGE_B_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_B_CS_PIN, .mode=0},
        .SpiChan = { .param = {
                        .selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
                        .clockSourceFrequency = SYS_MCONTROL_SPI_CLK_FREQ,
                        .desiredSpiClock = SYS_MCONTROL_SPI_BAUD,
                        .msbFirst = SYS_MCONTROL_SPI_MSBFIRST,
                        .clockPhase = SYS_MCONTROL_SPI_CLK_PHASE,
                        .clockPolarity = SYS_MCONTROL_SPI_CLK_POL,
                        .spiMode = SYS_MCONTROL_SPI_MODE
                        },
                    .baseAddress = SYS_MCONTROL_BRIDGE_A_SPI_BASE,
                    .slaveAddress = 0,
                    .mode = 0,
                    .configured = 0,
                    .enabled = 0,
                    .interrupt = 0}
    }
};
void setUp(void)
{
    // State[0] = (sMotionState_t){.V1={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V2={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V3={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V4={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V5={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V6={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V7={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.V8={.Dir=HBRIDGE_FWD,.State=HBRIDGE_ON,.time_ms=50}};
    // State[0] = (sMotionState_t){.Pump={.State=PUMP_ON}};
    // State[0] = (sMotionState_t){.next=&State[1]};

}

void tearDown(void)
{
}

// void test_MCONTROL_SetState_should_ReturnValid(void){ 
//         // printf("%u,%u\n",State[0].V1.Dir,State[0].V1.State);   
//         // printf("%d and %d\n",State[0].next,&State[1]);
//         TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_SetState(&State[0]));
// }

void test_MCONTROL_Init_should_Initialize_the_HBridgeICs(void){

    DRV8823_Init_Expect(&Motion.DRV_A);
    DRV8823_Init_Expect(&Motion.DRV_B);
    RELAY_Init_Ignore();
    
    MCONTROL_Init();

}



void run_mcontrol_set(uint8_t enable, eMotionStatus_t result1,eMotionStatus_t result2,eMotionStatus_t result3){
    DRV8823_ClearSleepMode_Ignore();
    DRV8823_CreateDatafield_IgnoreAndReturn(result1);
    DRV8823_Set_AllChannels_IgnoreAndReturn(result2);
    DRV8823_Set_AllChannels_IgnoreAndReturn(result3);
    DRV8823_Reset_Ignore();
    DRV8823_SetSleepMode_Ignore();

}

void test_MCONTROL_Set_should_SetDRV8823_with_CorrectValues(void){
    sMotionSetting_t state = { .Valve[0] = {.dir = 0, .current = 1u, .decay =1},
                               .Valve[1] = {.dir = 0, .current = 2u, .decay =1},
                               .Valve[2] = {.dir = 0, .current = 3u, .decay =1},
                               .Valve[3] = {.dir = 0, .current = 4u, .decay =1},                             
                               .Valve[4] = {.dir = 1, .current = 5u, .decay =0},
                               .Valve[5] = {.dir = 1, .current = 6u, .decay =0},
                               .Valve[6] = {.dir = 1, .current = 7u, .decay =0},
                               .Valve[7] = {.dir = 1, .current = 0u, .decay =0},         
                               .Pump = PUMP_ON };

    /** Setup the Valve Addresses */
    Test.Valve[0].Bridge = DRV_A;
    Test.Valve[0].Chan = DRV_CHAN_A;
    Test.Valve[1].Bridge = DRV_A;
    Test.Valve[1].Chan = DRV_CHAN_B;
    Test.Valve[2].Bridge = DRV_A;
    Test.Valve[2].Chan = DRV_CHAN_C;
    Test.Valve[3].Bridge = DRV_A;
    Test.Valve[3].Chan = DRV_CHAN_D;
    Test.Valve[4].Bridge = DRV_B;
    Test.Valve[4].Chan = DRV_CHAN_A;
    Test.Valve[5].Bridge = DRV_B;
    Test.Valve[5].Chan = DRV_CHAN_B;
    Test.Valve[6].Bridge = DRV_B;
    Test.Valve[6].Chan = DRV_CHAN_C;
    Test.Valve[7].Bridge = DRV_B;
    Test.Valve[7].Chan = DRV_CHAN_D;


    sDRV8823SetVal_t OutputA = { .outA = {.Phase=0,.Current=1,.Decay=1},
                                .outB = {.Phase=0,.Current=2,.Decay=1},
                                .outC = {.Phase=0,.Current=3,.Decay=1},
                                .outD = {.Phase=0,.Current=4,.Decay=1}};
    sDRV8823SetVal_t OutputB = { .outA = {.Phase=1,.Current=5,.Decay=0},
                                .outB = {.Phase=1,.Current=6,.Decay=0},
                                .outC = {.Phase=1,.Current=7,.Decay=0},
                                .outD = {.Phase=1,.Current=0,.Decay=0}};

    uint8_t enable = 1;
    // DRV8823_CreateDatafield_IgnoreAndReturn(DRV8823_OK);
    // DRV8823_Set_AllChannels_IgnoreAndReturn(DRV8823_OK);
    // DRV8823_Set_AllChannels_IgnoreAndReturn(DRV8823_OK);

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK,DRV8823_OK);

    TEST_ASSERT_EQUAL(MCONTROL_OK, MCONTROL_Set(&Test,&state));
    TEST_ASSERT_EQUAL_MEMORY(&state,&Test.Current,sizeof(state));

    printf("State: Valve[0]= %u, %u, %u\n",state.Valve[0].dir,state.Valve[0].current,state.Valve[0].decay);
    printf("DRVer: Valve[0]= %u, %u, %u\n",Test.Valve[0].State.dir,Test.Valve[0].State.current,Test.Valve[0].State.decay);
    

    // TEST_ASSERT_EQUAL_MEMORY(&OutputA,&Test.DRV_A.Send,sizeof(OutputA));
    // TEST_ASSERT_EQUAL_MEMORY(&state.Valve[0],&Test.Valve[0].State,sizeof(state.Valve[0]));

}








void test_MCONTROL_PCO2_Setstate_Zero_PumpOn_should_Call_the_CorrectFunction(void){
    ePCO2Dwell_t pco2State = PCO2_Normal;
    ePumpStates_t pumpState = PUMP_ON;
    uint8_t enable =1;
    // MCONTROL_Set_IgnoreAndReturn(MCONTROL_OK);
    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK,DRV8823_OK);
    RELAY_On_Ignore();
    TEST_ASSERT_EQUAL(MCONTROL_OK, MCONTROL_PCO2_Set(&ZeroPumpOn_State_Default,pco2State,pumpState));
}


void test_MCONTROL_PCO2_Set_should_Fail_if_MControlSet_Fails(void){
    ePCO2Dwell_t pco2State = PCO2_Normal;
    ePumpStates_t pumpState = PUMP_ON;
    uint8_t enable =1;

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_FAIL);
    TEST_ASSERT_EQUAL(MCONTROL_FAIL, MCONTROL_PCO2_Set(&ZeroPumpOn_State_Default,pco2State,pumpState));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_FAIL, DRV8823_OK);
    TEST_ASSERT_EQUAL(MCONTROL_FAIL, MCONTROL_PCO2_Set(&ZeroPumpOn_State_Default,pco2State,pumpState));

    run_mcontrol_set(enable, DRV8823_FAIL, DRV8823_OK, DRV8823_OK);
    TEST_ASSERT_EQUAL(MCONTROL_FAIL, MCONTROL_PCO2_Set(&ZeroPumpOn_State_Default,pco2State,pumpState));


}


// void setmode()

void test_MCONTROL_PCO2_SetMode_should_Set_CorrectMode_and_ReturnValid(void){
    DRV8823_Init_Ignore();
    RELAY_Init_Expect();
    MCONTROL_Init();

    uint8_t enable = 1;
    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_On_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_ZERO_PUMP_ON));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_ZERO_PUMP_OFF));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_ZERO_CAL));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_On_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_SPAN_PUMP_ON));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_SPAN_PUMP_OFF));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_SPAN_CAL));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_On_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_EQUIL_PUMP_ON));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_EQUIL_PUMP_OFF));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_On_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_AIR_PUMP_ON));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_AIR_PUMP_OFF));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_REST));

    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_OK);
    RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetMode(PCO2_DEPLOYMENT));
}


void test_MCONTROL_PCO2_SetMode_should_NOTSet_Mode_and_Fail_for_Invalid(void){
    uint8_t enable =1;
    run_mcontrol_set(enable, DRV8823_OK, DRV8823_OK, DRV8823_FAIL);
    // RELAY_Off_Expect();
    TEST_ASSERT_EQUAL(MCONTROL_FAIL,MCONTROL_PCO2_SetMode(PCO2_DEPLOYMENT));
}


void test_MCONTROL_PCO2_SetCurrent_should_AcceptValidValues_and_ReturnValid(void){
    
    uint8_t i;


    for(i=0;i<7;i++){
        TEST_ASSERT_EQUAL(MCONTROL_OK, MCONTROL_PCO2_SetCurrent(i));
        TEST_ASSERT_EQUAL(i,ZeroPumpOn_State.Valve[0].current);
        TEST_ASSERT_EQUAL(i,EquilPumpOn_State.Valve[5].current);
        TEST_ASSERT_EQUAL(i,Deploy_State.Valve[7].current);
    }
}


void test_MCONTROL_PCO2_SetCurrent_should_Fail_for_InvalidInputValues(void){

    TEST_ASSERT_EQUAL(MCONTROL_FAIL,MCONTROL_PCO2_SetCurrent(8));
    TEST_ASSERT_EQUAL(MCONTROL_FAIL,MCONTROL_PCO2_SetCurrent(255));

}


void test_MCONTROL_PCO2_SetPulse_should_AcceptValidValues_and_ReturnValid(void){

    uint32_t testval = 18;
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetPulse(testval));
    TEST_ASSERT_EQUAL(testval,Motion.Delay);

    testval = 512;
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetPulse(testval));
    TEST_ASSERT_EQUAL(testval,Motion.Delay);

    testval = 4096;
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetPulse(testval));
    TEST_ASSERT_EQUAL(testval,Motion.Delay);
}


void test_MCONTROL_PCO2_SetPulse_should_RejectInvalidValues_and_Fail(void){

    uint32_t testval = 18;
    TEST_ASSERT_EQUAL(MCONTROL_OK,MCONTROL_PCO2_SetPulse(testval));
    TEST_ASSERT_EQUAL(testval,Motion.Delay);

    testval = 14;
    TEST_ASSERT_EQUAL(MCONTROL_FAIL,MCONTROL_PCO2_SetPulse(testval));
    TEST_ASSERT_EQUAL(18,Motion.Delay);
}

void test_MCONTROL_PCO2_GetMode_should_ReturnCurrentMode(void){
    Motion.PCO2State = PCO2_SPAN_PUMP_ON;

    TEST_ASSERT_EQUAL(PCO2_SPAN_PUMP_ON,MCONTROL_PCO2_GetMode());

}

void test_MCONTROL_PCO2_GetPulse_should_ReturnDelayPulseMS(void){

    Motion.Delay = 564;
    TEST_ASSERT_EQUAL(564,MCONTROL_PCO2_GetPulse());
}