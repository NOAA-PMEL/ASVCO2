#include "unity.h"
#include "scheduler.h"
#include "mock_pco2.h"
#include "mock_AM08x5.h"
#include "mock_errors.h"

void setUp(void)
{
}

void tearDown(void)
{
}


void mock_create_task(sTimeReg_t *start, sTimeReg_t *interval, uint8_t sResult, uint8_t pResult){
    AM08X5_clear_time_struct_Expect(start);
    AM08X5_clear_time_struct_IgnoreArg_t();
    AM08X5_GetTime_Expect(start);
    AM08X5_GetTime_IgnoreArg_t();
    AM08X5_compare_time_ExpectAndReturn(start,start,sResult);
    AM08X5_compare_time_IgnoreArg_t2();

    if(true == sResult){
        AM08X5_clear_time_struct_Expect(start);
        AM08X5_clear_time_struct_IgnoreArg_t();
        PCO2_GetRunLength_Expect(start);
        PCO2_GetRunLength_IgnoreArg_run();
        AM08X5_compare_time_ExpectAndReturn(interval,start,pResult);
        AM08X5_compare_time_IgnoreArg_t2();
        if(false == pResult){
            Error_Expect(ERROR_MSG_0021,LOG_Priority_High);
        }
    } else {
        Error_Expect(ERROR_MSG_0020,LOG_Priority_High);
    }


}
void test_SCHEDULER_create_task_should_Fail_if_MaxNumber_of_Tasks_reached(void){
     /* Arrange */
    sTimeReg_t start;
    sTimeReg_t interval;
    taskCnt = 8;
    int16_t repeat = 0;

    /* Expect */

    /* Act */
    TEST_ASSERT_EQUAL(SCHEDULER_FAIL,SCHEDULER_create_task(PCO2_Run,&start,&interval,repeat));
}

void test_SCHEDULER_create_task_should_verify_start_time_and_return_valid(void){
    /* Arrange */
    sTimeReg_t start = { .year = 18,
                         .month = 4,
                         .date = 23,
                         .hour = 8,
                         .minute = 0,
                         .second = 0
                         };
    sTimeReg_t interval = { .year = 0,
                            .month = 0,
                            .date = 0,
                            .hour = 0,
                            .minute = 30,
                            .second = 0
                            };
    taskCnt = 0;
    int16_t repeat = 0;
    /* Expect */
    mock_create_task(&start,&interval,true,true);

    /* Act */
    TEST_ASSERT_EQUAL(SCHEDULER_OK,SCHEDULER_create_task(PCO2_Run,&start,&interval,repeat));
    TEST_ASSERT_EQUAL(&PCO2_Run, taskList[0].taskFunction);
    TEST_ASSERT_EQUAL(interval.minute,taskList[0].period.minute);
    TEST_ASSERT_EQUAL(1,taskCnt);
}

void test_SCHEDULER_create_task_should_fail_for_start_time_before_current_time(void){
    /* Arrange */
    sTimeReg_t start = { .year = 18,
                         .month = 4,
                         .date = 23,
                         .hour = 8,
                         .minute = 0,
                         .second = 0
                         };
    sTimeReg_t interval = { .year = 0,
                            .month = 0,
                            .date = 0,
                            .hour = 0,
                            .minute = 30,
                            .second = 0
                            };
    taskCnt = 0;
    int16_t repeat = 0;
    /* Expect */
    mock_create_task(&start,&interval,false,true);

    /* Act */
    TEST_ASSERT_EQUAL(SCHEDULER_FAIL,SCHEDULER_create_task(PCO2_Run,&start,&interval,repeat));
}


void test_SCHEDULER_create_task_should_fail_for_inverval_greater_than_sample_runtime(void){
    /* Arrange */
    sTimeReg_t start = { .year = 18,
                         .month = 4,
                         .date = 23,
                         .hour = 8,
                         .minute = 0,
                         .second = 0
                         };
    sTimeReg_t interval = { .year = 0,
                            .month = 0,
                            .date = 0,
                            .hour = 0,
                            .minute = 30,
                            .second = 0
                            };
    taskCnt = 0;
    int16_t repeat = 0;
    /* Expect */
    mock_create_task(&start,&interval,true,false);

    /* Act */
    TEST_ASSERT_EQUAL(SCHEDULER_FAIL,SCHEDULER_create_task(PCO2_Run,&start,&interval,repeat));
}


void test_SCHEDULER_delete_task_should_delete_task_and_decrement_count(void){
     /* Arrange */

    taskCnt = 5;

    taskList[0].taskFunction = PCO2_Run;
    taskList[1].taskFunction = PCO2_Run;
    taskList[2].taskFunction = PCO2_Run;
    taskList[3].taskFunction = PCO2_Run;
    taskList[4].taskFunction = PCO2_Run;
    taskList[0].start.hour = 0;
    taskList[1].start.hour = 1;
    taskList[2].start.hour = 2;
    taskList[3].start.hour = 3;
    taskList[4].start.hour = 4;

    /* Expect */
    

    /* Act */
    SCHEDULER_delete_task(1);
    TEST_ASSERT_EQUAL(2,taskList[1].start.hour);
    TEST_ASSERT_EQUAL(3,taskList[2].start.hour);
    TEST_ASSERT_EQUAL(4,taskList[3].start.hour);
    
}

void test_SCHEDULER_sort_should_sort_tasks_by_date_and_time(void){
    /* Arrange */
    taskCnt = 0;

    
    
    for(taskCnt=0;taskCnt<8;taskCnt++){
        taskList[taskCnt].taskFunction = PCO2_Run;
        taskList[taskCnt].start.year = 18;
        taskList[taskCnt].start.month = 4;
        taskList[taskCnt].start.date = 23;
        taskList[taskCnt].start.minute = 0;
        taskList[taskCnt].start.second = 0;
        taskList[taskCnt].start.hundredth = 0;
    }


    taskList[0].start.hour = 1;
    taskList[1].start.hour = 0;
    taskList[2].start.hour = 2;
    taskList[3].start.hour = 3;
    taskList[4].start.hour = 4;
    taskList[5].start.hour = 5;
    taskList[6].start.hour = 6;
    taskList[7].start.hour = 7;

    /* Expect */
    AM08X5_compare_time_ExpectAndReturn(&taskList[0].start,&taskList[1].start,true);
    AM08X5_compare_time_ExpectAndReturn(&taskList[1].start,&taskList[2].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[2].start,&taskList[3].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[3].start,&taskList[4].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[4].start,&taskList[5].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[5].start,&taskList[6].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[6].start,&taskList[7].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[0].start,&taskList[1].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[1].start,&taskList[2].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[2].start,&taskList[3].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[3].start,&taskList[4].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[4].start,&taskList[5].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[5].start,&taskList[6].start,false);
    AM08X5_compare_time_ExpectAndReturn(&taskList[6].start,&taskList[7].start,false);

    /* Act */
    SCHEDULER_sort();
    TEST_ASSERT_EQUAL(0,taskList[0].start.hour);
    TEST_ASSERT_EQUAL(1,taskList[1].start.hour);
    TEST_ASSERT_EQUAL(2,taskList[2].start.hour);
    TEST_ASSERT_EQUAL(3,taskList[3].start.hour);
    TEST_ASSERT_EQUAL(4,taskList[4].start.hour);
    TEST_ASSERT_EQUAL(5,taskList[5].start.hour);
    TEST_ASSERT_EQUAL(6,taskList[6].start.hour);
    TEST_ASSERT_EQUAL(7,taskList[7].start.hour);
}

void test_SCHEDULER_execute_task_should_run_task_and_schedule_next_if_repeating(void){
    /* Arrange */
    taskCnt = 1;
    sTimeReg_t start;  
    sTimeReg_t interval;  
    sTimeReg_t next = {.hour = 5};
    taskList[0].taskFunction = PCO2_Run;
    taskList[0].start.hour = 0;
    taskList[0].repeat = 7;


    /* Expect */
    AM08X5_clear_time_struct_Expect(&start);
    AM08X5_clear_time_struct_IgnoreArg_t();
    PCO2_Run_Expect();
    AM08X5_add_time_struct_ExpectAndReturn(&taskList[0].start,&taskList[0].period,&start,AM08X5_OK);
    AM08X5_add_time_struct_IgnoreArg_t3();
    AM08X5_add_time_struct_ReturnThruPtr_t3(&next);
    mock_create_task(&next,&taskList[0].period,true,true);

    /** Compare next alarm to current time and set */
    AM08X5_clear_time_struct_Expect(&start);
    AM08X5_clear_time_struct_IgnoreArg_t();
    AM08X5_GetTime_Expect(&start);
    AM08X5_GetTime_IgnoreArg_t();
    AM08X5_compare_time_ExpectAndReturn(&start,&start,true);
    AM08X5_compare_time_IgnoreArg_t1();
    AM08X5_compare_time_IgnoreArg_t2();
    AM08X5_AlarmSet_Expect(&RTC.nIRQ1,&taskList[0].start);

    /* Act */
    SCHEDULER_execute_task();
    TEST_ASSERT_EQUAL(5,taskList[0].start.hour);
    TEST_ASSERT_EQUAL(6,taskList[0].repeat);
}


void test_SCHEDULER_execute_task_should_run_final_fast_mode_and_schedule_normal_mode(void){
    /* Arrange */
    taskCnt = 1;
    sTimeReg_t start;  
    sTimeReg_t interval = {.hour=3};  
    sTimeReg_t next = {.hour = 5};
    taskList[0].taskFunction = PCO2_FastMode;
    taskList[0].start.hour = 0;
    taskList[0].repeat = 0;



    /* Expect */
    // AM08X5 Clear Time Struct
    AM08X5_clear_time_struct_Expect(&start);
    AM08X5_clear_time_struct_IgnoreArg_t();

    // Run the Function
    PCO2_FastMode_Expect();

    // PCO2 Get Normal Start Time
    PCO2_GetNormalStartTime_Expect(&next);
    PCO2_GetNormalStartTime_IgnoreArg_t();
    PCO2_GetNormalStartTime_ReturnThruPtr_t(&next);

    // PCO2 Get Normal Interval
    PCO2_GetNormalInterval_Expect(&interval);
    PCO2_GetNormalInterval_IgnoreArg_t();
    PCO2_GetNormalInterval_ReturnThruPtr_t(&interval);
    
    // SCHEDULER Create Task
    mock_create_task(&next,&interval,false,true);
    
    // SCHEDULER Delete Task
    AM08X5_clear_time_struct_Expect(&start);
    AM08X5_clear_time_struct_IgnoreArg_t();

    // AM08X5 Get Time
    AM08X5_GetTime_Expect(&start);
    AM08X5_GetTime_IgnoreArg_t();

    // AM08X5 Compare Time
    AM08X5_compare_time_ExpectAndReturn(&start,&start,true);
    AM08X5_compare_time_IgnoreArg_t1();
    AM08X5_compare_time_IgnoreArg_t2();

    // AM08X5 Alarm Set
    AM08X5_AlarmSet_Expect(&RTC.nIRQ1,&taskList[0].start);

    /* Act */

    SCHEDULER_execute_task();

}