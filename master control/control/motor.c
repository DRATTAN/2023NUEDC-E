/*
 * motor.c
 *
 *  Created on: 2023/07/27
 *      Author: RATTAN
 */

#include "motor.h"

static int16_t LeftSpeedExp = 0, RightSpeedExp = 0;
volatile int16_t LeftSpeedAct = 0, RightSpeedAct = 0;
int16_t  RightSpeedoffset = 0;
int16_t  LeftSpeedoffset = 0;


#define KP 5.0f
#define KI 1.5f
#define KD 0.01f

static PID leftmotorPID =    {KP, KI, KD, 0.0f, 0.0f, 0.0f};
static PID rightmotorPID =   {KP, KI, KD, 0.0f, 0.0f, 0.0f};



void QEI0_TIMER_ISR()
{
    static float duty = 0;
    LeftSpeedAct = LIB_QEI_GetPulse(QEI0) * LIB_QEI_GetDirect(QEI0) * 2 + LeftSpeedoffset;
    leftmotorPID . error = LeftSpeedExp - LeftSpeedAct;
    duty += IncPid(&leftmotorPID);
    Motor_LeftPwmDutySet((int16_t)duty);
    return;
}

void QEI1_TIMER_ISR()
{
    static float duty = 0;
    RightSpeedAct = LIB_QEI_GetPulse(QEI1) * LIB_QEI_GetDirect(QEI1) * 2 + RightSpeedoffset;
    rightmotorPID . error = RightSpeedExp - RightSpeedAct;
    duty += IncPid(&rightmotorPID);
    Motor_RightPwmDutySet((int16_t)duty);
    return;
}

void Motor_CycleInit()
{
    LIB_PWM_Init(0, 0, PWMFREQ, 0, 0);
    LIB_PWM_Init(0, 1, PWMFREQ, 0, 0);
    LIB_QEI_Init(1, QEIINTFREQ, QEI1_TIMER_ISR);
    LIB_QEI_Init(0, QEIINTFREQ, QEI0_TIMER_ISR);
}

inline void Motor_LeftPwmDutySet(int16_t duty)
{
    if(duty >= 0)
    {
        LIB_PWM_SetPulseWidth(0, 2, 0);
        if(duty > FULLPWMDUTY) duty  = FULLPWMDUTY;
        LIB_PWM_SetPulseWidth(0, 3, duty);
    }
    else
    {
        duty = -1 * duty;
        LIB_PWM_SetPulseWidth(0, 2, FULLPWMDUTY);
        if(duty > FULLPWMDUTY) duty  = FULLPWMDUTY;
        LIB_PWM_SetPulseWidth(0, 3, duty);
    }
}

inline void Motor_RightPwmDutySet(int16_t duty)
{
    if(duty >= 0)
    {
        LIB_PWM_SetPulseWidth(0, 0, 0);
        if(duty > FULLPWMDUTY) duty  = FULLPWMDUTY;
        LIB_PWM_SetPulseWidth(0, 1, duty);
    }
    else
    {
        duty = -1 * duty;
        LIB_PWM_SetPulseWidth(0, 0, FULLPWMDUTY);
        if(duty > FULLPWMDUTY) duty  = FULLPWMDUTY;
        LIB_PWM_SetPulseWidth(0, 1, duty);
    }
}

void Motor_ExpSpeedSet(int16_t left, int16_t right)
{
    LeftSpeedExp = left;
    RightSpeedExp = right;
}



void Motor_ExpSpeedGet(int16_t *left, int16_t *right)
{
    *left = LeftSpeedExp;
    *right = RightSpeedExp;
}

void Motor_ActSpeedGet(int16_t *left, int16_t *right)
{
    *left = LeftSpeedAct;
    *right = RightSpeedAct;
}

void Motor_PidParamSet(float kp, float ki, float kd)
{
    rightmotorPID.kp = kp;
    rightmotorPID.ki = ki;
    rightmotorPID.kd = kd;
    leftmotorPID.kp = kp;
    leftmotorPID.ki = ki;
    leftmotorPID.kd = kd;

}

void Motor_PidParamGet(float *kp, float *ki, float *kd)
{
    *kp = leftmotorPID.kp;
    *ki = leftmotorPID.ki;
    *kd = leftmotorPID.kd;
}

inline void Motor_OffsetSet(int16_t left, int16_t right)
{
    RightSpeedoffset = right;
    LeftSpeedoffset = left;
}
