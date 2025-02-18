/** @file relay.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 26, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */
#ifndef _RELAY_H
#define _RELAY_H

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
#define STATIC  static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent 
#endif
#endif

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "bsp.h"

/************************************************************************
*							MACROS
************************************************************************/

/************************************************************************
*							Structs & Enums
************************************************************************/
/*
 *  @enum Relay return Status
 *  @brief Relay Status States
 */
typedef enum{
    RELAY_OK = 0,
    RELAY_FAIL = 1
}eRelayStatus_t;

/*
 *  @enum Relay States
 *  @brief Relay Output State 
 */
typedef enum{
    RELAY_OFF = 0,      /**< Relay OFF Condition */
    RELAY_ON = 1        /**< Relay ON Condition */
}eRelayState_t;

/*
 *  @struct Relay Structure
 *  @brief Relay Structure
 */
typedef struct{
    eRelayState_t State;    /**< Current Relay State */
    sGPIO_t Toggle;      /**< Relay Toggle Line */
}sRelay_t;

/** Setup static function prototype for Unit Testing */
#ifdef UNIT_TESTING
STATIC void RELAY_AddPin(sGPIO_t *Toggle);
#endif


/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief Initialize Relay
 *
 *  Relay initialization function.  Initializes output pin and structures 
 *
 *  @param None
 *
 *  @return None
 */
void RELAY_Init(void);
/** @brief Relay ON
 *
 *  Set the relay to output ON.  
 *
 *  @param None
 *
 *  @return None
 */
void RELAY_On(void);

/** @brief Relay OFF
 *
 *  Set the relay to output OFF.  
 *
 *  @param None
 *
 *  @return None
 */
 void RELAY_Off(void);

#endif // _RELAY_H
