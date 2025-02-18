/** @file scheduler.h
 *  @brief PCO2 Scheduler
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 23, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note PCO2 System Scheduler
 *
 *  @bug  No known bugs
 */
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

/** Remove STATIC and extern values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
#endif
#ifndef extern
#define extern
#endif
#else
#ifndef STATIC
#define STATIC static
#endif
#endif

/************************************************************************
 *						STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <string.h>
#include <math.h>
#ifdef UNIT_TESTING
#include <stdio.h>
#endif

/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "pco2.h"
#include "AM08x5.h"
#include "logging.h"
#include "errorlist.h"

/************************************************************************
 *							MACROS
 ************************************************************************/
#define NUM_TASK_MAX (8)

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 * @enum Scheduler Status
 * Scheduler Function return status
 *
 */
typedef enum
{
  SCHEDULER_OK = 0x00u,
  SCHEDULER_FAIL = 0x01u
} eTaskStatus_t;

/**
 * @enum Alarm Flag Status
 * Scheduler Alarm Status Flag
 */
typedef enum
{
  SCHEDULER_ALARM_OFF = 0x00u,
  SCHEDULER_ALARM_FLAG = 0x01u,
  SCHEDULER_ALARM_SET = 0x02u,
  SCHEDULER_ALARM_LATE = 0x04u,
} eTaskAlarmFlag_t;

/**
 * @struct Task Structure
 * Task structure
 */
typedef struct Task
{
  void (*taskFunction)(void); /**< Pointer to executable function */
  sTimeReg_t start;           /**< Start time structure */
  sTimeReg_t period;          /**< Repeat period of task */
  int16_t repeat;             /**< Does the task repeat? -1 infinitely, 0-32767 Num Repeats*/
  char taskName[8];           /**< Name of the task for display */
} sTask_t;

/************************************************************************
 *							  Externs
 ************************************************************************/
#ifdef UNIT_TESTING
extern sTask_t taskList[NUM_TASK_MAX];
extern uint8_t taskCnt;
extern PERSISTENT volatile eTaskAlarmFlag_t scheduler_alarm_flag;
#endif
/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief Create Task
 *
 * Create a scheduler task with the appropriate function, start time and period
 *
 * @param (*func) Pointer to function to run
 * @param start Start time of task
 * @param period Period of recurring task
 */
eTaskStatus_t SCHEDULER_create_task(void (*func)(void), sTimeReg_t *start, sTimeReg_t *period, int16_t repeat, char *name);

/** @brief Execute Task
 *
 * Execute the next task in the scheduler
 *
 * @param None
 *
 * @return None
 */
void SCHEDULER_execute_task(void);

/** @brief Delete task
 *
 * Delete the selected task
 *
 * @param task Index of task to delete
 *
 * @return None
 */
void SCHEDULER_delete_task(uint8_t index);

/** @brief Delete all tasks
 *
 * Delete ALL tasks from Scheduler
 *
 * @param None
 *
 * @return None
 */
void SCHEDULER_delete_all_tasks(void);

/** @brief Print Tasks
 *
 * Print tasks to console
 *
 * @param None
 *
 * @return None
 */
void SCHEDULER_print_tasks(void);

/** @brief Set Task Alarm
 *
 * Set the alarm for the time in the first task
 *
 * @param None
 *
 * @return Alarm Flag
 */
eTaskAlarmFlag_t SCHEDULER_Set_TaskAlarm(void);

/** @brief Get Task Alarm
 *
 * Create string for time of next alarm
 *
 * @param *str Pointer to string to write to
 *
 * @return None
 */
void SCHEDULER_Get_TaskAlarm(char *str);

/** @brife Get Task Count
 *
 * Get the number of tasks in the scheduler queue.
 *
 * @param None
 *
 * @return Number of tasks in queue
 */
uint8_t SCHEDULER_Get_TaskCount(void);

#endif // _SCHEDULER_H
