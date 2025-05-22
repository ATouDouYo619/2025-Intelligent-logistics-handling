#include "usart4.h"                // 头文件改名
#include "jy61p.h"
#include "Delay.h"

uint8_t INS_1[5] = {0xFF,0xAA,0x69,0x88,0xB5};
uint8_t INS_2[5] = {0xFF,0xAA,0x01,0x08,0x00};
uint8_t INS_3[5] = {0xFF,0xAA,0x00,0x00,0x00};

void usart4_Init(uint32_t bound)   // 函数名修改
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  // 改为GPIOC
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);  // 改为UART4
    
    // PC10作为TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
    
    // PC11作为RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
     
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);  // 改为UART4
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  // 改为UART4
    
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;  // 中断通道修改
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(UART4, ENABLE);  // 改为UART4
}

void UART4_SendByte(uint8_t Byte)  // 函数名修改
{
    USART_SendData(UART4, Byte);    // 改为UART4
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void UART4_SendArray(uint8_t *Array, uint16_t Length)  // 函数名修改
{
    uint16_t i;
    for (i = 0; i < Length; i ++)
    {
        UART4_SendByte(Array[i]);
    }
}

void Jy61P_Init_Zero(void)
{
    UART4_SendArray(INS_1,5);  // 改用UART4发送
    Delay_ms(200);
    UART4_SendArray(INS_2,5);
    Delay_ms(3000);
    UART4_SendArray(INS_3,5);
    Delay_ms(200);
}

// 串口4中断服务函数
void UART4_IRQHandler(void)  // 中断函数名修改
{
    uint8_t RxData;
    if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)  // 改为UART4
    {
        RxData = USART_ReceiveData(UART4);  // 改为UART4
        jy61p_ReceiveData(RxData);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);  // 改为UART4
    }
}

