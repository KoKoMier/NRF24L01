#include "led.h"
#include "delay.h"
#include "usart.h"	 
#include "nrf24l01.h" 	 
#include "TI_usart2.h"


int main(void)
 {	 
	u8 tt = 0;
	u8 tmp_buf[4];	    
	u8 tmp_buf2[4];	    
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	USART2_Config(115200);
 	LED_Init();		  			//初始化与LED连接的硬件接口
 	NRF24L01_Init();    		//初始化NRF24L01 
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
			tmp_buf2[0]=tmp_buf[0];
			tmp_buf2[1]=tmp_buf[1];
			tmp_buf2[2]=tmp_buf[2];
			tmp_buf2[3]=tmp_buf[3];
			
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				printf("Sended DATA:%d \r\n",MV_FLAG);	
			}else
			{					
			if(tt > 20)
				{
					NRF24L01_Init();
					NRF24L01_Check();
					NRF24L01_TX_Mode();
					tt = 0;
				}
			tt++;
			printf("Send Failed \r\n"); 
			tmp_buf[0]=tmp_buf2[0];
			tmp_buf[1]=tmp_buf2[1];
			tmp_buf[2]=tmp_buf2[2];
			tmp_buf[3]=tmp_buf2[3];
			
			};
			//printf("%d %d %d\r]n",MV_COLOR,MV_ERROR,MV_FLAG);
			delay_ms(10);				    
		}; 

}
