#include "unity.h"
#include "statistics.h"

sStats_F_t Data;

void setUp(void)
{

}

void tearDown(void)
{
}







// void test_STATS_Clear_should_ClearTheStruct(void){

//     Data.numSamples = 5555;
//     Data.sum = 4.4f;
//     Data.M2 = 8.8f;
//     Data.mean = 1.1f;
//     Data.std = 2.2f;
//     Data.min = 3.3f;
//     Data.max = 4.4f;
//     float zero = 0.0f;

//     STATS_Clear(&Data);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.sum);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.M2);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.mean);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.std);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.min);
//     TEST_ASSERT_EQUAL_FLOAT(zero,Data.max);
//     TEST_ASSERT_EQUAL(0u,Data.numSamples);
// }


// void test_STATS_Add_should_AddToSum(void){

//     STATS_Clear(&Data);
//     float addval = 1.1f;

//     STATS_Add(&Data,addval);
//     TEST_ASSERT_EQUAL_FLOAT(addval,Data.sum);
//     TEST_ASSERT_EQUAL(1,Data.numSamples);

// }


// void test_STATS_Add_should_Find_MinMax(void){
//     STATS_Clear(&Data);     
//     float addval = 1.8f;

//     STATS_Add(&Data,addval);
//     TEST_ASSERT_EQUAL_FLOAT(addval,Data.min);
//     TEST_ASSERT_EQUAL_FLOAT(addval,Data.max);

//     addval = 1.3f;
//     STATS_Add(&Data,addval);
//     TEST_ASSERT_EQUAL_FLOAT(addval,Data.min);

//     addval = 2.8f;
//     STATS_Add(&Data,addval);
//     TEST_ASSERT_EQUAL_FLOAT(addval,Data.max);

// }


// void test_STATS_Computer_should_ComputeMean_and_STD(void){

//     STATS_Clear(&Data);
//     Data.sum = 101.0f;
//     Data.M2 = 33.3f;
//     Data.numSamples = 10;

//     STATS_Compute(&Data);
//     TEST_ASSERT_EQUAL_FLOAT(10.1f,Data.mean);
//     TEST_ASSERT_EQUAL_FLOAT(3.33f,Data.std);
// }


// void test_STATS_Compute_should_PutNAN_for_numSamplesEqualsZero(void){
//     STATS_Clear(&Data);
//     Data.sum = 101.0f;
//     Data.M2 = 33.3f;
//     Data.numSamples = 0;

//     STATS_Compute(&Data);
//     TEST_ASSERT_EQUAL_FLOAT(NAN,Data.mean);
//     TEST_ASSERT_EQUAL_FLOAT(NAN,Data.std);
// }


// void test_STATS_Clear_U32_should_ClearAll(void){
//     sStats_U32_t stat = {   .numSamples = 12,
//                             .mean = 328,
//                             .M2 = 6823,
//                             .std = 2,
//                             .sum = 128102,
//                             .min = 52,
//                             .max = 37};
//     STATS_Clear_U32(&stat);
//     TEST_ASSERT_EQUAL(0,stat.numSamples);
//     TEST_ASSERT_EQUAL(0,stat.mean);
//     TEST_ASSERT_EQUAL(0,stat.M2);
//     TEST_ASSERT_EQUAL(0,stat.std);
//     TEST_ASSERT_EQUAL(0,stat.sum);
//     TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF,stat.min);
//     TEST_ASSERT_EQUAL(0,stat.max);
    


// }


// void test_STATS_AddSample_should_UpdateStruct_EasyCase(void){
//     sStats_U32_t stat = {   .numSamples = 0,
//                             .mean = 0,
//                             .M2 = 0,
//                             .std = 0,
//                             .sum = 0,
//                             .min = 0,
//                             .max = 0};
    
//     // Arrange
//     uint32_t nums[8] = {0,2,4,6,8,10,12,14};
//     uint8_t cnt = 0;
//     uint32_t mins[8] = {0,0,0,0,0,0,0,0};
//     uint32_t means[8] = {0,1,2,3,4,5,6,7};
//     uint32_t M2[8] = {0,0,0,0,0,0,0,0};
//     uint8_t i,j;
//     uint64_t temp;

//     STATS_Clear_U32(&stat);
//     // Expect


//     // Act
//     for(i=0;i<8;i++){
//         // printf("%u ",i);
//         STATS_AddSample_U32(&stat,nums[i]);
//         TEST_ASSERT_EQUAL(nums[i],stat.max);
//         TEST_ASSERT_EQUAL(mins[i],stat.min);
//         TEST_ASSERT_UINT32_WITHIN(1,means[i],stat.mean);
//     }


// }


// void test_STATS_AddSample_should_UpdateStruct_HarderCase(void){
//     sStats_U32_t stat = {   .numSamples = 0,
//                             .mean = 0,
//                             .M2 = 0,
//                             .std = 0,
//                             .sum = 0,
//                             .min = 0,
//                             .max = 0};
    
//     // Arrange
//     uint32_t nums[8] = {14,12,10,8,6,4,2,0};
//     uint8_t cnt = 0;
//     uint32_t mins[8] = {0,0,0,0,0,0,0,0};
//     uint32_t means[8] = {14,12,5,4,3,2,1,0};
//     uint32_t M2[8] = {0,0,0,0,0,0,0,0};
//     uint8_t i,j;
//     uint64_t temp;
//     STATS_Clear_U32(&stat);
//     // Expect


//     // Act
//     for(i=0;i<8;i++){
//         printf("%u ",i);
//         STATS_AddSample_U32(&stat,nums[i]);
//         TEST_ASSERT_EQUAL(nums[0],stat.max);
//         TEST_ASSERT_EQUAL(nums[i],stat.min);
//         TEST_ASSERT_UINT32_WITHIN(1,means[i],stat.mean);
//     }


// }


void test_STATS_Clear_I32_should_ClearAll(void){
    sStats_I32_t stat = {   .numSamples = 12,
                            .mean = 328,
                            .M2 = 6823,
                            .std = -2,
                            .sum = 128102,
                            .min = -52,
                            .max = 37};
    STATS_Clear_I32(&stat);
    TEST_ASSERT_EQUAL(0,stat.numSamples);
    TEST_ASSERT_EQUAL(0,stat.mean);
    TEST_ASSERT_EQUAL(0,stat.M2);
    TEST_ASSERT_EQUAL(0,stat.std);
    TEST_ASSERT_EQUAL(0,stat.sum);
    TEST_ASSERT_EQUAL_INT32(0x7FFFFFFF,stat.min);
    TEST_ASSERT_EQUAL(0,stat.max);
    
}

void test_STATS_AddSample_should_UpdateStruct_EasyCase(void){
    sStats_U32_t stat = {   .numSamples = 0,
                            .mean = 0,
                            .M2 = 0,
                            .std = 0,
                            .sum = 0,
                            .min = 0,
                            .max = 0};
    
    // Arrange
    int32_t nums[8] = {0,2,4,6,8,10,12,14};
    int8_t cnt = 0;
    int32_t mins[8] = {0,0,0,0,0,0,0,0};
    int32_t means[8] = {0,1,2,3,4,5,6,7};
    int32_t M2[8] = {0,0,0,0,0,0,0,0};
    int8_t i,j;
    int64_t temp;

    STATS_Clear_I32(&stat);
    // Expect


    // Act
    for(i=0;i<8;i++){
        // printf("%u ",i);
        STATS_AddSample_I32(&stat,nums[i]);
        TEST_ASSERT_EQUAL(nums[i],stat.max);
        TEST_ASSERT_EQUAL(mins[i],stat.min);
        TEST_ASSERT_INT32_WITHIN(1,means[i],stat.mean);
    }
}