/** @file DRV8823.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 19, 2017
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
#ifndef _DRV8823_H
#define _DRV8823_H

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <stdint.h>
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "bsp.h"

/** Remove STATIC and PERSISTENT values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
STATIC uint16_t DRV8823_ChanA_Command(sDRV8823DataField_t *X);
STATIC uint16_t DRV8823_ChanB_Command(sDRV8823DataField_t *X);
STATIC uint16_t DRV8823_ChanC_Command(sDRV8823DataField_t *X);
STATIC uint16_t DRV8823_ChanD_Command(sDRV8823DataField_t *X);
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
*							MACROS
************************************************************************/
#define DRV8823_STROBE_DELAY_MS (SYS_DRV8823_STROBE_DELAY_MS)
#define DRV8823_RESET_DELAY_MS (SYS_DRV8823_RESET_DELAY_MS)
#define DRV8823_WAKEUP_DELAY_MS (SYS_DRV8823_WAKEUP_DELAY_MS)

/************************************************************************
*							Structs & Enums
************************************************************************/
/*
 *  @enum Return Status
 *  DRV8823 Status Result
 */
typedef enum
{
    DRV8823_OK,  /**< DRV8823 Command Pass */
    DRV8823_FAIL /**< DRV8823 Command Fail */
} eDRV8823Status_t;

/*
 *  @enum DRV8823 Channels
 *  DRV8823 Channel Names
 */
typedef enum
{
    DRV8823_CHAN_A = 0x0001u,  /**< Channel A */
    DRV8823_CHAN_B = 0x0002u,  /**< Channel B */
    DRV8823_CHAN_C = 0x0004u,  /**< Channel C */
    DRV8823_CHAN_D = 0x0008u,  /**< Channel D */
    DRV8823_CHAN_AB = 0x0003u, /**< Channel A&B */
    DRV8823_CHAN_CD = 0x000Cu, /**< Channel C&D */
    DRV8823_CHAN_ALL = 0x000Fu /**< All Channels */
} eDRV8823Chan_t;

/** 
 *  @struct DRV8823 Data Field Structure
 *  DRV8823 Data Field
 */
typedef struct
{
    uint8_t Decay : 1;   /**< Decay Mode, 0 = Slow, 1 = Fast */
    uint8_t Current : 3; /**< 3-Bit Current Regulation       */
    uint8_t Phase : 1;   /**< Phase Direction, 0 = Reverse, 1 = Forward */
    uint8_t Enable : 1;  /**< Enable bit, 0 = Disable, 1 = Enable */
} sDRV8823DataField_t;

/**
 *  @struct Set Value Structure
 *  DRV8823 Commands to Send
 */
typedef struct
{
    sDRV8823DataField_t outA; /**< Last Command sent to Bridges A */
    sDRV8823DataField_t outB; /**< Last Command sent to Bridges B */
    sDRV8823DataField_t outC; /**< Last Command sent to Bridges C and D */
    sDRV8823DataField_t outD; /**< Last Command sent to Bridges A */
    uint16_t dataAB;          /**< Actual AB Command sent */
    uint16_t dataCD;          /**< Actual CD Command Sent */
} sDRV8823SetVal_t;

/**
 * @num Bridge PWM Possibilities
 * @brief All possible PWM values for H-Bridge
 */
typedef enum
{
    PWM_20 = 20,  /**< 20% PWM */
    PWM_38 = 38,  /**< 38% PWM */
    PWM_56 = 56,  /**< 56% PWM */
    PWM_71 = 71,  /**< 71% PWM */
    PWM_83 = 83,  /**< 83% PWM */
    PWM_92 = 92,  /**< 92% PWM */
    PWM_98 = 98,  /**< 98% PWM */
    PWM_100 = 100 /**< 100% PWM */
} eDRV8823PWM_t;

/*
 *  @struct DRV8823 System Info
 *  DRV8823 System Information
 */
typedef struct
{
    sGPIO_t Strobe;        /**< Strobe Port/Pin Info */
    sGPIO_t nReset;        /**< nReset Port/Pin Info */
    sGPIO_t nSleep;        /**< nSleep Port/Pin Info */
    sGPIO_t cs;            /**< Chip Select Port/Pin Info */
    sSPI_t SpiChan;        /**< SPI Channel */
    sDRV8823SetVal_t Send; /**< Current values to send */
    sDRV8823SetVal_t Last; /**< Last command sent to channels */
} sDRV8823_t;

/************************************************************************
*					    Function Prototypes
************************************************************************/
/** @brief DRV8823 Initialization 
 *
 *  Initialize the DRV8823 H-Bridge Driver.  Wraps GPIO and SPI for Strobe, nReset, nSleep
 *  and SPI channel.
 *
 *  @param Pointer to DRV8823 Pin Structure
 *
 *  @return result
 */
void DRV8823_Init(sDRV8823_t *ic);

/** @brief DRV8823 Set Single Channel
 *
 *  Sets a single channel of the DRV8823 selected.  Maintains previous settings for all other channels.
 *
 *  @param *ic Pointer to DRV8823 Struct
 *  @param Chan Channel to set
 *  @param settings Channel values to set
 *  
 *  @return result
 */
eDRV8823Status_t DRV8823_Set_SingleChannel(sDRV8823_t *ic, eDRV8823Chan_t chan, sDRV8823DataField_t *settings);

/** @brief DRV8823 Set Output Channels
  *
  *  Sets the output channels to the setting selected in outputs variable.  
  *
  *  @param Chan Channel Pair to Strobe in
  *  @param *X Pointer to DRV8823 struct
  *  
  *  @return result
  */
eDRV8823Status_t DRV8823_Set_AllChannels(sDRV8823_t *ic, sDRV8823SetVal_t *outputs);

/** @brief DRV8823 Send Value but wait for Strobe
 *
 *  Send the value to a selected channel, but do not strobe in and output
 *
 *  @param Chan Channel(s) to send to
 *  @param *ic Pointer to data structure
 *
 *  @return result
 */
eDRV8823Status_t DRV8823_SendValue_AndWait_ForStrobe(eDRV8823Chan_t Chan, sDRV8823_t *ic);

/** @brief DRV8823 Prep IC
 * 
 *  @param *ic Pointer to DRV8823 data structure
 *
 *  @return result
 */
void DRV8823_PrepIC(sDRV8823_t *ic);

/** @brief DRV8823 Set Sleep Mode
 *
 *  Set the IC Sleep mode
 *
 *  @param *ic Pointer to DRV8823 Data structure
 *
 *  @return None
 */
void DRV8823_SetSleepMode(sDRV8823_t *ic);

/** @brief DRV8823 Clear Sleep Mode
 *
 *  Clear the IC Sleep mode
 *
 *  @param *ic Pointer to DRV8823 Data structure
 *
 *  @return result
 */
void DRV8823_ClearSleepMode(sDRV8823_t *ic);

/** @brief DRV8823 Reset
 *
 *  Reset the IC
 *
 *  @param *ic Pointer to DRV8823 Data structure
 *
 *  @return response
 */
void DRV8823_Reset(sDRV8823_t *ic);

/** @brief Create Datafield 
 *
 *  Generate a DRV8823 channel datafield from input parameters
 * 
 *  @param decay Use decay?  0=No Decay, 1=Decay
 *  @param current Current limit 0 - 7
 *  @param phase Direction of Channel Output, 0=+V, 1=-V
 *  @param enable Enable or disable the channel 0=Disable, 1=Enable
 *  @return result 
 */
eDRV8823Status_t DRV8823_CreateDatafield(sDRV8823DataField_t *output,
                                         uint8_t decay,
                                         uint8_t current,
                                         uint8_t direction,
                                         uint8_t enable);
#endif // _DRV8823_H
