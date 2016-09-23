#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************


**************************************************************************/
#define PI 3.14159265
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
void TIM1_UP_IRQHandler(void);  
void Set_Pwm(int moto1,int moto2,int moto3,int moto4);
void Moto_Balance(void);
void Xianfu_Pwm(void);
u8 Turn_Off(void);
int myabs(int a);
void Key(void);
void Moto_init_6000(void);
void Moto_Single_Output(int Select);
void Moto_Z_Balance(void);
void Set_Accel(u32 a1,u32 a2,u32 a3,u32 a4);
void Set_All_Accel(u32 a);
void Moto_Fly_Control(u32 Front_Weight,u32 Right_Weight,u32 Rotation_Weight);
void Moto_Flyoff(void);
void Moto_FallAlight(void);
void Moto_DropOut(void);
void Moto_init_Flyoff(u32 a);
void Moto_Fly_Front(u32 Front_Weight);
void Moto_Fly_Rotation(u32 Rotation_Weight);
void Set_All_Pwm(u32 a);
u32 Xianfu_Import_Pwm(u32 moto);
void init_Set_Pwm(int moto1,int moto2,int moto3,int moto4);
void Moto_Balance_Up(u32 Power);
void Moto_Balance_Down(u32 Power);

#endif
