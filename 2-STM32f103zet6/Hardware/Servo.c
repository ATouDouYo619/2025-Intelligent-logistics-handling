#include "Servo.h"


void TIM4_Servo_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; // PB6, PB7, PB8
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_InternalClockConfig(TIM4);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;  // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;  // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);
}

void PWM_TIM4_SetCompare(TIM_Channel channel, uint16_t compare)
{
    switch (channel) 
	{
        case TIM_CHANNEL_1:
            TIM_SetCompare1(TIM4, compare);
            break;
        case TIM_CHANNEL_2:
            TIM_SetCompare2(TIM4, compare);
            break;
        case TIM_CHANNEL_3:
            TIM_SetCompare3(TIM4, compare);
            break;
    }
}

void Servo_TIM4_Choose(ServoType Servo_Choose, TIM_Channel channel, float compare)
{
    uint16_t pwm_value = compare / 270.0 * 2000 + 500;

    switch (Servo_Choose)
	{
		case Servo_1:
		case Servo_2:
			PWM_TIM4_SetCompare(channel, pwm_value);
			break;
	}
}

void Servo_TIM4_Choose2(TIM_Channel channel, float compare)
{
	uint16_t pwm_value = compare / 270.0 * 2000 + 500;
	
	PWM_TIM4_SetCompare(channel, pwm_value);
}


void Paw_Control(float compare)
{
	Servo_TIM4_Choose(Servo_1,TIM_CHANNEL_1,compare);
}

void Holder_Control(float compare)
{
	Servo_TIM4_Choose(Servo_2,TIM_CHANNEL_2,compare);	
}

void Material_tray_Control(float compare)
{
	Servo_TIM4_Choose2(TIM_CHANNEL_3,compare);
}

