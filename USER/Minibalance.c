#include "sys.h"
/**************************************************************************
此代码的控制算法是通过验证的，但是请同学们不要直接使用到四轴上面，因为不同的电调，不同的解法，螺旋桨的安装都可能影响到最终的控制系统
请根据程序的底层和控制代码与您的硬件一个一个测试匹配
**************************************************************************/
u32 Distance;                               //超声波测距
u8 Way_Angle=2;                             //获取角度的算法，1:DMP2：卡尔曼 可以改成3 即互补滤波，
u8 Flag_Show=0,Flag_Stop=1;                 //显示标志位 启停标志位
u32 Moto_test;
u32 Moto_Stop_Flag;
int Temperature;                            //显示温度
int Moto1,Moto2,Moto3,Moto4;               //电机PWM变量 应是Motor的 向Moto致敬	
int k,j;
int Flyoff_Loop_num,Forward_Loop_num,FallAlight_Loop_num,DropOut_Loop_num,Balance_Loop_num,Fly_temp;
int Balance_Pwm_test;
int DropOut_Height;
u32 I;
float Pp,Pr,D;
int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
	LED_Init();                     //初始化与 LED 连接的硬件接口
	KEY_Init();                     //按键初始化
	OLED_Init();                    //OLED初始化
	uart_init(72,115200);           //初始化串口1
  uart2_init(72,9600);            //串口2初始化
	TIM3_Cap_Init(0XFFFF,72-1);	    //=====超声波初始化
	MiniBalance_PWM_Init(9999,35);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 改为MiniBalance_PWM_Init(9999,35) 200HZ
	IIC_Init();                     //模拟IIC初始化
  MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //初始化DMP     
  Timer1_Init(49,7199);           //=====5MS进一次中断服务函数
	delay_ms(15);
	para_init();										//对于特定四旋翼的参数设置
	moto_init();										//使电机初始化转动
// 	moto_single_test(); 						//电机轴判别
	delay_Loop(500);
	Subject1();											//题目1
//	delay_ms(10000);								//题目间延时
//	Subject2();											//题目2
//	delay_ms(10000);								//题目间延时
//	delay_Loop(500);
//	Subject3();
//	while(1){
//			Read_DMP();
//			Moto_Balance();
//	}
	while(1)
		{
     	if(Way_Angle==1)                     //DMP没有涉及到严格的时序问题，在主函数读取
				{
					Read_DMP();                      //===读取角速度和倾角
				}  
				if(Flag_Stop==1)//使用OLED显示屏和PC端串口调试助手
				{
					OLED_Refresh();									//显示屏状态刷新
					//Subject3();											//题目3
				//	Moto_Balance();
					//Moto_Balance();
//					Moto_para_test();
//					para_test(7125);								//悬停值测试

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
对于特定四旋翼的参数设置
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
OLED状态刷新
**************************************************************************/
void OLED_Refresh(void){
					Read_DMP();                      //===读取角速度和倾角
					Read_Distane();
					Way_Angle=1;//使用DMP
					Temperature=Read_Temperature();  //===读取MPU6050内置温度传感器数据，近似表示主板温度。	
					oled_show(); //===显示屏打开
					printf("X轴倾角%f  Y轴倾角%f   \r\n",Pitch,Roll);//向上位机发送数据
}
/**************************************************************************
使电机初始化转动
**************************************************************************/
void moto_init(void){
		for(j=0;j<=100;j++){
		OLED_Refresh();
		init_Set_Pwm(8000,8000,8000,8000);
	}
}
/**************************************************************************
Moto1 2 3 4 的判别
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
悬停值测试
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
起飞循环
输入变量：循环次数（时间）
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
前进循环
输入变量：前进动力值，循环次数（时间）
**************************************************************************/
void Forward_Loop(u32 Front_Weight,u32 loop){
	Forward_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Forward_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_Fly_Front(Front_Weight);
	}
}
/**************************************************************************
降落循环
输入变量：循环次数（时间）
**************************************************************************/
void FallAlight_Loop(u32 loop){
	FallAlight_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=FallAlight_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_FallAlight();
	}
}
/**************************************************************************
悬停循环
输入变量：循环次数（时间）
**************************************************************************/
void Balance_Loop(u32 loop){
	Balance_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Balance_Loop_num;Fly_temp++){
		OLED_Refresh();
		Moto_Balance();
	}
}
/**************************************************************************
掉落循环
输入变量：循环次数（时间）
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
题目1
**************************************************************************/
void Subject1(void){

  Flyoff_Loop(500);

	Forward_Loop(10,250);

	FallAlight_Loop(200);
	
	DropOut_Loop(500);
	
	init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
题目2
**************************************************************************/
void Subject2(void){
	Flyoff_Loop(500);
	Forward_Loop(100,500);
	FallAlight_Loop(500);
	DropOut_Loop(500);
	init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
题目3
**************************************************************************/
void Subject3(void){
	Flyoff_Loop(500);
	Balance_Loop(10000);
  FallAlight_Loop(500);
  DropOut_Loop(500);
  init_Set_Pwm(0,0,0,0);
}
/**************************************************************************
循环延时
输入变量：循环次数（时间）
**************************************************************************/
void delay_Loop(u32 loop){
	Flyoff_Loop_num = loop;
	for(Fly_temp=0;Fly_temp<=Flyoff_Loop_num;Fly_temp++){
		OLED_Refresh();
	}
}