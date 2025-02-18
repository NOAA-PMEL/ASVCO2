#include <msp430fr5994.h>
#include "bsp.h"
#include "console.h"
#include "command.h"
#include "logging.h"

#ifdef TEST_FATFS
#include "fatfs_test.h"
#endif

void update_schedule(void);
void main_enter_lpm(void);
void main_mode_normal(void);
void main_mode_watchdog(void);
void main_mode_scheduled_run(void);
void main_mode_console(void);
void main_mode_command(void);
void common_init(void);
void display_wakeup(void);
void counter_update(void);
void sensor_init(void);
void Reset_ISR(uint16_t value);

uint16_t rst_value = 0;

int main(void)
{
  rst_value = SYSRSTIV;

  /** Check reset condition */
  counter_update();

  /* Stop watchdog timer to prevent time out reset*/
  WDTCTL = WDTPW + WDTHOLD;

  PMM_unlockLPM5();

  __delay_cycles(10000);

  while (1)
  {
    /** Initialize the board */
    BSP_Init_Board();

    if ( rst_value > 0)
    {
      /** Log the Reset */
      Reset_ISR(rst_value);
    }
    
    /** Which Mode Are We in?  */
    switch (SysMode)
    {
    case SYS_Console:
      main_mode_console();
      break;
    case SYS_Command:
      main_mode_command();
      break;
    case SYS_Sample:
      main_mode_scheduled_run();
      break;
    case SYS_Watchdog:
      main_mode_watchdog();
      break;
    default:
      break;
    }

    /** Enter Low Power Mode */
    main_enter_lpm();
  }
}

void update_schedule(void)
{
  /** Set the alarm for the next task */
  if (SCHEDULER_Set_TaskAlarm() == SCHEDULER_ALARM_LATE)
  {
    SysMode = SYS_Sample;
    __bis_SR_register(GIE);
  }
  else
  {
    if (SysMode != SYS_Watchdog)
    {
      /** Print task schedule */
      SCHEDULER_print_tasks();
    }
    SysMode = SYS_Watchdog;
  }
}

void main_enter_lpm(void)
{
  /** Change mode to Watchdog */
  if (SysMode != SYS_Watchdog)
  {
    Sleep();
  }

  SysMode = SYS_Watchdog;

  /** Submode to Normal Mode*/
  sysinfo.Submode = SYS_Submode_Normal;

  /** Put Sensors in LPM */
  PCO2_LowPowerMode();

  /** Low Power on */
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);

  /** Put the board in Low Power Mode */
  BSP_Low_Power_Mode();

  /** Restart the timer */
  if (SysMode == SYS_Watchdog)
  {
    TA0R = 0;
  }
  else
  {
    TA0R = TA0CCR0 - 100; /** Hits the WD soon */
    SysMode = SYS_Watchdog;
  }

  /** Start the WD Timer Wakeup timer */
  TA0CCTL0 = CCIE;
  __bis_SR_register(GIE);

  /** Go into low power mode */
  __low_power_mode_3();
  __no_operation();
  rst_value = SYSRSTIV;
}

void display_wakeup(void)
{
  char msg[64];
  char str[21];
  memset(msg, 0, 64);

  SCHEDULER_Get_TaskAlarm(str);
  sprintf(msg, "Next wakeup @ %s", str);

  Log(msg, LOG_Priority_High);
}

void main_mode_console(void)
{

  /** Initialize the drivers */
  common_init();

  /** Go to sleep LPM0 and wait for wakeup*/
  uint16_t counter = 5000;
  BSP_Hit_WD();
  do
  {

    _delay_ms(1);
  } while ((pConsoleRxBuffer == &ConsoleRxBuffer[0]) &&
           (counter-- > 0));
  BSP_Hit_WD();

  if ((pConsoleRxBuffer == &ConsoleRxBuffer[0]))
  {
    return;
  }

  /** Parse the command for the mode */
  CONSOLE_ProcessHotkey();

  /** Enter Terminal Mode or Return */
  switch (ConsoleMode)
  {
  case SYS_Console_Terminal:
    CONSOLE_ClearBuffer();
    CONSOLE_TerminalMode();
    PCO2_Set_mconfig("");
    break;
  case SYS_Console_Hotkey:
  case SYS_Console_None:
  default:
    break;
  }

  /** Reset Console Mode */
  ConsoleMode = SYS_Console_None;

  /** Reset the buffer */
  CONSOLE_ClearBuffer();
}

void main_mode_command(void)
{
  /** Initialize the drivers */
  common_init();

  /** Start 5 second Timeout and UART Timeout */
  BSP_Hit_WD();
  COMMAND_TerminalMode();
  PCO2_Set_mconfig(false);

  /** Write state */
  Log("Sleep Mode", LOG_Priority_Low);

  /** Set the Watchdog Wakeup Timer */
  TA0R = TA0CCR0 - 100;

  return;
}

void main_mode_watchdog(void)
{
  /** Initialize the watchdog */
  BSP_Init_WD();

  /** Hit the watchdog */
  BSP_Hit_WD();

  return;
}

void main_mode_scheduled_run(void)
{
  /** Initialize the drivers */
  common_init();

  /** Sensor Init */
  sensor_init();

  /** Start PCO2 Run */
  CONSOLE_puts("Starting Run\r\n");
  SCHEDULER_execute_task();
}

void sensor_init(void)
{
  /** Initialize the rest of the pCO2 Sensor */
  PCO2_Initialize();

  /** Unlock the GPIO */
  PMM_unlockLPM5();
}

void common_init(void)
{

  /** Clear the buffers */
  CONSOLE_ClearBuffer();
  COMMAND_ClearBuffer();

  /** Initialize the Command Port */
  SetLogQuiet(false);
  COMMAND_Init();

  /** Initialize the Console Port */
  CONSOLE_Init();

  
  /** Initialize the LEDs */
  LED_Init();

  /** Initialize the RTC */
  AM08X5_Initialize();

  

  /** Start the interrupts */
  __bis_SR_register(GIE);
  
  /** Set the 1 Second Timer */
  AM08X5_SetIRQ();
  AM08X5_TimerSet(&RTC.timer, NULL);

  /** Suppress Command Timeout Flag */
  CommandTimeoutSuppress = true;

  return;
}

void counter_update(void)
{
  /** Check For Brownout Reset */
  if (PMM_getInterruptStatus(PMM_BOR_INTERRUPT))
  {
    sysinfo.BORCounter++;
  }

  /** Check for Hardware Reset */
  if (PMM_getInterruptStatus(PMM_POR_INTERRUPT))
  {
    sysinfo.PORCounter++;
  }

  /** Check for Hardware Reset */
  if (PMM_getInterruptStatus(PMM_RST_INTERRUPT))
  {
    sysinfo.RSTCounter++;
  }

  /** Clear the system reset interrupt vector */
  SYSRSTIV = 0x00;

  /** Increment the startup counter */
  sysinfo.StartupCounter++;
}

/** System Resets */
void Reset_ISR(uint16_t value)
{
  /** Initialize the Command Port */
  COMMAND_Init();

  /** Initialize the Console Port */
  CONSOLE_Init();
  switch (__even_in_range(value, SYSRSTIV__MPUSEGPIFG))
  {

  case SYSRSTIV_LPM5WU: /* SYSRSTIV : Port LPM5 Wake Up */
    Error("Reset From - LPM5 Wake Up", LOG_Priority_High);
    break;
  case SYSRSTIV_SECYV: /* SYSRSTIV : Security violation */
    Error("Reset From - Security Violation", LOG_Priority_High);
    break;
  case SYSRSTIV_SVSHIFG: /* SYSRSTIV : SVSHIFG */
    Error("No reset", LOG_Priority_High);
    break;
  case SYSRSTIV_DOBOR: /* SYSRSTIV : Do BOR */
    Error("Reset From - BOR", LOG_Priority_High);
    break;
  case SYSRSTIV_WDTTO: /* SYSRSTIV : WDT Time out */
    Error("Reset From - Watchdog Timeout", LOG_Priority_High);
    break;
  case SYSRSTIV_WDTKEY: /* SYSRSTIV : WDTKEY violation */
    Error("Reset From - WD Key Violation", LOG_Priority_High);
    break;
  case SYSRSTIV_FRCTLPW: /* SYSRSTIV : FRAM Key violation */
    Error("Reset From - FRAM Key Violation", LOG_Priority_High);
    break;
  case SYSRSTIV_UBDIFG: /* SYSRSTIV : FRAM Uncorrectable bit Error */
    Error("Reset From - FRAM Uncorrectable Bit Error", LOG_Priority_High);
    break;
  case SYSRSTIV_PERF: /* SYSRSTIV : peripheral/config area fetch */
    Error("Reset From - Peripheral/config area fetch", LOG_Priority_High);
    break;
  case SYSRSTIV_CSPW: /* SYSRSTIV : CS Password violation */
    Error("Reset From - CS Password violation", LOG_Priority_High);
    break;
  case SYSRSTIV_MPUSEGPIFG: /* SYSRSTIV : MPUSEGPIFG violation */
    Error("Reset From - MPUSEGPIFG violation", LOG_Priority_High);
    break;
  case SYSRSTIV_MPUSEGIIFG: /* SYSRSTIV : MPUSEGIIFG violation */
    Error("Reset From - MPUSEGIIFG violation", LOG_Priority_High);
    break;
  case SYSRSTIV_MPUSEG1IFG: /* SYSRSTIV : MPUSEG1IFG violation */
    Error("Reset From - MPUSEG1IFG violtaion", LOG_Priority_High);
    break;
  case SYSRSTIV_MPUSEG2IFG: /* SYSRSTIV : MPUSEG2IFG violation */
    Error("Reset From - MPUSEG2IFG violation", LOG_Priority_High);
    break;
  case SYSRSTIV_MPUSEG3IFG: /* SYSRSTIV : MPUSEG3IFG violation */
    Error("Reset From - MPUSEG3IFG violation", LOG_Priority_High);
    break;
    case SYSRSTIV_BOR: /* SYSRSTIV : BOR */
  case SYSRSTIV_DOPOR: /* SYSRSTIV : Do POR */
  case SYSRSTIV_PMMPW: /* SYSRSTIV : PMM Password violation */
  case SYSRSTIV_MPUPW: /* SYSRSTIV : MPU Password violation */
  case SYSRSTIV_RSTNMI: /* SYSRSTIV : RST/NMI */
  case SYSRSTIV_NONE:   /* No interrupt pending */
  default:
    break;
  }
}