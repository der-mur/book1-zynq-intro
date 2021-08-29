/******************************************************************************
 * @Title		:	Tasks
 * @Filename	:	tasks.c
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



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "tasks.h"


/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/

/* Use to slow down LED toggle rates, when loop rate is very fast */
static volatile uint32_t led1_count = 0;
static volatile uint32_t led2_count = 0;



/*****************************************************************************
 * Function: task1()
 *//**
 *
 * @brief		Simple task to toggle LED1. The led1_count variable is needed
 * 				to slow down the toggle rate when the code is running at a
 * 				very fast rate, as in that case, the LED might appear to be
 * 				always on.
 *
 * @return		None.
 *
 * @note		None.
 *
******************************************************************************/

void task1(void){

	psGpOutSet(PS_GP_OUT3);		/// TEST SIGNAL

	led1_count++;

	if (led1_count >= LED1_TOGGLE_COUNT)
	{
		axiGpOutToggle(LED1);
		led1_count = 0;
	}


	/* Dummy delay for test purposes */
	uint32_t idx = 0;
	for (idx = 0; idx <= 25; idx++) {
		psGpOutSet(PS_GP_OUT3);		/// TEST SIGNAL
	}


	psGpOutClear(PS_GP_OUT3);	/// TEST SIGNAL
}




/*****************************************************************************
 * Function: task2()
 *//**
 *
 * @brief		Simple task to toggle LED2. The led2_count variable is needed
 * 				to slow down the toggle rate when the code is running at a
 * 				very fast rate, as in that case, the LED might appear to be
 * 				always on.
 *
 * @return		None.
 *
 * @note		None.
 *
******************************************************************************/

void task2(void){

	psGpOutSet(PS_GP_OUT4);		/// TEST SIGNAL

	led2_count++;

	if (led2_count >= LED2_TOGGLE_COUNT)
	{
		axiGpOutToggle(LED2);
		led2_count = 0;
	}

	/* Dummy delay for test purposes */
	uint32_t idx = 0;
	for (idx = 0; idx <= 20; idx++) {
		psGpOutSet(PS_GP_OUT4);		/// TEST SIGNAL
	}

	psGpOutClear(PS_GP_OUT4);	/// TEST SIGNAL
}




/****** End functions *****/

/****** End of File **********************************************************/
