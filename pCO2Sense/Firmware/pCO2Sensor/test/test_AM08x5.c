#include "unity.h"
#include "AM08x5.h"
#include "mock_bsp.h"
#include "mock_buffer8.h"
#include "mock_errors.h"
void setUp(void)
{
}

void tearDown(void)
{
}

void test_AM08X5_Initialize_should_Init_and_ReturnValid(void){

    BSP_I2C_Init_Expect(&RTC.I2C);
    BSP_GPIO_Init_Expect(&RTC.nIRQ1.pin);
    BSP_GPIO_Init_Expect(&RTC.nIRQ2.pin);
    BSP_GPIO_Init_Expect(&RTC.TIRQ.pin);
    BSP_GPIO_Init_Expect(&RTC.EXTI.pin);
    BSP_GPIO_Init_Expect(&RTC.WDI.pin);
    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_Initialize());
}


void test_AM08X5_WriteReg_should_SendI2CMessage(void){
    uint8_t addr = 0xAA;
    uint8_t data = 0xCC;

    /** Mock the called functions */
    BSP_I2C_put_ExpectAndReturn(&RTC.I2C,data,BSP_OK);


    /** Run the test */
    AM08X5_WriteReg(addr,data);
    TEST_ASSERT_EQUAL(addr,RTC.I2C.slaveAddress);

}

void test_AM08X5_WriteReg_should_ReportError_for_BadI2CTx(void){
    uint8_t addr = 0xBB;
    uint8_t data = 0xEE;

    /** Mock the called functions */
    BSP_I2C_put_ExpectAndReturn(&RTC.I2C,data,BSP_FAIL);
    Error_Expect(ERROR_MSG_3900,LOG_Priority_Med);


    /** Run the test */
    AM08X5_WriteReg(addr,data);


}

void mock_burst_write(uint8_t addr, uint8_t *data, uint8_t length, eBSPStatus_t bsp){
     uint8_t i;
     /** Mock the called functions */
    BSP_I2C_puts_ExpectAndReturn(&RTC.I2C,data,length,bsp);

}

void mock_burst_read(uint8_t addr,uint8_t *returndata, uint8_t *expecteddata, uint8_t length){

	BSP_I2C_gets_Expect(&RTC.I2C,addr,returndata,length);
    BSP_I2C_gets_IgnoreArg_val();
    BSP_I2C_gets_ReturnArrayThruPtr_val(expecteddata,length);

}

void test_AM08X5_BurstWrite_should_SendI2CMessages(void){
    uint8_t addr = 0x0F;
    uint8_t data[16];
    uint8_t i;
    for(i=0;i<16;i++){
        data[i] = i;
    }

    mock_burst_write(addr,data,16,BSP_OK);

    /** Run the test */
    AM08X5_BurstWrite(addr,data,16);
    TEST_ASSERT_EQUAL(addr,RTC.I2C.slaveAddress);
}


void mock_read_reg(uint8_t addr, uint8_t returndata){
    BSP_I2C_get_ExpectAndReturn(&RTC.I2C,addr,returndata);
}

void test_AM08X5_ReadReg_should_ReadI2C_and_ReturnMessages(void){
    uint8_t addr = 0xF0;
    uint8_t value = 0xAB;
    uint8_t fakedata = 0x00;

    
    mock_read_reg(addr,value);

    /** Run the test */
    TEST_ASSERT_EQUAL(value,AM08X5_ReadReg(addr));
    TEST_ASSERT_EQUAL(addr,RTC.I2C.slaveAddress);

}


void test_AM08X5_BurstRead_should_ReadI2C_and_ReturnValues(void){
    // Arrange 
    uint8_t addr = 0xCB;
    uint8_t dlength = 16;
    uint8_t data[16];
    uint8_t *pd = &data[0];
    uint8_t expectedData[16];
    uint8_t *pe = &expectedData[0];
    uint8_t i;

    for(i=0;i<dlength;i++){
        data[i] = 0;
        expectedData[i] = i;
    }


    // Expect 
    BSP_I2C_gets_Expect(&RTC.I2C,addr,pd,dlength);
    BSP_I2C_gets_IgnoreArg_val();
    BSP_I2C_gets_ReturnArrayThruPtr_val(pe,dlength);
 

    // Act
    AM08X5_BurstRead(addr,pd,dlength);
    TEST_ASSERT_EQUAL(addr,RTC.I2C.slaveAddress);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData,data,dlength);
}

void ignore_writereg(uint8_t addr, uint8_t data, eBSPStatus_t bsp_stat){

    uint8_t datareg = data;

    BSP_I2C_put_IgnoreAndReturn(bsp_stat);

}

void ignore_readreg(uint8_t addr, uint8_t value, eBuffer8Status_t buf_stat, eBSPStatus_t bsp_stat){
    BSP_I2C_get_IgnoreAndReturn(value);
}

void mock_write_reg(uint8_t addr, uint8_t value, eBSPStatus_t bsp){
    BSP_I2C_put_ExpectAndReturn(&RTC.I2C,value, BSP_OK);
}

void test_AM08X5_osc_sel_should_SetOscillatorMode_and_ReturnValid(void){

    mock_read_reg(OSC_CONTROL_REG,0xFF);
    mock_write_reg(OSC_CONTROL_REG,0xA1,BSP_OK);
    mock_write_reg(OSC_CONTROL_REG,0x67,BSP_OK);
    mock_read_reg(OSC_STATUS_REG,0x08);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_osc_sel(0x00u));
}


void ignore_osc_cal(uint8_t osc){
    ignore_readreg(OSC_CONTROL_REG,0x05,BUFFER_C_OK,BSP_OK);
    ignore_writereg(OSC_CONTROL_REG,0xA1,BSP_OK);
    ignore_writereg(OSC_CONTROL_REG,0x05,BSP_OK);
    ignore_readreg(OSC_CONTROL_REG,0x10,BUFFER_C_OK,BSP_OK);
}

void test_AM08X5_OffsetCalculation_should_CalcOffset_SetOsc_and_ReturnValid(void){
    ignore_osc_cal(0);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_OffsetCalculation(32778));
    TEST_ASSERT_EQUAL(32778,RTC.cal.freq);
    TEST_ASSERT_EQUAL(97,RTC.cal.calxt);
    TEST_ASSERT_EQUAL(128,RTC.cal.oscstat);


}


void mock_get_time(uint8_t *data, uint8_t *bcddata, uint8_t length, uint8_t century, uint8_t mode){
    
    mock_burst_read(HUNDREDTHS_REG,data,bcddata,length);
    mock_read_reg(CONTROL_1_REG,0xFF);
    mock_read_reg(STATUS_REG,0xFF);
}

void test_AM08X5_get_time_should_ReturnValidTime(void){
    uint8_t i;
    uint8_t data[8] = {123,4,20,13,5,3,18,1};
    uint8_t bcddata[8] = {0,0,0,0,0,0,0,0};
    uint8_t mode = 0x00;
    uint8_t century = 0x00;

    /** convert to BCD */
    for(i=0;i<8;i++){
        bcddata[i] = dec2bcd(data[i]);
    }

    mock_get_time(data,bcddata,8,century,mode);

    AM08X5_get_time();

    TEST_ASSERT_EQUAL(data[0],RTC.time.time.hundredth);
    TEST_ASSERT_EQUAL(data[1],RTC.time.time.second);
    TEST_ASSERT_EQUAL(data[2],RTC.time.time.minute);
    TEST_ASSERT_EQUAL(data[3],RTC.time.time.hour);
    TEST_ASSERT_EQUAL(data[4],RTC.time.time.date);
    TEST_ASSERT_EQUAL(data[5],RTC.time.time.month);
    TEST_ASSERT_EQUAL(data[6],RTC.time.time.year);
    TEST_ASSERT_EQUAL(data[7],RTC.time.time.weekday);
}


void test_AM08X5_GetTimestamp_should_ReadTime_and_CreateTimestamp(void){
    uint8_t i;
    uint8_t data[8] = {73,42,17,8,7,3,18,1};
    uint8_t bcddata[8] = {0,0,0,0,0,0,0,0};
    uint8_t mode = 0x00;
    uint8_t century = 0x00;

    AM08X5_Datestamp_t ds = DS_YYsMMsDD;
    AM08X5_Timestamp_t ts = TS_TIME_HH_MM_SS_SS;

    char str[32];
    memset(str,0,32);

    /** convert to BCD */
    for(i=0;i<8;i++){
        bcddata[i] = dec2bcd(data[i]);
    }

    /** Mock the time retreival */
    mock_get_time(&data[0],&bcddata[0],8,century,mode);

    /** Run the function */
    AM08X5_GetTimestamp(&str[0],ds,ts);

    TEST_ASSERT_EQUAL_STRING("18/03/07 08:17:42.73",str);
    TEST_ASSERT_EQUAL_STRING(str,RTC.time.timestamp);

}

void test_AM08X5_GetTime_should_PopulateTimestruct(void){
    uint8_t i;
    uint8_t data[8] = {13,21,8,9,7,3,18,1};
    uint8_t bcddata[8] = {0,0,0,0,0,0,0,0};
    uint8_t mode = 0x00;
    uint8_t century = 0x00;
    sTimeReg_t t;

    AM08X5_Datestamp_t ds = DS_YYsMMsDD;
    AM08X5_Timestamp_t ts = TS_TIME_HH_MM_SS_SS;

    char str[32];
    memset(str,0,32);

    /** convert to BCD */
    for(i=0;i<8;i++){
        bcddata[i] = dec2bcd(data[i]);
    }

    /** Mock the time retreival */
    mock_get_time(&data[0],&bcddata[0],8,century,mode);

    /** Get Time */
    AM08X5_GetTime(&t);

    TEST_ASSERT_EQUAL(data[0],t.hundredth);
    TEST_ASSERT_EQUAL(data[1],t.second);
    TEST_ASSERT_EQUAL(data[2],t.minute);
    TEST_ASSERT_EQUAL(data[3],t.hour);
    TEST_ASSERT_EQUAL(data[4],t.date);
    TEST_ASSERT_EQUAL(data[5],t.month);
    TEST_ASSERT_EQUAL(data[6],t.year);
    TEST_ASSERT_EQUAL(data[7],t.weekday);
}





void test_valid_year_should_return_valid_for_valid_year(void){
    TEST_ASSERT_EQUAL(true,valid_year(18));
    TEST_ASSERT_EQUAL(true,valid_year(50));
    TEST_ASSERT_EQUAL(true,valid_year(99));
}

void test_valid_year_should_return_fail_for_invalid_year(void){
    TEST_ASSERT_EQUAL(false,valid_year(00));
    TEST_ASSERT_EQUAL(false,valid_year(17));
    TEST_ASSERT_EQUAL(false,valid_year(100));
    TEST_ASSERT_EQUAL(false,valid_year(255));
}

void test_valid_month_should_return_valid_for_valid_month(void){
    TEST_ASSERT_EQUAL(true,valid_month(1));
    TEST_ASSERT_EQUAL(true,valid_month(5));
    TEST_ASSERT_EQUAL(true,valid_month(12));
}

void test_valid_month_should_return_fail_for_invalid_month(void){
    TEST_ASSERT_EQUAL(false,valid_month(0));
    TEST_ASSERT_EQUAL(false,valid_month(13));
    TEST_ASSERT_EQUAL(false,valid_month(100));
    TEST_ASSERT_EQUAL(false,valid_month(255));
}

void test_valid_day_should_return_valid_for_valid_day(void){
    TEST_ASSERT_EQUAL(true,valid_day(1,3,18));
    TEST_ASSERT_EQUAL(true,valid_day(5,3,18));
    TEST_ASSERT_EQUAL(true,valid_day(30,3,18));
    TEST_ASSERT_EQUAL(true,valid_day(29,2,20));     /** Leap Year */
}

void test_valid_day_should_return_fail_for_invalid_day(void){
    TEST_ASSERT_EQUAL(false,valid_day(0,3,18));
    TEST_ASSERT_EQUAL(false,valid_day(32,3,18));
    TEST_ASSERT_EQUAL(false,valid_day(100,3,18));
    TEST_ASSERT_EQUAL(false,valid_day(255,3,18));
    TEST_ASSERT_EQUAL(false,valid_day(29,2,19));     /** Not A Leap Year */
}


void test_valid_hour_should_return_valid_for_valid_hour(void){
    TEST_ASSERT_EQUAL(true,valid_hour(0));
    TEST_ASSERT_EQUAL(true,valid_hour(5));
    TEST_ASSERT_EQUAL(true,valid_hour(15));
    TEST_ASSERT_EQUAL(true,valid_hour(23));
}

void test_valid_hour_should_return_fail_for_invalid_hour(void){
    TEST_ASSERT_EQUAL(false,valid_hour(24));
    TEST_ASSERT_EQUAL(false,valid_hour(100));
    TEST_ASSERT_EQUAL(false,valid_hour(255));
}

void test_valid_minute_should_return_valid_for_valid_minute(void){
    TEST_ASSERT_EQUAL(true,valid_minute(0));
    TEST_ASSERT_EQUAL(true,valid_minute(5));
    TEST_ASSERT_EQUAL(true,valid_minute(30));
    TEST_ASSERT_EQUAL(true,valid_minute(59));
}

void test_valid_minute_should_return_fail_for_invalid_minute(void){
    TEST_ASSERT_EQUAL(false,valid_minute(60));
    TEST_ASSERT_EQUAL(false,valid_minute(100));
    TEST_ASSERT_EQUAL(false,valid_minute(255));
}

void test_valid_second_should_return_valid_for_valid_second(void){
    TEST_ASSERT_EQUAL(true,valid_second(0));
    TEST_ASSERT_EQUAL(true,valid_second(5));
    TEST_ASSERT_EQUAL(true,valid_second(30));
    TEST_ASSERT_EQUAL(true,valid_second(59));
}

void test_valid_second_should_return_fail_for_invalid_second(void){
    TEST_ASSERT_EQUAL(false,valid_second(60));
    TEST_ASSERT_EQUAL(false,valid_second(100));
    TEST_ASSERT_EQUAL(false,valid_second(255));
}


void mock_clear_reg(uint8_t addr, uint8_t value, uint8_t expectedMaskValue,eBSPStatus_t bsp){

    /** Mock the Read Reg Function */
    mock_read_reg(addr,value);
    mock_write_reg(addr,expectedMaskValue,BSP_OK);


}


void test_AM08X5_ClearReg_should_Read_and_Mask_Register(void){
    uint8_t addr = 0xCD;
    uint8_t value = 0xFF;
    uint8_t maskval = 0x7F;
    uint8_t fakedata = 0x7F;
    uint8_t mask = 0x80;

    mock_clear_reg(addr,value,maskval,BSP_OK);



    AM08X5_ClearReg(addr,mask);

}


void test_AM08X5_SetTime_should_ParseString_and_WriteToChip(void){
    char str[] = "18/03/07 09:14:32";
    uint8_t data[8] = {0,32,14,9,7,3,18,0};
    uint8_t bcddata[8];
    uint8_t addr = 0x10;
    uint8_t i;

    uint8_t value = 0xFF;
    uint8_t mask = 0x40;
    uint8_t mvalue = value & ~mask;


    for(i=0;i<8;i++){
        bcddata[i] = dec2bcd(data[i]);
    }


    mock_clear_reg(CONTROL_1_REG,value,mvalue,BSP_OK);
    mock_burst_write(HUNDREDTHS_REG,bcddata,8,BSP_OK);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_SetTime(&str[0]));


}

void mock_set_reg(uint8_t addr, uint8_t rxVal, uint8_t expectedMaskVal){

    BSP_I2C_get_ExpectAndReturn(&RTC.I2C,addr,rxVal);
    BSP_I2C_put_ExpectAndReturn(&RTC.I2C,expectedMaskVal,BSP_OK);
    BSP_I2C_put_IgnoreArg_val();

}

void test_AM08X5_SetReg_should_ReadValue_ApplyMask_and_Write(void){
    uint8_t addr = 0x7F;
    uint8_t mask = 0x0B;

    mock_set_reg(addr,0xF0,0xFB);
    AM08X5_SetReg(addr,mask);
}


void mock_config_squarewave(uint8_t pin, eAM08X5_SQFS_t sqfs){
    uint8_t value = 0xFF;
    uint8_t setVal = 0x00;
    uint8_t temp;
    mock_read_reg(SQW_REG,value);

    temp = value & 0x70;
    temp |= sqfs;

    if(pin==0x00){

    } else if (pin == 0x01){
        mock_clear_reg(CONTROL_2_REG,0x03,0x00,BSP_OK);
        mock_set_reg(CONTROL_2_REG,0x01,BSP_OK);
        setVal = 0x01;
        temp |= 0x80;
    } else if(pin == 0x02){
        mock_clear_reg(CONTROL_2_REG,0x1C,0x00,BSP_OK);
        mock_set_reg(CONTROL_2_REG,0x04,BSP_OK);
        temp |= 0x80;
    } else {

    }
    mock_write_reg(SQW_REG,temp,BSP_OK);

}

void test_AM08X5_ConfigSquareWave_should_SetSquareWave_on_CorrectPin(void){
    /** Given */
    uint8_t pin = AM08X5_OUTPUT_NIRQ1;
    eAM08X5_SQFS_t sqfs = SQFS_32768;

    /** When */
    mock_config_squarewave(pin,sqfs);

    /** Then */
    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_ConfigSquareWave(pin,sqfs));

}

void test_AM08X5_ConfigSquareWave_should_ClearRegister_for_Disable_Instruction(void){
    /** Given */
    uint8_t pin = AM08X5_SQW_OUT_OFF;
    eAM08X5_SQFS_t sqfs = SQFS_CENTURY;

    /** When */
    mock_config_squarewave(pin,sqfs);

    /** Then */
    TEST_ASSERT_EQUAL(AM08X5_OK, AM08X5_ConfigSquareWave(pin,sqfs));
}

void test_AM08X5_CrystalCal_should_SetTheControlRegisters(void){
    /** Given */
    uint8_t pin = AM08X5_SQW_OUT_OFF;
    eAM08X5_SQFS_t sqfs = SQFS_CENTURY;

    /** When */
    mock_write_reg(CAL_XT_REG,0x00u,BSP_OK);
    mock_write_reg(OSC_STATUS_REG,0x00u,BSP_OK);

    /** Config Squarewave */
    mock_config_squarewave(pin,sqfs);
    pin = AM08X5_OUTPUT_NIRQ1;
    sqfs = SQFS_32768;
    mock_config_squarewave(pin,sqfs);

    /** Then */
    AM08X5_CrystalCal_Set();


}

void test_AM08X5_CrystalCal_Clear_should_ClearSQWRegister(void){
    /** Given */
    uint8_t pin = AM08X5_SQW_OUT_OFF;
    eAM08X5_SQFS_t sqfs = SQFS_CENTURY;

    /** When */
    mock_config_squarewave(pin,sqfs);

    /** Then */
    AM08X5_CrystalCal_Clear();
}


void mock_set_alarm(uint8_t rpt, uint8_t mode, uint8_t pin, uint8_t *pVal, sTimeReg_t *t){
    uint8_t temp;
    uint8_t data[8];
    uint8_t value = 0x3F;

    data[0] = dec2bcd(0x00);            /** Round seconds only */
    data[1] = dec2bcd(t->second);
    data[2] = dec2bcd(t->minute);
    data[3] = dec2bcd(t->hour);
    data[4] = dec2bcd(t->date);
    data[5] = dec2bcd(t->month);
    data[6] = dec2bcd(t->weekday);

    mock_clear_reg(TIMER_CTRL_REG,0x1C,0x00,BSP_OK);
    mock_clear_reg(INT_MASK_REG,0x64,0x00,BSP_OK);
    mock_clear_reg(STATUS_REG,0x04,0x00,BSP_OK);

    if(pin == 1){
        mock_read_reg(CONTROL_2_REG,value);
        mock_set_reg(CONTROL_2_REG,0x03,0x03);

    }

    if(0 != rpt){
        mock_set_reg(TIMER_CTRL_REG,rpt<<2,rpt<<2);
        mock_set_reg(INT_MASK_REG,mode<<5,mode<<5);
        BSP_I2C_puts_ExpectAndReturn(&RTC.I2C,&data[0],7,BSP_OK);
        BSP_I2C_puts_IgnoreArg_val();
        mock_set_reg(INT_MASK_REG,0x04,0x04);
    } else {
        mock_set_reg(INT_MASK_REG,0x60,0x60);
    }
}

void test_AM08X5_set_alarm_should_SetMode_and_returnValid(void){
    uint8_t repeat = 1;
    uint8_t intmode = 1;
    uint8_t pin = 1;
    uint8_t value = 0xff;
    uint8_t *pVal = &value;
    sTimeReg_t t = {   .hundredth = 37,
                            .second = 5,
                            .minute = 7,
                            .hour = 22,
                            .date = 15,
                            .month = 3,
                            .year = 18,
                            .weekday = 5
                            };


    mock_set_alarm(repeat,intmode,pin,pVal,&t);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_set_alarm(&t, repeat,intmode,pin));
    TEST_ASSERT_EQUAL(t.month,RTC.nIRQ1.alarm.month);
}

void test_AM08X5_set_alarm_should_clear_alarm_for_repeat_equalZero(void){
    uint8_t repeat = 0;
    uint8_t intmode = 1;
    uint8_t pin = 1;
    uint8_t value = 0xff;
    uint8_t *pVal = &value;
    sTimeReg_t t = {   .hundredth = 37,
                            .second = 5,
                            .minute = 7,
                            .hour = 22,
                            .date = 15,
                            .month = 3,
                            .year = 18,
                            .weekday = 5
                            };


    mock_set_alarm(repeat,intmode,pin,pVal,&t);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_set_alarm(&t, repeat,intmode,pin));
    TEST_ASSERT_EQUAL(t.month,RTC.nIRQ1.alarm.month);
}

void test_AM08X5_AlarmClear_should_ClearAlarm_Register(void){
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            }


                        };

    uint8_t repeat = 0;
    uint8_t intmode = 1;
    uint8_t pin = 1;
    uint8_t value = 0xff;
    uint8_t *pVal = &value;
    mock_set_alarm(repeat,intmode,pin,pVal,&TT.alarm);
    AM08X5_AlarmClear(&TT);
    TEST_ASSERT_EQUAL(0,TT.alarm.year);
    TEST_ASSERT_EQUAL(0,TT.nextAlarm.date);
    TEST_ASSERT_EQUAL(0,TT.repeat.hour);
}

void test_AM08X5_add_time_struct_should_AddStruct_easyHour_and_Return(void){
    sTimeReg_t t1 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 12,
                        .date = 19,
                        .month = 3,
                        .year = 18,
                        .weekday = 5
                        };

    sTimeReg_t t2 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 6,
                        .date = 0,
                        .month = 0,
                        .year = 0,
                        .weekday = 0
                        };
    sTimeReg_t t3;

    AM08X5_clear_time_struct(&t3);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_add_time_struct(&t1,&t2,&t3));
    TEST_ASSERT_EQUAL(18,t3.hour);
    TEST_ASSERT_EQUAL(19,t3.date);
}

void test_AM08X5_add_time_struct_should_AddStruct_hour_changingdate_and_Return(void){
    sTimeReg_t t1 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 23,
                        .date = 19,
                        .month = 3,
                        .year = 18,
                        .weekday = 5
                        };

    sTimeReg_t t2 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 12,
                        .date = 0,
                        .month = 0,
                        .year = 0,
                        .weekday = 0
                        };
    sTimeReg_t t3;

    AM08X5_clear_time_struct(&t3);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_add_time_struct(&t1,&t2,&t3));
    TEST_ASSERT_EQUAL(11,t3.hour);
    TEST_ASSERT_EQUAL(20,t3.date);
}


void test_AM08X5_add_time_struct_should_AddStruct_leapyear_and_Return(void){
    sTimeReg_t t1 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 23,
                        .date = 28,
                        .month = 2,
                        .year = 20,
                        .weekday = 5
                        };

    sTimeReg_t t2 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 48,
                        .date = 0,
                        .month = 0,
                        .year = 0,
                        .weekday = 0
                        };
    sTimeReg_t t3;

    AM08X5_clear_time_struct(&t3);

    TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_add_time_struct(&t1,&t2,&t3));
    TEST_ASSERT_EQUAL(23,t3.hour);
    TEST_ASSERT_EQUAL(1,t3.date);
    TEST_ASSERT_EQUAL(3,t3.month);
}

void test_AM08X5_add_time_struct_run_through_months(void){
    sTimeReg_t t1 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 0,
                        .date = 15,
                        .month = 1,
                        .year = 18,
                        .weekday = 5
                        };

    sTimeReg_t t2 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 0,
                        .date = 0,
                        .month = 1,
                        .year = 0,
                        .weekday = 0
                        };
    sTimeReg_t t3;

    AM08X5_clear_time_struct(&t3);
    uint8_t i;
    uint8_t month;
    // printf("\n\n*****");
    for(i=2;i<24;i++){
        TEST_ASSERT_EQUAL(AM08X5_OK,AM08X5_add_time_struct(&t1,&t2,&t3));
        month = ((i - 1) % 12 ) + 1;
        TEST_ASSERT_EQUAL(month,t3.month);
       
        t1 = t3;
    }

}

void test_AM08X5_AlarmSet_should_SetAlarm_and_Alarm_field(void){
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            }


                        };
    sTimeReg_t t = { 0 };
    t.year = 18;
    t.month = 3;
    t.date = 19;
    t.hour = 22;

    uint8_t value = 0xff;
    uint8_t *pVal = &value;

    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&t);
    AM08X5_AlarmSet(&TT,&t);
    TEST_ASSERT_EQUAL(t.year,TT.alarm.year);
    TEST_ASSERT_EQUAL(t.month,TT.alarm.month);
    TEST_ASSERT_EQUAL(t.date,TT.alarm.date);
    TEST_ASSERT_EQUAL(t.hour,TT.alarm.hour);
}


void test_AM08X5_AlarmSet_with_Repeat_should_SetAlarm_and_NextAlarm_field(void){
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 12,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 37,
                                            .second = 5,
                                            .minute = 7,
                                            .hour = 22,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 0,
                                            .date = 0,
                                            .month = 0,
                                            .year = 0,
                                            .weekday = 5
                                            }


                        };

    AM08X5_clear_time_struct(&TT.nextAlarm);
    AM08X5_clear_time_struct(&TT.repeat);
    sTimeReg_t t = { 0 };
    t.year = 18;
    t.month = 3;
    t.date = 19;
    t.hour = 12;
    sTimeReg_t rpt = { 0 };
    AM08X5_clear_time_struct(&rpt);
    rpt.hour = 6;
    
    uint8_t value = 0xff;
    uint8_t *pVal = &value;

    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&t);
    AM08X5_AlarmSet_with_Repeat(&TT,&t,&rpt);
    TEST_ASSERT_EQUAL(t.year,TT.alarm.year);
    TEST_ASSERT_EQUAL(t.month,TT.alarm.month);
    TEST_ASSERT_EQUAL(t.date,TT.alarm.date);
    TEST_ASSERT_EQUAL(t.hour,TT.alarm.hour);
    
    TEST_ASSERT_EQUAL(18,TT.nextAlarm.hour);
}





void test_AM08X5_compare_time_should_CompareTwoTimes(void){
    sTimeReg_t t1 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 0,
                        .date = 1,
                        .month = 3,
                        .year = 18,
                        .weekday = 5
                        };

    sTimeReg_t t2 = {  .hundredth = 0,
                        .second = 0,
                        .minute = 0,
                        .hour = 0,
                        .date = 1,
                        .month = 2,
                        .year = 18,
                        .weekday = 0
                        };

    TEST_ASSERT_EQUAL(true,AM08X5_compare_time(&t1,&t2));

}


void test_AM08X5_AlarmSet_Next_should_SetAlarm_with_NextAlarmDT(void){

    /** Given */
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 12,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 13,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 1,
                                            .date = 0,
                                            .month = 0,
                                            .year = 0,
                                            .weekday = 5
                                            },
                            .repeatCycles = 1
                        };

    RTC.time.time.year = 18;
    RTC.time.time.month = 3;
    RTC.time.time.date = 15;
    RTC.time.time.hour = 12;
    RTC.time.time.minute = 45;
    RTC.time.time.second = 30;

    uint8_t val = 0xff;
    uint8_t *pVal = &val;
    /** When */
    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&TT.alarm);

    /** Then */
    AM08X5_AlarmSet_Next(&TT);
    TEST_ASSERT_EQUAL(13,TT.alarm.hour);
    TEST_ASSERT_EQUAL(14,TT.nextAlarm.hour);
}

void test_AM08X5_AlarmSet_Next_should_NotRepeat_if_RepeatIsOff(void){
/** Given */
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 12,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 13,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 1,
                                            .date = 0,
                                            .month = 0,
                                            .year = 0,
                                            .weekday = 5
                                            },
                            .repeatCycles = 0
                        };

    RTC.time.time.year = 18;
    RTC.time.time.month = 3;
    RTC.time.time.date = 15;
    RTC.time.time.hour = 12;
    RTC.time.time.minute = 45;
    RTC.time.time.second = 30;

    uint8_t val = 0xff;
    uint8_t *pVal = &val;
    /** When */

    /** Then */
    AM08X5_AlarmSet_Next(&TT);
    TEST_ASSERT_EQUAL(12,TT.alarm.hour);
}


void test_AM08X5_AlarmSet_Next_should_SetAlarm_with_NextAlarmDT_and_decrease_repeats(void){

    /** Given */
    sAM08X5Alarm_t TT = {  .alarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 0,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .nextAlarm = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 1,
                                            .date = 15,
                                            .month = 3,
                                            .year = 18,
                                            .weekday = 5
                                            },
                            .repeat = {      .hundredth = 0,
                                            .second = 0,
                                            .minute = 0,
                                            .hour = 1,
                                            .date = 0,
                                            .month = 0,
                                            .year = 0,
                                            .weekday = 5
                                            },
                            .repeatCycles = 3
                        };

    RTC.time.time.year = 18;
    RTC.time.time.month = 3;
    RTC.time.time.date = 15;
    RTC.time.time.hour = 0;
    RTC.time.time.minute = 45;
    RTC.time.time.second = 30;

    uint8_t val = 0xff;
    uint8_t *pVal = &val;
    uint8_t i;

    /** When */
    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&TT.alarm);
    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&TT.alarm);
    mock_set_alarm(0,1,SYSTEM_RTC_ALARM_PIN,pVal,&TT.alarm);

    /** Then */
    for(i=0;i<3;i++){        
        AM08X5_AlarmSet_Next(&TT);
        RTC.time.time.hour += 1;
        TEST_ASSERT_EQUAL(i+1,TT.alarm.hour);
        TEST_ASSERT_EQUAL(i+2,TT.nextAlarm.hour);
    }

    AM08X5_AlarmSet_Next(&TT);
    TEST_ASSERT_EQUAL(3,TT.alarm.hour);

}



void test_AM08X5_calculate_seconds_should_return_ValidNumberOfSeconds(void){
    sTimeReg_t t = {.year = 0,
                    .month = 0,
                    .date = 0,
                    .hour = 12,
                    .minute = 45,
                    .second = 32
                    };

    TEST_ASSERT_EQUAL(45932,AM08X5_calculate_seconds(&t));

    t.date = 4;
    TEST_ASSERT_EQUAL(391532,AM08X5_calculate_seconds(&t));
}

void test_AM08X5_GetTimestamp_ISO8601_should_return_formatted_timestamp(void){
    /* Arrange */
    uint8_t i;
    uint8_t data[8] = {00,59,38,8,4,5,18,1};
    uint8_t bcddata[8] = {0,0,0,0,0,0,0,0};
    uint8_t mode = 0x00;
    uint8_t century = 0x00;
    sTimeReg_t t;

    char str[20];
    memset(str,0,20);
    char expected[] = "2018-05-04T08:38:59Z";

    /** convert to BCD */
    for(i=0;i<8;i++){
        bcddata[i] = dec2bcd(data[i]);
    }

    /* Expect */
     mock_get_time(&data[0],&bcddata[0],8,century,mode);

     /* Act */
     AM08X5_GetTimestamp_ISO8601(str);
     TEST_ASSERT_EQUAL_STRING(expected,str);


}
    
   