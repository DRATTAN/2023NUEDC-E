/*
 * motor.h
 *
 *  Created on: 2023/07/27
 *      Author: RATTAN
 */

#ifndef CONTROL_MOTOR_H_
#define CONTROL_MOTOR_H_

#include "control.h"

#define PWMFREQ 2500
#define FULLPWMDUTY (CPU_F / 32 / PWMFREQ) - 1
#define QEIINTFREQ 100

extern volatile int16_t LeftSpeedAct, RightSpeedAct;

void Motor_CycleInit(void);
void Motor_LeftPwmDutySet(int16_t);
void Motor_RightPwmDutySet(int16_t);
void Motor_ExpSpeedSet(int16_t, int16_t);
void Motor_ExpSpeedGet(int16_t *, int16_t *);
void Motor_ActSpeedGet(int16_t *, int16_t *);
void Motor_PidParamSet(float, float, float);
void Motor_PidParamGet(float *, float *, float *);
void Motor_OffsetSet(int16_t left, int16_t right);

#endif /* CONTROL_MOTOR_H_ */
