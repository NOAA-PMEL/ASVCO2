
#include "msp430fr5994.h"
#include "src/LED.h"
#include "src/bsp.h"
#include "src/relay.h"
#include "src/DRV8823.h"
#include "src/motioncontrol.h"
#include "src/command.h"
#include "src/console.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog    
  
  /** Initialize the Board */
  BSP_Init_Board();
  MCONTROL_Init();

  /** Unlock the GPIO */
  PMM_unlockLPM5();

  /** Initialize the System Clocks */
  while(BSP_SYSCLK_Init()==BSP_FAIL);
  
  /** LED Initialize */
  LED_Init();
  LED_Command("GREEN OFF\r\n");
  LED_Command("RED OFF\r\n");

  /** Unlock the GPIO */
  PMM_unlockLPM5();

  /** Main Loop */
  BSP_Timer_Init();

  /** Initialize the Command Port */
  COMMAND_Init();
 
  /** Prep the command buffer */
  command.msg = false;
  

  
  __bis_SR_register(GIE);       // Enter LPM0 w/ interrupt
  
  /** Turn the RS232 to TTL Convert ON */
  /** @todo Add this to UART Init */
  GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN3);
  
  /** Set the mode to DEPLOY on Startup */
  /** @todo Remove this from main and put elsewhere */
  COMMAND_SetMode("DEPLOY");

  /** Main Loop */
  while (1)
  {    

    /** If there is a newline, parse the message */
    if(command.msg==true){
      COMMAND_ParseMsg();
      command.msg = false;
    }
   
    /** Run the LED Functions */
    /** @todo put these in a timer call */
    grnLED.Run(&grnLED);
    redLED.Run(&redLED);

    /** Put the H-bridge to sleep */
    MCONTROL_SleepState();
    
    /** If there are no new messages, go to low power mode */
    if(command.msg == false){
      __low_power_mode_4();
    }    
  }
}
