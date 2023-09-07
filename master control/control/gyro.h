/*
 * gyro.h
 *
 *  Created on: 2023/07/30
 *      Author: RATTAN
 */

#ifndef CONTROL_GYRO_H_
#define CONTROL_GYRO_H_

#include "control.h"
#include "PERI_INCLUDE.h"
#include "math.h"



void Gyro_CycleInit(void);
void Gyro_Reset(void);
void Gyro_Cycle(void);
float Gyro_AngleActGet(void);
float Gyro_AngleExpGet(void);
void Gyro_AngleExpSet(float);
void Gyro_TurnOffset(float, int16_t);
void Gyro_PidParamGet(float *, float *, float *);
void Gyro_PidParamSet(float, float, float);

#endif /* CONTROL_GYRO_H_ */
