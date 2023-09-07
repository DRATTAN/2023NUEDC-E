/*
 * oled.h
 *
 *  Created on: 2023/03/05
 *      Author: RATTAN
 */

#ifndef _OLED_H_
#define _OLED_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "front.h"
#include "lib_i2c.h"


#define Oled_Addr 0x78 >> 1
#define I2C_NUM I2C0

void Oled_Clear(void);
void Oled_Clear_Part(uint8_t, uint8_t, uint8_t);
void Oled_Init(void);
void Oled_Fill(uint8_t);
void Oled_SetPos(uint8_t, uint8_t);
void Oled_ShowChar(uint8_t, uint8_t, char);
void Oled_ShowString(uint8_t, uint8_t, const char*);
void Oled_ShowLine(uint8_t, const uint8_t*);
void Oled_ShowBmp(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*);
void Oled_ShowUint32(uint8_t,uint8_t,const uint32_t);
void Oled_ShowInt32(uint8_t,uint8_t,const int32_t);
void Oled_ShowUint16(uint8_t,uint8_t,const uint16_t);
void Oled_ShowInt16(uint8_t,uint8_t,const int16_t);
void Oled_ShowUint8(uint8_t,uint8_t,const uint8_t);
void Oled_ShowInt8(uint8_t,uint8_t ,const int8_t);
void Oled_ShowFloat(uint8_t,uint8_t,const float);
void Oled_ShowParamUint8(uint8_t, uint8_t, const char*, const uint8_t);
void Oled_ShowParamInt8(uint8_t, uint8_t, const char*, const int8_t);
void Oled_ShowParamUint16(uint8_t, uint8_t, const char*, const uint16_t);
void Oled_ShowParamInt16(uint8_t, uint8_t, const char*, const int16_t);
void Oled_ShowParamFloat(uint8_t, uint8_t, const char*, const float);
void Oled_test(uint8_t, uint8_t, const int16_t);


#endif
