#include "unity.h"
#include "statistics.h"
#include "float.h"
#define UNITY_INCLUDE_FLOAT

#define LENGTH_OF(x)				(sizeof(x)/sizeof(x[0]))

void setUp(void)
{
}

void tearDown(void)
{
}


void test_STATS_CalculateMean_should_AcceptValidEntries(void)
{
	float value[] = {1.0,3.1,5.4};
	uint16_t length = 0;
	float mean = 0.0;
	
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value[0],LENGTH_OF(value),&mean));
	
}


void test_STATS_CalculateMean_should_RejectBadLength(void)
{
	float value[] = {1.0};
	uint16_t length = 0;
	float mean = 0.0;
	
	TEST_ASSERT_EQUAL(1, STATS_CalculateMean(&value[0],0,&mean));
	
}

void test_STATS_CalculateMean_should_CalculateMeanOfNumbers(void)
{
	float value1[] = {1.0,1.0,1.0};
	float value2[] = {1.0,2.0,3.0};
	float value3[] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
	float value4[] = {0.0,1000};
	float value5[] = {10.0,3.0,20.0,2.0,100.0,1.0};
	uint16_t length = 0;
	float mean = 0.0;


	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value1[0],LENGTH_OF(value1),&mean));
	TEST_ASSERT_EQUAL_FLOAT(1.0,mean);
	
	mean = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value2[0],LENGTH_OF(value2),&mean));
	TEST_ASSERT_EQUAL_FLOAT(2.0,mean);
	
	mean = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value3[0],LENGTH_OF(value3),&mean));
	TEST_ASSERT_EQUAL_FLOAT(3.0,mean);
	
	mean = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value4[0],LENGTH_OF(value4),&mean));
	TEST_ASSERT_EQUAL_FLOAT(500.0,mean);
	
	mean = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value5[0],LENGTH_OF(value5),&mean));
	TEST_ASSERT_FLOAT_WITHIN(0.01,22.666,mean);
}



void test_STATS_FindMax_should_AcceptValidEntries(void)
{
	float value[] = {1.0,3.1,5.4};
	uint16_t length = 0;
	float max = 0.0;
	
	TEST_ASSERT_EQUAL(0, STATS_FindMax(&value[0],LENGTH_OF(value),&max));
	
}

void test_STATS_FindMax_should_RejectBadLength(void)
{
	
	float value[] = {1.0};
	uint16_t length = 0;
	float max = 0.0;
	
	TEST_ASSERT_EQUAL(1, STATS_FindMax(&value[0],0,&max));
}


void test_STATS_FindMax_should_FindTheMaxValue(void)
{
	float value1[] = {1.0,1.1,1.2,1.3};
	float value2[] = {-181.9,-29.5,274.1,999.9};
	float value3[] = {9.9,9.99,9.999,.99,9.9999,9.9,9.9,9.99};
	float value4[] = {8398234.1893,22.102,81.180, -109834,29};
	uint16_t length = 0;
	float max = 0;
	
	TEST_ASSERT_EQUAL(0, STATS_FindMax(&value1[0],LENGTH_OF(value1),&max));
	TEST_ASSERT_EQUAL_FLOAT(1.3,max);
	
	max = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_FindMax(&value2[0],LENGTH_OF(value2),&max));
	TEST_ASSERT_EQUAL_FLOAT(999.9,max);
	
	max = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_FindMax(&value3[0],LENGTH_OF(value3),&max));
	TEST_ASSERT_EQUAL_FLOAT(9.9999,max);
	
	max = 0.0;
	TEST_ASSERT_EQUAL(0, STATS_FindMax(&value4[0],LENGTH_OF(value4),&max));
	TEST_ASSERT_EQUAL_FLOAT(8398234.1893,max);
	

}


void test_STATS_FindMin_should_AcceptValidEntries(void)
{
	float value[] = {1.0,3.1,5.4};
	float min = 0.0;
	
	TEST_ASSERT_EQUAL(0, STATS_FindMin(&value[0],LENGTH_OF(value),&min));
}


void test_STATS_FindMin_should_RejectBadLength(void)
{
	
	float value[] = {1.0};
	uint16_t length = 0;
	float max = 0.0;
	
	TEST_ASSERT_EQUAL(1, STATS_FindMin(&value[0],0,&max));
}

void test_STATS_FindMin_should_FindTheMinValue(void)
{
	float value1[] = {5};
	float value2[] = {1,999};
	float value3[] = {-5, -918.1, 28502,99999,-98783.92};
	float value4[] = {87923.6664,10987.748763,198271.23,1823.11,-1791,-28371,23897.32,717840.977,-1739752.3232};
	
	float min = 0;
	
	TEST_ASSERT_EQUAL(0, STATS_FindMin(&value1[0],LENGTH_OF(value1),&min));
	TEST_ASSERT_EQUAL_FLOAT(5,min);
	
	min = 0;
	TEST_ASSERT_EQUAL(0, STATS_FindMin(&value2[0],LENGTH_OF(value2),&min));
	TEST_ASSERT_EQUAL_FLOAT(1,min);
	
	min = 0;
	TEST_ASSERT_EQUAL(0, STATS_FindMin(&value3[0],LENGTH_OF(value3),&min));
	TEST_ASSERT_EQUAL_FLOAT(-98783.92,min);
	
	min = 0;
	TEST_ASSERT_EQUAL(0, STATS_FindMin(&value4[0],LENGTH_OF(value4),&min));
	TEST_ASSERT_EQUAL_FLOAT(-1739752.3232,min);
	
	

}


void test_STATS_ComputeSTD_should_AcceptValidEntries(void)
{
	float value[] = {1,1,1};
	float std = 0;
	
	TEST_ASSERT_EQUAL(0, STATS_ComputeSTD(&value[0],LENGTH_OF(value),1,&std));
}

void test_STATS_ComputeSTD_should_RejectBadLengths(void)
{
	float value[] = {1.0};
	uint16_t length = 0;
	float max = 0.0;
	
	TEST_ASSERT_EQUAL(1, STATS_FindMin(&value[0],length,&max));
	
}

void test_STATS_ComputeSTD_should_ComputeValidSTD(void)
{
	float value1[] = {1.0,1.0,1.0};
	float value2[] = {99.9,100.0,100.1};
	float value3[] = {1000,2000,2000,3000};
	float value4[] = {-8543.2,-7543.8,-8323.5,-8683.124,-8342.832};
	float mean = 0;
	float std = 0;
	
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value1[0],LENGTH_OF(value1),&mean));
	TEST_ASSERT_EQUAL(0, STATS_ComputeSTD(&value1[0],LENGTH_OF(value1),mean,&std));
	TEST_ASSERT_FLOAT_WITHIN(0.1,0.0,std);
	
	mean = 0;
	std = 0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value2[0],LENGTH_OF(value2),&mean));
	TEST_ASSERT_EQUAL(0, STATS_ComputeSTD(&value2[0],LENGTH_OF(value2),mean,&std));
	TEST_ASSERT_FLOAT_WITHIN(0.05,0.1,std);
	
	mean = 0;
	std = 0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value3[0],LENGTH_OF(value3),&mean));
	TEST_ASSERT_EQUAL(2000,mean);
	TEST_ASSERT_EQUAL(0, STATS_ComputeSTD(&value3[0],LENGTH_OF(value3),mean,&std));
	TEST_ASSERT_FLOAT_WITHIN(0.5,707.1,std);

	mean = 0;
	std = 0;
	TEST_ASSERT_EQUAL(0, STATS_CalculateMean(&value4[0],LENGTH_OF(value4),&mean));
	TEST_ASSERT_EQUAL(-8287.29,mean);
	TEST_ASSERT_EQUAL(0, STATS_ComputeSTD(&value4[0],LENGTH_OF(value4),mean,&std));
	TEST_ASSERT_FLOAT_WITHIN(0.5,394.8,std);
	
	
}