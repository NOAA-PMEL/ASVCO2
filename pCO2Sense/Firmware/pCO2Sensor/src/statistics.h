/** @file statistics.h
 *  @brief Statistics calculation functions
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date February 26, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Statistic Calculations
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _STATISTICS_H
#define _STATISTICS_H

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
 *							Structs & Enums
 ************************************************************************/

/**
 * @struct Float Statistics Struct
 * @brief Float Statistics Struct
 */
typedef struct
{
  uint32_t numSamples; /**< Currnet count */
  float sum;           /**< Sum of all data */
  float M2;            /**< Deviation of data */
  float M2_old;
  float mean; /**< Mean of data */
  float std;  /**< Standard Deviation of Data */
  float min;  /**< Minimum of Data */
  float max;  /**< Maximum of Data */
  float mean_old;
  float std_old;
} sStats_F_t;

/**
 * @struct Uint32  Statistics Struct
 * @brief Uint32  Statistics Struct
 */
typedef struct
{
  uint32_t numSamples;
  uint32_t sum;
  uint64_t M2;   /**< Deviation of data */
  uint32_t mean; /**< Mean of data */
  uint32_t std;  /**< Standard Deviation of Data */
  uint32_t min;  /**< Minimum of Data */
  uint32_t max;  /**< Maximum of Data */
} sStats_U32_t;

/**
 * @struct Uint16 Statistics Struct
 * @brief Uint16 Statistics Struct
 */
typedef struct
{
  uint16_t numSamples;
  uint16_t sum;
  uint32_t M2;   /**< Deviation of data */
  uint16_t mean; /**< Mean of data */
  uint16_t std;  /**< Standard Deviation of Data */
  uint16_t min;  /**< Minimum of Data */
  uint16_t max;  /**< Maximum of Data */
} sStats_U16_t;

/**
 * @struct Uint8 Statistics Struct
 * @brief Uint8 Statistics Struct
 */
typedef struct
{
  uint8_t numSamples;
  uint8_t sum;
  uint16_t M2;  /**< Deviation of data */
  uint8_t mean; /**< Mean of data */
  uint8_t std;  /**< Standard Deviation of Data */
  uint8_t min;  /**< Minimum of Data */
  uint8_t max;  /**< Maximum of Data */
} sStats_U8_t;

/**
 * @struct Int32 Statistics Struct
 * @brief Int32 Statistics Struct
 */
typedef struct
{
  int32_t numSamples;
  int32_t sum;
  int64_t M2;   /**< Deviation of data */
  int32_t mean; /**< Mean of data */
  int32_t std;  /**< Standard Deviation of Data */
  int32_t min;  /**< Minimum of Data */
  int32_t max;  /**< Maximum of Data */
} sStats_I32_t;

/**
 * @struct Int16 Statistics Struct
 * @brief Int16 Statistics Struct
 */
typedef struct
{
  int16_t numSamples;
  int16_t sum;
  int32_t M2; /**< Deviation of data */
  int32_t M2_old;
  int16_t mean; /**< Mean of data */
  int16_t mean_old;
  int16_t std; /**< Standard Deviation of Data */
  int16_t min; /**< Minimum of Data */
  int16_t max; /**< Maximum of Data */
} sStats_I16_t;

/**
 * @struct Int8 Statistics Struct
 * @brief Int8 Statistics Struct
 */
typedef struct
{
  int8_t numSamples;
  int8_t sum;
  int16_t M2;  /**< Deviation of data */
  int8_t mean; /**< Mean of data */
  int8_t std;  /**< Standard Deviation of Data */
  int8_t min;  /**< Minimum of Data */
  int8_t max;  /**< Maximum of Data */
} sStats_I8_t;

/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Clear the Data Structure
 *
 *  Clear the float structure
 *
 *  @param *s Pointer to the float struct
 *
 *  @return None
 */
void STATS_Clear(sStats_F_t *s);

/** @brief Clear the Int16 Data Struct
 *
 * Clear the Int16 Data structure
 *
 * @param *s Pointer to Int16 data struct
 *
 * @return
 */
void STATS_Clear_I16(sStats_I16_t *s);

/** @brief Add data to Data Struct
 *
 *  Adds data to struct and computes new min/max
 *
 *  @param *s Pointer to the float struct
 *  @param val Value of data to add
 *
 *  @return None
 */
void STATS_Add(sStats_F_t *s, float val);

/** @brief Compute statistical data
 *
 *  Compute statistical data in structure
 *
 *  @param *s Pointer to the float struct to compute
 *
 *  @return None
 */
void STATS_Compute(sStats_F_t *s);

/** @brief Add data to Int16 Data Struct
 *
 *  Adds data to Int16 struct and computes new min/max
 *
 *  @param *s Pointer to the int16 struct
 *  @param val Value of data to add
 *
 *  @return None
 */
void STATS_AddSample_I16(sStats_I16_t *var, int16_t newSample);

/** @brief Compute statistical data Int16
 *
 *  Compute Int16 statistical data in structure
 *
 *  @param *s Pointer to the int16 struct to compute
 *
 *  @return None
 */
void STATS_Compute_I16(sStats_I16_t *s);

#endif // _STATISTICS_H
