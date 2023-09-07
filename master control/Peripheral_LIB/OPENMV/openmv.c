/*
 * openmv.c
 *
 *  Created on: 2023/08/01
 *      Author: RATTAN
 */


#include "openmv.h"

volatile int32_t pan_error = 0, tilt_error = 0, step = 5, error = 5;
volatile uint16_t duty_pan = 3750, duty_tilt = 3950;
static volatile uint8_t buffer = 0;
static uint8_t str[200];
static uint8_t strs[30];
extern volatile uint8_t openmv_ok;


void OPENMV_UART_INIT_ISR(void);
void ISR_CALL_BACK(void);

void Openmv_Init()
{
    LIB_UART_Init(OPENMV_UART, 115200, OPENMV_UART_INIT_ISR);
    return ;
}

void ISR_CALL_BACK()
{

    int16_t pan_step_offset, tilt_step_offset;
    int32_t is_steepjump = 0;
    sscanf((char*)str, "[%d],[%d],[%d],[%d],[%d],%s", &pan_error, &tilt_error, &step, &error, &is_steepjump, strs);
    if(strcmp((char*) strs, "init") == 0) openmv_ok = 0;
    else if(strcmp((char*) strs, "ready") == 0) openmv_ok = 1;
    if(is_steepjump)
    {
        if(abs(pan_error) > 6 * error) pan_step_offset = 3 * step;
        else if (abs(pan_error) < error) pan_step_offset = 2;
        else pan_step_offset = step;

        if(abs(tilt_error) > 6 * error) tilt_step_offset = 3 * step;
        else if (abs(tilt_error) < error) tilt_step_offset = 2;
        else tilt_step_offset = step;
    }
    else
    {
        pan_step_offset = step;
        tilt_step_offset = step;
    }
    if(pan_error > error )
    {
        duty_pan -= pan_step_offset;
        LIB_PWM_SetPulseWidth(0, 0, duty_pan);
    }
    else if(pan_error < -1 * error)
    {
        duty_pan += pan_step_offset;
        LIB_PWM_SetPulseWidth(0, 0, duty_pan);
    }
    if(tilt_error > error )
    {
        duty_tilt -= tilt_step_offset;
        LIB_PWM_SetPulseWidth(0, 1, duty_tilt);
    }
    else if(tilt_error < -1 * error)
    {
        duty_tilt += tilt_step_offset;
        LIB_PWM_SetPulseWidth(0, 1, duty_tilt);
    }

}

void OPENMV_UART_INIT_ISR()
{
    static uint8_t counter = 0;
    buffer = LIB_UART_GetBlockData(OPENMV_UART);
    if(buffer == '\0' || counter == 49)
    {
        str[counter] = '\0';
        counter = 0;
        ISR_CALL_BACK();
    }
    else
    {
        str[counter] = buffer;
        counter++;
    }
}
