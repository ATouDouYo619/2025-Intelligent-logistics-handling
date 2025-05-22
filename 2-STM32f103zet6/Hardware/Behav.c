#include "Behav.h"
#include "Motor.h"
#include "Motor_Init.h"
#include "Servo.h"
#include "control.h"
#include "Delay.h"
#include "UART1.h"
#include "UART2.h"
#include "UART3.h"
#include "OLED.h"
#include "jy61p.h"
#include "usart4.h" 

void Start_Attitude(void)	//出库姿态,避免压线
{
	Shang_Sheng(6400*3.75,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Material_tray_Control(Start_3);
	Holder_Control(Start_2);
	Paw_Control(Start_1);
	Delay_ms(1500);
}

void Finish_Attitude(void)
{
	Material_tray_Control(Start_3);
	Paw_Control(Start_1);
	Delay_ms(200);
}

void Code_Move(void) // 二维码识别函数,若第一次扫不到二维码,将会前后移动,直到获取任务成功
{
	int u = 1;
	float t = 0.1;
	while (u)
	{
		if (Code_RxFlag == 1)                                                                                                                                                                                                                                                                                                                                          
		{
			Delay_ms(20);
			HMI_send_string("t0.txt", (char *)Code_RxPacket);
			u = 0;
			t = 0;
		}
		else
		{
			Qian_Jin(6400 * t, 		4, 5, 10);
			while (motor_sta != STOP);
			Hou_Tui(6400 * t * 2, 	4, 5, 10);
			while (motor_sta != STOP);
			Qian_Jin(6400 * t, 		4, 5, 10);
			while (motor_sta != STOP);
			t = t + 0.1;
		}
	}
}

void Level_Rectify(void) 	// 水平校正
{
	int m = 1;
	
	X_Coord = 0;
	
	while (m)
	{
		int ck = Ping3, ek = 0;

		// 计算误差
        ek = ck - X_Coord;

        // 限幅（强制将误差限制在 ±5 以内）
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 允许误差在 ±2 内跳出循环
        if (abs(ek) <= 1)   // 新增判断条件
        {
            m = 0;          // 退出循环
            continue;       // 跳过后续移动操作
        }

        // 根据误差方向移动
        if (ek < 0)
        {
            ek = -ek;
            Hou_Tui(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            Qian_Jin(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
	}
}

void Level_Rectify_2(void)    // 水平校正
{
    int m = 1;

    while (m)
    {
        int ck = Ping, ek = 0;

        // 计算误差
        ek = ck - X_Coord;

        // 限幅（强制将误差限制在 ±5 以内）
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 允许误差在 ±2 内跳出循环
        if (abs(ek) <= 1)   // 新增判断条件
        {
            m = 0;          // 退出循环
            continue;       // 跳过后续移动操作
        }

        // 根据误差方向移动
        if (ek < 0)
        {
            ek = -ek;
            Hou_Tui(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            Qian_Jin(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}

void Level_Rectify_3(void)    // 水平校正
{
    int m = 1;
	
	X_Coord = 0;
	
    while (m)
    {
        int ck = Ping2, ek = 0;

        // 计算误差
        ek = ck - X_Coord;

        // 限幅（强制将误差限制在 ±5 以内）
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 允许误差在 ±2 内跳出循环
        if (abs(ek) <= 1)   // 新增判断条件
        {
            m = 0;          // 退出循环
            continue;       // 跳过后续移动操作
        }

        // 根据误差方向移动
        if (ek < 0)
        {
            ek = -ek;
            Hou_Tui(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            Qian_Jin(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}


void Crosswise_Rectify(void) 	// 横向校正
{
	int m = 1;

	Y_Coord = 0;
	
	while (m)
	{
		int ck = Heng3, ek = 0;

		// 计算误差
        ek = ck - Y_Coord;

        // 限幅
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 检查是否在允许的误差范围内（±2）
        if (abs(ek) <= 1)  // 修改条件为允许浮动2
        {
            m = 0;  // 满足条件，退出循环
            continue; // 跳过后续移动操作
        }

        // 根据误差方向进行移动
        if (ek < 0)
        {
            ek = -ek;
            Zuo_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            You_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
	}
}

void Crosswise_Rectify_2(void)    // 横向校正
{
    int m = 1;

    while (m)
    {
        int ck = Heng, ek = 0;
		
        // 计算误差
        ek = ck - Y_Coord;

        // 限幅
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 检查是否在允许的误差范围内（±2）
        if (abs(ek) <= 1)  // 修改条件为允许浮动2
        {
            m = 0;  // 满足条件，退出循环
            continue; // 跳过后续移动操作
        }

        // 根据误差方向进行移动
        if (ek < 0)
        {
            ek = -ek;
            Zuo_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            You_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}

void Crosswise_Rectify_3(void)    // 横向校正
{
    int m = 1;
	
	Y_Coord = 0;
	
    while (m)
    {
        int ck = Heng2, ek = 0;

        // 计算误差
        ek = ck - Y_Coord;

        // 限幅
        if (ek > 7)
            ek = 7;
        else if (ek < -7)
            ek = -7;

        // 检查是否在允许的误差范围内（±2）
        if (abs(ek) <= 1)  // 修改条件为允许浮动2
        {
            m = 0;  // 满足条件，退出循环
            continue; // 跳过后续移动操作
        }

        // 根据误差方向进行移动
        if (ek < 0)
        {
            ek = -ek;
            Zuo_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            You_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}


int C = 1;

void Zhua_Qu_Material(uint8_t i)			//抓取物料盘物料
{
	if (i == 0x31) // 红色
	{
		Material_tray_Control(Red_Material);
		Paw_Control(CLOSE);
		Delay_ms(480);
		Shang_Sheng(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(600);
		Xiao_Jiang(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(LITOPEN);
		Delay_ms(300);
		Shang_Sheng(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
//		Holder_Control(Holder_Out);
//		Delay_ms(500);
//		Paw_Control(SWITCH);
	}

	if (i == 0x32) // 绿色
	{
		Material_tray_Control(Green_Material);
		Paw_Control(CLOSE);
		Delay_ms(480);
		Shang_Sheng(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(600);
		Xiao_Jiang(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(LITOPEN);
		Delay_ms(300);
		Shang_Sheng(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
//		Holder_Control(Holder_Out);
//		Delay_ms(500);
//		Paw_Control(SWITCH);		
	}

	if (i == 0x33) // 蓝色
	{
		Material_tray_Control(Blue_Material);
		Paw_Control(CLOSE);
		Delay_ms(480);
		Shang_Sheng(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(600);
		Xiao_Jiang(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(LITOPEN);
		Delay_ms(300);
		Shang_Sheng(6400*1.1,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
//		Holder_Control(Holder_Out);
//		Delay_ms(500);
//		Paw_Control(SWITCH);		
	}

//	if (C < 3)
//	{
//		Holder_Control(Holder_Out);
//		Paw_Control(SWITCH);
//		Delay_ms(500);
//		Xiao_Jiang(6400 * 0, step_accel, step_decel, set_speed);
//		while (sigan_sta != STOP);
//	}
//	else
//	{
//		C = 0;
//		Paw_Control(SWITCH);
//	}
//	C++;
}

void Fang_Zhi_FirstFloor(uint8_t i)			// 放置物料到第一层
{
	if (i == 0x31) // 红色
	{
		
		Material_tray_Control(Red_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(720);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);	
		Delay_ms(720);
		Xiao_Jiang(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Delay_ms(200);
		Paw_Control(SWITCH);
		Delay_ms(500);
//		Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	}
	if (i == 0x32) // 绿色
	{
		
		Material_tray_Control(Green_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(720);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);	
		Delay_ms(720);
		Xiao_Jiang(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Delay_ms(200);
		Paw_Control(SWITCH);
		Delay_ms(500);
//		Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	}
	if (i == 0x33) // 蓝色
	{
		
		Material_tray_Control(Blue_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(720);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);	
		Delay_ms(720);
		Xiao_Jiang(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Delay_ms(200);
		Paw_Control(SWITCH);
		Delay_ms(500);
//		Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	}
}

void Zhua_Qu_FirstFloor(uint8_t i)			//抓取第一层物料
{
	if (i == 0x31) // 红色
	{
		
		Material_tray_Control(Red_Material);
		Paw_Control(CLOSE);
		Delay_ms(600);
		Shang_Sheng(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(800);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);	
		Paw_Control(LITOPEN);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(720);
		Paw_Control(SWITCH);	
	}
	if (i == 0x32) // 绿色
	{	
		Material_tray_Control(Green_Material);
		Paw_Control(CLOSE);
		Delay_ms(600);
		Shang_Sheng(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(800);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);	
		Paw_Control(LITOPEN);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(720);
		Paw_Control(SWITCH);	
	}
	if (i == 0x33) // 蓝色
	{
		Material_tray_Control(Blue_Material);
		Paw_Control(CLOSE);
		Delay_ms(600);
		Shang_Sheng(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_WuLiao);
		Delay_ms(800);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);	
		Paw_Control(LITOPEN);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(720);
		Paw_Control(SWITCH);	
	}
}

void Duo_Ma(uint8_t Code)
{
	if(Code == 0x31)
	{
		Material_tray_Control(Red_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(500);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(750);
		Xiao_Jiang(6400*1.67,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(SWITCH);
		Delay_ms(500);
	}
	if(Code == 0x32)
	{
		Material_tray_Control(Green_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(500);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(750);
		Xiao_Jiang(6400*1.67,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(SWITCH);	
		Delay_ms(500);		
	}
	if(Code == 0x33)
	{
		Material_tray_Control(Blue_Material);
		Delay_ms(500);
		Holder_Control(Holder_WuLiao);
		Delay_ms(500);
		Paw_Control(LITOPEN);
		Delay_ms(500);
		Xiao_Jiang(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(CLOSE);
		Delay_ms(200);
		Shang_Sheng(6400*0.85,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Holder_Control(Holder_Out);
		Delay_ms(750);
		Xiao_Jiang(6400*1.67,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
		Paw_Control(SWITCH);	
		Delay_ms(500);
	}
}

void Pan_Duan(uint8_t Code) // 摄像头判断的物料 -----------------抓
{
	int i = 1;
	while (i)
	{
		if (Code == 0x31 && WuLiao_Colour_ID == 0x41) // 1-->红
		{
			Delay_ms(450);
			Zhua_Qu_Material(Code);
			i = 0;
//			OLED_ShowString(4, 1, "Case 1");
		}
		else if (Code == 0x32 && WuLiao_Colour_ID == 0x42) // 2-->绿
		{
			Delay_ms(450);
			Zhua_Qu_Material(Code);
			i = 0;
		}
		else if (Code == 0x33 && WuLiao_Colour_ID == 0x43) // 3-->蓝
		{
			Delay_ms(450);
			Zhua_Qu_Material(Code);
			i = 0;
		}
//		OLED_ShowHexNum(3,1,WuLiao_Colour_ID,2);
	}
}

//传入值为Code[0]  或Code[4]
void Pan_Duan1(uint8_t Code)	//判断放置------------放1
{
	if (Code == 0x33)
	{
		int k = 1;
		while (k)
		{

			if (Code == 0x33)
			{

//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k--;
			}
		}
	}
	if (Code == 0x32)
	{
		int k = 1;
		while (k)
		{
			if (Code == 0x32)
			{
//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Fang_Zhi_FirstFloor(0x32);
				k--;
			}
		}
	}
	if (Code == 0x31)
	{
		int k = 1;
		while (k)
		{
			if (Code == 0x31)
			{
//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x31);
				k--;
			}
		}
	}
	Delay_ms(100);
}

//传入值为Code[0],Code[1] 或Code[4],Code[5]
void Pan_Duan2(uint8_t Code1, uint8_t Code2)	//判断放置------------放2
{
	if (Code1 == 0x31 && Code2 == 0x32)
	{

		int k = 1;
		while (k)
		{

			if (Code1 == 0x31 && Code2 == 0x32)
			{
				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x32);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x31 && Code2 == 0x33)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x31 && Code2 == 0x33)
			{

				Qian_Jin(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x32 && Code2 == 0x31)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x32 && Code2 == 0x31)
			{

				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);

				Fang_Zhi_FirstFloor(0x31);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x32 && Code2 == 0x33)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x32 && Code2 == 0x33)
			{

				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x33 && Code2 == 0x31)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x33 && Code2 == 0x31)
			{

				Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x31);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x33 && Code2 == 0x32)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x33 && Code2 == 0x32)
			{

				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x32);
				k = 0;
			}
		}
	}
}

//传入值为Code[0],Code[1],Code[2]  或Code[4],Code[5],Code[6]
void Pan_Duan3(uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
		if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)
		{

			int k = 1;
			while (k)
			{

				if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)
				{

					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x33);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)
		{
			int k = 1;
			while (k)
			{
				if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)
				{

					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)
		{
			int k = 1;
			while (k)
			{

				if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)
				{

					Qian_Jin(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x33);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)
		{
			int k = 1;
			while (k)
			{
				if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)
				{

					Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x31);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)
		{
			int k = 1;
			while (k)
			{

				if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)
				{

					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)
		{
			int k = 1;
			while (k)
			{
				if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)
				{

					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x31);
					k--;
				}
			}
		}
}
//传入值为Code[0],Code[1],Code[2]  或Code[4],Code[5],Code[6]
void Pan_Duan4(uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
		if (Code_RxPacket[0] == 0x31 && Code_RxPacket[1] == 0x32 && Code_RxPacket[2] == 0x33)
		{
			int k = 1;
			while (k)
			{
				// delay_ms(100);

				if (Code_RxPacket[0] == 0x31 && Code_RxPacket[1] == 0x32 && Code_RxPacket[2] == 0x33)
				{
					Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					k--;
				}
			}
		}
		
		/********************************************/
		if (Code_RxPacket[0] == 0x31 && Code_RxPacket[1] == 0x33 && Code_RxPacket[2] == 0x32)
		{
			int k = 1;
			while (k)
			{

				if (Code_RxPacket[0] == 0x31 && Code_RxPacket[1] == 0x33 && Code_RxPacket[2] == 0x32)
				{
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[0] == 0x32 && Code_RxPacket[1] == 0x31 && Code_RxPacket[2] == 0x33)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[0] == 0x32 && Code_RxPacket[1] == 0x31 && Code_RxPacket[2] == 0x33)
				{
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);		while (motor_sta != STOP);

					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[0] == 0x32 && Code_RxPacket[1] == 0x33 && Code_RxPacket[2] == 0x31)
		{
			int k = 1;
			while (k)
			{

				if (Code_RxPacket[0] == 0x32 && Code_RxPacket[1] == 0x33 && Code_RxPacket[2] == 0x31)
				{
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);	while(motor_sta != STOP);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[0] == 0x33 && Code_RxPacket[1] == 0x31 && Code_RxPacket[2] == 0x32)
		{
			int k = 1;
			while (k)
			{

				if (Code_RxPacket[0] == 0x33 && Code_RxPacket[1] == 0x31 && Code_RxPacket[2] == 0x32)
				{
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[0] == 0x33 && Code_RxPacket[1] == 0x32 && Code_RxPacket[2] == 0x31)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[0] == 0x33 && Code_RxPacket[1] == 0x32 && Code_RxPacket[2] == 0x31)
				{
					Qian_Jin(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);	while(motor_sta != STOP);
					k--;
				}
			}
		}

}

/********************************************************精加工放置函数**************************/
//传入值为Code[0]  或Code[4]
void Pan_Duan5(uint8_t Code)	//判断放置------------放1
{
	if (Code == 0x33)
	{
		int k = 1;
		while (k)
		{

			if (Code == 0x33)
			{

//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k--;
			}
		}
	}
	if (Code == 0x32)
	{
		int k = 1;
		while (k)
		{
			if (Code == 0x32)
			{
//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Fang_Zhi_FirstFloor(0x32);
				k--;
			}
		}
	}
	if (Code == 0x31)
	{
		int k = 1;
		while (k)
		{
			if (Code == 0x31)
			{
//				Xiao_Jiang(6400 * 1.35, sigan_accel, sigan_decel, sigan_speed);while (sigan_sta == STOP);
//				Delay_ms(1200);
				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x31);
				k--;
			}
		}
	}
	Delay_ms(100);
}

//传入值为Code[0],Code[1] 或Code[4],Code[5]
void Pan_Duan6(uint8_t Code1, uint8_t Code2)	//判断放置------------放2
{
	if (Code1 == 0x31 && Code2 == 0x32)
	{

		int k = 1;
		while (k)
		{


			if (Code1 == 0x31 && Code2 == 0x32)
			{
				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x32);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x31 && Code2 == 0x33)
	{
		int k = 1;
		while (k)
		{



			if (Code1 == 0x31 && Code2 == 0x33)
			{

				Qian_Jin(6400 * 1.21, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x32 && Code2 == 0x31)
	{
		int k = 1;
		while (k)
		{


			if (Code1 == 0x32 && Code2 == 0x31)
			{

				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x31);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x32 && Code2 == 0x33)
	{
		int k = 1;
		while (k)
		{


			if (Code1 == 0x32 && Code2 == 0x33)
			{

				Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x33);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x33 && Code2 == 0x31)
	{
		int k = 1;
		while (k)
		{


			if (Code1 == 0x33 && Code2 == 0x31)
			{

				Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x31);
				k = 0;
			}
		}
	}
	/********************************************/
	if (Code1 == 0x33 && Code2 == 0x32)
	{
		int k = 1;
		while (k)
		{

			if (Code1 == 0x33 && Code2 == 0x32)
			{

				Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
				Fang_Zhi_FirstFloor(0x32);
				k = 0;
			}
		}
	}
}

//传入值为Code[0],Code[1],Code[2]  或Code[4],Code[5],Code[6]
void Pan_Duan7(uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
		if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)
		{

			int k = 1;
			while (k)
			{


				if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)
				{

					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x33);
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)
		{
			int k = 1;
			while (k)
			{


				if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)
				{

					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)
		{
			int k = 1;
			while (k)
			{


				if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)
				{

					Qian_Jin(6400 * 1.21, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x33);
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)
		{
			int k = 1;
			while (k)
			{


				if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)
				{

					Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)
		{
			int k = 1;
			while (k)
			{


				if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)
				{

					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)
		{
			int k = 1;
			while (k)
			{
				if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)
				{

					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					Fang_Zhi_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while(motor_sta != STOP);
					k--;
				}
			}
		}
}

void Pan_Duan8(uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
		if (Code_RxPacket[4] == 0x31 && Code_RxPacket[5] == 0x32 && Code_RxPacket[6] == 0x33)
		{
			int k = 1;
			while (k)
			{
				// delay_ms(100);

				if (Code_RxPacket[4] == 0x31 && Code_RxPacket[5] == 0x32 && Code_RxPacket[6] == 0x33)
				{
					Hou_Tui(6400 * 1.21, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);while (motor_sta != STOP);
					k--;
				}
			}
		}
		
		/********************************************/
		if (Code_RxPacket[4] == 0x31 && Code_RxPacket[5] == 0x33 && Code_RxPacket[6] == 0x32)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[4] == 0x31 && Code_RxPacket[5] == 0x33 && Code_RxPacket[6] == 0x32)
				{
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[4] == 0x32 && Code_RxPacket[5] == 0x31 && Code_RxPacket[6] == 0x33)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[4] == 0x32 && Code_RxPacket[5] == 0x31 && Code_RxPacket[6] == 0x33)
				{
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					
					Hou_Tui(6400 * 0.6, step_accel, step_decel, set_speed);		while (motor_sta != STOP);

					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[4] == 0x32 && Code_RxPacket[5] == 0x33 && Code_RxPacket[6] == 0x31)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[4] == 0x32 && Code_RxPacket[5] == 0x33 && Code_RxPacket[6] == 0x31)
				{
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);	while(motor_sta != STOP);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[4] == 0x33 && Code_RxPacket[5] == 0x31 && Code_RxPacket[6] == 0x32)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[4] == 0x33 && Code_RxPacket[5] == 0x31 && Code_RxPacket[6] == 0x32)
				{
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					k--;
				}
			}
		}
		/********************************************/
		if (Code_RxPacket[4] == 0x33 && Code_RxPacket[5] == 0x32 && Code_RxPacket[6] == 0x31)
		{
			int k = 1;
			while (k)
			{

				// delay_ms(100);

				if (Code_RxPacket[4] == 0x33 && Code_RxPacket[5] == 0x32 && Code_RxPacket[6] == 0x31)
				{
					Qian_Jin(6400*1.2,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x33);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x32);
					Xiao_Jiang(6400*3.51,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
					Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
					Zhua_Qu_FirstFloor(0x31);
					Qian_Jin(6400 * 0.6, step_accel, step_decel, set_speed);	while(motor_sta != STOP);
					k--;
				}
			}
		}
}

void JY61_Rectify(float Expect_Yaw)
{
    int m = 1;
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

    while(m)
    {
        float Actual_Yaw = Yaw; 
        float ek = Expect_Yaw - Actual_Yaw;

        // 调整角度差到[-180°, 180°)范围
        ek = fmod(ek + 180.0f, 360.0f) - 180.0f;

        if(ek > 15) ek = 15;
        else if(ek < -15) ek = -15;

        if(fabs(ek) <= 1.5)
        {
            m = 0;
            continue;
        }

        if(ek < 0)
        {
            // 顺时针转动（右转）
            float abs_ek = -ek; // 取绝对值
            You_Zi_Zhuan(6400 * abs_ek * 0.0001, 40000 * abs_ek, 1, 8000 * abs_ek); 
            while(motor_sta != STOP);
        }
        else if(ek > 0)
        {
            // 逆时针转动（左转）
            Zuo_Zi_Zhuan(6400 * ek * 0.0001, 40000 * ek, 1, 8000 * ek); 
            while(motor_sta != STOP);
        }
    }
    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
}

void Pan_Duan9(uint8_t Code1)
{
	int k = 1;
	
	while (k)
	{
		if (Code1 == 0x31)
		{
			Hou_Tui(6400*3.8,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
			k--;
		}
		else if(Code1 == 0x32)
		{
			Hou_Tui(6400*3.2,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
			k--;
		}
		else if(Code1 == 0x33)
		{
			Hou_Tui(6400*2.6,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
			k--;
		}				
	}
}

//传入值为Code_RxPacket 4,5,6
void Pan_Duan10(uint8_t Code1, uint8_t Code2, uint8_t Code3)
{
	if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)	//123
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x31 && Code2 == 0x32 && Code3 == 0x33)
			{
				Duo_Ma(0x31);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x32);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x33);
				
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				k--;
			}
		}
	}
	if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)	//132
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x31 && Code2 == 0x33 && Code3 == 0x32)
			{
				Duo_Ma(0x31);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x33);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x32);
				
				k--;
			}
		}
	}
	if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)	//213
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x32 && Code2 == 0x31 && Code3 == 0x33)
			{
				Duo_Ma(0x32);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x31);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*1.21,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
				Duo_Ma(0x33);
				
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);				
				k--;
			}
		}
	}
	if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)	//231
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x32 && Code2 == 0x33 && Code3 == 0x31)
			{
				Duo_Ma(0x32);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x33);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*1.21,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
				Duo_Ma(0x31);
				
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);				
				k--;
			}
		}
	}
	if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)	//312
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x33 && Code2 == 0x31 && Code3 == 0x32)
			{
				Duo_Ma(0x33);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*1.21,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x31);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);		while(motor_sta != STOP);
				Duo_Ma(0x32);
				
				k--;
			}
		}
	}
	if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)	//321
	{
		int k = 1;
		while (k)
		{
			if (Code1 == 0x33 && Code2 == 0x32 && Code3 == 0x31)
			{
				Duo_Ma(0x33);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x32);
				Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
				Hou_Tui(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				Duo_Ma(0x31);
				
				Qian_Jin(6400*0.6,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
				k--;
			}
		}
	}
}

void DuoMa_Ping_Rectify(void)
{
    int m = 1;
	
	X_Colour = 0;
	
    while (m)
    {
        int ck = DuoMa_Ping, ek = 0;

        // 计算误差
        ek = ck - X_Colour;

        // 限幅（强制将误差限制在 ±5 以内）
        if (ek > 12)
            ek = 12;
        else if (ek < -12)
            ek = -12;

        // 允许误差在 ±2 内跳出循环
        if (abs(ek) <= 1)   // 新增判断条件
        {
            m = 0;          // 退出循环
            continue;       // 跳过后续移动操作
        }

        // 根据误差方向移动
        if (ek < 0)
        {
            ek = -ek;
            Hou_Tui(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            Qian_Jin(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}

void DuoMa_Heng_Rectify(void)
{
    int m = 1;
	
	Y_Colour = 0;
	
    while (m)
    {
        int ck = DuoMa_Heng, ek = 0;

        // 计算误差
        ek = ck - Y_Colour;

        // 限幅
        if (ek > 12)
            ek = 12;
        else if (ek < -12)
            ek = -12;

        // 检查是否在允许的误差范围内（±2）
        if (abs(ek) <= 1)  // 修改条件为允许浮动2
        {
            m = 0;  // 满足条件，退出循环
            continue; // 跳过后续移动操作
        }

        // 根据误差方向进行移动
        if (ek < 0)
        {
            ek = -ek;
            Zuo_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
        else if (ek > 0)
        {
            You_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
            while (motor_sta != STOP);
        }
    }
}

void WuLiao_Heng_Rectify(void)
{
    int m = 1;
	Y_Colour = 0;

    while (m)
    {
        int ck = DuoMa_Heng;  // 获取当前编码器值
        int ek = 0;           // 误差
        // 如果 Y_Colour == 0，立即停止所有电机
        if (Y_Colour == 0) 
        {                 
            // 立即停止所有电机（关键操作）
            You_PingYi(6400 * 0, 2000 * ek, 1, 8000 * ek);  
        }
        else 
        {
            // 计算误差
            ek = ck - Y_Colour;

            // 限幅
            if (ek > 6) ek = 6;
            else if (ek < -6) ek = -6;

            // 检查是否在允许的误差范围内（±2）
            if (abs(ek) <= 2) 
            {
                m = 0;            // 满足条件，退出循环
            }
            else 
            {
                // 根据误差方向进行移动
                if (ek < 0) 
                {
                    ek = -ek;         // 取绝对值
                    Zuo_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
                    while (motor_sta != STOP);  // 等待电机停止
                } 
                else if (ek > 0) 
                {
                    You_PingYi(6400 * ek * 0.0001, 2000 * ek, 1, 8000 * ek);
                    while (motor_sta != STOP);  // 等待电机停止
                }
            }
        }
    }
}
