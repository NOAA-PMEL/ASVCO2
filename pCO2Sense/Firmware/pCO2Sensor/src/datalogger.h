#ifndef _DATALOGGER_H
#define _DATALOGGER_H

#include <stdint.h>

typedef enum{
    DL_OK = 0x00u,
    DL_FAIL = 0x01
}eDLStatus_t;

eDLStatus_t DATALOGGER_Init(void);

void DATALOGGER_ParseError(char *str);

void DATALOGGER_Write_SampleRaw(char *mode, uint8_t modeLen, char *str, uint16_t strLen);

void DATALOGGER_Write_Sample(char *str);

void DATALOGGER_Write_LicorCal(char *str);

#endif // _DATALOGGER_H
