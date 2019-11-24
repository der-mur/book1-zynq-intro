/******************************************************************************
 * @Title		:	Triple Timer Counter 0 Interface
 * @Filename	:	ttc0_if.c
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





/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "ttc0_if.h"



/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/



/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/


/* Declare instance and associated pointer for TTC0 */
static XTtcPs		XTtc0PsInst;
static XTtcPs 		*p_XTtc0PsInst = &XTtc0PsInst;


/* Task trigger signals; Scope is local to this file.
 * Accessor functions are used to give access to other files. */
static uint32_t volatile trigger_task1;
static uint32_t volatile trigger_task2;


/****************************************************************************/
/***************** Macros (Inline Functions) Definitions ********************/
/****************************************************************************/



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/



/*****************************************************************************
 * Function: xTtc0Init()
 *//**
 *
 * @brief		Configures the TTC0 Timer for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	TTC0 Timer for this project.
 *
 * 				The initialisation steps are:
 * 				(1) DEVICE LOOK-UP => Calls function "XTtcPs_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XTtcPs_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XTtcPs_SelfTest"
 * 				(4) SPECIFIC CONFIG => Configures TTC0 for this project
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

int xTtc0Init(uint32_t *p_inst) {

	int status;


	/* Pointer to XTtcPs_Config is required for later functions. */
	XTtcPs_Config *p_XTtc0PsCfg = NULL;


	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XTtc0PsCfg = XTtcPs_LookupConfig(PS7_TTC0_DEVICE_ID);
 	if (p_XTtc0PsCfg == NULL)
	{
 		status = XST_FAILURE;
 		return status;
	}


 	/* ---------------------------------------------------------------------
	 * ------------ STEP 2: DRIVER INITIALISATION ------------
	 * -------------------------------------------------------------------- */
 	/*  TIMER MUST BE DISABLED BEFORE ATTEMPTING CONFIGURATION */
 	XTtcPs_WriteReg(p_XTtc0PsCfg->BaseAddress, XTTCPS_CNT_CNTRL_OFFSET, 1U);

 	status = XTtcPs_CfgInitialize(p_XTtc0PsInst, p_XTtc0PsCfg, p_XTtc0PsCfg->BaseAddress);
 	if (status != XST_SUCCESS)
	{
 		return status;
	}


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
 	status = XTtcPs_SelfTest(p_XTtc0PsInst);
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
	/* Configuration steps are:
	* (1) Set clock control options (prescaler)
	* (2) Set Match mode.
	* (3) Set MATCH0 value to TASK1_MATCH.
	* (4) Set MATCH1 value to TASK2_MATCH.
	* (5) Enable MATCH0 and MATCH1 interrupts. */
	XTtcPs_SetPrescaler(p_XTtc0PsInst, PRESCALER_VALUE);
	XTtcPs_SetOptions(p_XTtc0PsInst, XTTCPS_OPTION_MATCH_MODE);
	XTtcPs_SetMatchValue(p_XTtc0PsInst, 0, TASK1_MATCH);
	XTtcPs_SetMatchValue(p_XTtc0PsInst, 1, TASK2_MATCH);
	XTtcPs_EnableInterrupts(p_XTtc0PsInst, XTTCPS_IXR_MATCH_0_MASK
										| XTTCPS_IXR_MATCH_1_MASK );


	/* === END CONFIGURATION SEQUENCE ===  */


	/* Update the pointer in the calling code */
	*p_inst = (uint32_t) p_XTtc0PsInst;

	/* Return initialisation result to calling code */
	return status;

}



/*****************************************************************************
 * Function: xTtc0IntrHandler()
 *//**
 *
 * @brief		Interrupt handler for the TTC0 Timer. Used in this project
 * 				for triggering tasks in the main code.
 *
 *
 * @details		The basic aim of this function is to assert trigger_task1
 * 				and trigger_task2 at the correct timing intervals. TTC0 is
 * 				configured in Match mode, and match registers 0 and 1 are used:
 * 				MATCH0 = trigger task 1.
 * 				MATCH1 = trigger task 2.
 *
 * 				The basic flow every time an interrupt occurs is:
 * 				(1) Read the TTC0 interrupt status.
 * 				(2) Clear the interrupt.
 * 				(3) If interrupt status = MATCH0:
 * 						(a) Assert trigger_task1.
 * 				(4) Else if interrupt status = MATCH1:
 * 						(a) Assert trigger_task2.
 * 						(b) Reset TTC0 count so that the task sequence can
 * 							start again.
 *
 *
 * @note		Match values are defined in ttc0_if.h
 *
****************************************************************************/

void xTtc0IntrHandler(void *CallBackRef){


	psGpOutSet(PS_GP_OUT0); /// SET TEST SIGNAL: TIMNG INTERRUPT ///

	trigger_task1 = 0U;
	trigger_task2 = 0U;



	/* Read and clear TTC0 interrupts */
	uint32_t status_event = 0U;
	status_event = XTtcPs_GetInterruptStatus((XTtcPs *)CallBackRef);
	XTtcPs_ClearInterruptStatus((XTtcPs *)CallBackRef, status_event);

	/* Assert trigger_taskX depending on the MATCH interrupt. */

	if (0 != (XTTCPS_IXR_MATCH_0_MASK & status_event))
	{
		psGpOutSet(PS_GP_OUT1); 	/// SET TEST SIGNAL: TRIGGER TASK 1 ///

		trigger_task1 = 1U;

		psGpOutClear(PS_GP_OUT1);   /// CLEAR TEST SIGNAL: TRIGGER TASK 1 ///
	}
	else if (0 != (XTTCPS_IXR_MATCH_1_MASK & status_event))
	{
		psGpOutSet(PS_GP_OUT2); 	/// SET TEST SIGNAL: TRIGGER TASK 2 ///

		trigger_task2 = 1U;
		resetTtc0();

		psGpOutClear(PS_GP_OUT2);   /// CLEAR TEST SIGNAL: TRIGGER TASK 1 ///
	}
	else
		{ }

	psGpOutClear(PS_GP_OUT0);

}







/*****************************************************************************
 * Function: startTtc0()
 *//**
 *
 * @brief		Starts TTC0 by calling XTtcPs_Start().
 *
 * @return 		None
 *
 * @note		None
 *
****************************************************************************/

void startTtc0(void)
{
	XTtcPs_Start(p_XTtc0PsInst);
}



/*****************************************************************************
 * Function: startTtc0()
 *//**
 *
 * @brief		Resets(and restarts) TTC0 by calling XTtcPs_ResetCounterValue().
 *
 * @return 		None
 *
 * @note		None
 *
****************************************************************************/

void resetTtc0(void)
{
	XTtcPs_ResetCounterValue(p_XTtc0PsInst);
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
