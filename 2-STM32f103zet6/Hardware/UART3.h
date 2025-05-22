#ifndef __UART3_H
#define __UART3_H

#include "stm32f10x.h"
#include "stdint.h"

#define Code_Bound 		115200

extern uint8_t Code_RxPacket[];
extern uint8_t Code_TxPacket[];
extern char Code_ChangeData[];
extern uint8_t Code_RxFlag;

void UART3_Code_Init(int bound);
void Code_SendByte(uint8_t Byte);
void Code_SendArray(uint8_t *Array, s16 Length);

void Code_SendPacket(void);
void Code_Packet_Init(void);

void HexToString(unsigned char *hexData, char *result, int length);
void HexToArray(unsigned char *hexData, char *result, int length);

uint8_t Code_GetRxFlag(void);

#endif
