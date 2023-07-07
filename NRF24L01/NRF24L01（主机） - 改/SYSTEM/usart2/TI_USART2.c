#include "stm32f10x.h"
#include <stdio.h>
#include "Delay.h"
#include "TI_usart2.h"

unsigned char usart2_data;
short GOGO;
short MV_ERROR;
short MV_FLAG;
short DATA;

static void NVIC_Configguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//嵌套向量中断控制器组选择
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //中断分组
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //中断主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //中断抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //中断使能
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Config(int a)
{
	//USART初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_InitTypeDef USART_InitStructure;
	
	//打开串口GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//打开串口外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//USART 的TX设置为推挽复用模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART 的RX设置为浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置串口工作参数
	USART_InitStructure.USART_BaudRate = a;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	//串口中断优先级配置
	NVIC_Configguration();
	
	//使能串口接收中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	//使能串口
	USART_Cmd(USART2,ENABLE);

}


void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		usart2_data=USART_ReceiveData(USART2);
		usartGetData(usart2_data);
	}
	USART_ClearFlag(USART2,USART_IT_RXNE);
}

void usartGetData(unsigned char followData)
{
	static unsigned char followRxBuffer[10];
	static unsigned char followRxCnt = 0;	

	followRxBuffer[followRxCnt]=followData;	//将收到的数据存入缓冲区中
	followRxCnt++;

	if (followRxBuffer[0]!=0x01) //数据头不对，则重新开始寻找0x55数据头
	{ 
		followRxCnt=0;
		return;
	}
	if (followRxCnt<10) 
	{
		return;
	}
	else
	{		
		if(followRxBuffer[1] == 0x01)
		{
			MV_FLAG = (short)((followRxBuffer[3]<<8)|followRxBuffer[2]);   
			GOGO = (short)((followRxBuffer[5]<<8)|followRxBuffer[4]);   
			MV_ERROR = (short)((followRxBuffer[7]<<8)|followRxBuffer[6]);    
			DATA = (short)((followRxBuffer[9]<<10)|followRxBuffer[8]);    
		}
	}
	followRxCnt=0;
}
