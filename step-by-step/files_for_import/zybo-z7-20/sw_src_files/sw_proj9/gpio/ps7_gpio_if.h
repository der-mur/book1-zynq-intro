/******************************************************************************
 * @Title		:	PS7 GPIO Interface (Header File)
 * @Filename	:	ps7_gpio_if.h
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent Zybo-Z7-20
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (C) 2021  Derek Murray
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
******************************************************************************/


#ifndef SRC_GPIO_PS7_GPIO_IF_H_
#define SRC_GPIO_PS7_GPIO_IF_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xgpiops.h"



/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define PS7_GPIO_DEVICE_ID			XPAR_PS7_GPIO_0_DEVICE_ID


/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/

/* ----------------------------------------------------------------------------
 * ----- PS7 GPIO -----
 *//**
 * For this project, LED4 and all PMOD JF pins are outputs.
 * BTN4 and BTN5 are inputs.
 * ---------------------------------------------------------------------------*/

typedef enum
{
	LED4 = 7,			/** MIO 7 */
	PS_GP_OUT0 = 13,	/** MIO13, PMOD JF PIN 1*/
	PS_GP_OUT1 = 10,	/** MIO10, PMOD JF PIN 2*/
	PS_GP_OUT2 = 11,	/** MIO11, PMOD JF PIN 3*/
	PS_GP_OUT3 = 12, 	/** MIO12, PMOD JF PIN 4*/
	PS_GP_OUT4 = 0, 	/** MIO0, PMOD JF PIN 5*/
	PS_GP_OUT5 = 9, 	/** MIO9, PMOD JF PIN 6*/
	PS_GP_OUT6 = 14, 	/** MIO14, PMOD JF PIN 7*/
	PS_GP_OUT7 = 15 	/** MIO15, PMOD JF PIN 8*/
}PsGpio_OutPin_t;

typedef enum
{
	BTN4 = 50,			/** MIO 50 */
	BTN5 = 51			/** MIO 51 */
}PsGpio_InPin_t;



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialisation */
int psGpioInit(void);


/* Interface functions */
void psGpOutSet(PsGpio_OutPin_t pin);
void psGpOutClear(PsGpio_OutPin_t pin);
void psGpOutToggle(PsGpio_OutPin_t pin);
uint32_t psGpInRead(PsGpio_InPin_t pin);


#endif /* SRC_GPIO_PS7_GPIO_IF_H_ */
