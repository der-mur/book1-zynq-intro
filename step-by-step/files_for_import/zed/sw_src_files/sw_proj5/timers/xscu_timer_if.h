/******************************************************************************
 * @Title		:	SCU Timer Interface (Header File)
 * @Filename	:	xscu_timer_if.h
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
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

#ifndef SRC_TIMERS_XSCU_TIMER_IF_H_
#define SRC_TIMERS_XSCU_TIMER_IF_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xscutimer.h"
#include "../gpio/ps7_gpio_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define SCUTIMER_DEBUG						0

#define SCUTIMER_DEVICE_ID			XPAR_PS7_SCUTIMER_0_DEVICE_ID


 /* This effectively controls the main program loop time. */
// #define SCU_TIMER_LOAD_VALUE		(0x01000000) // ??? @ 667MHz/2
// #define SCU_TIMER_LOAD_VALUE		(0x00001A0A) // 20us @ 667MHz/2
#define SCU_TIMER_LOAD_VALUE		(0x00000D05) // 10us @ 667MHz/2


#define TASK1_INTR_COUNT				2U	// 2 interrupt count units
#define TASK2_INTR_COUNT				5U	// 5 interrupt count units
/* === IMPORTANT NOTES ===
 * 1. Count values must not be set to 0.
 * 2. The TASK2 count must have a higher value than the TASK1 count for
 * 		reliable operation.
 * 3. The TASK count values should be chosen such that tasks can run to completion
 * 		before the next task starts. */


/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/




/*****************************************************************************/
/************************ Macros (Inline Functions) **************************/
/*****************************************************************************/

#define printf 		xil_printf 	/* Small foot-print printf function */



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialization */
/* NOTE: *p_inst is being returned, not passed to the function! */
int xScuTimerInit(uint32_t *p_inst);


/* Interrupt handler */
// void xScuTimerIntrHandler(void *CallBackRef);
void xScuTimerIntrHandler(void);

/* Interface functions */
void waitScuTimerExpired(void);
void startScuTimer(void);

/* Task functions */
uint32_t getTask1TriggerState(void);
uint32_t getTask2TriggerState(void);

#endif /* SRC_TIMERS_XSCU_TIMER_IF_H_ */
