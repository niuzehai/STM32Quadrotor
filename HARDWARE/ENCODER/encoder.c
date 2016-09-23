#include "encoder.h"
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	RCC->APB1ENR|=1<<0;     //TIM2时钟使能
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	GPIOA->CRL&=0XFFFFFF00;//PA0 PA1
	GPIOA->CRL|=0X00000044;//浮空输入
	/* 把定时器初始化为编码器模式 */ 
	TIM2->PSC = 0x0;//预分频器
	TIM2->ARR = ENCODER_TIM_PERIOD-1;//设定计数器自动重装值 
  TIM2->CCMR1 |= 1<<0;          //输入模式，IC1FP1映射到TI1上
  TIM2->CCMR1 |= 1<<8;          //输入模式，IC2FP2映射到TI2上
  TIM2->CCER |= 0<<1;           //IC1不反向
  TIM2->CCER |= 0<<5;           //IC2不反向
	TIM2->SMCR |= 3<<0;	          //SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM2->CR1 |= 0x01;    //CEN=1，使能定时器
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR|=1<<1;     //TIM3时钟使能
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	GPIOA->CRL&=0X00FFFFFF;//PA6 PBA
	GPIOA->CRL|=0X44000000;//浮空输入
	/* 把定时器初始化为编码器模式 */ 
	TIM3->PSC = 0x0;//预分频器
	TIM3->ARR = ENCODER_TIM_PERIOD-1;//设定计数器自动重装值 
  TIM3->CCMR1 |= 1<<0;          //输入模式，IC1FP1映射到TI1上
  TIM3->CCMR1 |= 1<<8;          //输入模式，IC2FP2映射到TI2上
  TIM3->CCER |= 0<<1;           //IC1不反向
  TIM3->CCER |= 0<<5;           //IC2不反向
	TIM3->SMCR |= 3<<0;	          //SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM3->CR1 |= 0x01;    //CEN=1，使能定时器
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}


