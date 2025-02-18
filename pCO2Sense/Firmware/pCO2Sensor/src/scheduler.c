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
 *  @note System functions for PCO2 Sampling
 *
 *
 *  @bug  No known bugs
 */
#include "scheduler.h"

/************************************************************************
 *					        Static Variables
 ************************************************************************/
PERSISTENT sTask_t taskList[NUM_TASK_MAX];
PERSISTENT uint8_t taskCnt;
PERSISTENT volatile eTaskAlarmFlag_t scheduler_alarm_flag = SCHEDULER_ALARM_OFF;
/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
STATIC void SCHEDULER_sort(void);
STATIC uint8_t SCHEDULER_Reschedule(sTask_t *task);
STATIC void SCHEDULER_RemoveOld(void);
/************************************************************************
 *					         Functions
 ************************************************************************/
eTaskStatus_t SCHEDULER_create_task(void (*func)(void), sTimeReg_t *start, sTimeReg_t *period, int16_t repeat, char *name)
{
  eTaskStatus_t result = SCHEDULER_FAIL;
  sTimeReg_t now;
  sTimeReg_t settime;
  sTimeReg_t addtime;

  /** Are we maxed out for tasks? */
  if (NUM_TASK_MAX <= taskCnt)
  {
    return result;
  }

  /** Are we maxed out for tasks? */
  if (NUM_TASK_MAX <= taskCnt)
  {
    return result;
  }

  /** Create the task */
  sTask_t taskToAdd;
  taskToAdd.taskFunction = func;
  taskToAdd.start = *start;
  taskToAdd.period = *period;
  taskToAdd.repeat = repeat;

  /** Add the name */
  strcpy(taskToAdd.taskName, name);

  /** Check start time against current time */
  AM08X5_clear_time_struct(&now);
  AM08X5_GetTime(&now);

  if (AM08X5_compare_time(&now, &taskToAdd.start) == true)
  {
    /** It is later than the task.  Set the alarm to the next hour */
    /** Clear the addtime reg */
    AM08X5_clear_time_struct(&addtime);

    /** Add 1 hour */
    addtime.hour = 1;
    AM08X5_add_time_struct(&now, &addtime, &settime);

    /** Add that time to taskToAdd */
    taskToAdd.start = settime;
  }

  /** Convert to BCD */
  AM08X5_DEC_to_BCD(&taskToAdd.start);

  /** Add to the task list */
  memcpy(&taskList[taskCnt], &taskToAdd, sizeof(taskToAdd));

  /** Increment the number of tasks Index */
  taskCnt++;

  /** Return successful */
  result = SCHEDULER_OK;
  return result;
}

void SCHEDULER_delete_task(uint8_t index)
{
  uint8_t i;
  for (i = index; i < taskCnt; i++)
  {
    taskList[i] = taskList[i + 1];

    taskList[i + 1].repeat = 0;
    AM08X5_clear_time_struct(&taskList[i + 1].period);
    AM08X5_clear_time_struct(&taskList[i + 1].start);
    taskList[i + 1].taskFunction = NULL;
  }
  if (taskCnt > 0)
  {
    taskCnt--;
  }
}

void SCHEDULER_delete_all_tasks(void)
{
  for (uint8_t i = 0; i < NUM_TASK_MAX - 1; i++)
  {
    taskList[i].repeat = 0;
    taskList[i].taskFunction = NULL;
    AM08X5_clear_time_struct(&taskList[i].period);
    AM08X5_clear_time_struct(&taskList[i].start);
  }
  taskCnt = 0;
}

void SCHEDULER_execute_task(void)
{
  sTimeReg_t t1;
  sTimeReg_t t2;
  AM08X5_clear_time_struct(&t1);

  /** If there is a task in the queue, run it */
  if (taskCnt > 0)
  {
    taskList[0].taskFunction();
  }
  else
  {
    return;
  }

  /** If the task is repeating, reschedule it */
  if (SCHEDULER_Reschedule(&taskList[0]) != true)
  {
    /** If the task is PCO2 Fast Mode, set Normal mode next */
    if (PCO2_FastMode == taskList[0].taskFunction)
    {
      PCO2_GetNormalStartTime(&t1);
      PCO2_GetNormalInterval(&t2);
      SCHEDULER_create_task(PCO2_NormalMode, &t1, &t2, -1, "NORM");
    }
  }

  /** Delete the task from the queue */
  SCHEDULER_delete_task(0);

  return;
}

void SCHEDULER_print_tasks(void)
{
  uint8_t i;
  char msg[64];

  sTimeReg_t t;

  for (i = 0; i < taskCnt; i++)
  {
    t = taskList[i].start;

    sprintf(msg, "Task %u: %s, %04u-%02u-%02uT%02u:%02u:%02uZ", i,
            taskList[i].taskName,
            t.year + 2000,
            t.month,
            t.date,
            t.hour,
            t.minute,
            t.second);

    Log(msg, LOG_Priority_High);
  }

  return;
}

eTaskAlarmFlag_t SCHEDULER_Set_TaskAlarm(void)
{
  eTaskAlarmFlag_t result = SCHEDULER_ALARM_OFF;
  sTimeReg_t tnow;
  sTimeReg_t sec;
  sTimeReg_t alarm;

  /** Check for Missed Alarms, Remove the old and reschedule if repeating */
  SCHEDULER_RemoveOld();

  /** Sort the existing tasks by date and time */
  /** Do this outside the unit test, since it's a very complicated test to write */
#ifndef UNIT_TESTING
  SCHEDULER_sort();
#endif

  /** If there is a task in the scheduler, set the alarm */
  if (taskCnt > 0)
  {
    /** Get the current time */
    AM08X5_clear_time_struct(&tnow);
    AM08X5_GetTime(&tnow);

    /** Add ten seconds to allow for completing LPM and shutdown */
    AM08X5_clear_time_struct(&sec);
    sec.second = 1;
    AM08X5_add_time_struct(&tnow, &sec, &tnow);

    /** If the alarm is in the future, set it */
    alarm = taskList[0].start;
    if (true == AM08X5_compare_time(&alarm, &tnow))
    {
      AM08X5_AlarmSet(&RTC.nIRQ1, &alarm);

      /** Convert TaskList struct to Decimal from BDC */
      result = SCHEDULER_ALARM_SET;
    }
    else
    {
      /** If the alarm has already passed, return that info */
      result = SCHEDULER_ALARM_LATE;
    }
  }

  return result;
}

void SCHEDULER_Get_TaskAlarm(char *str)
{
  AM08X5_AlarmGet_Str(&RTC.nIRQ1, str);
}

uint8_t SCHEDULER_Get_TaskCount(void)
{
  return taskCnt;
}

/************************************************************************
 *					   Static Functions
 ************************************************************************/

STATIC uint8_t SCHEDULER_Reschedule(sTask_t *task)
{
  uint8_t rescheduled = false;

  if ((task->repeat > 0) || (task->repeat == -1))
  {
    sTimeReg_t t1;
    AM08X5_clear_time_struct(&t1);
    AM08X5_add_time_struct(&task->start, &task->period, &t1);
    if (0 < task->repeat)
    {
      task->repeat--;
    }

    SCHEDULER_create_task(task->taskFunction, &t1, &taskList[0].period, task->repeat, task->taskName);
    rescheduled = true;
  }

  return rescheduled;
}

STATIC void SCHEDULER_sort(void)
{
  sTask_t tempTask;
  uint8_t i, j;
  sTimeReg_t *t1;
  sTimeReg_t *t2;
  for (i = 0; i < (taskCnt - 1); i++)
  {
    for (j = 0; j < (taskCnt - 1); j++)
    {
      t1 = &taskList[j].start;
      t2 = &taskList[j + 1].start;
      if (AM08X5_compare_time(t1, t2) == true)
      {
        tempTask = taskList[j];
        taskList[j] = taskList[j + 1];
        taskList[j + 1] = tempTask;
      }
    }
  }
}

STATIC void SCHEDULER_RemoveOld(void)
{
  /** Look for Old Tasks.  If we missed them by over 1 minute, delete and reschedule if required */
  uint8_t i;
  sTimeReg_t now;
  sTimeReg_t temp;
  AM08X5_clear_time_struct(&now);
  AM08X5_clear_time_struct(&temp);

  /** Get the current time */
  AM08X5_GetTime(&now);

  /** Go through each task and see if the the current time is > alarm time */
  if (0 < taskCnt)
  {
    for (i = 0; i < taskCnt; i++)
    {
      /** Creating new tasks if needed */
      if (AM08X5_compare_time(&now, &taskList[i].start) == true)
      {
        /** The Task is greater than the current time.  If it is repeating, reset it */
        if ((taskList[i].repeat != 0))
        {
          /** Create the new task */
          SCHEDULER_create_task(taskList[i].taskFunction, &now, &taskList[i].period, taskList[i].repeat, taskList[i].taskName);
        }
      }
    }
    /** Deleting old tasks if needed */
    uint8_t index = 0;
    for (i = 0; i < taskCnt; i++)
    {
      /** Delete the task if it's old */
      if (AM08X5_compare_time(&now, &taskList[index].start) == true)
      {
        SCHEDULER_delete_task(index);
      }
      else
      {
        index++;
      }
    }
  }
}
