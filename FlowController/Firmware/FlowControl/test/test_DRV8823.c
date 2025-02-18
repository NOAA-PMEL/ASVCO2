#include "unity.h"
#include "DRV8823.h"
#include "mock_bsp.h"
#include "mock_buffer8.h"

sSPI_t spi = {
    .param = {
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
    .interrupt = 0
};

sDRV8823_t DRV8823_a = {
    .Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode=1},
    .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode=1},
    .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode=1},
    .cs = {.port = SYS_MCONTROL_BRIDGE_A_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_A_CS_PIN, .mode=1},
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
};

sDRV8823_t DRV8823_b = {
    .Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode=1},
    .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode=1},
    .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode=1},
    .cs = {.port = SYS_MCONTROL_BRIDGE_B_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_B_CS_PIN, .mode=1},
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
};

void setUp(void)
{
    DRV8823_a.Last.outA.Decay = 1;
    DRV8823_a.Last.outA.Current = 7;
    DRV8823_a.Last.outA.Phase = 1;
    DRV8823_a.Last.outA.Enable = 1;
    DRV8823_a.Last.outB.Decay = 1;
    DRV8823_a.Last.outB.Current = 7;
    DRV8823_a.Last.outB.Phase = 1;
    DRV8823_a.Last.outB.Enable = 1;
    DRV8823_a.Last.outC.Decay = 1;
    DRV8823_a.Last.outC.Current = 7;
    DRV8823_a.Last.outC.Phase = 1;
    DRV8823_a.Last.outC.Enable = 1;
    DRV8823_a.Last.outD.Decay = 1;
    DRV8823_a.Last.outD.Current = 7;
    DRV8823_a.Last.outD.Phase = 1;
    DRV8823_a.Last.outD.Enable = 1;
    DRV8823_a.Last.dataAB = 0x0FFF;
    DRV8823_a.Last.dataCD = 0x0FFF;


    DRV8823_a.Send.outA.Decay = 0;
    DRV8823_a.Send.outA.Current = 0;
    DRV8823_a.Send.outA.Phase = 0;
    DRV8823_a.Send.outA.Enable = 0;
    DRV8823_a.Send.outB.Decay = 0;
    DRV8823_a.Send.outB.Current = 0;
    DRV8823_a.Send.outB.Phase = 0;
    DRV8823_a.Send.outB.Enable = 0;
    DRV8823_a.Send.outC.Decay = 0;
    DRV8823_a.Send.outC.Current = 0;
    DRV8823_a.Send.outC.Phase = 0;
    DRV8823_a.Send.outC.Enable = 0;
    DRV8823_a.Send.outD.Decay = 0;
    DRV8823_a.Send.outD.Current = 0;
    DRV8823_a.Send.outD.Phase = 0;
    DRV8823_a.Send.outD.Enable =0;
    DRV8823_a.Send.dataAB = 0x0000;
    DRV8823_a.Send.dataCD = 0x0000;

}

void tearDown(void)
{
}

void test_DRV8823_Init_should_SetValidPortsAndPins(void)
{
    BSP_GPIO_Init_Expect(&DRV8823_a.Strobe);
    BSP_GPIO_Init_Expect(&DRV8823_a.nReset);
    BSP_GPIO_Init_Expect(&DRV8823_a.nSleep);
    BSP_GPIO_Init_Expect(&DRV8823_a.cs);
    BSP_SPI_Init_Expect(&DRV8823_a.SpiChan);

    BSP_GPIO_SetPin_Expect(&DRV8823_a.nReset);
    BSP_GPIO_SetPin_Expect(&DRV8823_b.nSleep);


    DRV8823_Init(&DRV8823_a);
}


void test_DRV8823_SetSleepMode_should_SetSleepPin_Low(void){
    BSP_GPIO_ClearPin_Expect(&DRV8823_a.nSleep);
    DRV8823_SetSleepMode(&DRV8823_a);

}

void test_DRV8823_ClearSleepMode_should_ClearSleepPin_High(void){
    BSP_GPIO_SetPin_Expect(&DRV8823_a.nSleep);
    DRV8823_ClearSleepMode(&DRV8823_a);
}

void test_DRV8823_SetReset_should_SetResetPin_Low(void){
    BSP_GPIO_ClearPin_Expect(&DRV8823_a.nReset);
    DRV8823_SetReset(&DRV8823_a);
}

void test_DRV8823_ClearReset_should_ClearResetPin_High(void){
    BSP_GPIO_SetPin_Expect(&DRV8823_a.nReset);
    DRV8823_ClearReset(&DRV8823_a);
}

void test_DRV8823_SetCS_should_SetCSPin_High(void){
    BSP_GPIO_SetPin_Expect(&DRV8823_a.cs);
    DRV8823_SetCS(&DRV8823_a);
}

void test_DRV8823_ClearCS_should_ClearCSPin_Low(void){
    BSP_GPIO_ClearPin_Expect(&DRV8823_a.cs);
    DRV8823_ClearCS(&DRV8823_a);
}

void test_DRV8823_SetStrobe_should_SetStrobePin_High(void){
    BSP_GPIO_SetPin_Expect(&DRV8823_a.Strobe);
    DRV8823_SetStrobe(&DRV8823_a);
}

void test_DRV8823_ClearCS_should_ClearStrobePin_Low(void){
    BSP_GPIO_ClearPin_Expect(&DRV8823_a.Strobe);
    DRV8823_ClearStrobe(&DRV8823_a);
}


void test_DRV8823_Reset_should_ClearPins_SetReset_then_ReleaseReset(void){
    BSP_GPIO_ClearPin_Expect(&DRV8823_b.cs);
    BSP_GPIO_ClearPin_Expect(&DRV8823_b.Strobe);
    BSP_GPIO_SetPin_Expect(&DRV8823_b.nSleep);
    BSP_SPI_Disable_Expect(&DRV8823_b.SpiChan);

    BSP_GPIO_ClearPin_Expect(&DRV8823_b.nReset);
    BSP_GPIO_SetPin_Expect(&DRV8823_b.nReset);
    BSP_SPI_Enable_Expect(&DRV8823_b.SpiChan);

    DRV8823_Reset(&DRV8823_b);

}

void test_DRV8823_StrobeIC_should_SetStrobe_Wait_AndClearStrobe(void){
    BSP_GPIO_SetPin_Expect(&DRV8823_b.Strobe);
    BSP_GPIO_ClearPin_Expect(&DRV8823_b.Strobe);

    DRV8823_StrobeIC(&DRV8823_b);
}


void test_DRV8823_CreateChannelMsg_should_CreateCorrectField_for_ChanA(void){

    sDRV8823DataField_t data = {.Decay = 1,
                                .Current = 7,
                                .Phase = 1,
                                .Enable = 1};

    uint16_t retValue = 0x003F;

    TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_A,&data));

    data.Decay = 1;
    data.Current = 5;
    data.Phase = 0;
    data.Enable = 1;
    retValue = 0x0035;
    TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_A,&data));

}

void test_DRV8823_CreateChannelMsg_should_CreateCorrectField_for_ChanB(void){
    
        sDRV8823DataField_t data = {.Decay = 1,
                                    .Current = 7,
                                    .Phase = 1,
                                    .Enable = 1};
    
        uint16_t retValue = 0x0FC0;
    
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_B,&data));
    
        data.Decay = 1;
        data.Current = 5;
        data.Phase = 0;
        data.Enable = 1;
        retValue = 0x0D40;
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_B,&data));
    
}


void test_DRV8823_CreateChannelMsg_should_CreateCorrectField_for_ChanC(void){
    
        sDRV8823DataField_t data = {.Decay = 1,
                                    .Current = 7,
                                    .Phase = 1,
                                    .Enable = 1};
    
        uint16_t retValue = 0x103F;
    
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_C,&data));
    
        data.Decay = 1;
        data.Current = 5;
        data.Phase = 0;
        data.Enable = 1;
        retValue = 0x1035;
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_C,&data));
    
}


void test_DRV8823_CreateChannelMsg_should_CreateCorrectField_for_ChanD(void){
    
        sDRV8823DataField_t data = {.Decay = 1,
                                    .Current = 7,
                                    .Phase = 1,
                                    .Enable = 1};
    
        uint16_t retValue = 0x1FC0;
    
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_D,&data));
    
        data.Decay = 1;
        data.Current = 5;
        data.Phase = 0;
        data.Enable = 1;
        retValue = 0x1D40;
        TEST_ASSERT_EQUAL_HEX16(retValue,DRV8823_CreateChannelMsg(DRV8823_CHAN_D,&data));
    
}


void set_drv8823_valid(sDRV8823_t *ic, eBSPStatus_t result){
    /** Add to the SPI Send Buffer */
    Buffer8_put_IgnoreAndReturn(BUFFER_8_OK);
    Buffer8_put_IgnoreAndReturn(BUFFER_8_OK);
    Buffer8_put_IgnoreAndReturn(BUFFER_8_OK);
    Buffer8_put_IgnoreAndReturn(BUFFER_8_OK);

    /** Clear the Reset & Sleep Pins */
    BSP_GPIO_SetPin_Expect(&ic->nReset);
    BSP_GPIO_SetPin_Expect(&ic->nSleep);

    /** Set the Chip Select */
    BSP_GPIO_SetPin_Expect(&ic->cs);

    /** Send data over the SPI (4-bytes) */
    BSP_SPI_SendBytes_ExpectAndReturn(&ic->SpiChan, result);

    if(result == BSP_OK){
        /** Set and Clear Strobe */
        BSP_GPIO_SetPin_Expect(&ic->Strobe);
        BSP_GPIO_ClearPin_Expect(&ic->Strobe);
    }

    /** Clear the Chip Select */
    BSP_GPIO_ClearPin_Expect(&ic->cs);

}


void test_DRV8823_Set_should_SendSPIData_and_ReturnValid(void){
    DRV8823_a.Send.outB.Decay = 1;
    DRV8823_a.Send.outB.Current = 7;
    DRV8823_a.Send.outB.Phase = 1;
    DRV8823_a.Send.outB.Enable = 1;

    /** Run Mock Setup Function */
    set_drv8823_valid(&DRV8823_a, BSP_OK);
    
    /** Run the test */
    TEST_ASSERT_EQUAL(DRV8823_OK,DRV8823_Set(&DRV8823_a));
}

void test_DRV8823_Set_should_ReturnInvalid_for_FailedSPI(void){
    DRV8823_a.Send.outB.Decay = 1;
    DRV8823_a.Send.outB.Current = 7;
    DRV8823_a.Send.outB.Phase = 1;
    DRV8823_a.Send.outB.Enable = 1;

    /** Run Mock Setup Function */
    set_drv8823_valid(&DRV8823_a, BSP_FAIL);
    
    /** Run the test */
    TEST_ASSERT_EQUAL(DRV8823_FAIL,DRV8823_Set(&DRV8823_a));

    /** Make sure the data transferred to the Last is the same as Send */
    TEST_ASSERT_EQUAL(0x0FFF, DRV8823_a.Last.dataAB);
    TEST_ASSERT_EQUAL(0x0FFF, DRV8823_a.Last.dataCD);
}

void strobe_ic(sDRV8823_t *ic){
    BSP_GPIO_SetPin_Expect(&ic->Strobe);
    BSP_GPIO_ClearPin_Expect(&ic->Strobe);
}


void test_DRV8823_Set_SingleChannel_should_SetCorrectChannel_and_ReturnValid(void){
    eDRV8823Chan_t chan = DRV8823_CHAN_A;
    sDRV8823DataField_t settings = {.Decay = 1,
                                .Current = 7,
                                .Phase = 0,
                                .Enable = 1};

    BSP_SPI_Enable_Expect(&DRV8823_a.SpiChan);
    set_drv8823_valid(&DRV8823_a,BSP_OK);  
    // strobe_ic(&DRV8823_a);
    BSP_SPI_Disable_Expect(&DRV8823_a.SpiChan);
    TEST_ASSERT_EQUAL(DRV8823_OK,DRV8823_Set_SingleChannel(&DRV8823_a,chan,&settings));
    TEST_ASSERT_EQUAL_HEX16(0x0FFD,DRV8823_a.Last.dataAB);
}


void test_DRV8823_Set_SingleChannel_should_ReturnInvalid_for_FailedSPI(void){
    eDRV8823Chan_t chan = DRV8823_CHAN_A;
    sDRV8823DataField_t settings = {.Decay = 1,
                                .Current = 7,
                                .Phase = 0,
                                .Enable = 1};

    BSP_SPI_Enable_Expect(&DRV8823_a.SpiChan);
    set_drv8823_valid(&DRV8823_a,BSP_FAIL);  
    BSP_SPI_Disable_Expect(&DRV8823_a.SpiChan);
    TEST_ASSERT_EQUAL(DRV8823_FAIL,DRV8823_Set_SingleChannel(&DRV8823_a,chan,&settings));
    TEST_ASSERT_EQUAL_HEX16(0x0FFF,DRV8823_a.Last.dataAB);
}

void test_DRV8823_Set_AllChannels_should_SetAllChannels_with_NewValues_and_ReturnValid(void){
    sDRV8823SetVal_t settings= {.outA = {.Decay=1, .Current = 4, .Phase=0,.Enable=1},
                                .outB = {.Decay=1, .Current = 3, .Phase=0,.Enable=1},
                                .outC = {.Decay=1, .Current = 2, .Phase=0,.Enable=1},
                                .outD = {.Decay=1, .Current = 1, .Phase=0,.Enable=1}};

    BSP_SPI_Enable_Expect(&DRV8823_b.SpiChan);
    set_drv8823_valid(&DRV8823_b, BSP_OK);
    set_drv8823_valid(&DRV8823_b, BSP_OK);
    BSP_SPI_Disable_Expect(&DRV8823_b.SpiChan);
    // strobe_ic(&DRV8823_b);
    // BSP_GPIO_SetPin_Expect(&DRV8823_b.Strobe);
    TEST_ASSERT_EQUAL(DRV8823_OK, DRV8823_Set_AllChannels(&DRV8823_b,&settings));
    TEST_ASSERT_EQUAL(settings.outA.Decay, DRV8823_b.Send.outA.Decay);
    TEST_ASSERT_EQUAL(settings.outA.Current, DRV8823_b.Send.outA.Current);
    TEST_ASSERT_EQUAL(settings.outB.Current, DRV8823_b.Send.outB.Current);
    TEST_ASSERT_EQUAL(settings.outC.Current, DRV8823_b.Send.outC.Current);
    TEST_ASSERT_EQUAL(settings.outD.Current, DRV8823_b.Send.outD.Current);
}


void test_DRV8823_CreateDatafiled_should_CreateValid_and_ReturnValid(void){

    sDRV8823DataField_t output;
    uint8_t decay = 1;
    uint8_t current = 5;
    uint8_t dir = 1;
    uint8_t enable = 1;

    TEST_ASSERT_EQUAL(DRV8823_OK, DRV8823_CreateDatafield(&output,decay,current,dir,enable));

    TEST_ASSERT_EQUAL(decay,output.Decay);
    TEST_ASSERT_EQUAL(current,output.Current);
    TEST_ASSERT_EQUAL(dir,output.Phase);
    TEST_ASSERT_EQUAL(enable, output.Enable);
}

void test_DRV8823_CreateDatafield_should_Fail_and_NotUpdate_for_Invalid(void){
    sDRV8823DataField_t output;
    uint8_t decay = 1;
    uint8_t current = 5;
    uint8_t dir = 2;
    uint8_t enable = 1;

    TEST_ASSERT_EQUAL(DRV8823_FAIL, DRV8823_CreateDatafield(&output,decay,current,dir,enable));
    TEST_ASSERT_NOT_EQUAL(dir, output.Phase);

    dir=1;
    TEST_ASSERT_EQUAL(DRV8823_OK, DRV8823_CreateDatafield(&output,decay,current,dir,enable));

    current = 8;
    TEST_ASSERT_EQUAL(DRV8823_FAIL, DRV8823_CreateDatafield(&output,decay,current,dir,enable));
    TEST_ASSERT_NOT_EQUAL(current, output.Current);

    current = 0;
    TEST_ASSERT_EQUAL(DRV8823_OK, DRV8823_CreateDatafield(&output,decay,current,dir,enable));
    decay = 2;
    TEST_ASSERT_EQUAL(DRV8823_FAIL, DRV8823_CreateDatafield(&output,decay,current,dir,enable));
    TEST_ASSERT_NOT_EQUAL(decay, output.Decay);
}