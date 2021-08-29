/******************************************************************************
 * @Title		:	Tasks
 * @Filename	:	tasks.c
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

#include "tasks.h"


/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/

/* Use to slow down LED toggle rates, when loop rate is very fast */
// static volatile uint32_t led1_count = 0;
// static volatile uint32_t led2_count = 0;



/* Added for sw_proj10 for shared variable test. */
/* Global variables for Task 1 and Task 2. */
static uint32_t volatile g_task1_shared_var;
static uint32_t volatile g_task2_shared_var;



/*****************************************************************************
 * Function: task1()
 *//**
 *
 * @brief		Task has been modified for a shared variable test. Normally,
 * 				interrupts should be disabled at the start of the task so that
 * 				the shared variable cannot be 'trampled on' by other code that
 * 				accesses the shared variable. If running a test, though, then
 * 				interrupts remain enabled, and external code (the UART handler)
 * 				might also write to the shared variable. If this happens, then
 * 				LED1 will be turned on to indicated the undesired access.
 *
 * 				This task sets the shared variable to 0xA5A5_A5A5 soon after
 * 				the task starts. A small dummy delay is then run.
 * 				While the task is running, the UART ISR may have changed the
 * 				variable. If so, LED1 turns on
 *
 * @return		None.
 *
 * @note		Set TASK1_SHARED_VAR_TEST to '1' in header file to enable test.
 *
 *
******************************************************************************/

void task1(void)
{

	/* 1. Disable interrupts to protect shared variable. */
	/* If running the shared variable test, this line will not be executed,
	 * meaning that external code might overwrite the variable. */
#if !TASK1_SHARED_VAR_TEST
	disableInterrupts();
#endif

	psGpOutSet(PS_GP_OUT3);		/// TEST SIGNAL: ENTERING TASK 1



	/* 2. Set the shared variable. */
	setTask1SharedVariable(0xA5A5A5A5);


	/* 3. Dummy delay for test purposes */
	uint32_t idx;
	for (idx = 0; idx <= 50; idx++)
	{
	  psGpOutSet(PS_GP_OUT3);  /// TEST SIGNAL
	}


	/* 4. Check if the shared variable has been altered. */
	if (g_task1_shared_var != 0xA5A5A5A5)
	{
		axiGpOutSet(LED1);
	}
	else {}

	psGpOutClear(PS_GP_OUT3);	/// TEST SIGNAL: LEAVING TASK 1


	/* 5. Re-enable interrupts when task is finished. */
	/* If running the shared variable test, this line will not be executed. */
#if !TASK1_SHARED_VAR_TEST
	enableInterrupts();
#endif


}





/*****************************************************************************
 * Function: task2()
 *//**
 *
 * @brief		Task has been modified for a shared variable test. Normally,
 * 				interrupts should be disabled at the start of the task so that
 * 				the shared variable cannot be 'trampled on' by other code that
 * 				accesses the shared variable. If running a test, though, then
 * 				interrupts remain enabled, and external code (the UART handler)
 * 				might also write to the shared variable. If this happens, then
 * 				LED2 will be turned on to indicated the undesired access.
 *
 * 				This task sets the shared variable to 0xA5A5_A5A5 soon after
 * 				the task starts. A small dummy delay is then run.
 * 				While the task is running, the UART ISR may have changed the
 * 				variable. If so, LED1 turns on
 *
 * @return		None.
 *
 * @note		Set TASK2_SHARED_VAR_TEST to '1' in header file to enable test.
 *
 *
******************************************************************************/

void task2(void)
{

	/* 1. Disable interrupts to protect shared variable. */
	/* If running the shared variable test, this line will not be executed,
	 * meaning that external code might overwrite the variable. */
#if !TASK2_SHARED_VAR_TEST
	disableInterrupts();
#endif

	psGpOutSet(PS_GP_OUT4);		/// TEST SIGNAL: ENTERING TASK 2



	/* 2. set the shared variable. */
	setTask2SharedVariable(0xA5A5A5A5);


	/* 3. Dummy delay for test purposes */
	uint32_t idx;
	for (idx = 0; idx <= 50; idx++)
	{
	  psGpOutSet(PS_GP_OUT4);  /// TEST SIGNAL
	}


	/* 4. Check if the shared variable has been altered. */
	if (g_task2_shared_var != 0xA5A5A5A5)
	{
		axiGpOutSet(LED2);
	}
	else {}

	psGpOutClear(PS_GP_OUT4);	/// TEST SIGNAL: LEAVING TASK 2


	/* 5. Re-enable interrupts when task is finished. */
	/* If running the shared variable test, this line will not be executed. */
#if !TASK2_SHARED_VAR_TEST
	enableInterrupts();
#endif


}


/* === ADDED SW_PROJ10 === */

/*****************************************************************************
 * Function: setTask1SharedVariable()
 *//**
 *
 * @brief		Function to allow Task 1 global variable to be set.
 * 				Can be used in external code to modify the variable.
 *
 * @param[in]	uint32_t value:	Value to set the global variable to.
 *
 * @return		None.
 *
 * @note		None.
 *
 *
******************************************************************************/


void setTask1SharedVariable(uint32_t value)
{
  g_task1_shared_var = value;
}


/*****************************************************************************
 * Function: getTask1SharedVariable()
 *//**
 *
 * @brief		Function to allow Task 1 global variable to be read back.
 * 				Can be used in external code if desired.
 *
 *
 * @return		uint32_t: Current value of global variable.
 *
 * @note		None.
 *
 *
******************************************************************************/

uint32_t getTask1SharedVariable(void)
{
  return g_task1_shared_var;
}




/*****************************************************************************
 * Function: setTask2SharedVariable()
 *//**
 *
 * @brief		Function to allow Task 2 global variable to be set.
 * 				Can be used in external code to modify the variable.
 *
 * @param[in]	uint32_t value:	Value to set the global variable to.
 *
 * @return		None.
 *
 * @note		None.
 *
 *
******************************************************************************/


void setTask2SharedVariable(uint32_t value)
{
  g_task2_shared_var = value;
}


/*****************************************************************************
 * Function: getTask2SharedVariable()
 *//**
 *
 * @brief		Function to allow Task 2 global variable to be read back.
 * 				Can be used in external code if desired.
 *
 *
 * @return		uint32_t: Current value of global variable.
 *
 * @note		None.
 *
 *
******************************************************************************/

uint32_t getTask2SharedVariable(void)
{
  return g_task2_shared_var;
}




/****** End functions *****/

/****** End of File **********************************************************/
