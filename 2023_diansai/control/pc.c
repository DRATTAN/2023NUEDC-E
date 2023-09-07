/*
 * pc.c
 *
 *  Created on: 2023/07/29
 *      Author: RATTAN
 */

#include "pc.h"

static uint8_t PC_UART = 0;

static uint8_t pc_param_buffer[64];

void PCUARTXRXISR(void);

void Pc_UartInit(uint8_t UART_NUM)
{
    PC_UART = UART_NUM;
    LIB_UART_PrintfInit(UART_NUM, PCUARTXRXISR);
}

void Pc_GetParam(void)
{
    static unsigned int d0, d1;
    sscanf((char*)pc_param_buffer, "%u,%u", &d0, &d1);
    LIB_PWM_SetPulseWidth(1, 6, d0);
    LIB_PWM_SetPulseWidth(1, 7, d1);
    Oled_ShowParamFloat(0, 1, "CH6:", d0);
    Oled_ShowParamFloat(0, 2, "CH7:", d1);
}

void PCUARTXRXISR()
{
    static uint8_t counter = 0;
    static uint8_t buffer = 0;
    buffer = LIB_UART_GetBlockData(PC_UART);
    if((buffer == '\0') || (buffer == '\r') || (buffer == '\n') || counter == 49)
    {
        pc_param_buffer[counter] = '\0';
        counter = 0;
        Pc_GetParam();
    }
    else
    {
        pc_param_buffer[counter] = buffer;
        counter++;
    }

}
