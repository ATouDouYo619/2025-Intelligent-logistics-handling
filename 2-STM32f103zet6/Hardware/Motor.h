#ifndef __Motor_H
#define __Motor_H
#include "stm32f10x.h"                  // Device header
#include "math.h"

extern uint8_t  motor_sta;
extern uint8_t  sigan_sta;

typedef struct
{
	uint8_t run_state;    		//电机旋转状态
	uint8_t dir ;        		//电机选装方向
	int step_delay;   			//下个脉冲周期（时间间隔）启动时为加速度
	int decel_start;  			//启动速度位置
	int decel_val;    			//减速阶段步数
	int min_delay;    			//最小脉冲周期（最大速度，即匀速段的速度）
	int accel_count;  			//加速阶段计数值
}speedRampData;



#define TRUE     1
#define FALSE    0


/*4个状态*/
#define STOP              0 //停止
#define ACCEL             1 //加速
#define DECEL             2 //减速
#define RUN               3 //匀速



#define TIM_PRESCALER      	32
#define T1_FREQ            (SystemCoreClock/(TIM_PRESCALER+1))	//定时器频率

#define STEP_ANGLE			1.8									//步距角
#define FSPR              	6400        						//单圈步进电机步数

#define MICRO_STEP       	32         							//细分数
#define SPR               	(FSPR*MICRO_STEP)  					//16细分步数

#define ALPHA             ((float)(2*3.14159/SPR))  
#define A_T_x10           ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148       ((float)((T1_FREQ*0.676)/10))
#define A_SQ              ((float)(2*100000*ALPHA)) 
#define A_x200            ((float)(200*ALPHA))
	
void MOTOR_Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void SiGan_Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

void speed_decision(void);
void SiGan_speed_decision(void);

void Qian_Jin(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void Hou_Tui(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

void Zuo_PingYi(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void Zuo_Zi_Zhuan(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

void You_PingYi(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void You_Zi_Zhuan(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

void Shang_Sheng(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void Xiao_Jiang(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);

#endif
