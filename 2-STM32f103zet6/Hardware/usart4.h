#ifndef __USART4_H
#define __USART4_H

#include "stm32f10x.h"   
#include "stdint.h"

#define JY61_Bound	9600

extern uint8_t INS_1[5];
extern uint8_t INS_2[5];
extern uint8_t INS_3[5];

void usart4_Init(uint32_t bound);
void UART4_SendByte(uint8_t Byte);
void UART4_SendArray(uint8_t *Array, uint16_t Length);

void Jy61P_Init_Zero(void);

#endif



