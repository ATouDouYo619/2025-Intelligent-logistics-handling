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

//uint32_t auto_speed = 5;
//uint32_t auto_accel = 2;
//uint32_t auto_decel = 2;

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
/***************************************出库获取任务并前往物料盘***************************************************/
	
    //获取任务并前往物料盘
    Qian_Jin(6400 * 1, step_accel, step_decel, set_speed);while (motor_sta != STOP);
    Zuo_PingYi(6400 * 0.85, heny_accel, heny_decel, heny_speed);while (motor_sta != STOP);
	Finish_Attitude();
    Qian_Jin(6400 * 1.27, step_accel, step_decel, set_speed);while (motor_sta != STOP);
    Code_Move();
	Holder_Control(Holder_Out);
	Delay_ms(300);		
    Qian_Jin(6400 * 3.56, step_accel, step_decel, set_speed);while (motor_sta != STOP);
	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
    //靠近物料盘（改动）
    You_PingYi(6400 * 0.718, heny_accel, heny_decel, heny_speed);while (motor_sta != STOP);
	
/********************************************开启检测颜色坐标*******************************************************/
	
	UART2_MaixCam_SendByte(0x04);   //开启检测颜色模式
	UART2_MaixCam_SendByte(0x04);
	UART2_MaixCam_SendByte(0x04);
	Delay_ms(100);
	
	WuLiao_Heng_Rectify();

	UART2_MaixCam_SendByte(0x03);  
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	Delay_ms(500);
	
/***************************************开启检测颜色模式并抓取3次***************************************************/
	
	UART2_MaixCam_SendByte(0x01);   //开启检测颜色模式
	UART2_MaixCam_SendByte(0x01);
	UART2_MaixCam_SendByte(0x01);
	Delay_ms(100);

	Pan_Duan(Code_RxPacket[0]);
	Holder_Control(Holder_Out);
	Delay_ms(500);
	Paw_Control(SWITCH);
	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Pan_Duan(Code_RxPacket[1]);
	Holder_Control(Holder_Out);
	Delay_ms(500);
	Paw_Control(SWITCH);
	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Pan_Duan(Code_RxPacket[2]);
	Paw_Control(SWITCH);
	
	Delay_ms(100);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);	//关闭
	Delay_ms(100);

/***************************************前往粗加工区域***************************************************/

	Holder_Control(90);
	Zuo_PingYi(6400 * 0.6, heny_accel, heny_decel, heny_speed);	while(motor_sta != STOP);
	Hou_Tui(6400*1.72,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
	Zuo_Zi_Zhuan(6400*1.11,auto_accel,auto_decel,auto_speed);	while(motor_sta != STOP);
	Qian_Jin(6400*6.52,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
	Zuo_Zi_Zhuan(6400*1.13,auto_accel,auto_decel,auto_speed);	while(motor_sta != STOP);

/***************************************进行粗加工圆环校准***************************************************/

	Paw_Control(SWITCH);
	
	You_PingYi(6400*0.38,heny_accel, heny_decel, heny_speed);	while(motor_sta != STOP);

	Holder_Control(Holder_Out);
	Delay_ms(400);

	Xiao_Jiang(6400*3.5,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
	
	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);	//开启检测色环模式

	Crosswise_Rectify_2();
	Level_Rectify_2();
	
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);	//关闭
	
	Shang_Sheng(6400*3.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);

/***************************************开启检测圆环模式并放置3次***************************************************/

	Pan_Duan1(Code_RxPacket[0]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan2(Code_RxPacket[0],Code_RxPacket[1]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan3(Code_RxPacket[0],Code_RxPacket[1],Code_RxPacket[2]);
	
/***************************************抓取地面物料3次***************************************************/

	Pan_Duan4(Code_RxPacket[0],Code_RxPacket[1],Code_RxPacket[2]);
	
/***************************************前往精加工区域***************************************************/

	Zuo_PingYi(6400 * 0.5, heny_accel, heny_decel, heny_speed);		while (motor_sta != STOP);
	Holder_Control(90);
	Hou_Tui(6400*3.5,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	You_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);		while(motor_sta != STOP);
	Hou_Tui(6400*3.15,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	
	You_PingYi(6400 * 0.19, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);
	
	Holder_Control(Holder_Out);
	Delay_ms(350);
	
	Xiao_Jiang(6400*3.5,sigan_accel,sigan_decel,sigan_speed);		while(sigan_sta!= STOP);
	
/***************************************进行精加工圆环校准***************************************************/

	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);	//开启检测色环模式
	
	Crosswise_Rectify_3();
	Level_Rectify_3();
	
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);	//关闭

	Shang_Sheng(6400*3.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);

/***************************************开启检测圆环模式并放置3次***************************************************/

	Pan_Duan5(Code_RxPacket[0]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan6(Code_RxPacket[0],Code_RxPacket[1]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan7(Code_RxPacket[0],Code_RxPacket[1],Code_RxPacket[2]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);

//	
/***************************************第二轮前往物料盘***************************************************/

	Zuo_PingYi(6400 * 0.5, heny_accel, heny_decel, heny_speed);		while (motor_sta != STOP);
	Hou_Tui(6400*3.72,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	You_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);		while(motor_sta != STOP);
	Hou_Tui(6400*1.66,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	You_PingYi(6400 * 0.275, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);
	
/********************************************开启检测颜色坐标*******************************************************/
	
	UART2_MaixCam_SendByte(0x04);   //开启检测颜色模式
	UART2_MaixCam_SendByte(0x04);
	UART2_MaixCam_SendByte(0x04);
	Delay_ms(100);
	
	WuLiao_Heng_Rectify();

	UART2_MaixCam_SendByte(0x03);  
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	Delay_ms(500);

	
/***************************************第二轮抓取物料盘物料***************************************************/

	UART2_MaixCam_SendByte(0x01);
	UART2_MaixCam_SendByte(0x01);
	UART2_MaixCam_SendByte(0x01);	//开启检测色环模式

	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan(Code_RxPacket[4]);
	Holder_Control(Holder_Out);
	Delay_ms(500);
	Paw_Control(SWITCH);
	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Pan_Duan(Code_RxPacket[5]);
	Holder_Control(Holder_Out);
	Delay_ms(500);
	Paw_Control(SWITCH);
	Xiao_Jiang(6400*1.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Pan_Duan(Code_RxPacket[6]);
	Paw_Control(SWITCH);
	
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);	//关闭
	Delay_ms(100);
	
/***************************************第二轮前往粗加工***************************************************/

	Holder_Control(90);
	Zuo_PingYi(6400 * 0.6, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);
	Hou_Tui(6400*1.55,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
	Zuo_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);	while(motor_sta != STOP);
	Qian_Jin(6400*6.70,step_accel,step_decel,set_speed);			while(motor_sta != STOP);
	Zuo_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);	while(motor_sta != STOP);
	
/***************************************进行第二轮粗加工圆环校准***************************************************/

	Paw_Control(SWITCH);
	
	You_PingYi(6400*0.23,heny_accel, heny_decel, heny_speed);	while(motor_sta != STOP);
	Holder_Control(Holder_Out);
	Delay_ms(350);

	Xiao_Jiang(6400*3.5,sigan_accel,sigan_decel,sigan_speed);	while(sigan_sta!= STOP);
	
/***************************************开启检测圆环模式并放置3次***************************************************/

	JY61_Rectify(177);
	
	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);
	UART2_MaixCam_SendByte(0x02);	//开启检测色环模式

	Crosswise_Rectify();
	Level_Rectify();
	
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);	//关闭

	Shang_Sheng(6400*3.5,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Pan_Duan1(Code_RxPacket[4]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan2(Code_RxPacket[4],Code_RxPacket[5]);
	Shang_Sheng(6400*3.52,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	Pan_Duan3(Code_RxPacket[4],Code_RxPacket[5],Code_RxPacket[6]);
	
	Pan_Duan8(Code_RxPacket[4],Code_RxPacket[5],Code_RxPacket[6]);

/***************************************二轮前往精加工***************************************************/

	Zuo_PingYi(6400 * 0.5, heny_accel, heny_decel, heny_speed);		while (motor_sta != STOP);
	Holder_Control(90);
	Hou_Tui(6400*3.2,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	You_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);		while(motor_sta != STOP);
	
	Pan_Duan9(Code_RxPacket[4]);
	
	JY61_Rectify(88);

	You_PingYi(6400 * 0.32, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);
	Holder_Control(Holder_Out);	
	Delay_ms(350);
	Xiao_Jiang(6400*2.85,sigan_accel,sigan_decel,sigan_speed);		while(sigan_sta!= STOP);
	
/***************************************垛码矫正***************************************************/
	
	UART2_MaixCam_SendByte(0x04);
	UART2_MaixCam_SendByte(0x04);
	UART2_MaixCam_SendByte(0x04);
	Delay_ms(200);

	DuoMa_Heng_Rectify();
	DuoMa_Ping_Rectify();
	
	Delay_ms(100);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);
	UART2_MaixCam_SendByte(0x03);

/***************************************垛码放置***************************************************/

	Shang_Sheng(6400*2.85,sigan_accel,sigan_decel,sigan_speed);		while(sigan_sta!= STOP);
	
	Pan_Duan10(Code_RxPacket[4],Code_RxPacket[5],Code_RxPacket[6]);
/***********************************************************************************************/

	Shang_Sheng(6400*1.67,sigan_accel,sigan_decel,sigan_speed);while(sigan_sta!= STOP);
	
	Holder_Control(90);
	Zuo_PingYi(6400 * 0.4, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);
	
	Hou_Tui(6400*3.72,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	
	You_Zi_Zhuan(6400*1.126,auto_accel,auto_decel,auto_speed);		while(motor_sta != STOP);
	
	Hou_Tui(6400*7.58,step_accel,step_decel,set_speed);				while(motor_sta != STOP);
	
	You_PingYi(6400 * 0.5, heny_accel, heny_decel, heny_speed);	while (motor_sta != STOP);

}
