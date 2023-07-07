#include "led.h"
#include "delay.h"
#include "usart.h"	 
#include "nrf24l01.h" 	 
#include "TI_usart2.h"


int main(void)
 {	 
	u16 t=0;
	u16 tt=0;	 
	u8 tmp_buf[4];	    
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	USART2_Config(115200);
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
 	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error");
		delay_ms(200);
	}
	printf("NRF24L01 OK"); 	 			
	printf("NRF24L01 TX_Mode");	
	NRF24L01_TX_Mode();
	while(1)
	{	  	
			tmp_buf[0]=MV_FLAG;
			tmp_buf[1]=GOGO;
			tmp_buf[2]=MV_ERROR;
			tmp_buf[3]=DATA;

			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
//				printf("Sended DATA:");	
				for(t = 0; t < 4; t++){
					
					printf("%d", MV_FLAG);
				}   
			}else
			{					
				printf("Send Failed "); 
			};
			//printf("%d %d %d\r]n",MV_COLOR,MV_ERROR,MV_FLAG);
			delay_ms(10);				    
		}; 

}
