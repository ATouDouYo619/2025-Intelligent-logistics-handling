#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"
#include "stdint.h"

extern uint32_t set_speed;		//最大速度,	为accel*0.1*rad/sec^2  10倍且2个脉冲
extern uint32_t step_accel;		//加速度,	为decel*0.1*rad/sec^2 
extern uint32_t step_decel;		//减速度,	为speed*0.1*rad/sec

extern uint32_t auto_speed;
extern uint32_t auto_accel;
extern uint32_t auto_decel;

extern uint32_t sigan_speed;
extern uint32_t sigan_accel;
extern uint32_t sigan_decel;

extern uint32_t heny_speed;
extern uint32_t heny_accel;
extern uint32_t heny_decel;

void Control(void);

#endif
