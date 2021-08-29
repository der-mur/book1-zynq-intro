/******************************************************************************
 * @Title		:	SCU Timer Interface (Header File)
 * @Filename	:	xscu_timer_if.h
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

#ifndef SRC_TIMERS_XSCU_TIMER_IF_H_
#define SRC_TIMERS_XSCU_TIMER_IF_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xscutimer.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define SCUTIMER_DEVICE_ID			XPAR_PS7_SCUTIMER_0_DEVICE_ID


/* This effectively controls the main program loop time. */
#define SCU_TIMER_LOAD_VALUE		(0x00001A0A) // 20us @ 667MHz/2


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialisation */
int xScuTimerInit(void);

/* Interface functions */
void waitScuTimerExpired(void);
void startScuTimer(void);



#endif /* SRC_TIMERS_XSCU_TIMER_IF_H_ */
