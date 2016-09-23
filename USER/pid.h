#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"
//-----constants -----------------------------------
#define IG_MAX 400.0
#define I_MAX 200.0
#define I_P_MAX 200.0
#define I_H_MAX 200.0
#define PG_MAX 800.0
//#define DG_MAX 400.0
#define PID_X_MAX	500
#define PID_Y_MAX	500
#define PID_OUT_MAX 1200.0
#define PID_H_OUT_MAX 1200.0
#define PID_Z_MAX	500
#define PID_Z_MIN 180
#define MAX_GYRO_ERROR 350.0
#define MAX_TARGET_ANGLE_M2 60.0//20.0*2
#define MAX_TARGET_ANGLE		30.0//20.0*2
#define MAX_TARGET_RATE_M2 200.0//100.0*2
#define MAX_TARGET_RATE 	 100.0
#define MAX_DELTA_X     50.0
#define MAX_DELTA_Y     50.0
#define MAX_DELTA_Z     50.0
#define MAX_Z_RATE     500.0
#define TARGET_Z_RATE 150
//------- Quad mode --------
#define _QUAD_X_
//#define _QUAD_PLUS_
//--------------------------
#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
#define MINMAX(x, min, max)	(MIN(MAX((x), (min)), (max)))
#define CONSTRAIN(x, a)		(MINMAX(x, -(a), (a)))
//-------------------------------------------------

//------- variables -------------------------------
extern float PIDx,PIDy,PIDz;
extern float rx_value[6];
extern float throttle;
extern float height;
//-------------------------------------------------


//------------- functions -------------------------
void PIDx_Update(float dt);
void PIDy_Update(float dt);
void PIDz_Update(float dt);
void PIDzp_Update(float dt);
void PIDxp_Update(float dt);
void PIDyp_Update(float dt);
void PID_GetGainValue(void);
void MORTOR_Output(void);
void keepHeight(float height);
//-------------------------------------------------
#endif