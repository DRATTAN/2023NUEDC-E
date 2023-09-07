/*
 * control.h
 *
 *  Created on: 2023/07/27
 *      Author: RATTAN
 */

#ifndef CONTROL_CONTROL_H_
#define CONTROL_CONTROL_H_

#include "LIB_INCLUDE.h"
#include "motor.h"
#include "pc.h"
#include "gyro.h"

struct _PID
{
    float kp;
    float ki;
    float kd;
    float error;
    float lasterror;
    float lastlasterror;
};

typedef struct _PID PID;


float IncPid(PID *);
float PosPid(PID *);
float GetVbat(uint8_t);

#endif /* CONTROL_CONTROL_H_ */
