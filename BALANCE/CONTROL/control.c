#include "control.h"	
#include "filter.h"	

#define PI 3.14159265
u32 SuspendPower;
u32 TargetHeight;
int Sensitive;
int Up_Out_Sensitive;
float Ip,Ir;
/**************************************************************************
作者：Mini Balance 

**************************************************************************/

/**************************************************************************
函数功能：5MS定时中断函数 5MS控制周期
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
  		Led_Flash(400);                                          //===LED闪烁;	      
			Key();                                                  //===扫描按键状态

//// 			Moto1=3*(2000-5*Pitch+gyro[1]*8/100  -5*Roll+gyro[0]*8/100 -Ip -Ir) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
////			Moto2=3*(2000+5*Pitch-gyro[1]*8/100  +5*Roll-gyro[0]*8/100 +Ip -Ir) ;////////////// 
////			Moto3=3*(2000-5*Pitch+gyro[1]*8/100  +5*Roll-gyro[0]*8/100 -Ip -Ir) ;////////////// 
////			Moto4=3*(2000+5*Pitch-gyro[1]*8/100  -5*Roll+gyro[0]*8/100 +Ip -Ir) ;////////////// 
//		 	Moto1=(SuspendPower-SuspendPower/2000*5*Pitch+gyro[1]*SuspendPower/2000*8/100  -5*Roll+gyro[0]*SuspendPower/2000*8/100 ) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
//			Moto2=(SuspendPower+SuspendPower/2000*5*Pitch-gyro[1]*SuspendPower/2000*8/100  +5*Roll-gyro[0]*SuspendPower/2000*8/100 ) ;////////////// 
//			Moto3=(SuspendPower-SuspendPower/2000*5*Pitch+gyro[1]*SuspendPower/2000*8/100  +5*Roll-gyro[0]*SuspendPower/2000*8/100 ) ;////////////// 
//			Moto4=(SuspendPower+SuspendPower/2000*5*Pitch-gyro[1]*SuspendPower/2000*8/100  -5*Roll+gyro[0]*SuspendPower/2000*8/100 ) ;////////////// 
//		
//			Moto1=(SuspendPower-SuspendPower/Sensitive*5*Pitch+gyro[1]*SuspendPower/Sensitive*8/100  -5*Roll+gyro[0]*SuspendPower/Sensitive*8/100 -Ip -Ir) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
//			Moto2=(SuspendPower+SuspendPower/Sensitive*5*Pitch-gyro[1]*SuspendPower/Sensitive*8/100  +5*Roll-gyro[0]*SuspendPower/Sensitive*8/100 +Ip -Ir) ;////////////// 
//			Moto3=(SuspendPower-SuspendPower/Sensitive*5*Pitch+gyro[1]*SuspendPower/Sensitive*8/100  +5*Roll-gyro[0]*SuspendPower/Sensitive*8/100 -Ip -Ir) ;////////////// 
//			Moto4=(SuspendPower+SuspendPower/Sensitive*5*Pitch-gyro[1]*SuspendPower/Sensitive*8/100  -5*Roll+gyro[0]*SuspendPower/Sensitive*8/100 +Ip -Ir) ;////////////// 
//  		Moto1=(SuspendPower-Pp*Pitch+gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
//			Moto2=(SuspendPower+Pp*Pitch-gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto3=(SuspendPower-Pp*Pitch+gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto4=(SuspendPower+Pp*Pitch-gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//////////////  
// 		//Xianfu_Pwm();                                            //===PWM限幅
//      if(Turn_Off()==0)                   //===如果不存在异常
// 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器    		
	}       
} 


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{			
			
			moto1 = Xianfu_Import_Pwm(moto1);
			moto2 = Xianfu_Import_Pwm(moto2);
			moto3 = Xianfu_Import_Pwm(moto3);
			moto4 = Xianfu_Import_Pwm(moto4);
	
			PWMA=myabs(moto1);
			PWMB=myabs(moto2);	
	   	PWMC=myabs(moto3);
			PWMD=myabs(moto4);	
}
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
作    者：Mini Balance
**************************************************************************/
void init_Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{			
	
			PWMA=myabs(moto1);
			PWMB=myabs(moto2);	
	   	PWMC=myabs(moto3);
			PWMD=myabs(moto4);	
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=4000; 
	
    if(Moto1<0) Moto1=0;	
		if(Moto1<Amplitude)  Moto1=Amplitude;	
	  if(Moto2<0) Moto2=0;	
		if(Moto2<Amplitude)  Moto2=Amplitude;		
	  if(Moto3<0) Moto3=0;	
		if(Moto3<Amplitude)  Moto3=Amplitude;	
	  if(Moto4<0) Moto4=0;	
		if(Moto4<Amplitude)  Moto4=Amplitude;		

}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
u32 Xianfu_Import_Pwm(u32 moto)
{	
	  int Amplitude=6000; 
		int Max_Amplitude=7600;
	
    if(moto<=0) moto=0;	
		if(moto<Amplitude)  moto=Amplitude;	
		if(moto>Max_Amplitude)  moto=Max_Amplitude;	
		return moto;
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
作    者：Mini Balance
**************************************************************************/
void Xianfu1_Pwm(void)
{	
	  int Amplitude=1000; 
    if(Moto1<0) Moto1=0;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	  if(Moto2<0) Moto2=0;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	  if(Moto3<0) Moto3=0;	
		if(Moto3>Amplitude)  Moto3=Amplitude;	
	  if(Moto4<0) Moto4=0;	
		if(Moto4>Amplitude)  Moto4=Amplitude;		
	
}
/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
作    者：Mini Balance
**************************************************************************/
u8 Turn_Off(void)
{
	    u8 temp;
			if(Pitch<-40||Pitch>40||1==Flag_Stop||Roll<-40||Roll>40)
			{	                                                 //===倾角大于40度关闭电机
	//		Set_All_Pwm(0);
      temp=1;                                            //===Flag_Stop置1关闭电机
      }
			else
      temp=0;
      return temp;			
}
	

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1){
		Moto_para_test();								//悬停值测试
	}
//		Flag_Stop=!Flag_Stop;
	if(tmp==2){
		Moto_Balance();
	}
}
void Moto_init_6000(void)  
{                                                 
 		Set_Pwm(6000,6000,6000,6000);                        //===赋值给PWM寄存器    		
} 
void Moto_Z_Balance(void){
		
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
  		Led_Flash(400);                                          //===LED闪烁;	      
			Key();                                                  //===扫描按键状态
 			Moto1=2000-5*Pitch+gyro[1]*8/100  -5*Roll+gyro[0]*8/100;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
			Moto2=2000+5*Pitch-gyro[1]*8/100  +5*Roll-gyro[0]*8/100;////////////// 
			Moto3=2000-5*Pitch+gyro[1]*8/100  +5*Roll-gyro[0]*8/100  ;////////////// 
			Moto4=2000+5*Pitch-gyro[1]*8/100  -5*Roll+gyro[0]*8/100  ;////////////// 
   		Xianfu_Pwm();                                            //===PWM限幅
      if(Turn_Off()==0)                   //===如果不存在异常
 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器    		
	}       
}

//////wangzhouyang
//void Moto_Balance_BK(void)
//{
//	PIDx_Update(1e-5);
//	PIDx_Update(1e-5);
//	PIDx_Update(1e-5);
//	PIDzp_Update(1e-5);
//	MORTOR_Output();
//}
//////wangzhouyang
void Moto_Balance(void)
{
//	if(TIM1->SR&0X0001)//5ms定时中断
//	{   
//		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
//  		Led_Flash(400);                                          //===LED闪烁;	      
//			Key();                                                  //===扫描按键状态
			Ip += 0.1*Pitch;
			Ir += 0.1*Roll;
			Moto1=(SuspendPower+Pp*Pitch+gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
			Moto2=(SuspendPower-Pp*Pitch-gyro[1]*D  -Pr*Roll-gyro[0]*D) ;////////////// 
			Moto3=(SuspendPower+Pp*Pitch+gyro[1]*D  -Pr*Roll-gyro[0]*D) ;////////////// 
			Moto4=(SuspendPower-Pp*Pitch-gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//////////////  
	//  		Xianfu_Pwm();                                            //===PWM限幅
//      if(Turn_Off()==0)                   //===如果不存在异常
 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器   
	
//	}       
}

void Moto_Balance_Up(u32 Power)
{
//	if(TIM1->SR&0X0001)//5ms定时中断
//	{   
//		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
//  		Led_Flash(400);                                          //===LED闪烁;	      
//			Key();                                                  //===扫描按键状态
//			Moto1=(Power+Pp*Pitch+gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
//			Moto2=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto3=(Power+Pp*Pitch+gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto4=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//////////////  
			Moto1=(Power-Pp*Pitch+gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
			Moto2=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
			Moto3=(Power-Pp*Pitch+gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
			Moto4=(Power+Pp*Pitch-gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//////////////  
 //  		Xianfu_Pwm();                                            //===PWM限幅
//      if(Turn_Off()==0)                   //===如果不存在异常
 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器    		
//	}       
}
void Moto_Balance_Down(u32 Power)
{		
//	if(TIM1->SR&0X0001)//5ms定时中断
//	{   
//		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
//  		Led_Flash(400);                                          //===LED闪烁;	      
//			Key();                                                  //===扫描按键状态
//			Moto1=(Power+Pp*Pitch+gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
//			Moto2=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto3=(Power+Pp*Pitch+gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
//			Moto4=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll+gyro[0]*D) ;//////////////  
			Moto1=(Power-Pp*Pitch+gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//平衡的PD控制 可自行加入Z轴P控制抑制自旋 
			Moto2=(Power+Pp*Pitch-gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
			Moto3=(Power-Pp*Pitch+gyro[1]*D  +Pr*Roll-gyro[0]*D) ;////////////// 
			Moto4=(Power+Pp*Pitch-gyro[1]*D  -Pr*Roll+gyro[0]*D) ;//////////////  
 //  		Xianfu_Pwm();                                            //===PWM限幅
//      if(Turn_Off()==0)                   //===如果不存在异常
 			Set_Pwm(Moto1,Moto2,Moto3,Moto4);                        //===赋值给PWM寄存器    		
//	}       
}
void Moto_Single_Output(int Select)
{
//	if(TIM1->SR&0X0001)//5ms定时中断
//	{   
//		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位		 
//  		Led_Flash(400);                                          //===LED闪烁;	      
//			Key();                                                  //===扫描按键状态
//			
//      if(Turn_Off()==0)                   //===如果不存在异常  		
			if(Select == 1){
					init_Set_Pwm(7000,0,0,0);
			}else if(Select == 2){
					init_Set_Pwm(0,7000,0,0);
			}else if(Select == 3){
					init_Set_Pwm(0,0,7000,0);
			}else if(Select == 4){
					init_Set_Pwm(0,0,0,7000);
			}
//	}       
}
void Moto_Flyoff(void){
	if(Distance>TargetHeight){
		Moto_Balance_Up(SuspendPower + (myabs((TargetHeight - Distance))/Up_Out_Sensitive));
	}else{
		Moto_Balance_Up(SuspendPower + (-myabs((TargetHeight - Distance))/Up_Out_Sensitive));
	}
}

void Moto_FallAlight(void){
//	if(Distance>TargetHeight){
//		Set_All_Accel((TargetHeight - Distance)/Up_Out_Sensitive);
//	}else{
//		Set_All_Accel(-40);
			Moto_Balance_Down(SuspendPower+100);
//	}
}
void Moto_DropOut(void){
	if(Distance>DropOut_Height){
		Moto_FallAlight();
	}else{
		Set_All_Pwm(0);
		Moto_Stop_Flag = 1;
	}
}
void Moto_Fly_Control(u32 Front_Weight,u32 Right_Weight,u32 Rotation_Weight){
	Moto1 = 0 + Right_Weight - Front_Weight - Rotation_Weight;
	Moto2 = 0 - Right_Weight + Front_Weight - Rotation_Weight; 
	Moto3 = 0 - Right_Weight - Front_Weight + Rotation_Weight; 
	Moto4 = 0 + Right_Weight + Front_Weight + Rotation_Weight; 
	Set_Accel(Moto1,Moto2,Moto3,Moto4);
}
void Set_Accel(u32 a1,u32 a2,u32 a3,u32 a4){
	Set_Pwm(SuspendPower-a1,SuspendPower-a2,SuspendPower-a3,SuspendPower-a4);
}
void Set_All_Pwm(u32 a){
	Set_Pwm(a,a,a,a);
}
void Set_All_Accel(u32 a){
	Set_Accel(a,a,a,a);
}
void Moto_Fly_Front(u32 Front_Weight){
	Moto_Fly_Control(Front_Weight,0,0);
}
void Moto_Fly_Rotation(u32 Rotation_Weight){
	Moto_Fly_Control(0,0,Rotation_Weight);
}


