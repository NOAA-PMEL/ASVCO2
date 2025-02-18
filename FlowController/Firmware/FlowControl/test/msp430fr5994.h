#ifndef _MSP430FR5994_H
#define _MSP430FR5994_H

#include <stdint.h>


/* CSCTL2 Control Bits */
#define SELM                             (0x0007)        /* Selects the MCLK source */
#define SELM_L                           (0x0007)
#define SELM0                            (0x0001)        /* Selects the MCLK source */
#define SELM0_L                          (0x0001)
#define SELM1                            (0x0002)        /* Selects the MCLK source */
#define SELM1_L                          (0x0002)
#define SELM2                            (0x0004)        /* Selects the MCLK source */
#define SELM2_L                          (0x0004)
#define SELM_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK */
#define SELM_1                           (0x0001)        /* VLOCLK */
#define SELM_1_L                         (0x0001)
#define SELM_2                           (0x0002)        /* LFMODCLK */
#define SELM_2_L                         (0x0002)
#define SELM_3                           (0x0003)        /* DCOCLK */
#define SELM_3_L                         (0x0003)
#define SELM_4                           (0x0004)        /* MODCLK */
#define SELM_4_L                         (0x0004)
#define SELM_5                           (0x0005)        /* HFXTCLK when HFXT available, otherwise DCOCLK */
#define SELM_5_L                         (0x0005)
#define SELM__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK */
#define SELM__VLOCLK                     (0x0001)        /* VLOCLK */
#define SELM__VLOCLK_L                   (0x0001)
#define SELM__LFMODCLK                   (0x0002)        /* LFMODCLK */
#define SELM__LFMODCLK_L                 (0x0002)
#define SELM__DCOCLK                     (0x0003)        /* DCOCLK */
#define SELM__DCOCLK_L                   (0x0003)
#define SELM__MODCLK                     (0x0004)        /* MODCLK */
#define SELM__MODCLK_L                   (0x0004)
#define SELM__HFXTCLK                    (0x0005)        /* HFXTCLK when HFXT available, otherwise DCOCLK */
#define SELM__HFXTCLK_L                  (0x0005)
#define SELS                             (0x0070)        /* Selects the SMCLK source */
#define SELS_L                           (0x0070)
#define SELS0                            (0x0010)        /* Selects the SMCLK source */
#define SELS0_L                          (0x0010)
#define SELS1                            (0x0020)        /* Selects the SMCLK source */
#define SELS1_L                          (0x0020)
#define SELS2                            (0x0040)        /* Selects the SMCLK source */
#define SELS2_L                          (0x0040)
#define SELS_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELS_1                           (0x0010)        /* VLOCLK */
#define SELS_1_L                         (0x0010)
#define SELS_2                           (0x0020)        /* LFMODCLK */
#define SELS_2_L                         (0x0020)
#define SELS_3                           (0x0030)        /* DCOCLK */
#define SELS_3_L                         (0x0030)
#define SELS_4                           (0x0040)        /* MODCLK */
#define SELS_4_L                         (0x0040)
#define SELS_5                           (0x0050)        /* HFXTCLK when HFXT available, otherwise DCOCLK. */
#define SELS_5_L                         (0x0050)
#define SELS__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELS__VLOCLK                     (0x0010)        /* VLOCLK */
#define SELS__VLOCLK_L                   (0x0010)
#define SELS__LFMODCLK                   (0x0020)        /* LFMODCLK */
#define SELS__LFMODCLK_L                 (0x0020)
#define SELS__DCOCLK                     (0x0030)        /* DCOCLK */
#define SELS__DCOCLK_L                   (0x0030)
#define SELS__MODCLK                     (0x0040)        /* MODCLK */
#define SELS__MODCLK_L                   (0x0040)
#define SELS__HFXTCLK                    (0x0050)        /* HFXTCLK when HFXT available, otherwise DCOCLK. */
#define SELS__HFXTCLK_L                  (0x0050)
#define SELA                             (0x0700)        /* Selects the ACLK source */
#define SELA_H                           (0x0007)
#define SELA0                            (0x0100)        /* Selects the ACLK source */
#define SELA0_H                          (0x0001)
#define SELA1                            (0x0200)        /* Selects the ACLK source */
#define SELA1_H                          (0x0002)
#define SELA2                            (0x0400)        /* Selects the ACLK source */
#define SELA2_H                          (0x0004)
#define SELA_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELA_1                           (0x0100)        /* VLOCLK */
#define SELA_1_H                         (0x0001)
#define SELA_2                           (0x0200)        /* LFMODCLK */
#define SELA_2_H                         (0x0002)
#define SELA__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELA__VLOCLK                     (0x0100)        /* VLOCLK */
#define SELA__VLOCLK_H                   (0x0001)
#define SELA__LFMODCLK                   (0x0200)        /* LFMODCLK */
#define SELA__LFMODCLK_H                 (0x0002)

/* CSCTL6 Control Bits */
#define ACLKREQEN                        (0x0001)        /* ACLK clock request enable */
#define ACLKREQEN_L                      (0x0001)
#define ACLKREQEN_0                      (0x0000)        /* ACLK conditional requests are disabled */
#define ACLKREQEN_1                      (0x0001)        /* ACLK conditional requests are enabled */
#define ACLKREQEN_1_L                    (0x0001)
#define ACLKREQEN__DISABLE               (0x0000)        /* ACLK conditional requests are disabled */
#define ACLKREQEN__ENABLE                (0x0001)        /* ACLK conditional requests are enabled */
#define ACLKREQEN__ENABLE_L              (0x0001)
#define MCLKREQEN                        (0x0002)        /* MCLK clock request enable */
#define MCLKREQEN_L                      (0x0002)
#define MCLKREQEN_0                      (0x0000)        /* MCLK conditional requests are disabled */
#define MCLKREQEN_1                      (0x0002)        /* MCLK conditional requests are enabled */
#define MCLKREQEN_1_L                    (0x0002)
#define MCLKREQEN__DISABLE               (0x0000)        /* MCLK conditional requests are disabled */
#define MCLKREQEN__ENABLE                (0x0002)        /* MCLK conditional requests are enabled */
#define MCLKREQEN__ENABLE_L              (0x0002)
#define SMCLKREQEN                       (0x0004)        /* SMCLK clock request enable */
#define SMCLKREQEN_L                     (0x0004)
#define SMCLKREQEN_0                     (0x0000)        /* SMCLK conditional requests are disabled */
#define SMCLKREQEN_1                     (0x0004)        /* SMCLK conditional requests are enabled */
#define SMCLKREQEN_1_L                   (0x0004)
#define SMCLKREQEN__DISABLE              (0x0000)        /* SMCLK conditional requests are disabled */
#define SMCLKREQEN__ENABLE               (0x0004)        /* SMCLK conditional requests are enabled */
#define SMCLKREQEN__ENABLE_L             (0x0004)
#define MODCLKREQEN                      (0x0008)        /* MODCLK clock request enable */
#define MODCLKREQEN_L                    (0x0008)
#define MODCLKREQEN_0                    (0x0000)        /* MODCLK conditional requests are disabled */
#define MODCLKREQEN_1                    (0x0008)        /* MODCLK conditional requests are enabled */
#define MODCLKREQEN_1_L                  (0x0008)
#define MODCLKREQEN__DISABLE             (0x0000)        /* MODCLK conditional requests are disabled */
#define MODCLKREQEN__ENABLE              (0x0008)        /* MODCLK conditional requests are enabled */
#define MODCLKREQEN__ENABLE_L            (0x0008)

/* CSCTL4 Control Bits */
#define LFXTOFF                          (0x0001)        /* LFXT off */
#define LFXTOFF_L                        (0x0001)
#define LFXTOFF_0                        (0x0000)        /* LFXT is on if LFXT is selected via the port selection and LFXT
                                                            is not in bypass mode of operation */
#define LFXTOFF_1                        (0x0001)        /* LFXT is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK */
#define LFXTOFF_1_L                      (0x0001)
#define SMCLKOFF                         (0x0002)        /* SMCLK off */
#define SMCLKOFF_L                       (0x0002)
#define SMCLKOFF_0                       (0x0000)        /* SMCLK on */
#define SMCLKOFF_1                       (0x0002)        /* SMCLK off */
#define SMCLKOFF_1_L                     (0x0002)
#define VLOOFF                           (0x0008)        /* VLO off */
#define VLOOFF_L                         (0x0008)
#define VLOOFF_0                         (0x0000)        /* VLO is on */
#define VLOOFF_1                         (0x0008)        /* VLO is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK or if not used as a source for the RTC in LPM3.5 */
#define VLOOFF_1_L                       (0x0008)
#define LFXTBYPASS                       (0x0010)        /* LFXT bypass select */
#define LFXTBYPASS_L                     (0x0010)
#define LFXTBYPASS_0                     (0x0000)        /* LFXT sourced from external crystal */
#define LFXTBYPASS_1                     (0x0010)        /* LFXT sourced from external clock signal */
#define LFXTBYPASS_1_L                   (0x0010)
#define LFXTDRIVE                        (0x00c0)        /* LFXT oscillator current */
#define LFXTDRIVE_L                      (0x00c0)
#define LFXTDRIVE0                       (0x0040)        /* LFXT oscillator current */
#define LFXTDRIVE0_L                     (0x0040)
#define LFXTDRIVE1                       (0x0080)        /* LFXT oscillator current */
#define LFXTDRIVE1_L                     (0x0080)
#define LFXTDRIVE_0                      (0x0000)        /* Lowest drive strength and current consumption LFXT oscillator */
#define LFXTDRIVE_1                      (0x0040)        /* Increased drive strength LFXT oscillator */
#define LFXTDRIVE_1_L                    (0x0040)
#define LFXTDRIVE_2                      (0x0080)        /* Increased drive strength LFXT oscillator */
#define LFXTDRIVE_2_L                    (0x0080)
#define LFXTDRIVE_3                      (0x00c0)        /* Maximum drive strength and maximum current consumption LFXT 
                                                            oscillator */
#define LFXTDRIVE_3_L                    (0x00c0)
#define HFXTOFF                          (0x0100)        /* Turns off the HFXT oscillator */
#define HFXTOFF_H                        (0x0001)
#define HFXTOFF_0                        (0x0000)        /* HFXT is on if HFXT is selected via the port selection and HFXT
                                                            is not in bypass mode of operation */
#define HFXTOFF_1                        (0x0100)        /* HFXT is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK */
#define HFXTOFF_1_H                      (0x0001)
#define HFFREQ                           (0x0c00)        /* HFXT frequency selection */
#define HFFREQ_H                         (0x000c)
#define HFFREQ0                          (0x0400)        /* HFXT frequency selection */
#define HFFREQ0_H                        (0x0004)
#define HFFREQ1                          (0x0800)        /* HFXT frequency selection */
#define HFFREQ1_H                        (0x0008)
#define HFFREQ_0                         (0x0000)        /* 0 to 4 MHz */
#define HFFREQ_1                         (0x0400)        /* Greater than 4 MHz to 8 MHz */
#define HFFREQ_1_H                       (0x0004)
#define HFFREQ_2                         (0x0800)        /* Greater than 8 MHz to 16 MHz */
#define HFFREQ_2_H                       (0x0008)
#define HFFREQ_3                         (0x0c00)        /* Greater than 16 MHz to 24 MHz */
#define HFFREQ_3_H                       (0x000c)
#define HFXTBYPASS                       (0x1000)        /* HFXT bypass select */
#define HFXTBYPASS_H                     (0x0010)
#define HFXTBYPASS_0                     (0x0000)        /* HFXT sourced from external crystal */
#define HFXTBYPASS_1                     (0x1000)        /* HFXT sourced from external clock signal */
#define HFXTBYPASS_1_H                   (0x0010)
#define HFXTDRIVE                        (0xc000)        /* HFXT oscillator current */
#define HFXTDRIVE_H                      (0x00c0)
#define HFXTDRIVE0                       (0x4000)        /* HFXT oscillator current */
#define HFXTDRIVE0_H                     (0x0040)
#define HFXTDRIVE1                       (0x8000)        /* HFXT oscillator current */
#define HFXTDRIVE1_H                     (0x0080)
#define HFXTDRIVE_0                      (0x0000)        /* Lowest current consumption */
#define HFXTDRIVE_1                      (0x4000)        /* Increased drive strength HFXT oscillator */
#define HFXTDRIVE_1_H                    (0x0040)
#define HFXTDRIVE_2                      (0x8000)        /* Increased drive strength HFXT oscillator */
#define HFXTDRIVE_2_H                    (0x0080)
#define HFXTDRIVE_3                      (0xc000)        /* Maximum drive strength HFXT oscillator */


/* UCA0IE Control Bits */
#define UCRXIE                           (0x0001)        /* Receive interrupt enable */
#define UCRXIE_L                         (0x0001)
#define UCRXIE_0                         (0x0000)        /* Interrupt disabled */
#define UCRXIE_1                         (0x0001)        /* Interrupt enabled */
#define UCRXIE_1_L                       (0x0001)
#define UCTXIE                           (0x0002)        /* Transmit interrupt enable */
#define UCTXIE_L                         (0x0002)
#define UCTXIE_0                         (0x0000)        /* Interrupt disabled */
#define UCTXIE_1                         (0x0002)        /* Interrupt enabled */
#define UCTXIE_1_L                       (0x0002)
#define UCSTTIE                          (0x0004)        /* Start bit interrupt enable */
#define UCSTTIE_L                        (0x0004)
#define UCSTTIE_0                        (0x0000)        /* Interrupt disabled */
#define UCSTTIE_1                        (0x0004)        /* Interrupt enabled */
#define UCSTTIE_1_L                      (0x0004)
#define UCTXCPTIE                        (0x0008)        /* Transmit complete interrupt enable */
#define UCTXCPTIE_L                      (0x0008)
#define UCTXCPTIE_0                      (0x0000)        /* Interrupt disabled */
#define UCTXCPTIE_1                      (0x0008)        /* Interrupt enabled */
#define UCTXCPTIE_1_L                    (0x0008)

/* UCA0IFG Control Bits */
#define UCRXIFG                          (0x0001)        /* Receive interrupt flag */
#define UCRXIFG_L                        (0x0001)
#define UCRXIFG_0                        (0x0000)        /* No interrupt pending */
#define UCRXIFG_1                        (0x0001)        /* Interrupt pending */
#define UCRXIFG_1_L                      (0x0001)
#define UCTXIFG                          (0x0002)        /* Transmit interrupt flag */
#define UCTXIFG_L                        (0x0002)
#define UCTXIFG_0                        (0x0000)        /* No interrupt pending */
#define UCTXIFG_1                        (0x0002)        /* Interrupt pending */
#define UCTXIFG_1_L                      (0x0002)
#define UCSTTIFG                         (0x0004)        /* Start bit interrupt flag */
#define UCSTTIFG_L                       (0x0004)
#define UCSTTIFG_0                       (0x0000)        /* No interrupt pending */
#define UCSTTIFG_1                       (0x0004)        /* Interrupt pending */
#define UCSTTIFG_1_L                     (0x0004)
#define UCTXCPTIFG                       (0x0008)        /* Transmit ready interrupt enable */
#define UCTXCPTIFG_L                     (0x0008)
#define UCTXCPTIFG_0                     (0x0000)        /* No interrupt pending */
#define UCTXCPTIFG_1                     (0x0008)        /* Interrupt pending */
#define UCTXCPTIFG_1_L                   (0x0008)


/* UCA0CTLW0 Control Bits */
#define UCSWRST                          (0x0001)        /* Software reset enable */
#define UCSWRST_L                        (0x0001)
#define UCSWRST_0                        (0x0000)        /* Disabled. eUSCI_A reset released for operation */
#define UCSWRST_1                        (0x0001)        /* Enabled. eUSCI_A logic held in reset state */
#define UCSWRST_1_L                      (0x0001)
#define UCSWRST__DISABLE                 (0x0000)        /* Disabled. eUSCI_A reset released for operation */
#define UCSWRST__ENABLE                  (0x0001)        /* Enabled. eUSCI_A logic held in reset state */
#define UCSWRST__ENABLE_L                (0x0001)
#define UCTXBRK                          (0x0002)        /* Transmit break */
#define UCTXBRK_L                        (0x0002)
#define UCTXBRK_0                        (0x0000)        /* Next frame transmitted is not a break */
#define UCTXBRK_1                        (0x0002)        /* Next frame transmitted is a break or a break/synch */
#define UCTXBRK_1_L                      (0x0002)
#define UCTXADDR                         (0x0004)        /* Transmit address */
#define UCTXADDR_L                       (0x0004)
#define UCTXADDR_0                       (0x0000)        /* Next frame transmitted is data */
#define UCTXADDR_1                       (0x0004)        /* Next frame transmitted is an address */
#define UCTXADDR_1_L                     (0x0004)
#define UCDORM                           (0x0008)        /* Dormant */
#define UCDORM_L                         (0x0008)
#define UCDORM_0                         (0x0000)        /* Not dormant. All received characters set UCRXIFG. */
#define UCDORM_1                         (0x0008)        /* Dormant. Only characters that are preceded by an idle-line or 
                                                            with address bit set UCRXIFG. In UART mode with automatic baud-
                                                            rate detection, only the combination of a break and synch 
                                                            field sets UCRXIFG. */
#define UCDORM_1_L                       (0x0008)
#define UCBRKIE                          (0x0010)        /* Receive break character interrupt enable */
#define UCBRKIE_L                        (0x0010)
#define UCBRKIE_0                        (0x0000)        /* Received break characters do not set UCRXIFG */
#define UCBRKIE_1                        (0x0010)        /* Received break characters set UCRXIFG */
#define UCBRKIE_1_L                      (0x0010)
#define UCRXEIE                          (0x0020)        /* Receive erroneous-character interrupt enable */
#define UCRXEIE_L                        (0x0020)
#define UCRXEIE_0                        (0x0000)        /* Erroneous characters rejected and UCRXIFG is not set */
#define UCRXEIE_1                        (0x0020)        /* Erroneous characters received set UCRXIFG */
#define UCRXEIE_1_L                      (0x0020)
#define UCSSEL                           (0x00c0)        /* eUSCI_A clock source select */
#define UCSSEL_L                         (0x00c0)
#define UCSSEL0                          (0x0040)        /* eUSCI_A clock source select */
#define UCSSEL0_L                        (0x0040)
#define UCSSEL1                          (0x0080)        /* eUSCI_A clock source select */
#define UCSSEL1_L                        (0x0080)
#define UCSSEL_0                         (0x0000)        /* UCLK */
#define UCSSEL_1                         (0x0040)        /* ACLK */
#define UCSSEL_1_L                       (0x0040)
#define UCSSEL_2                         (0x0080)        /* SMCLK */
#define UCSSEL_2_L                       (0x0080)
#define UCSSEL__UCLK                     (0x0000)        /* UCLK */
#define UCSSEL__ACLK                     (0x0040)        /* ACLK */
#define UCSSEL__ACLK_L                   (0x0040)
#define UCSSEL__SMCLK                    (0x0080)        /* SMCLK */
#define UCSSEL__SMCLK_L                  (0x0080)
#define UCSYNC                           (0x0100)        /* Synchronous mode enable */
#define UCSYNC_H                         (0x0001)
#define UCSYNC_0                         (0x0000)        /* Asynchronous mode */
#define UCSYNC_1                         (0x0100)        /* Synchronous mode */
#define UCSYNC_1_H                       (0x0001)
#define UCSYNC__ASYNC                    (0x0000)        /* Asynchronous mode */
#define UCSYNC__SYNC                     (0x0100)        /* Synchronous mode */
#define UCSYNC__SYNC_H                   (0x0001)
#define UCMODE                           (0x0600)        /* eUSCI_A mode */
#define UCMODE_H                         (0x0006)
#define UCMODE0                          (0x0200)        /* eUSCI_A mode */
#define UCMODE0_H                        (0x0002)
#define UCMODE1                          (0x0400)        /* eUSCI_A mode */
#define UCMODE1_H                        (0x0004)
#define UCMODE_0                         (0x0000)        /* UART mode */
#define UCMODE_1                         (0x0200)        /* Idle-line multiprocessor mode */
#define UCMODE_1_H                       (0x0002)
#define UCMODE_2                         (0x0400)        /* Address-bit multiprocessor mode */
#define UCMODE_2_H                       (0x0004)
#define UCMODE_3                         (0x0600)        /* UART mode with automatic baud-rate detection */
#define UCMODE_3_H                       (0x0006)
#define UCSPB                            (0x0800)        /* Stop bit select */
#define UCSPB_H                          (0x0008)
#define UCSPB_0                          (0x0000)        /* One stop bit */
#define UCSPB_1                          (0x0800)        /* Two stop bits */
#define UCSPB_1_H                        (0x0008)
#define UC7BIT                           (0x1000)        /* Character length */
#define UC7BIT_H                         (0x0010)
#define UC7BIT_0                         (0x0000)        /* 8-bit data */
#define UC7BIT_1                         (0x1000)        /* 7-bit data */
#define UC7BIT_1_H                       (0x0010)
#define UC7BIT__8BIT                     (0x0000)        /* 8-bit data */
#define UC7BIT__7BIT                     (0x1000)        /* 7-bit data */
#define UC7BIT__7BIT_H                   (0x0010)
#define UCMSB                            (0x2000)        /* MSB first select */
#define UCMSB_H                          (0x0020)
#define UCMSB_0                          (0x0000)        /* LSB first */
#define UCMSB_1                          (0x2000)        /* MSB first */
#define UCMSB_1_H                        (0x0020)
#define UCPAR                            (0x4000)        /* Parity select */
#define UCPAR_H                          (0x0040)
#define UCPAR_0                          (0x0000)        /* Odd parity */
#define UCPAR_1                          (0x4000)        /* Even parity */
#define UCPAR_1_H                        (0x0040)
#define UCPAR__ODD                       (0x0000)        /* Odd parity */
#define UCPAR__EVEN                      (0x4000)        /* Even parity */
#define UCPAR__EVEN_H                    (0x0040)
#define UCPEN                            (0x8000)        /* Parity enable */
#define UCPEN_H                          (0x0080)
#define UCPEN_0                          (0x0000)        /* Parity disabled */
#define UCPEN_1                          (0x8000)        /* Parity enabled. Parity bit is generated (UCAxTXD) and expected
                                                             (UCAxRXD). In address-bit multiprocessor mode, the address 
                                                            bit is included in the parity calculation. */
#define UCPEN_1_H                        (0x0080)
#define UCSWRST     (0x0001)
#define UCSSEL__UCLK                     (0x0000)        /* UCLK */
#define UCSSEL__ACLK                     (0x0040)        /* ACLK */
#define UCSSEL__ACLK_L                   (0x0040)
#define UCSSEL__SMCLK                    (0x0080)        /* SMCLK */
#define UCSSEL__SMCLK_L                  (0x0080)

#define __MSP430_BASEADDRESS_EUSCI_A0__ 0x05C0
#define EUSCI_A0_BASE          __MSP430_BASEADDRESS_EUSCI_A0__
#define __MSP430_BASEADDRESS_EUSCI_A1__ 0x05E0
#define EUSCI_A1_BASE          __MSP430_BASEADDRESS_EUSCI_A1__
#define __MSP430_BASEADDRESS_EUSCI_A2__ 0x0600
#define EUSCI_A2_BASE          __MSP430_BASEADDRESS_EUSCI_A2__
#define __MSP430_BASEADDRESS_EUSCI_A3__ 0x0620
#define EUSCI_A3_BASE          __MSP430_BASEADDRESS_EUSCI_A3__

#define __MSP430_BASEADDRESS_EUSCI_B0__ 0x0640
#define EUSCI_B0_BASE          __MSP430_BASEADDRESS_EUSCI_B0__
#define __MSP430_BASEADDRESS_EUSCI_B1__ 0x0680
#define EUSCI_B1_BASE          __MSP430_BASEADDRESS_EUSCI_B1__
#define __MSP430_BASEADDRESS_EUSCI_B2__ 0x06C0
#define EUSCI_B2_BASE          __MSP430_BASEADDRESS_EUSCI_B2__
#define __MSP430_BASEADDRESS_EUSCI_B3__ 0x0700
#define EUSCI_B3_BASE          __MSP430_BASEADDRESS_EUSCI_B3__


/* UCB0CTLW0 Control Bits */
#define UCTXSTT                          (0x0002)        /* Transmit START condition in master mode */
#define UCTXSTT_L                        (0x0002)
#define UCTXSTT_0                        (0x0000)        /* Do not generate START condition */
#define UCTXSTT_1                        (0x0002)        /* Generate START condition */
#define UCTXSTT_1_L                      (0x0002)
#define UCTXSTP                          (0x0004)        /* Transmit STOP condition in master mode */
#define UCTXSTP_L                        (0x0004)
#define UCTXSTP_0                        (0x0000)        /* No STOP generated */
#define UCTXSTP_1                        (0x0004)        /* Generate STOP */
#define UCTXSTP_1_L                      (0x0004)
#define UCTXNACK                         (0x0008)        /* Transmit a NACK */
#define UCTXNACK_L                       (0x0008)
#define UCTXNACK_0                       (0x0000)        /* Acknowledge normally */
#define UCTXNACK_1                       (0x0008)        /* Generate NACK */
#define UCTXNACK_1_L                     (0x0008)
#define UCTR                             (0x0010)        /* Transmitter/receiver */
#define UCTR_L                           (0x0010)
#define UCTR_0                           (0x0000)        /* Receiver */
#define UCTR_1                           (0x0010)        /* Transmitter */
#define UCTR_1_L                         (0x0010)
#define UCTR__RX                         (0x0000)        /* Receiver */
#define UCTR__TX                         (0x0010)        /* Transmitter */
#define UCTR__TX_L                       (0x0010)
#define UCTXACK                          (0x0020)        /* Transmit ACK condition in slave mode */
#define UCTXACK_L                        (0x0020)
#define UCTXACK_0                        (0x0000)        /* Do not acknowledge the slave address */
#define UCTXACK_1                        (0x0020)        /* Acknowledge the slave address */
#define UCTXACK_1_L                      (0x0020)
#define UCSSEL_3                         (0x00c0)        /* SMCLK */
#define UCSSEL_3_L                       (0x00c0)
#define UCSSEL__UCLKI                    (0x0000)        /* UCLKI */
#define UCMM                             (0x2000)        /* Multi-master environment select */
#define UCMM_H                           (0x0020)
#define UCMM_0                           (0x0000)        /* Single master environment. There is no other master in the 
                                                            system. The address compare unit is disabled. */
#define UCMM_1                           (0x2000)        /* Multi-master environment */
#define UCMM_1_H                         (0x0020)
#define UCMM__SINGLE                     (0x0000)        /* Single master environment. There is no other master in the 
                                                            system. The address compare unit is disabled. */
#define UCMM__MULTI                      (0x2000)        /* Multi-master environment */
#define UCMM__MULTI_H                    (0x0020)
#define UCSLA10                          (0x4000)        /* Slave addressing mode select */
#define UCSLA10_H                        (0x0040)
#define UCSLA10_0                        (0x0000)        /* Address slave with 7-bit address */
#define UCSLA10_1                        (0x4000)        /* Address slave with 10-bit address */
#define UCSLA10_1_H                      (0x0040)
#define UCSLA10__7BIT                    (0x0000)        /* Address slave with 7-bit address */
#define UCSLA10__10BIT                   (0x4000)        /* Address slave with 10-bit address */
#define UCSLA10__10BIT_H                 (0x0040)
#define UCA10                            (0x8000)        /* Own addressing mode select */
#define UCA10_H                          (0x0080)
#define UCA10_0                          (0x0000)        /* Own address is a 7-bit address */
#define UCA10_1                          (0x8000)        /* Own address is a 10-bit address */
#define UCA10_1_H                        (0x0080)



/* CSCTL0 Control Bits */
#define CSKEY                            (0xa500)        /* CSKEY password */
#define CSKEY_H                          (0x00a5)
#define CSKEY0                           (0x0100)        /* CSKEY password */
#define CSKEY0_H                         (0x0001)
#define CSKEY1                           (0x0200)        /* CSKEY password */
#define CSKEY1_H                         (0x0002)
#define CSKEY2                           (0x0400)        /* CSKEY password */
#define CSKEY2_H                         (0x0004)
#define CSKEY3                           (0x0800)        /* CSKEY password */
#define CSKEY3_H                         (0x0008)
#define CSKEY4                           (0x1000)        /* CSKEY password */
#define CSKEY4_H                         (0x0010)
#define CSKEY5                           (0x2000)        /* CSKEY password */
#define CSKEY5_H                         (0x0020)
#define CSKEY6                           (0x4000)        /* CSKEY password */
#define CSKEY6_H                         (0x0040)
#define CSKEY7                           (0x8000)        /* CSKEY password */
#define CSKEY7_H                         (0x0080)

/* CSCTL1 Control Bits */
#define DCOFSEL                          (0x000e)        /* DCO frequency select */
#define DCOFSEL_L                        (0x000e)
#define DCOFSEL0                         (0x0002)        /* DCO frequency select */
#define DCOFSEL0_L                       (0x0002)
#define DCOFSEL1                         (0x0004)        /* DCO frequency select */
#define DCOFSEL1_L                       (0x0004)
#define DCOFSEL2                         (0x0008)        /* DCO frequency select */
#define DCOFSEL2_L                       (0x0008)
#define DCOFSEL_0                        (0x0000)        /* If DCORSEL = 0: 1 MHz; If DCORSEL = 1: 1 MHz */
#define DCOFSEL_1                        (0x0002)        /* If DCORSEL = 0: 2.67 MHz; If DCORSEL = 1: 5.33 MHz */
#define DCOFSEL_1_L                      (0x0002)
#define DCOFSEL_2                        (0x0004)        /* If DCORSEL = 0: 3.33 MHz; If DCORSEL = 1: 6.67 MHz */
#define DCOFSEL_2_L                      (0x0004)
#define DCOFSEL_3                        (0x0006)        /* If DCORSEL = 0: 4 MHz; If DCORSEL = 1: 8 MHz */
#define DCOFSEL_3_L                      (0x0006)
#define DCOFSEL_4                        (0x0008)        /* If DCORSEL = 0: 5.33 MHz; If DCORSEL = 1: 16 MHz */
#define DCOFSEL_4_L                      (0x0008)
#define DCOFSEL_5                        (0x000a)        /* If DCORSEL = 0: 6.67 MHz; If DCORSEL = 1: 21 MHz */
#define DCOFSEL_5_L                      (0x000a)
#define DCOFSEL_6                        (0x000c)        /* If DCORSEL = 0: 8 MHz; If DCORSEL = 1: 24 MHz */
#define DCOFSEL_6_L                      (0x000c)
#define DCOFSEL_7                        (0x000e)        /* If DCORSEL = 0: Reserved. Defaults to 8. It is not recommended
                                                             to use this setting; If DCORSEL = 1: Reserved. Defaults to 24.
                                                            It is not recommended to use this setting */
#define DCOFSEL_7_L                      (0x000e)
#define DCORSEL                          (0x0040)        /* DCO range select */
#define DCORSEL_L                        (0x0040)

/* CSCTL2 Control Bits */
#define SELM                             (0x0007)        /* Selects the MCLK source */
#define SELM_L                           (0x0007)
#define SELM0                            (0x0001)        /* Selects the MCLK source */
#define SELM0_L                          (0x0001)
#define SELM1                            (0x0002)        /* Selects the MCLK source */
#define SELM1_L                          (0x0002)
#define SELM2                            (0x0004)        /* Selects the MCLK source */
#define SELM2_L                          (0x0004)
#define SELM_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK */
#define SELM_1                           (0x0001)        /* VLOCLK */
#define SELM_1_L                         (0x0001)
#define SELM_2                           (0x0002)        /* LFMODCLK */
#define SELM_2_L                         (0x0002)
#define SELM_3                           (0x0003)        /* DCOCLK */
#define SELM_3_L                         (0x0003)
#define SELM_4                           (0x0004)        /* MODCLK */
#define SELM_4_L                         (0x0004)
#define SELM_5                           (0x0005)        /* HFXTCLK when HFXT available, otherwise DCOCLK */
#define SELM_5_L                         (0x0005)
#define SELM__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK */
#define SELM__VLOCLK                     (0x0001)        /* VLOCLK */
#define SELM__VLOCLK_L                   (0x0001)
#define SELM__LFMODCLK                   (0x0002)        /* LFMODCLK */
#define SELM__LFMODCLK_L                 (0x0002)
#define SELM__DCOCLK                     (0x0003)        /* DCOCLK */
#define SELM__DCOCLK_L                   (0x0003)
#define SELM__MODCLK                     (0x0004)        /* MODCLK */
#define SELM__MODCLK_L                   (0x0004)
#define SELM__HFXTCLK                    (0x0005)        /* HFXTCLK when HFXT available, otherwise DCOCLK */
#define SELM__HFXTCLK_L                  (0x0005)
#define SELS                             (0x0070)        /* Selects the SMCLK source */
#define SELS_L                           (0x0070)
#define SELS0                            (0x0010)        /* Selects the SMCLK source */
#define SELS0_L                          (0x0010)
#define SELS1                            (0x0020)        /* Selects the SMCLK source */
#define SELS1_L                          (0x0020)
#define SELS2                            (0x0040)        /* Selects the SMCLK source */
#define SELS2_L                          (0x0040)
#define SELS_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELS_1                           (0x0010)        /* VLOCLK */
#define SELS_1_L                         (0x0010)
#define SELS_2                           (0x0020)        /* LFMODCLK */
#define SELS_2_L                         (0x0020)
#define SELS_3                           (0x0030)        /* DCOCLK */
#define SELS_3_L                         (0x0030)
#define SELS_4                           (0x0040)        /* MODCLK */
#define SELS_4_L                         (0x0040)
#define SELS_5                           (0x0050)        /* HFXTCLK when HFXT available, otherwise DCOCLK. */
#define SELS_5_L                         (0x0050)
#define SELS__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELS__VLOCLK                     (0x0010)        /* VLOCLK */
#define SELS__VLOCLK_L                   (0x0010)
#define SELS__LFMODCLK                   (0x0020)        /* LFMODCLK */
#define SELS__LFMODCLK_L                 (0x0020)
#define SELS__DCOCLK                     (0x0030)        /* DCOCLK */
#define SELS__DCOCLK_L                   (0x0030)
#define SELS__MODCLK                     (0x0040)        /* MODCLK */
#define SELS__MODCLK_L                   (0x0040)
#define SELS__HFXTCLK                    (0x0050)        /* HFXTCLK when HFXT available, otherwise DCOCLK. */
#define SELS__HFXTCLK_L                  (0x0050)
#define SELA                             (0x0700)        /* Selects the ACLK source */
#define SELA_H                           (0x0007)
#define SELA0                            (0x0100)        /* Selects the ACLK source */
#define SELA0_H                          (0x0001)
#define SELA1                            (0x0200)        /* Selects the ACLK source */
#define SELA1_H                          (0x0002)
#define SELA2                            (0x0400)        /* Selects the ACLK source */
#define SELA2_H                          (0x0004)
#define SELA_0                           (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELA_1                           (0x0100)        /* VLOCLK */
#define SELA_1_H                         (0x0001)
#define SELA_2                           (0x0200)        /* LFMODCLK */
#define SELA_2_H                         (0x0002)
#define SELA__LFXTCLK                    (0x0000)        /* LFXTCLK when LFXT available, otherwise VLOCLK. */
#define SELA__VLOCLK                     (0x0100)        /* VLOCLK */
#define SELA__VLOCLK_H                   (0x0001)
#define SELA__LFMODCLK                   (0x0200)        /* LFMODCLK */
#define SELA__LFMODCLK_H                 (0x0002)

/* CSCTL3 Control Bits */
#define DIVM                             (0x0007)        /* MCLK source divider */
#define DIVM_L                           (0x0007)
#define DIVM0                            (0x0001)        /* MCLK source divider */
#define DIVM0_L                          (0x0001)
#define DIVM1                            (0x0002)        /* MCLK source divider */
#define DIVM1_L                          (0x0002)
#define DIVM2                            (0x0004)        /* MCLK source divider */
#define DIVM2_L                          (0x0004)
#define DIVM_0                           (0x0000)        /* /1 */
#define DIVM_1                           (0x0001)        /* /2 */
#define DIVM_1_L                         (0x0001)
#define DIVM_2                           (0x0002)        /* /4 */
#define DIVM_2_L                         (0x0002)
#define DIVM_3                           (0x0003)        /* /8 */
#define DIVM_3_L                         (0x0003)
#define DIVM_4                           (0x0004)        /* /16 */
#define DIVM_4_L                         (0x0004)
#define DIVM_5                           (0x0005)        /* /32 */
#define DIVM_5_L                         (0x0005)
#define DIVM__1                          (0x0000)        /* /1 */
#define DIVM__2                          (0x0001)        /* /2 */
#define DIVM__2_L                        (0x0001)
#define DIVM__4                          (0x0002)        /* /4 */
#define DIVM__4_L                        (0x0002)
#define DIVM__8                          (0x0003)        /* /8 */
#define DIVM__8_L                        (0x0003)
#define DIVM__16                         (0x0004)        /* /16 */
#define DIVM__16_L                       (0x0004)
#define DIVM__32                         (0x0005)        /* /32 */
#define DIVM__32_L                       (0x0005)
#define DIVS                             (0x0070)        /* SMCLK source divider */
#define DIVS_L                           (0x0070)
#define DIVS0                            (0x0010)        /* SMCLK source divider */
#define DIVS0_L                          (0x0010)
#define DIVS1                            (0x0020)        /* SMCLK source divider */
#define DIVS1_L                          (0x0020)
#define DIVS2                            (0x0040)        /* SMCLK source divider */
#define DIVS2_L                          (0x0040)
#define DIVS_0                           (0x0000)        /* /1 */
#define DIVS_1                           (0x0010)        /* /2 */
#define DIVS_1_L                         (0x0010)
#define DIVS_2                           (0x0020)        /* /4 */
#define DIVS_2_L                         (0x0020)
#define DIVS_3                           (0x0030)        /* /8 */
#define DIVS_3_L                         (0x0030)
#define DIVS_4                           (0x0040)        /* /16 */
#define DIVS_4_L                         (0x0040)
#define DIVS_5                           (0x0050)        /* /32 */
#define DIVS_5_L                         (0x0050)
#define DIVS__1                          (0x0000)        /* /1 */
#define DIVS__2                          (0x0010)        /* /2 */
#define DIVS__2_L                        (0x0010)
#define DIVS__4                          (0x0020)        /* /4 */
#define DIVS__4_L                        (0x0020)
#define DIVS__8                          (0x0030)        /* /8 */
#define DIVS__8_L                        (0x0030)
#define DIVS__16                         (0x0040)        /* /16 */
#define DIVS__16_L                       (0x0040)
#define DIVS__32                         (0x0050)        /* /32 */
#define DIVS__32_L                       (0x0050)
#define DIVA                             (0x0700)        /* ACLK source divider */
#define DIVA_H                           (0x0007)
#define DIVA0                            (0x0100)        /* ACLK source divider */
#define DIVA0_H                          (0x0001)
#define DIVA1                            (0x0200)        /* ACLK source divider */
#define DIVA1_H                          (0x0002)
#define DIVA2                            (0x0400)        /* ACLK source divider */
#define DIVA2_H                          (0x0004)
#define DIVA_0                           (0x0000)        /* /1 */
#define DIVA_1                           (0x0100)        /* /2 */
#define DIVA_1_H                         (0x0001)
#define DIVA_2                           (0x0200)        /* /4 */
#define DIVA_2_H                         (0x0002)
#define DIVA_3                           (0x0300)        /* /8 */
#define DIVA_3_H                         (0x0003)
#define DIVA_4                           (0x0400)        /* /16 */
#define DIVA_4_H                         (0x0004)
#define DIVA_5                           (0x0500)        /* /32 */
#define DIVA_5_H                         (0x0005)
#define DIVA__1                          (0x0000)        /* /1 */
#define DIVA__2                          (0x0100)        /* /2 */
#define DIVA__2_H                        (0x0001)
#define DIVA__4                          (0x0200)        /* /4 */
#define DIVA__4_H                        (0x0002)
#define DIVA__8                          (0x0300)        /* /8 */
#define DIVA__8_H                        (0x0003)
#define DIVA__16                         (0x0400)        /* /16 */
#define DIVA__16_H                       (0x0004)
#define DIVA__32                         (0x0500)        /* /32 */
#define DIVA__32_H                       (0x0005)

/* CSCTL4 Control Bits */
#define LFXTOFF                          (0x0001)        /* LFXT off */
#define LFXTOFF_L                        (0x0001)
#define LFXTOFF_0                        (0x0000)        /* LFXT is on if LFXT is selected via the port selection and LFXT
                                                            is not in bypass mode of operation */
#define LFXTOFF_1                        (0x0001)        /* LFXT is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK */
#define LFXTOFF_1_L                      (0x0001)
#define SMCLKOFF                         (0x0002)        /* SMCLK off */
#define SMCLKOFF_L                       (0x0002)
#define SMCLKOFF_0                       (0x0000)        /* SMCLK on */
#define SMCLKOFF_1                       (0x0002)        /* SMCLK off */
#define SMCLKOFF_1_L                     (0x0002)
#define VLOOFF                           (0x0008)        /* VLO off */
#define VLOOFF_L                         (0x0008)
#define VLOOFF_0                         (0x0000)        /* VLO is on */
#define VLOOFF_1                         (0x0008)        /* VLO is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK or if not used as a source for the RTC in LPM3.5 */
#define VLOOFF_1_L                       (0x0008)
#define LFXTBYPASS                       (0x0010)        /* LFXT bypass select */
#define LFXTBYPASS_L                     (0x0010)
#define LFXTBYPASS_0                     (0x0000)        /* LFXT sourced from external crystal */
#define LFXTBYPASS_1                     (0x0010)        /* LFXT sourced from external clock signal */
#define LFXTBYPASS_1_L                   (0x0010)
#define LFXTDRIVE                        (0x00c0)        /* LFXT oscillator current */
#define LFXTDRIVE_L                      (0x00c0)
#define LFXTDRIVE0                       (0x0040)        /* LFXT oscillator current */
#define LFXTDRIVE0_L                     (0x0040)
#define LFXTDRIVE1                       (0x0080)        /* LFXT oscillator current */
#define LFXTDRIVE1_L                     (0x0080)
#define LFXTDRIVE_0                      (0x0000)        /* Lowest drive strength and current consumption LFXT oscillator */
#define LFXTDRIVE_1                      (0x0040)        /* Increased drive strength LFXT oscillator */
#define LFXTDRIVE_1_L                    (0x0040)
#define LFXTDRIVE_2                      (0x0080)        /* Increased drive strength LFXT oscillator */
#define LFXTDRIVE_2_L                    (0x0080)
#define LFXTDRIVE_3                      (0x00c0)        /* Maximum drive strength and maximum current consumption LFXT 
                                                            oscillator */
#define LFXTDRIVE_3_L                    (0x00c0)
#define HFXTOFF                          (0x0100)        /* Turns off the HFXT oscillator */
#define HFXTOFF_H                        (0x0001)
#define HFXTOFF_0                        (0x0000)        /* HFXT is on if HFXT is selected via the port selection and HFXT
                                                            is not in bypass mode of operation */
#define HFXTOFF_1                        (0x0100)        /* HFXT is off if it is not used as a source for ACLK, MCLK, or 
                                                            SMCLK */
#define HFXTOFF_1_H                      (0x0001)
#define HFFREQ                           (0x0c00)        /* HFXT frequency selection */
#define HFFREQ_H                         (0x000c)
#define HFFREQ0                          (0x0400)        /* HFXT frequency selection */
#define HFFREQ0_H                        (0x0004)
#define HFFREQ1                          (0x0800)        /* HFXT frequency selection */
#define HFFREQ1_H                        (0x0008)
#define HFFREQ_0                         (0x0000)        /* 0 to 4 MHz */
#define HFFREQ_1                         (0x0400)        /* Greater than 4 MHz to 8 MHz */
#define HFFREQ_1_H                       (0x0004)
#define HFFREQ_2                         (0x0800)        /* Greater than 8 MHz to 16 MHz */
#define HFFREQ_2_H                       (0x0008)
#define HFFREQ_3                         (0x0c00)        /* Greater than 16 MHz to 24 MHz */
#define HFFREQ_3_H                       (0x000c)
#define HFXTBYPASS                       (0x1000)        /* HFXT bypass select */
#define HFXTBYPASS_H                     (0x0010)
#define HFXTBYPASS_0                     (0x0000)        /* HFXT sourced from external crystal */
#define HFXTBYPASS_1                     (0x1000)        /* HFXT sourced from external clock signal */
#define HFXTBYPASS_1_H                   (0x0010)
#define HFXTDRIVE                        (0xc000)        /* HFXT oscillator current */
#define HFXTDRIVE_H                      (0x00c0)
#define HFXTDRIVE0                       (0x4000)        /* HFXT oscillator current */
#define HFXTDRIVE0_H                     (0x0040)
#define HFXTDRIVE1                       (0x8000)        /* HFXT oscillator current */
#define HFXTDRIVE1_H                     (0x0080)
#define HFXTDRIVE_0                      (0x0000)        /* Lowest current consumption */
#define HFXTDRIVE_1                      (0x4000)        /* Increased drive strength HFXT oscillator */
#define HFXTDRIVE_1_H                    (0x0040)
#define HFXTDRIVE_2                      (0x8000)        /* Increased drive strength HFXT oscillator */
#define HFXTDRIVE_2_H                    (0x0080)
#define HFXTDRIVE_3                      (0xc000)        /* Maximum drive strength HFXT oscillator */
#define HFXTDRIVE_3_H                    (0x00c0)

/* CSCTL5 Control Bits */
#define LFXTOFFG                         (0x0001)        /* LFXT oscillator fault flag */
#define LFXTOFFG_L                       (0x0001)
#define LFXTOFFG_0                       (0x0000)        /* No fault condition occurred after the last reset */
#define LFXTOFFG_1                       (0x0001)        /* LFXT fault; an LFXT fault occurred after the last reset */
#define LFXTOFFG_1_L                     (0x0001)
#define HFXTOFFG                         (0x0002)        /* HFXT oscillator fault flag */
#define HFXTOFFG_L                       (0x0002)
#define HFXTOFFG_0                       (0x0000)        /* No fault condition occurred after the last reset */
#define HFXTOFFG_1                       (0x0002)        /* HFXT fault; an HFXT fault occurred after the last reset */
#define HFXTOFFG_1_L                     (0x0002)
#define ENSTFCNT1                        (0x0040)        /* Enable start counter for LFXT */
#define ENSTFCNT1_L                      (0x0040)
#define ENSTFCNT1_0                      (0x0000)        /* Startup fault counter disabled. Counter is cleared. */
#define ENSTFCNT1_1                      (0x0040)        /* Startup fault counter enabled */
#define ENSTFCNT1_1_L                    (0x0040)
#define ENSTFCNT1__DISABLE               (0x0000)        /* Startup fault counter disabled. Counter is cleared. */
#define ENSTFCNT1__ENABLE                (0x0040)        /* Startup fault counter enabled */
#define ENSTFCNT1__ENABLE_L              (0x0040)
#define ENSTFCNT2                        (0x0080)        /* Enable start counter for HFXT */
#define ENSTFCNT2_L                      (0x0080)
#define ENSTFCNT2_0                      (0x0000)        /* Startup fault counter disabled. Counter is cleared. */
#define ENSTFCNT2_1                      (0x0080)        /* Startup fault counter enabled */
#define ENSTFCNT2_1_L                    (0x0080)
#define ENSTFCNT2__DISABLE               (0x0000)        /* Startup fault counter disabled. Counter is cleared. */
#define ENSTFCNT2__ENABLE                (0x0080)        /* Startup fault counter enabled */
#define ENSTFCNT2__ENABLE_L              (0x0080)

/* CSCTL6 Control Bits */
#define ACLKREQEN                        (0x0001)        /* ACLK clock request enable */
#define ACLKREQEN_L                      (0x0001)
#define ACLKREQEN_0                      (0x0000)        /* ACLK conditional requests are disabled */
#define ACLKREQEN_1                      (0x0001)        /* ACLK conditional requests are enabled */
#define ACLKREQEN_1_L                    (0x0001)
#define ACLKREQEN__DISABLE               (0x0000)        /* ACLK conditional requests are disabled */
#define ACLKREQEN__ENABLE                (0x0001)        /* ACLK conditional requests are enabled */
#define ACLKREQEN__ENABLE_L              (0x0001)
#define MCLKREQEN                        (0x0002)        /* MCLK clock request enable */
#define MCLKREQEN_L                      (0x0002)
#define MCLKREQEN_0                      (0x0000)        /* MCLK conditional requests are disabled */
#define MCLKREQEN_1                      (0x0002)        /* MCLK conditional requests are enabled */
#define MCLKREQEN_1_L                    (0x0002)
#define MCLKREQEN__DISABLE               (0x0000)        /* MCLK conditional requests are disabled */
#define MCLKREQEN__ENABLE                (0x0002)        /* MCLK conditional requests are enabled */
#define MCLKREQEN__ENABLE_L              (0x0002)
#define SMCLKREQEN                       (0x0004)        /* SMCLK clock request enable */
#define SMCLKREQEN_L                     (0x0004)
#define SMCLKREQEN_0                     (0x0000)        /* SMCLK conditional requests are disabled */
#define SMCLKREQEN_1                     (0x0004)        /* SMCLK conditional requests are enabled */
#define SMCLKREQEN_1_L                   (0x0004)
#define SMCLKREQEN__DISABLE              (0x0000)        /* SMCLK conditional requests are disabled */
#define SMCLKREQEN__ENABLE               (0x0004)        /* SMCLK conditional requests are enabled */
#define SMCLKREQEN__ENABLE_L             (0x0004)
#define MODCLKREQEN                      (0x0008)        /* MODCLK clock request enable */
#define MODCLKREQEN_L                    (0x0008)
#define MODCLKREQEN_0                    (0x0000)        /* MODCLK conditional requests are disabled */
#define MODCLKREQEN_1                    (0x0008)        /* MODCLK conditional requests are enabled */
#define MODCLKREQEN_1_L                  (0x0008)
#define MODCLKREQEN__DISABLE             (0x0000)        /* MODCLK conditional requests are disabled */
#define MODCLKREQEN__ENABLE              (0x0008)        /* MODCLK conditional requests are enabled */
#define MODCLKREQEN__ENABLE_L            (0x0008)

/* UCA0CTLW0_SPI Control Bits */
#define UCSTEM                           (0x0002)        /* STE mode select in master mode. */
#define UCSTEM_L                         (0x0002)
#define UCSTEM_0                         (0x0000)        /* STE pin is used to prevent conflicts with other masters */
#define UCSTEM_1                         (0x0002)        /* STE pin is used to generate the enable signal for a 4-wire 
                                                            slave */
#define UCSTEM_1_L                       (0x0002)
#define UCMST                            (0x0800)        /* Master mode select */
#define UCMST_H                          (0x0008)
#define UCMST_0                          (0x0000)        /* Slave mode */
#define UCMST_1                          (0x0800)        /* Master mode */
#define UCMST_1_H                        (0x0008)
#define UCMST__SLAVE                     (0x0000)        /* Slave mode */
#define UCMST__MASTER                    (0x0800)        /* Master mode */
#define UCMST__MASTER_H                  (0x0008)
#define UCCKPL                           (0x4000)        /* Clock polarity select */
#define UCCKPL_H                         (0x0040)
#define UCCKPL_0                         (0x0000)        /* The inactive state is low */
#define UCCKPL_1                         (0x4000)        /* The inactive state is high */
#define UCCKPL_1_H                       (0x0040)
#define UCCKPL__LOW                      (0x0000)        /* The inactive state is low */
#define UCCKPL__HIGH                     (0x4000)        /* The inactive state is high */
#define UCCKPL__HIGH_H                   (0x0040)
#define UCCKPH                           (0x8000)        /* Clock phase select */
#define UCCKPH_H                         (0x0080)
#define UCCKPH_0                         (0x0000)        /* Data is changed on the first UCLK edge and captured on the 
                                                            following edge. */
#define UCCKPH_1                         (0x8000)        /* Data is captured on the first UCLK edge and changed on the 
                                                            following edge. */
#define UCCKPH_1_H                       (0x0080)

/* UCA0CTLW1 Control Bits */
#define UCGLIT                           (0x0003)        /


#define __MSP430_BASEADDRESS_TA0__ 0x0340
#define TA0_BASE               __MSP430_BASEADDRESS_TA0__

/* TA0CTL Control Bits */
#define TAIFG                            (0x0001)        /* TimerA interrupt flag */
#define TAIFG_L                          (0x0001)
#define TAIFG_0                          (0x0000)        /* No interrupt pending */
#define TAIFG_1                          (0x0001)        /* Interrupt pending */
#define TAIFG_1_L                        (0x0001)
#define TAIE                             (0x0002)        /* TimerA interrupt enable */
#define TAIE_L                           (0x0002)
#define TAIE_0                           (0x0000)        /* Interrupt disabled */
#define TAIE_1                           (0x0002)        /* Interrupt enabled */
#define TAIE_1_L                         (0x0002)
#define TACLR                            (0x0004)        /* TimerA clear */
#define TACLR_L                          (0x0004)
#define MC                               (0x0030)        /* Mode control */
#define MC_L                             (0x0030)
#define MC0                              (0x0010)        /* Mode control */
#define MC0_L                            (0x0010)
#define MC1                              (0x0020)        /* Mode control */
#define MC1_L                            (0x0020)
#define MC_0                             (0x0000)        /* Stop mode: Timer is halted */
#define MC_1                             (0x0010)        /* Up mode: Timer counts up to TAxCCR0 */
#define MC_1_L                           (0x0010)
#define MC_2                             (0x0020)        /* Continuous mode: Timer counts up to 0FFFFh */
#define MC_2_L                           (0x0020)
#define MC_3                             (0x0030)        /* Up/down mode: Timer counts up to TAxCCR0 then down to 0000h */
#define MC_3_L                           (0x0030)
#define MC__STOP                         (0x0000)        /* Stop mode: Timer is halted */
#define MC__UP                           (0x0010)        /* Up mode: Timer counts up to TAxCCR0 */
#define MC__UP_L                         (0x0010)
#define MC__CONTINUOUS                   (0x0020)        /* Continuous mode: Timer counts up to 0FFFFh */
#define MC__CONTINUOUS_L                 (0x0020)
#define MC__UPDOWN                       (0x0030)        /* Up/down mode: Timer counts up to TAxCCR0 then down to 0000h */
#define MC__UPDOWN_L                     (0x0030)
#define ID                               (0x00c0)        /* Input divider */
#define ID_L                             (0x00c0)
#define ID0                              (0x0040)        /* Input divider */
#define ID0_L                            (0x0040)
#define ID1                              (0x0080)        /* Input divider */
#define ID1_L                            (0x0080)
#define ID_0                             (0x0000)        /* /1 */
#define ID_1                             (0x0040)        /* /2 */
#define ID_1_L                           (0x0040)
#define ID_2                             (0x0080)        /* /4 */
#define ID_2_L                           (0x0080)
#define ID_3                             (0x00c0)        /* /8 */
#define ID_3_L                           (0x00c0)
#define ID__1                            (0x0000)        /* /1 */
#define ID__2                            (0x0040)        /* /2 */
#define ID__2_L                          (0x0040)
#define ID__4                            (0x0080)        /* /4 */
#define ID__4_L                          (0x0080)
#define ID__8                            (0x00c0)        /* /8 */
#define ID__8_L                          (0x00c0)
#define TASSEL                           (0x0300)        /* TimerA clock source select */
#define TASSEL_H                         (0x0003)
#define TASSEL0                          (0x0100)        /* TimerA clock source select */
#define TASSEL0_H                        (0x0001)
#define TASSEL1                          (0x0200)        /* TimerA clock source select */
#define TASSEL1_H                        (0x0002)
#define TASSEL_0                         (0x0000)        /* TAxCLK */
#define TASSEL_1                         (0x0100)        /* ACLK */
#define TASSEL_1_H                       (0x0001)
#define TASSEL_2                         (0x0200)        /* SMCLK */
#define TASSEL_2_H                       (0x0002)
#define TASSEL_3                         (0x0300)        /* INCLK */
#define TASSEL_3_H                       (0x0003)
#define TASSEL__TACLK                    (0x0000)        /* TAxCLK */
#define TASSEL__ACLK                     (0x0100)        /* ACLK */
#define TASSEL__ACLK_H                   (0x0001)
#define TASSEL__SMCLK                    (0x0200)        /* SMCLK */
#define TASSEL__SMCLK_H                  (0x0002)
#define TASSEL__INCLK                    (0x0300)        /* INCLK */
#define TASSEL__INCLK_H                  (0x0003)

/* TA0CCTL Control Bits */
#define CCIFG                            (0x0001)        /* Capture/compare interrupt flag */
#define CCIFG_L                          (0x0001)
#define CCIFG_0                          (0x0000)        /* No interrupt pending */
#define CCIFG_1                          (0x0001)        /* Interrupt pending */
#define CCIFG_1_L                        (0x0001)
#define COV                              (0x0002)        /* Capture overflow */
#define COV_L                            (0x0002)
#define COV_0                            (0x0000)        /* No capture overflow occurred */
#define COV_1                            (0x0002)        /* Capture overflow occurred */
#define COV_1_L                          (0x0002)
#define OUT                              (0x0004)        /* Output */
#define OUT_L                            (0x0004)
#define OUT_0                            (0x0000)        /* Output low */
#define OUT_1                            (0x0004)        /* Output high */
#define OUT_1_L                          (0x0004)
#define OUT__LOW                         (0x0000)        /* Output low */
#define OUT__HIGH                        (0x0004)        /* Output high */
#define OUT__HIGH_L                      (0x0004)
#define CCI                              (0x0008)        /* Capture/compare input */
#define CCI_L                            (0x0008)
#define CCIE                             (0x0010)        /* Capture/compare interrupt enable */
#define CCIE_L                           (0x0010)
#define CCIE_0                           (0x0000)        /* Interrupt disabled */
#define CCIE_1                           (0x0010)        /* Interrupt enabled */
#define CCIE_1_L                         (0x0010)
#define OUTMOD                           (0x00e0)        /* Output mode */
#define OUTMOD_L                         (0x00e0)
#define OUTMOD0                          (0x0020)        /* Output mode */
#define OUTMOD0_L                        (0x0020)
#define OUTMOD1                          (0x0040)        /* Output mode */
#define OUTMOD1_L                        (0x0040)
#define OUTMOD2                          (0x0080)        /* Output mode */
#define OUTMOD2_L                        (0x0080)
#define OUTMOD_0                         (0x0000)        /* OUT bit value */
#define OUTMOD_1                         (0x0020)        /* Set */
#define OUTMOD_1_L                       (0x0020)
#define OUTMOD_2                         (0x0040)        /* Toggle/reset */
#define OUTMOD_2_L                       (0x0040)
#define OUTMOD_3                         (0x0060)        /* Set/reset */
#define OUTMOD_3_L                       (0x0060)
#define OUTMOD_4                         (0x0080)        /* Toggle */
#define OUTMOD_4_L                       (0x0080)
#define OUTMOD_5                         (0x00a0)        /* Reset */
#define OUTMOD_5_L                       (0x00a0)
#define OUTMOD_6                         (0x00c0)        /* Toggle/set */
#define OUTMOD_6_L                       (0x00c0)
#define OUTMOD_7                         (0x00e0)        /* Reset/set */
#define OUTMOD_7_L                       (0x00e0)
#define CAP                              (0x0100)        /* Capture mode */
#define CAP_H                            (0x0001)
#define CAP_0                            (0x0000)        /* Compare mode */
#define CAP_1                            (0x0100)        /* Capture mode */
#define CAP_1_H                          (0x0001)
#define CAP__COMPARE                     (0x0000)        /* Compare mode */
#define CAP__CAPTURE                     (0x0100)        /* Capture mode */
#define CAP__CAPTURE_H                   (0x0001)
#define SCCI                             (0x0400)        /* Synchronized capture/compare input */
#define SCCI_H                           (0x0004)
#define SCS                              (0x0800)        /* Synchronize capture source */
#define SCS_H                            (0x0008)
#define SCS_0                            (0x0000)        /* Asynchronous capture */
#define SCS_1                            (0x0800)        /* Synchronous capture */
#define SCS_1_H                          (0x0008)
#define SCS__ASYNC                       (0x0000)        /* Asynchronous capture */
#define SCS__SYNC                        (0x0800)        /* Synchronous capture */
#define SCS__SYNC_H                      (0x0008)
#define CCIS                             (0x3000)        /* Capture/compare input select */
#define CCIS_H                           (0x0030)
#define CCIS0                            (0x1000)        /* Capture/compare input select */
#define CCIS0_H                          (0x0010)
#define CCIS1                            (0x2000)        /* Capture/compare input select */
#define CCIS1_H                          (0x0020)
#define CCIS_0                           (0x0000)        /* CCIxA */
#define CCIS_1                           (0x1000)        /* CCIxB */
#define CCIS_1_H                         (0x0010)
#define CCIS_2                           (0x2000)        /* GND */
#define CCIS_2_H                         (0x0020)
#define CCIS_3                           (0x3000)        /* VCC */
#define CCIS_3_H                         (0x0030)
#define CCIS__CCIA                       (0x0000)        /* CCIxA */
#define CCIS__CCIB                       (0x1000)        /* CCIxB */
#define CCIS__CCIB_H                     (0x0010)
#define CCIS__GND                        (0x2000)        /* GND */
#define CCIS__GND_H                      (0x0020)
#define CCIS__VCC                        (0x3000)        /* VCC */
#define CCIS__VCC_H                      (0x0030)
#define CM                               (0xc000)        /* Capture mode */
#define CM_H                             (0x00c0)
#define CM0                              (0x4000)        /* Capture mode */
#define CM0_H                            (0x0040)
#define CM1                              (0x8000)        /* Capture mode */
#define CM1_H                            (0x0080)
#define CM_0                             (0x0000)        /* No capture */
#define CM_1                             (0x4000)        /* Capture on rising edge */
#define CM_1_H                           (0x0040)
#define CM_2                             (0x8000)        /* Capture on falling edge */
#define CM_2_H                           (0x0080)
#define CM_3                             (0xc000)        /* Capture on both rising and falling edges */
#define CM_3_H                           (0x00c0)
#define CM__NONE                         (0x0000)        /* No capture */
#define CM__RISING                       (0x4000)        /* Capture on rising edge */
#define CM__RISING_H                     (0x0040)
#define CM__FALLING                      (0x8000)        /* Capture on falling edge */
#define CM__FALLING_H                    (0x0080)
#define CM__BOTH                         (0xc000)        /* Capture on both rising and falling edges */
#define CM__BOTH_H                       (0x00c0)

/* TA0R Control Bits */
#define TAXR                             (0xffff)        /* Timer_A count register */
#define TAXR_L                           (0x00ff)
#define TAXR_H                           (0x00ff)
#define TAXR0                            (0x0001)        /* Timer_A count register */
#define TAXR0_L                          (0x0001)
#define TAXR1                            (0x0002)        /* Timer_A count register */
#define TAXR1_L                          (0x0002)
#define TAXR2                            (0x0004)        /* Timer_A count register */
#define TAXR2_L                          (0x0004)
#define TAXR3                            (0x0008)        /* Timer_A count register */
#define TAXR3_L                          (0x0008)
#define TAXR4                            (0x0010)        /* Timer_A count register */
#define TAXR4_L                          (0x0010)
#define TAXR5                            (0x0020)        /* Timer_A count register */
#define TAXR5_L                          (0x0020)
#define TAXR6                            (0x0040)        /* Timer_A count register */
#define TAXR6_L                          (0x0040)
#define TAXR7                            (0x0080)        /* Timer_A count register */
#define TAXR7_L                          (0x0080)
#define TAXR8                            (0x0100)        /* Timer_A count register */
#define TAXR8_H                          (0x0001)
#define TAXR9                            (0x0200)        /* Timer_A count register */
#define TAXR9_H                          (0x0002)
#define TAXR10                           (0x0400)        /* Timer_A count register */
#define TAXR10_H                         (0x0004)
#define TAXR11                           (0x0800)        /* Timer_A count register */
#define TAXR11_H                         (0x0008)
#define TAXR12                           (0x1000)        /* Timer_A count register */
#define TAXR12_H                         (0x0010)
#define TAXR13                           (0x2000)        /* Timer_A count register */
#define TAXR13_H                         (0x0020)
#define TAXR14                           (0x4000)        /* Timer_A count register */
#define TAXR14_H                         (0x0040)
#define TAXR15                           (0x8000)        /* Timer_A count register */
#define TAXR15_H                         (0x0080)

/* TA0EX0 Control Bits */
#define TAIDEX                           (0x0007)        /* Input divider expansion */
#define TAIDEX_L                         (0x0007)
#define TAIDEX0                          (0x0001)        /* Input divider expansion */
#define TAIDEX0_L                        (0x0001)
#define TAIDEX1                          (0x0002)        /* Input divider expansion */
#define TAIDEX1_L                        (0x0002)
#define TAIDEX2                          (0x0004)        /* Input divider expansion */
#define TAIDEX2_L                        (0x0004)
#define TAIDEX_0                         (0x0000)        /* Divide by 1 */
#define TAIDEX_1                         (0x0001)        /* Divide by 2 */
#define TAIDEX_1_L                       (0x0001)
#define TAIDEX_2                         (0x0002)        /* Divide by 3 */
#define TAIDEX_2_L                       (0x0002)
#define TAIDEX_3                         (0x0003)        /* Divide by 4 */
#define TAIDEX_3_L                       (0x0003)
#define TAIDEX_4                         (0x0004)        /* Divide by 5 */
#define TAIDEX_4_L                       (0x0004)
#define TAIDEX_5                         (0x0005)        /* Divide by 6 */
#define TAIDEX_5_L                       (0x0005)
#define TAIDEX_6                         (0x0006)        /* Divide by 7 */
#define TAIDEX_6_L                       (0x0006)
#define TAIDEX_7                         (0x0007)        /* Divide by 8 */
#define TAIDEX_7_L                       (0x0007)
#define TAIDEX__1                        (0x0000)        /* Divide by 1 */
#define TAIDEX__2                        (0x0001)        /* Divide by 2 */
#define TAIDEX__2_L                      (0x0001)
#define TAIDEX__3                        (0x0002)        /* Divide by 3 */
#define TAIDEX__3_L                      (0x0002)
#define TAIDEX__4                        (0x0003)        /* Divide by 4 */
#define TAIDEX__4_L                      (0x0003)
#define TAIDEX__5                        (0x0004)        /* Divide by 5 */
#define TAIDEX__5_L                      (0x0004)
#define TAIDEX__6                        (0x0005)        /* Divide by 6 */
#define TAIDEX__6_L                      (0x0005)
#define TAIDEX__7                        (0x0006)        /* Divide by 7 */
#define TAIDEX__7_L                      (0x0006)
#define TAIDEX__8                        (0x0007)        /* Divide by 8 */
#define TAIDEX__8_L                      (0x0007)

/* TA0IV Control Bits */
#define TAIV                             (0xffff)        /* TimerA interrupt vector value */
#define TAIV_L                           (0x00ff)
#define TAIV_H                           (0x00ff)
#define TAIV0                            (0x0001)        /* TimerA interrupt vector value */
#define TAIV0_L                          (0x0001)
#define TAIV1                            (0x0002)        /* TimerA interrupt vector value */
#define TAIV1_L                          (0x0002)
#define TAIV2                            (0x0004)        /* TimerA interrupt vector value */
#define TAIV2_L                          (0x0004)
#define TAIV3                            (0x0008)        /* TimerA interrupt vector value */
#define TAIV3_L                          (0x0008)
#define TAIV4                            (0x0010)        /* TimerA interrupt vector value */
#define TAIV4_L                          (0x0010)
#define TAIV5                            (0x0020)        /* TimerA interrupt vector value */
#define TAIV5_L                          (0x0020)
#define TAIV6                            (0x0040)        /* TimerA interrupt vector value */
#define TAIV6_L                          (0x0040)
#define TAIV7                            (0x0080)        /* TimerA interrupt vector value */
#define TAIV7_L                          (0x0080)
#define TAIV8                            (0x0100)        /* TimerA interrupt vector value */
#define TAIV8_H                          (0x0001)
#define TAIV9                            (0x0200)        /* TimerA interrupt vector value */
#define TAIV9_H                          (0x0002)
#define TAIV10                           (0x0400)        /* TimerA interrupt vector value */
#define TAIV10_H                         (0x0004)
#define TAIV11                           (0x0800)        /* TimerA interrupt vector value */
#define TAIV11_H                         (0x0008)
#define TAIV12                           (0x1000)        /* TimerA interrupt vector value */
#define TAIV12_H                         (0x0010)
#define TAIV13                           (0x2000)        /* TimerA interrupt vector value */
#define TAIV13_H                         (0x0020)
#define TAIV14                           (0x4000)        /* TimerA interrupt vector value */
#define TAIV14_H                         (0x0040)
#define TAIV15                           (0x8000)        /* TimerA interrupt vector value */
#define TAIV15_H                         (0x0080)
#define TAIV_0                           (0x0000)        /* No interrupt pending */
#define TAIV_2                           (0x0002)        /* Interrupt Source: Capture/compare 1; Interrupt Flag: TAxCCR1 
                                                            CCIFG; Interrupt Priority: Highest */
#define TAIV_2_L                         (0x0002)
#define TAIV_4                           (0x0004)        /* Interrupt Source: Capture/compare 2; Interrupt Flag: TAxCCR2 
                                                            CCIFG */
#define TAIV_4_L                         (0x0004)
#define TAIV_6                           (0x0006)        /* Interrupt Source: Capture/compare 3; Interrupt Flag: TAxCCR3 
                                                            CCIFG */
#define TAIV_6_L                         (0x0006)
#define TAIV_8                           (0x0008)        /* Interrupt Source: Capture/compare 4; Interrupt Flag: TAxCCR4 
                                                            CCIFG */
#define TAIV_8_L                         (0x0008)
#define TAIV_10                          (0x000a)        /* Interrupt Source: Capture/compare 5; Interrupt Flag: TAxCCR5 
                                                            CCIFG */
#define TAIV_10_L                        (0x000a)
#define TAIV_12                          (0x000c)        /* Interrupt Source: Capture/compare 6; Interrupt Flag: TAxCCR6 
                                                            CCIFG */
#define TAIV_12_L                        (0x000c)
#define TAIV_14                          (0x000e)        /* Interrupt Source: Timer overflow; Interrupt Flag: TAxCTL TAIFG;
                                                            Interrupt Priority: Lowest */
#define TAIV_14_L                        (0x000e)
#define TAIV__NONE                       (0x0000)        /* No interrupt pending */
#define TAIV__TACCR1                     (0x0002)        /* Interrupt Source: Capture/compare 1; Interrupt Flag: TAxCCR1 
                                                            CCIFG; Interrupt Priority: Highest */
#define TAIV__TACCR1_L                   (0x0002)
#define TAIV__TACCR2                     (0x0004)        /* Interrupt Source: Capture/compare 2; Interrupt Flag: TAxCCR2 
                                                            CCIFG */
#define TAIV__TACCR2_L                   (0x0004)
#define TAIV__TACCR3                     (0x0006)        /* Interrupt Source: Capture/compare 3; Interrupt Flag: TAxCCR3 
                                                            CCIFG */
#define TAIV__TACCR3_L                   (0x0006)
#define TAIV__TACCR4                     (0x0008)        /* Interrupt Source: Capture/compare 4; Interrupt Flag: TAxCCR4 
                                                            CCIFG */
#define TAIV__TACCR4_L                   (0x0008)
#define TAIV__TACCR5                     (0x000a)        /* Interrupt Source: Capture/compare 5; Interrupt Flag: TAxCCR5 
                                                            CCIFG */
#define TAIV__TACCR5_L                   (0x000a)
#define TAIV__TACCR6                     (0x000c)        /* Interrupt Source: Capture/compare 6; Interrupt Flag: TAxCCR6 
                                                            CCIFG */
#define TAIV__TACCR6_L                   (0x000c)
#define TAIV__TAIFG                      (0x000e)        /* Interrupt Source: Timer overflow; Interrupt Flag: TAxCTL TAIFG;
                                                            Interrupt Priority: Lowest */
#define TAIV__TAIFG_L                    (0x000e)

#define __MSP430_BASEADDRESS_TB0__ 0x03C0
#define TB0_BASE               __MSP430_BASEADDRESS_TB0__


/* TB0CTL Control Bits */
#define TBIFG                            (0x0001)        /* TimerB interrupt flag */
#define TBIFG_L                          (0x0001)
#define TBIFG_0                          (0x0000)        /* No interrupt pending */
#define TBIFG_1                          (0x0001)        /* Interrupt pending */
#define TBIFG_1_L                        (0x0001)
#define TBIE                             (0x0002)        /* TimerB interrupt enable */
#define TBIE_L                           (0x0002)
#define TBIE_0                           (0x0000)        /* Interrupt disabled */
#define TBIE_1                           (0x0002)        /* Interrupt enabled */
#define TBIE_1_L                         (0x0002)
#define TBCLR                            (0x0004)        /* TimerB clear */
#define TBCLR_L                          (0x0004)
#define TBSSEL                           (0x0300)        /* TimerB clock source select */
#define TBSSEL_H                         (0x0003)
#define TBSSEL0                          (0x0100)        /* TimerB clock source select */
#define TBSSEL0_H                        (0x0001)
#define TBSSEL1                          (0x0200)        /* TimerB clock source select */
#define TBSSEL1_H                        (0x0002)
#define TBSSEL_0                         (0x0000)        /* TBxCLK */
#define TBSSEL_1                         (0x0100)        /* ACLK */
#define TBSSEL_1_H                       (0x0001)
#define TBSSEL_2                         (0x0200)        /* SMCLK */
#define TBSSEL_2_H                       (0x0002)
#define TBSSEL_3                         (0x0300)        /* INCLK */
#define TBSSEL_3_H                       (0x0003)
#define TBSSEL__TBCLK                    (0x0000)        /* TBxCLK */
#define TBSSEL__ACLK                     (0x0100)        /* ACLK */
#define TBSSEL__ACLK_H                   (0x0001)
#define TBSSEL__SMCLK                    (0x0200)        /* SMCLK */
#define TBSSEL__SMCLK_H                  (0x0002)
#define TBSSEL__INCLK                    (0x0300)        /* INCLK */
#define TBSSEL__INCLK_H                  (0x0003)
#define CNTL                             (0x1800)        /* Counter length */
#define CNTL_H                           (0x0018)
#define CNTL0                            (0x0800)        /* Counter length */
#define CNTL0_H                          (0x0008)
#define CNTL1                            (0x1000)        /* Counter length */
#define CNTL1_H                          (0x0010)
#define CNTL_0                           (0x0000)        /* 16-bit, TBxR(max) = 0FFFFh */
#define CNTL_1                           (0x0800)        /* 12-bit, TBxR(max) = 0FFFh */
#define CNTL_1_H                         (0x0008)
#define CNTL_2                           (0x1000)        /* 10-bit, TBxR(max) = 03FFh */
#define CNTL_2_H                         (0x0010)
#define CNTL_3                           (0x1800)        /* 8-bit, TBxR(max) = 0FFh */
#define CNTL_3_H                         (0x0018)
#define CNTL__16                         (0x0000)        /* 16-bit, TBxR(max) = 0FFFFh */
#define CNTL__12                         (0x0800)        /* 12-bit, TBxR(max) = 0FFFh */
#define CNTL__12_H                       (0x0008)
#define CNTL__10                         (0x1000)        /* 10-bit, TBxR(max) = 03FFh */
#define CNTL__10_H                       (0x0010)
#define CNTL__8                          (0x1800)        /* 8-bit, TBxR(max) = 0FFh */
#define CNTL__8_H                        (0x0018)
#define TBCLGRP                          (0x6000)        /* TBxCLn group */
#define TBCLGRP_H                        (0x0060)
#define TBCLGRP0                         (0x2000)        /* TBxCLn group */
#define TBCLGRP0_H                       (0x0020)
#define TBCLGRP1                         (0x4000)        /* TBxCLn group */
#define TBCLGRP1_H                       (0x0040)
#define TBCLGRP_0                        (0x0000)        /* Each TBxCLn latch loads independently */
#define TBCLGRP_1                        (0x2000)        /* TBxCL1+TBxCL2 (TBxCCR1 CLLD bits control the update); TBxCL3+
                                                            TBxCL4 (TBxCCR3 CLLD bits control the update); TBxCL5+TBxCL6 (
                                                            TBxCCR5 CLLD bits control the update); TBxCL0 independent */
#define TBCLGRP_1_H                      (0x0020)
#define TBCLGRP_2                        (0x4000)        /* TBxCL1+TBxCL2+TBxCL3 (TBxCCR1 CLLD bits control the update); 
                                                            TBxCL4+TBxCL5+TBxCL6 (TBxCCR4 CLLD bits control the update); 
                                                            TBxCL0 independent */
#define TBCLGRP_2_H                      (0x0040)
#define TBCLGRP_3                        (0x6000)        /* TBxCL0+TBxCL1+TBxCL2+TBxCL3+TBxCL4+TBxCL5+TBxCL6 (TBxCCR1 CLLD
                                                            bits control the update) */
#define TBCLGRP_3_H                      (0x0060)

/* TB0CCTL Control Bits */
#define CLLD                             (0x0600)        /* Compare latch load */
#define CLLD_H                           (0x0006)
#define CLLD0                            (0x0200)        /* Compare latch load */
#define CLLD0_H                          (0x0002)
#define CLLD1                            (0x0400)        /* Compare latch load */
#define CLLD1_H                          (0x0004)
#define CLLD_0                           (0x0000)        /* TBxCLn loads on write to TBxCCRn */
#define CLLD_1                           (0x0200)        /* TBxCLn loads when TBxR counts to 0 */
#define CLLD_1_H                         (0x0002)
#define CLLD_2                           (0x0400)        /* TBxCLn loads when TBxR counts to 0 (up or continuous mode). 
                                                            TBxCLn loads when TBxR counts to TBxCL0 or to 0 (up/down mode).
 */
#define CLLD_2_H                         (0x0004)
#define CLLD_3                           (0x0600)        /* TBxCLn loads when TBxR counts to TBxCLn */
#define CLLD_3_H                         (0x0006)

/* TB0R Control Bits */
#define TBXR                             (0xffff)        /* Timer_B count register */
#define TBXR_L                           (0x00ff)
#define TBXR_H                           (0x00ff)
#define TBXR0                            (0x0001)        /* Timer_B count register */
#define TBXR0_L                          (0x0001)
#define TBXR1                            (0x0002)        /* Timer_B count register */
#define TBXR1_L                          (0x0002)
#define TBXR2                            (0x0004)        /* Timer_B count register */
#define TBXR2_L                          (0x0004)
#define TBXR3                            (0x0008)        /* Timer_B count register */
#define TBXR3_L                          (0x0008)
#define TBXR4                            (0x0010)        /* Timer_B count register */
#define TBXR4_L                          (0x0010)
#define TBXR5                            (0x0020)        /* Timer_B count register */
#define TBXR5_L                          (0x0020)
#define TBXR6                            (0x0040)        /* Timer_B count register */
#define TBXR6_L                          (0x0040)
#define TBXR7                            (0x0080)        /* Timer_B count register */
#define TBXR7_L                          (0x0080)
#define TBXR8                            (0x0100)        /* Timer_B count register */
#define TBXR8_H                          (0x0001)
#define TBXR9                            (0x0200)        /* Timer_B count register */
#define TBXR9_H                          (0x0002)
#define TBXR10                           (0x0400)        /* Timer_B count register */
#define TBXR10_H                         (0x0004)
#define TBXR11                           (0x0800)        /* Timer_B count register */
#define TBXR11_H                         (0x0008)
#define TBXR12                           (0x1000)        /* Timer_B count register */
#define TBXR12_H                         (0x0010)
#define TBXR13                           (0x2000)        /* Timer_B count register */
#define TBXR13_H                         (0x0020)
#define TBXR14                           (0x4000)        /* Timer_B count register */
#define TBXR14_H                         (0x0040)
#define TBXR15                           (0x8000)        /* Timer_B count register */
#define TBXR15_H                         (0x0080)

/* TB0CCR Control Bits */
#define TBXCCRN                          (0xffff)        /* Timer_B capture/compare register. */
#define TBXCCRN_L                        (0x00ff)
#define TBXCCRN_H                        (0x00ff)
#define TBXCCRN0                         (0x0001)        /* Timer_B capture/compare register. */
#define TBXCCRN0_L                       (0x0001)
#define TBXCCRN1                         (0x0002)        /* Timer_B capture/compare register. */
#define TBXCCRN1_L                       (0x0002)
#define TBXCCRN2                         (0x0004)        /* Timer_B capture/compare register. */
#define TBXCCRN2_L                       (0x0004)
#define TBXCCRN3                         (0x0008)        /* Timer_B capture/compare register. */
#define TBXCCRN3_L                       (0x0008)
#define TBXCCRN4                         (0x0010)        /* Timer_B capture/compare register. */
#define TBXCCRN4_L                       (0x0010)
#define TBXCCRN5                         (0x0020)        /* Timer_B capture/compare register. */
#define TBXCCRN5_L                       (0x0020)
#define TBXCCRN6                         (0x0040)        /* Timer_B capture/compare register. */
#define TBXCCRN6_L                       (0x0040)
#define TBXCCRN7                         (0x0080)        /* Timer_B capture/compare register. */
#define TBXCCRN7_L                       (0x0080)
#define TBXCCRN8                         (0x0100)        /* Timer_B capture/compare register. */
#define TBXCCRN8_H                       (0x0001)
#define TBXCCRN9                         (0x0200)        /* Timer_B capture/compare register. */
#define TBXCCRN9_H                       (0x0002)
#define TBXCCRN10                        (0x0400)        /* Timer_B capture/compare register. */
#define TBXCCRN10_H                      (0x0004)
#define TBXCCRN11                        (0x0800)        /* Timer_B capture/compare register. */
#define TBXCCRN11_H                      (0x0008)
#define TBXCCRN12                        (0x1000)        /* Timer_B capture/compare register. */
#define TBXCCRN12_H                      (0x0010)
#define TBXCCRN13                        (0x2000)        /* Timer_B capture/compare register. */
#define TBXCCRN13_H                      (0x0020)
#define TBXCCRN14                        (0x4000)        /* Timer_B capture/compare register. */
#define TBXCCRN14_H                      (0x0040)
#define TBXCCRN15                        (0x8000)        /* Timer_B capture/compare register. */
#define TBXCCRN15_H                      (0x0080)

/* TB0EX0 Control Bits */
#define TBIDEX                           (0x0007)        /* Input divider expansion */
#define TBIDEX_L                         (0x0007)
#define TBIDEX0                          (0x0001)        /* Input divider expansion */
#define TBIDEX0_L                        (0x0001)
#define TBIDEX1                          (0x0002)        /* Input divider expansion */
#define TBIDEX1_L                        (0x0002)
#define TBIDEX2                          (0x0004)        /* Input divider expansion */
#define TBIDEX2_L                        (0x0004)
#define TBIDEX_0                         (0x0000)        /* Divide by 1 */
#define TBIDEX_1                         (0x0001)        /* Divide by 2 */
#define TBIDEX_1_L                       (0x0001)
#define TBIDEX_2                         (0x0002)        /* Divide by 3 */
#define TBIDEX_2_L                       (0x0002)
#define TBIDEX_3                         (0x0003)        /* Divide by 4 */
#define TBIDEX_3_L                       (0x0003)
#define TBIDEX_4                         (0x0004)        /* Divide by 5 */
#define TBIDEX_4_L                       (0x0004)
#define TBIDEX_5                         (0x0005)        /* Divide by 6 */
#define TBIDEX_5_L                       (0x0005)
#define TBIDEX_6                         (0x0006)        /* Divide by 7 */
#define TBIDEX_6_L                       (0x0006)
#define TBIDEX_7                         (0x0007)        /* Divide by 8 */
#define TBIDEX_7_L                       (0x0007)
#define TBIDEX__1                        (0x0000)        /* Divide by 1 */
#define TBIDEX__2                        (0x0001)        /* Divide by 2 */
#define TBIDEX__2_L                      (0x0001)
#define TBIDEX__3                        (0x0002)        /* Divide by 3 */
#define TBIDEX__3_L                      (0x0002)
#define TBIDEX__4                        (0x0003)        /* Divide by 4 */
#define TBIDEX__4_L                      (0x0003)
#define TBIDEX__5                        (0x0004)        /* Divide by 5 */
#define TBIDEX__5_L                      (0x0004)
#define TBIDEX__6                        (0x0005)        /* Divide by 6 */
#define TBIDEX__6_L                      (0x0005)
#define TBIDEX__7                        (0x0006)        /* Divide by 7 */
#define TBIDEX__7_L                      (0x0006)
#define TBIDEX__8                        (0x0007)        /* Divide by 8 */
#define TBIDEX__8_L                      (0x0007)

/* TB0IV Control Bits */
#define TBIV                             (0xffff)        /* Timer_B interrupt vector value */
#define TBIV_L                           (0x00ff)
#define TBIV_H                           (0x00ff)
#define TBIV0                            (0x0001)        /* Timer_B interrupt vector value */
#define TBIV0_L                          (0x0001)
#define TBIV1                            (0x0002)        /* Timer_B interrupt vector value */
#define TBIV1_L                          (0x0002)
#define TBIV2                            (0x0004)        /* Timer_B interrupt vector value */
#define TBIV2_L                          (0x0004)
#define TBIV3                            (0x0008)        /* Timer_B interrupt vector value */
#define TBIV3_L                          (0x0008)
#define TBIV4                            (0x0010)        /* Timer_B interrupt vector value */
#define TBIV4_L                          (0x0010)
#define TBIV5                            (0x0020)        /* Timer_B interrupt vector value */
#define TBIV5_L                          (0x0020)
#define TBIV6                            (0x0040)        /* Timer_B interrupt vector value */
#define TBIV6_L                          (0x0040)
#define TBIV7                            (0x0080)        /* Timer_B interrupt vector value */
#define TBIV7_L                          (0x0080)
#define TBIV8                            (0x0100)        /* Timer_B interrupt vector value */
#define TBIV8_H                          (0x0001)
#define TBIV9                            (0x0200)        /* Timer_B interrupt vector value */
#define TBIV9_H                          (0x0002)
#define TBIV10                           (0x0400)        /* Timer_B interrupt vector value */
#define TBIV10_H                         (0x0004)
#define TBIV11                           (0x0800)        /* Timer_B interrupt vector value */
#define TBIV11_H                         (0x0008)
#define TBIV12                           (0x1000)        /* Timer_B interrupt vector value */
#define TBIV12_H                         (0x0010)
#define TBIV13                           (0x2000)        /* Timer_B interrupt vector value */
#define TBIV13_H                         (0x0020)
#define TBIV14                           (0x4000)        /* Timer_B interrupt vector value */
#define TBIV14_H                         (0x0040)
#define TBIV15                           (0x8000)        /* Timer_B interrupt vector value */
#define TBIV15_H                         (0x0080)
#define TBIV_0                           (0x0000)        /* No interrupt pending */
#define TBIV_2                           (0x0002)        /* Interrupt Source: Capture/compare 1; Interrupt Flag: TBxCCR1 
                                                            CCIFG; Interrupt Priority: Highest */
#define TBIV_2_L                         (0x0002)
#define TBIV_4                           (0x0004)        /* Interrupt Source: Capture/compare 2; Interrupt Flag: TBxCCR2 
                                                            CCIFG */
#define TBIV_4_L                         (0x0004)
#define TBIV_6                           (0x0006)        /* Interrupt Source: Capture/compare 3; Interrupt Flag: TBxCCR3 
                                                            CCIFG */
#define TBIV_6_L                         (0x0006)
#define TBIV_8                           (0x0008)        /* Interrupt Source: Capture/compare 4; Interrupt Flag: TBxCCR4 
                                                            CCIFG */
#define TBIV_8_L                         (0x0008)
#define TBIV_10                          (0x000a)        /* Interrupt Source: Capture/compare 5; Interrupt Flag: TBxCCR5 
                                                            CCIFG */
#define TBIV_10_L                        (0x000a)
#define TBIV_12                          (0x000c)        /* Interrupt Source: Capture/compare 6; Interrupt Flag: TBxCCR6 
                                                            CCIFG */
#define TBIV_12_L                        (0x000c)
#define TBIV_14                          (0x000e)        /* Interrupt Source: Timer overflow; Interrupt Flag: TBxCTL TBIFG;
                                                            Interrupt Priority: Lowest */
#define TBIV_14_L                        (0x000e)
#define TBIV__NONE                       (0x0000)        /* No interrupt pending */
#define TBIV__TBCCR1                     (0x0002)        /* Interrupt Source: Capture/compare 1; Interrupt Flag: TBxCCR1 
                                                            CCIFG; Interrupt Priority: Highest */
#define TBIV__TBCCR1_L                   (0x0002)
#define TBIV__TBCCR2                     (0x0004)        /* Interrupt Source: Capture/compare 2; Interrupt Flag: TBxCCR2 
                                                            CCIFG */
#define TBIV__TBCCR2_L                   (0x0004)
#define TBIV__TBCCR3                     (0x0006)        /* Interrupt Source: Capture/compare 3; Interrupt Flag: TBxCCR3 
                                                            CCIFG */
#define TBIV__TBCCR3_L                   (0x0006)
#define TBIV__TBCCR4                     (0x0008)        /* Interrupt Source: Capture/compare 4; Interrupt Flag: TBxCCR4 
                                                            CCIFG */
#define TBIV__TBCCR4_L                   (0x0008)
#define TBIV__TBCCR5                     (0x000a)        /* Interrupt Source: Capture/compare 5; Interrupt Flag: TBxCCR5 
                                                            CCIFG */
#define TBIV__TBCCR5_L                   (0x000a)
#define TBIV__TBCCR6                     (0x000c)        /* Interrupt Source: Capture/compare 6; Interrupt Flag: TBxCCR6 
                                                            CCIFG */
#define TBIV__TBCCR6_L                   (0x000c)
#define TBIV__TBIFG                      (0x000e)        /* Interrupt Source: Timer overflow; Interrupt Flag: TBxCTL TBIFG;
                                                            Interrupt Priority: Lowest */
#define TBIV__TBIFG_L                    (0x000e)

/** UC Ports */
extern uint16_t UCA0BRW;
extern uint16_t UCA0CTLW0;
extern uint16_t UCA0CTLW1;
extern uint16_t UCA0MCTLW;
extern uint16_t UCA0IE;
extern uint16_t UCA0STATW;
extern uint16_t UCA0TXBUF;
extern uint16_t UCA0RXBUF;

extern uint16_t UCA1BRW;
extern uint16_t UCA1CTLW0;
extern uint16_t UCA1CTLW1;
extern uint16_t UCA1MCTLW;
extern uint16_t UCA1IE;
extern uint16_t UCA1STATW;
extern uint16_t UCA1TXBUF;
extern uint16_t UCA1RXBUF;

extern uint16_t UCA2BRW;
extern uint16_t UCA2CTLW0;
extern uint16_t UCA2CTLW1;
extern uint16_t UCA2MCTLW;
extern uint16_t UCA2IE;
extern uint16_t UCA2STATW;
extern uint16_t UCA2TXBUF;
extern uint16_t UCA2RXBUF;

extern uint16_t UCA3BRW;
extern uint16_t UCA3CTLW0;
extern uint16_t UCA3CTLW1;
extern uint16_t UCA3MCTLW;
extern uint16_t UCA3IE;
extern uint16_t UCA3STATW;
extern uint16_t UCA3TXBUF;
extern uint16_t UCA3RXBUF;



/** GPIO Ports and Settings */
extern uint16_t P1OUT;
extern uint16_t P2OUT;
extern uint16_t P3OUT;
extern uint16_t P4OUT;
extern uint16_t P5OUT;
extern uint16_t P6OUT;
extern uint16_t P7OUT;
extern uint16_t P8OUT;

extern uint16_t P1IN;
extern uint16_t P2IN;
extern uint16_t P3IN;
extern uint16_t P4IN;
extern uint16_t P5IN;
extern uint16_t P6IN;
extern uint16_t P7IN;
extern uint16_t P8IN;

extern uint16_t P1DIR;
extern uint16_t P2DIR;
extern uint16_t P3DIR;
extern uint16_t P4DIR;
extern uint16_t P5DIR;
extern uint16_t P6DIR;
extern uint16_t P7DIR;
extern uint16_t P8DIR;

extern uint16_t P1SEL0;
extern uint16_t P2SEL0;
extern uint16_t P3SEL0;
extern uint16_t P4SEL0;
extern uint16_t P5SEL0;
extern uint16_t P6SEL0;
extern uint16_t P7SEL0;
extern uint16_t P8SEL0;

extern uint16_t P1SEL1;
extern uint16_t P2SEL1;
extern uint16_t P3SEL1;
extern uint16_t P4SEL1;
extern uint16_t P5SEL1;
extern uint16_t P6SEL1;
extern uint16_t P7SEL1;
extern uint16_t P8SEL1;

extern uint16_t P1REN;
extern uint16_t P2REN;
extern uint16_t P3REN;
extern uint16_t P4REN;
extern uint16_t P5REN;
extern uint16_t P6REN;
extern uint16_t P7REN;
extern uint16_t P8REN;

extern uint16_t P1IFG;
extern uint16_t P2IFG;
extern uint16_t P3IFG;
extern uint16_t P4IFG;
extern uint16_t P5IFG;
extern uint16_t P6IFG;
extern uint16_t P7IFG;
extern uint16_t P8IFG;

extern uint16_t P1IES;
extern uint16_t P2IES;
extern uint16_t P3IES;
extern uint16_t P4IES;
extern uint16_t P5IES;
extern uint16_t P6IES;
extern uint16_t P7IES;
extern uint16_t P8IES;

extern uint16_t P1IE;
extern uint16_t P2IE;
extern uint16_t P3IE;
extern uint16_t P4IE;
extern uint16_t P5IE;
extern uint16_t P6IE;
extern uint16_t P7IE;
extern uint16_t P8IE;

#endif