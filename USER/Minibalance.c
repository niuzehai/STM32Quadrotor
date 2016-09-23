#include "sys.h"
/**************************************************************************
�˴���Ŀ����㷨��ͨ����֤�ģ�������ͬѧ�ǲ�Ҫֱ��ʹ�õ��������棬��Ϊ��ͬ�ĵ������ͬ�Ľⷨ���������İ�װ������Ӱ�쵽���յĿ���ϵͳ
����ݳ���ĵײ�Ϳ��ƴ���������Ӳ��һ��һ������ƥ��
**************************************************************************/
u32 Distance;                               //���������
u8 Way_Angle=2;                             //��ȡ�Ƕȵ��㷨��1:DMP2�������� ���Ըĳ�3 �������˲���
u8 Flag_Show=0,Flag_Stop=1;                 //��ʾ��־λ ��ͣ��־λ
u32 Moto_test;
u32 Moto_Stop_Flag;
int Temperature;                            //��ʾ�¶�
int Moto1,Moto2,Moto3,Moto4;               //���PWM���� Ӧ��Motor�� ��Moto�¾�	
int k,j;
int Flyoff_Loop_num,Forward_Loop_num,FallAlight_Loop_num,DropOut_Loop_num,Balance_Loop_num,Fly_temp;
int Balance_Pwm_test;
int DropOut_Height;
u32 I;
float Pp,Pr,D;
int main(void)
{ 
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init(72);                 //��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	LED_Init();                     //��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();                     //������ʼ��
	OLED_Init();                    //OLED��ʼ��
	uart_init(72,115200);           //��ʼ������1
  uart2_init(72,9600);            //����2��ʼ��
	TIM3_Cap_Init(0XFFFF,72-1);	    //=====��������ʼ��
	MiniBalance_PWM_Init(9999,35);   //=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� ��ΪMiniBalance_PWM_Init(9999,35) 200HZ
	IIC_Init();                     //ģ��IIC��ʼ��
  MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //��ʼ��DMP     
  Timer1_Init(49,7199);           //=====5MS��һ���жϷ�����
	delay_ms(15);
	para_init();										//�����ض�������Ĳ�������
	moto_init();										//ʹ�����ʼ��ת��
// 	moto_single_test(); 						//������б�
	delay_Loop(500);
	Subject1();											//��Ŀ1
//	delay_ms(10000);								//��Ŀ����ʱ
//	Subject2();											//��Ŀ2
//	delay_ms(10000);								//��Ŀ����ʱ
//	delay_Loop(500);
//	Subject3();
//	while(1){
//			Read_DMP();
//			Moto_Balance();
//	}
	while(1)
		{
     	if(Way_Angle==1)                     //DMPû���漰���ϸ��ʱ�����⣬����������ȡ
				{
					Read_DMP();                      //===��ȡ���ٶȺ����
				}  
				if(Flag_Stop==1)//ʹ��OLED��ʾ����PC�˴��ڵ�������
				{
					OLED_Refresh();									//��ʾ��״̬ˢ��
					//Subject3();											//��Ŀ3
				//	Moto_Balance();
					//Moto_Balance();
//					Moto_para_test();
//					para_test(7125);								//��ֵͣ����

//					Moto_Balance();
//					if(Distance < 500){
//						Moto_test = 8000;
//						Set_Pwm(Moto_test,Moto_test,Moto_test,Moto_test);
//					}else{
//						for(Moto_test = 6000;Moto_test>1000;Moto_test--){
//							OLED_Refresh();
//							Set_Pwm(Moto_test,Moto_test,Moto_test,Moto_test);
//						}
//					}
				}	
		} 
}
/**************************************************************************
�����ض�������Ĳ�������
**************************************************************************/
void para_init(void){
		D = 0; 
		Pp = -4.3;
		Pr = 4.3;
		SuspendPower = 7100;
		Sensitive = 4000;
		Up_Out_Sensitive = 1;
		DropOut_Height = 50;
		TargetHeight = 500;
		Flyoff_Loop_num = 500;
		Forward_Loop_num = 500;
		FallAlight_Loop_num = 500;
}
/**************************************************************************
OLED״̬ˢ��
**************************************************************************/
void OLED_Refresh(void){
					Read_DMP();                      //===��ȡ���ٶȺ����
					Read_Distane();
					Way_Angle=1;//ʹ��DMP
					Temperature=Read_Temperature();  //===��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ�	
					oled_show(); //===��ʾ����
					printf("X�����%f  Y�����%f   \r\n",Pitch,Roll);//����λ����������
}
/**************************************************************************
ʹ�����ʼ��ת��
**************************************************************************/
void moto_init(void){
		for(j=0;j<=100;j++){
		OLED_Refresh();
		init_Set_Pwm(8000,8000,8000,8000);
	}
}
/**************************************************************************
Moto1 2 3 4 ���б�
**************************************************************************/
void moto_single_test(void){
	for(k=1;k<=4;k++){
		for(j=0;j<300;j++){
			OLED_Refresh();
			Moto_Single_Output(k);
		}
	}
}

/**************************************************************************
��ֵͣ����
**************************************************************************/
void para_test(u32 pwm){
		while(1){
		Set_Pwm(pwm,pwm,pwm,pwm);
		}
}
void Moto_para_test(void){

		if(Distance>TargetHeight){
			Set_All_Pwm(Balance_Pwm_test = 6000+5*(Distance-TargetHeight));
		}else{
			Set_All_Pwm(Balance_Pwm_test = 6000+5*(Distance-TargetHeight));
		}
	
}

/**************************************************************************
���ѭ��
���������ѭ��������ʱ�䣩
**************************************************************************/
void Flyoff_Loop(u32 loop){
	Flyoff_Loop_num = loop;
	Fly_temp = 8000;
	while(1){
		if(Fly_temp>7080){
			Moto_Balance_Up(Fly_temp = Fly_temp-6);
			OLED_Refresh();
		}else{
			break;
		}
	}
	for(Fly_temp=0;Fly_temp<=Flyoff_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_Balance_Up(7080);
	}
}
/**************************************************************************
ǰ��ѭ��
���������ǰ������ֵ��ѭ��������ʱ�䣩
**************************************************************************/
void Forward_Loop(u32 Front_Weight,u32 loop){
	Forward_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Forward_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_Fly_Front(Front_Weight);
	}
}
/**************************************************************************
����ѭ��
���������ѭ��������ʱ�䣩
**************************************************************************/
void FallAlight_Loop(u32 loop){
	FallAlight_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=FallAlight_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_FallAlight();
	}
}
/**************************************************************************
��ͣѭ��
���������ѭ��������ʱ�䣩
**************************************************************************/
void Balance_Loop(u32 loop){
	Balance_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Balance_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_Balance();
	}
}
/**************************************************************************
����ѭ��
���������ѭ��������ʱ�䣩
**************************************************************************/
void DropOut_Loop(u32 loop){
//	DropOut_Loop_num = loop;
//	for(Fly_temp=0;Fly_temp<=DropOut_Loop_num;Fly_temp++){
	while(1){
//		if(Moto_Stop_Flag != 1){
//			OLED_Refresh();
//			Moto_DropOut();
////	}
//		}else{
			init_Set_Pwm(0,0,0,0);
			break;
//		}
	}
}
/**************************************************************************
��Ŀ1
**************************************************************************/
void Subject1(void){

  Flyoff_Loop(500);

	Forward_Loop(10,250);

	FallAlight_Loop(200);
	
	DropOut_Loop(500);
	
	init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
��Ŀ2
**************************************************************************/
void Subject2(void){
	Flyoff_Loop(500);
	Forward_Loop(100,500);
	FallAlight_Loop(500);
	DropOut_Loop(500);
	init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
��Ŀ3
**************************************************************************/
void Subject3(void){
	Flyoff_Loop(500);
	Balance_Loop(10000);
  FallAlight_Loop(500);
  DropOut_Loop(500);
  init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
ѭ����ʱ
���������ѭ��������ʱ�䣩
**************************************************************************/
void delay_Loop(u32 loop){
	Flyoff_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Flyoff_Loop_num;Fly_temp++){
		OLED_Refresh();
	}
}