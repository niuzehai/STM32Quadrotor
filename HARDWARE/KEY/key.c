#include "key.h"
/**************************************************************************
�������ܣ�������ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	GPIOA->CRH&=0X0FFFFFFF; 
	GPIOA->CRH|=0X80000000;//PA15 ��������
  GPIOA->ODR|=1<<15; //PA15 ����	
} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫�� 
**************************************************************************/
u8 click_N_Double (u8 time)
{
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single;
		if(0==KEY&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
			if(flag_key==1) 
			{
				double_key++;
				count_key=1;	
			}
			if(double_key==2) 
			{
		  	double_key=0;
			  count_single=0;
				return 2;//˫��ִ�е�ָ��
			}
		}
		if(1==KEY)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			if(++count_single>time)
			{
			double_key=0;
			count_single=0;	
			return 1;//����ִ�е�ָ��
			}
		}	
		return 0;
}
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
u8 click(void)
{
			static u8 flag_key=1;//�������ɿ���־
			if(flag_key&&KEY==0)
			{
			flag_key=0;
			return 1;	// ��������
			}
			else if(1==KEY)			flag_key=1;
			return 0;//�ް�������
}
