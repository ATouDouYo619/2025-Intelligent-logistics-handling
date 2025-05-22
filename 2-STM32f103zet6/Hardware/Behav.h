#ifndef __BEHAV_H
#define __BEHAV_H

#include "stm32f10x.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>

// 横向矫正
// 水平矫正

//一轮粗
#define Heng 123	//125
#define Ping 152	//164

//一轮精
#define Heng2 121	//122
#define Ping2 148	//152

//二轮粗
#define Heng3 122	//125
#define Ping3 152	//162

//垛码
#define DuoMa_Heng 	118
#define DuoMa_Ping 	154

void Start_Attitude(void);    // 初始姿态
void Finish_Attitude(void);   // 结束姿态
void Code_Move(void);         // 二维码移动

/******************************************************************************************/
void Level_Rectify_2(void);  	// 一轮粗加工水平校正2
void Level_Rectify_3(void);		// 一轮精加工水平校正3

/******************************************************************************************/
void Level_Rectify(void);     	// 二轮粗加工水平矫正

/******************************************************************************************/
void Crosswise_Rectify_2(void);	// 一轮粗加工横向矫正2
void Crosswise_Rectify_3(void);	// 一轮精加工横向矫正3

/******************************************************************************************/
void Crosswise_Rectify(void); 	// 二轮粗加工横向矫正

/******************************************************************************************/
void Zhua_Qu_Material(uint8_t i);    // 抓取物料盘物料
void Fang_Zhi_FirstFloor(uint8_t i); // 放置物料盘物料至第一层
void Zhua_Qu_FirstFloor(uint8_t i);  // 抓取地面物料

/******************************************************************************************/
void Pan_Duan(uint8_t Code); 	// 	抓3

/******************************************************************************************/
void Pan_Duan1(uint8_t Code); 									// 	放1
void Pan_Duan2(uint8_t Code1, uint8_t Code2); 					//	放2
void Pan_Duan3(uint8_t Code1, uint8_t Code2, uint8_t Code3);	//	放3

/******************************************************************************************/
void Pan_Duan4(uint8_t Code1, uint8_t Code2, uint8_t Code3);	//抓地123

/******************************************************************************************/
void Pan_Duan5(uint8_t Code); 									// 	精放1
void Pan_Duan6(uint8_t Code1, uint8_t Code2); 					//	精放2
void Pan_Duan7(uint8_t Code1, uint8_t Code2, uint8_t Code3);	//	精放3

void Duo_Ma(uint8_t Code);	//垛码动作

/******************************************************************************************/
void Pan_Duan8(uint8_t Code1, uint8_t Code2, uint8_t Code3);	//二轮抓地123

void Pan_Duan9(uint8_t Code1);

void Pan_Duan10(uint8_t Code1, uint8_t Code2, uint8_t Code3);	//垛码123

void JY61_Rectify(float Expect_Yaw);

void DuoMa_Ping_Rectify(void);
void DuoMa_Heng_Rectify(void);

void WuLiao_Heng_Rectify(void);

#endif
