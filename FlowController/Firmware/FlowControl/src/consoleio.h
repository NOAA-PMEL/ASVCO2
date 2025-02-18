#ifndef _CONSOLEIO_H
#define _CONSOLEIO_H

#include <string.h>
#include <stdint.h>
#include "bsp.h"

#ifndef STR_ENDLINE
#define STR_ENDLINE       ("\r\n")
#endif

#define CONSOLEIO_PORT    (SYS_CONSOLEIO_PORT)
/*
 * @brief Console Input/Output Pass/Fail error codes
 */
typedef enum {
  CONSOLEIO_OK = 0u,      /**< Console I/O Pass */
  CONSOLEIO_FAIL = 1u     /**< Console I/O Fail */
}eConsoleIOError_t;


extern sUART_t console;

/* @brief Console Initialize
 * 
 *  Console initialize function
 * 
 *  @param Pointer to UART Structure for specified console port
 *
 *  @return None
 */
 void CONSOLEIO_Init(sUART_t *uart);

/* @brief Console Send 
 * 
 *  Console send function.  Sends multiple chars.
 *
 *  @param *str Char array pointer
 *  @param strLen Number of chars to send
 * 
 *  @return result
 */
void CONSOLEIO_Send(sUART_t *uart,char *str,uint16_t strLen);

void CONSOLEIO_SendString(sUART_t *uart,char *str);

/* @brief Console Read
 *
 *  Console read function.  Read single command.
 *
 *  @param *str Char array pointer
 * 
 *  @return result
 */
 uint16_t CONSOLEIO_Read(sUART_t *uart,char *str);

#endif // _CONSOLEIO_H
