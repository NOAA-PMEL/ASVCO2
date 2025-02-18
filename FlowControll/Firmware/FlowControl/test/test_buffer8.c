#include "unity.h"
#include "buffer8.h"

sCircularBuffer8_t TestBuf;

void setUp(void)
{
	Buffer8_Clear(&TestBuf);
}

void tearDown(void)
{
}


void test_Buffer8_Put_should_AddData_to_Buffer(void){
	uint8_t i;
	uint8_t testval = 0x84;

	// printf("%d,%d,%d\n",TestBuf.size,TestBuf.read,TestBuf.write);
	for(i=0;i<BUFFER_8_SIZE;i++){
		// printf("%d: %d,%d,%d\n",i,TestBuf.size,TestBuf.read,TestBuf.write);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}
	
}

void test_Buffer8_Put_should_Block_when_BufferFull(void){
	uint8_t i;
	uint8_t testval = 0x70;

	/** Add a bunch of data and see if the buffer is valid */
	for(i=0;i<BUFFER_8_SIZE;i++){
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}

	/** Try to add more data, should block */
	TEST_ASSERT_EQUAL(BUFFER_8_FULL,Buffer8_put(&TestBuf,'9'));
	TEST_ASSERT_EQUAL(BUFFER_8_FULL,Buffer8_put(&TestBuf,'8'));
	TEST_ASSERT_EQUAL(BUFFER_8_FULL,Buffer8_put(&TestBuf,'7'));
	TEST_ASSERT_EQUAL(BUFFER_8_FULL,Buffer8_put(&TestBuf,'6'));

	/** Make sure the original data is there */
	testval = 0x70;
	for(i=0;i<BUFFER_8_SIZE;i++){
		TEST_ASSERT_EQUAL(testval,TestBuf.buffer[i]);
		testval++;
	}
}



void test_Buffer8_get_should_Return_Validuint16_t_NoError(void){

	uint8_t i;
	uint8_t testval = 0x0B;
	uint8_t retval = 0;

	/** Set the buffer with chars */
	for(i=0;i<BUFFER_8_SIZE;i++){
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		testval++;
	}

	/** Reset the test value */
	testval = 0x0B;

	/** Retreive the buffer values */
	for(i=0;i<BUFFER_8_SIZE;i++){
		
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_get(&TestBuf,&retval));
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}
}

void test_Buffer8_put_should_Wrap_when_StartingFromHigherIndex(void){
	uint8_t i;
	uint8_t testval = 0;
	uint8_t retval = 0;
	uint8_t teststr[ACTUAL_BUFFER_8_SIZE];
	uint8_t testlen = 20;

	/** Partially set the buffer (5 deep) with chars */
	for(i=0;i<testlen;i++){
		testval = (i+0x0A);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		testval++;
	}
	/** Retreive those 5 values so the write index is greater than 0 */
	// strncpy(teststr,TestBuf.buffer,BUFFER_8_SIZE);
	// printf("%s\n",teststr);
	for(i=0;i<testlen;i++){
		testval = (i+0x0A);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_get(&TestBuf,&retval));
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}


	/** Set the write and read index to a middle register */
	TestBuf.write = testlen;
	TestBuf.read = testlen;

	/** Write a full buffer worth, should wrap after index = BUFFER_8_SIZE */
	for(i=0;i<BUFFER_8_SIZE;i++){
		testval = (i);
		// printf("%d: %u,%u,%c\n",i,TestBuf.read,TestBuf.write,testval);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		testval++;
	}

	/** Retreive the buffer values to see if wrap is successful*/
	for(i=0;i<BUFFER_8_SIZE;i++){
		
		testval = (i);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_get(&TestBuf,&retval));
		// printf("%c,",retval);
		// printf("%d: %c,%c\n",i,testval,retval);
		// printf("%d,%d\n",TestBuf.read,TestBuf.write);
		TEST_ASSERT_EQUAL(testval,retval);
		testval++;
	}


}


void test_Buffer8_Backspace_should_RemoveOneChar(void){
	uint8_t i;
	uint8_t testval = 0;
	uint8_t testlen = 5;

	/** Partially set the buffer (5 deep) with chars */
	for(i=0;i<testlen;i++){
		testval = (i+0xAB);
		TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
		testval++;
	}

	TEST_ASSERT_EQUAL(testlen,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(testlen-1,TestBuf.size);

	
}

void test_Buffer8_Backspace_should_Wrap_and_RemoveOneChar(void){
	uint8_t i;
	uint8_t testval = 0x4F;
	uint8_t testlen = 5;

	TestBuf.write = BUFFER_8_SIZE-1;
	TestBuf.read = BUFFER_8_SIZE-1;

	
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_put(&TestBuf,testval));
	TEST_ASSERT_EQUAL(3,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(2,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(1,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_8_OK,Buffer8_Backspace(&TestBuf));
	TEST_ASSERT_EQUAL(0,TestBuf.size);
	TEST_ASSERT_EQUAL(BUFFER_8_EMPTY,Buffer8_Backspace(&TestBuf));

}





