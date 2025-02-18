/** @file Buffer8.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Dec 4, 2017
 *  @version
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

#ifndef _BUFFER_8_H_
#define _BUFFER_8_H_

/** Remove STATIC and PERSISTENT values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
#endif
#ifndef PERSISTENT
#define PERSISTENT
#endif
#else
#ifndef STATIC
#define STATIC static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent
#endif
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include <stdint.h>
#include <string.h>

/************************************************************************
 *						STANDARD LIBRARIES
 ************************************************************************/

/************************************************************************
 *							MACROS
 ************************************************************************/

#define BUFFER_8_SIZE (128)
#define ACTUAL_BUFFER_8_SIZE (BUFFER_8_SIZE + 1)

typedef enum
{
    BUFFER_8_OK = 0x00u,              /**< Buffer is OK */
    BUFFER_8_ERROR = 0x01u,           /**< Buffer Failed */
    BUFFER_8_FULL = 0x02u,            /**< Buffer is Full */
    BUFFER_8_EMPTY = 0x04u,           /**< Buffer is Empty */
    BUFFER_8_STRING_TOO_LONG = 0x08u, /**< String is too long to use*/
    BUFFER_8_NO_STRING = 0x10u,       /**< No String to return */

} eBuffer8Status_t;

typedef struct _CircularBuffer8_t
{
    uint8_t buffer[ACTUAL_BUFFER_8_SIZE];
    uint16_t size;
    uint16_t read;
    uint16_t write;

} sCircularBuffer8_t;

/************************************************************************
 *					GLOBAL FUNCTION PROTOTYPES
 ************************************************************************/
/** @brief Clear the buffer
 *
 *	Clear the unsigned 8-bit value buffer
 *
 *  @param *buf pointer to the unsigned 8-bit value buffer
 *
 *  @return None
 */
void Buffer8_Clear(sCircularBuffer8_t *buf);

/** @brief Add data to buffer
 *
 *	Adds a single unsigned 8-bit value to the buffer
 *
 *  @param *buf pointer to the unsigned 8-bit value buffer
 *  @param value unsigned 8-bit value value to add
 *
 *  @return result
 */
eBuffer8Status_t Buffer8_put(sCircularBuffer8_t *buf, uint8_t val);

/** @brief Write Line (Put String)
 *
 *  Writes a string
 *
 * @param *buf pointer to the char buffer
 * @param *val string to write
 * @param len Length of string to write
 *
 * @return result
 */
eBuffer8Status_t Buffer8_puts(sCircularBuffer8_t *buf, uint8_t *val, uint16_t len);

/** @brief Get unsigned 8-bit value from buffer
 *
 *	Retreives one unsigned 8-bit value from the buffer
 *
 *  @param *buf pointer to the unsigned 8-bit value buffer
 *  @param *value pointer to the unsigned 8-bit value variable
 *
 *  @return result
 */
eBuffer8Status_t Buffer8_get(sCircularBuffer8_t *buf, uint8_t *value);

/** @brief Get "string" of 8-bit value from buffer
 *
 * Retreives all data in buffer, or all data up to a maximum string length requested.
 *
 * @param *buf Pointer to the unsigned 8-bit value buffer
 * @param *str Pointer to the return string buffer
 * @param maxLen Maximum length of the str buffer
 *
 * @return result
 */
eBuffer8Status_t Buffer8_gets(sCircularBuffer8_t *buf, char *str, uint16_t maxLen);
/** @brief Backapace removes one value
 *
 *  Delete the previous buffer if a backapace is written
 *
 * @param *buf pointer to the unsigned 8-bit value buffer
 *
 * @return result
 */
eBuffer8Status_t Buffer8_Backspace(sCircularBuffer8_t *buf);

#endif
