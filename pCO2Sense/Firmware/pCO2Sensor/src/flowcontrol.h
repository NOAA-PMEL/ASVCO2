/** @file flowcontrol.h
 *  @brief Flow control driver
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date March 23, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *  @note Command interface between micro and PMEL EDD Flow Control Board
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _FLOWCONTROL_H
#define _FLOWCONTROL_H

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
#include <string.h>
#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
#include <math.h>

#ifdef UNIT_TESTING
#include <stdio.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "MAX310x.h"
#include "logging.h"
#include "errorlist.h"
#include "AM08x5.h"
#include "console.h"
#include "command.h"
/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 * @enum Flow Result
 * @brief Flow Result
 */
typedef enum
{
    FLOW_OK = 0x00u,
    FLOW_FAIL = 0x01u
} eFLOWStatus_t;

/**
 * @enum Flow Control States
 * @brief Flow COntrol States
 */
typedef enum
{
    FLOW_STATE_ZERO_PUMP_ON = 0x01u,    /**< Zero, Pump On */
    FLOW_STATE_ZERO_PUMP_OFF = 0x02u,   /**< Zero, Pump Off */
    FLOW_STATE_ZERO_VENT = 0x03u,       /**< Zero, Vent */
    FLOW_STATE_ZERO_PUMP_POST = 0x04u,  /**< Zero, Post */
    FLOW_STATE_SPAN_PUMP_ON = 0x05u,    /**< Span, Pump On */
    FLOW_STATE_SPAN_PUMP_OFF = 0x06u,   /**< Span, Pump Off */
    FLOW_STATE_SPAN_VENT = 0x07u,       /**< Span, Vent*/
    FLOW_STATE_SPAN_PUMP_POST = 0x08u,  /**< Span, Post */
    FLOW_STATE_EQUIL_PUMP_ON = 0x09u,   /**< Equilibrate, Pump On */
    FLOW_STATE_EQUIL_PUMP_OFF = 0x0Au,  /**< Equilibrate, Pump Off */
    FLOW_STATE_EQUIL_VENT = 0x0Bu,      /**< Equilibrate, Vent */
    FLOW_STATE_EQUIL_PUMP_POST = 0x0Cu, /**< Equilibrate, Post */
    FLOW_STATE_AIR_PUMP_ON = 0x0Du,     /**< Air, Pump On */
    FLOW_STATE_AIR_PUMP_OFF = 0x0Eu,    /**< Air, Pump Off */
    FLOW_STATE_AIR_VENT = 0x0Fu,        /**< Air, Vent */
    FLOW_STATE_AIR_POST = 0x10u,        /**< Air, Post Vent */
    FLOW_STATE_REST = 0x14u,            /**< Rest */
    FLOW_STATE_DEPLOY = 0x15u,          /**< Deploy */
    FLOW_STATE_VENT = 0x17u,            /**< Vent/Dry Mode */
    FLOW_STATE_PRESSURIZE = 0x18u,      /**< Auto-Pressurize Mode */
    FLOW_STATE_PRG1 = 0x19u, /** Purge 1*/
    FLOW_STATE_PRG2 = 0x20u, /** Purge 2*/
    FLOW_STATE_PRG3 = 0x21u, /** Purge 3*/
    FLOW_STATE_PRG4 = 0x22u, /** Purge 4*/
    FLOW_STATE_PRG5 = 0x23u, /** Purge 5*/
    FLOW_STATE_PRG6 = 0x24u, /** Purge 6*/
    FLOW_STATE_PRG7 = 0x25u, /** Purge 7*/
    FLOW_STATE_PRG8 = 0x26u, /** Purge 8*/
    FLOW_STATE_TEST_ZERO_0 = 0x0100u,
    FLOW_STATE_TEST_ZERO_1 = 0x0200u,
    FLOW_STATE_TEST_ZERO_2 = 0x0400u,
    FLOW_STATE_TEST_SPAN_0 = 0x0800u,
    FLOW_STATE_TEST_SPAN_1 = 0x1000u,
    FLOW_STATE_TEST_SPAN_2 = 0x2000u,
    FLOW_STATE_TEST_EQUIL_0 = 0x4000u,
    FLOW_STATE_TEST_EQUIL_1 = 0x8000u,
    FLOW_STATE_TEST_AIR_0 = 0x10000u,
    FLOW_STATE_TEST_AIR_1 = 0x20000u,
    FLOW_STATE_TEST_PUMP = 0x80000u
} eFLOWState_t;

/**
 * @struct Power State Struct
 * @brief Power State Struct
 */
typedef struct
{
    sGPIO_t pin;     /**< Power Pin Struct */
    bool state;      /**< false = OFF, true = ON */
    sTimeReg_t time; /**< Time Power was turned ON */
} sFlowPowerState_t;

/**
 * @struct Flow Control Struct
 * @brief Flow Control Struct
 */
typedef struct
{
    eFLOWState_t currentState; /**< Current State */
    eFLOWState_t nextState;    /**< Next State */
    sFlowPowerState_t Power;   /**< Power State */
    sMAX310X_t UART;           /**< UART Port Struct */
} sFLOW_t;

#ifdef UNIT_TESTING
extern sFLOW_t Flow;
#endif
/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Initialize Flow Control Unit
 *
 * Initialize the PMEL Serial Flow Control (Pump & Valve) Board
 *
 *  @param None
 *
 *  @return result
 */
eFLOWStatus_t FLOW_Init(void);

/** @brief Set Flow Conrol Mode
 *
 *  Sets the PMEEL Serial Flow Control (Pump & Valve) Board Modes.  Options include:
 *  Zero (Pump On, Pump Off, Post Cal)
 *  Span (Pump ON, Pump Off, Post Cal)
 *  Equilibrate (Pump On, Pump Off, Post Cal)
 *  Air (Pump On, Pump Off)
 *  Rest
 *  Deploy
 *
 *  @param state Operational state
 *
 *  @return result
 */
eFLOWStatus_t FLOW_SetMode(eFLOWState_t state);

/** @brief Get Mode
 *
 *  Get the current FLow Controller mode
 *
 *  @param
 *
 *  @return
 */
eFLOWStatus_t FLOW_GetMode(void);

/** @brief Get the Flow Control State
 *
 * Returns the flow control state
 *
 * @param *str Flow control state
 *
 * @return None
 */
void FLOW_GetState(char *str);

/** @brief Sleep Mode
 *
 *  Set Flow Controller to sleep.  Powers Flow Controller OFF, MAX310X to Sleep
 *
 *  @param None
 *
 *  @return None
 */
void FLOW_Sleep(void);

/** @brief Wakeup Mode
 *
 *  Wakes the Flow Controller up.  Powers the Flow Controller ON, Initializes the MAX310X,
 *  sets the statup conditions.
 *
 *  @param None
 *
 *  @return None
 */
void FLOW_Wakeup(void);

/** @brief Toggle Pump Power
 *
 * Toggles the pump power state
 *
 * @param None
 *
 * @return None
 */
void FLOW_PumpToggle(void);

/** @brief Turn Pump ON
 *
 * Turn the Pump ON
 *
 * @param None
 *
 * @return None
 */
void FLOW_PumpON(void);

/** Turn Pump OFF
 *
 * Turn the Pump OFF
 *
 * @param None
 *
 * @return None
 */
void FLOW_PumpOFF(void);

/** Set the Pulse Length
 *
 * Set the pulse length on the valve (ms)
 *
 * @param pulse Pulse length (ms)
 *
 * @return reusult
 */
eFLOWStatus_t FLOW_SetPulse(uint16_t pulse);

/** Get the Pulse Length
 *
 * Get the pulse length on the valve (ms)
 *
 * @param None
 *
 * @return Pulse length (0-1000ms)
 */
int16_t FLOW_GetPulse(void);

/** Set the Current chopping
 *
 * Set the current chopping value (0-7)
 *
 * 0 - 20% chopping
 * 1 - 38% chopping
 * 2 - 56% chopping
 * 3 - 71% chopping
 * 4 - 83% chopping
 * 5 - 92% chopping
 * 6 - 98% chopping
 * 7 - 100% chopping
 *
 * @param current Chopping current (0-7)
 *
 * @return reusult
 */
eFLOWStatus_t FLOW_SetCurrent(uint16_t current);

/** Get the Current chopping
 *
 * Get the Current chopping
 *
 * @param None
 *
 * @return Current chopping (0-7)
 */
int16_t FLOW_GetCurrent(void);

/** Direct write to FC
 *
 * Directly write to the Flow Controller
 *
 * @param *val Pointer to the string to send
 * @param len String length
 *
 */
void FLOW_Write(char *val, uint8_t len);

/** Read from FC
 *
 * Directly read from the Flow Controller
 *
 * @param *val Pointer to the string
 * @param *len Pointer to the read string length
 *
 */
void FLOW_Read(char *val, uint8_t *len);

#endif // _FLOWCONTROL_H
