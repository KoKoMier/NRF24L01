#ifndef __TI_USART2_H
#define __TI_USART2_H


extern short GOGO;
extern short MV_ERROR;
extern short MV_FLAG;
extern short DATA;

void USART2_Config(int a);
void usartGetData(unsigned char followData);

#endif
