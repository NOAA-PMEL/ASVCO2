/** @file buffer8.c
 *  @brief uint16_tacter circular buffer library
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date Dec 4, 2015
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

 #include "buffer8.h"
 
 /*******************************************************************************
 *					STATIC FUNCTIONS PROTOTYPES
 *******************************************************************************/
#ifdef BUFFER8_SCAN
 STATIC uint16_t Buffer8_Scan(sCircularBuffer8_t *buf,uint16_t val);
#endif
 STATIC void Buffer8_Size(sCircularBuffer8_t *buf);
 STATIC uint16_t Buffer8_NextIndex(uint16_t idx);
 STATIC uint16_t Buffer8_PrevIndex(uint16_t idx);
 /*******************************************************************************
 *					GLOBAL FUNCTIONS
 *******************************************************************************/
 void Buffer8_Clear(sCircularBuffer8_t *buf)
 { 
     buf->read = 0;
     buf->write = 0;
     memset(buf,0,ACTUAL_BUFFER_8_SIZE);
 
 
     return;
 }
 
 
 eBuffer8Status_t	Buffer8_put(sCircularBuffer8_t *buf, uint8_t val){
     eBuffer8Status_t result = BUFFER_8_ERROR;
     
     /** Determine the index to write to */
     uint16_t NextWrite = Buffer8_NextIndex(buf->write);
     
     /** If the next index is equal to the read index, the buffer is full */
     if(NextWrite == buf->read){
         result = BUFFER_8_FULL;
     } else {
         /** Write the value to the buffer */
         buf->buffer[buf->write] = val;
         /** Update the index to the next value */
         buf->write = NextWrite;
         /** Update the buffer size */
         Buffer8_Size(buf);
         /** Update the result */
         result = BUFFER_8_OK;
     }
     return result;
 }
 
 eBuffer8Status_t Buffer8_get(sCircularBuffer8_t *buf, uint8_t *value){
     eBuffer8Status_t result = BUFFER_8_ERROR;
 
     /** If buffer read index is not == to write index, then there is data */
     /** otherwise it is empty */
     if(buf->read != buf->write){
         /** Read the buffer Value */
         *value = buf->buffer[buf->read];
         /** Clear the current buffer value */
         buf->buffer[buf->read] = 0;
         /** Update the Index */
         buf->read = Buffer8_NextIndex(buf->read);
         /** Update the buffer size */
         Buffer8_Size(buf);
         /** Update the result */
         result = BUFFER_8_OK;
 
     } else {
         result = BUFFER_8_EMPTY;
     }
     return result;
 }
 
 
 eBuffer8Status_t Buffer8_Backspace(sCircularBuffer8_t *buf){
     eBuffer8Status_t result = BUFFER_8_ERROR;
 
     if(buf->write == buf->read){
         result = BUFFER_8_EMPTY;
     } else {
         buf->write = Buffer8_PrevIndex(buf->write);
         buf->buffer[buf->write] = 0;
         Buffer8_Size(buf);
         result = BUFFER_8_OK;
     }
 
     return result;
 }
 
 
 
 
 /*******************************************************************************
 *					STATIC FUNCTIONS 
 *******************************************************************************/
 /** @brief Scan buffer for value
  *
  * Scans the buffer for a specified value, returns the number of uint16_t value from
  * start of string through location of specified value.
  *
  * @param *buf Pointer to the Circular Buffer
  * @param val Value to scan for
  *
  * @return len Length of string
  */
#ifdef BUFFER8_SCAN
 STATIC uint16_t Buffer8_Scan(sCircularBuffer8_t *buf,uint16_t val){
     uint16_t CountIdx = 0;
     uint16_t ReadIdx = buf->read;
     uint16_t WriteIdx = buf->write;
 
     while(ReadIdx != WriteIdx){
         CountIdx++;
         if(buf->buffer[ReadIdx] == val){
             break;
         }
         ReadIdx = Buffer8_NextIndex(ReadIdx);
         
     }
 
     if(ReadIdx == WriteIdx){
         CountIdx = 0;
     }
     
     return CountIdx;
 
  }
#endif
 
 /** @brief Compute the buffer size
  *
  * Computes the size of the allocated buffer and saves it
  * in the structure
  *
  * @param *buf Pointer to the Circular Buffer
  *
  * @return None
  */
 STATIC void Buffer8_Size(sCircularBuffer8_t *buf){
     if(buf->write == buf->read){
       buf->size = 0;
     } else if(buf->write > buf->read){
       buf->size = (buf->write - buf->read);
     } else {
       buf->size = BUFFER_8_SIZE - buf->read + buf->write + 1;
     }
   }
   
 
 /** @brief Compute the next index
  *
  * Computes the next index value for the buffer 
  *
  * @param idx Index to increment
  *
  * @return Compensated Index
  */
 STATIC uint16_t Buffer8_NextIndex(uint16_t idx){
     
     if(++idx == ACTUAL_BUFFER_8_SIZE){
         idx= 0;
     }
     return idx;
   }
 
 /** @brief Compute the previous index
  *
  * Computes the previous index value for the buffer 
  *
  * @param idx Index to decrement
  *
  * @return Compensated Index
  */
 STATIC uint16_t Buffer8_PrevIndex(uint16_t idx){
     if(idx == 0){
         idx = ACTUAL_BUFFER_8_SIZE -1;
     } else {
         idx--;
     }
     return idx;
 }