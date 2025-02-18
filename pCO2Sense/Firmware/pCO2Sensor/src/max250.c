/** @file max250.c
 *  @brief MAX250 O2 Sensor drivers
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date January 15, 2018
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *  @note MAX250 Configuration drivers
 *  @note
 *
 *  @bug  No known bugs
 */
#include "max250.h"

PERSISTENT sMAX250_t MAX250 = {
    .Analog = {
        .param = {
            .sampleHoldSignalSourceSelect = O2_ADC_SRC,
            .clockSourceSelect = O2_ADC_CLK_SRC,
            .clockSourceDivider = O2_ADC_CLK_DIVIDER,
            .clockSourcePredivider = O2_ADC_CLK_PREDIVIDER,
            .internalChannelMap = O2_ADC_INT_CHN_MAP},
        .config = {.memoryBufferControlIndex = O2_ADC_BUF_IDX, .inputSourceSelect = O2_ADC_SRC_SEL, .refVoltageSourceSelect = O2_ADC_REF_VOLT_SRC_SEL, .endOfSequence = O2_ADC_EOS, .windowComparatorSelect = O2_ADC_WIN_COMP_SEL, .differentialModeSelect = O2_ADC_DIFF_MODE_ENABLE},
        .baseAddress = ADC12_B_BASE,
        .configured = false,
        .enabled = false,
        .interrupt = false,
        .refVoltage = O2_ADC_REF_VOLTAGE},
    .Pin = {.port = O2_ANALOG_PORT, .pin = O2_ANALOG_PIN, .mode = O2_ANALOG_DIR, .inverted = O2_ANALOG_POL},
    .Power = {.port = O2_POWER_PORT, .pin = O2_POWER_PIN, .mode = O2_POWER_DIR, .inverted = O2_POWER_POL},
    .O2 = NAN,
    .cal = MAX250_CAL_DEFAULT,
    .zero = MAX250_MIN_VOLTAGE,
    .max = MAX250_MAX_VOLTAGE};

/************************************************************************
 *					   Static Functions Prototypes
 ************************************************************************/
void MAX250_CalculateO2(float *v, float *data);
/************************************************************************
 *					   Global Functions
 ************************************************************************/

void MAX250_Init(void)
{
    Log("O2 Initialize", LOG_Priority_High);

    /** Initialize the VREF Pin */
    BSP_GPIO_SetPin(&MAX250.Power);

    /** Initialize the Analog Port */
    BSP_ANALOG_Init(&MAX250.Analog);
}

float MAX250_Read(void)
{
    float voltage = 0.0;
    float o2val = 0.0;
    float temp = 0;
    const uint8_t numSamples = 5;
    /** Read the Analog Pin 5 times */
    uint8_t i;

    if (MAX250.RefOn == false)
    {
        MAX250_PowerON();
        _delay_ms(5);
    }

    for (i = 0; i < numSamples; i++)
    {
        voltage = BSP_ANALOG_ReadVoltage(&MAX250.Analog);
        MAX250_CalculateO2(&voltage, &temp);
        o2val += temp;
    }

    o2val /= numSamples;

    return o2val;
}

float MAX250_ReadVoltage(void)
{
    if (MAX250.RefOn == false)
    {
        MAX250_Init();
        MAX250_PowerON();
        _delay_ms(5);
    }

    float voltage = 0.0;
    const uint8_t numSamples = 5;
    uint8_t i;

    for (i = 0; i < numSamples; i++)
    {
        voltage += BSP_ANALOG_ReadVoltage(&MAX250.Analog);
    }

    voltage /= numSamples;

    return voltage;
}

void MAX250_SetCal(float cal)
{
    MAX250.cal = cal;
    return;
}
float MAX250_GetCal(void)
{
    return MAX250.cal;
}
void MAX250_PowerON(void)
{
    Log("O2 On", LOG_Priority_High);
    MAX250.RefOn = true;
    BSP_GPIO_SetPin(&MAX250.Power);
    return;
}

void MAX250_PowerOFF(void)
{
    Log("O2 Off", LOG_Priority_High);
    MAX250.RefOn = false;
    BSP_GPIO_ClearPin(&MAX250.Power);
    return;
}
/************************************************************************
 *					   Static Functions
 ************************************************************************/
/** @brief Convert voltage to O2 value
 *
 * Converts a voltage to the corresponding O2 reading
 *
 * @brief *v Pointer to voltage read on ADC
 * @brief *data Pointer to calibration value
 *
 * @return O2 value
 */
void MAX250_CalculateO2(float *v, float *data)
{
    *data = 0;
    *data = *v;
    *data *= MAX250.cal;

    return;
}