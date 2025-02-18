/** @file buffer16.h
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

#ifndef _BUFFER_16_H_
#define _BUFFER_16_H_

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

#define BUFFER_16_SIZE (128)
#define ACTUAL_BUFFER_16_SIZE (BUFFER_16_SIZE + 1)

/**
 * @enum Buffer 16 Status
 * Buffer 16 Status
 */
typedef enum
{
    BUFFER_16_OK = 0x00u,              /**< Buffer is OK */
    BUFFER_16_ERROR = 0x01u,           /**< Buffer Failed */
    BUFFER_16_FULL = 0x02u,            /**< Buffer is Full */
    BUFFER_16_EMPTY = 0x04u,           /**< Buffer is Empty */
    BUFFER_16_STRING_TOO_LONG = 0x08u, /**< String is too long to use*/
    BUFFER_16_NO_STRING = 0x10u,       /**< No String to return */

} eBuffer16Status_t;

/**
 * @struct Circular Buffer 16
 * Circular Buffer for uint16_t data
 */
typedef struct _CircularBuffer16_t
{
    uint16_t buffer[ACTUAL_BUFFER_16_SIZE];
    uint16_t size;
    uint16_t read;
    uint16_t write;

} sCircularBuffer16_t;

/************************************************************************
 *					GLOBAL FUNCTION PROTOTYPES
 ************************************************************************/
/** @brief Clear the buffer
 *
 *	Clear the unsigned 16-bit value buffer
 *
 *  @param *buf pointer to the unsigned 16-bit value buffer
 *
 *  @return None
 */
void Buffer16_Clear(sCircularBuffer16_t *buf);

/** @brief Add data to buffer
 *
 *	Adds a single unsigned 16-bit value to the buffer
 *
 *  @param *buf pointer to the unsigned 16-bit value buffer
 *  @param value unsigned 16-bit value value to add
 *
 *  @return result
 */
eBuffer16Status_t Buffer16_put(sCircularBuffer16_t *buf, uint16_t value);

/** @brief Get unsigned 16-bit value from buffer
 *
 *	Retreives one unsigned 16-bit value from the buffer
 *
 *  @param *buf pointer to the unsigned 16-bit value buffer
 *  @param *value pointer to the unsigned 16-bit value variable
 *
 *  @return result
 */
eBuffer16Status_t Buffer16_get(sCircularBuffer16_t *buf, uint16_t *value);

/** @brief Backapace removes one value
 *
 *  Delete the previous buffer if a backapace is written
 *
 * @param *buf pointer to the unsigned 16-bit value buffer
 *
 * @return result
 */
eBuffer16Status_t Buffer16_Backspace(sCircularBuffer16_t *buf);

#endif
