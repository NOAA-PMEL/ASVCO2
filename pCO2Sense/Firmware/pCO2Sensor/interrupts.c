#include "msp430fr5994.h"
#include "sysinfo.h"
#include "logging.h"
/************************************************************************
 *					INTERRUPT VECTOR
 ************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
  switch (__even_in_range(P1IV, P1IV_P1IFG7))
  {
  case P1IV_NONE:
    break; /*Vector 0 - No Interrupt */
  case P1IV_P1IFG0:
    break; /* Vector 2 - Interrupt on Pin 1, Pin 0*/
  case P1IV_P1IFG1:
    break; /* Vector 4 - Interrupt on Pin 1, Pin 1 */
  case P1IV_P1IFG2:
    /** Timer 1 Counter */
    if (sysinfo.timer1.tFlag == true)
    {
      if (sysinfo.timer1.tCounter <= 0)
      {
        sysinfo.timer1.tFlag = false;
        __low_power_mode_off_on_exit();
      }
      else
      {
        sysinfo.timer1.tCounter--;
      }
    }

    /** Timer 2 Counter*/
    if (sysinfo.timer2.tFlag == true)
    {
      if (sysinfo.timer2.tCounter <= 0)
      {
        sysinfo.timer2.tFlag = false;
        __low_power_mode_off_on_exit();
      }
      else
      {
        sysinfo.timer2.tCounter--;
      }
    }
    break; /* Vector 6 - Interrupt on Pin 1, Pin 2 */
  case P1IV_P1IFG3:

    break; /* Vector 8 - Interrupt on Pin 1, Pin 3 */
  case P1IV_P1IFG4:
    /** nIRQ1 - Alarm Pin */
    SysMode = SYS_Sample;
    __low_power_mode_off_on_exit();
    break; /* Vector 10 - Interrupt on Pin 1, Pin 4 */
  case P1IV_P1IFG5:
    /** nIRQ2 - 8Hz Pin, used at the Sample Timer Counter */
    if (sysinfo.sampleTimer.tFlag == true)
    {
      if (sysinfo.sampleTimer.tCounter <= 0)
      {
        sysinfo.sampleTimer.tFlag = false;
        __low_power_mode_off_on_exit();
      }
      else
      {
        sysinfo.sampleTimer.tCounter--;
      }
    }
    break; /* Vector 12 - Interrupt on Pin 1, Pin 5 */
  case P1IV_P1IFG6:
    break; /* Vector 14 - Interrupt on Pin 1, Pin 6 */
  case P1IV_P1IFG7:
    break; /* Vector 16 - Interrupt on Pin 1, Pin 7 */
  default:
    break; /* Default case */
  }
}

/**************************** PORT2_VECTOR  *******************************/
#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
  switch (__even_in_range(P2IV, P2IV_P2IFG7))
  {
  case P2IV_NONE:
    break; /*Vector 0 - No Interrupt */
  case P2IV_P2IFG0:
    break; /* Vector 2 - Interrupt on Port 2, Pin 0*/
  case P2IV_P2IFG1:
    break; /* Vector 4 - Interrupt on Port 2, Pin 1 */
  case P2IV_P2IFG2:
    break; /* Vector 6 - Interrupt on Port 2, Pin 2 */
  case P2IV_P2IFG3:
    break; /* Vector 8 - Interrupt on Port 2, Pin 3 */
  case P2IV_P2IFG4:
    break; /* Vector 10 - Interrupt on Port 2, Pin 4 */
  case P2IV_P2IFG5:
    break; /* Vector 12 - Interrupt on Port 2, Pin 5 */
  case P2IV_P2IFG6:
    /** This Channel detects if a character has come in on the COMMAND Port */
    SysMode = SYS_Command;
    __low_power_mode_off_on_exit();
    break; /* Vector 14 - Interrupt on Port 2, Pin 6 */
  case P2IV_P2IFG7:
    /** This Channel detects if a character has come in on the CONSOLE Port */
    SysMode = SYS_Console;
    __low_power_mode_off_on_exit();
    break; /* Vector 16 - Interrupt on Port 2, Pin 7 */
  default:
    break; /* Default case */
  }
}

/**************************** PORT3_VECTOR  *******************************/
#pragma vector = PORT3_VECTOR
__interrupt void Port_3(void)
{
  switch (__even_in_range(P3IV, P3IV_P3IFG7))
  {
  case P3IV_NONE:
    break; /*Vector 0 - No Interrupt */
  case P3IV_P3IFG0:
    break; /* Vector 2 - Interrupt on Port 3, Pin 0 */
  case P3IV_P3IFG1:
    break; /* Vector 4 - Interrupt on Port 3, Pin 1 */
  case P3IV_P3IFG2:
    break; /* Vector 6 - Interrupt on Port 3, Pin 2 */
  case P3IV_P3IFG3:
    break; /* Vector 8 - Interrupt on Port 3, Pin 3 */
  case P3IV_P3IFG4:
    break; /* Vector 10 - Interrupt on Port 3, Pin 4 */
  case P3IV_P3IFG5:
    break; /* Vector 12 - Interrupt on Port 3, Pin 5 */
  case P3IV_P3IFG6:
    break; /* Vector 14 - Interrupt on Port 3, Pin 6*/
  case P3IV_P3IFG7:
    break; /* Vector 16 - Interrupt on Port 3, Pin 7 */
  default:
    break; /* Default case */
  }
}

/**************************** PORT4_VECTOR  *******************************/
#pragma vector = PORT4_VECTOR
__interrupt void Port_4(void)
{
  switch (__even_in_range(P4IV, P4IV_P4IFG7))
  {
  case P4IV_NONE:
    break; /*Vector 0 - No Interrupt */
  case P4IV_P4IFG0:
    break; /* Vector 2 - Interrupt on Port 4, Pin 0 */
  case P4IV_P4IFG1:
    break; /* Vector 4 - Interrupt on Port 4, Pin 1 */
  case P4IV_P4IFG2:
    break; /* Vector 6 - Interrupt on Port 4, Pin 2 */
  case P4IV_P4IFG3:
    break; /* Vector 8 - Interrupt on Port 4, Pin 3 */
  case P4IV_P4IFG4:
    break; /* Vector 10 - Interrupt on Port 4, Pin 4 */
  case P4IV_P4IFG5:
    break; /* Vector 12 - Interrupt on Port 4, Pin 5 */
  case P4IV_P4IFG6:
    break; /* Vector 14 - Interrupt on Port 4, Pin 6 */
  case P4IV_P4IFG7:
    break; /* Vector 16 - Interrupt on Port 4, Pin 7 */
  default:
    break; /* Default case */
  }
}

/**************************** UART_A0  *******************************/
/************************* Console Port ******************************/
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
  case USCI_NONE:
    break;
  case USCI_UART_UCRXIFG:
    /** Put the Rx value in the buffer and increment the buffer */
    *pConsoleRxBuffer = UCA0RXBUF;
    SysMode = SYS_Console;
    /** Timeout */
    if (ConsoleTimeoutSuppress == true)
    {
      /** Restart the UART timeout */
      TA1R = 0x00;
    }

    /** Look for consecutive Ctrl-C */
    if (*pConsoleRxBuffer == 0x03)
    {
      if (++Console_CtrlC_Count >= 3)
      {
//        OutString("", LOG_OUTPUT_Reset);
        Reset();
        PMMCTL0 = (0XA500 | PMMSWPOR);
      }
    }
    else
    {
      /** Add Echo and backspace */
      if (ConsoleMode == SYS_Console_Terminal)
      {
        /** Backspace */
        if (*pConsoleRxBuffer == 0x08)
        {
          if (pConsoleRxBuffer > &ConsoleRxBuffer[0])
          {
            UCA0TXBUF = *pConsoleRxBuffer;
            __delay_cycles(500);
            UCA0TXBUF = ' ';
            __delay_cycles(500);
            UCA0TXBUF = 0x08;
            pConsoleRxBuffer -= 2;
          }
          else
          {
            pConsoleRxBuffer--;
          }
        }
        else
        {

          UCA0TXBUF = *pConsoleRxBuffer;
        }
        Console_CtrlC_Count = 0;
      }
    }

    /** Increment the Buffer index */
    pConsoleRxBuffer++;

    /** Null the buffer before next write */
    *pConsoleRxBuffer = NULL;

    break;
  case USCI_UART_UCTXIFG:
    /** Clear the Tx Flag */
    UCA0IFG &= ~(UCTXIFG);
    break;
  case USCI_UART_UCSTTIFG:
    UCA0IFG &= ~(UCSTTIFG);
    break;
  case USCI_UART_UCTXCPTIFG:
    UCA0IFG &= ~(UCTXCPTIFG);
    break;
  }
}

/**************************** UART_A1  *******************************/
/************************* Command Port ******************************/
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
  case USCI_NONE:
    break;
  case USCI_UART_UCRXIFG:
    /** Put the Rx value in the buffer */
    SysMode = SYS_Command;
    *pCommandRxBuffer = UCA1RXBUF;

    /** Look for consecutive Ctrl-C or ESC */
    if( 
      (*pCommandRxBuffer == 0x03) ||
      (*pCommandRxBuffer == 0x1B)
    )
    {
      if (++Console_CtrlC_Count >= 3)
      {
//        OutString("", LOG_OUTPUT_Reset);
        Reset();
        SysMode = SYS_Command;
        PMMCTL0 = (0XA500 | PMMSWPOR);
      }
    }
    else
    {
      /** Add Echo and backspace */
      if (ConsoleMode == SYS_Console_Terminal)
      {
        /** Backspace */
        if (*pCommandRxBuffer == 0x08)
        {
          if (pCommandRxBuffer > &CommandRxBuffer[0])
          {
            UCA1TXBUF = *pCommandRxBuffer;
            __delay_cycles(500);
            UCA1TXBUF = ' ';
            __delay_cycles(500);
            UCA1TXBUF = 0x08;
            pCommandRxBuffer -= 2;
          }
          else
          {
            pCommandRxBuffer--;
          }
        }
        else
        {

          UCA1TXBUF = *pCommandRxBuffer;
        }
        Console_CtrlC_Count = 0;
      }
    }
    if (CommandTimeoutSuppress == true)
    {
      /** Restart the UART timeout */
      TA1R = 0x00;
    }

    /** Increment the Buffer index */
    pCommandRxBuffer++;

    /** Null the buffer before next write */
    *pCommandRxBuffer = NULL;
    break;
  case USCI_UART_UCTXIFG:
    UCA1IFG &= ~(UCTXIFG);
    break;
  case USCI_UART_UCSTTIFG:
    break;
  case USCI_UART_UCTXCPTIFG:
    break;
  }
}

/**************************** UART_A2  *******************************/
/************************* Licor Port ******************************/
#pragma vector = USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  switch (__even_in_range(UCA2IV, USCI_UART_UCTXCPTIFG))
  {
  case USCI_NONE:
    break;
  case USCI_UART_UCRXIFG:
    *pLicorRxBuffer = UCA2RXBUF;
    if (*pLicorRxBuffer == '\n')
    {
      LicorRxFlag = true;
    }
    if (++pLicorRxBuffer < (&LicorRxBuffer[0] + LICOR_BUFFER_LEN))
    {
      *pLicorRxBuffer = NULL;
    }
    break;
  case USCI_UART_UCTXIFG:
    UCA2IFG &= ~(UCTXIFG);
    break;
  case USCI_UART_UCSTTIFG:
    break;
  case USCI_UART_UCTXCPTIFG:
    break;
  }
}

/**************************** UART_B0  *******************************/
/************************* RTC I2C Port ******************************/
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  static uint8_t rxCount = 0;
  static uint8_t txCount = 1;
  switch (__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
  case USCI_NONE:
    break; /* Vector 0: No interrupts */
  case USCI_I2C_UCALIFG:
    break;                 /* Vector 2: ALIFG */
  case USCI_I2C_UCNACKIFG: /* Vector 4: NACKIFG */
    EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);
    __delay_cycles(2);
    break;
  case USCI_I2C_UCSTTIFG:
    break; /* Vector 6: STTIFG */
  case USCI_I2C_UCSTPIFG:
    break; /* Vector 8: STPIFG */
  case USCI_I2C_UCRXIFG3:
    break; /* Vector 10: RXIFG3 */
  case USCI_I2C_UCTXIFG3:
    break; /* Vector 12: TXIFG3 */
  case USCI_I2C_UCRXIFG2:
    break; /* Vector 14: RXIFG2 */
  case USCI_I2C_UCTXIFG2:
    break; /* Vector 16: TXIFG2 */
  case USCI_I2C_UCRXIFG1:
    break; /* Vector 18: RXIFG1 */
  case USCI_I2C_UCTXIFG1:
    break; /* Vector 20: TXIFG1 */
  case USCI_I2C_UCRXIFG0:
    /* Receive Vector */
    I2C_B0_RxData[rxCount] = UCB0RXBUF;

    if (++rxCount >= I2C_B0_MaxCount - 1)
    {
      EUSCI_B_I2C_masterReceiveMultiByteStop(EUSCI_B0_BASE);
      HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW0) |= UCTXSTP;
      I2C_B0_MaxCount = 0;
      rxCount = 0;
    }
    break;                /* Vector 22: RXIFG0 */
  case USCI_I2C_UCTXIFG0: /* Vector 24: TXIFG0 */
    /* Transmit Vector */
    _delay_ms(10);
    UCB0TXBUF = I2C_B0_TxData[txCount];
    if (++txCount >= I2C_B0_TxCount)
    {
      EUSCI_B_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
      HWREG16(EUSCI_B0_BASE + OFS_UCBxCTLW0) |= UCTXSTP;
      I2C_B0_TxCount = 0;
      txCount = 1;
      __bic_SR_register_on_exit(CPUOFF);
    }
    break;
  case USCI_I2C_UCBIT9IFG:
    break; /* Vector 30: 9th bit */
  case USCI_I2C_UCBCNTIFG:
    break;

  default:
    break;
  }
}

/**************************** UART_B1  *******************************/
/************************* RH I2C Port ******************************/
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
  static uint8_t rxCount = 0;
  static uint8_t txCount = 1;
  switch (__even_in_range(UCB1IV, USCI_I2C_UCBIT9IFG))
  {
  case USCI_NONE:
    break; /* Vector 0: No interrupts */
  case USCI_I2C_UCALIFG:
    break;                 /* Vector 2: ALIFG */
  case USCI_I2C_UCNACKIFG: /* Vector 4: NACKIFG */
    break;
  case USCI_I2C_UCSTTIFG:
    break; /* Vector 6: STTIFG */
  case USCI_I2C_UCSTPIFG:
    break; /* Vector 8: STPIFG */
  case USCI_I2C_UCRXIFG3:
    break; /* Vector 10: RXIFG3 */
  case USCI_I2C_UCTXIFG3:
    break; /* Vector 12: TXIFG3 */
  case USCI_I2C_UCRXIFG2:
    break; /* Vector 14: RXIFG2 */
  case USCI_I2C_UCTXIFG2:
    break; /* Vector 16: TXIFG2 */
  case USCI_I2C_UCRXIFG1:
    break; /* Vector 18: RXIFG1 */
  case USCI_I2C_UCTXIFG1:
    break; /* Vector 20: TXIFG1 */
  case USCI_I2C_UCRXIFG0:
    /* Receive Vector */
    _delay_ms(1);
    I2C_B0_RxData[rxCount] = UCB1RXBUF;

    if (++rxCount >= I2C_B0_MaxCount)
    {
      EUSCI_B_I2C_masterReceiveMultiByteStop(EUSCI_B1_BASE);
      HWREG16(EUSCI_B1_BASE + OFS_UCBxCTLW0) |= UCTXSTP;
      I2C_B0_MaxCount = 0;
      rxCount = 0;
      EUSCI_B_I2C_disableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    }
    break;                /* Vector 22: RXIFG0 */
  case USCI_I2C_UCTXIFG0: /* Vector 24: TXIFG0 */
    /* Transmit Vector */
    _delay_ms(1);
    UCB1TXBUF = I2C_B0_TxData[txCount];
    if (++txCount >= I2C_B0_TxCount)
    {
      EUSCI_B_I2C_masterSendMultiByteStop(EUSCI_B1_BASE);
      HWREG16(EUSCI_B1_BASE + OFS_UCBxCTLW0) |= UCTXSTP;
      I2C_B0_TxCount = 0;
      txCount = 1;
      __bic_SR_register_on_exit(CPUOFF);
    }
    break;
  case USCI_I2C_UCBIT9IFG:
    break; /* Vector 30: 9th bit */
  case USCI_I2C_UCBCNTIFG:
    break;

  default:
    break;
  }
}

/**************************** UART_B2 *******************************/
/************************* UART SPI Port ******************************/
#pragma vector = USCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
{
  switch (__even_in_range(UCB2IV, USCI_SPI_UCTXIFG))
  {
  case USCI_NONE:
    break;
  case USCI_SPI_UCRXIFG:
    UCB2IFG &= ~UCRXIFG0;
    break;
  case USCI_SPI_UCTXIFG:
    UCB2IFG &= ~UCTXIFG0;
    break;
  default:
    break;
  }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
  /** Watchdog Wakeup Timer */
  if (SysMode == SYS_Watchdog)
  {
    __low_power_mode_off_on_exit();
  }
}
