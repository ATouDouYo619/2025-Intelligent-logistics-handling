#ifndef __Motor_Init_H
#define __Motor_Init_H
#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "Motor.h"  

//输出比较模式周期设置0xFFFF
#define TIM_PERIOD                   0xFFFF

void MOTOR_TIM3_GPIO_Init(void);
void MOTOR_TIM3_Init(void);

void MOTOR_TIM2_GPIO_Init(void);
void MOTOR_TIM2_Init(void);

#endif
