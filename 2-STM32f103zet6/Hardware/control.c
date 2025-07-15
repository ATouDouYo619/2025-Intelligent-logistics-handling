#include "control.h"
#include "Behav.h"
#include "motor.h"
#include "UART2.h"
#include "UART3.h"
#include "Delay.h"
#include "Servo.h"

//轮子前进后退速度
uint32_t set_speed =  42; 		// 最大速度,	为speed*0.1*rad/sec
uint32_t step_accel = 8; 		// 加速度,	为decel*0.1*rad/sec^2
uint32_t step_decel = 8; 		// 减速度,	为accel*0.1*rad/sec^2

//轮子转90度速度
uint32_t auto_speed = 5;
uint32_t auto_accel = 2;
uint32_t auto_decel = 2;

//丝杠速度
uint32_t sigan_speed = 150;
uint32_t sigan_accel = 90;
uint32_t sigan_decel = 90;

//轮子平移速度
uint32_t heny_speed = 8;
uint32_t heny_accel = 4;
uint32_t heny_decel = 4;

void Control(void)
{
/***************************************出库获取任务并前往物料盘*****************************************************/


/********************************************开启检测颜色坐标*******************************************************/


/***************************************开启检测颜色模式并抓取3次***************************************************/


/*********************************************前往粗加工区域*******************************************************/


............


}
