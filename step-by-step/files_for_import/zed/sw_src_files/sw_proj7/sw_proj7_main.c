/******************************************************************************
 * @Title		:	Zynq Fundamentals Software Project 7
 * @Filename	:	sw_proj7_main.c
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

#include "sw_proj7_main.h"



/*****************************/

int main(void)
{


	// ********************************************************************************* //
	// *****  INITIALIZATION PHASE *****
	// ********************************************************************************* //

#if MAIN_DEBUG
	printf("\n\r------------------------------------------------------------\n\r");
	printf("----------- Zynq Fundamentals Software Project 7 -----------\n\r");
	printf("------------------------------------------------------------\n\r");
	printf("Title: Button Debouncing\r\n");
	printf("Architecture: FG/BG Polled State Machine.\r\n");
	printf("Timing: Triple Timer Counter 0, Wave 0.\r\n\r\n");
#endif


	/* ----------------------------------------
	 * SYSTEM INITIALISATION (drivers, etc)
	 * ----------------------------------------
	 * If initialisation is successful, LED0 is turned on to indicate that the system
	 * is ready. If initialisation is not successful, then LED0 will flash, and the
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
	// *****   MAIN PROGRAM [TASK STATE MACHINE ARCHITECTURE] *****
	// ********************************************************************************* //

#if MAIN_DEBUG
	printf("\n\rRunning main program; LED9 should be toggling.\n\r");
	printf("\n\rPress BTN8 to toggle LED3.\n\r");	
#endif



	for(;;) // Infinite loop
	{

		/* Use to slow down LED9 toggle rate, when loop rate is very fast */
		static uint32_t led9_count = 0U;

		/* Flags for WDT to know that task completed */
		static uint32_t task1_complete = 0U;
		static uint32_t task2_complete = 0U;

		static enum {
			INIT,
			TASK1,
			TASK2,
			SERVICE_WDT
		} state = INIT;


		/* ----- (1) INITIALIZATION  ----------------------------------- */
		switch (state) {
			case INIT:
				enableInterrupts();
				startTtc0();
				task1_complete = 0U;
				task2_complete = 0U;
				state = TASK1;
				break;


		/* ----- (2) RUN TASKS 1 AND 2 ----------------------------------- */
		/* For each task case:
		 * (a) Wait for task trigger signal.
		 * (b) Call the task.
		 * (c) When task returns, set 'taskX_complete' signal.
		 * (d) Set the next state.*/

		case TASK1:
			if (getTask1TriggerState() == 1U)
			{
				task1();
				task1_complete = 1U;
				state = TASK2;
			}
			break;


		case TASK2:
			if (getTask2TriggerState() == 1U)
			{
				task2();
				task2_complete = 1U;
				state = SERVICE_WDT;
			}
			break;



			/* ----- (3) SERVICE WDT----------------------------------------- */
			/* Main aim is to check that task1 and task2 have completed.
			 * If they have, the watchdog will be serviced, the task_complete
			 * signals will be cleared, and the sequencer will go back to
			 * TASK1 state. If the tasks have not completed,then the sequencer
			 * stays in this state, the watchdog will not be serviced, and the
			 * system eventually resets.
			 *
			 * The LED9 count logic is used to slow the visible LED toggle rate
			 * when the system is running very fast.  */

			case SERVICE_WDT:
				psGpOutSet(PS_GP_OUT5);			/// TEST SIGNAL

				if ( (task1_complete == 1U) && (task2_complete == 1U) )
				{
					led9_count++;
					if (led9_count == LED9_TOGGLE_COUNT)
					{
						psGpOutToggle(LED9);
						led9_count = 0;
					}
					task1_complete = 0U;
					task2_complete = 0U;
					/* --- 'TICKLE' WATCHDOG --- */
					restartScuWdt();
					state = TASK1;
				}
				psGpOutClear(PS_GP_OUT5);	/// TEST SIGNAL
				break;

			} /* End switch */
	}

		return 0;
}


/****** End main()  *****/

/****** End of File **********************************************************/
