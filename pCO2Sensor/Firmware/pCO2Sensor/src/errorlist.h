/** @file logging.h
 *  @brief Error Handling Library
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 9, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note Error handling library
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _ERRORSLIST_H
#define _ERRORSLIST_H

/**
 * Debug and Error messages are of the following format:
 *
 * XXXXX Message
 *
 * where XXXX is a four digit identifier.
 *
 *  Example Debug Message
 *  #define DEBUG0000   ("0000 Generic Debug Message")
 *
 *  Example Error Message
 *  #define ERROR0100  ("0100 Failure to Update")
 */
#define DEBUG_ID_LENGTH (4)
#define ERROR_ID_LENGTH (8)

/******************************* Debug Messages ******************************/

#define DEBUG_MSG_1 ("0001 ")

/******************************* Error Messages ******************************/
/** E0000 - E0049: System Errors */
#define ERROR_MSG_01010001 ("01010001 Startup Error")

#define ERROR_MSG_01010002 ("01010002 SCHEDULER Start Time in Past")
#define ERROR_MSG_01010004 ("01010004 SCHEDULER Interval Longer Than Run")

#define ERROR_MSG_01010008 ("01010008 CONSOLE Init Failure")
#define ERROR_MSG_01010010 ("01010010 COMMAND Init Failure")
#define ERROR_MSG_01010011 ("01010011 Invalid Command")
#define ERROR_MSG_01010012 ("01010012 Invalid Hotkey")
#define ERROR_MSG_01010020 ("01010020 BSP GPIO Error")
#define ERROR_MSG_01010021 ("01010021 BSP UART Baud Reg Error")
#define ERROR_MSG_01010022 ("01010022 BSP UART EUSCI A Init Error")
#define ERROR_MSG_01010023 ("01010023 BSP UART Set Buad Error")
#define ERROR_MSG_01010030 ("01010030 BSP I2C Send Error")
#define ERROR_MSG_01010100 ("01010100 BSP SYSCLK Init Failure")
#define ERROR_MSG_01010040 ("01010040 MAX310x Init Error")
#define ERROR_MSG_01010041 ("01010041 MAX310x Wakeup Error")

/** E0050 - E0099: Bus Errors */

/** PCO2 General Errors */
#define ERROR_MSG_00010001 ("00010001 PCO2 Licor Init Fail")
#define ERROR_MSG_00010002 ("00010002 PCO2 Flow Init Fail")
#define ERROR_MSG_00010004 ("00010004 PCO2 RH Init Fail")
#define ERROR_MSG_00010008 ("00010008 PCO2 DL Init Fail")

#define ERROR_MSG_00010010 ("00010010 PCO2 Config Fail")
#define ERROR_MSG_00010020 ("00010020 PCO2 Zero Fail")
#define ERROR_MSG_00010040 ("00010040 PCO2 Span Fail")
#define ERROR_MSG_00010080 ("00010080 PCO2 Span2 Fail")
#define ERROR_MSG_00010100 ("00010100 PCO2 Equil Fail")
#define ERROR_MSG_00010200 ("00010200 PCO2 Air Fail")
#define ERROR_MSG_00010400 ("00010400 PCO2 Rest Fail")
#define ERROR_MSG_00010800 ("00010800 PCO2 Deploy Fail")
#define ERROR_MSG_00010A00 ("00010A00 PCO2 Purge Fail")
#define ERROR_MSG_00010A01 ("00010A01 PCO2 PRG1 Fail")
#define ERROR_MSG_00010A02 ("00010A01 PCO2 PRG2 Fail")
#define ERROR_MSG_00010A03 ("00010A01 PCO2 PRG3 Fail")
#define ERROR_MSG_00010A04 ("00010A01 PCO2 PRG4 Fail")
#define ERROR_MSG_00010A05 ("00010A01 PCO2 PRG5 Fail")
#define ERROR_MSG_00010A06 ("00010A01 PCO2 PRG6 Fail")
#define ERROR_MSG_00010A07 ("00010A01 PCO2 PRG7 Fail")
#define ERROR_MSG_00010A08 ("00010A01 PCO2 PRG8 Fail")

#define ERROR_MSG_00011000 ("00011000 PCO2 Flow REST Fail")
#define ERROR_MSG_00012000 ("00012000 PCO2 Flow DPLY Fail")
#define ERROR_MSG_00014000 ("00014000 PCO2 Invalid Mode")

#define ERROR_MSG_0130 ("0130 PCO2 Invalid Cal State")

/** PCO2 Zero Errors */
#define ERROR_MSG_00020001 ("00020001 PCO2 Licor Zero Failed")
#define ERROR_MSG_00020002 ("00020002 PCO2 Zero Flow ZERO_ON Fail")
#define ERROR_MSG_00020004 ("00020004 PCO2 Zero SAMPLE 1 Fail")
#define ERROR_MSG_00020008 ("00020008 PCO2 Zero Flow ZERO_OFF Fail")
#define ERROR_MSG_00020010 ("00020010 PCO2 Zero Flow VENT Fail")
#define ERROR_MSG_00020020 ("00020020 PCO2 Zero SAMPLE 2 Fail")
#define ERROR_MSG_00020040 ("00020040 PCO2 Zero CAL Fail")
#define ERROR_MSG_00020080 ("00020080 PCO2 Zero Flow POSTCAL Fail")
#define ERROR_MSG_00020100 ("00020100 PCO2 Zero SAMPLE 3 Fail")

/** PCO2 Span Errors */
#define ERROR_MSG_00040001 ("00040001 PCO2 Licor Span Failed")
#define ERROR_MSG_00040002 ("00040002 PCO2 Span Flow SPAN_ON Fail")
#define ERROR_MSG_00040004 ("00040004 PCO2 Span SAMPLE 1 Fail")
#define ERROR_MSG_00040008 ("00040008 PCO2 Span Flow SPAN_OFF Fail")
#define ERROR_MSG_00040010 ("00040010 PCO2 Span Flow VENT Fail")
#define ERROR_MSG_00040020 ("00040020 PCO2 Span SAMPLE 2 Fail")
#define ERROR_MSG_00040080 ("00040080 PCO2 Span CAL Fail")
#define ERROR_MSG_00040100 ("00040100 PCO2 Span Flow POSTCAL Fail")
#define ERROR_MSG_00040200 ("00040200 PCO2 Span SAMPLE 3 Fail")
#define ERROR_MSG_00040400 ("00040400 PCO2 Span Diff Not Met - Skipped")

/** PCO2 Span2 Errors */
#define ERROR_MSG_00080001 ("00080001 PCO2 Licor Span2 Failed")
#define ERROR_MSG_00080002 ("00080002 PCO2 Span2 Flow SPAN2_ON Fail")
#define ERROR_MSG_00080004 ("00080004 PCO2 Span2 SAMPLE 1 Fail")
#define ERROR_MSG_00080008 ("00080008 PCO2 Span2 Flow SPAN2_OFF Fail")
#define ERROR_MSG_00080010 ("00080010 PCO2 Span2 Flow VENT Fail")
#define ERROR_MSG_00080020 ("00080020 PCO2 Span2 SAMPLE 2 Fail")
#define ERROR_MSG_00080040 ("00080040 PCO2 Span2 CAL Fail")
#define ERROR_MSG_00080080 ("00080080 PCO2 Span2 Flow POSTCAL Fail")
#define ERROR_MSG_00080100 ("00080100 PCO2 Span2 SAMPLE 3 Fail")

/** PCO2 Equilibrate & Air Failures */
#define ERROR_MSG_00100002 ("00100002 PCO2 Equil Flow EQUIL_ON Fail")
#define ERROR_MSG_00100004 ("00100004 PCO2 Equil SAMPLE 1 Fail")
#define ERROR_MSG_00100008 ("00100008 PCO2 Equil Flow EQUIL_OFF 1 Fail")
#define ERROR_MSG_00100010 ("00100010 PCO2 Equil Flow VENT Fail")
#define ERROR_MSG_00100020 ("00100020 PCO2 Equil Flow EQUIL_OFF 2 Fail")
#define ERROR_MSG_00100040 ("00100040 PCO2 Equil SAMPLE 2 Fail")

#define ERROR_MSG_00100200 ("00100200 PCO2 Air Flow AIR_ON Fail")
#define ERROR_MSG_00100400 ("00100400 PCO2 Air SAMPLE 1 Fail")
#define ERROR_MSG_00100800 ("00100800 PCO2 Air Flow AIR_OFF 1 Fail")
#define ERROR_MSG_00101000 ("00101000 PCO2 Air Flow VENT Fail")
#define ERROR_MSG_00102000 ("00102000 PCO2 Air Flow EQUIL_OFF Fail")
#define ERROR_MSG_00104000 ("00104000 PCO2 Air SAMPLE 2 Fail")

/** E1000 - E1999: MAX3109*/
#define ERROR_MSG_1001 ("0101 MAX310X Buffer Error")
#define ERROR_MSG_1010 ("0110 MAX310X SPI Error")
#define ERROR_MSG_1020 ("0120 MAX310X Invalid Baudrate")
#define ERROR_MSG_001010
/** E2000 - E2999:  */

/** E3000 - E3999:  RTC Errors */
#define ERROR_MSG_00200002 ("00200002 RTC Alarm Before Current Time")
#define ERROR_MSG_00200004 ("00200004 RTC Alarm After Current Alarm")
#define ERROR_MSG_00200008 ("00200008 RTC Alarm Repeat = 0")
#define ERROR_MSG_00200010 ("00200010 RTC Invalid Month")
#define ERROR_MSG_00200020 ("00200020 RTC SQW Invalid Pin")
#define ERROR_MSG_00200040 ("00200040 RTC Alarm Invalid Pin")
#define ERROR_MSG_00200080 ("00200080 RTC Msg Too Long")
#define ERROR_MSG_00200100 ("00200100 RTC Msg Length > Buffer")
#define ERROR_MSG_00200200 ("00200200 RTC Msg Length Too Short")
#define ERROR_MSG_00200400 ("00200400 RTC I2C Transmission Error")
#define ERROR_MSG_00200800 ("00200800 RTC I2C Recieve Error")
#define ERROR_MSG_00201000 ("00201000 RTC I2C Hang")
#define ERROR_MSG_00202001 ("00202001 RTC Offset Calc Error")
#define ERROR_MSG_00202000 ("00202000 RTC Oscillator Select Error")
#define ERROR_MSG_00202002 ("00202002 RTC Square Wave Config Error")

/** E4000 - E4999:  Flow Controller Errors*/
#define ERROR_MSG_00400001 ("00400001 FLOW Failed To Init")
#define ERROR_MSG_00400002 ("00400002 FLOW Failed On Startup")

#define ERROR_MSG_00400004 ("00400004 FLOW Invalid Flow State")
#define ERROR_MSG_00400008 ("00400008 FLOW Mode Set Failure")
#define ERROR_MSG_00400010 ("00400010 FLOW Message NACK")
#define ERROR_MSG_00400020 ("00400020 FLOW Message Not Sent")
#define ERROR_MSG_00400040 ("00400040 FLOW Mode Not Received")

/** E5000 - E5999:  RH/Temp/Pressure Sensor Errors */
#define ERROR_MSG_00400100 ("00400100 RH Sensor Error")
#define ERROR_MSG_00400200 ("00400200 RH I2C Failure")
/** E6000 - E6999:  Oxygen Sensor Errors */
#define ERROR_MSG_00401000 ("00401000 O2 Sensor Error")

/** E7000 - E7999:  LiCor 820/830 Errors */
#define ERROR_MSG_00800002 ("00800002 Invalid Sensor Type")
#define ERROR_MSG_00800004 ("00800004 Invalid XML Parent Tag")
#define ERROR_MSG_00800008 ("00800008 Invalid XML Child Tag")
#define ERROR_MSG_00800010 ("00800010 Invalid XML LVL3 Tag")

#define ERROR_MSG_00800020 ("00800020 Invalid XML Combo")
#define ERROR_MSG_00800040 ("00800040 Invalid XML Level 1")
#define ERROR_MSG_00800080 ("00800080 Invalid XML Level 2")
#define ERROR_MSG_00800100 ("00800100 Invalid XML Level 3")
#define ERROR_MSG_00800200 ("00800200 Invalid XML Level 4")

#define ERROR_MSG_00800400 ("00800400")
#define ERROR_MSG_00800800 ("00800800 No H2O in LI820/840")
#define ERROR_MSG_00801000 ("00801000 No Pump")

/** E8000 - E8999:  Datalogger Errors */

/** E9000 - E9999:  */

#endif // _ERRORLIST_H