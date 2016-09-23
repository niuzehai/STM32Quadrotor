#ifndef __FILTER_H
#define __FILTER_H
  /**************************************************************************


**************************************************************************/
/*һάkalman�˲��ṹ�嶨��,A=1,B=0,H=1*/
typedef struct Kalman_filter
{
	float C_last;				    /*�ϴ�Ԥ�����Э������� C(k|k-1)*/
	float X_last;				    /*ϵͳ״̬Ԥ������о���*/
	
	float Q;						/*��������Э����*/
	float R;						/*��������Э����*/
	
	float K;						/*���������棬�о���*/
	float X;						/*���Ź�����������о���*/
	float C;						/*���Ź���Э�������C(k|k)*/
                            
	float input;				    /*����ֵ����Z(k)*/
}
kal_filter;
extern float angle, angle_dot; 	
void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);
float kalman_filter(kal_filter* k_flt, float input);
#endif
