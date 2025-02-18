/** @file LED.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 27, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _LED_H
#define _LED_H

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
#include <ctype.h>
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"

/************************************************************************
 *							MACROS
 ************************************************************************/
#ifdef UNIT_TESTING
#define LED_GREEN_PORT (GPIO_PORT_P8)
#define LED_GREEN_PIN (GPIO_PIN2)
#define LED_RED_PORT (GPIO_PORT_P8)
#define LED_RED_PIN (GPIO_PIN3)
#else
#define LED_GREEN_PORT (SYS_LED_GREEN_PORT)
#define LED_GREEN_PIN (SYS_LED_GREEN_PIN)
#define LED_RED_PORT (SYS_LED_RED_PORT)
#define LED_RED_PIN (SYS_LED_RED_PIN)
#endif

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#define LED_COMMAND_SPACE_CNT (1)
#define LED_COMMAND_STR_OFF ("OFF")
#define LED_COMMAND_STR_ON ("ON")
#define LED_COMMAND_STR_1HZ ("FLASH1")
#define LED_COMMAND_STR_4HZ ("FLASH4")
#define LED_COMMAND_STR_SOS ("SOS")
/************************************************************************
 *							Structs & Enums
 ************************************************************************/

/**
 *  @enum LED Command Status
 *  @brief Return codes for LED Commands
 */
typedef enum
{
    LED_OK = 0,        /**< Valid LED Command */
    LED_FAIL = 1,      /**< General Command Error */
    LED_BAD_STATE = 2, /**< Invalid State Commanded */
    LED_BAD_LED = 3    /**< Invalid LED Commanded */
} eLEDCommandStatus_t;
/**
 *  @enum LED States
 *  @brief LED States
 */
typedef enum
{
    LED_OFF_STATE = 0,       /**< LED is in OFF State*/
    LED_ON_STEADY = 1,       /**< LED is in ON Steady State*/
    LED_ON_FLASHING_1HZ = 2, /**< LED is Flashing at 1Hz */
    LED_ON_FLASHING_4HZ = 3, /**< LED is Flashing at 4Hz */
    LED_ON_FLASHING_SOS = 4  /**< LED is Flashing SOS */
} eLEDStates_t;

/**
 *  @enum LED Current Status
 *  @brief LED Currently On or Off
 */
typedef enum
{
    LED_OFF = 0, /**< LED is OFF */
    LED_ON = 1   /**< LED is ON */
} eLEDCurrent_t;

/**
 *  @struct LED Pattern Structure
 *  @brief Structure of LED Pattern to display
 */
typedef struct
{
    eLEDCurrent_t Status;
    uint32_t Pattern;
    uint8_t idx;
    uint8_t max;
} sLEDPattern_t;

/**
 *  @struct LED Structure
 *  @brief LED Information Structure
 */
struct sLED
{
    sGPIO_t Toggle;     /**< Port/Pin Definition for LED */
    eLEDStates_t State; /**< LED State */
    uint16_t Count;     /**< Millisecond Countdown until next State */
    uint8_t Flag;       /**< Flag indicating counter timer or state change */
    sLEDPattern_t Out;  /**< Pattern to Send */
    void (*Run)(struct sLED *);
};

extern struct sLED redLED;
extern struct sLED grnLED;
/************************************************************************
 *					    Function Prototypes
 ************************************************************************/
/** @brief Initialize the LED
 *
 *  Initialize the LED pin
 *
 *  @param None
 *
 *  @return None
 */
void LED_Init(void);

/** @brief Process the current LED State
 *
 *  Increment the counter and process the current state.
 *
 *  @param *p Pointer to LED structure
 *
 *  @return
 */
void LED_Process(struct sLED *p);

/** @brief Set the State Requested
 *
 *  Set the LED State requested.  States available are:
 *  OFF, ON, Flash 1Hz, Flash 4Hz, SOS
 *
 *  @param *p Pointer to LED structure
 *  @param State State to run
 *
 *  @return
 */
eLEDCommandStatus_t LED_SetState(struct sLED *p, eLEDStates_t State);

/** @brief Parse LED Command
 *
 *  Parse the LED command and set the state.
 *
 *  @param *command Pointer to the command string
 *
 *  @return result
 */
eLEDCommandStatus_t LED_Command(char *command);

/************************************************************************
 *			  Static Function Prototypes for Unit Testing
 ************************************************************************/

#ifdef UNIT_TESTING
STATIC void LED_Set(struct sLED *p);
STATIC void LED_Clear(struct sLED *p);
STATIC void LED_State_Off(struct sLED *p);
STATIC void LED_State_On(struct sLED *p);
STATIC void LED_State_Flash_1Hz(struct sLED *p);
STATIC void LED_State_Flash_4Hz(struct sLED *p);
STATIC void LED_State_Flash_SOS(struct sLED *p);
#endif
#endif // _LED_H
