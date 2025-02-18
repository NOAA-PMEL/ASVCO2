/** @file crc.h
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
 * @note
 *
 * @bug  No known bugs
 */
#ifndef _CRC_H
#define _CRC_H

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
 *                        STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>

/************************************************************************
 *                          HEADER FILES
 ************************************************************************/

/************************************************************************
 *                             MACROS
 ************************************************************************/

/************************************************************************
 *                        Structs & Enums
 ************************************************************************/
/**
 *  @struct CRC Struct
 *  @brief CRC Accumulator Structure
 */
typedef struct
{
    uint32_t accum;    /**< CRC Accumulator value */
    uint32_t numBytes; /**< Number of bytes CRC performed on */
} sCRC_t;
/************************************************************************
 *                            EXTERNS
 ************************************************************************/
#ifdef UNIT_TESTING
extern sCRC_t CRC;
#endif

/************************************************************************
 *                     Function Prototypes
 ************************************************************************/
/** @brief Calculate CRC from Char Buffer
 *
 * Calculate the 8-bit CRC value from the char buffer provided.
 *
 *  @param *buf Pointer to the char buffer with data to calculate CRC on
 *  @param numBytes Length of buffer
 *
 *  @return CRC Value
 */
uint16_t CRC_Calc(char *buf, uint32_t numBytes);

#endif // _CRC_H
