/******************************************************************************
 * @Title		:	SCU Timer Interface
 * @Filename	:	xscu_timer_if.c
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


/***************************** Include Files ********************************/

#include "xscu_timer_if.h"




/************************** Variable Definitions ****************************/

/* Declare instance and associated pointer for XScuTimer */
static XScuTimer		XScuTimerInst;
static XScuTimer 		*p_XScuTimerInst = &XScuTimerInst;


/* Task trigger signals; Scope is local to this file.
 * Accessor functions are used to give access to other files. */
static uint32_t volatile trigger_task1;
static uint32_t volatile trigger_task2;



/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/



/*****************************************************************************
 * Function: xScuTimerInit()
 *//**
 *
 * @brief		Configures the SCU Timer for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	SCU Timer.
 *
 * 				The initialisation steps are:
 * 				(1) DEVICE LOOK-UP => Calls function "XScuTimer_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XScuTimer_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XScuTimer_SelfTest"
 * 				(4) SPECIFIC CONFIG => Configures SCU Timer for this project
 *
 * 				If any of the first three states results in XST_FAILURE, the
 * 				initialisation will stop and the XST_FAILURE code will be
 * 				returned to the calling code. If initialisation completes with
 * 				no failures, then XST_SUCCESS is returned.
 *
 * @return		Integer indicating result of configuration attempt.
 * 				0 = SUCCESS, 1 = FAILURE
 *
 * @note		None
 *
******************************************************************************/

int xScuTimerInit(uint32_t *p_inst) {

	int status;

	/* Pointer to XScuTimer_Config is required for later functions. */
	XScuTimer_Config *p_XScuTimerCfg = NULL;



	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XScuTimerCfg = XScuTimer_LookupConfig(SCUTIMER_DEVICE_ID);
 	if (p_XScuTimerCfg == NULL)
	{
 		status = XST_FAILURE;
 		return status;
	}


	 /* ---------------------------------------------------------------------
	  * ------------ STEP 2: DRIVER INITIALISATION ------------
	  * -------------------------------------------------------------------- */
 	status = XScuTimer_CfgInitialize(p_XScuTimerInst, p_XScuTimerCfg, p_XScuTimerCfg->BaseAddr);
	 if (status != XST_SUCCESS)
	 {
		 return status;
	 }


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
	status = XScuTimer_SelfTest(p_XScuTimerInst);
	Xil_AssertNonvoid(status == XST_SUCCESS);

	/* If the assertion test fails, we won't get here, but
	* leave the code in anyway, for possible future changes. */
	if (status != XST_SUCCESS)
	{
		return status;
	}

	/* ---------------------------------------------------------------------
	* ------------ STEP 4: PROJECT-SPECIFIC CONFIGURATION ------------
	* -------------------------------------------------------------------- */
	XScuTimer_EnableInterrupt(p_XScuTimerInst);
	XScuTimer_EnableAutoReload(p_XScuTimerInst);
	XScuTimer_LoadTimer(p_XScuTimerInst, SCU_TIMER_LOAD_VALUE);

	/* === END CONFIGURATION SEQUENCE ===  */



	/* Return the pointer to the instance */
	*p_inst = (uint32_t) p_XScuTimerInst;

	/* Return initialisation result to calling code */
	return status;

}



/*****************************************************************************
 * Function: xScuTimerIntrHandler()
 *//**
 *
 * @brief		Interrupt handler for the XSCU Timer. Used in this project
 * 				for triggering tasks in the main code.
 *
 *
 * @details		The basic aim of this function is to assert g_execute_task1
 * 				and g_execute_task2 at the correct timing intervals.
 * 				The flow every time an interrupt occurs is:
 * 				(1) Clear trigger_task1 and trigger_task2, and increment
 * 					the interrupt count.
 * 				(2) Check if the interrupt count is equal to the TASK1_INTR_COUNT.
 * 					If so, assert the trigger_task1 global.
 * 				(3) Else, check if the interrupt count is equal to the TASK2_INTR_COUNT.
 * 					If so, assert trigger_task2, and clear the interrupt count,
 * 					so that the process can repeat.
 *
****************************************************************************/


void xScuTimerIntrHandler(void){

	/* Keep track of interrupt count */
	static uint32_t volatile intr_count = 0U;

	psGpOutSet(PS_GP_OUT0); /// SET TEST SIGNAL: TIMNG INTERRUPT ///


	/* ---------- TASK TIMING LOGIC ---------- */

	/// (1) Init: Clear trigger_taskX variables and increment the intr_count
	psGpOutClear(PS_GP_OUT1); /// CLEAR TEST SIGNAL: TRIGGER TASK 1 ///
	trigger_task1 = 0U;

	psGpOutClear(PS_GP_OUT2); /// CLEAR TEST SIGNAL: TRIGGER TASK 2 ///
	trigger_task2 = 0U;

	intr_count++;

	/* (2)	Check if the interrupt count is equal to the TASK1_INTR_COUNT.
	 * 		If so, assert the trigger_task1 global. */
	if (intr_count == TASK1_INTR_COUNT)
	{
		psGpOutSet(PS_GP_OUT1); /// SET TEST SIGNAL: TRIGGER TASK 1 ///
		trigger_task1 = 1U;
	}

	/* (3)	Check if the interrupt count is equal to the TASK2_INTR_COUNT.
	 * 		If so, assert the g_trigger_task2 global.
	 * 		We also clear the interrupt count in this logic, so that the
	 * 		entire sequence can repeat. */
	else if (intr_count >= TASK2_INTR_COUNT)
	{
		psGpOutSet(PS_GP_OUT2); /// SET TEST SIGNAL: TRIGGER TASK 2 ///
		trigger_task2 = 1U;

		intr_count = 0U;
	}
	else
	{ }



	psGpOutClear(PS_GP_OUT0); /// CLEAR TEST SIGNAL: TIMNG INTERRUPT ///

}




/*****************************************************************************
 * Function: startScuTimer()
 *//**
 *
 * @brief		Calls the XScuTimer function "XScuTimer_Start".
 *
 * @param		None.
 *
 * @return		None.
 *
 * @note		None
 *
******************************************************************************/

void startScuTimer(void){

	XScuTimer_Start(p_XScuTimerInst);

}



/*****************************************************************************
 * Function: waitScuTimerExpired()
 *//**
 *
 * @brief 		Wait for the SCU private timer to reach 0, then return.
 *
 * @details		The following steps must be carried out for effective operation:
 * 				1. In the do-while loop, read the Private Timer interrupt
 * 				register. Bit 0 = 1 means count has reached 0 and loop can exit.
 * 				2. Clear bit 0 in the interrupt register (by writing 1 to it).
 * 				3. Restart the timer by calling XScuTimer_RestartTimer.
 *
 * @param		None.
 *
 * @return		None
 *
 * @note		This function is designed to operate independently of the Zynq
 * 				interrupt system; HOWEVER SCU Timer Interrupts must be enabled
 * 				because the timer interrupt register is used to check if the
 * 				timer has expired. In this project, the timer interrupts are
 * 				enabled using XScuTimer_EnableInterrupt() during system
 * 				initialisation.
 *
******************************************************************************/

void waitScuTimerExpired(void){

	u32 scu_timer_expired;

	/* Extract the base address from the instance pointer */
	uint32_t base_addr = p_XScuTimerInst->Config.BaseAddr;

	/* 'Spin' in this loop while waiting for the count reach 0 */
	do {
		scu_timer_expired = XScuTimer_GetIntrReg(base_addr)
								& XSCUTIMER_ISR_EVENT_FLAG_MASK;
	}
	while (scu_timer_expired == 0);


	/* Clear the 'count = 0' bit in the Private Timer Interrupt Register
	 * (setting the bit clears it), and call the Xilinx restart function. */
	XScuTimer_SetIntrReg(base_addr, XSCUTIMER_ISR_EVENT_FLAG_MASK);
	XScuTimer_RestartTimer(p_XScuTimerInst);

}



/*****************************************************************************
 * Function: getTask1TriggerState()
 *//**
 *
 * @brief		Accessor function to allow top-level main() code to read
 * 				global trigger_task1 status.
 *
 * @return		Current value of trigger state.
 *
 * @note		None.
 *
******************************************************************************/

uint32_t getTask1TriggerState(void)
{
	return trigger_task1;
}



/*****************************************************************************
 * Function: getTask2TriggerState()
 *//**
 *
 * @brief		Accessor function to allow top-level main() code to read
 * 				trigger_task2 status.
 *
 * @return		Current value of trigger state.
 *
 * @note		None.
 *
******************************************************************************/
uint32_t getTask2TriggerState(void)
{
	return trigger_task2;
}



/****** End functions *****/

/****** End of File **********************************************************/


