/*
 * control.c
 *
 *  Created on: 2023/07/29
 *      Author: RATTAN
 */

#include "control.h"

float IncPid(PID *pid)
{
    float res = 0;
    res = (pid -> kp) * ((pid -> error) - (pid -> lasterror)) + (pid -> ki) * (pid -> error) + (pid -> kd) * ((pid -> error) - 2.0f * (pid -> lasterror) + (pid -> lastlasterror));
    pid -> lastlasterror = pid -> lasterror;
    pid -> lasterror = pid -> error;
    return res;
}

float PosPid(PID *pid)
{
    float res = 0;
    pid -> lastlasterror += pid -> error;
    res = (pid -> kd) * ((pid -> error) - (pid -> lasterror)) + (pid -> kp) * (pid -> error) + (pid -> ki) * (pid -> lastlasterror);
    pid -> lasterror = pid -> error;
    return res;
}

float GetVbat(uint8_t Vbat_Channel)
{
    uint16_t buffer[8];
    LIB_ADC_GetValue(buffer);
    return ((float)buffer[Vbat_Channel] / 4096.0f * 3.3f*4.0f);
}

