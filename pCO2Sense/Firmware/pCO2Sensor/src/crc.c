/** @file crc.c
 * @brief
 *
 * @author Matt Casari, matthew.casari@noaa.gov
 * @date May 11th, 2018
 * @version 1.0.0
 *
 * @copyright National Oceanic and Atmospheric Administration
 * @copyright Pacific Marine Environmental Lab
 * @copyright Environmental Development Division
 *
 * @note The Accum and Calc functions are based on examples provided bye
 * Scott Stalin.
 *
 * @bug  No known bugs
 */
#include "crc.h"

/************************************************************************
 *	                  Static Functions Prototypes
 ************************************************************************/
STATIC void CRC_Clear(void);
STATIC void CRC_Accum(char ch);

/************************************************************************
 *                        Variables (Global)
 ************************************************************************/

/************************************************************************
 *                         Static Variables
 ************************************************************************/
STATIC PERSISTENT sCRC_t CRC;
/************************************************************************
 *                            Functions
 ************************************************************************/
uint16_t CRC_Calc(char *buf, uint32_t numBytes)
{
    /** Clear the CRC Struct */
    CRC_Clear();

    /** Run the CRC until all bytes accounted for */
    while (numBytes > CRC.numBytes)
    {
        CRC_Accum(*buf++);
    }

    /** Compatibility with XMODEM CRC */
    CRC_Accum(0);
    CRC_Accum(0);

    return (uint16_t)(CRC.accum >> 8);
}

/************************************************************************
 *                         Static Functions
 ************************************************************************/
/** @brief Clear CRC Struct
 *
 * Clear the CRC structure.
 *
 * @param None
 *
 * @return None
 */
STATIC void CRC_Clear(void)
{
    CRC.accum = 0L;
    CRC.numBytes = 0L;
}
/** @brief
 *
 *
 *
 *  @param
 *
 *  @return
 */
STATIC void CRC_Accum(char ch)
{
    uint8_t i;
    CRC.accum |= (ch & 0xff);
    for (i = 0; i < 8; i++)
    {
        CRC.accum <<= 1;
        if (CRC.accum & 0x1000000L)
        {
            CRC.accum ^= 0x102100L;
        }
    }
    CRC.numBytes++;
}
