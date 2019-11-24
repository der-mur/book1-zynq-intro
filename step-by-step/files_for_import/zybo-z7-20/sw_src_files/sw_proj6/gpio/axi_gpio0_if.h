/******************************************************************************
 * @Title		:	AXI GPIO Interface (Header File)
 * @Filename	:	axi_gpio0_if.h
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent Zybo-Z7-20
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (C) 2020  Derek Murray
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

#ifndef SRC_GPIO_AXI_GPIO0_IF_H_
#define SRC_GPIO_AXI_GPIO0_IF_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xgpio.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define AXI_GPIO0_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID


/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/

/* ----- AXI GPIO 0 -----
 *//**
 * AXI GPIO Channel 1 = Output
 * AXI GPIO Channel 2 = Input channel.
 * MASK = 0x0FFF means lower 12 bits of channel 2 are set to input. */
#define AXI_GPIO0_OP_CHANNEL 		1U
#define AXI_GPIO0_IP_CHANNEL 		2U
#define AXI_GPIO0_IP_MASK			(0x00000FFF)


/* ----------------------------------------------------------------------------
 * ----- AXI GPIO 0 -----
 *//**
 * For this project, channel 1 is the output channel.
 * 8 outputs are used: 4 x Board LEDS 0-3; 4 x GP_OUT 0-3 (PMOD JE Pins 1-4)
 * Channel 2 is the input channel. 12 inputs are used:
 * 4 x Board BTNS 0-3; 4 x Board SW 0-3; 4 x GP_IN 0-3 (PMOD JE pins 7-10)
 * --------------------------------------------------------------------------*/

typedef enum
{
	LED0, LED1, LED2, LED3,
	GP_OUT0, GP_OUT1, GP_OUT2,
	GP_OUT3
}AxiGpio0_OutPin_t;


typedef enum
{
	BTN0, BTN1, BTN2, BTN3,
	SW0, SW1, SW2, SW3,
	GP_IN0, GP_IN1, GP_IN2, GP_IN3
}AxiGpio0_InPin_t;




/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialisation */
int axiGpio0Init(void);
int axiGpio0Init_orig(void);


/* Interface functions */
void axiGpOutSet(AxiGpio0_OutPin_t pin);
void axiGpOutClear(AxiGpio0_OutPin_t pin);
void axiGpOutToggle(AxiGpio0_OutPin_t pin);
uint32_t axiGpInRead(AxiGpio0_InPin_t pin);


#endif /* SRC_GPIO_AXI_GPIO0_IF_H_ */
