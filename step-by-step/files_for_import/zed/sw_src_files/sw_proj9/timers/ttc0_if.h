/******************************************************************************
 * @Title		:	Triple Timer Counter 0 Interface (Header File)
 * @Filename	:	ttc0_if.h
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

#ifndef SRC_TIMERS_TTC0_IF_H_
#define SRC_TIMERS_TTC0_IF_H_



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xttcps.h"

#include "../gpio/ps7_gpio_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define TTC0_DEBUG						0

/* PS7 TTC_0 (3 timers, 0,1,2) */
#define PS7_TTC0_DEVICE_ID			XPAR_PS7_TTC_0_DEVICE_ID


/* Counter match values */
/* TTC Clock = 111MHz => Period = 9ns */
/* Prescaler = 0 => Prescaler enabled; resolution = 18ns */
#define PRESCALER_VALUE			0U
#define TASK1_MATCH				1000U	// ~18us (18ns x 1000)
#define TASK2_MATCH				55555U	// ~1ms (18ns x 55555)

/*
 *      _TASK1               _TASK1
 * ____| |__________________| |______________
 * 0   18us                 1.018ms etc
 *                  _TASK2               _TASK2
 * ________________| |__________________| |__
 * 0               1ms                  2ms etc
 */



/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/


/****************************************************************************/
/***************** Macros (Inline Functions) Definitions ********************/
/****************************************************************************/


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialisation */
/* NOTE: *p_inst is being returned, not passed to the function! */
int xTtc0Init(uint32_t *p_inst);


/* Interrupt handler */
void xTtc0IntrHandler(void *CallBackRef);


/* Interface functions */
void startTtc0(void);
void resetTtc0(void);

/* Task functions */
uint32_t getTask1TriggerState(void);
uint32_t getTask2TriggerState(void);



#endif /* SRC_TIMERS_TTC0_IF_H_ */
