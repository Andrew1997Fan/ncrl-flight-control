#include "stm32f4xx.h"
#include "proj_config.h"

/*
 * << avilon >>
 * m1: pd12 (timer4 channel1)
 * m2: pd13 (timer4 channel2)
 * m3: pe14 (timer1 channel4)
 * m4: pe13 (timer1 channel3)
 * m5: pd14 (timer4 channel3)
 * m6: pd15 (timer4 channel4)
 * m7: pe11 (timer1 channel2)
 * m8: pe9  (timer1 channel1), currently assigned to send camera trigger pulse
 */
/* << pixhawk 2.4.6 >>
 * m1: pe14 (timer1 channel4)
 * m2: pe13 (timer1 channel3)
 * m3: pe11 (timer1 channel2)
 * m4: pe9  (timer1 channel1)
 * m5: pd13 (timer4 channel2)
 * m6: pd14 (timer4 channel3), currently assigned to send camera trigger pulse
 */

void pwm_timer1_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
#endif
	GPIO_InitTypeDef GPIO_InitStruct = {
#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
		.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14,
#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
		.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14,
#endif
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_Speed = GPIO_Speed_100MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	GPIO_Init(GPIOE, &GPIO_InitStruct);

#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
	/* motor8: vins mono camera triggering (gpio mode) */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
#endif

	/* 180MHz / (25000 * 18) = 400Hz = 0.0025s */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
		.TIM_Period = 25000 - 1,
		.TIM_Prescaler = 18 - 1,
		.TIM_ClockDivision = TIM_CKD_DIV1,
		.TIM_CounterMode = TIM_CounterMode_Up
	};

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct = {
		.TIM_OCMode = TIM_OCMode_PWM1,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = 0,
	};

#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
	//TIM_OC1Init(TIM1, &TIM_OCInitStruct); //motor8: vins mono camera triggering
	//TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
#endif
	TIM_Cmd(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


void pwm_timer4_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
#endif

	GPIO_InitTypeDef GPIO_InitStruct = {
#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
		.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
		.GPIO_Pin =  GPIO_Pin_13,
#endif
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_Speed = GPIO_Speed_100MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	GPIO_Init(GPIOD, &GPIO_InitStruct);

#if (UAV_HARDWARE ==  UAV_HARDWARE_PIXHAWK2_4_6)
	/* motor6: vins mono camera triggering (gpio mode) */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
#endif

	/* 90MHz / (25000 * 9) = 400Hz = 0.0025s */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
		.TIM_Period = 25000 - 1,
		.TIM_Prescaler = 9 - 1,
		.TIM_ClockDivision = TIM_CKD_DIV1,
		.TIM_CounterMode = TIM_CounterMode_Up
	};

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct = {
		.TIM_OCMode = TIM_OCMode_PWM1,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = 0,
	};

#if (UAV_HARDWARE == UAV_HARDWARE_AVILON)
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);

#elif (UAV_HARDWARE == UAV_HARDWARE_PIXHAWK2_4_6)
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	//TIM_OC3Init(TIM4, &TIM_OCInitStruct);
#endif

	TIM_Cmd(TIM4, ENABLE);
}

/*
 * imu heater: pb15 (timer8 channel3)
 */
void pwm_timer8_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM8);

	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* 180MHz / (50000 * 9) = 400Hz = 0.0025s */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
		.TIM_Period = 50000 - 1,
		.TIM_Prescaler = 9 - 1,
		.TIM_ClockDivision = TIM_CKD_DIV1,
		.TIM_CounterMode = TIM_CounterMode_Up
	};
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);

	TIM_OCInitTypeDef TIM_OCInitStruct = {
		.TIM_OCMode = TIM_OCMode_PWM1,
		.TIM_OutputState = TIM_OutputState_Enable,
		.TIM_Pulse = 0,
	};
	TIM_OC3Init(TIM8, &TIM_OCInitStruct);

	TIM_Cmd(TIM8, ENABLE);
}
