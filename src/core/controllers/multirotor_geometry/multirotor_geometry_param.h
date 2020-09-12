#ifndef __MULTIROTOR_GEOMETRY_PARAM_H__
#define __MULTIROTOR_GEOMETRY_PARAM_H__

#include "common_list.h"

enum {
	//... reserved for common parameters
	/*----------------------------------*/
	MR_GEO_GAIN_ROLL_P = COMMON_PARAM_CNT,
	MR_GEO_GAIN_ROLL_D,
	MR_GEO_GAIN_PITCH_P,
	MR_GEO_GAIN_PITCH_D,
	MR_GEO_GAIN_YAW_P,
	MR_GEO_GAIN_YAW_D,
	MR_GEO_GAIN_RATE_YAW,
	MR_GEO_GAIN_POS_X,
	MR_GEO_GAIN_VEL_X,
	MR_GEO_GAIN_POS_Y,
	MR_GEO_GAIN_VEL_Y,
	MR_GEO_GAIN_POS_Z,
	MR_GEO_GAIN_VEL_Z,
	MR_GEO_GAIN_POS_X_I,
	MR_GEO_GAIN_POS_Y_I,
	MR_GEO_GAIN_POS_Z_I,
	MR_GEO_UAV_MASS,
	MR_GEO_INERTIA_JXX,
	MR_GEO_INERTIA_JYY,
	MR_GEO_INERTIA_JZZ,
	PWM_TO_THRUST_C1,
	PWM_TO_THRUST_C2,
	PWM_TO_THRUST_C3,
	PWM_TO_THRUST_C4,
	PWM_TO_THRUST_C5,
	PWM_TO_THRUST_C6,
	THRUST_TO_PWM_C1,
	THRUST_TO_PWM_C2,
	THRUST_TO_PWM_C3,
	THRUST_TO_PWM_C4,
	THRUST_TO_PWM_C5,
	THRUST_TO_PWM_C6,
	THRUST_MAX,
	/*----------------------------------*/
	MR_GEO_PARAM_LIST_SIZE
} MULTIROTOR_GEOMETRY_PARAM_ID;

void init_multirotor_geometry_param_list(void);

#endif
