/** @file motioncontrol.c
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.gov
 *  @date September 26, 2017
 *  @version 1.0.0
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Engineering Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */
#include "motioncontrol.h"

/************************************************************************
*			    Variables (Static in Module)
************************************************************************/
STATIC PERSISTENT sMotionControl_t Motion = {
    .DRV_A = {
        .Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode = 0},
        .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode = 0},
        .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode = 0},
        .cs = {.port = SYS_MCONTROL_BRIDGE_A_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_A_CS_PIN, .mode = 0},
        .SpiChan = {.param = {
                        .selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
                        .clockSourceFrequency = SYS_MCONTROL_SPI_CLK_FREQ,
                        .desiredSpiClock = SYS_MCONTROL_SPI_BAUD,
                        .msbFirst = SYS_MCONTROL_SPI_MSBFIRST,
                        .clockPhase = SYS_MCONTROL_SPI_CLK_PHASE,
                        .clockPolarity = SYS_MCONTROL_SPI_CLK_POL,
                        .spiMode = SYS_MCONTROL_SPI_MODE},
                    .baseAddress = SYS_MCONTROL_BRIDGE_A_SPI_BASE,
                    .slaveAddress = 0,
                    .mode = 0,
                    .configured = 0,
                    .enabled = 0,
                    .interrupt = 0}},
    .DRV_B = {.Strobe = {.port = SYS_MCONTROL_STROBE_PORT, .pin = SYS_MCONTROL_STROBE_PIN, .mode = 0}, .nReset = {.port = SYS_MCONTROL_NRESET_PORT, .pin = SYS_MCONTROL_NRESET_PIN, .mode = 0}, .nSleep = {.port = SYS_MCONTROL_NSLEEP_PORT, .pin = SYS_MCONTROL_NSLEEP_PIN, .mode = 0}, .cs = {.port = SYS_MCONTROL_BRIDGE_B_CS_PORT, .pin = SYS_MCONTROL_BRIDGE_B_CS_PIN, .mode = 0}, .SpiChan = {.param = {.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK, .clockSourceFrequency = SYS_MCONTROL_SPI_CLK_FREQ, .desiredSpiClock = SYS_MCONTROL_SPI_BAUD, .msbFirst = SYS_MCONTROL_SPI_MSBFIRST, .clockPhase = SYS_MCONTROL_SPI_CLK_PHASE, .clockPolarity = SYS_MCONTROL_SPI_CLK_POL, .spiMode = SYS_MCONTROL_SPI_MODE}, .baseAddress = SYS_MCONTROL_BRIDGE_A_SPI_BASE, .slaveAddress = 0, .mode = 0, .configured = 0, .enabled = 0, .interrupt = 0}},
    .Delay = MOTION_VALVE_PULSE_DELAY,
    .Valve[0] = {.Bridge = MOTION_VALVE_1_BRIDGE, .Chan = MOTION_VALVE_1_CHAN},
    .Valve[1] = {.Bridge = MOTION_VALVE_2_BRIDGE, .Chan = MOTION_VALVE_2_CHAN},
    .Valve[2] = {.Bridge = MOTION_VALVE_3_BRIDGE, .Chan = MOTION_VALVE_3_CHAN},
    .Valve[3] = {.Bridge = MOTION_VALVE_4_BRIDGE, .Chan = MOTION_VALVE_4_CHAN},
    .Valve[4] = {.Bridge = MOTION_VALVE_5_BRIDGE, .Chan = MOTION_VALVE_5_CHAN},
    .Valve[5] = {.Bridge = MOTION_VALVE_6_BRIDGE, .Chan = MOTION_VALVE_6_CHAN},
    .Valve[6] = {.Bridge = MOTION_VALVE_7_BRIDGE, .Chan = MOTION_VALVE_7_CHAN},
    .Valve[7] = {.Bridge = MOTION_VALVE_8_BRIDGE, .Chan = MOTION_VALVE_8_CHAN},
};

STATIC const sMotionSetting_t ZeroPump_State_Default = {
    .Valve[0] = {.dir = MOTION_ZERO_PUMP_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_ZERO_PUMP_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_ZERO_PUMP_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_ZERO_PUMP_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_ZERO_PUMP_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_ZERO_PUMP_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_ZERO_PUMP_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_ZERO_PUMP_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_ON};

STATIC const sMotionSetting_t ZeroCal_State_Default = {
    .Valve[0] = {.dir = MOTION_ZERO_CAL_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_ZERO_CAL_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_ZERO_CAL_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_ZERO_CAL_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_ZERO_CAL_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_ZERO_CAL_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_ZERO_CAL_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_ZERO_CAL_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t ZeroVent_State_Default = {
    .Valve[0] = {.dir = MOTION_ZERO_VENT_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_ZERO_VENT_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_ZERO_VENT_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_ZERO_VENT_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_ZERO_VENT_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_ZERO_VENT_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_ZERO_VENT_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_ZERO_VENT_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t SpanPump_State_Default = {
    .Valve[0] = {.dir = MOTION_SPAN_PUMP_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_SPAN_PUMP_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_SPAN_PUMP_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_SPAN_PUMP_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_SPAN_PUMP_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_SPAN_PUMP_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_SPAN_PUMP_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_SPAN_PUMP_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t SpanCal_State_Default = {
    .Valve[0] = {.dir = MOTION_SPAN_CAL_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_SPAN_CAL_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_SPAN_CAL_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_SPAN_CAL_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_SPAN_CAL_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_SPAN_CAL_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_SPAN_CAL_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_SPAN_CAL_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t SpanVent_State_Default = {
    .Valve[0] = {.dir = MOTION_SPAN_VENT_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_SPAN_VENT_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_SPAN_VENT_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_SPAN_VENT_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_SPAN_VENT_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_SPAN_VENT_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_SPAN_VENT_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_SPAN_VENT_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t EquilPump_State_Default = {
    .Valve[0] = {.dir = MOTION_EQUIL_PUMP_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_EQUIL_PUMP_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_EQUIL_PUMP_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_EQUIL_PUMP_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_EQUIL_PUMP_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_EQUIL_PUMP_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_EQUIL_PUMP_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_EQUIL_PUMP_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_ON};
STATIC const sMotionSetting_t EquilVent_State_Default = {
    .Valve[0] = {.dir = MOTION_EQUIL_VENT_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_EQUIL_VENT_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_EQUIL_VENT_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_EQUIL_VENT_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_EQUIL_VENT_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_EQUIL_VENT_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_EQUIL_VENT_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_EQUIL_VENT_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};
STATIC const sMotionSetting_t EquilPost_State_Default = {
    .Valve[0] = {.dir = MOTION_EQUIL_POST_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_EQUIL_POST_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_EQUIL_POST_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_EQUIL_POST_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_EQUIL_POST_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_EQUIL_POST_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_EQUIL_POST_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_EQUIL_POST_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};
STATIC const sMotionSetting_t AirPump_State_Default = {
    .Valve[0] = {.dir = MOTION_AIR_PUMP_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_AIR_PUMP_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_AIR_PUMP_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_AIR_PUMP_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_AIR_PUMP_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_AIR_PUMP_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_AIR_PUMP_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_AIR_PUMP_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_ON};
STATIC const sMotionSetting_t AirVent_State_Default = {
    .Valve[0] = {.dir = MOTION_AIR_VENT_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_AIR_VENT_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_AIR_VENT_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_AIR_VENT_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_AIR_VENT_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_AIR_VENT_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_AIR_VENT_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_AIR_VENT_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};
STATIC const sMotionSetting_t AirPost_State_Default = {
    .Valve[0] = {.dir = MOTION_AIR_POST_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_AIR_POST_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_AIR_POST_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_AIR_POST_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_AIR_POST_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_AIR_POST_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_AIR_POST_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_AIR_POST_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};
STATIC const sMotionSetting_t Rest_State_Default = {
    .Valve[0] = {.dir = MOTION_REST_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_REST_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_REST_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_REST_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_REST_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_REST_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_REST_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_REST_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t Deploy_State_Default = {
    .Valve[0] = {.dir = MOTION_DEPLOY_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_DEPLOY_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_DEPLOY_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_DEPLOY_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_DEPLOY_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_DEPLOY_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_DEPLOY_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_DEPLOY_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t Pressurize_State_Default = {
    .Valve[0] = {.dir = MOTION_PRESSURE_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRESSURE_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRESSURE_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRESSURE_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRESSURE_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRESSURE_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRESSURE_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRESSURE_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG1_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG1_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG1_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG1_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG1_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG1_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG1_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG1_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG1_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_ON};

STATIC const sMotionSetting_t PRG2_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG2_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG2_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG2_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG2_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG2_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG2_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG2_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG2_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG3_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG3_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG3_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG3_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG3_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG3_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG3_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG3_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG3_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG4_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG4_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG4_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG4_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG4_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG4_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG4_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG4_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG4_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG5_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG5_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG5_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG5_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG5_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG5_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG5_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG5_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG5_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG6_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG6_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG6_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG6_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG6_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG6_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG6_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG6_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG6_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};
STATIC const sMotionSetting_t PRG7_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG7_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG7_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG7_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG7_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG7_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG7_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG7_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG7_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

STATIC const sMotionSetting_t PRG8_State_Default = {
    .Valve[0] = {.dir = MOTION_PRG8_DIR_V1,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[1] = {.dir = MOTION_PRG8_DIR_V2,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[2] = {.dir = MOTION_PRG8_DIR_V3,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[3] = {.dir = MOTION_PRG8_DIR_V4,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[4] = {.dir = MOTION_PRG8_DIR_V5,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[5] = {.dir = MOTION_PRG8_DIR_V6,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[6] = {.dir = MOTION_PRG8_DIR_V7,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Valve[7] = {.dir = MOTION_PRG8_DIR_V8,
                 .current = MOTION_VALVE_NOM_CURRENT,
                 .decay = MOTION_VALVE_DECAY},
    .Pump = PUMP_OFF};

    
STATIC PERSISTENT sMotionSetting_t ZeroPump_State;
STATIC PERSISTENT sMotionSetting_t ZeroVent_State;
STATIC PERSISTENT sMotionSetting_t ZeroCal_State;

STATIC PERSISTENT sMotionSetting_t SpanPump_State;
STATIC PERSISTENT sMotionSetting_t SpanVent_State;
STATIC PERSISTENT sMotionSetting_t SpanCal_State;

STATIC PERSISTENT sMotionSetting_t EquilPump_State;
STATIC PERSISTENT sMotionSetting_t EquilVent_State;
STATIC PERSISTENT sMotionSetting_t EquilPost_State;

STATIC PERSISTENT sMotionSetting_t AirPump_State;
STATIC PERSISTENT sMotionSetting_t AirVent_State;
STATIC PERSISTENT sMotionSetting_t AirPost_State;

STATIC PERSISTENT sMotionSetting_t Rest_State;
STATIC PERSISTENT sMotionSetting_t Deploy_State;

//STATIC PERSISTENT sMotionSetting_t Vent_State;
STATIC PERSISTENT sMotionSetting_t Pressurize_State;
// STATIC PERSISTENT sMotionSetting_t Purge_State;
// STATIC PERSISTENT sMotionSetting_t Clear_State;

STATIC PERSISTENT sMotionSetting_t PRG1_State;
STATIC PERSISTENT sMotionSetting_t PRG2_State;
STATIC PERSISTENT sMotionSetting_t PRG3_State;
STATIC PERSISTENT sMotionSetting_t PRG4_State;
STATIC PERSISTENT sMotionSetting_t PRG5_State;
STATIC PERSISTENT sMotionSetting_t PRG6_State;
STATIC PERSISTENT sMotionSetting_t PRG7_State;
STATIC PERSISTENT sMotionSetting_t PRG8_State;

/************************************************************************
*			     Static Functions Prototypes
************************************************************************/
STATIC eMotionStatus_t MCONTROL_PCO2_Set(sMotionSetting_t *state);
STATIC eMotionStatus_t MCONTROL_Set(sMotionControl_t *control, sMotionSetting_t *state, uint8_t enable);
STATIC void MCONTROL_PCO2_SetPump(ePumpStates_t state);

/************************************************************************
*					    Functions
************************************************************************/
void MCONTROL_Init(void)
{
    /** Initialize the settings for each state */
    ZeroPump_State = ZeroPump_State_Default;
    ZeroVent_State = ZeroVent_State_Default;
    ZeroCal_State = ZeroCal_State_Default;

    SpanPump_State = SpanPump_State_Default;
    SpanVent_State = SpanVent_State_Default;
    SpanCal_State = SpanCal_State_Default;

    EquilPump_State = EquilPump_State_Default;
    EquilVent_State = EquilVent_State_Default;
    EquilPost_State = EquilPost_State_Default;

    AirPump_State = AirPump_State_Default;
    AirVent_State = AirVent_State_Default;
    AirPost_State = AirPost_State_Default;

    Rest_State = Rest_State_Default;
    Deploy_State = Deploy_State_Default;
    Pressurize_State = Pressurize_State_Default;
    PRG1_State = PRG1_State_Default;
    PRG2_State = PRG2_State_Default;
    PRG3_State = PRG3_State_Default;
    PRG4_State = PRG4_State_Default;
    PRG5_State = PRG5_State_Default;
    PRG6_State = PRG6_State_Default;
    PRG7_State = PRG7_State_Default;
    PRG8_State = PRG8_State_Default;

    /** Initialize the DRV8823 Channels */
    DRV8823_Init(&Motion.DRV_A);
    DRV8823_Init(&Motion.DRV_B);

    /** Initialize the Relay */
    RELAY_Init();
}

void MCONTROL_SleepState(void)
{
    DRV8823_SetSleepMode(&Motion.DRV_A);
    DRV8823_SetSleepMode(&Motion.DRV_B);
    return;
}

eMotionStatus_t MCONTROL_PCO2_SetMode(ePCO2State_t mode)
{
    eMotionStatus_t result = MCONTROL_FAIL;
    sMotionSetting_t *pState;
//    uint16_t mode_u;

    /** Take out of sleep mode */
    DRV8823_ClearSleepMode(&Motion.DRV_A);

/** Pause to wake up */
#ifndef UNIT_TESTING
    _delay_ms(MOTION_VALVE_WAKEUP);
#endif

//    switch ((mode & 0xFFFE))
    switch(mode)
    {
    case PCO2_ZERO_PUMP_ON:
    case PCO2_ZERO_PUMP_OFF:
        pState = &ZeroPump_State;
        break;
    case PCO2_ZERO_CAL:
        pState = &ZeroCal_State;
        break;
    case PCO2_ZERO_VENT:
        pState = &ZeroVent_State;
        break;
    case PCO2_SPAN_PUMP_ON:
        pState = &SpanPump_State;
        break;
    case PCO2_SPAN_CAL:
        pState = &SpanCal_State;
        break;
    case PCO2_SPAN_PUMP_OFF:
    case PCO2_SPAN_VENT:
        pState = &SpanVent_State;
        break;
    case PCO2_EQUIL_PUMP_ON:
    case PCO2_EQUIL_PUMP_OFF:
        pState = &EquilPump_State;
        break;
    case PCO2_EQUIL_VENT:
        pState = &EquilVent_State;
        break;
    case PCO2_EQUIL_POST:
        pState = &EquilPost_State;
        break;
    case PCO2_AIR_PUMP_ON:
    case PCO2_AIR_PUMP_OFF:
        pState = &AirPump_State;
        break;
    case PCO2_AIR_VENT:
        pState = &AirVent_State;
        break;
    case PCO2_AIR_POST:
        pState = &AirPost_State;
        break;
    case PCO2_REST:
        pState = &Rest_State;
        break;
    case PCO2_DEPLOYMENT:
        pState = &Deploy_State;
        break;
    case PCO2_VENT:
        break;
    case PCO2_PRESSURIZE:
        pState = &Pressurize_State;
        break;
    case PCO2_PRG1:
        pState = &PRG1_State;
        break;
    case PCO2_PRG2:
        pState = &PRG2_State;
        break;
    case PCO2_PRG3:
        pState = &PRG3_State;
        break;
    case PCO2_PRG4:
        pState = &PRG4_State;
        break;
    case PCO2_PRG5:
        pState = &PRG5_State;
        break;
    case PCO2_PRG6:
        pState = &PRG6_State;
        break;
    case PCO2_PRG7:
        pState = &PRG7_State;
        break;
    case PCO2_PRG8:
        pState = &PRG8_State;
        break;
    default:
        break;
    }

    /** Set the State */
    result = MCONTROL_PCO2_Set(pState);
    if (mode & 0x01)
    {
        MCONTROL_PCO2_SetPump(PUMP_ON);
    }
    else
    {
        MCONTROL_PCO2_SetPump(PUMP_OFF);
    }

    /** Update the current Motion Control State */
    if (result == MCONTROL_OK)
    {
        Motion.PCO2State = mode;
    }

    /** Reset the DRV8823's */
    DRV8823_Reset(&Motion.DRV_A);

    /** Put DRV8823 to sleep for low power */
    DRV8823_SetSleepMode(&Motion.DRV_A);

    return result;
}

void MCONTROL_PCO2_TogglePump(void)
{
    ePCO2State_t state = MCONTROL_PCO2_GetMode();

    uint8_t pump = ~(MCONTROL_PCO2_GetPump()) & 0x01;

    state &= 0xFE;
    state |= (pump & 0x01);
    Motion.PCO2State = state;

    MCONTROL_PCO2_SetPump((ePumpStates_t) pump);
}
int8_t MCONTROL_PCO2_GetPump(void)
{
    return Motion.Current.Pump;
}
int8_t MCONTROL_PCO2_GetDir(uint8_t chan)
{
    if (chan <= 8)
    {
        return (int)Motion.Current.Valve[chan].dir;
    }
    else
    {
        return -1;
    }
}

ePCO2State_t MCONTROL_PCO2_GetMode(void)
{

    return Motion.PCO2State;
}

eMotionStatus_t MCONTROL_PCO2_SetCurrent(uint8_t current)
{
    eMotionStatus_t result = MCONTROL_FAIL;

    uint8_t i;

    if (current < 8)
    {
        for (i = 0; i < 8; i++)
        {

            ZeroPump_State.Valve[i].current = current;
            ZeroVent_State.Valve[i].current = current;
            ZeroCal_State.Valve[i].current = current;
            SpanPump_State.Valve[i].current = current;
            SpanVent_State.Valve[i].current = current;
            SpanCal_State.Valve[i].current = current;
            EquilPump_State.Valve[i].current = current;
            EquilVent_State.Valve[i].current = current;
            AirPump_State.Valve[i].current = current;
            AirVent_State.Valve[i].current = current;
            Rest_State.Valve[i].current = current;
            Deploy_State.Valve[i].current = current;
        }
        result = MCONTROL_OK;
    }

    return result;
}

uint8_t MCONTROL_PCO2_GetCurrent(void)
{

    return ZeroPump_State.Valve[0].current;
}

eMotionStatus_t MCONTROL_PCO2_SetPulse(uint32_t pulse)
{
    eMotionStatus_t result = MCONTROL_FAIL;
    if (pulse >= MOTION_VALVE_PULSE_DELAY_MINIMUM)
    {
        Motion.Delay = pulse;
        result = MCONTROL_OK;
    }
    return result;
}

uint32_t MCONTROL_PCO2_GetPulse(void)
{
    return Motion.Delay;
}


void MCONTROL_SetValve(uint8_t valve, uint8_t direction) {
    sDRV8823_t *pIc;
    eDRV8823Chan_t chan = Motion.Valve[valve].Chan;
    sDRV8823DataField_t settings = {
        .Decay = ZeroPump_State.Valve[0].decay,
        .Current = 4,
        .Enable = true,
        .Phase = direction,
    };
    
    
    switch(Motion.Valve[valve].Bridge) 
    {
    case DRV_A:
      pIc = &Motion.DRV_A;
      break;
    case DRV_B:
      pIc = &Motion.DRV_B;
      break;
    }

    
    settings.Enable = true;
    DRV8823_Set_SingleChannel( pIc, chan, &settings);
    _delay_ms(Motion.Delay);
    settings.Enable = true;
    DRV8823_Set_SingleChannel( pIc, chan, &settings);
    
    Motion.Current.Valve[valve].dir = direction;
}

void MCONTROL_Panic(void){
    // uint8_t pulse[24] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 100, 100, 100, 100, 100};
    uint8_t pulse[24] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 35, 40, 45, 40, 45, 30, 35, 30, 35, 50, 50, 50, 50, 50, 50};
   
    sDRV8823DataField_t DF;
    DF.Current = 0x4;
    DF.Decay = 0x1;
    DF.Phase = 0x1;
    DF.Enable = 0x1;
    sDRV8823SetVal_t DRVval;

    /** Cycle pulse the valve with varying pulse width */
    for(uint8_t i=0;i<24;i++){
        
        /** Pulse it positive */
        /** Set the Valve High */
        DF.Enable = 0x1;
        DF.Phase = 0x1;
        DRVval.outA = DF;
        DRVval.outB = DF;
        DRVval.outC = DF;
        DRVval.outD = DF;
        DRV8823_Set_AllChannels(&Motion.DRV_A, &DRVval);
        DRV8823_Set_AllChannels(&Motion.DRV_B, &DRVval);
        _delay_ms(pulse[i]);

        /** Disable the valve (set to 0) */
        DF.Enable = 0x0;
        DRVval.outA = DF;
        DRVval.outB = DF;
        DRVval.outC = DF;
        DRVval.outD = DF;
        DRV8823_Set_AllChannels(&Motion.DRV_A, &DRVval);
        DRV8823_Set_AllChannels(&Motion.DRV_B, &DRVval);
        _delay_ms(pulse[i]);

        /** Pulse it negative */
        /** Set the valve Low */
        DF.Enable = 0x1;
        DF.Phase = 0x0;
        DRVval.outA = DF;
        DRVval.outB = DF;
        DRVval.outC = DF;
        DRVval.outD = DF;
        DRV8823_Set_AllChannels(&Motion.DRV_A, &DRVval);
        DRV8823_Set_AllChannels(&Motion.DRV_B, &DRVval);
        _delay_ms(pulse[i]);
         /** Disable the valve (set to 0) */
        DF.Enable = 0x0;
        DRVval.outA = DF;
        DRVval.outB = DF;
        DRVval.outC = DF;
        DRVval.outD = DF;
        DRV8823_Set_AllChannels(&Motion.DRV_A, &DRVval);
        DRV8823_Set_AllChannels(&Motion.DRV_B, &DRVval);
        _delay_ms(pulse[i]);
    }

    /** Re-init to put into Rest mode */
    MCONTROL_Init();

}
/************************************************************************
*					   Static Functions
************************************************************************/
/** @brief Set the Pump State 
 * 
 * Set the pump state to the users command
 * 
 * @param state State to Set (PUMP_ON or PUMP_OFF)
 * 
 * @return None
 */
STATIC void MCONTROL_PCO2_SetPump(ePumpStates_t state)
{
    if (state == PUMP_ON)
    {
        RELAY_On();
    }
    else
    {
        RELAY_Off();
    }
    Motion.Current.Pump = state;
}

/** @brief Set the PCO2 Zero State
 *
 *  Sets the valves and pump to the correct states to run PCO2 ZERO.  
 *  For 
 *
 *
 *  @param *state Pointer to Motion State Structure
 * 
 *  @return result
 */
STATIC eMotionStatus_t MCONTROL_PCO2_Set(sMotionSetting_t *state)
{
    eMotionStatus_t result = MCONTROL_FAIL;
//    uint8_t i;

    /** Set the valves and check for valid set */
    if (MCONTROL_Set(&Motion, state, VALVE_ENABLE) == MCONTROL_OK)
    {
/** Wait for the number of milliseconds needed for the Valves */
#ifndef UNIT_TESTING
        _delay_ms(Motion.Delay);
#endif
        /** Clear the valve pulse */
        if (MCONTROL_Set(&Motion, state, VALVE_DISABLE) == MCONTROL_OK)
        {

            /** If the valves set, Set the Pump State */
            if (state->Pump == PUMP_ON)
            {
                RELAY_On();
            }
            else
            {
                RELAY_Off();
            }

            Motion.Current = *state;
            result = MCONTROL_OK;
        }
    }

    return result;
}

/** @brief Set the board state
 *
 * Set the board state
 *
 * @param *control Pointer to Motion control structure
 * @param *state Pointer to state to set
 * @param enable Enable the state?
 *
 * @return result
 */
STATIC eMotionStatus_t MCONTROL_Set(sMotionControl_t *control, sMotionSetting_t *state, uint8_t enable)
{

    eMotionStatus_t result = MCONTROL_FAIL;
    sDRV8823SetVal_t DRVA;
    sDRV8823SetVal_t DRVB;
    sDRV8823SetVal_t *pDRV;

    sDRV8823DataField_t Data;
    sValveState_t VState;
    uint8_t i;

    /** Create the Valve Settings */
    for (i = 0; i < 8; i++)
    {

        VState = state->Valve[i];

        if (DRV8823_CreateDatafield(&Data, VState.decay, VState.current, VState.dir, enable) == DRV8823_OK)
        {

            switch (control->Valve[i].Bridge)
            {
            case DRV_A:
                pDRV = &DRVA;

                break;
            case DRV_B:
                pDRV = &DRVB;
                break;
            default:
                /** @todo Add Error handling */
                break;
            }

            switch (control->Valve[i].Chan)
            {
            case DRV_CHAN_A:
                pDRV->outA = Data;
                break;
            case DRV_CHAN_B:
                pDRV->outB = Data;
                break;
            case DRV_CHAN_C:
                pDRV->outC = Data;
                break;
            case DRV_CHAN_D:
                pDRV->outD = Data;
                break;
            default:
                /** @todo Add Error Handling */
                break;
            }
            result = MCONTROL_OK;
        }
        else
        {
            result = MCONTROL_FAIL;
            break;
        }
    }

    /** If there are valid results, Set the Bridges */
    if (result == MCONTROL_OK)
    {
        result = MCONTROL_FAIL;
        if (DRV8823_Set_AllChannels(&control->DRV_A, &DRVA) == DRV8823_OK)
        {
            if (DRV8823_Set_AllChannels(&control->DRV_B, &DRVB) == DRV8823_OK)
            {

                result = MCONTROL_OK;
                control->Current = *state;
                /** Start the Timer */
                /** @todo Clear the Valve Timer */
            }
        }
    }

    return result;
}


