#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "stdint.h"

#define Paw_Servo			Servo_1
#define	Holder				Servo_2
#define	Material_tray		Servo_3

#define Start_1				180
#define Start_2				42.5
#define Start_3				82

#define SWITCH 				180	//开
#define CLOSE				80.5	//关
#define LITOPEN				95	//微张

#define Holder_In 			80
#define Holder_Out			222.5
#define Holder_WuLiao		78.7

#define Red_Material		2.5
#define Green_Material		125.5
#define Blue_Material		247.5

typedef enum {
    TIM_CHANNEL_1,
    TIM_CHANNEL_2,
    TIM_CHANNEL_3,
} TIM_Channel;

typedef enum {
    Servo_1,
    Servo_2
} ServoType;

void TIM4_Servo_Init(void);
void Servo_TIM4_Choose(ServoType Servo_Choose, TIM_Channel channel, float compare);
void Servo_TIM4_Choose2(TIM_Channel channel, float compare);;

void Paw_Control(float compare);
void Holder_Control(float compare);
void Material_tray_Control(float compare);

#endif
