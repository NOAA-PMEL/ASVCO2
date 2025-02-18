#include "msp430fr5994.h"
#include "src/bsp.h"
#include "src/LED.h"
#include "src/command.h"
#include "src/buffer_c.h"

/************************************************************************
*					INTERRUPT VECTOR
************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void Port_1 (void)
{
  switch (__even_in_range(P1IV,P1IV_P1IFG7))
  {
    case P1IV_NONE:
    break;			/*Vector 0 - No Interrupt */
    case P1IV_P1IFG0:
    break;		/* Vector 2 - Interrupt on Pin 1, Pin 0*/
    case P1IV_P1IFG1:
    break;		/* Vector 4 - Interrupt on Pin 1, Pin 1 */
    case P1IV_P1IFG2:
    break;		/* Vector 6 - Interrupt on Pin 1, Pin 2 */
    case P1IV_P1IFG3:
    break;		/* Vector 8 - Interrupt on Pin 1, Pin 3 */
    case P1IV_P1IFG4:
    break;		/* Vector 10 - Interrupt on Pin 1, Pin 4 */
    case P1IV_P1IFG5:
    break;		/* Vector 12 - Interrupt on Pin 1, Pin 5 */
    case P1IV_P1IFG6:
    break;		/* Vector 14 - Interrupt on Pin 1, Pin 6 */
    case P1IV_P1IFG7:
    break;		/* Vector 16 - Interrupt on Pin 1, Pin 7 */
    default:
    break;		/* Default case */
  }
}

/**************************** PORT2_VECTOR  *******************************/
#pragma vector = PORT2_VECTOR
__interrupt void Port_2 (void)
{
  switch (__even_in_range(P2IV,P2IV_P2IFG7))
  {
    case P2IV_NONE:
    break;			/*Vector 0 - No Interrupt */
    case P2IV_P2IFG0:
    break;		/* Vector 2 - Interrupt on Port 2, Pin 0*/
    case P2IV_P2IFG1:
    break;		/* Vector 4 - Interrupt on Port 2, Pin 1 */
    case P2IV_P2IFG2:
    break;		/* Vector 6 - Interrupt on Port 2, Pin 2 */
    case P2IV_P2IFG3:
    break;		/* Vector 8 - Interrupt on Port 2, Pin 3 */
    case P2IV_P2IFG4:
    break;		/* Vector 10 - Interrupt on Port 2, Pin 4 */
    case P2IV_P2IFG5:
    break;		/* Vector 12 - Interrupt on Port 2, Pin 5 */
    case P2IV_P2IFG6:
    break;		/* Vector 14 - Interrupt on Port 2, Pin 6 */
    case P2IV_P2IFG7:
    break;		/* Vector 16 - Interrupt on Port 2, Pin 7 */
    default:
    break;		/* Default case */
  }
}

/**************************** PORT3_VECTOR  *******************************/
#pragma vector = PORT3_VECTOR
__interrupt void Port_3 (void)
{
  switch (__even_in_range(P3IV,P3IV_P3IFG7))
  {
    case P3IV_NONE:
    break;			/*Vector 0 - No Interrupt */
    case P3IV_P3IFG0:
    break;		/* Vector 2 - Interrupt on Port 3, Pin 0 */
    case P3IV_P3IFG1:
    break;		/* Vector 4 - Interrupt on Port 3, Pin 1 */
    case P3IV_P3IFG2:
    break;		/* Vector 6 - Interrupt on Port 3, Pin 2 */
    case P3IV_P3IFG3:
    break;		/* Vector 8 - Interrupt on Port 3, Pin 3 */
    case P3IV_P3IFG4:
    break;		/* Vector 10 - Interrupt on Port 3, Pin 4 */
    case P3IV_P3IFG5:
    break;		/* Vector 12 - Interrupt on Port 3, Pin 5 */
    case P3IV_P3IFG6:
    break;		/* Vector 14 - Interrupt on Port 3, Pin 6*/
    case P3IV_P3IFG7:
    break;		/* Vector 16 - Interrupt on Port 3, Pin 7 */
    default:
    break;		/* Default case */
  }
}

/**************************** PORT4_VECTOR  *******************************/
#pragma vector = PORT4_VECTOR
__interrupt void Port_4 (void)
{
  switch (__even_in_range(P4IV,P4IV_P4IFG7))
  {
    case P4IV_NONE:
    break;			/*Vector 0 - No Interrupt */
    case P4IV_P4IFG0:
    break;			/* Vector 2 - Interrupt on Port 4, Pin 0 */
    case P4IV_P4IFG1:
    break;			/* Vector 4 - Interrupt on Port 4, Pin 1 */
    case P4IV_P4IFG2:
    break;			/* Vector 6 - Interrupt on Port 4, Pin 2 */
    case P4IV_P4IFG3:
    break;			/* Vector 8 - Interrupt on Port 4, Pin 3 */
    case P4IV_P4IFG4:
    break;			/* Vector 10 - Interrupt on Port 4, Pin 4 */
    case P4IV_P4IFG5:
    break;			/* Vector 12 - Interrupt on Port 4, Pin 5 */
    case P4IV_P4IFG6:
    break;			/* Vector 14 - Interrupt on Port 4, Pin 6 */
    case P4IV_P4IFG7:
    break;			/* Vector 16 - Interrupt on Port 4, Pin 7 */
    default:
    break;		/* Default case */
  }
}

/**************************** UART_A1  *******************************/

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  char temp;
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      temp = EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
      BufferC_putc(&command.buf,temp);
      if(temp == '\n'){
        command.msg = true;
        __low_power_mode_off_on_exit();
      }
      break;
    case USCI_UART_UCTXIFG: 
      UCA1IFG &= ~(UCTXIFG);
      break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}




void USCIB0_ISR(void)
{

}


#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
  switch(__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
  {
    case USCI_NONE: break;
    case USCI_SPI_UCRXIFG:
      EUSCI_B1.Rx.buf[EUSCI_B1.Rx.ByteCnt] = UCB1RXBUF;
      if(EUSCI_B1.Rx.ByteCnt){
        EUSCI_B1.Rx.ByteCnt--;
        EUSCI_B_SPI_transmitData(EUSCI_B1.baseAddress,EUSCI_B1.Tx.buf[EUSCI_B1.Tx.ByteCnt++]);

      } else {
        EUSCI_B1.complete = true;
        EUSCI_B1.success = true;
        EUSCI_B1.busy = false;
        UCA0IE &= ~UCRXIE;
      }
      UCB1IFG &= ~UCRXIFG;
      break;
    case USCI_SPI_UCTXIFG:
      UCA0IE &= ~UCTXIE;
      break;
    default: break;
  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{ 
  if(Timer.timer.ms >0 ){
    if(--Timer.timer.ms == 0){
      Timer.timer.flag = true;
    }
  }
  
  if(Timer.spi.ms >0 ){
    if(--Timer.spi.ms == 0){
      Timer.spi.flag = true;
    }
  }
  
  if(Timer.i2c.ms >0 ){
    if(--Timer.i2c.ms == 0){
      Timer.i2c.flag = true;
    }
  }
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
{
  redLED.Run(&redLED);

  
}


