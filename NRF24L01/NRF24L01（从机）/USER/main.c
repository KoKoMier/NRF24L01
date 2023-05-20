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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
 	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error");
		delay_ms(200);
	}
	printf("NRF24L01 OK"); 	 

#ifdef NRF24L01_RX   //RX模式

		printf("NRF24L01 RX_Mode");	
		printf("Received DATA:");	
		NRF24L01_RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
			{
				tmp_buf[32]=0;//加入字符串结束符
				for(t = 0; t < 32; t++){
				
					printf("%c", tmp_buf[t]);
				}
				printf("\r");
			}else delay_us(100);	   		    
		}
#endif
		
#ifdef NRF24L01_TX	//TX模式
						    
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
