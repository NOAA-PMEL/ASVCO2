/** @file buffer_c.h
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
 *  @note
 *
 *  @bug  No known bugs
 */

#ifndef _BUFFER_C_H_
#define _BUFFER_C_H_

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

#define BUFFER_C_SIZE (4096)
#define ACTUAL_BUFFER_C_SIZE (BUFFER_C_SIZE + 1)

/** @enum Char buffer status
 * Char buffer status
 */
typedef enum
{
	BUFFER_C_OK = 0x00u,			  /**< Buffer is OK */
	BUFFER_C_ERROR = 0x01u,			  /**< Buffer Failed */
	BUFFER_C_FULL = 0x02u,			  /**< Buffer is Full */
	BUFFER_C_EMPTY = 0x04u,			  /**< Buffer is Empty */
	BUFFER_C_STRING_TOO_LONG = 0x08u, /**< String is too long to use*/
	BUFFER_C_NO_STRING = 0x10u,		  /**< No String to return */

} eBufferCStatus_t;

/** @struct Circular Buffer struct
 * Circular buffer struct
 */
typedef struct _CircularBufferC_t
{
	char buffer[ACTUAL_BUFFER_C_SIZE]; /**< Circular buffer */
	uint16_t size;					   /**< size of buffer */
	uint16_t read;					   /**< Read location */
	uint16_t write;					   /**< Write location */

} sCircularBufferC_t;

/************************************************************************
 *					GLOBAL FUNCTION PROTOTYPES
 ************************************************************************/
/** @brief Clear the buffer
 *
 *	Clear the char buffer
 *
 *  @param *buf pointer to the char buffer
 *
 *  @return None
 */
void BufferC_Clear(sCircularBufferC_t *buf);

/** @brief Add char to buffer
 *
 *	Adds a single char to the buffer
 *
 *  @param *buf pointer to the char buffer
 *  @param value char value to add
 *
 *  @return result
 */
eBufferCStatus_t BufferC_putc(sCircularBufferC_t *buf, char value);

/** @brief Get char from buffer
 *
 *	Retreives one char from the buffer
 *
 *  @param *buf pointer to the char buffer
 *  @param *value pointer to the char variable
 *
 *  @return result
 */
eBufferCStatus_t BufferC_getc(sCircularBufferC_t *buf, char *value);

/** @brief Backapace removes one char
 *
 *  Delete the previous buffer if a backapace is written
 *
 * @param *buf pointer to the char buffer
 *
 * @return result
 */
eBufferCStatus_t BufferC_Backspace(sCircularBufferC_t *buf);

/** @brief Write Line (Put String)
 *
 *  Writes a line to the Buffer (NO <CR><LF> Termination )
 *
 * @param *buf pointer to the char buffer
 * @param *val string to write
 * @param len Length of string to write
 *
 * @return result
 */
eBufferCStatus_t BufferC_puts(sCircularBufferC_t *buf, char *val, uint16_t len);

/** @brief Read Line (Get String)
 *
 *  Reads a line from the Buffer (terminated with \n).  String is NULL terminated
 *  Maximum
 *
 * @param *buf pointer to the char buffer
 * @param *str string to return
 * @param maxLen Maximum length of return string
 *
 * @return result
 */
eBufferCStatus_t BufferC_gets(sCircularBufferC_t *buf, char *str, uint16_t maxLen);

#endif
