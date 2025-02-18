/** @file LI8x0.h
 *  @brief LIcor 800 series drivers
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 15, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note Licor LI-820/830 Drivers
 *  @note
 *
 *  @bug  No known bugs
 */
#ifndef _LI8X0_H
#define _LI8X0_H

/** Remove STATIC and PERSISTENT values if running TEST */
/** Add the actual values if running release */
#ifdef UNIT_TESTING
#ifndef STATIC
#define STATIC
#endif
#ifndef PERSISTENT
#define PERSISTENT
#endif
#else
#ifndef STATIC
#define STATIC static
#endif
#ifndef PERSISTENT
#define PERSISTENT __persistent
#endif
#endif

/************************************************************************
 *						STANDARD LIBRARIES
 ************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef UNIT_TESTING
#include <stdio.h>
#endif
/************************************************************************
 *							HEADER FILES
 ************************************************************************/
#include "bsp.h"
#include "sysinfo.h"
#include "AM08X5.h"
#include "logging.h"
#include "errorlist.h"
/************************************************************************
 *							MACROS
 ************************************************************************/

/************************************************************************
 *							Structs & Enums
 ************************************************************************/
/**
 *  @enum Licor Status
 *  @brief Licor Status
 */
typedef enum
{
    LICOR_OK = 0u,
    LICOR_FAIL = 1u
} eLicorStatus_t;

/**
 *  @enum Licor Type
 *  @brief Licor Type
 */
typedef enum
{
    LICOR_LI820 = 0x01, /**< Licor Li-820 (Old Style) */
    LICOR_LI830 = 0x02, /**< Licor Li-830 (New Style) */
    LICOR_LI840 = 0x03, /**< Licor Li-840 (Old Style, with H20) */
    LICOR_LI850 = 0x04  /**< Licor LI-850 (New Styell, with H20) */
} eLicor_t;

/**
 *  @enum Licor XML Level 1
 *  @brief Licor XML Level 1
 */
typedef enum
{
    LICOR_XML_LVL1_NONE = 0x00,
    LICOR_XML_LVL1_820 = LICOR_LI820,
    LICOR_XML_LVL1_830 = LICOR_LI830,
    LICOR_XML_LVL1_840 = LICOR_LI840,
    LICOR_XML_LVL1_850 = LICOR_LI850
} eLicorXMLLvl1_t;

/**
 *  @enum Licor XML Level 2
 *  @brief Licor XML Level 2
 */
typedef enum
{
    LICOR_XML_LVL2_NONE = 0x00,
    LICOR_XML_LVL2_DATA = 0x01,
    LICOR_XML_LVL2_AUXDATA = 0x02,
    LICOR_XML_LVL2_RS232 = 0x03,
    LICOR_XML_LVL2_CFG = 0x04,
    LICOR_XML_LVL2_CAL = 0x05,
    LICOR_XML_LVL2_POLY = 0x06,
    LICOR_XML_LVL2_PUMP = 0x07,
    LICOR_XML_LVL2_SOURCE = 0x08,
    LICOR_XML_LVL2_TECH = 0x09,
    LICOR_XML_LVL2_SER = 0x0A,
    LICOR_XML_LVL2_VER = 0x0B,
    LICOR_XML_LVL2_ACK = 0x0C,
    LICOR_XML_LVL2_ERROR = 0x0D
} eLicorXMLLvl2_t;

/**
 *  @enum Licor XML Lvel 3
 *  @brief Licor XML Level 3
 */
typedef enum
{
    LICOR_XML_LVL3_NONE = 0x00,
    LICOR_XML_LVL3_FLOWRATE = 0x01,
    LICOR_XML_LVL3_CELLTEMP = 0x02,
    LICOR_XML_LVL3_CELLPRES = 0x03,
    LICOR_XML_LVL3_CO2 = 0x04,
    LICOR_XML_LVL3_CO2ABS = 0x05,
    LICOR_XML_LVL3_CO2REF = 0x06,
    LICOR_XML_LVL3_H2O = 0x07,
    LICOR_XML_LVL3_H2OABS = 0x08,
    LICOR_XML_LVL3_H2ODEW = 0x09,
    LICOR_XML_LVL3_IVOLT = 0x0A,
    LICOR_XML_LVL3_RAW = 0x0B,
    LICOR_XML_LVL3_PCA = 0x10,
    LICOR_XML_LVL3_PHA = 0x11,
    LICOR_XML_LVL3_BB = 0x12,
    LICOR_XML_LVL3_PSI = 0x13,
    LICOR_XML_LVL3_ECHO = 0x20,
    LICOR_XML_LVL3_STRIP = 0x21,
    LICOR_XML_LVL3_HEATER = 0x40,
    LICOR_XML_LVL3_PCOMP = 0x41,
    LICOR_XML_LVL3_FILTER = 0x42,
    LICOR_XML_LVL3_OUTRATE = 0x43,
    LICOR_XML_LVL3_ALARMS = 0x44,
    LICOR_XML_LVL3_BENCH = 0x45,
    LICOR_XML_LVL3_SPAN = 0x46,
    LICOR_XML_LVL3_DACS = 0x47,
    LICOR_XML_LVL3_CO2ZERO = 0x50,
    LICOR_XML_LVL3_CO2SPAN = 0x51,
    LICOR_XML_LVL3_CO2SPAN2 = 0x52,
    LICOR_XML_LVL3_CO2LASTZERO = 0x53,
    LICOR_XML_LVL3_CO2KZERO = 0x54,
    LICOR_XML_LVL3_CO2LASTSPAN = 0x55,
    LICOR_XML_LVL3_CO2LASTSPAN2 = 0x56,
    LICOR_XML_LVL3_CO2KSPAN = 0x57,
    LICOR_XML_LVL3_CO2KSPAN2 = 0x58,
    LICOR_XML_LVL3_H2OZERO = 0x60,
    LICOR_XML_LVL3_H2OSPAN = 0x61,
    LICOR_XML_LVL3_H2OSPAN2 = 0x62,
    LICOR_XML_LVL3_H2OLASTZERO = 0x63,
    LICOR_XML_LVL3_H2OLASTSPAN = 0x64,
    LICOR_XML_LVL3_H2OLASTSPAN2 = 0x65,
    LICOR_XML_LVL3_H20KZERO = 0x66,
    LICOR_XML_LVL3_H2OKSPAN = 0x67,
    LICOR_XML_LVL3_H2OKSPAN2 = 0x68,
    LICOR_XML_LVL3_DATE = 0x70,
    LICOR_XML_LVL3_XS = 0x72,
    LICOR_XML_LVL3_RESET = 0x73,
    LICOR_XML_LVL3_PRESS = 0x75,
    LICOR_XML_LVL3_ENABLED = 0x80,
    LICOR_XML_LVL3_TIME = 0x81,
    LICOR_XML_LVL3_DRIVE = 0x82,
    LICOR_XML_LVL3_STATUS = 0x84,
    LICOR_XML_LVL3_PUMP = 0x90,
    LICOR_XML_LVL3_SOURCE = 0x91,
    LICOR_XML_LVL3_SERIALNUM = 0x92

} eLicorXMLLvl3_t;

/**
 *  @enum Licor XML Lvel 4
 *  @brief Licor XML Level 3
 */
typedef enum
{
    LICOR_XML_LVL4_NONE = 0x00,
    LICOR_XML_LVL4_CO2 = 0x01,
    LICOR_XML_LVL4_CO2REF = 0x02,
    LICOR_XML_LVL4_H2O = 0x03,
    LICOR_XML_LVL4_H2OREF = 0x04,
    LICOR_XML_LVL4_LOGIC = 0x10,
    LICOR_XML_LVL4_SOURCE = 0x11,
    LICOR_XML_LVL4_ENABLED = 0x12,
    LICOR_XML_LVL4_HIGH = 0x13,
    LICOR_XML_LVL4_HDEAD = 0x14,
    LICOR_XML_LVL4_LOW = 0x15,
    LICOR_XML_LVL4_LDEAD = 0x16,
    LICOR_XML_LVL4_RANGE = 0x20,
    LICOR_XML_LVL4_D1 = 0x21,
    LICOR_XML_LVL4_D1_0 = 0x22,
    LICOR_XML_LVL4_D1_F = 0x23,
    LICOR_XML_LVL4_D2 = 0x24,
    LICOR_XML_LVL4_D2_0 = 0x25,
    LICOR_XML_LVL4_D2_F = 0x26,
    LICOR_XML_LVL4_A0 = 0x30,
    LICOR_XML_LVL4_A1 = 0x31,
    LICOR_XML_LVL4_A2 = 0x32,
    LICOR_XML_LVL4_A3 = 0x33,
    LICOR_XML_LVL4_A4 = 0x34,
    LICOR_XML_LVL4_MODEL = 0x40,
    LICOR_XML_LVL4_KI = 0x41,
    LICOR_XML_LVL4_KP = 0x42,
    LICOR_XML_LVL4_KD = 0x43,
    LICOR_XML_LVL4_OFFSET = 0x44,
    LICOR_XML_LVL4_GAIN = 0x45,
    LICOR_XML_LVL4_FLOW = 0x46,
    LICOR_XML_LVL4_VOLTAGE = 0x50
} eLicorXMLLvl4_t;

/**
 *  @struct XML Command Structure
 *  @brief XML Command Structure
 */
typedef struct
{
    eLicorXMLLvl1_t LVL1; /**< Level 1 */
    eLicorXMLLvl2_t LVL2; /**< Level 2 */
    eLicorXMLLvl3_t LVL3; /**< Level 3 */
    eLicorXMLLvl4_t LVL4; /**< Level 4 */
} sLicorXML_t;

/**
 *  @struct Licor Data Structure
 *  @brief All elements from returned LiCor Data
 */
typedef struct
{
    float co2;         /**< CO2 in ppm */
    float co2abs;      /**< CO2 Absorbption */
    float h2o;         /**< H2O in mmol/mol */
    float h2odewpoint; /**< H2O in degC */
    float h2oabs;      /**< H2O absorbtion */
    float celltemp;    /**< Cell temperature */
    float cellpres;    /**< Cell Pressure */
    float ivolt;       /**< Input Voltage */
    float flowrate;    /**< Measured flow rate */
    struct
    {
        int32_t co2;    /**< Raw CO2 detector reading */
        int32_t co2ref; /**< Raw CO2 reference reading */
        int32_t h2o;    /**< Raw H2O detector reading */
        int32_t h2oref; /**< Raw H2O reference reading */
    } raw;
} sLicorData_t; /**< Raw Data */

/**
 * @struct Auxiliary Data
 * @brief Auxiliary Data
 */
typedef struct
{
    float pca;     /**< Auxiliary Data PCA */
    float pha;     /**< Auxiliary Data PHA */
    float psi;     /**< Auxiliary Data PSI */
    float bb_eff;  /**< Auxiliary Data BB_EFF */
} sLicorAuxdata_t; /**< Auxiliary Data */

/**
 * @struct Licor RS232 Data
 * @brief Licor RS232 Data
 */
typedef struct
{
    uint16_t co2 : 1;         /**< CO2 Output */
    uint16_t co2abs : 1;      /**< CO2 Absorbtion Output */
    uint16_t h2o : 1;         /**< H2O Output */
    uint16_t h2odewpoint : 1; /**< H2O Dewpoint Output */
    uint16_t h2oabs : 1;      /**< H2O Absorption Output */
    uint16_t celltemp : 1;    /**< Cell Temperature Output */
    uint16_t cellpres : 1;    /**< Cell Pressure Output */
    uint16_t ivolt : 1;       /**< Input Voltage Output */
    uint16_t echo : 1;        /**< Echo commands Output */
    uint16_t strip : 1;       /**< Strip XML Output */
    uint16_t flowrate : 1;    /**< Flow rate Output */
    struct
    {
        uint16_t co2 : 1;    /**< Raw CO2 detector reading */
        uint16_t co2ref : 1; /**< Raw CO2 reference reading */
        uint16_t h2o : 1;    /**< Raw H2O detector reading */
        uint16_t h2oref : 1; /**< Raw H2O reference reading */
    } raw;
} sLicorRs232_t; /**< Select which display in <data> call */

/**
 * @struct Licor Alarms
 * @brief Licor Alarms
 */
typedef struct
{
    enum
    {
        TTL = 0x01,
        SWG = 0x02
    } logic;
    uint8_t enabled : 1; /**< Enable Alarms */
    enum
    {
        CO2 = 0x01, /**< Channel CO2 Alarm */
        H2O = 0x02  /**< Channel H2O Alarm */
    } sources;
    float high;  /**< High alarm on value */
    float hdead; /**< High alarm off value */
    float low;   /**< Low alarm on value */
    float ldead; /**< Low alarm off value */
} sLicorAlarms_t;

/**
 * @struct Licor Dac
 * @brief Licor Dac
 */
typedef enum
{
    LICOR_DAC_CO2 = 0x01,
    LICOR_DAC_H2O = 0x02,
    LICOR_DAC_H2ODEWPOINT = 0x03,
    LICOR_DAC_CELLPRES = 0x04,
    LICOR_DAC_CELLTEMP = 0x05,
    LICOR_DAC_NONE = 0x00
} eLicorD_t;

typedef enum
{
    R_2_5, /**< Output voltage 0 - 2.5V */
    R_5_0  /**< Output voltage 0 - 5.0V */
} eLicorDACRange_t;

/**
 * @struct Licor DAC
 * @brief Licor DAC
 */
typedef struct
{
    eLicorDACRange_t range;
    eLicorD_t d1; /**< DAC1 Setting */
    float d1_0;   /**< Value where DAC1 outputs 0V */
    float d1_f;   /**< Value where DAC1 outputs full scale */
    eLicorD_t d2; /**< DAC2 Settings */
    float d2_0;   /**< Value where DAC2 outptus 0V */
    float d2_f;   /**< Value where DAC2 outputs full scale */
} sLicorDAC_t;

typedef enum
{
    LICOR_PUMP_ONLY = 0x00,      /**< Pump Present */
    LICOR_FLOW_ONLY = 0x01,      /**< Flow Controller Present */
    LICOR_PUMP_AND_FLOW = 0x02,  /**< Pump and FLow Controller Present */
    LICOR_NO_PUMP_OR_FLOW = 0x03 /**< Neither Pump or Flow Contorller */
} eLicorPumpStatus_t;

/**
 * @struct Licor Pump
 * @brief Licor Brief
 */
typedef struct
{
    uint8_t enabled : 1; /**< Turns pump on/off */
    uint32_t time;       /**< Number of hours pump has run */
    uint8_t drive;       /**< Percent of total drive power */
    eLicorPumpStatus_t status;
} sLicorPump_t;

/**
 * @struct Licor Runtime
 * @brief Licor Runtime (Not currently implemented)
 */
typedef struct
{
    int32_t time; /**< Number of hours of source operation */
} sLicorSource_t;

/**
 * @struct Licor Tech
 * @brief Licor Tech
 */
typedef struct
{
    struct
    {
        char model[16]; /**< Pump Model */
        int32_t ki;     /**< Integer value of pump PID */
        int32_t kp;     /**< Proportional value of pump PID */
        int32_t kd;     /**< Derivative value of pump PID */
        int32_t offset; /**< Pump control offset */
        int32_t gain;   /**< Pump control gain */
        float flow;     /**< Flow setting in LPM (0 - 1.0LPM) */
    } pump;
    struct
    {
        float voltage; /**< Source voltage settings (0 - 5.0V) */
    } source;
} sLicorTech_t;

/**
 * @struct Licor Configurations
 * @brief Licor Configurations
 */
typedef struct
{
    float outrate;  /**< Output Data every N seconds 0.5 - 20 */
    uint8_t heater; /**< Heater On/Off */
    uint8_t pcomp;  /**< Pressure Compensation On/Off */
    uint8_t filter; /**< Set a 0 - 20 second filter */
    uint8_t bench;  /**< Optical Bench Length */
    uint8_t spn1;
    uint8_t span;
    sLicorAlarms_t alarms; /**< Alarm tags */
    sLicorDAC_t dacs;      /**< DAC Tags */
} sLicorCfg_t;

/**
 * @struct Licor Calibrations
 * @brief Licor Calibrations
 */
typedef struct
{
    char date[20];         /**< Current date (20 char) */
    uint8_t h2ozero : 1;   /**< Start H2O Zero */
    float h2ospan;         /**< Start the H2O Span 1 */
    float h2ospan2;        /**< Start the H2O Span 2 */
    char h2olastzero[20];  /**< Date of last H2O zero */
    char h2olastspan[20];  /**< Date of last H2O span */
    char h2olastspan2[20]; /**< Date of last H2O span 2 */
    float h2okzero;        /**< H2O Zero Calibration Constant */
    float h2okspan;        /**< H2O Span 1 Calibration Constant */
    float h2okspan2;       /**< H2O Span 2 Calibration Constant */
    uint8_t co2zero : 1;   /**< Start CO2 Zero */
    float co2span;         /**< Start the Co2 span 1 */
    float co2span2;        /**< Start the CO2 span 2 */
    char co2lastzero[20];  /**< Date of last CO2 zero */
    char co2lastspan[20];  /**< Date of last CO2 span */
    char co2lastspan2[20]; /**< Date of last CO2 span 2 */
    float co2kzero;        /**< CO2 Zero Calibration Constant */
    float co2kspan;        /**< CO2 Span 1 Calibration Constant */
    float co2kspan2;       /**< CO2 Span 2 Calibration Constant */
} sLicorCal_t;

/**
 * @struct Licor Polynomial
 * @brief Licor Polynomial
 */
typedef struct
{
    char date[20];     /**< Serial Num & Factory Cal date HGA XXXX yyyy-mm-dd */
    float bb;          /**< Band broadening value */
    float xs;          /**< Cross sensitivity value */
    uint8_t reset : 1; /**< Reset ??? */
    struct
    {
        float a1; /**< Floating point coefficients for Co2 cal inverse rectangular hyperbola */
        float a2; /**< Floating point coefficients for Co2 cal inverse rectangular hyperbola */
        float a3; /**< Floating point coefficients for Co2 cal inverse rectangular hyperbola */
        float a4; /**< Floating point coefficients for Co2 cal inverse rectangular hyperbola */
    } co2;
    struct
    {
        float a1; /**< Floating point coefficient for H2O cal polynomial */
        float a2; /**< Floating point coefficient for H2O cal polynomial */
        float a3; /**< Floating point coefficient for H2O cal polynomial */
    } h2o;
    struct
    {
        float a0; /**< Floating point coefficient for Co2 pressure correction */
        float a1; /**< Floating point coefficient for Co2 pressure correction */
    } pres;
} sLicorPolynomial_t;

/**
 * @struct Licor Tags
 * @brief Licor Tags
 */
typedef struct
{
    sLicorData_t data;       /**< Data Fields */
    sLicorAuxdata_t auxdata; /**< Auxiliary Data Fields */
    sLicorRs232_t rs232;     /**< RS232 Output Data Fields */
    sLicorCfg_t cfg;         /**< Configuration */
    sLicorCal_t cal;         /**< Calibration */
    sLicorPolynomial_t poly; /**< Polynomial */
    sLicorPump_t pump;       /**< Pump status */
    sLicorSource_t source;   /**< Hours the source has operated */
    sLicorTech_t tech;       /**< Diagnostics */
    char serialnum[32];      /**< Licor Serial Number */
    char ver[16];            /**< Licor version number */
    uint8_t ack : 1;         /**< Acknowledgement of command */
    char error[128];         /**< Error, includes a message */
} sLicorTags_t;

/**
 * @struct Licor Power State
 * @brief Licor Power State
 */
typedef struct
{
    sGPIO_t pin;     /**< Power Pin Struct */
    bool state;      /**< false = OFF, true = ON */
    sTimeReg_t time; /**< Time Power was turned ON */
} sLicorPowerState_t;

/**
 * @struct Licor Structure
 * @brief Licor Structure
 */
typedef struct
{
    sLicorPowerState_t Power; /**< Licor Power Pin Struct */
    sUART_t UART;             /**< Licor UART Structure */
    eLicor_t type;            /**< Type of Licor Connected */
    sLicorTags_t sysconfig;   /**< Configuration to set in Licor */
    sLicorTags_t readconfig;  /**< Current Licor Settings */
} sLicor_t;

#ifdef UNIT_TESTING
extern sLicor_t *pLicor1;
extern char LicorBuffer[LICOR_BUFFER_LEN];
extern char *pLicorRxBuffer;
#endif
/************************************************************************
 *					   Functions Prototypes
 ************************************************************************/
/** @brief LI-8x0 Power ON
 *
 * Apply power to the Licor
 *
 * @param None
 *
 * @return None
 */
void LI8x0_PowerON(void);

/** @brief LI-8x0 Power OFF
 *
 * Remove power to the Licor
 *
 * @param None
 *
 * @return None
 */
void LI8x0_PowerOFF(void);

/** @brief Get Licor Power State
 *
 * Get the current power state of the Licor
 *
 * @return Power State (0 = OFF, 1 = ON)
 */
bool LI8x0_GetPower(void);

/** @brief Initialize LiCor LI-8x0
 *
 * Initialize the Power Pin and UART to LiCor LI-8x0
 *
 *  @param None
 *
 *  @return None
 */
eLicorStatus_t LI8x0_Init(void);

/** @brief Configure the Licor LI-8x0
 *
 * Configure the Licor to the user settings
 *
 * @param None
 *
 * @return None
 */
eLicorStatus_t LI8x0_Config(void);

/** @brief Get the Licor Serial #
 *
 *
 *
 * @param *str Pointer to string to write to
 *
 * @return Status
 */
eLicorStatus_t LI8x0_Get_SerialNumber(char *str);

/** @brief Get the Licor Firmware Version #
 *
 *
 *
 * @param *str Pointer to string to write to
 *
 * @return Status
 */
eLicorStatus_t LI8x0_Get_Version(char *str);

/** @brief Get the Licor CO2 Poly A1 coefficient
 * 
 * @param *str Pointer to string to write to
 *
 * @return None
 */
void LI8x0_Get_ccva1_1(char *str);

/** @brief Get the Licor CO2 Poly A2 coefficient
 * 
 * @param *str Pointer to string to write to
 *
 * @return None
 */
void LI8x0_Get_ccva1_2(char *str);

/** @brief Get the Licor CO2 Poly A3 coefficient
 * 
 * @param *str Pointer to string to write to
 *
 * @return None
 */
void LI8x0_Get_ccva1_3(char *str);

/** @brief Get the Licor CO2 Poly A4 coefficient
 * 
 * @param *str Pointer to string to write to
 *
 * @return None
 */
void LI8x0_Get_ccva1_4(char *str);


/** @brief Get the Licor zero coefficient
 *
 *
 *
 * @param *str Pointer to string to write to
 *
 * @return Status
 */
eLicorStatus_t LI8x0_Get_Zero(char *str);
float LI8x0_Get_KZero_float(void);

/** @brief Get the Licor span coefficient
 *
 *
 *
 * @param *str Pointer to string to write to
 *
 * @return Status
 */
eLicorStatus_t LI8x0_Get_Span(char *str);
float LI8x0_Get_KSpan_float(void);

/** @brief Get the Licor span2 coefficient
 *
 *
 *
 * @param *str Pointer to string to write to
 *
 * @return Status
 */
eLicorStatus_t LI8x0_Get_Span2(char *str);
float LI8x0_Get_KSpan2_float(void);

eLicorStatus_t LI8x0_Set_Zero(float val);

/** @brief Create Tags String
 *
 * Create a XML tag string (start and end) from
 *
 *  @param XML XML
 *  @param *sstr Pointer to send string (start tags)
 *  @param *estr Pointer to end string (end tags )
 *
 *  @return Status
 */
eLicorStatus_t LI8x0_CreateTags(sLicorXML_t *XML,
                                char *sstr,
                                uint16_t *slen,
                                char *estr,
                                uint16_t *elen);

/** @brief Auto Discover Licor Model
 *
 * Auto-Discover which model of Licor is being used
 *
 * @param None
 *
 * @return None
 */
eLicorStatus_t LI8x0_AutoDiscover(void);

/** @brief Create System Information String
 *
 * Create the Licor System Information String
 *
 * @param Pointer to sysinfo string
 * @param Pointer to length of string
 *
 * @return None
 */
void LI8x0_CreateString_SysInfo(char *sstr, uint16_t *len);

/** @brief Licor Gas Zero Calibration
 *
 * Calibrate the Licor with the zero gase
 *
 * @param None
 *
 * @return result
 */
eLicorStatus_t LI8x0_Zero(void);

/** @brief Span Gas Calibration
 *
 * Calibrate the Licor with the span gas
 *
 * @param span Concentration of CO2 (ppm)
 *
 * @return result
 */
eLicorStatus_t LI8x0_Span(float span);

/** @brief Span 2 Gas Calibration
 *
 * Calibrate the Licor with the Span 2 gas
 *
 * @param span Concentration of CO2 (ppm)
 *
 * @return result
 */
eLicorStatus_t LI8x0_Span2(float span);

/** @brief Clear the char buffer
 *
 * Clear the Licor char array buffer
 *
 * @param None
 *
 * @return None
 */
void LI8x0_ClearBuffer(void);

/** @brief Sample the Licor
 *
 * Run the Licor Sample routine to measure CO2 in ppm
 *
 * @param None
 *
 * @return result
 */
eLicorStatus_t LI8x0_Sample(void);
/** @brief Query Licor
 *
 * THIS IS THE SEND-TO-LICOR COMMAND ONLY!
 * MUST BE USED IN CONJUNCTION WITH LI8x0_Sample_Read(*data)
 * This call sends a request to the Licor to return data
 *
 * @param None
 *
 * @return result
 */
eLicorStatus_t LI8x0_Sample_Query(void);

/** @brief Read Licor
 *
 * MUST BE USED AFTER LI8x0_Sample_Query() IS CALLED!
 * Reads the Licor buffer for the data response
 *
 * @param *data Pointer to data structure
 *
 * @return result
 */
eLicorStatus_t LI8x0_Sample_Read(sLicorData_t *data);
/** @brief Read Licor Sample
 *
 * Read Licor Data after sample
 *
 * @param *data Pointer to Licor Data Structure
 *
 * @return result
 */
eLicorStatus_t LI8x0_ReadSample(sLicorData_t *data);

/** @brief Clear Licor Data Struct
 *
 * Clears licor data structure (uses NAN)
 *
 * @param *data Pointer to data struct
 *
 * @return None
 */
void LI8x0_Clear_Data(sLicorData_t *data);

/** @brief Licor Heater State
 *
 * Set Licor Heater state
 *
 * @param state (0 = OFF, 1 = ON)
 *
 * @return result
 */
eLicorStatus_t LI8x0_Heater(uint8_t state);

/** @brief Send string to Licor
 *
 * Send String over UART to Licor
 *
 * @param *sstr Pointer to string to send
 * @param strlen Number of characters in string
 *
 * @return None
 */
void LI8x0_SendString(char *sstr, uint16_t strlen);

/** @brief Get Licor Cal Data
 *
 * Request Licor calibration date from Licor.  Populates Licor Structure.
 *
 * @param None
 *
 * @return result
 */
eLicorStatus_t LI8x0_GetCal(void);

/** @brief Create Error String
 *
 * Create a string from the errors given by the Licor/
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Error(char *sstr, uint8_t indentLevel);

/** @brief Create Data String
 *
 * Create a string from the data received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Data(char *sstr, uint8_t indentLevel);

/** @brief Create Auxdata String
 *
 * Create a string from the auxdate received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Auxdata(char *sstr, uint8_t indentLevel);

/** @brief Create RS232 String
 *
 * Create a string from the RS232 settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_RS232(char *sstr, uint8_t indentLevel);

/** @brief Create Configuration String
 *
 * Create a string from the Configuration settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Cfg(char *sstr, uint8_t indentLevel);

/** @brief Create Calibration String
 *
 * Create a string from the Calibration settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Cal(char *sstr, uint8_t indentLevel);

/** @brief Create Polynomial String
 *
 * Create a string from the polynomials received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Poly(char *sstr, uint8_t indentLevel);

/** @brief Create Pump String
 *
 * Create a string from the Pump Settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Pump(char *sstr, uint8_t indentLevel);

/** @brief Create Source String
 *
 * Create a string from the Source Settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Source(char *sstr, uint8_t indentLevel);

/** @brief Create Tech String
 *
 * Create a string from the Tech Settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Tech(char *sstr, uint8_t indentLevel);

/** @brief Create Serial String
 *
 * Create a string from the Serial Settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateString_Serial(char *sstr, uint8_t indentLevel);

/** @brief Create DAC String
 *
 * Create a string from the DAC settings received from Licor
 *
 * @param *sstr Pointer to string to send
 * @param indentLevel XML level.
 *
 * @return None
 */
void LI8x0_CreateSubstring_DAC(char *sstr, eLicorD_t D, uint8_t indentLevel);

/** @brief Get Licor Info
 *
 * Return all Licor status info.  Equivalent to sending ?
 *
 * @param None
 *
 * @return result
 */
eLicorStatus_t LI8x0_GetInfo(void);

eLicorStatus_t LI8x0_Set_Span(float val);
eLicorStatus_t LI8x0_Set_Span2(float val);

#ifdef UNIT_TESTING
STATIC eLicorStatus_t LI8x0_ValidateXML(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_ValidXML_Level2(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_ValidXML_Level3(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Validate_NoPump(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Validate_NoH2O(sLicorXML_t *XML);
STATIC eLicorStatus_t LI8x0_Level1Str(eLicorXMLLvl1_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);
STATIC eLicorStatus_t LI8x0_Level2Str(eLicorXMLLvl2_t LVL,
                                      char *sstr,
                                      uint16_t *slen,
                                      char *estr,
                                      uint16_t *elen);

#endif

#endif // _LI8X0_H
