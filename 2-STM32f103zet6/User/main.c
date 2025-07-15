#include "stm32f10x.h" // Device header
#include <string.h>
#include "Delay.h"
#include "OLED.h"
#include "UART1.h"
#include "UART2.h"
#include "UART3.h"
#include "usart4.h"
#include "Motor.h"
#include "Motor_Init.h"
#include "Servo.h"
#include "control.h"
#include "Behav.h"
#include "Key.h"
#include "jy61p.h"

int main(void)
{
	Key_Init();
	
	UART1_Scrn_Init(Scrn_Bound);
	UART2_MaixCam_Init(MaixCam_Bound);
	UART3_Code_Init(Code_Bound);
	
	usart4_Init(JY61_Bound);

	MOTOR_TIM2_GPIO_Init();
	MOTOR_TIM2_Init();
	MOTOR_TIM3_GPIO_Init();
	MOTOR_TIM3_Init();
	TIM4_Servo_Init();	

	Jy61P_Init_Zero();
	Start_Attitude();
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1)
	{	Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 1);
		Delay_ms(20);
	}
	
	Control();
	
	while(1){}
}	
