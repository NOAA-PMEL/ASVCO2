/** @file command.h
 *  @brief Command Function Header
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date December 18th, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */

#ifndef _COMMAND_H
#define _COMMAND_H

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
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "bsp.h"
#include "motioncontrol.h"

/************************************************************************
*							Structs & Enums
************************************************************************/
/** 
 *  @enum COMMAND Status
 *  @brief COMMAND Status 
 */
typedef enum
{
    COMMAND_OK = 0u,  /**< Command passes */
    COMMAND_FAIL = 1u /**< Command failure */
} eCommandStatus_t;

/**
 *  @enum COMMAND Operation Mode 
 *  @brief Operation mode of system
 */
typedef enum
{
    COMMAND_MODE_1,
    COMMAND_MODE_2
} eCommandMode_t;

/** The command structure is global */
extern sUART_t command;

/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief Init Command Port.
 *
 *  Initialize the Command Port to 9600 baud.
 *
 *  @return None
 */
void COMMAND_Init(void);

/** @brief Parse the Command Buffer
 *
 * Parse the command buffer for the new message
 *
 * @param None
 *
 * @return Result 
 */
eCommandStatus_t COMMAND_ParseMsg(void);

/** @brief Set the COMMAND Mode 
 *
 * Select the command mode from the buffer string provided.
 *
 * @param *mode Pointer to the command mode string
 *
 * @return result
 */
eCommandStatus_t COMMAND_SetMode(char *mode);

/** @brief Get the COMMAND Mode
 *
 *  Request the command mode string
 *
 *  @param None
 *
 *  @return pointer to mode string
 */
char *COMMAND_GetMode(void);

/** @brief Set the Driver current 
 *  
 * Set the H-Bridge Driver Current (0=20%, 7 = 100%)
 *
 *  @param current (0 - 7)
 * 
 *  @return result
 */
eCommandStatus_t COMMAND_SetCurrent(uint8_t current);

/** @brief Get the Driver Current 
 * 
 * Get the current Driver current value
 *
 * @param None
 *
 * @return current value (0=20%, 7=100%)
 */
uint8_t COMMAND_GetCurrent(void);

/** @brief Set the Specified Channel 
 *
 *  @todo not currently implemented
 */
eCommandStatus_t COMMAND_SetChannel(uint8_t channel);

/** @brief Get the Specific Channel
 *
 *  @todo Maybe remove this?  Seems like it wasn't thought through 
 */
uint8_t COMMAND_GetChannel(void);

/** @brief Set Pulse Length
 *
 * Set the number of milliseconds the Driver should pulse the valves
 *
 * @param ms Time to pulse (in milliseconds)
 *
 * @return result
 */
eCommandStatus_t COMMAND_SetPulse(uint16_t ms);

/** Get Pulse Length
 *
 *  Get the current pulse length
 *
 *  @param None
 *
 * @return pulse length in milliseconds
 */
uint32_t COMMAND_GetPulse(void);

/** @brief Get the Firmware Version
 *
 * Return the firmware version
 *
 * @param None
 * 
 * @return Pointer to version string
 *
 */
char *COMMAND_GetVersion(void);

/** @brief Get the Serial Number
 * 
 * Get the board serial number
 * 
 * @param None
 * 
 * @return Pointer to serial number string 
 */
char *COMMAND_GetSerial(void);

/** @brief Get the startup count
 *
 * Get the number of system restart counts
 *
 * @param None
 *
 * @return Number of system restarts
 */
uint32_t COMMAND_GetCount(void);

#endif // _COMMAND_H
