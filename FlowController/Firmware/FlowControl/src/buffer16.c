/** @file Buffer16.c
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

 #include "buffer16.h"
 
 /*******************************************************************************
 *					STATIC FUNCTIONS PROTOTYPES
 *******************************************************************************/
 STATIC uint16_t Buffer16_Scan(sCircularBuffer16_t *buf,uint16_t val);
 STATIC void Buffer16_Size(sCircularBuffer16_t *buf);
 STATIC uint16_t Buffer16_NextIndex(uint16_t idx);
 STATIC uint16_t Buffer16_PrevIndex(uint16_t idx);
 /*******************************************************************************
 *					GLOBAL FUNCTIONS
 *******************************************************************************/
 void Buffer16_Clear(sCircularBuffer16_t *buf)
 {
     uint16_t i = 0;
 
     buf->read = 0;
     buf->write = 0;
     memset(buf,0,ACTUAL_BUFFER_16_SIZE);
 
 
     return;
 }
 
 
 eBuffer16Status_t	Buffer16_put(sCircularBuffer16_t *buf, uint16_t val){
     eBuffer16Status_t result = BUFFER_16_ERROR;
     
     /** Determine the index to write to */
     uint16_t NextWrite = Buffer16_NextIndex(buf->write);
     
     /** If the next index is equal to the read index, the buffer is full */
     if(NextWrite == buf->read){
         result = BUFFER_16_FULL;
     } else {
         /** Write the value to the buffer */
         buf->buffer[buf->write] = val;
         /** Update the index to the next value */
         buf->write = NextWrite;
         /** Update the buffer size */
         Buffer16_Size(buf);
         /** Update the result */
         result = BUFFER_16_OK;
     }
     return result;
 }
 
 eBuffer16Status_t Buffer16_get(sCircularBuffer16_t *buf, uint16_t *value){
     eBuffer16Status_t result = BUFFER_16_ERROR;
     uint16_t NextRead = buf->read;
 
     /** If buffer read index is not == to write index, then there is data */
     /** otherwise it is empty */
     if(buf->read != buf->write){
         /** Read the buffer Value */
         *value = buf->buffer[buf->read];
         /** Clear the current buffer value */
         buf->buffer[buf->read] = 0;
         /** Update the Index */
         buf->read = Buffer16_NextIndex(buf->read);
         /** Update the buffer size */
         Buffer16_Size(buf);
         /** Update the result */
         result = BUFFER_16_OK;
 
     } else {
         result = BUFFER_16_EMPTY;
     }
     return result;
 }
 
 
 eBuffer16Status_t Buffer16_Backspace(sCircularBuffer16_t *buf){
     eBuffer16Status_t result = BUFFER_16_ERROR;
 
     if(buf->write == buf->read){
         result = BUFFER_16_EMPTY;
     } else {
         buf->write = Buffer16_PrevIndex(buf->write);
         buf->buffer[buf->write] = 0;
         Buffer16_Size(buf);
         result = BUFFER_16_OK;
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
 STATIC uint16_t Buffer16_Scan(sCircularBuffer16_t *buf,uint16_t val){
     uint16_t CountIdx = 0;
     uint16_t ReadIdx = buf->read;
     uint16_t WriteIdx = buf->write;
 
     while(ReadIdx != WriteIdx){
         CountIdx++;
         if(buf->buffer[ReadIdx] == val){
             break;
         }
         ReadIdx = Buffer16_NextIndex(ReadIdx);
         
     }
 
     if(ReadIdx == WriteIdx){
         CountIdx = 0;
     }
     
     return CountIdx;
 
  }
 
 
 /** @brief Compute the buffer size
  *
  * Computes the size of the allocated buffer and saves it
  * in the structure
  *
  * @param *buf Pointer to the Circular Buffer
  *
  * @return None
  */
 STATIC void Buffer16_Size(sCircularBuffer16_t *buf){
     if(buf->write == buf->read){
       buf->size = 0;
     } else if(buf->write > buf->read){
       buf->size = (buf->write - buf->read);
     } else {
       buf->size = BUFFER_16_SIZE - buf->read + buf->write + 1;
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
 STATIC uint16_t Buffer16_NextIndex(uint16_t idx){
     
     if(++idx == ACTUAL_BUFFER_16_SIZE){
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
 STATIC uint16_t Buffer16_PrevIndex(uint16_t idx){
     if(idx == 0){
         idx = ACTUAL_BUFFER_16_SIZE -1;
     } else {
         idx--;
     }
     return idx;
 }