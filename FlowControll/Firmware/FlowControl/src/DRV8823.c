/** @file DRV8823.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 19th, 2017
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
#include "DRV8823.h"

/************************************************************************
*			     Static Functions Prototypes
************************************************************************/
eDRV8823Status_t DRV8823_Set(sDRV8823_t *ic,
                             eDRV8823Chan_t chan);
STATIC void DRV8823_StrobeIC(sDRV8823_t *ic);
STATIC void DRV8823_SetReset(sDRV8823_t *ic);
STATIC void DRV8823_ClearReset(sDRV8823_t *ic);
STATIC void DRV8823_SetStrobe(sDRV8823_t *ic);
STATIC void DRV8823_ClearStrobe(sDRV8823_t *ic);
STATIC void DRV8823_SetCS(sDRV8823_t *ic);
STATIC void DRV8823_ClearCS(sDRV8823_t *ic);
STATIC uint16_t DRV8823_CreateChannelMsg(eDRV8823Chan_t chan,
                                         sDRV8823DataField_t *data);
/************************************************************************
*					    Functions
************************************************************************/
void DRV8823_Init(sDRV8823_t *IC)
{

    /** Set Specific SPI Settings */
    IC->SpiChan.param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    IC->SpiChan.param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;

    /* Initialize GPIO */
    BSP_GPIO_Init(&IC->Strobe);
    BSP_GPIO_Init(&IC->nReset);
    BSP_GPIO_Init(&IC->nSleep);
    BSP_GPIO_Init(&IC->cs);

    /* Initialize SPI Port */
    BSP_SPI_Init(&IC->SpiChan);

    /** Clear the Reset and Sleep Lines */
    DRV8823_ClearReset(IC);
    DRV8823_ClearSleepMode(IC);

    return;
}

eDRV8823Status_t DRV8823_Set_AllChannels(sDRV8823_t *ic,
                                         sDRV8823SetVal_t *outputs)
{
    eDRV8823Status_t result = DRV8823_FAIL;
    outputs->dataAB = 0u;
    outputs->dataCD = 0u;

    /** Enable the SPI */
    BSP_SPI_Enable(&ic->SpiChan);

    /** Create the send message */
    outputs->dataAB = DRV8823_CreateChannelMsg(DRV8823_CHAN_A, &outputs->outA);
    outputs->dataAB |= DRV8823_CreateChannelMsg(DRV8823_CHAN_B, &outputs->outB);
    outputs->dataCD = DRV8823_CreateChannelMsg(DRV8823_CHAN_C, &outputs->outC);
    outputs->dataCD |= DRV8823_CreateChannelMsg(DRV8823_CHAN_D, &outputs->outD);

    /** Transfer the send message to the Send buffer */
    ic->Send = *outputs;

    /** Set the data into Channel A&B */
    if (DRV8823_Set(ic, DRV8823_CHAN_AB) == DRV8823_OK)
    {
        /** If the transfer was valid, set the data into C&D */
        if (DRV8823_Set(ic, DRV8823_CHAN_CD) == DRV8823_OK)
        {
            /** If the transfer was valid, copy the send data into the last buffer */
            ic->Last = ic->Send;
            /** Result is passing */
            result = DRV8823_OK;
        }
    }

    /** Disable the SPI */
    BSP_SPI_Disable(&ic->SpiChan);
    return result;
}

eDRV8823Status_t DRV8823_Set_SingleChannel(sDRV8823_t *ic,
                                           eDRV8823Chan_t chan,
                                           sDRV8823DataField_t *settings)
{

    eDRV8823Status_t result = DRV8823_FAIL;

    /** Enable the SPI */
    BSP_SPI_Enable(&ic->SpiChan);

    /** Start setting the Send data with the Last data */
    ic->Send.dataAB = ic->Last.dataAB;
    ic->Send.dataCD = ic->Last.dataCD;

    /** Create the send message */
    uint16_t temp = DRV8823_CreateChannelMsg(chan, settings);

    /** Update the send data for the selected channel */
    switch (chan)
    {
    case DRV8823_CHAN_A:
        ic->Send.outA = *settings;
        ic->Send.dataAB = (ic->Send.dataAB & ~0x003F);
        ic->Send.dataAB |= temp;
        break;
    case DRV8823_CHAN_B:
        ic->Send.outB = *settings;
        ic->Send.dataAB = (ic->Send.dataAB & ~0x0FC0) | temp;
        break;
    case DRV8823_CHAN_C:
        ic->Send.outC = *settings;
        ic->Send.dataCD = (ic->Send.dataCD & ~0x003F) | temp;
        break;
    case DRV8823_CHAN_D:
        ic->Send.outD = *settings;
        ic->Send.dataCD = (ic->Send.dataCD & ~0x0FC0) | temp;

        break;
    default:
        break;
    }

    /** Set the Device */
    if (DRV8823_Set(ic, chan) == DRV8823_OK)
    {
        /** Strobe the Device to Set Output */
        ic->Last = ic->Send;
        result = DRV8823_OK;
    }

    /** Disable the SPI */
    BSP_SPI_Disable(&ic->SpiChan);

    return result;
}

void DRV8823_SetSleepMode(sDRV8823_t *ic)
{
    /* Set Sleep Mode Pin */
    BSP_GPIO_ClearPin(&ic->nSleep);

    return;
}

void DRV8823_ClearSleepMode(sDRV8823_t *ic)
{
    /* Clear Sleep Mode */
    BSP_GPIO_SetPin(&ic->nSleep);
    return;
}

void DRV8823_Reset(sDRV8823_t *ic)
{
    /** Reset the device */
    /** Set CS Low, Strobe Low, nSleep High */
    DRV8823_ClearCS(ic);
    DRV8823_ClearStrobe(ic);
    DRV8823_ClearSleepMode(ic);

    /** Disable SPI */
    BSP_SPI_Disable(&ic->SpiChan);

    /** Set the Reset line */
    DRV8823_SetReset(ic);
/** Wait for reset duration */
#ifndef UNIT_TESTING
    _delay_ms(DRV8823_RESET_DELAY_MS);
#endif

    /** Clear the Reset line */
    DRV8823_ClearReset(ic);

    /** Enable the SPI */
    BSP_SPI_Enable(&ic->SpiChan);
}

eDRV8823Status_t DRV8823_CreateDatafield(sDRV8823DataField_t *output,
                                         uint8_t decay,
                                         uint8_t current,
                                         uint8_t direction,
                                         uint8_t enable)
{

    eDRV8823Status_t result = DRV8823_FAIL;

    if ((decay > 1) || (current > 7) || (direction > 1) || (enable > 1))
    {
        result = DRV8823_FAIL;
    }
    else
    {
        output->Decay = decay;
        output->Current = current;
        output->Phase = direction;
        output->Enable = enable;
        result = DRV8823_OK;
    }

    return result;
}

/************************************************************************
*					    Static Functions
************************************************************************/
/** @brief DRV8823 Set IC Outputs
 *
 *  Preps DRV8823 IC for new configuration and sends the data in the Send structure
 *
 *  @param *ic Pointer to the DRV8823 Structure
 *  @param strobe Strobe the IC? 0=N0, 1=Yes
 *
 *  @return result;
 */
STATIC eDRV8823Status_t DRV8823_Set(sDRV8823_t *ic,
                                    eDRV8823Chan_t chan)
{
    eDRV8823Status_t result = DRV8823_FAIL;
    uint8_t lb = 0u;
    uint8_t hb = 0u;

    /** Split the Low and High Bytes of AB Send value, save to circular SPI buffer */
    if ((chan == DRV8823_CHAN_A) || (chan == DRV8823_CHAN_B) || (chan == DRV8823_CHAN_AB))
    {
        lb = ic->Send.dataAB & 0x00FF;
        hb = (ic->Send.dataAB >> 8) & 0x00FF;
        Buffer8_put(&ic->SpiChan.Tx, lb);
        Buffer8_put(&ic->SpiChan.Tx, hb);
    }
    else if ((chan == DRV8823_CHAN_C) || (chan == DRV8823_CHAN_D) || (chan == DRV8823_CHAN_CD))
    {
        lb = ic->Send.dataCD & 0x00FF;
        hb = (ic->Send.dataCD >> 8) & 0x00FF;
        Buffer8_put(&ic->SpiChan.Tx, lb);
        Buffer8_put(&ic->SpiChan.Tx, hb);
    }

    /** Clear the Sleep & Reset Lines */
    DRV8823_ClearReset(ic);
    DRV8823_ClearSleepMode(ic);

    /** Set Chip Select */
    DRV8823_SetCS(ic);

    /** Send the bytes */
    if (BSP_SPI_SendBytes(&ic->SpiChan) == BSP_OK)
    {
        /** If the transfer is valid, strobe the results in */
        DRV8823_StrobeIC(ic);
        result = DRV8823_OK;
    }

    /** Turn off the Chip Select */
    DRV8823_ClearCS(ic);

    /** Return */
    return result;
}

/** @brief DRV8823 Channel Field for SPI Message 
 *
 *  Create the correct field value for the DRV8823 channel field.  Based on
 *  the Texas Instruments Datasheet for the DRV8823 - SLVS913E January 2009, 
 *  revised January 2016.
 *  
 *  Data format for Bridge A & B:
 *  | D15 | D14 | D13 | D12 | D11 | D10 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
 *  ---------------------------------------------------------------------------------------
 *  |  0  |  0  |  0  |  0  | BDEC| B12 | B11| B10| BPH| BEN|ADEC| A12| A11| A10| APH| AEN|
 * 
 * 
 *  Data format for Bridge C & D:
 *  | D15 | D14 | D13 | D12 | D11 | D10 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
 *  ---------------------------------------------------------------------------------------
 *  |  0  |  0  |  0  |  1  | DDEC| D12 | D11| D10| DPH| DEN|CDEC| C12| C11| C10| CPH| CEN|
 *
 *
 *  where:
 *  xDEC is the Decay Mode
 *  x11:x13 is the Current Mode
 *  xPH is the Phase(Direction) to Engergize
 *  xEN is the Enable bit
 *  @param chan Channel to create
 *  @param data Data structure to set DRV8823 to
 *
 *  @return output MSG Field for selected channel 
 */
STATIC uint16_t DRV8823_CreateChannelMsg(eDRV8823Chan_t chan,
                                         sDRV8823DataField_t *data)
{
    uint16_t output = 0;
    uint16_t temp = 0;

    output |= data->Enable;

    temp = data->Phase << 1;
    output |= temp;

    temp = data->Current << 2;
    output |= temp;

    temp = data->Decay << 5;
    output |= temp;

    if (chan == DRV8823_CHAN_B || chan == DRV8823_CHAN_D)
    {
        output = output << 6;
    }

    if (chan == DRV8823_CHAN_C || chan == DRV8823_CHAN_D)
    {
        output |= 1 << 12;
    }

    return output;
}

/** @brief DRV8823 Set Reset Line
 *
 *  Put the DRV8823 in RESET
 *
 *  @param *ic Pointer to DRV8823 Data structure
 *
 *  @return response
 */
STATIC void DRV8823_SetReset(sDRV8823_t *ic)
{
    BSP_GPIO_ClearPin(&ic->nReset);
    return;
}

/** @brief DRV8823 Clear Reset Line
 *
 *  Take the DRV8823 OUT OF RESET
 *
 *  @param *ic Pointer to DRV8823 Data structure
 *
 *  @return response
 */
STATIC void DRV8823_ClearReset(sDRV8823_t *ic)
{
    BSP_GPIO_SetPin(&ic->nReset);
    return;
}

/** @brief DRV8823 Set Chip Select
 *
 *  Set the Chip Select on the DRV8823
 *
 *  @param *ic Pointer to DRV8823 Data Structure
 *
 *  @return result
 */
STATIC void DRV8823_SetCS(sDRV8823_t *ic)
{
    /* Set the CS Pin */
    BSP_GPIO_SetPin(&ic->cs);
    return;
}

/** @brief DRV8823 Clear Chip Select
 *
 *  Clear the Chip Select on the DRV8823
 *
 *  @param *ic Pointer to DRV8823 Data Structure
 *
 *  @return result
 */
STATIC void DRV8823_ClearCS(sDRV8823_t *ic)
{
    /* Set the CS Pin */
    BSP_GPIO_ClearPin(&ic->cs);
    return;
}

/** @brief DRV8823 Strobe IC
 *
 *  Strobe data into IC to set outputs 
 *
 *  @param *ic Pointer to DRV8823 data structure
 *
 *  @return result
 */
STATIC void DRV8823_StrobeIC(sDRV8823_t *ic)
{
    /* Strobe H-Bridge to set values */
    DRV8823_SetStrobe(ic);

/** Delay */
#ifndef UNIT_TESTING
    _delay_ms(DRV8823_STROBE_DELAY_MS);
#endif

    /* Clear the Strobe */
    DRV8823_ClearStrobe(ic);
    return;
}

/** @brief DRV8823 Set Strobe
 *
 *  Set the Strobe on the DRV8823
 *
 *  @param *ic Pointer to DRV8823 Data Structure
 *
 *  @return result
 */
STATIC void DRV8823_SetStrobe(sDRV8823_t *ic)
{
    /* Set the CS Pin */
    BSP_GPIO_SetPin(&ic->Strobe);
    return;
}

/** @brief DRV8823 Clear Strobe
 *
 *  Clear the Strobe on the DRV8823
 *
 *  @param *ic Pointer to DRV8823 Data Structure
 *
 *  @return result
 */
STATIC void DRV8823_ClearStrobe(sDRV8823_t *ic)
{
    /* Set the CS Pin */
    BSP_GPIO_ClearPin(&ic->Strobe);
    return;
}
