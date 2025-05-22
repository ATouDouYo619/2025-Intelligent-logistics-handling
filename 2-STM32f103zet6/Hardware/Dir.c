#include "Dir.h"

MotorControl motors[5] = 
{
    {GPIOD, GPIO_Pin_8}, 	// MOTOR1
    {GPIOD, GPIO_Pin_9}, 	// MOTOR2
    {GPIOD, GPIO_Pin_10}, 	// MOTOR3
    {GPIOD, GPIO_Pin_11},  	// MOTOR4
	{GPIOD, GPIO_Pin_13}  	// MOTOR5
};							// Dir

void SetMotor_Direction(MotorType motor, MotorDir dir)	//设置单个步进电机方向
{
    if (dir == Zheng) 
    {
		GPIO_SetBits(motors[motor].port, motors[motor].pin); // Set high for forward
	}
	
    else if (dir == Fun) 
    {
		GPIO_ResetBits(motors[motor].port, motors[motor].pin); // Set low for backward
	}
}

void SetAllMotorsDirection(MotorDir dir1, MotorDir dir2, MotorDir dir3, MotorDir dir4)	//设置多个步进电机方向
{
    SetMotor_Direction(MOTOR1, dir1);
    SetMotor_Direction(MOTOR2, dir2);
    SetMotor_Direction(MOTOR3, dir3);
    SetMotor_Direction(MOTOR4, dir4);
}

void SetMotor_En(StepMotorType step, MotorEn en) 	//设置步进电机使能引脚状态
{
    switch (step) {
        case Rise:
            if (en == En) 
			{
                GPIO_ResetBits(GPIO_Type_EN_Pin, GPIO_EN_Two_Pin);
            } else if (en == DisEn) 
			{
                GPIO_SetBits(GPIO_Type_EN_Pin, GPIO_EN_Two_Pin);
            }
            break;
        case Drop:
            if (en == En) 
			{
                GPIO_ResetBits(GPIO_Type_EN_Pin, GPIO_EN_One_Pin);
            } else if (en == DisEn) {
                GPIO_SetBits(GPIO_Type_EN_Pin, GPIO_EN_One_Pin);
            }
            break;
    }
}

void MoveSetDirection(MoveDir Behavior)		//底盘轮子方向 前后左右
{
	SetMotor_En(Drop,En);
    switch (Behavior) 
	{
        case Forward:
            SetAllMotorsDirection(Fun, Fun, Zheng, Zheng);
            break;
        case Backward:
            SetAllMotorsDirection(Zheng, Zheng, Fun, Fun);
            break;
        case Right:
            SetAllMotorsDirection(Fun, Zheng, Fun, Zheng);	 		
            break;
		case Left:
            SetAllMotorsDirection(Zheng, Fun, Zheng, Fun);	 			
			break;
		
		case Left_rotation:
            SetAllMotorsDirection(Zheng, Zheng, Zheng, Zheng);	 			
			break;
		case Right_rotation:
            SetAllMotorsDirection(Fun, Fun, Fun, Fun);	 			
			break;	
	}
}

void UpSeSetDirection(UpDir dir)	//直线导轨方向
{
	switch(dir)
	{
		case UP:
			SetMotor_Direction(MOTOR5,Zheng);
			break;
		case DROP:
			SetMotor_Direction(MOTOR5,Fun);
			break;
	}
}


