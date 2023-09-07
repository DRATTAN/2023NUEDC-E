#include "LIB_INCLUDE.h"
#include "PERI_INCLUDE.h"
#include "STRUCT_INCLUDE.h"
#include "control.h"

void Start(void);
void Reset(void);
void openmv_reset(void);
void Settimu(void);

uint8_t status = 0;
uint8_t timu = 1;
volatile uint8_t openmv_ok = 0;

extern int32_t pan_error, tilt_error, step, error;
extern uint16_t duty_pan, duty_tilt;

#define RESETPORT ISR_GPIOA
#define RESETPIN ISR_GPIO_PIN_6

#define SETPORT ISR_GPIOF
#define SETPIN ISR_GPIO_PIN_0

#define STARTPORT ISR_GPIOA
#define STARTPIN ISR_GPIO_PIN_7

#define OPENMV_RESET_OUT_PORT GPIOE
#define OPENMV_RESET_OUT_PIN GPIO_PIN_4

#define OPENMV_RESET_IN_PORT ISR_GPIOE
#define OPENMV_RESET_IN_PIN ISR_GPIO_PIN_5

int main(void)
{
    IntMasterEnable();
    LIB_CLOCK_MainClockSet(CLOCK_XTAL_PLL_80M);
    LIB_PWM_Init(0, 0, 250 , 3750, 3950);
    Openmv_Init();
    //LIB_ADC_Init(0x09);
    LIB_I2C_Init(I2C0, 1);
    Oled_Init();
    Oled_Clear();
    LIB_GPIO_Init(OPENMV_RESET_OUT_PORT, OPENMV_RESET_OUT_PIN, GPIO_DIR_OUTPUT, GPIO_PIN_TYPE_OUTPUT_STD , GPIO_PIN_SET);
    LIB_GPIO_ExtiInit(RESETPORT, RESETPIN, GPIO_EXTI_TRIGGER_FALLING, Reset);
    LIB_GPIO_ExtiInit(SETPORT, SETPIN, GPIO_EXTI_TRIGGER_FALLING, Settimu);
    LIB_GPIO_ExtiInit(STARTPORT, STARTPIN, GPIO_EXTI_TRIGGER_FALLING, Start);
    LIB_GPIO_ExtiInit(OPENMV_RESET_IN_PORT, OPENMV_RESET_IN_PIN, GPIO_EXTI_TRIGGER_FALLING, openmv_reset);
    //Pc_UartInit(1);


    while(1)
    {
        if(openmv_ok == 0 ) Oled_ShowString(0, 0, "OpenMV offline");
        else if(openmv_ok == 1 ) Oled_ShowString(0, 0, "OpenMV online ");
        Oled_ShowParamUint8(0, 1, "timu:", timu);
        Oled_ShowParamUint8(0, 2, "status:", status);
        delay_ms(10);
    }
}

void openmv_reset()
{
    delay_ms(50);
    if(LIB_GPIO_ReadPin(GPIOE,GPIO_PIN_5)) return;
    LIB_GPIO_WritePin(OPENMV_RESET_OUT_PORT, OPENMV_RESET_OUT_PIN, 0);
    delay_ms(50);
    LIB_GPIO_WritePin(OPENMV_RESET_OUT_PORT, OPENMV_RESET_OUT_PIN, 1);
}

void Reset()
{
    delay_ms(50);
    if(LIB_GPIO_ReadPin(GPIOA,GPIO_PIN_6)) return;
    status = 0;
    LIB_UART_SendData(7, 0);
}

void Start()
{
    delay_ms(50);
    if(LIB_GPIO_ReadPin(GPIOA,GPIO_PIN_7)) return;
    status = 1;
    if(timu == 1)     LIB_UART_SendData(7, 0);
    else LIB_UART_SendData(7, timu);
}

void Settimu()
{
    delay_ms(50);
    if(LIB_GPIO_ReadPin(GPIOF,GPIO_PIN_0)) return;
    timu += 1;
    if(timu > 5) timu = 1;
}
