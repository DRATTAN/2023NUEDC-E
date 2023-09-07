/*
 * gyro.c
 *
 *  Created on: 2023/07/30
 *      Author: RATTAN
 */

#include "gyro.h"

static volatile float  AngleExp = 0.0f, AngleAct = 360.0f;
static PID gyroPID = {0.3f, 0.01f, 0.02f, 0.0f, 0.0f, 0.0f};

volatile uint8_t gyro_buffer[11];
volatile uint8_t flags = 0;


void GYROUART5RXINTISR(void);


void Gyro_CycleInit()
{
    LIB_UART_Init(GY62_UART, 115200, GYROUART5RXINTISR);
    while(AngleAct != 360.0f)
    Gyro_Reset();
}

void Gyro_PidParamGet(float *kp, float *ki, float *kd)
{
    *kp = gyroPID.kp;
    *ki = gyroPID.ki;
    *kd = gyroPID.kd;
}

void Gyro_PidParamSet(float kp, float ki, float kd)
{
    gyroPID.kp = kp;
    gyroPID.ki = ki;
    gyroPID.kd = kd;
}

inline void Gyro_Cycle()
{
    static float res = 0.0f;
    gyroPID . error = AngleExp - AngleAct;
    res = PosPid(&gyroPID);
    if(res > 100.0f) res = 100.0f;
    if(res < -100.0f) res = -100.0f;
    Motor_OffsetSet(-1.0f * res, res);

}

void Gyro_Reset()
{
    uint8_t reset_table[] = {0xFF, 0xAA, 0x52};
    flags = 0;
    while(1) if(flags == 1) break;
    LIB_UART_TransmitData(GY62_UART, reset_table, 3);
}

inline float Gyro_AngleActGet()
{
    return AngleAct;
}

inline float Gyro_AngleExpGet()
{
    return AngleExp;
}

inline void Gyro_AngleExpSet(float a)
{
    AngleExp = a;
}

void Gyro_TurnOffset(float angle, int16_t speed)
{
    if(angle == 0) return;
    else if(angle > 0)
    {
        Gyro_Reset();
        Motor_OffsetSet(-1 * speed, speed);
        while(AngleAct < angle - 5.0f || AngleAct > angle + 5.0f) __nop();
        Motor_OffsetSet(0, 0);
    }
    else
    {
        Gyro_Reset();
        Motor_OffsetSet(speed, -1 * speed);
        while(AngleAct < angle - 5.0f || AngleAct > angle + 5.0f) __nop();
        Motor_OffsetSet(0, 0);
    }
}

void GYROUART5RXINTISR(void)
{
    static uint8_t counter = 0;
    static volatile float Roll = 0.0f;
    static volatile float Pitch = 0.0f;
    static volatile float Yaw = 0.0f;

    //static uint8_t flags = 0x00;

    gyro_buffer[counter] = LIB_UART_GetBlockData(GY62_UART);

    if(gyro_buffer[0] != 0x53) counter = 0;
    else
    {
        counter++;
        flags = 0;
    }

    if(counter == 10)
    {
        counter = 0;
        gyro_buffer[0] = 0x00;
        //Roll = ((float) ((uint16_t) gyro_buffer[2] << 8 | (uint16_t) gyro_buffer[1])) / 32768.0f * 180.0f;
        //Pitch = ((float) ((uint16_t) gyro_buffer[4] << 8 | (uint16_t) gyro_buffer[3])) / 32768.0f * 180.0f;
        AngleAct = ((float) *((int16_t*) (gyro_buffer + 5)))/32768.0f * -180.0f;
        //AngleAct = Yaw;
        //Gyro_Cycle();
        flags = 1;

    }
}

