#ifndef __DIR_H
#define __DIR_H

#include "stm32f10x.h"
#include "stdint.h"

#define GPIO_Type_EN_Pin 			GPIOD
#define GPIO_EN_One_Pin 			GPIO_Pin_12
#define GPIO_EN_Two_Pin 			GPIO_Pin_15

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} MotorControl;

typedef enum {
    MOTOR1,
    MOTOR2,
    MOTOR3,
    MOTOR4,
	MOTOR5
} MotorType;

typedef enum{
	Fun   = 0,
	Zheng = 1
}MotorDir;

typedef enum{
	Drop   = 0,
	Rise = 1
}StepMotorType;

typedef enum{
	DisEn   = 0,
	En = 1
}MotorEn;

typedef enum{
	Forward, 
	Backward,
	Right,
	Left,
	Left_rotation,
	Right_rotation
}MoveDir;

typedef enum{
	UP,
	DROP
}UpDir;

void SetMotor_Direction(MotorType motor, MotorDir dir);
void SetAllMotorsDirection(MotorDir dir1, MotorDir dir2, MotorDir dir3, MotorDir dir4);
void SetMotor_En(StepMotorType step, MotorEn en);
void MoveSetDirection(MoveDir Behavior);
void UpSeSetDirection(UpDir dir);


#endif
