#ifndef MCAL_GPIO_H
#define MCAL_GPIO_H

#include "stm32f407_base.h"

// set-reset
//typedef enum{
//	GPIO_PIN_SET = 1U,
//	GPIO_PIN_RESET = 0U
//}GPIO_PinState_t;
#define GPIO_PIN_SET						1U
#define GPIO_PIN_RESET					0U

// GPIO_MODER
//typedef enum{
//	GPIO_MODE_INPUT = 				0U,
//	GPIO_MODE_OUTPUT = 				1U,
//	GPIO_MODE_ALTFN = 				2U,
//	GPIO_MODE_ANALOG = 				3U,
//}GPIO_PinMode_t;

#define GPIO_MODE_INPUT 				0U
#define GPIO_MODE_OUTPUT 				1U
#define GPIO_MODE_ALTFN					2U
#define GPIO_MODE_ANALOG				3U

//GPIO_SPEED
//typedef enum{
//	GPIO_SPEED_LOW = 					0U,
//	GPIO_SPEED_MEDIUM = 			1U,
//	GPIO_SPEED_HIGH = 				2U,
//	GPIO_SPEED_VERY_HIGH = 		3U,
//}GPIO_PinSpeed;
#define GPIO_SPEED_LOW					0U
#define GPIO_SPEED_MEDIUM				1U
#define GPIO_SPEED_HIGH					2U
#define GPIO_SPEED_VERY_HIGH		3U

//GPIO_OUTPUT_TYPE
//typedef enum{
//	GPIO_OUTPUT_TYPE_PP = 		0U,
//	GPIO_OUTPUT_TYPE_OD = 		1U,
//}GPIO_OutputType;
#define GPIO_OUTPUT_TYPE_PP			0U
#define GPIO_OUTPUT_TYPE_OD			1U

//GPIO_PULL-UP_PULL-DOWN
//typedef enum{
//	GPIO_NO_PP = 							0U,
//	GPIO_PULL_UP = 						1U,
//	GPIO_PULL_DOWN = 					2U,
//}GPIO_PUPD;
#define GPIO_NO_PP							0U
#define GPIO_PULL_UP						1U
#define GPIO_PULL_DOWN					2U

typedef struct{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PUPD;
	uint8_t GPIO_OutputType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_Type_Def *Port;
	GPIO_PinConfig_t Pinconfig;
}GPIO_Handle_t;


void MCAL_GPIO_Init(GPIO_Handle_t *pHandle);
void MCAL_GPIO_WritePin(GPIO_Type_Def *Port, uint8_t PinNumber, uint8_t status);
void MCAL_GPIO_TogglePin(GPIO_Type_Def *Port, uint8_t PinNumber);
uint8_t MCAL_GPIO_ReadPin(GPIO_Type_Def *Port, uint8_t PinNumber);


#endif
