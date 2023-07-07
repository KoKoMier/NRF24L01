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
	//Ƕ�������жϿ�������ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //�жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //�ж������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�ж���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //�ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Config(int a)
{
	//USART��ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	USART_InitTypeDef USART_InitStructure;
	
	//�򿪴���GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//�򿪴�������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//USART ��TX����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART ��RX����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//���ô��ڹ�������
	USART_InitStructure.USART_BaudRate = a;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	//�����ж����ȼ�����
	NVIC_Configguration();
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	//ʹ�ܴ���
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

	followRxBuffer[followRxCnt]=followData;	//���յ������ݴ��뻺������
	followRxCnt++;

	if (followRxBuffer[0]!=0x01) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
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
