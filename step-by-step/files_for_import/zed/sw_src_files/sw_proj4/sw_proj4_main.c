/******************************************************************************
 * @Title		:	Zynq Fundamentals Software Project 4
 * @Filename	:	sw_proj4_main.c
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



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "sw_proj4_main.h"



/*****************************/

int main(void)
{


	// ********************************************************************************* //
	// *****  INITIALIZATION PHASE *****
	// ********************************************************************************* //

#if MAIN_DEBUG
	printf("\n\r-----------------------------------------------------------\n\r");
	printf("----------- Zynq Fundamentals Software Project 4 ----------\n\r");
	printf("-----------------------------------------------------------\n\r");
	printf("Title: Zynq-7000 Timers.\r\n");
	printf("Architecture: Polling with SCU Timer task timing.\r\n\r\n");
#endif


	/*----------------------------------------
	 * SYSTEM INITIALIZATION (drivers, etc)
	 *----------------------------------------
	 * If initialization is successful, LED0 is turned on to indicate that the system
	 * is ready. If initialization is not successful, then LED0 will flash, and the
	 * program will not continue to the main phase.
	 */

	/* Variable for initialisation status */
	int init_status;

	/* Run initialisation */
	init_status = sys_init();

	if (init_status == XST_SUCCESS){
		axiGpOutSet(LED0);
		#if MAIN_DEBUG
			printf("\n\rSystem ready: LED0 should be on.\n\r");
		#endif
	}
	else {
		#if MAIN_DEBUG
			printf("\n\r!!! INITIALIZATION FAILED !!!\n\r");
		#endif
		while(1) { // Stay in this loop

		/* Toggle LED to show init failed, and 'kick' WDT
		 * so that debug can be carried out e.g. via XSCT.
		 * (Assumes that WDT was not the failing component.) */
		axiGpOutToggle(LED0);
		restartScuWdt();

		uint32_t delay = 0U;
		for (delay = 0; delay < INIT_FAIL_LOOP_DELAY; delay++)
			{}
		}
	}


	// ********************************************************************************* //
	// *****   MAIN PROGRAM [SIMPLE POLLING 'ARCHITECTURE'] *****
	// ********************************************************************************* //

#if MAIN_DEBUG
	printf("\n\rRunning main program; LED4 should be toggling.\n\r");
#endif

	/* Variables to store switch read values */
	volatile uint32_t sw1_state;
	volatile uint32_t sw2_state;
	volatile uint32_t sw3_state;

	/* Use to slow down LED toggle rates, when loop rate is very fast */
	uint32_t led1_count = 0U;
	uint32_t led2_count = 0U;
	uint32_t led4_count = 0U;

	/* Start the SCU timer; this controls the loop delay below. */
	startScuTimer();

	for(;;) // Infinite loop
	{

		/* ----- (1) RUN TASKS 1 AND 2 ----------------------------------- */


		/* ----- TASK 1 -----
		 *//**
		 * TASK: If SW1 is OFF, then toggle LED1
		 * TEST SIGNAL: Assert test signal at start of task;
		 * De-assert at end of task.
		 */
		psGpOutSet(PS_GP_OUT3);		/// SET TEST SIGNAL

		sw1_state = axiGpInRead(SW1);
		if (sw1_state == 0U)
		{
			led1_count++;
			if (led1_count == LED1_TOGGLE_COUNT)
			{
				axiGpOutToggle(LED1);
				led1_count = 0U;
			}
		}
		else
		{
			axiGpOutSet(LED1);
		}

		psGpOutClear(PS_GP_OUT3); 	/// TEST SIGNAL



		/* ----- TASK 2 -----
		 *//**
		 * TASK: If SW2 is OFF, then toggle LED2
		 * TEST SIGNAL: Assert test signal at start of task;
		 * De-assert at end of task.
		 */
		psGpOutSet(PS_GP_OUT4); 	/// TEST SIGNAL

		sw2_state = axiGpInRead(SW2);
		if (sw2_state == 0U)
		{
			led2_count++;
			if (led2_count == LED2_TOGGLE_COUNT)
			{
				axiGpOutToggle(LED2);
				led2_count = 0U;
			}
		}
		else
		{
			axiGpOutSet(LED2);
		}

		psGpOutClear(PS_GP_OUT4);	/// TEST SIGNAL




		/* ----- (2) Delay before restart polling ----------------------------------- */

		/*
		*//**
		* Assert test signal before entering waitScuTimerExpired() function;
		* De-assert when function returns.
		*/
		psGpOutSet(PS_GP_OUT0);		/// TEST SIGNAL

		waitScuTimerExpired();

		psGpOutClear(PS_GP_OUT0);	/// TEST SIGNAL




		/* ----- (3) Service the SCU WDT  ------------------------------------------- */

		/* SW3 = OFF: Normal operation. LED4 toggles to indicate program is running.
		 * The SCU watchdog will be 'tickled' with the restartScuWdt() call.
		 * SW3 = ON: Simple test for the watchdog. The processor will eventually reset
		 * because the watchdog will not be serviced.
		 */

		psGpOutSet(PS_GP_OUT5);			/// TEST SIGNAL
		sw3_state = axiGpInRead(SW3);

		if (sw3_state == 0U)
		{
			led4_count++;
			if (led4_count == LED4_TOGGLE_COUNT)
			{
				psGpOutToggle(LED4);
				led4_count = 0U;
			}
			restartScuWdt();
		}

		psGpOutClear(PS_GP_OUT5);		/// TEST SIGNAL

	}

	// Should never get here
	return 0;

}

/****** End main()  *****/

/****** End of File **********************************************************/



