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


    /* Variables for debouncer */

	static volatile uint32_t debouncer_count = RELEASE_COUNT;
	static volatile uint32_t btn4_debounced_state = 0U;

	volatile uint32_t btn4_current_state = 0U;

	/* Short single pulses for valid pressed/released states: */
	volatile uint32_t btn4_released_one_shot = 0U;
	volatile uint32_t btn4_pressed_one_shot = 0U;



	/* TEST SIGNAL LOGIC: TASK 1 RUNNING */
	psGpOutSet(PS_GP_OUT3);		/// SET TEST SIGNAL: TASK 1 RUNNING


	/* Read current state of BTN4 */
	btn4_current_state = psGpInRead(BTN4);



	/* TEST SIGNAL LOGIC: BTN4 CURRENT STATE */
	if 	(btn4_current_state == 1U )
	{
		axiGpOutSet(GP_OUT0);		/// SET TEST SIGNAL: BTN4 CURRENT STATE
	}
	else
	{
		axiGpOutClear(GP_OUT0);		/// CLEAR TEST SIGNAL: BTN4 CURRENT STATE
	}


	/* ---------------------------
	 * Main debounce code.
	 * -------------------------- */

	/* Check if BTN4 has changed state by comparing current state with debounced state.
	 * (1) If no change, just set count value for when state does change.
	 * (2) If change detected, decrement counter until state considered stable.
	 * (3) Update de-bounce logic when counter expires.
	 * (4) Reset counter, and set one-shot variables. */
	if (btn4_current_state == btn4_debounced_state)
	/* (1) No change; just set count value. */
	{
		if (btn4_debounced_state == 1U)
		{
			debouncer_count = RELEASE_COUNT;
		}
		else
		{
			debouncer_count = PRESS_COUNT;
		}
	}
	else
	/* (2) Button has changed, so wait for state to become stable.
	 * Do this by decrementing the counter until it is zero. */
	{
		axiGpOutSet(GP_OUT2);		/// SET TEST SIGNAL: COUNTING
		--debouncer_count;

		if(debouncer_count == 0U)
		/* (3) De-bounce time is expired, so update variables indicating new state. */
		{
			btn4_debounced_state = btn4_current_state;
			// (4) Reset the count value, and set one-shot signals
			if (btn4_debounced_state == 1U)
			{
				debouncer_count = RELEASE_COUNT;
				btn4_pressed_one_shot = 1U;
				btn4_released_one_shot = 0U;
			}
			else
			{
				debouncer_count = PRESS_COUNT;
				btn4_pressed_one_shot = 0U;
				btn4_released_one_shot = 1U;
			}
			axiGpOutClear(GP_OUT2);		/// CLEAR TEST SIGNAL: COUNTING
		}
	}


	/* TEST SIGNAL LOGIC: BTN4 DEBOUNCED STATE */
	if 	(btn4_debounced_state == 1U )
	{
		axiGpOutSet(GP_OUT1);		/// SET TEST SIGNAL: BTN4 DEBOUNCED STATE
	}
	else
	{
		axiGpOutClear(GP_OUT1);		/// CLEAR TEST SIGNAL: BTN4 DEBOUNCED STATE
	}



	/* Now, we can safely toggle LED3 without fear of bounce.
	 * We will use the one-shot-released flag, but we could
	 * alternatively use the one-shot-pressed flag. */
	if (btn4_released_one_shot == 1U)
	{
		axiGpOutToggle(LED3);
	}
	/* END OF DE-BOUNCE CODE */


	/* ORIGINAL TASK LOGIC: TOGGLE LED1 */
	led1_count++;

	if (led1_count >= LED1_TOGGLE_COUNT)
	{
		axiGpOutToggle(LED1);
		led1_count = 0;
	}


	/* Dummy delay for test purposes */
	uint32_t idx = 0;
	for (idx = 0; idx <= 50; idx++) {
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
	for (idx = 0; idx <= 80; idx++) {
		psGpOutSet(PS_GP_OUT4);		/// TEST SIGNAL
	}

	psGpOutClear(PS_GP_OUT4);	/// TEST SIGNAL
}




/****** End functions *****/

/****** End of File **********************************************************/
