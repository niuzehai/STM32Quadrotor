#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	
extern u8 Usart3_Receive;
void uart2_init(u32 pclk2,u32 bound);
void USART2_IRQHandler(void);
#endif

