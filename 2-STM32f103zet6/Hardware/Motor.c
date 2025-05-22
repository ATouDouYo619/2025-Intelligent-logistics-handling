#include "stm32f10x.h"                  // Device header
#include "Motor.h"  
#include "Motor_Init.h"  
#include <math.h>
#include "Dir.h"

speedRampData srd= {STOP,0,0,0,0,0,0};	//加减速变量

uint8_t  motor_sta        = 0;			//电机状态
uint8_t  sigan_sta        = 0;			//导轨状态

/**
	* @brief 电机加减速
	* @param step   步数
	* @param accel  加速度，	为accel*0.1*rad/sec^2  10倍且2个脉冲
	* @param decel  减速度，	为decel*0.1*rad/sec^2 
	* @param speed  最大速度，	为speed*0.1*rad/sec
	* @return null
 */

void MOTOR_Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
		
    uint16_t tim_count; 														//用于存放中断时刻的计数值
    unsigned int max_s_lim;                                     				//达到最大速度时的步数
    unsigned int accel_lim;														//必须开始减速的步数（如果还没有加速度到最大速度时）

    if(motor_sta!= STOP)  														//只允许步进电机在停止时才能继续
			return;			
    if(step < 0)   																//逆时针
		{
			step = -step;      
		}		     
    if(step == 1)   															//如果只移动一步
    {       
      srd.accel_count = -1; 													//只走一步
      srd.run_state = DECEL;													//减速状态
      srd.step_delay = 1000;													//短延时
     }
	 else if(step != 0)  														//步数为0时才能移动
    {					
				srd.min_delay = (int32_t)(A_T_x10/speed);
				srd.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);
																																						 //tep_delay = 1/tt * sqrt(2*alpha/accel)
																																						 //step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
    
				if(max_s_lim == 0)																							
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 									 
   
				if(accel_lim == 0) 																									
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																					 
				{
					srd.decel_val = accel_lim - step;																	
				}
				else
				{
					srd.decel_val = -(max_s_lim*accel/decel);													 
				}
				
				if(srd.decel_val == 0) 																						
				{
					srd.decel_val = -1;
				}    
				srd.decel_start = step + srd.decel_val;															
				
				if(srd.step_delay <= srd.min_delay)																	
				{
					srd.step_delay = srd.min_delay;
					srd.run_state = RUN;
				}
				else
				{
					srd.run_state = ACCEL;
				}
    
				srd.accel_count = 0;																								
		
			}
			motor_sta = 1;  																										
			tim_count = TIM_GetCounter(TIM3);																			
			TIM_SetCompare1(TIM3,tim_count+srd.step_delay/2);												
			TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);																		
			TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Enable);
			
			TIM_SetCompare2(TIM3,tim_count+srd.step_delay/2);												
			TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);															
			TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
			
			TIM_SetCompare3(TIM3,tim_count+srd.step_delay/2);												
			TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);														
			TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Enable);
			
			TIM_SetCompare4(TIM3,tim_count+srd.step_delay/2);											
			TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);														
			TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
			
			TIM_Cmd(TIM3, ENABLE);																							
}

void SiGan_Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
		
    uint16_t tim_count; 																							
    unsigned int max_s_lim;                                     					 
    unsigned int accel_lim;																							 

		if(sigan_sta!= STOP)  																					 
			return;			
    if(step < 0)   																					 
			{
				step = -step;      
			}		   
    if(step == 1)   																			 
    {       
      srd.accel_count = -1; 																	 
        
      srd.run_state = DECEL;													 
        
      srd.step_delay = 1000;									 

     }
    
    else if(step != 0)  											 
    {					
				srd.min_delay = (int32_t)(A_T_x10/speed);															 
				srd.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		 
																																			 
																																					 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));						 
    
				if(max_s_lim == 0)																				 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 							 
   
				if(accel_lim == 0) 													 
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)											 
				{
					srd.decel_val = accel_lim - step;						 
				}
				else
				{
					srd.decel_val = -(max_s_lim*accel/decel);				 
				}
   
				if(srd.decel_val == 0) 									 
				{
					srd.decel_val = -1;
				}    
				srd.decel_start = step + srd.decel_val;		 
 
    
				if(srd.step_delay <= srd.min_delay)								 
				{
					srd.step_delay = srd.min_delay;
					srd.run_state = RUN;
				}
				else
				{
					srd.run_state = ACCEL;
				}
    
				srd.accel_count = 0;										 
		
			}
			sigan_sta = 1;  													 
			tim_count = TIM_GetCounter(TIM2);									 
			TIM_SetCompare2(TIM2,tim_count+srd.step_delay/2);					 
			TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);									 
			TIM_CCxCmd(TIM2,TIM_Channel_1,TIM_CCx_Enable);
			TIM_Cmd(TIM2, ENABLE);												 
}

/**
 * @brief T型加减速决策
 * 放在中断中执行
 */
void speed_decision(void)                                       
{
		__IO uint32_t tim_count=0;
		__IO uint32_t tmp = 0;  
	  uint16_t new_step_delay=0;                                         
	  __IO static uint16_t last_accel_delay=0;                             
	  __IO static uint32_t step_count = 0; 															
	  __IO static int32_t rest = 0;																						
	  __IO static uint8_t i=0;																								
	 
	  if ((TIM_GetITStatus(TIM3, TIM_IT_CC1)== SET) ||(TIM_GetITStatus(TIM3, TIM_IT_CC2)== SET)
          ||TIM_GetITStatus(TIM3, TIM_IT_CC3)== SET||(TIM_GetITStatus(TIM3, TIM_IT_CC4)== SET))
	  {	  
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);															
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		  tim_count = TIM_GetCounter(TIM3);	
			tmp = tim_count+srd.step_delay/2;
		  TIM_SetCompare1(TIM3,tmp);																				
			TIM_SetCompare2(TIM3,tmp);
			TIM_SetCompare3(TIM3,tmp);
			TIM_SetCompare4(TIM3,tmp);
			i++; 
			if(i==2)																														
			{
				i=0; 
				switch(srd.run_state)
				{
					case STOP:																											
						step_count = 0;
						rest = 0;
						TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
					  TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Disable);
						TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
					  TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Disable);
						TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
					  TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);
						TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
					  TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
					  TIM_Cmd(TIM3, DISABLE);					                     
						motor_sta = 0;  
						break;
					
					case ACCEL:																									
					step_count++;
					srd.accel_count++;
					new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4 * srd.accel_count + 1));
					rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);	   
					
					if(step_count >= srd.decel_start) 														
					{
						srd.accel_count = srd.decel_val;															
						srd.run_state = DECEL;																			
					}
					
					else if(new_step_delay <= srd.min_delay)												
					{
						last_accel_delay = new_step_delay;														
						new_step_delay = srd.min_delay;   														
						rest = 0;            																		     
						srd.run_state = RUN;																			
					}
					break;
					
					case RUN:
			  step_count++;  																									  
			  new_step_delay = srd.min_delay;   																	 
			  if(step_count >= srd.decel_start)   														
						{
				srd.accel_count = srd.decel_val;  																
				new_step_delay = last_accel_delay;																	
				srd.run_state = DECEL;           															
			  }
			  break;
						
					case DECEL:
			  step_count++;  																				

			  srd.accel_count++; 																						
			  new_step_delay = srd.step_delay - (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); 
			  rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);			
			  if(srd.accel_count >= 0) 																					
			  {
					srd.run_state = STOP;
			  }
			  break;
				}
			srd.step_delay = new_step_delay; 																
			}
		}
}

void SiGan_speed_decision(void)
{
	__IO uint32_t tim_count=0;
	__IO uint32_t tmp = 0;  
  uint16_t new_step_delay=0;                                     
  __IO static uint16_t last_accel_delay=0;                                  
  __IO static uint32_t step_count = 0; 																		
  __IO static int32_t rest = 0;																						
  __IO static uint8_t i=0;																						
 
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1)== SET)
  {	  
		
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);																	
	  tim_count = TIM_GetCounter(TIM2);																				
		tmp = tim_count+srd.step_delay/2;
	  TIM_SetCompare1(TIM2,tmp);																							
		i++; 
		if(i==2)																																	
		{
			i=0; 
			switch(srd.run_state)
			{
				case STOP:																													
					step_count = 0;
					rest = 0;
					
					TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
				  TIM_CCxCmd(TIM2,TIM_Channel_1,TIM_CCx_Disable);
				  TIM_Cmd(TIM2, DISABLE);					                           
					sigan_sta = 0;  
					break;
				
				case ACCEL:																												
				step_count++;
				srd.accel_count++;
				new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4 * srd.accel_count + 1));
				rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);				
				
					if(step_count >= srd.decel_start) 																
					{
						srd.accel_count = srd.decel_val;																
						srd.run_state = DECEL;																		
					}
					
					else if(new_step_delay <= srd.min_delay)												
					{
						last_accel_delay = new_step_delay;											
						new_step_delay = srd.min_delay;   												
						rest = 0;            															           
						srd.run_state = RUN;															
					}
					break;
					
				case RUN:
          step_count++;  																						  
          new_step_delay = srd.min_delay;   																 
          if(step_count >= srd.decel_start)   										
					{
            srd.accel_count = srd.decel_val;  									
            new_step_delay = last_accel_delay;												
            srd.run_state = DECEL;           												
          }
          break;
					
				case DECEL:
          step_count++;  																						

          srd.accel_count++; 																					
          new_step_delay = srd.step_delay - (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1));
          rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);
          if(srd.accel_count >= 0) 																					
          {
            srd.run_state = STOP;
          }
          break;
			}
			 srd.step_delay = new_step_delay; 																
		}
	}
}

//定时器2	直线导轨
void TIM2_IRQHandler(void)
{
	SiGan_speed_decision();
}

//定时器3	4个轮子
void TIM3_IRQHandler(void)
{
	speed_decision();
}

/****************************************************底盘轮子动作********************************************************/

void Qian_Jin(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)		//前进
{
	MoveSetDirection(Forward);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

void Hou_Tui(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)		//后退
{
	MoveSetDirection(Backward);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

void Zuo_PingYi(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)	//左平移
{
	MoveSetDirection(Left);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

void Zuo_Zi_Zhuan(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)	//原地左转
{
	MoveSetDirection(Left_rotation);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

void You_PingYi(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)	//右平移
{
	MoveSetDirection(Right);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

void You_Zi_Zhuan(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)	//原地右转
{
	MoveSetDirection(Right_rotation);
	MOTOR_Move(step, accel, decel, speed);
//	SetMotor_En(Drop,DisEn);
}

/****************************************************直线导轨动作********************************************************/

void Shang_Sheng(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)		//上升
{
	UpSeSetDirection(UP);
	SiGan_Move(step, accel, decel, speed);
//	SetMotor_En(Rise,DisEn);
}

void Xiao_Jiang(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)		//上升
{
	UpSeSetDirection(DROP);
	SiGan_Move(step, accel, decel, speed);
//	SetMotor_En(Rise,DisEn);
}
