#include "show.h"
  /**************************************************************************


**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
u32 mult;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
		OLED_Display_On();  //显示屏打开
//		//=============显示滤波器=======================//	
//		                      OLED_ShowString(00,0,"WAY-");
//		                      OLED_ShowNumber(30,0, Way_Angle,1,12);
//	       if(Way_Angle==1)	OLED_ShowString(45,0,"DMP");
//		else if(Way_Angle==2)	OLED_ShowString(45,0,"Kalman");
//		else if(Way_Angle==3)	OLED_ShowString(45,0,"Hubu");
//		//=============显示温度=======================//	
//		                      OLED_ShowString(00,10,"Wendu");
//		                      OLED_ShowNumber(45,10,Temperature/10,2,12);
//		                      OLED_ShowNumber(68,10,Temperature%10,1,12);
//		                      OLED_ShowString(58,10,".");
//		                      OLED_ShowString(80,10,"`C");
//		                      OLED_ShowString(00,20,"Juli");
//		                      OLED_ShowNumber(45,20,Distance,5,12);
//			                    OLED_ShowString(00,30,"Yaw");
//		                      OLED_ShowNumber(45,30,Yaw,5,12);
//	

		                      OLED_ShowString(00,0,"Moto1");
		                      OLED_ShowNumber(45,0, TIM4->CCR1,4,12);
		                      OLED_ShowString(00,10,"Moto2");
		                      OLED_ShowNumber(45,10, TIM4->CCR2,4,12);
		                      OLED_ShowString(00,20,"Moto3");
		                      OLED_ShowNumber(45,20, TIM4->CCR3,4,12);
			                    OLED_ShowString(00,30,"Moto4");
		                      OLED_ShowNumber(45,30, TIM4->CCR4,4,12);

		                      OLED_ShowString(0,40,"Pitch");
		
		if(Pitch<0)		OLED_ShowNumber(75,40,Pitch+360,3,12);
					        OLED_ShowNumber(75,40,Pitch,3,12);
			//=============显示角度=======================//
			                      OLED_ShowString(0,50,"Roll");
		if(Roll<0)		OLED_ShowNumber(75,50,Roll+360,3,12);
					        OLED_ShowNumber(75,50,Roll,3,12);


//													mult = 1000;
//		                      OLED_ShowString(00,0,"acc0");
//		                      OLED_ShowNumber(45,0, accel[0]/16384,3,12);
//		                      OLED_ShowString(00,10,"acc1");
//		                      OLED_ShowNumber(45,10,accel[1]/16384,3,12);
//		                      OLED_ShowString(00,20,"acc2");
//		                      OLED_ShowNumber(45,20,accel[2]/16384,3,12);
//			                    OLED_ShowString(00,30,"gyr0");
//		                      OLED_ShowNumber(45,30,gyro[0]/131,3,12);
//			                    OLED_ShowString(00,40,"gyr1");
//		                      OLED_ShowNumber(45,40,gyro[1]/131,3,12);	
//			                    OLED_ShowString(00,50,"gyr2");
//		                      OLED_ShowNumber(45,50,gyro[2]/131,3,12);

		//=============刷新=======================//
		OLED_Refresh_Gram();	
	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无

**************************************************************************/
void APP_Show(void)
{    
	  int app_2,app_3,app_4;
	
		printf("Z%d:%d:%d:%dL$",(u8)app_2,(u8)app_3,(u8)app_4,(int)Pitch);
}

