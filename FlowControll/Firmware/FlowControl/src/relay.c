/** @file relay.c
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
#include "relay.h"

/************************************************************************
*			     Static Functions Prototypes
************************************************************************/

/************************************************************************
*			    Variables (Static in Module)
************************************************************************/
STATIC PERSISTENT sRelay_t Relay = {.State = RELAY_OFF,
                                    .Toggle = { .port= SYS_RELAY_TOGGLE_PORT,
                                                .pin=SYS_RELAY_TOGGLE_PIN,
                                                .inverted = SYS_RELAY_TOGGLE_POL}};


/************************************************************************
*					    Functions
************************************************************************/
void RELAY_Init(void){

    /* Initialize the GPIO Output */
    BSP_GPIO_Init(&Relay.Toggle);

    return;
}

void RELAY_On(void){

    BSP_GPIO_SetPin(&Relay.Toggle);

    return;
}

void RELAY_Off(void){

    BSP_GPIO_ClearPin(&Relay.Toggle);

    return ;
}

/************************************************************************
*					    Static Functions
************************************************************************/
