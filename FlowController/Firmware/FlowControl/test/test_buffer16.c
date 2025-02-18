#include "unity.h"
#include "buffer16.h"

sCircularBuffer16_t TestBuf;

void setUp(void)
{
	Buffer16_Clear(&TestBuf);
}

void tearDown(void)
{
}


void test_Buffer16_Put_should_AddData_to_Buffer(void){
	uint8_t i;
	uint16_t testval = 0x8844;

	// printf("%d,%d,%d\n",TestBuf.size,TestBuf.read,TestBuf.write);
	for(i=0;i<BUFFER_16_SIZE;i++){
		// printf("%d: %d,%d,%d\n",i,TestBuf.size,TestBuf.read,TestBuf.write);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}
	
}

void test_Buffer16_Put_should_Block_when_BufferFull(void){
	uint8_t i;
	uint16_t testval = 0x7070;

	/** Add a bunch of data and see if the buffer is valid */
	for(i=0;i<BUFFER_16_SIZE;i++){
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}

	/** Try to add more data, should block */
	TEST_ASSERT_EQUAL(BUFFER_16_FULL,Buffer16_put(&TestBuf,'9'));
	TEST_ASSERT_EQUAL(BUFFER_16_FULL,Buffer16_put(&TestBuf,'8'));
	TEST_ASSERT_EQUAL(BUFFER_16_FULL,Buffer16_put(&TestBuf,'7'));
	TEST_ASSERT_EQUAL(BUFFER_16_FULL,Buffer16_put(&TestBuf,'6'));

	/** Make sure the original data is there */
	testval = 0x7070;
	for(i=0;i<BUFFER_16_SIZE;i++){
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}
}



void test_Buffer16_get_should_Return_Validuint16_t_NoError(void){

	uint8_t i;
	uint16_t testval = 0x0B00;
	uint16_t retval = 0;

	/** Set the buffer with chars */
	for(i=0;i<BUFFER_16_SIZE;i++){
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		testval++;
	}

	/** Reset the test value */
	testval = 0x0B00;

	/** Retreive the buffer values */
	for(i=0;i<BUFFER_16_SIZE;i++){
		
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_get(&TestBuf,&retval));
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}
}

void test_Buffer16_put_should_Wrap_when_StartingFromHigherIndex(void){
	uint8_t i;
	uint16_t testval = 0;
	uint16_t retval = 0;
	uint16_t teststr[ACTUAL_BUFFER_16_SIZE];
	uint8_t testlen = 20;

	/** Partially set the buffer (5 deep) with chars */
	for(i=0;i<testlen;i++){
		testval = (i+0x000A);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		testval++;
	}
	/** Retreive those 5 values so the write index is greater than 0 */
	// strncpy(teststr,TestBuf.buffer,BUFFER_16_SIZE);
	// printf("%s\n",teststr);
	for(i=0;i<testlen;i++){
		testval = (i+0x000A);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_get(&TestBuf,&retval));
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}


	/** Set the write and read index to a middle register */
	TestBuf.write = testlen;
	TestBuf.read = testlen;

	/** Write a full buffer worth, should wrap after index = BUFFER_16_SIZE */
	for(i=0;i<BUFFER_16_SIZE;i++){
		testval = (i);
		// printf("%d: %u,%u,%c\n",i,TestBuf.read,TestBuf.write,testval);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		testval++;
	}

	/** Retreive the buffer values to see if wrap is successful*/
	for(i=0;i<BUFFER_16_SIZE;i++){
		
		testval = (i);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_get(&TestBuf,&retval));
		// printf("%c,",retval);
		// printf("%d: %c,%c\n",i,testval,retval);
		// printf("%d,%d\n",TestBuf.read,TestBuf.write);
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}


}


void test_Buffer16_Backspace_should_RemoveOneChar(void){
	uint8_t i;
	uint16_t testval = 0;
	uint8_t testlen = 5;

	/** Partially set the buffer (5 deep) with chars */
	for(i=0;i<testlen;i++){
		testval = (i+0xA00B);
		TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
		testval++;
	}

	TEST_ASSERT_EQUAL(testlen,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(testlen-1,TestBuf.size);

	
}

void test_Buffer16_Backspace_should_Wrap_and_RemoveOneChar(void){
	uint8_t i;
	uint16_t testval = 0x45EF;
	uint8_t testlen = 5;

	TestBuf.write = BUFFER_16_SIZE-1;
	TestBuf.read = BUFFER_16_SIZE-1;

	
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(3,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(2,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(1,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_16_OK,Buffer16_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(0,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_16_EMPTY,Buffer16_Backspace(&TestBuf));

}





