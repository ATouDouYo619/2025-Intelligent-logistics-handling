#include "UART3.h"

uint8_t Code_RxPacket[7]={0};
uint8_t Code_TxPacket[7];

char Code_ChangeData[7];

uint8_t Code_RxFlag;

void UART3_Code_Init(int bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// USART3 TX (PB10)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// USART3 RX (PB11)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE); //USART3	
}

void Code_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
}

void Code_SendArray(uint8_t *Array, s16 Length)
{
	u16 i;
	for (i = 0; i < Length; i++)
	{
		Code_SendByte(Array[i]);
	}
}

void Code_SendPacket(void)
{
	Code_SendArray(Code_RxPacket, 7);
}

void Code_Packet_Init(void)
{
    for (int i = 0; i < 7; i++) 
	{
        Code_RxPacket[i] = 0;
        Code_TxPacket[i] = Code_RxPacket[i];
    }
}

void HexToString(unsigned char *hexData, char *result, int length) 
{
    int j = 0;
    for (int i = 0; i < length; i++) 
    {
//        if (i == 3) 
//        {
//            result[j++] = '+'; 
//        }
        result[j++] = hexData[i];
    }
}


void HexToArray(unsigned char *hexData, char *result, int length) 
{
    for (int i = 0; i < length; i++) 
	{
        result[i] = hexData[i];
    }
}

uint8_t Code_GetRxFlag(void)
{
	if(Code_RxFlag == 1)
	{
		Code_RxFlag = 0;
		return 1;
	}
	return 0;
}

void USART3_IRQHandler(void) 
{
	static uint8_t pRxPacket = 0;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);
		Code_RxPacket[pRxPacket] = RxData;
		pRxPacket ++;
		if(pRxPacket >=7)
		{
			pRxPacket = 0;
			Code_RxFlag = 1;
		}
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}

