#include "UART2.h"

uint8_t UART2_RxPacket[4];
uint8_t UART2_RxPacket_2[4];
uint8_t UART2_Color_ID[1];

uint8_t UART2_RxFlag;
uint8_t UART2_Color_Flag;
uint8_t UART2_Color_XY_Flag;

uint16_t X_Coord,Y_Coord;

uint8_t Color_ID;

uint16_t X_Colour,Y_Colour;
uint8_t WuLiao_Colour_ID;

void UART2_MaixCam_Init(uint32_t baudRate)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // USART2 TX (PA2)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2 RX (PA3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);    
}

uint8_t UART2_GetRxFlag(void)
{
	if (UART2_RxFlag == 1)
	{
		UART2_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t UART2_GetRxFlag_1(void)
{
	if (UART2_Color_Flag == 1)
	{
		UART2_Color_Flag = 0;
		return 1;
	}
	return 0;
}

uint8_t UART2_GetRxFlag_2(void)
{
	if (UART2_Color_XY_Flag == 1)
	{
		UART2_Color_XY_Flag = 0;
		return 1;
	}
	return 0;
}

void UART2_MaixCam_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

uint16_t UART2_Data_Deal(UART2Data XY, uint8_t *data1) 
{
    uint16_t DEAL_DATA = 0;

    if (XY == X) 
	{
        DEAL_DATA = (data1[0] << 8) | data1[1];  // 组合为 X 数据
    } else if (XY == Y) 
	{
        DEAL_DATA = (data1[2] << 8) | data1[3];  // 组合为 Y 数据
    }
    return DEAL_DATA;
}

void USART2_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART2);

        if (RxState == 0)  				// 初始状态
        {
            if (RxData == 0xAA)  		// 检测到圆环中心坐标开始
            {
                RxState = 1;
                pRxPacket = 0;
            }
            else if (RxData == 0xCC)  	// 检测到颜色ID开始
            {
                RxState = 3;
                pRxPacket = 0;
            }
            else if (RxData == 0xDD)  	// 检测到颜色中心坐标开始
            {
                RxState = 5;
                pRxPacket = 0;
            }
        }
        else if (RxState == 1)  		// 状态1，接收圆环数据包
        {    
            UART2_RxPacket[pRxPacket] = RxData;
            pRxPacket++;
            if (pRxPacket >= 4)
            {
                RxState = 2;
				X_Coord = UART2_Data_Deal(X, UART2_RxPacket);
				Y_Coord = UART2_Data_Deal(Y, UART2_RxPacket);
            }
        }
        else if (RxState == 3)  		// 状态3，接收颜色ID
        {    
            UART2_Color_ID[pRxPacket] = RxData;
            pRxPacket++;
            if (pRxPacket >= 1)
            {
                RxState = 4;
				WuLiao_Colour_ID = UART2_Color_ID[0];
            }
        }
        else if (RxState == 5)  		// 状态5，接收颜色中心坐标数据包
        {    
            UART2_RxPacket_2[pRxPacket] = RxData;
            pRxPacket++;
            if (pRxPacket >= 4)
            {
                RxState = 6;
				X_Colour = UART2_Data_Deal(X, UART2_RxPacket_2);
				Y_Colour = UART2_Data_Deal(Y, UART2_RxPacket_2);
            }
        }

        else if (RxState == 2)
        {
            if (RxData == 0xBB)
            {
                RxState = 0;
                UART2_RxFlag = 1;
            }
        }
        else if (RxState == 4)  // 状态4，检查结束标志
        {
            if (RxData == 0xBB)
            {
                RxState = 0;
                UART2_Color_Flag = 1;
            }
        }
        else if (RxState == 6)  // 状态4，检查结束标志
        {
            if (RxData == 0xBB)
            {
                RxState = 0;
                UART2_Color_XY_Flag = 1;
            }
        }
		
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  // 清除接收中断标志
    }
}

