#ifndef __UART2_H
#define __UART2_H

#include "stm32f10x.h"
#include "stdint.h"
#include <stddef.h>

#define MaixCam_Bound 9600

extern uint8_t UART2_RxFlag;

extern uint8_t UART2_RxPacket[4];
extern uint8_t UART2_RxPacket_2[4];
extern uint8_t UART2_Color_ID[1];

extern uint16_t X_Coord,Y_Coord;
extern uint16_t X_Colour,Y_Colour;
extern uint8_t Color_ID;

extern uint8_t WuLiao_Colour_ID;

typedef enum{
	X,
	Y
}UART2Data;

void UART2_MaixCam_Init(uint32_t baudRate);
uint8_t UART2_GetRxFlag(void);
uint8_t UART2_GetRxFlag_1(void);
uint8_t UART2_GetRxFlag_2(void);
void UART2_MaixCam_SendByte(uint8_t Byte);
uint16_t UART2_Data_Deal(UART2Data XY,uint8_t *data1);


#endif

