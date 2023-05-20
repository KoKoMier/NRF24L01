#include "led.h"
#include "delay.h"
#include "usart.h"	 
#include "nrf24l01.h" 	 

#define NRF24L01_RX
//#define NRF24L01_TX

 int main(void)
 {	 
	u16 t=0;			 
	u8 tmp_buf[33];		    
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
 	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error");
		delay_ms(200);
	}
	printf("NRF24L01 OK"); 	 

#ifdef NRF24L01_RX   //RXģʽ

		printf("NRF24L01 RX_Mode");	
		printf("Received DATA:");	
		NRF24L01_RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
			{
				tmp_buf[32]=0;//�����ַ���������
				for(t = 0; t < 32; t++){
				
					printf("%c", tmp_buf[t]);
				}
				printf("\r");
			}else delay_us(100);	   		    
		}
#endif
		
#ifdef NRF24L01_TX	//TXģʽ
						    
		printf("NRF24L01 TX_Mode");	
		NRF24L01_TX_Mode();
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				printf("Sended DATA:");	
				for(t = 0; t < 32; t++){
				
					printf("%c", tmp_buf[t]);
				}   
			}else
			{										   	
				printf("Send Failed "); 
			};
			delay_ms(1500);				    
		}; 
#endif
}
