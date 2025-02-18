/** @file LED.c
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
 *	@note
 *
 *  @bug  No known bugs
 */
 #include "LED.h"

 /************************************************************************
*			     Static Functions Prototypes
************************************************************************/
STATIC void LED_Set(struct sLED *p);
STATIC void LED_Clear(struct sLED *p);
STATIC void LED_State_Off(struct sLED *p);
STATIC void LED_State_On(struct sLED *p);
STATIC void LED_State_Flash_1Hz(struct sLED *p);
STATIC void LED_State_Flash_4Hz( struct sLED *p);
STATIC void LED_State_Flash_SOS( struct sLED *p);
/************************************************************************
*			         Variables (Global)
************************************************************************/
struct sLED grnLED = {.Toggle = {.port=LED_GREEN_PORT,.pin=LED_GREEN_PIN, .mode=0,
                                 .configured=0, .inputValue=0,.pullups=1, .interrupt = 0,
                                 .interruptEdge=0, .inverted=0},
                      .State = LED_OFF_STATE,    
                      .Count = 0,
                      .Flag = 0,
                      .Out = {LED_OFF,0x0000000,0,0},
                      .Run = LED_Process};

struct sLED redLED = {.Toggle = {.port=LED_RED_PORT,.pin=LED_RED_PIN, .mode=0,
                                 .configured=0, .inputValue=0,.pullups=1, .interrupt = 0,
                                 .interruptEdge=0, .inverted=0},
                         .State = LED_OFF_STATE,
                         .Count = 0,
                         .Flag = 0,
                         .Out = {LED_OFF,0x0000000,0,0},
                         .Run = LED_Process};
/************************************************************************
*			    Variables (Static in Module)
************************************************************************/

STATIC const uint32_t SOS = 0x0055DDD5;
STATIC const uint32_t FOURHZ = 0x55555555;
STATIC const uint32_t ONEHZ = 0x0F0F0F0F;
STATIC const uint32_t STEADYON = 0xFFFFFFFF;
STATIC const uint32_t STEADYOFF = 0x00000000;

/************************************************************************
*					    Functions
************************************************************************/
void LED_Init(void){

    /** Initialize the Green & Red LEDs */
    BSP_GPIO_Init(&redLED.Toggle);
    BSP_GPIO_Init(&grnLED.Toggle);

    /** Set the LEDs to Steady ON */
    grnLED.State = LED_ON_STEADY;
    redLED.State = LED_ON_STEADY;
    #ifndef UNIT_TESTING
    grnLED.Run(&grnLED);
    redLED.Run(&redLED);
    #endif



    return;
}

void LED_Process(struct sLED *p){

    /** AND the pattern with the index to determine if the LED is on or off */
    /** Set LED and Status */

    if(((p->Out.Pattern >> p->Out.idx)&0x00000001u)==0){
        BSP_GPIO_ClearPin(&p->Toggle);
        p->Out.Status = LED_OFF;
    } else {
        BSP_GPIO_SetPin(&p->Toggle);
        p->Out.Status = LED_ON;
    }

    /** Increment the index and roll over if at the end */
    p->Out.idx += 1;
    if(p->Out.idx >= p->Out.max){
        p->Out.idx = 0;
    }

    return;
}


eLEDCommandStatus_t LED_Command(char *command){
    eLEDCommandStatus_t result = LED_FAIL;
    char *pStr[80];
    char *pToken;
    uint8_t i;
    uint8_t len;
    uint8_t spCnt = 0;  /** Space character counts */
    struct sLED *pLED;          /** LED Choice */

    len = strlen(command);
    for(i=0;i<len;i++){
        command[i] = toupper(command[i]);
    }

    pStr[0] = strtok(command,"\r\n");
    len = strlen(pStr[0]);

    for(i=0;i<len;i++){
        if(command[i]==' '){
            spCnt++;
        }
    }
    /** If Valid # of spaces, check command and set */
    if(spCnt == LED_COMMAND_SPACE_CNT){

        pToken = strtok(pStr[0]," ");
        uint8_t validLED = true;
        /** Check Color Selected */
        if(strncmp(pToken,"RED",3) == 0){
            pLED = &redLED;
            validLED = true;
        } else if(strncmp(pToken,"GREEN",5) == 0){
            pLED = &grnLED;
            validLED = true;
        } else {
            validLED = false;
        }

        /** If valid color, check state */
        if(validLED == true){
            pToken = strtok(NULL,"");
            /** Find the correct command */
            if(strncmp(LED_COMMAND_STR_ON,pToken,strlen(LED_COMMAND_STR_ON))==0){
                LED_State_On(pLED);
                result=LED_OK;
            } else if (strncmp(LED_COMMAND_STR_OFF,pToken,strlen(LED_COMMAND_STR_OFF))==0){
                LED_State_Off(pLED);
                result=LED_OK;
            } else if (strncmp(LED_COMMAND_STR_1HZ,pToken,strlen(LED_COMMAND_STR_1HZ))==0){
                LED_State_Flash_1Hz(pLED);
                result=LED_OK;
            } else if (strncmp(LED_COMMAND_STR_4HZ,pToken,strlen(LED_COMMAND_STR_4HZ))==0){
                LED_State_Flash_4Hz(pLED);
                result=LED_OK;
            } else if (strncmp(LED_COMMAND_STR_SOS,pToken,strlen(LED_COMMAND_STR_SOS))==0){
                LED_State_Flash_SOS(pLED);
                result=LED_OK;
            }

            if(result == LED_OK){
                pLED->Run(pLED);
            }
        }
    }
  return result;
}

eLEDCommandStatus_t LED_SetState(struct sLED *p, eLEDStates_t State){
    eLEDCommandStatus_t result = LED_FAIL;

    switch(State){
        case LED_OFF_STATE:
            LED_State_Off(p);
            result = LED_OK;
            break;
        case LED_ON_STEADY:
            LED_State_On(p);
            result = LED_OK;
            break;
        case LED_ON_FLASHING_1HZ:
            LED_State_Flash_1Hz(p);
            result = LED_OK;
            break;
        case LED_ON_FLASHING_4HZ:
            LED_State_Flash_4Hz(p); 
            result = LED_OK;
            break;
        case LED_ON_FLASHING_SOS:
            LED_State_Flash_SOS(p);
            result = LED_OK;
            break;
        default:
            LED_State_Flash_SOS(p);
            result = LED_FAIL;
            break;
    }

    return result;
}

/************************************************************************
*					    Static Functions
************************************************************************/
/** @brief Turn LED OFF
 *
 *  Set LED to permanent OFF State 
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_State_Off(struct sLED *p){
    if(p->Out.Status != LED_OFF){
        LED_Clear(p);
        p->Out.Status = LED_OFF;
    }
    p->State = LED_OFF_STATE;
    p->Out.Pattern = STEADYOFF;
    p->Out.idx = 0;
    p->Out.max = 0;

    #ifdef DISPLAY_STATE
    printf("\nLED STATE = OFF\n");
    #endif
    return;
}

/** @brief Turn LED ON
 *
 *  Set LED to permanent ON State 
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_State_On( struct sLED *p){
    if(p->Out.Status != LED_ON){
        LED_Set(p);
        p->Out.Status=LED_ON;
    }
    p->State = LED_ON_STEADY;
    p->Out.Pattern = STEADYON;
    p->Out.idx = 0;
    p->Out.max = 0;
    #ifdef DISPLAY_STATE
    printf("\nLED STATE = ON\n");
    #endif
}

/** @brief 1HZ LED Flash
 *
 *  Set LED to 1 Hz Flash
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_State_Flash_1Hz( struct sLED *p){
    if(p->Out.Status != LED_OFF){
        LED_Clear(p);
        p->Out.Status = LED_OFF;
    }
    p->State = LED_ON_FLASHING_1HZ;
    p->Out.Pattern = ONEHZ;
    p->Out.idx = 0;
    p->Out.max = 32;
    #ifdef DISPLAY_STATE
    printf("\nLED STATE = 1Hz\n");
    #endif
    return;
}

/** @brief 4HZ LED Flash
 *
 *  Set LED to 4Hz Flash 
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_State_Flash_4Hz( struct sLED *p){
    if(p->Out.Status != LED_OFF){
        LED_Clear(p);
        p->Out.Status = LED_OFF;
    }
    p->State = LED_ON_FLASHING_4HZ;
    p->Out.Pattern = FOURHZ;
    p->Out.idx = 0;
    p->Out.max = 32;
    #ifdef DISPLAY_STATE
    printf("\nLED STATE = 4Hz\n");
    #endif
}

/** @brief LED Flash SOS
 *
 *  Set LED to Flash SOS
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_State_Flash_SOS( struct sLED *p){
    if(p->Out.Status != LED_OFF){
        LED_Clear(p);
        p->Out.Status = LED_OFF;
    }
    p->State = LED_ON_FLASHING_SOS;
    p->Out.Pattern = SOS;
    p->Out.idx = 0;
    p->Out.max = 32;
    #ifdef DISPLAY_STATE
    printf("\nLED STATE = SOS\n");
    #endif
}


/** @brief Set LED ON
 *
 *  Turn LED ON
 * 
 *  @param *p Pointer to LED structure
 *
 *  @return None
 */
STATIC void LED_Set(struct sLED *p){
    BSP_GPIO_SetPin(&p->Toggle);
    return;
}

/** @brief Clear LED OFF
 *
 *  Turn LED OFF
 * 
 *  @param *p Pointer to LED Structure
 *
 *  @return None
 */
STATIC void LED_Clear(struct sLED *p){
    BSP_GPIO_ClearPin(&p->Toggle);
    return;
}