/** @file version.h
 *  @brief firmware version file
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date February 7, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @bug  No known bugs
 */
#ifndef _VERSION_H
#define _VERSION_H

#ifdef UNIT_TESTING
#define VERSION ("v0.0.0-999-TEST")
#else
#define VERSION ("VER_VAR")
#define DATETIME ("VER_DATE")
#endif
#endif // _VERSION_H