#include "timer.h"
#include "led.h"
/**************************************************************************
�������ܣ���ʱ�жϳ�ʼ��
��ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void Timer1_Init(u16 arr,u16 psc)  
{  
	RCC->APB2ENR|=1<<11;//TIM1ʱ��ʹ��    
 	TIM1->ARR=arr;      //�趨�������Զ���װֵ   
	TIM1->PSC=psc;      //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM1->DIER|=1<<0;   //��������ж�				
	TIM1->DIER|=1<<6;   //�������ж�	   
	TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��
	MY_NVIC_Init(1,3,TIM1_UP_IRQn,2);
}  
/**************************************************************************
�������ܣ���ʱ��2ͨ��4���벶��
��ڲ�������ڲ�����arr���Զ���װֵ  psc��ʱ��Ԥ��Ƶ�� 
����  ֵ����
**************************************************************************/
void TIM2_Cap_Init(u16 arr,u16 psc)	
{	 
	RCC->APB1ENR|=1<<0;   	//TIM2 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��   	 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008200;//Pa.2 �������   	Pa.3 ���� 
	
  TIM2->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM2->PSC=psc;  		//Ԥ��Ƶ�� 
	TIM2->CCMR2|=1<<8;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM2->CCMR2|=0<<12; 		//IC1F=0000 ���������˲��� ���˲�
 	TIM2->CCMR2|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM2->CCER|=0<<13; 		//CC1P=0	�����ز���
	TIM2->CCER|=1<<12; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM2->DIER|=1<<4;   	//�������ж�				
	TIM2->DIER|=1<<0;   	//��������ж�	
	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
	MY_NVIC_Init(1,3,TIM2_IRQn,1);
}
void TIM3_Cap_Init(u16 arr,u16 psc)	
{	 
	RCC->APB1ENR|=1<<1;     //TIM3ʱ��ʹ��     
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��   	
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ�� 	
	GPIOA->CRL&=0XFFFFFF00; 
	GPIOA->CRL|=0X00000028;//  	PB.0 ���� PB.1���
	GPIOB->CRL&=0XFFFFFF00; 
	GPIOB->CRL|=0X00000028;//  	PB.0 ���� PB.1���
  TIM3->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM3->PSC=psc;  		//Ԥ��Ƶ�� 
	TIM3->CCMR2|=1<<0;	//ѡ������� 
 	TIM3->CCMR2|=0<<4; 	// ���������˲��� ���˲�
 	TIM3->CCMR2|=0<<2; 	//���������Ƶ,����Ƶ 

	TIM3->CCER|=0<<9; 	//�����ز���
	TIM3->CCER|=1<<8; 	//�������������ֵ������Ĵ�����

	TIM3->DIER|=1<<3;   //�������ж�				
	TIM3->DIER|=1<<0;   //��������ж�	
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	MY_NVIC_Init(1,3,TIM3_IRQn,1);
}
/**************************************************************************
�������ܣ����������ջز�����
��ڲ�������
����  ֵ����
**************************************************************************/
u16 TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;
void Read_Distane(void)
{   
	 PAout(1)=1;
	 PBout(1)=1;
	 delay_us(15);  
	 PAout(1)=0;	 
	 PBout(1)=0;	
			if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance=TIM3CH3_CAPTURE_STA&0X3F;
			Distance*=65536;					        //���ʱ���ܺ�
			Distance+=TIM3CH3_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance=Distance*170/1000;
			printf("%d \r\n",Distance);
			TIM3CH3_CAPTURE_STA=0;			//������һ�β���
		}				
}
/**************************************************************************
�������ܣ��������ز������ȡ�ж�
��ڲ�������
����  ֵ����

**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	u16 tsr;
	tsr=TIM3->SR;
	if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
				{
								if(tsr&0X01)//���
								{	    
										if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
										{
											if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
											{
												TIM3CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
												TIM3CH3_CAPTURE_VAL=0XFFFF;
											}else TIM3CH3_CAPTURE_STA++;
										}	 
								}
						   	if(tsr&0x08)//����3���������¼�
				    	{	
											if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
											{	  			
											TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
											TIM3CH3_CAPTURE_VAL=TIM3->CCR3;	//��ȡ��ǰ�Ĳ���ֵ.
											TIM3->CCER&=~(1<<9);			//CC1P=0 ����Ϊ�����ز���
									  	}else  								//��δ��ʼ,��һ�β���������
				   	{
											TIM3CH3_CAPTURE_STA=0;			//���
											TIM3CH3_CAPTURE_VAL=0;
											TIM3CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
											TIM3->CNT=0;					//���������
											TIM3->CCER|=1<<9; 				//CC1P=1 ����Ϊ�½��ز���
							}		    
					    	}			     	    					   
		   }
			 TIM3->SR=0;//����жϱ�־λ 	     
}

