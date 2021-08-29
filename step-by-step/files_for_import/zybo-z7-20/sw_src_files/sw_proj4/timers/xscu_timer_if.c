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
 * 				(1) DEVICE LOOK-UP => Calls function "XGpio_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XGpio_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XGpio_SelfTest"
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

int xScuTimerInit(void) {

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
	XScuTimer_LoadTimer(p_XScuTimerInst, SCU_TIMER_LOAD_VALUE);

	/* === END CONFIGURATION SEQUENCE ===  */



	/* Return initialisation result to calling code */
	return status;

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

	uint32_t scu_timer_expired;

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


/****** End functions *****/

/****** End of File **********************************************************/


