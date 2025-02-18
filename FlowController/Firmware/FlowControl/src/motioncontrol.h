/** @file motioncontrol.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 26, 2017
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
#ifndef _MOTIONCONTROL_H
#define _MOTIONCONTROL_H

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
#include <time.h>
#include <stdint.h>

/************************************************************************
*							HEADER FILES
************************************************************************/
#include "relay.h"
#include "DRV8823.h"
#include "bsp.h"

/************************************************************************
*							MACROS
************************************************************************/
#define MCONTROL_NSLEEP_PORT (SYS_MCONTROL_NSLEEP_PORT)
#define MCONTROL_NSLEEP_PIN (SYS_MCONTROL_NSLEEP_PIN)
#define MCONTROL_NRESET_PORT (SYS_MCONTROL_NRESET_PORT)
#define MCONTROL_NRESET_PIN (SYS_MCONTROL_NRESET_PIN)
#define MCONTROL_STROBE_PORT (SYS_MCONTROL_STROBE_PORT)
#define MCONTROL_STROBE_PIN (SYS_MCONTROL_STROBE_PIN)

#define MCONTROL_BRIDGE_A_CS_PORT (SYS_MCONTROL_BRIDGE_A_CS_PORT)
#define MCONTROL_BRIDGE_B_CS_PIN (SYS_MCONTROL_BRIDGE_B_CS_PIN)

#define VALVE_ENABLE (1)
#define VALVE_DISABLE (0)

/************************************************************************
*							Structs & Enums
************************************************************************/
/**
 *  @enum Motion Control Result Status
 *  @brief Results for Motion Control Command 
 */
typedef enum
{
    MCONTROL_OK = 0,   /**< Valid Motion Control Command + Execution */
    MCONTROL_FAIL = 1, /**< Invalid Motion Control Command or Execution */
} eMotionStatus_t;

/**
 *  @enum Bridge States
 *  @brief Possible States of H-Bridge Output 
 */
typedef enum
{
    HBRIDGE_OFF = 0,      /**< Output Energized */
    HBRIDGE_ON = 1,       /**< Output Energized */
    HBRIDGE_ON_TIMER = 2, /**< Output Energized Until Timer Complete */
    HBRIDGE_PWM = 3,      /**< Output Pulse Width Modulated */
    HBRIDGE_PWM_TIMER = 4 /**< Output Pulse Width Modulated Until Timer Completes then Off */
} eHBridgeStates_t;

/** 
 * @enum Bridge Output Direction
 * @brief Possible Output Directions of H-Bridge Output
 */
typedef enum
{
    HBRIDGE_REV = 0, /**< Output Reverse Biased */
    HBRIDGE_FWD = 1  /**< Output Forward Biased */
} eHBridgeDir_t;

/** 
 *  @enum Motion Control State 
 *  @brief Controller states for Valves & Pump
 *  See pCO2 Operational States in Documentation for more information 
 */
typedef enum
{
    PCO2_ZERO_PUMP_ON = 0x0003u,  /**< Zero State, Pump ON */
    PCO2_ZERO_PUMP_OFF = 0x0002u, /**< Zero State, Pump OFF */
    PCO2_ZERO_VENT = 0x0004u,     /**< Zero State, Vent */
    PCO2_ZERO_CAL = 0x0008u,      /**< Zero State, Calibration */
    PCO2_SPAN_PUMP_ON = 0x0010u,  /**< Span State, Pump ON */
    PCO2_SPAN_PUMP_OFF = 0x0012u, /**< Span State, Pump Off */
    PCO2_SPAN_VENT = 0x0014u,       /**< Span State, Vent */
    PCO2_SPAN_CAL = 0x0018u,       /**< Span State, Calibration */
    PCO2_EQUIL_PUMP_ON = 0x0023u,  /**< Equilibrate State, Pump ON */
    PCO2_EQUIL_PUMP_OFF = 0x0022u, /**< Equilibrate State, Pump OFF */
    PCO2_EQUIL_VENT = 0x0024u,      /**< Equilibrate State, Vent */
    PCO2_EQUIL_POST = 0x0028u,      /**< Equilibrate State, Post Vent */
    PCO2_AIR_PUMP_ON = 0x0043u,  /**< Air State, Pump ON */
    PCO2_AIR_PUMP_OFF = 0x0042u, /**< Air State, Pump OFF */
    PCO2_AIR_VENT = 0x0044u,    /**< Air State, Vent */
    PCO2_AIR_POST = 0x0048u,   /**< Air State, Post Vent */
    PCO2_REST = 0x0082u,       /**< Rest State */
    PCO2_DEPLOYMENT = 0x0084u, /**< Deployment State */
    PCO2_VENT = 0x0088u,        /**< Not in use */
    PCO2_PRESSURIZE = 0x0100u,  /**< Auto-pressurization state */
    PCO2_PRG1 = 0x0111u, /** Purge State 1*/
    PCO2_PRG2 = 0x0120u, /** Purge State 1*/
    PCO2_PRG3 = 0x0130u, /** Purge State 1*/
    PCO2_PRG4 = 0x0140u, /** Purge State 1*/
    PCO2_PRG5 = 0x0150u, /** Purge State 1*/
    PCO2_PRG6 = 0x0160u, /** Purge State 1*/
    PCO2_PRG7 = 0x0170u, /** Purge State 1*/
    PCO2_PRG8 = 0x0180u, /** Purge State 1*/
} ePCO2State_t;

/**
 * @enum Driver Selection
 */
typedef enum
{
    DRV_NA = 0x00u, /**< Unassigned */
    DRV_A = 0x01u,  /**< Bridge A */
    DRV_B = 0x02u   /**< Bridge B */
} eDRVBridge_t;

/** 
 * @enum Driver Channel 
 */
typedef enum
{
    DRV_CHAN_NONE = 0x00u, /** Currently Unassigned */
    DRV_CHAN_A = 0x01u,    /**< Channel A */
    DRV_CHAN_B = 0x02u,    /**< Channel B */
    DRV_CHAN_C = 0x04u,    /**< Channel C */
    DRV_CHAN_D = 0x08u,    /**< Channel D */
} eDRVChan_t;

/** 
 * @struct Valve State Structure
 */
typedef struct
{
    uint8_t dir;     /**< Direction state */
    uint8_t current; /**< Current settings 0 - 7 */
    uint8_t decay;
} sValveState_t;

/** 
 * @struct Valve Structure 
 * @brief Structure containing valve states, directions, channel 
 */
typedef struct
{
    /* Add Function */

    eDRVBridge_t Bridge; /**< Which HBridge is the valve on */
    eDRV8823Chan_t Chan; /**< Which HBridge Channel is the valve on */
    sValveState_t State; /**< Current settings 0 - 7 */
} sValve_t;

/** 
 *  @struct Pump States
 *  @brief 
 */
typedef enum
{
    PUMP_OFF = 0, /**< Pump OFF */
    PUMP_ON = 1   /**< Pump ON */
} ePumpStates_t;

/** 
 *  @struct Dwell States
 *  @brief Dwell states (Post Cal) for PCO2 System
 */
typedef enum
{
    PCO2_Normal = 0x01u,
    PCO2_Dwell = 0x02u
} ePCO2Dwell_t;

/**
 * @struct Motion Setting (valves and pump)
 */
typedef struct
{
    sValveState_t Valve[8]; /**< Valve State Structures for all Valves.  Valve[0] = V1, etc. */
    ePumpStates_t Pump;     /**< Pump On/Off State */
} sMotionSetting_t;

/**
 * @struct Current state of Motion controller
 */
typedef struct
{
    sDRV8823_t DRV_A;         /**< HBridge A */
    sDRV8823_t DRV_B;         /**< HBridge B */
    sRelay_t Relay;           /**< Pump Relay */
    sValve_t Valve[8];        /**< Valve Array */
    uint32_t Delay;           /**< Millisecond Delay between latching pulse on/off. 0=Always ON */
    sMotionSetting_t Current; /**< Current State */
    ePCO2State_t PCO2State;   /**< The current PCO2 State, if used */
} sMotionControl_t;

#ifdef UNIT_TESTING
extern sMotionControl_t Motion;
extern const char ValveName[8][3];
extern const sMotionSetting_t ZeroPumpOn_State_Default;
extern sMotionSetting_t ZeroPumpOn_State;
extern sMotionSetting_t ZeroPumpOff_State;
extern sMotionSetting_t ZeroPumpCal_State;
extern sMotionSetting_t SpanPumpOn_State;
extern sMotionSetting_t SpanPumpOff_State;
extern sMotionSetting_t SpanPumpCal_State;
extern sMotionSetting_t EquilPumpOn_State;
extern sMotionSetting_t EquilPumpOff_State;
extern sMotionSetting_t AirPumpOn_State;
extern sMotionSetting_t AirPumpOff_State;
extern sMotionSetting_t Rest_State;
extern sMotionSetting_t Deploy_State;
#endif

/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief Motion Control Initialize
 *
 *  Initialie the motion controller, including both bridges
 *
 *  @param None
 *
 *  @return None
 */
void MCONTROL_Init(void);

/** @brief Put Bridges to Sleep
 *
 *  Put H-Bridge circuits in sleep mode to conserve power
 *
 *  @param None
 *
 *  @return None
 */
void MCONTROL_SleepState(void);

/** @brief Set PCO2 Mode
 *
 *  Set the selected mode.  Sets valves and pump.
 *
 *  @param mode Mode to set 
 *
 *  @return result
 */
eMotionStatus_t MCONTROL_PCO2_SetMode(ePCO2State_t mode);

/** @brief Toggle Pump State
 * 
 * Toggles between the pump on/off states
 * 
 * @param None
 * 
 * @return None
 */
void MCONTROL_PCO2_TogglePump(void);

/** @brief Get Pump State
 * 
 * Returns the current pump state
 * 
 * @param None
 * 
 * @return 0:Off, 1:On, -1:error
 */
int8_t MCONTROL_PCO2_GetPump(void);

/** @brief Get Valve Direction
 * 
 * Returns the direction state of the selected valve
 * 
 * @param chan Valve channel to return
 * 
 * @return 0:A, 1:B
 */
int8_t MCONTROL_PCO2_GetDir(uint8_t chan);

/** @brief Get Current PCO2 Mode
 *
 *  Get the PCO2 mode the controller is currently set to
 *
 *  @param None
 *  
 *  @return Current PCO2 state
 */
ePCO2State_t MCONTROL_PCO2_GetMode(void);

/** @brief Set the Bridge Current
 *
 *  Change the drive current to the selected value.  0 - 7 are valid  Values are:
 *
 *  0 = 20% Drive Current
 *  1 = 38% Drive Current
 *  2 = 56% Drive Current
 *  3 = 71% Drive Current
 *  4 = 83% Drive Current
 *  5 = 92% Drive Current
 *  6 = 98% Drive Current
 *  7 = 100% Drive Current
 *
 *  @param current Current to set
 *
 *  @return result
 */
eMotionStatus_t MCONTROL_PCO2_SetCurrent(uint8_t current);

/** @brief Get Current Setting
 *
 *  Get the current drive current setting
 *
 *  @param None
 *  
 *  @return current (0 - 7)
 */
uint8_t MCONTROL_PCO2_GetCurrent(void);

/** @brief Set Pulse Length
 *
 *  Set the pulse duration length (between set and off) in milliseconds
 * 
 *  @param pulse Millisecond pulse duration
 * 
 *  @return result
 */
eMotionStatus_t MCONTROL_PCO2_SetPulse(uint32_t pulse);

/** @brief Get Pulse Length
 *
 *  Get the current pulse length duration in milliseconds
 *  
 *  @param None
 *  
 *  @return pulse in Milliseconds.
 */
uint32_t MCONTROL_PCO2_GetPulse(void);

/** @brief Panic strobing
 * 
 * Strobe the valves at different pulse-widths from 10ms - 100ms
 * to try and clear a stuck valve.
 * 
 * @param None
 * 
 * @return None
 */
void MCONTROL_Panic(void);

/** @brief Set individual valve channel
 * 
 * Set the valve channel direction individually
 * 
 * @param valve Valve to switch
 * @param direction Direction to switch (0=BC, 1=AC)
 * 
 * @return none
 */

void MCONTROL_SetValve(uint8_t valve, uint8_t direction);

#endif // _MOTIONCONTROL_H
