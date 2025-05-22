#ifndef __UART1_H
#define __UART1_H

#include "stm32f10x.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define Scrn_Bound 115200

void UART1_Scrn_Init(int bound);
void HMI_send_string(char* name, char* showdata);
void HMI_send_number(char* name, int num);
void HMI_send_float(char* name, float num);
int fputc(int ch, FILE *f);
void UART1_SendByte(uint8_t Byte);

#endif
