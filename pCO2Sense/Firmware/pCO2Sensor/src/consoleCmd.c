/** @file consoleCmd.c
 *  @brief Console Commands
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date April 25, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *  @note
 *
 *  @bug  No known bugs
 */
#include "consoleCmd.h"

/************************************************************************
*					   STATIC Functions Prototypes
************************************************************************/
STATIC eConsoleCmdStatus_t CONSOLECMD_Quit(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Ver(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Help(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_Help(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Report(sUART_t *pUART, const char buffer[]);
#ifdef INCLUDE_TELOS_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_CShow(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_gps(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_idata(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sdata(sUART_t *pUART, const char buffer[]);
#endif
STATIC eConsoleCmdStatus_t CONSOLECMD_time(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SetBaudrate(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Log(sUART_t *pUART, const char buffer[]);
#ifdef INCLUDE_SCHEDULER_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_Tasks(sUART_t *pUART, const char buffer[]);
#endif
STATIC eConsoleCmdStatus_t CONSOLECMD_Startup(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_I2CFailures(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SerialNum(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Sample(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Span(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Span2(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SpanDiff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Equil(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Warmup(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SpanFlowOn( sUART_t *pUART, const char buffer[] );
STATIC eConsoleCmdStatus_t CONSOLECMD_Purge(sUART_t *pUART, const char buffer[] );
STATIC eConsoleCmdStatus_t CONSOLECMD_PumpOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_PumpOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SampleCO2(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Vent(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Heater(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_with_Purge(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_Quiet(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_Quiet_with_Purge(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FieldCheck(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorConfig(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorHeaterOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorHeaterOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKZero(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKSpan(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKSpan2(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorSample(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorSerial(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorVersion(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPulse(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowCurrent(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPurge(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ReadO2(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ReadO2Voltage(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_CalO2(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_O2Interval(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_O2Time(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_AnalogOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_AnalogOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ReadRH(sUART_t *pUART, const char buffer[]);
#ifdef INCLUDE_LED_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_GreenLedOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_GreenLedOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_RedLedOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_RedLedOff(sUART_t *pUART, const char buffer[]);
#endif
STATIC eConsoleCmdStatus_t CONSOLECMD_TestMode(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_NormalMode(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_StreamData(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Tpos(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_Apos(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_SReset(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorPassthrough(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPassThrough(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_RTCcal(sUART_t *pUART, const char buffer[]);
#ifdef TEST_RTC
STATIC eConsoleCmdStatus_t CONSOLECMD_TimeReg(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ChargeOn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ChargeOff(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_RTC_AutoCal(sUART_t *pUART, const char buffer[]);
#endif
STATIC eConsoleCmdStatus_t CONSOLECMD_MConfig(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ssct(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sscsc(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_lsstds(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sstds(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sstdsf(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ssconc(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_avalid(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_pbias(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_lpbm(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sscrh(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_atrhs(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_o2s(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_manufacturer(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sscssn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_sssn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_ssn(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_lssc(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_TerminalMode(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_Time(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_SamplingMode(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_SamplingSchedule(sUART_t *pUART, const char buffer[]);
STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_ReturnToSleep(sUART_t *pUART, const char buffer[]);
STATIC bool is_write_cmd(const char buffer[]);

STATIC bool verify_test_mode(sUART_t *pUART, eConsoleSubmode_t mode);
/************************************************************************
*					        STATIC Variables
************************************************************************/
STATIC const sConsoleCommandTable_t mConsoleCommandTable[] =
    {
        /** Basic System Commands */
        {"", NULL, HELP("\r\n*** SYSTEM SETTING COMMANDS ***")},
        {"?", &CONSOLECMD_Help, HELP("This Help menu")},
        {"Ctrl-C", NULL, HELP("3 times.  Exit Logging by reset(during sample)")},
        {"q", &CONSOLECMD_Quit, HELP("Exit Terminal Mode and sleep")},
        {"report", &CONSOLECMD_Report, HELP("Report all menu settings")},
        {"ver", &CONSOLECMD_Ver, HELP("Get the version string")},
        {"serial", &CONSOLECMD_SerialNum, HELP("Serial Number")},
        
#ifdef INCLUDE_SCHEDULER_COMMANDS
        {"tasks", &CONSOLECMD_Tasks, HELP("Display Tasks in queue")},
#endif
        {"startup", &CONSOLECMD_Startup, HELP("Display System Startup Count")},
        {"i2cfail", &CONSOLECMD_I2CFailures, HELP("Display I2C Failure Restarts")},
        {"sreset", &CONSOLECMD_SReset, HELP("Software Reset")},
        {"time", &CONSOLECMD_time, HELP("Sensor Date & Time (yy mm dd hh mm ss")},
        {"baud", &CONSOLECMD_SetBaudrate, HELP("Set CMD Baudrate")},
        {"log", &CONSOLECMD_Log, HELP("Set the Log Level (debug or info)")},
        /** TELOS Compliant Commands */
#ifdef INCLUDE_TELOS_COMMANDS
        {"", NULL, HELP("\r\n*** TELOS COMPLIANT COMMANDS ***")},
        {"cshow", &CONSOLECMD_CShow, HELP("Show COM Data")},
        {"gps", &CONSOLECMD_gps, HELP("GPS Data (-2 for PCO2 Sensor)")},
        {"time", &CONSOLECMD_time, HELP("Sensor Date & Time (yy mm dd hh mm ss")},
        {"idata", &CONSOLECMD_idata, HELP("Iridium Data")},
        {"sdata", &CONSOLECMD_sdata, HELP("Sensor Raw Data")},
        {"who", &CONSOLECMD_SerialNum, HELP("Serial Number")},
#endif
        /** pCO2 Individual Configuration Commands (Compatible with MApCO2) */
        {"", NULL, HELP("\r\n*** PCO2 CONFIG (MApCO2 Compatible) ***")},
        {"span", &CONSOLECMD_Span, HELP("Span Gas Concentration (ppm)")},
        {"span2", &CONSOLECMD_Span2, HELP("Span Gas Concentration (ppm)")},
        {"spandiff", &CONSOLECMD_SpanDiff, HELP("Span Pressure Difference (kPa)")},
        {"equil", &CONSOLECMD_Equil, HELP("Length of Equilibration Pump (sec)")},
        {"warmup", &CONSOLECMD_Warmup, HELP("Length of LICOR Warmup (sec)")},
        {"pumpon", &CONSOLECMD_PumpOn, HELP("Length of pump on during cycle (sec)")},
        {"pumpoff", &CONSOLECMD_PumpOff, HELP("Length of pump off during a cycle (sec)")},
        {"spanflow", &CONSOLECMD_SpanFlowOn, HELP("Span Flow ON duration (sec)")},
        {"purge", &CONSOLECMD_Purge, HELP("Purge duration (sec)")},
        {"sampleco2", &CONSOLECMD_SampleCO2, HELP("Length of data sample in cycle (sec)")},
        {"vent", &CONSOLECMD_Vent, HELP("Legth of delay between open/close vent (sec)")},
        {"heater", &CONSOLECMD_Heater, HELP("Licor Heater OFF or ON")},
        {"o2int", &CONSOLECMD_O2Interval, HELP("Set interval between O2 cal (days)")},
        {"o2time", &CONSOLECMD_O2Time, HELP("Time of day for O2 Cal (hh:mm:ss)")},
        

        /** Individual Status Commands */
        {"sample", &CONSOLECMD_Sample, HELP("Data Sampling Interval (Hz)")},

        /** Commands to start Logging */
        {"", NULL, HELP("\r\n*** LOGGING COMMANDS ***")},
        {"ts", &CONSOLECMD_TakeSample, HELP("Start Sample: Immediately")},
        {"tsp", &CONSOLECMD_TakeSample_with_Purge, HELP("Start Sample: Immediately with Purge")},
        {"tsq", &CONSOLECMD_TakeSample_Quiet, HELP("Start Sample: Quiet Mode")},
        {"tspq", &CONSOLECMD_TakeSample_Quiet_with_Purge, "Start Sample: Quiet Mode with Purge"},

        /** Field Check */
        {"", NULL, HELP("\r\n*** Field Check ***")},
        {"fcheck", &CONSOLECMD_FieldCheck, HELP("Field Check")},
        /** Valve Control Setting Commands */
        {"", NULL, HELP("\r\n*** VALVE SETTING COMMANDS ***")},
        {"tpos", &CONSOLECMD_Tpos, HELP("Sets Test For Cycling Flow Modes")},

        /** Set Test Mode Command */
        {"", NULL, HELP("\r\n*** TEST MODE ***")},
        {"test", &CONSOLECMD_TestMode, HELP("Enter Test Mode")},
        {"norm", &CONSOLECMD_NormalMode, HELP("Exit Test Mode")},
        {"stream", &CONSOLECMD_StreamData, HELP("Stream Data (Any key to exit)")},

        /** Licor Testing Commands */
        {"", NULL, HELP("\r\n*** LICOR COMMANDS ***")},
        {"licoron", &CONSOLECMD_LicorOn, HELP("Turn Licor ON")},
        {"licoroff", &CONSOLECMD_LicorOff, HELP("Turn Licor OFF")},
        {"licorcfg", &CONSOLECMD_LicorConfig, HELP("Test Mode Only: Get Licor Settings")},
        {"likzero", &CONSOLECMD_LicorKZero, HELP("Get/Set Licor co2kzero coefficient")},
        {"likspan", &CONSOLECMD_LicorKSpan, HELP("Get/Set Licor co2kspan coefficient")},
        {"likspan2", &CONSOLECMD_LicorKSpan2, HELP("Get/Set Licor co2kspan2 coefficient")},
        {"lisample", &CONSOLECMD_LicorSample, HELP("Test Mode Only: Sample the Licor")},
        {"heateron", &CONSOLECMD_LicorHeaterOn, HELP("Test Mode Only: Turn Licor Heater ON")},
        {"heateroff", &CONSOLECMD_LicorHeaterOff, HELP("Test Mode Only: Turn Licor Heater OFF")},
        {"liserial", &CONSOLECMD_LicorSerial, HELP("Get Licor Serial #")},
        {"liver", &CONSOLECMD_LicorVersion, HELP("Get LICOR Firmware Version #")},
        {"lipass", &CONSOLECMD_LicorPassthrough, HELP("Allow Direct Access to LICOR UART")},

        /** Flow Control Module Commands */
        {"", NULL, HELP("\r\n*** FLOW CONTROL COMMANDS ***")},
        {"flowon", &CONSOLECMD_FlowOn, HELP("Test Mode Only: Turn Flow Control ON")},
        {"flowoff", &CONSOLECMD_FlowOff, HELP("Test Mode Only: Turn Flow Control OFF")},
        {"flowpass", &CONSOLECMD_FlowPassThrough, HELP("Allow Direct Access to Flow Controller UART")},
        {"fpulse", &CONSOLECMD_FlowPulse, HELP("Set the pulse length of the HBridge")},
        {"fcurrent", &CONSOLECMD_FlowCurrent, HELP("Set the HBridge clipping current (0-7)")},
        {"fpurge", &CONSOLECMD_FlowPurge, HELP("Run the Purge routine")},
        /** Oxygen Sensor Commands */
        {"", NULL, HELP("\r\n*** OXYGEN (O2) SENSOR COMMANDS ***")},
        {"O2", &CONSOLECMD_ReadO2, HELP("Test Mode Only: Read O2 Sensor")},
        {"O2volt", &CONSOLECMD_ReadO2Voltage, HELP("Test Mode Only: Read O2 port voltage")},
        {"o2cal", &CONSOLECMD_CalO2, HELP("Test Mode Only: Calibrate O2 sensor")},
        {"anaon", &CONSOLECMD_AnalogOn, HELP("Test Mode Only: Turn Analog ON")},
        {"anaoff", &CONSOLECMD_AnalogOff, HELP("Test Mode Only: Turn Analog OFF")},

        /** Relative Humidity (RH)Sensor Commands */
        {"", NULL, HELP("\r\n*** RELATIVE HUMIDITY (RH) SENSOR COMMANDS ***")},
        {"rh", &CONSOLECMD_ReadRH, HELP("Test Mode Only: Read RH Sensor")},

        /** Real Time Clock (RTC) Commands */
        {"", NULL, HELP("\r\n*** REAL-TIME CLOCK (RTC) COMMANDS ***")},
        {"rtccal", &CONSOLECMD_RTCcal, HELP("RTC Calibration")},
#ifdef TEST_RTC
        {"treg", &CONSOLECMD_TimeReg, HELP("RTC Register Value")},
        {"tcon", &CONSOLECMD_ChargeOn, HELP("Charge RTC Cap")},
        {"tcoff", &CONSOLECMD_ChargeOff, HELP("Stop Charge RTC Cap")},
        {"tcal", &CONSOLECMD_RTC_AutoCal, HELP("Autocal RTC RC Osc")},
#endif
        /** Manufacture Commands */
        {"", NULL, HELP("\r\n*** MANUFACTURER CONFIG COMMANDS ***")},
        {"mconfig", &CONSOLECMD_MConfig, HELP("Allow access to Manufacture Commands")},
        {"ssct", &CONSOLECMD_ssct, HELP("secondaryspan_calibrated_temperature")},
        {"sscsc", &CONSOLECMD_sscsc, HELP("secondaryspan_calibrated_spanconcentration ")},
        {"lsstds", &CONSOLECMD_lsstds, HELP("last_secondaryspan_temperaturedependantslope ")},
        {"sstds", &CONSOLECMD_sstds, HELP("secondaryspan_temperaturedependantslope ")},
        {"sstdsf", &CONSOLECMD_sstdsf, HELP("secondaryspan_temperaturedependantslopefit")},
        {"sscssn", &CONSOLECMD_sscssn, HELP("secondaryspan_calibrated_spanserialnumber")},
        {"lssc", &CONSOLECMD_lssc, HELP("last_secondaryspan_calibrated")},
        {"ssn", &CONSOLECMD_ssn, HELP("ASVCO2_span_serialnumber")},
        {"ssconc", &CONSOLECMD_ssconc, HELP("ASVCO2_secondaryspan_concentration ")},
        {"sssn", &CONSOLECMD_sssn, HELP("ASVCO2_secondaryspan_serialnumber")},
        {"avalid", &CONSOLECMD_avalid, HELP("last_ASVCO2_validation ")},
        {"pbias", &CONSOLECMD_pbias, HELP("pressure_bias")},
        {"lpbm", &CONSOLECMD_lpbm, HELP("last_pressure_bias_measured ")},
        {"sscrh", &CONSOLECMD_sscrh, HELP("secondaryspan_calibrated_RH")},
        {"atrhs", &CONSOLECMD_atrhs, HELP("ASVCO2_ATRH_serial")},
        {"o2s", &CONSOLECMD_o2s, HELP("ASVCO2_O2_serial")},
        {"mfr", &CONSOLECMD_manufacturer, HELP("ASVCO2_manufacturer")},
#ifdef SPARE_PORT
        {"spareon", &CONSOLECMD_SpareOn, HELP("Test Mode Only: Turn Licor ON")},
        {"spareoff", &CONSOLECMD_SpareOff, HELP("Test Mode Only: Turn Licor OFF")},
#endif
        /** LED Commands */
#ifdef INCLUDE_LED_COMMANDS
        {"", NULL, HELP("\r\n*** LED COMMANDS ***")},
#ifdef GREEN_LED
        {"grnon", &CONSOLECMD_GreenLedOn, HELP("Test Mode Only: Turn GREEN LED ON")},
        {"grnoff", &CONSOLECMD_GreenLedOff, HELP("Test Mode Only: Turn GREEN LED OFF")},
#endif
#ifdef RED_LED
        {"redon", &CONSOLECMD_RedLedOn, HELP("Test Mode Only: Turn RED LED ON")},
        {"redoff", &CONSOLECMD_RedLedOff, HELP("Test Mode Only: Turn RED LED OFF")},
#endif
#endif
        CONSOLE_COMMAND_TABLE_END /* Must be last */
};

STATIC const sConsoleHotkeyTable_t mConsoleHotkeyTable[] =
    {
        {"?", &CONSOLECMD_HOTKEY_Help, HELP("This Help menu")},
        {"R", &CONSOLECMD_TakeSample, HELP("Start Sample Run")},
        {"O", &CONSOLECMD_CalO2, HELP("Calibrate O2 Sensor")},
        {"T", &CONSOLECMD_HOTKEY_TerminalMode, HELP("Console Terminal Mode")},
        {"t", &CONSOLECMD_HOTKEY_Time, HELP("Current Time")},
        {"S", &CONSOLECMD_HOTKEY_SamplingMode, HELP("Current Sampling Mode")},
        {"s", &CONSOLECMD_HOTKEY_SamplingSchedule, HELP("Current Sampling Schedule")},
        {"d", &CONSOLECMD_Report, HELP("Display Settings")},
        {"L", &CONSOLECMD_LicorSerial, HELP("LICOR Serial Number")},
        {"l", &CONSOLECMD_LicorVersion, HELP("LICOR Firmware Version")},
        {"A", &CONSOLECMD_Apos, HELP("Air Mode(Purge)")},
        {"q", &CONSOLECMD_HOTKEY_ReturnToSleep, HELP("Return to sleep mode")},
        CONSOLE_COMMAND_TABLE_END /* Must be last */
};

const char *OtherCommands[2] = {"Ctrl-C : Exit Logging Mode", "abcd"};
#ifdef UNIT_TESTING
uint32_t consoleCommandTableSize = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
#endif

STATIC char sendstr[CONSOLECMD_SENDSTR_LEN];
/************************************************************************
*					         Functions
************************************************************************/

/************************************************************************
*					   STATIC Functions 
************************************************************************/
STATIC eConsoleCmdStatus_t CONSOLECMD_Help(sUART_t *pUART, const char buffer[])
{
  uint32_t i;
  uint32_t tableLength;
  eConsoleCmdStatus_t result = CONSOLECMD_OK;

  IGNORE_UNUSED_VARIABLE(buffer);

  tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
  for (i = 0u; i < tableLength - 1u; i++)
  {
    CONSOLEIO_SendStr(pUART, (char *)mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
    CONSOLEIO_SendStr(pUART, "\r\t\t: ");
    CONSOLEIO_SendStr(pUART, (char *)mConsoleCommandTable[i].help);
#endif
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
#endif
  }

  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_Help(sUART_t *pUART, const char buffer[])
{
  uint32_t i;
  uint32_t tableLength;
  eConsoleCmdStatus_t result = CONSOLECMD_OK;

  IGNORE_UNUSED_VARIABLE(buffer);

  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  tableLength = sizeof(mConsoleHotkeyTable) / sizeof(mConsoleHotkeyTable[0]);
  for (i = 0u; i < tableLength - 1u; i++)
  {
    CONSOLEIO_SendStr(pUART, (char *)mConsoleHotkeyTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
    CONSOLEIO_SendStr(pUART, "\r\t: ");
    CONSOLEIO_SendStr(pUART, (char *)mConsoleHotkeyTable[i].help);
#endif
    CONSOLEIO_SendStr(pUART, STR_ENDLINE);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
#endif
  }

  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Ver(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_OK;

  IGNORE_UNUSED_VARIABLE(buffer);
  CONSOLEIO_SendStr(pUART, "ver= ");
  CONSOLEIO_SendStr(pUART, (char *)sysinfo.ver);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);
  return result;
}

#ifdef INCLUDE_TELOS_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_CShow(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  sTimeReg_t t;

  const char endstr[] = "NA\r\nsamplestart = 00:00:00\r\nsampleinterval = 00:00:00\r\nsampleperiod = 00:00:00\r\n";

  /** Get the Start Time */
  char temp[9];

  /** Add to buffer */
  strcpy(sendstr, "COM1\r\nPCO2\r\nsamplestart = ");

  /** Get Start time */
  PCO2_GetNormalStartTime(&t);
  sprintf(temp, "%02u:%02u:%02u", t.hour, t.minute, t.second);
  strcat(sendstr, temp);
  strcat(sendstr, "\r\nsampleinterval = ");

  /** Get Interval time */
  if (PCO2_GetSampleMode() == PCO2_SAMPLE_MODE_NORMAL)
  {
    PCO2_GetNormalInterval(&t);
  }
  else
  {
    PCO2_GetFastInterval(&t);
  }
  sprintf(temp, "%02u:%02u:%02u", t.hour, t.minute, t.second);
  strcat(sendstr, temp);
  strcat(sendstr, "\r\nsampleperiod = ");

  /** Get the period */
  PCO2_GetRunLength(&t);
  sprintf(temp, "%02u:%02u:%02u", t.hour, t.minute, t.second);
  strcat(sendstr, temp);
  strcat(sendstr, "\r\n");

  /** Add the other two fields */
  strcat(sendstr, endstr);
  strcat(sendstr, endstr);

  /** Send the CSHOW string */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}
#endif

STATIC eConsoleCmdStatus_t CONSOLECMD_Report(sUART_t *pUART, const char buffer[])
{

  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char timestamp[24];
  memset(timestamp, 0, 24);
  char temp[64];
  memset(temp, 0, 64);

  float temp_f = 0.0;
  uint32_t temp_u32 = 0;
  uint8_t temp_u8 = 0;

  sTimeReg_t runtime;

  AM08X5_GetTimestamp_ISO8601(timestamp);

  memset(sendstr, 0, CONSOLECMD_SENDSTR_LEN);

  strcat(sendstr, "ASVCO2v2\r\n");

  sprintf(temp, "serial= %s\r\n", sysinfo.SerialNumber);
  strcat(sendstr, temp);

  sprintf(temp, "ver= %s\r\n", sysinfo.ver);
  strcat(sendstr, temp);

  sprintf(temp, "startup= %lu\r\n", sysinfo.StartupCounter);
  strcat(sendstr, temp);

  sprintf(temp, "gps= %li\r\n", sysinfo.gps);
  strcat(sendstr, temp);

  sprintf(temp, "time= %s\r\n", timestamp);
  strcat(sendstr, temp);

  temp_f = PCO2_GetSpan();
  sprintf(temp, "span= %f\r\n", temp_f);
  strcat(sendstr, temp);

  temp_f = PCO2_GetSpanDiff();
  sprintf(temp, "spandiff= %f\r\n", temp_f);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetEquil();
  sprintf(temp, "equil= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetWarmup();
  sprintf(temp, "warmup= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetSpanFlowON();
  sprintf(temp, "spanflow= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetPurge();
  sprintf(temp, "purge= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetPumpON();
  sprintf(temp, "pumpon= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetPumpOFF();
  sprintf(temp, "pumpoff= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetSampleCO2();
  sprintf(temp, "sampleco2= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u32 = PCO2_GetVent();
  sprintf(temp, "vent= %lu\r\n", temp_u32);
  strcat(sendstr, temp);

  temp_u8 = PCO2_GetHeater();
  sprintf(temp, "heater= %d\r\n", temp_u8);
  strcat(sendstr, temp);

  temp_u8 = PCO2_GetSample();
  sprintf(temp, "sample= %d\r\n", temp_u8);
  strcat(sendstr, temp);

  char temp2[16];
  memset(temp2, 0, 16);
  LI8x0_Get_SerialNumber(temp2);
  sprintf(temp, "LI_ser= %s\r\n", temp2);
  strcat(sendstr, temp);

  memset(temp2, 0, 16);
  LI8x0_Get_Version(temp2);
  sprintf(temp, "LI_ver= %s\r\n", temp2);
  strcat(sendstr, temp);
  
  CONSOLEIO_SendStr(pUART, sendstr);
  memset(sendstr, 0, CONSOLECMD_SENDSTR_LEN);
  
  memset(temp2, 0, 16);
  LI8x0_Get_ccva1_1(temp2);
  sprintf(temp, "LI_ccva1= %s\r\n", temp2);
  strcat(sendstr, temp);
  
  memset(temp2, 0, 16);
  LI8x0_Get_ccva1_2(temp2);
  sprintf(temp, "LI_ccva2= %s\r\n", temp2);
  strcat(sendstr, temp);
  
  memset(temp2, 0, 16);
  LI8x0_Get_ccva1_3(temp2);
  sprintf(temp, "LI_ccva3= %s\r\n", temp2);
  strcat(sendstr, temp);
  
  memset(temp2, 0, 16);
  LI8x0_Get_ccva1_4(temp2);
  sprintf(temp, "LI_ccva4= %s\r\n", temp2);
  strcat(sendstr, temp);

  PCO2_Calculate_RunTime(&runtime);
  sprintf(temp, "runtime= %02d:%02d:%02d\r\n", runtime.hour,
          runtime.minute,
          runtime.second);
  strcat(sendstr, temp);

  /** Send the REPORT string */
  CONSOLEIO_SendStr(pUART, sendstr);
  
  /** Send the manufacture/cal settings */
  memset(sendstr, 0, CONSOLECMD_SENDSTR_LEN);
  
  /** */
  sprintf(sendstr, "secondaryspan_calibrated_temperature= %.3f\r\n", PCO2_Get_ssct());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "secondaryspan_calibrated_spanconcentration= %.2f\r\n", PCO2_Get_sscsc());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  char temps[24];
  PCO2_Get_lssc(temps);
  sprintf(sendstr, "last_secondaryspan_calibration= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_lsstds(temps);
  sprintf(sendstr, "last_secondaryspan_temperaturedependantslope= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "secondaryspan_temperaturedependantslope= %.8lf\r\n", PCO2_Get_sstds());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "secondaryspan_temperaturedependantslopefit= %.3f\r\n", PCO2_Get_sstdsf());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "secondaryspan_calibrated_rh= %.2f\r\n", PCO2_Get_sscrh());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_sscssn(temps);
  sprintf(sendstr, "secondaryspan_calibrated_spanserialnumber= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "ASVCO2_secondaryspan_concentration= %.2f\r\n", PCO2_Get_ssconc());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_sssn(temps);
  sprintf(sendstr, "ASVCO2_secondaryspan_serialnumber= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_ssn(temps);
  sprintf(sendstr, "ASVCO2_span_serialnumber= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_avalid(temps);
  sprintf(sendstr, "last_ASVCO2_validation= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  sprintf(sendstr, "pressure_bias= %.3f\r\n", PCO2_Get_pbias());
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_lpbm(temps);
  sprintf(sendstr, "last_pressure_bias_measured= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_atrhs(temps);
  sprintf(sendstr, "ASVCO2_ATRH_serial = %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_o2s(temps);
  sprintf(sendstr, "ASVCO2_O2_serial = %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  PCO2_Get_manufacturer(temps);
  sprintf(sendstr, "ASVCO2_manufacturer= %s\r\n", temps);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  result = CONSOLECMD_OK;
  return result;
}

#ifdef INCLUDE_TELOS_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_gps(sUART_t *pUART, const char buffer[])
{
  char gps[16];
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  sprintf(gps, "gps= %li", sysinfo.gps);

  CONSOLEIO_SendStr(pUART, gps);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);
  IGNORE_UNUSED_VARIABLE(buffer);

  result = CONSOLECMD_OK;
  return result;
}
#endif 

#ifdef TEST_RTC
STATIC eConsoleCmdStatus_t CONSOLECMD_TimeReg(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char timestr[32];
  memset(timestr, 0, 32);
  /** Check for write or read command */
  char temp[5];
  uint8_t reg = 0;
  uint8_t i;
  uint8_t len = 1;
  
  
  if (true == is_write_cmd(buffer))
  {

    strcpy(temp, &buffer[2]);
    reg = atoi(temp);
    temp[2] = NULL;
  }
  else
  {
    len = 46;
  }

  for (i = 0; i < len; i++)
  {
    memset(temp, 0, 5);
    sprintf(temp, "0x%02x", i);
    CONSOLEIO_SendStr(pUART, "REG");
    CONSOLEIO_SendStr(pUART, temp);
    CONSOLEIO_SendStr(pUART, "= ");
    AM08X5_GetRegisterValue(reg++, temp);
    CONSOLEIO_SendStr(pUART, temp);
    CONSOLEIO_SendStr(pUART, "\r\n");
  }

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ChargeOn(sUART_t *pUART, const char buffer[])
{
  Log("Trickle Charge: ON", LOG_Priority_High);
  AM08X5_Charge_ON();
  return CONSOLECMD_OK;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ChargeOff(sUART_t *pUART, const char buffer[])
{
  Log("Trickle Charge: OFF", LOG_Priority_High);
  AM08X5_Charge_OFF();
  return CONSOLECMD_OK;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_MConfig(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  
  memset(sendstr, 0, CONSOLECMD_SENDSTR_LEN);
  
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if( strncmp(&buffer[2], "PMEL", 4) == 0)
    {
      sprintf(sendstr, "mconfig set");
      PCO2_Set_mconfig(true);
    } else {
      sprintf(sendstr, "mconfig clear");
      PCO2_Set_mconfig(false);
    }
  }

 

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ssct(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_ssct(val);
      
    }
    
  }
  val = PCO2_Get_ssct();
  sprintf(sendstr, "ssct= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_sscsc(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_sscsc(val);
      
    }
    
  }
  val = PCO2_Get_sscsc();
  sprintf(sendstr, "sscsc= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_sstds(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  double val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_sstds(val);
      
    }
    
  }
  val = PCO2_Get_sstds();
  sprintf(sendstr, "sstds= %llf", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_lsstds(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "lsstds string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_lsstds((char*) temp);
    }
    
  }
  PCO2_Get_lsstds(temp);
  sprintf(sendstr, "lsstds= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_sstdsf(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_sstdsf(val);
      
    }
    
  }
  val = PCO2_Get_sstdsf();
  sprintf(sendstr, "sstdsf= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ssconc(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;

  val = PCO2_Get_ssconc();
  sprintf(sendstr, "ssconc= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_avalid(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "avalid string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_avalid((char*) temp);
    }
    
  }
  PCO2_Get_avalid(temp);
  sprintf(sendstr, "avalid= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_pbias(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_pbias(val);
      
    }
    
  }
  val = PCO2_Get_pbias();
  sprintf(sendstr, "pbias= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_lpbm(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "lpbm string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_lpbm((char*) temp);
    }
    
  }
  PCO2_Get_lpbm(temp);
  sprintf(sendstr, "lpbm= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_sscrh(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float val = 0.0f;
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      val = atof(&buffer[2]);
      PCO2_Set_sscrh(val);
      
    }
    
  }
  val = PCO2_Get_sscrh();
  sprintf(sendstr, "sscrh= %f", val);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_atrhs(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  PCO2_Get_atrhs(temp);
  sprintf(sendstr, "atrhs= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_o2s(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "o2s string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_o2s((char*) temp);
    }
    
  }
  PCO2_Get_o2s(temp);
  sprintf(sendstr, "o2s= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_manufacturer(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "Manufacturer string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_manufacturer((char*) temp);
    }
    
  }
  PCO2_Get_manufacturer(temp);
  sprintf(sendstr, "mfr= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
  
}
STATIC eConsoleCmdStatus_t CONSOLECMD_sscssn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "sscssn string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_sscssn((char*) temp);
    }
    
  }
  PCO2_Get_sscssn(temp);
  sprintf(sendstr, "sscssn= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_lssc(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "lssc string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_lssc((char*) temp);
    }
    
  }
  PCO2_Get_lssc(temp);
  sprintf(sendstr, "lssc= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
  
  
}


STATIC eConsoleCmdStatus_t CONSOLECMD_sssn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "sssn string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_sssn((char*) temp);
    }
    
  }
  PCO2_Get_sssn(temp);
  sprintf(sendstr, "sssn= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ssn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[24];
  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    if(!PCO2_Get_mconfig())
    {
      sprintf(sendstr, "NOT UNLOCKED - Must unlock mconfig first"); 
      CONSOLEIO_SendStr(pUART, sendstr);
      return result;
    }
    else 
    {
      if(strlen(&buffer[2]) > 23)
      {
        CONSOLEIO_SendStr(pUART, "ssn string too long\r\n");
        CONSOLEIO_SendStr(pUART, (char*) &buffer[2]);
        return result;
      }
      strcpy(temp, &buffer[2]);
      temp[strcspn(temp,"\r")] = 0;
      PCO2_Set_ssn((char*) temp);
    }
    
  }
  PCO2_Get_ssn(temp);
  sprintf(sendstr, "ssn= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}



STATIC eConsoleCmdStatus_t CONSOLECMD_RTC_AutoCal(sUART_t *pUART, const char buffer[])
{
  Log("Start RTC Autocal", LOG_Priority_High);
  AM08X5_Autocal_RC_Set();
  CONSOLECMD_TimeReg(pUART, "treg");
  
  _delay_ms(120000);
  Log("2 minutes elapsed", LOG_Priority_High);
  CONSOLECMD_TimeReg(pUART, "treg");
  AM08X5_Autocal_RC_Clear();
  
  Log("Autocal Cleared", LOG_Priority_High);
  CONSOLECMD_TimeReg(pUART, "treg");
  return CONSOLECMD_OK;
}
#endif

STATIC eConsoleCmdStatus_t CONSOLECMD_time(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char timestr[32];
  memset(timestr, 0, 32);
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    /** Write the time */
    if (AM08X5_OK != AM08X5_SetTime((char *)&buffer[2]))
    {
      return result;
    }
    CONSOLEIO_SendStr(pUART, "Time Set\r\n");
  }

  /** Get the Time */
  AM08X5_GetTimestamp_ISO8601(sendstr);

  /** Send the time */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_SetBaudrate(sUART_t *pUART, const char buffer[])
{
  
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  uint32_t baudrate;
  
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    char temp[10];
    memset(temp,0,10);
    strcpy(temp, (char*)&buffer[2]);
    baudrate= (uint32_t) atol(temp);
    
    /** Write the time */
    if (COMMAND_OK != COMMAND_SetBaudrate(baudrate))
    {
      return result;
    } else {
      
      CONSOLEIO_SendStr(pUART, "Baudrate Set\r\n");
    }
  }

  /** Get the Baudrate */
  baudrate = COMMAND_GetBaudrate();
  sprintf(sendstr, "baud= %lu", baudrate);
  
  /** Send the Baudrate */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
  
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Log(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
//  eLogOutputType_t mode = LOG_OUTPUT_Log;
  char output[10] = "";
  const char *pBuffer = &buffer[2];

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {

    if(strncmp(pBuffer, "debug", 5)==0)
    {
      SetLogLevel(LOG_LEVEL_Debug);
    } else if (strncmp(pBuffer, "error", 4)==0) {
      SetLogLevel(LOG_LEVEL_Error);
    } else {
      return result;
    }
  }

  /** Get the Baudrate */
  GetLogLevel(output);
  sprintf(sendstr, "log= %s", output);
  
  /** Send the Baudrate */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}
#ifdef INCLUDE_TELOS_COMMANDS
STATIC eConsoleCmdStatus_t CONSOLECMD_idata(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Call up Iridium data and send */
  PCO2_Get_idata();

  /** Request iData from PCO2 module */
  IGNORE_UNUSED_VARIABLE(buffer);
    
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_sdata(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  
  PCO2_Get_sdata();
  
  /** Request sData from PCO2 module */
  IGNORE_UNUSED_VARIABLE(buffer);

  result = CONSOLECMD_OK;
  return result;
}

#endif


STATIC eConsoleCmdStatus_t CONSOLECMD_Startup(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char startup[20];
  memset(startup, 0, 20);

  IGNORE_UNUSED_VARIABLE(buffer);
  /** Write # of Startups */
  sprintf(startup, "startup= %lu", sysinfo.StartupCounter);
  CONSOLEIO_SendStr(pUART, startup);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  /** Write # of Resets */
  sprintf(startup, "RST= %lu", sysinfo.RSTCounter);
  CONSOLEIO_SendStr(pUART, startup);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  /** Write # of Brown out Resets (BOR) */
  sprintf(startup, "BOR= %lu", sysinfo.BORCounter);
  CONSOLEIO_SendStr(pUART, startup);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  /** Write # of Power On Resets (POR) */
  sprintf(startup, "POR= %lu", sysinfo.PORCounter);
  CONSOLEIO_SendStr(pUART, startup);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_I2CFailures(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char startup[20];
  memset(startup, 0, 20);

  IGNORE_UNUSED_VARIABLE(buffer);
  sprintf(startup, "I2C Failures = %lu", sysinfo.I2CFailures);
  CONSOLEIO_SendStr(pUART, startup);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SerialNum(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  char temp[20];
  uint8_t i;

  /** Copy Serial Number without \r\n */
  if (true == is_write_cmd(buffer))
  {
    for (i = 0; i < 20; i++)
    {
      if ((buffer[i + 2] == '\r') || (buffer[i + 2] == '\n'))
      {
        temp[i] = NULL;
        break;
      }
      else
      {
        temp[i] = buffer[i + 2];
      }
    }
    if (i > 19)
    {
      i = 19;
    }
    strncpy(sysinfo.SerialNumber, temp, i);
    sysinfo.SerialNumber[i] = NULL;
  }

  /** Write Serial Number */
  CONSOLEIO_SendStr(pUART, "ser= ");
  CONSOLEIO_SendStr(pUART, sysinfo.SerialNumber);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Sample(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint8_t sample = 0;

  /** Get the Sample */
  sample = PCO2_GetSample();
  sprintf(sendstr, "sample= %u\r\n", sample);

  /** Send the sample */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Span(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  float span = 0.0;

  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    span = atof(&buffer[2]);
    PCO2_SetSpan(span);
  }

  memset(sendstr, 0, 64);
  span = PCO2_GetSpan();

  sprintf(sendstr, "span= %.2f\r\n", span);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Span2(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  float span = 0.0;

  /** Check for write command */
  if (true == is_write_cmd(buffer))
  {
    span = atof(&buffer[2]);
    PCO2_SetSpan2(span);
  }

  memset(sendstr, 0, 64);
  span = PCO2_GetSpan2();

  sprintf(sendstr, "span2= %.2f\r\n", span);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SpanDiff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  float spandiff = 0.0;

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    spandiff = atof(&buffer[2]);
    PCO2_SetSpanDiff(spandiff);
  }

  spandiff = PCO2_GetSpanDiff();
  sprintf(sendstr, "spandiff= %0.2f\r\n", spandiff);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Equil(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t eqsecs = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    eqsecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetEquil(eqsecs);
  }

  eqsecs = PCO2_GetEquil();
  sprintf(sendstr, "equil= %lu\r\n", eqsecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Warmup(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t warmup = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    warmup = strtoul(&buffer[2], NULL, 10);
    PCO2_SetWarmup(warmup);
  }

  warmup = PCO2_GetWarmup();
  sprintf(sendstr, "warmup= %lu\r\n", warmup);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SpanFlowOn( sUART_t *pUART, const char buffer[] )
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t spansecs = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    spansecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetSpanFlowON(spansecs);
  }

  spansecs = PCO2_GetSpanFlowON();
  sprintf(sendstr, "spanflow= %lu\r\n", spansecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;

}

STATIC eConsoleCmdStatus_t CONSOLECMD_Purge( sUART_t *pUART, const char buffer[] )
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t purgesecs = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    purgesecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetPurge(purgesecs);
  }

  purgesecs = PCO2_GetPurge();
  sprintf(sendstr, "purge= %lu\r\n", purgesecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;

}

STATIC eConsoleCmdStatus_t CONSOLECMD_PumpOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t pumpsecs = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    pumpsecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetPumpON(pumpsecs);
  }

  pumpsecs = PCO2_GetPumpON();
  sprintf(sendstr, "pumpon= %lu\r\n", pumpsecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_PumpOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t pumpsecs = 0;
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    pumpsecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetPumpOFF(pumpsecs);
  }

  pumpsecs = PCO2_GetPumpOFF();
  sprintf(sendstr, "pumpoff= %lu\r\n", pumpsecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SampleCO2(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t co2secs = 0;

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    co2secs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetSampleCO2(co2secs);
  }

  co2secs = PCO2_GetSampleCO2();
  sprintf(sendstr, "sampleco2= %lu\r\n", co2secs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Vent(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint32_t ventsecs = 0;

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    ventsecs = strtoul(&buffer[2], NULL, 10);
    PCO2_SetVent(ventsecs);
  }

  /** Get the Vent time */
  ventsecs = PCO2_GetVent();
  sprintf(sendstr, "vent= %lu\r\n", ventsecs);

  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Heater(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  memset(sendstr, 0, 64);
  uint8_t heater = 0;

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    switch (buffer[2])
    {
    case '0':
      heater = 0;
      break;
    case '1':
      heater = 1;
      break;
    default:
      return result;
      break;
    }

    /** Set the Heater to Commanded State */
    PCO2_SetHeater(heater);
  }

  /** Get Heater State */
  heater = PCO2_GetHeater();
  sprintf(sendstr, "heater= %u\r\n", heater);

  /** Send Heater State */
  CONSOLEIO_SendStr(pUART, sendstr);

  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);
//  OutString("ts", LOG_OUTPUT_None);
  
  /** Run The Sample */
  PCO2_InstantRun();

  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_with_Purge(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);
//  OutString("ts", LOG_OUTPUT_None);
  
  /** Run The Sample */
  PCO2_InstantRun_with_Purge();

  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_Quiet(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);
    
  /** Indicate Start of Run */
  Message("Starting Sample - Quiet Mode\r\n");
  
  /** Set Quiet Mode */
  SetLogQuiet(true);

  /** Run The Sample */
  PCO2_InstantRun();
  
  /** Set Previous Verbosity */
  SetLogQuiet(false);
  
  /** Indicate End of Sample */
  Message("Sample Complete\r\n");
    
  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_TakeSample_Quiet_with_Purge(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);
  
  /** Indicate Start of Run */
  Message("Starting Sample - Quiet Mode\r\n");
  
  /** Set Quiet Mode */
  SetLogQuiet(true);

  /** Run The Sample */
  PCO2_InstantRun_with_Purge();
  
  /** Set Previous Verbosity */
  SetLogQuiet(false);
  
  /** Indicate End of Sample */
  Message("Sample Complete\r\n");
    
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_FieldCheck(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  eConsoleSubmode_t startMode = sysinfo.Submode;
  
  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    CONSOLECMD_TestMode(pUART, buffer);
    Log("Test Mode Set", LOG_Priority_High);
  }

  enum mode {
    MODE_SPAN,
    MODE_EPON,
    MODE_QUIT
  };

  enum mode current = MODE_SPAN;
  uint16_t timer = 0;

  volatile char *p1Start = pConsoleRxBuffer;
  volatile char *p2Start = pCommandRxBuffer;

  sTimeReg_t time_to_add = {
    .century = 0,
    .year = 0,
    .month = 0,
    .date = 0,
    .hour = 0,
    .minute = 0,
    .second = 45
  };

  sTimeReg_t now;
  sTimeReg_t end;
  AM08X5_GetTime(&now);
  AM08X5_add_time_struct(&now, &time_to_add, &end);

  Log("Starting Field Check...", LOG_Priority_High);
  Log("Starting Span Flow...", LOG_Priority_High);
  FLOW_SetMode(FLOW_STATE_SPAN_PUMP_ON);
  do{
    AM08X5_GetTime(&now);
    switch(current)
    {
      case MODE_SPAN:
      // if(timer >= 45)
      if(AM08X5_compare_time(&now, &end))
      {
        Log("Span Flow complete...", LOG_Priority_High);
        Log("Starting EPON Mode...", LOG_Priority_High);
        FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_ON);
        current = MODE_EPON;
        AM08X5_clear_time_struct(&time_to_add);
        time_to_add.minute=10;
        AM08X5_add_time_struct(&now, &time_to_add, &end);
      }
      break;
      case MODE_EPON:
      // if(timer > 600)
      if(AM08X5_compare_time(&now, &end))
      {
        Log("EPON Mode complete...", LOG_Priority_High);
        current = MODE_QUIT;
      }
      break;
      case MODE_QUIT:
        break;
      default:
        current = MODE_QUIT;
        break;
    }

    PCO2_StreamSensors();

    if( (p1Start != pConsoleRxBuffer) || (p2Start != pCommandRxBuffer))
    {
      current = MODE_QUIT;
    }
    timer++;
  }while(current != MODE_QUIT);

  Log("Starting REST Mode...", LOG_Priority_High);
  FLOW_SetMode(FLOW_STATE_REST);


  if(startMode == SYS_Submode_Normal)
  {
    CONSOLECMD_NormalMode(pUART, buffer);
    Log("Returning to normal mode", LOG_Priority_High);
  }

  result = CONSOLECMD_OK;
  return result;

}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Initialize the Licor */
  LI8x0_Init();

  /** Configure the Licor */
  result = (eConsoleCmdStatus_t)LI8x0_Config();

  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Clear the time struct */
  LI8x0_PowerOFF();

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorConfig(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Add Licor Configure to Scheduler */
    LI8x0_Config();
    LI8x0_GetInfo();
    uint16_t len;
    memset(sendstr, 0, CONSOLECMD_SENDSTR_LEN);
    LI8x0_CreateString_SysInfo((char *)&sendstr, &len);
    CONSOLEIO_SendStr(pUART, sendstr);
  }

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorHeaterOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Turn Heater ON*/
    LI8x0_Heater(1);
  }
  result = CONSOLECMD_OK;
  return result;
}
STATIC eConsoleCmdStatus_t CONSOLECMD_LicorHeaterOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Turn Heater ON*/
    LI8x0_Heater(0);
  }
  result = CONSOLECMD_OK;
  return result;
}


STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKZero(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float value = 0.0;
  bool li_init_state = LI8x0_GetPower();
  if(li_init_state == false)
  {
    LI8x0_PowerON();
    LI8x0_Init();
    LI8x0_Config();
  }
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    /** Convert buffer to float */
    value = atof((char *)&buffer[2]);
    LI8x0_Set_Zero(value);
    
  }
  LI8x0_GetInfo();
  
  char temp[30];
  LI8x0_Get_Zero(temp);
  sprintf(sendstr, "co2kzero= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  if(li_init_state == false)
  {
    LI8x0_PowerOFF();
  }
  
  result = CONSOLECMD_OK;
  return result;
}


STATIC bool check_valid_number(const char *pBuf, uint8_t len, float *value){
  
     /** Convert buffer to float */

    bool valid = false;
    for(uint8_t i=0; i<16; i++) {
      if(pBuf[i] == '\r' || pBuf[i] == '\n')
      {
        break;
      }
      
      if(i==0)
      {
        valid = ((pBuf[i] >= '0') && (pBuf[i] <= '9')) || (pBuf[i] == '.') || (pBuf[i] == '-'); 
      } else {
        valid = ((pBuf[i] >= '0') && (pBuf[i] <= '9')) || (pBuf[i] == '.'); 
      }
      if( valid != true)
      {
        break;
      }
    }
    
    if(valid)
    {
      *value = atof((char *)pBuf);
      
    }
    
    return valid;
  
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKSpan(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float value = 0.0;
  bool li_init_state = LI8x0_GetPower();
  if(li_init_state == false)
  {
    LI8x0_PowerON();
    LI8x0_Init();
    LI8x0_Config();
  }
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    if(check_valid_number((char*)&buffer[2], 16, &value))
    {
      LI8x0_Set_Span(value);
    }
  }
  LI8x0_GetInfo();
  
  char temp[30];
  LI8x0_Get_Span(temp);
  sprintf(sendstr, "co2kspan= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  if(li_init_state == false)
  {
    LI8x0_PowerOFF();
  }
  
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorKSpan2(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float value = 0.0;
  bool li_init_state = LI8x0_GetPower();
  if(li_init_state == false)
  {
    LI8x0_PowerON();
    LI8x0_Init();
    LI8x0_Config();
  }
  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    if(check_valid_number((char*)&buffer[2], 16, &value))
    {
      LI8x0_Set_Span2(value);
    }
  }
  LI8x0_GetInfo();
  
  char temp[30];
  LI8x0_Get_Span2(temp);
  sprintf(sendstr, "co2kspan2= %s", temp);
  CONSOLEIO_SendStr(pUART, sendstr);
  
  if(li_init_state == false)
  {
    LI8x0_PowerOFF();
  }
  
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorSample(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  PCO2_Initialize();

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Check for write or read command */
    if (LI8x0_GetPower() == false)
    {
      LI8x0_PowerON();
      _delay_ms(5000);
    }

    LI8x0_Sample();

    memset(sendstr, NULL, CONSOLECMD_SENDSTR_LEN);
    LI8x0_CreateString_Data((char *)&sendstr, 0);
    CONSOLEIO_SendStr(pUART, sendstr);
  }
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorSerial(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    PCO2_Initialize();
    LI8x0_Config();
    _delay_ms(2000);
  }
  char str[80] = "LICOR Serial #";

  LI8x0_Get_SerialNumber(sendstr);
  strcat(str, sendstr);
  Data(str);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_LicorVersion(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    PCO2_Initialize();
    LI8x0_Config();
    _delay_ms(2000);
  }
  char str[80] = "LICOR Firmware Version #";
  LI8x0_GetInfo();
  LI8x0_Get_Version(sendstr);
  strcat(str, sendstr);
  Data(str);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_FlowOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Initialize the Flow Controller */
    FLOW_Init();

    /** Flow ON */
    FLOW_Wakeup();
  }
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_FlowOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Flow OFF */
  FLOW_Sleep();

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPulse(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  
    int16_t pulse = 0;
    uint8_t cnt = 0;
    
  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    FLOW_Sleep();
    FLOW_Init();
    FLOW_Wakeup();
    if (true == is_write_cmd(buffer))
    {
        pulse  = atoi((char *)&buffer[2]);
        FLOW_SetPulse(pulse);
        _delay_ms(500);
    }
    
    while(cnt < 5)
    {
      pulse = FLOW_GetPulse();
      if(pulse > 0)
      {
        sprintf(sendstr, "Flow Control Pulse: %u ms", pulse);
        break;
      } else {
        sprintf(sendstr, "Flow Control Pulse: Error reading\r\n");
        cnt++;
        CONSOLEIO_SendStr(pUART, sendstr);
        CONSOLEIO_SendStr(pUART, "Retry; restarting FLOW\r\n");
        FLOW_Sleep();
        FLOW_Init();
        FLOW_Wakeup();       
      }
    }
  }

  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}
STATIC eConsoleCmdStatus_t CONSOLECMD_FlowCurrent(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  
    int16_t current = 0;
    uint8_t cnt =0;
   
  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    FLOW_Sleep();
    FLOW_Init();
    FLOW_Wakeup();
    if (true == is_write_cmd(buffer))
    {
        current  = atoi((char *)&buffer[2]);
        FLOW_SetCurrent(current);
        _delay_ms(500);
    }
    
    while(cnt < 5)
    {
      current = FLOW_GetCurrent();
      if(current > 0)
      {
        sprintf(sendstr, "Flow Control Current: %u", current);
        break;
      } else {
        sprintf(sendstr, "Flow Control Current: Error reading");
        cnt++;
        CONSOLEIO_SendStr(pUART, sendstr);
        CONSOLEIO_SendStr(pUART, "Retry; restarting FLOW\r\n");
        FLOW_Sleep();
        FLOW_Init();
        FLOW_Wakeup();  
      }
    }
  }

  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}



STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPurge(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  ePCO2Status_t status = PCO2_FAIL;
  
    int16_t current = 0;
    uint8_t cnt =0;
   
  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
//    FLOW_Sleep();
//    FLOW_Init();
//    FLOW_Wakeup();
//    if (true == is_write_cmd(buffer))
//    {
//        current  = atoi((char *)&buffer[2]);
//        FLOW_SetCurrent(current);
//        _delay_ms(500);
//    }
//    
//    while(cnt < 5)
//    {
//      current = FLOW_GetCurrent();
//      if(current > 0)
//      {
//        sprintf(sendstr, "Flow Control Current: %u", current);
//        break;
//      } else {
//        sprintf(sendstr, "Flow Control Current: Error reading");
//        cnt++;
//        CONSOLEIO_SendStr(pUART, sendstr);
//        CONSOLEIO_SendStr(pUART, "Retry; restarting FLOW\r\n");
//        FLOW_Sleep();
//        FLOW_Init();
//        FLOW_Wakeup();  
//      }
//    }
    status = PCO2_State_Purge();
//    if(status == PCO2_OK)
//    {
//        Log("Purge Success");
//    } else {
//      Log("Purge Fail");
//    }
  }
  
  

  CONSOLEIO_SendStr(pUART, sendstr);
  result = CONSOLECMD_OK;
  return result;
}
#ifdef SPARE_PORT
STATIC eConsoleCmdStatus_t CONSOLECMD_SpareOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  sTimeReg_t t1;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SpareOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  sTimeReg_t t1;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  result = CONSOLECMD_OK;
  return result;
}
#endif

STATIC eConsoleCmdStatus_t CONSOLECMD_ReadO2(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float o2 = 0;
  char str[16];

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Read the O2 Value */
    o2 = MAX250_Read();

    sprintf(str, "O2= %3.3f\r\n", o2);

    CONSOLEIO_SendStr(pUART, str);
  }
  result = CONSOLECMD_OK;
  return result;
}
STATIC eConsoleCmdStatus_t CONSOLECMD_ReadO2Voltage(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float o2 = 0;
  char str[16];

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Read the O2 Value */
    o2 = MAX250_ReadVoltage();

    sprintf(str, "O2= %3.3f\r\n", o2);

    CONSOLEIO_SendStr(pUART, str);
  }
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_CalO2(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float o2cal = 0;
  char str[24];

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);
  
  bool test_mode = verify_test_mode(pUART, sysinfo.Submode);

  if (test_mode != true)
  {
    CONSOLECMD_TestMode(pUART, buffer);
  }

  /** Read the O2 Value */
  PCO2_CalO2();

  /** Read the Cal Coefficience */
  o2cal = MAX250_GetCal();
  UCA1IFG &= ~0x020;
  UCA1TXBUF = 0x00;

  _delay_ms(1000);
  UCA1IFG |= 0x02;
  memset(str, 0, 24);
  strcpy(str, "O2cal complete\n");
  
//  OutString(str, LOG_OUTPUT_None);
  Message(str);
  
  memset(str, 0, 24);
  sprintf(str, "O2cal= %3.3f\r\n", o2cal);
//  OutString(str, LOG_OUTPUT_None);
  Message(str);

  /** Read O2 Voltage */
  memset(str, 0, 24);
  sprintf(str, "O2volt= %3.3f\r\n", MAX250_ReadVoltage());
//  OutString(str, LOG_OUTPUT_None);
  Message(str);
  
  /** Read O2 Value */
  memset(str, 0, 24);
  sprintf(str, "O2 = %3.3f%\r\n", MAX250_Read());
//  OutString(str, LOG_OUTPUT_None);
  Message(str);
    
  if(!test_mode)
  {
    CONSOLECMD_NormalMode(pUART, buffer);
  }

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_O2Interval(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  sTimeReg_t interval;
  sTimeReg_t *pInterval = &interval;
  memset(sendstr, 0, 64);

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    interval.date = atoi((char *)&buffer[2]);
    PCO2_SetO2Interval(pInterval);
  }

  return result;
}
STATIC eConsoleCmdStatus_t CONSOLECMD_O2Time(sUART_t *pUART, const char buffer[])
{

  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  sTimeReg_t time;
  sTimeReg_t *pTime = &time;
  memset(sendstr, 0, 64);

  /** Check for write or read command */
  if (true == is_write_cmd(buffer))
  {
    time.date = atoi((char *)&buffer[2]);
    PCO2_SetO2Time(pTime);
  }

  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_AnalogOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Add Licor ON to Scheduler */
    MAX250_PowerON();
  }
  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_AnalogOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Add Licor ON to Scheduler */
  MAX250_PowerOFF();

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_ReadRH(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  float rh = 0.0f;
  float temperature = 0.0f;
  char str[32];

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Read the O2 Value */
    SHT3X_GetTempAndHumidity(&temperature, &rh, REPEATAB_HIGH, MODE_POLLING, 10);

    sprintf(str, "rh= %3.3f\r\n", rh);
    CONSOLEIO_SendStr(pUART, str);
    sprintf(str, "temperature= %3.3fdegC\r\n", temperature);
    CONSOLEIO_SendStr(pUART, str);
  }

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_RTCcal(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Set the Calibration Channel */
  AM08X5_CrystalCal_Set();

  /** Wait for user input Frequency or Ctrl-Z */
  float freq = 0.0;

  /** Reset the Console buffer */
  memset((char *)ConsoleRxBuffer, 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
  pCommandRxBuffer = &CommandRxBuffer[0];
  /** */
  char temp[32];
  memset(temp, NULL, 32);
  uint8_t cnt = 100;
  uint16_t t_len = 0;
  while (true && (cnt-- > 0))
  {
      CONSOLEIO_Receive(pUART, temp, 32, &t_len);

    if (t_len > 0)
    {
      /** Check for Ctrl-C */
      if (strchr(temp, 0x03) != NULL)
      {
        freq = 0.0;
        break;
      }

      /** Convert to Number */
      freq = atof(temp);
      break;
    }
    _delay_ms(1000);
  }

  if ((freq > 32700) && (freq < 32836))
  {
    /** Convert Frequency to Offset */
    if (AM08X5_OffsetCalculation(freq) == AM08X5_OK)
    {
      result = CONSOLECMD_OK;
    }
    else
    {
      result = CONSOLECMD_FAIL;
    }
  }
  else
  {
    CONSOLEIO_SendStr(pUART, "Invalid Entry\r\n");
  }

  AM08X5_CrystalCal_Clear();

  return result;
}

#ifdef GREEN_LED
STATIC eConsoleCmdStatus_t CONSOLECMD_GreenLedOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Green LED ON*/
  LED_SetState(&grnLED, LED_ON_STEADY);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_GreenLedOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Green LED OFF*/
  LED_SetState(&grnLED, LED_OFF_STATE);

  result = CONSOLECMD_OK;
  return result;
}
#endif

#ifdef RED_LED
STATIC eConsoleCmdStatus_t CONSOLECMD_RedLedOn(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Red LED ON*/
  LED_SetState(&redLED, LED_ON_STEADY);

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_RedLedOff(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Red LED ON*/
  LED_SetState(&redLED, LED_OFF_STATE);

  result = CONSOLECMD_OK;
  return result;
}
#endif
STATIC eConsoleCmdStatus_t CONSOLECMD_TestMode(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Submode to TEST MODE */
  sysinfo.Submode = SYS_Submode_Test;

  /** Set up sensors */
  BSP_Hit_WD();
  PCO2_Initialize();

  /** Configure the Licor */
  BSP_Hit_WD();
  LI8x0_Config();

  /** Get Licor System Info */
  BSP_Hit_WD();
  LI8x0_GetInfo();

  result = CONSOLECMD_OK;
  BSP_Hit_WD();
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_NormalMode(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Submode to TEST MODE */
  sysinfo.Submode = SYS_Submode_Normal;

  /** Set up sensors */
  PCO2_LowPowerMode();

  result = CONSOLECMD_OK;
  return result;
}



STATIC eConsoleCmdStatus_t CONSOLECMD_StreamData(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    /** Turn the Licor OFF */
    LI8x0_PowerON();

    volatile char *p1Buf = pConsoleRxBuffer;
    volatile char *p2Buf = pCommandRxBuffer;
    const char DataHeader[] = "Mode,TS,SN,CO2(ppm),CTemp(C),CPres(kPa),CO2(Raw),CO2Ref(Raw),RH(%),RH_T(C),O2(%)";
    Data((char *)DataHeader);
    do
    {

      PCO2_StreamSensors();

      /** Switch Pump On/Off if commanded */
      if ((p1Buf != pConsoleRxBuffer) || (p2Buf != pCommandRxBuffer) )
      {
        if ((*p1Buf == 'P') || (*p1Buf == 'p') ||
            (*p2Buf == 'P') || (*p2Buf == 'p') )
        {
          FLOW_PumpToggle();
          pConsoleRxBuffer = p1Buf;
          pCommandRxBuffer = p2Buf;
        }
      }
    } while ( (p1Buf == pConsoleRxBuffer) && (p2Buf == pCommandRxBuffer));
  }

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Apos(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    CONSOLECMD_TestMode(pUART, buffer);
    Log("Test Mode Set", LOG_Priority_High);
  }
  /** Set the Valves to AIR Mode */
  FLOW_SetMode(FLOW_STATE_AIR_PUMP_ON);

  /** Submode to TEST MODE */
  sysinfo.Submode = SYS_Submode_Test;

  /** Enter Streaming */
  CONSOLECMD_StreamData(pUART, buffer);

  result = CONSOLECMD_OK;
  return result;
}



STATIC eConsoleCmdStatus_t CONSOLECMD_Tpos(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;
  bool quitFlag = false;
  bool pumpOnFlag = false;
  uint8_t zero_cnt = 0;
  uint8_t span_cnt = 0;
  uint8_t equil_cnt = 0;
  uint8_t air_cnt = 0;
  uint8_t purge_cnt = 0;
  eFLOWStatus_t fresult = FLOW_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  if (verify_test_mode(pUART, sysinfo.Submode) == true)
  {
    volatile char *p1Buf = pConsoleRxBuffer;
    volatile char *p2Buf = pCommandRxBuffer;
    volatile char Buf;
    fresult = FLOW_SetMode(FLOW_STATE_REST);
    if (fresult == FLOW_OK)
    {
      do
      {
        BSP_Hit_WD();
        /** Switch Pump On/Off if commanded */
        if ((p1Buf != pConsoleRxBuffer) || (p2Buf != pCommandRxBuffer) )
        {
          if(p1Buf != pConsoleRxBuffer)
          {
            Buf = *p1Buf;
          } else {
            Buf = *p2Buf;
          }
          switch (Buf)
          {
          case 'A':
          case 'a':
            zero_cnt = 0;
            span_cnt = 0;
            equil_cnt = 0;
            purge_cnt = 0;
            switch (air_cnt++)
            {
            case 0:
              fresult = FLOW_SetMode(FLOW_STATE_AIR_PUMP_ON);
              break;
            case 1:
              fresult = FLOW_SetMode(FLOW_STATE_AIR_VENT);
              break;
            case 2:
              fresult = FLOW_SetMode(FLOW_STATE_AIR_PUMP_OFF);
              break;
            case 3:
              fresult = FLOW_SetMode(FLOW_STATE_AIR_POST);
              air_cnt = 0;
              break;
            default:
              air_cnt = 0;
              break;
            }
            break;

          case 'E':
          case 'e':
            zero_cnt = 0;
            span_cnt = 0;
            air_cnt = 0;
            purge_cnt = 0;
            switch (equil_cnt++)
            {
            case 0:
              fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_ON);
              break;
            case 1:
              fresult = FLOW_SetMode(FLOW_STATE_EQUIL_VENT);
              break;
            case 2:
              fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_OFF);
              break;
            case 3:
              fresult = FLOW_SetMode(FLOW_STATE_EQUIL_PUMP_POST);
              equil_cnt = 0;
              break;
            default:
              equil_cnt = 0;
              break;
            }
            break;

          case 'R':
          case 'r':
            fresult = FLOW_SetMode(FLOW_STATE_REST);
            pumpOnFlag = false;
            break;

          case 'Z':
          case 'z':
            equil_cnt = 0;
            span_cnt = 0;
            air_cnt = 0;
            purge_cnt = 0;
            switch (zero_cnt++)
            {
            case 0:
              fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_ON);
              break;
            case 1:
              fresult = FLOW_SetMode(FLOW_STATE_ZERO_VENT);
              break;
            case 2:
              fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_OFF);
              break;
            case 3:
              fresult = FLOW_SetMode(FLOW_STATE_ZERO_PUMP_POST);
              zero_cnt = 0;
              break;
            default:
              zero_cnt = 0;
              fresult = FLOW_FAIL;
              break;
            }
            break;

          case 'S':
          case 's':
            equil_cnt = 0;
            zero_cnt = 0;
            air_cnt = 0;
            purge_cnt = 0;
            switch (span_cnt++)
            {
            case 0:
              fresult = FLOW_SetMode(FLOW_STATE_SPAN_PUMP_ON);
              break;
            case 1:
              fresult = FLOW_SetMode(FLOW_STATE_SPAN_VENT);
              break;
            case 2:
              fresult = FLOW_SetMode(FLOW_STATE_SPAN_PUMP_OFF);
              break;
            case 3:
              fresult = FLOW_SetMode(FLOW_STATE_SPAN_PUMP_POST);
              span_cnt = 0;
              break;
            default:
              span_cnt = 0;
              fresult = FLOW_FAIL;
              break;
            }
            break;

          case 'D':
          case 'd':
            fresult = FLOW_SetMode(FLOW_STATE_DEPLOY);
            break;
          case 'P':
          case 'p':
            pumpOnFlag ^= true;
            FLOW_PumpToggle();
            fresult = FLOW_OK;
            break;

          case 'Q':
          case 'q':
            fresult = FLOW_SetMode(FLOW_STATE_REST);
            quitFlag = true;
            fresult = FLOW_OK;
            break;

          case 'U':
          case 'u':
            equil_cnt = 0;
            zero_cnt = 0;
            air_cnt = 0;
            span_cnt = 0;
            switch (purge_cnt++)
            {
            case 0:
              fresult = FLOW_SetMode(FLOW_STATE_PRG1);
              break;
            case 1:
              fresult = FLOW_SetMode(FLOW_STATE_PRG2);
              break;
            case 2:
              fresult = FLOW_SetMode(FLOW_STATE_PRG3);
              break;
            case 3:
              fresult = FLOW_SetMode(FLOW_STATE_PRG4);
              break;
            case 4:
              fresult = FLOW_SetMode(FLOW_STATE_PRG5);
              break;
            case 5:
              fresult = FLOW_SetMode(FLOW_STATE_PRG6);
              break;
            case 6:
              fresult = FLOW_SetMode(FLOW_STATE_PRG7);
              break;
            case 7:
              fresult = FLOW_SetMode(FLOW_STATE_PRG8);
              purge_cnt = 0;
              break;
            default:
              purge_cnt = 0;
              fresult = FLOW_FAIL;
              break;
            }
            break;
          case '*':
            fresult = FLOW_SetMode(FLOW_STATE_PRESSURIZE);
            break;
          case '0':
            LI8x0_Zero();
            fresult = FLOW_OK;
            break;
          case '1':
            LI8x0_Span(PCO2_GetSpan());
            fresult = FLOW_OK;
            break;
          case '2':
            LI8x0_Span2(PCO2_GetSpan2());
            fresult = FLOW_OK;
            break;
          default:

            break;
          }
        }
        if (fresult != FLOW_OK)
        {
          quitFlag = true;
        }

        if (quitFlag != true)
        {
          pConsoleRxBuffer = p1Buf;
          pCommandRxBuffer= p2Buf;
          do
          {
            BSP_Hit_WD();
            PCO2_StreamSensors();
          } while ((p1Buf == pConsoleRxBuffer) && (p2Buf == pCommandRxBuffer));
        }
        BSP_Hit_WD();
      } while (quitFlag != true);
    }
  }

  if (fresult == FLOW_OK)
  {
    result = CONSOLECMD_OK;
    FLOW_SetMode(FLOW_STATE_REST);
    return result;
  }
  else
  {
    Log("FAIL to Set Flow Mode", LOG_Priority_High);
    return result;
  }
}

STATIC eConsoleCmdStatus_t CONSOLECMD_SReset(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Set the Valves to AIR Mode */
  FLOW_SetMode(FLOW_STATE_REST);

  /** Take the system out of Test Mode */
  CONSOLECMD_NormalMode(pUART, buffer);

  /** Software Reset */
#ifndef UNIT_TESTING
  PMMCTL0 |= PMMSWBOR;
#endif

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_FlowPassThrough(sUART_t *pUART, const char buffer[])
{
  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Power the Licor On */
  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    CONSOLECMD_TestMode(pUART, buffer);
    Log("Test Mode Set", LOG_Priority_High);
  }
   
  _delay_ms(500);

  /**  Put UART In Passthrough */
  UartPassthrough = SYS_UART_FLOW_PASSTHROUGH;

  /** Reset the Console buffer */
  memset((char *)ConsoleRxBuffer, 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
  
  /** Reset the Command buffer */
  memset((char *)CommandRxBuffer, 0, SYSTEM_COMMAND_RX_BUFFER_LEN);
  pCommandRxBuffer = &CommandRxBuffer[0];

  while (1)
  {
    BSP_Hit_WD();
    /** Write the string */
    uint16_t len =0;
    memset(sendstr, 0, 256);
    CONSOLEIO_Receive(pUART, sendstr, 256, &len);
    for(uint8_t i=0;i<len;i++)
    {
      if(sendstr[i] == '\x1B')
      {
        return 0;
      }
    }
      
      
    if(len >= 2) {
        if( (sendstr[len-1] != '\n') && (sendstr[len-2] !='\r'))
        {
          strncat((char*)sendstr, "\r\n", 2);
          len += 2;
        }
        FLOW_Write((char *)sendstr, len);
        
        /** Reset the Console buffer */
        memset((char *)ConsoleRxBuffer, 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
        pConsoleRxBuffer = &ConsoleRxBuffer[0];
        memset((char *)CommandRxBuffer, 0, SYSTEM_COMMAND_RX_BUFFER_LEN);
        pCommandRxBuffer = &CommandRxBuffer[0];
    }

    char rcv[32];
    uint8_t rlen = 0;
    
    do{
      
        FLOW_Read(rcv, &rlen);
        rcv[rlen]=0;
        if(rlen != 0)
        {
          for(uint8_t i=0;i<rlen;i++)
          {
            if(rcv[i]<0x20)
            {
              if(rcv[i] == 0x06) {
                CONSOLEIO_SendStr(pUART, "<ACK> ");
              }else if(rcv[i] == 0x15) {
                CONSOLEIO_SendStr(pUART, "<NAK> ");
              }
              if(rcv[i] != 0x0A || rcv[i] != 0x0D)
              {
                rcv[i] = ' ';
              }
            }
          }
         CONSOLEIO_SendStr(pUART, rcv);
         
        }
    }while(rlen > 0);
  }
}


STATIC eConsoleCmdStatus_t CONSOLECMD_LicorPassthrough(sUART_t *pUART, const char buffer[])
{
  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Power the Licor On */
  if (verify_test_mode(pUART, sysinfo.Submode) != true)
  {
    CONSOLECMD_TestMode(pUART, buffer);
    Log("Test Mode Set", LOG_Priority_High);
  }
  
  _delay_ms(500);

  /**  Put UART In Passthrough */
  UartPassthrough = SYS_UART_LICOR_PASSTHROUGH;

  /** Reset the Console buffer */
  memset((char *)ConsoleRxBuffer, 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
  pConsoleRxBuffer = &ConsoleRxBuffer[0];
  
  /** Reset the Command buffer */
  memset((char *)CommandRxBuffer, 0, SYSTEM_COMMAND_RX_BUFFER_LEN);
  pCommandRxBuffer = &CommandRxBuffer[0];

  /** Reset the Licor Buffer */
  memset((char *)LicorRxBuffer, 0, LICOR_BUFFER_LEN);
  pLicorRxBuffer = &LicorRxBuffer[0];
  LicorRxFlag = false;

  while (1)
  {
    BSP_Hit_WD();
    if ((strchr((char *)ConsoleRxBuffer, '\n') != NULL) ||
        (strchr((char *)CommandRxBuffer, '\n') != NULL) )
    {
      /** Write the string */
      uint16_t len =0;
      CONSOLEIO_Receive(pUART, sendstr, 256, &len);
      LI8x0_SendString((char *)sendstr, len);

      /** Reset the Console buffer */
      memset((char *)ConsoleRxBuffer, 0, SYSTEM_CONSOLE_RX_BUFFER_LEN);
      pConsoleRxBuffer = &ConsoleRxBuffer[0];
      memset((char *)CommandRxBuffer, 0, SYSTEM_COMMAND_RX_BUFFER_LEN);
      pCommandRxBuffer = &CommandRxBuffer[0];
    }

    if (LicorRxFlag == true)
    {
      CONSOLEIO_SendStr(pUART, (char *)LicorRxBuffer);
      /** Reset the Licor Buffer */
      memset((char *)LicorRxBuffer, 0, LICOR_BUFFER_LEN);
      pLicorRxBuffer = &LicorRxBuffer[0];
      LicorRxFlag = false;
    }
  }
}

/** @brief Check Write Command
 * 
 * Checks the buffer for a "= " (equals sign and space) to determine
 * if it is a write string or 
 * 
 * @param buffer Pointer to buffer 
 * 
 * @return result (write=true or read=false)
 */
STATIC bool is_write_cmd(const char buffer[])
{
  bool result = false;

  if (('=' == buffer[0]) && (' ' == buffer[1]))
  {
    result = true;
  }

  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_TerminalMode(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Set the Mode to Terminal */
  ConsoleMode = SYS_Console_Terminal;

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_Time(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  char timestr[32];
  memset(timestr, 0, 32);

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Get the Time */
  AM08X5_GetTimestamp_ISO8601(timestr);

  /** Send the time */
  CONSOLEIO_SendStr(pUART, timestr);
  CONSOLEIO_SendStr(pUART, STR_ENDLINE);

  /** Set the Mode to Hotkey */
  ConsoleMode = SYS_Console_Hotkey;

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_SamplingMode(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Set the Mode to Hotkey */
  ConsoleMode = SYS_Console_Hotkey;

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_SamplingSchedule(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Set the Mode to Hotkey */
  ConsoleMode = SYS_Console_Hotkey;

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_HOTKEY_ReturnToSleep(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** Set the Valves to AIR Mode */

  /** Set the Mode to Hotkey */
  ConsoleMode = SYS_Console_Hotkey;

  result = CONSOLECMD_OK;
  return result;
}

STATIC eConsoleCmdStatus_t CONSOLECMD_Quit(sUART_t *pUART, const char buffer[])
{
  eConsoleCmdStatus_t result = CONSOLECMD_FAIL;

  /** Ingore other strings in the buffer */
  IGNORE_UNUSED_VARIABLE(buffer);

  /** */
  ConsoleMode = SYS_Console_None;

  result = CONSOLECMD_OK;
  return result;
}

STATIC bool verify_test_mode(sUART_t *pUART, eConsoleSubmode_t mode)
{
  bool status = false;
  if (mode != SYS_Submode_Test)
  {
    Log("Must be in TEST mode\r\n", LOG_Priority_High);
    status = false;
  }
  else
  {
    status = true;
  }

  return status;
}

const sConsoleCommandTable_t *CONSOLECMD_GetTable(void)
{
  return &mConsoleCommandTable[0];
}

const sConsoleHotkeyTable_t *CONSOLECMD_GetHotkeys(void)
{
  return &mConsoleHotkeyTable[0];
}