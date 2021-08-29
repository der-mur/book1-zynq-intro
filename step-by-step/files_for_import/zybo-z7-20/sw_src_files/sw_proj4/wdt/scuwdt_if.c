/******************************************************************************
 * @Title		:	SCUWDT Interface
 * @Filename	:	scuwdt_if.c
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

#include "scuwdt_if.h"



/************************** Variable Definitions ****************************/

/* Declare instance and associated pointer for SCUWDT */
static XScuWdt		XScuWdtInst;
static XScuWdt 		*p_XScuWdtInst = &XScuWdtInst;




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


/*****************************************************************************
 * Function: xScuWdtInit()
 *//**
 *
 * @brief		Configures the SCUWDT for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	SCU WDT.
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


int xScuWdtInit(void){

	int status;

	/* Pointer to XScuWdt_Config is required for functions below. */
	XScuWdt_Config *p_XScuWdtCfg = NULL;


#if SCUWDT_DEBUG
	checkRebootStatus();
#endif

	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XScuWdtCfg = XScuWdt_LookupConfig(SCUWDT_DEVICE_ID);
 	if (p_XScuWdtCfg == NULL)
	{
 		status = XST_FAILURE;
 		return status;
	}


 	/* ---------------------------------------------------------------------
 	 * ------------ STEP 2: DRIVER INITIALISATION ------------
	 * -------------------------------------------------------------------- */
 	status = XScuWdt_CfgInitialize(p_XScuWdtInst, p_XScuWdtCfg, p_XScuWdtCfg->BaseAddr);
 	if (status != XST_SUCCESS)
 	{
 		return status;
	}


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
	status = XScuWdt_SelfTest(p_XScuWdtInst);
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
	// Load the watchdog counter register, and start it.
	XScuWdt_LoadWdt(p_XScuWdtInst, SCUWDT_LOAD_VALUE);
	XScuWdt_Start(p_XScuWdtInst);

	/* === END CONFIGURATION SEQUENCE ===  */


	/* Return initialisation result to calling code */
	return status;
}




/*****************************************************************************
 * Function: restartScuWdt()
 *//**
 *
 * @brief		Calls the XScuWdt function "restartScuWdt" to 'kick' the
* 				SCU watchdog timer.
 *
 * @return		None.
 *
 * @note		None.
 *
******************************************************************************/

void restartScuWdt(void)
{
	XScuWdt_RestartWdt(p_XScuWdtInst);
}



/*****************************************************************************
 * Function: checkRebootStatus()
 *//**
 *
 * @brief		Reads the SLCR Reboot status register and prints out the status
 * 				of possible reboot causes.
 *
 * @return		None.
 *
 * @note		This function is only really meant for debug/informative
 * 				reasons. A terminal must be connected.
 *
******************************************************************************/

void checkRebootStatus(void){

	uint32_t slcr_reboot_sts = Xil_In32(SLCR_REBOOT_STATUS_REG);

	printf("\r\n-----------------------------------------------------------\r\n");
	printf("SLCR Reboot Status Register: \r\n");
	printf("SWDT_RST  = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_SWDT_RST_MASK) != 0) );
	printf("AWDT0_RST = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_AWDT0_RST_MASK) != 0) );
	printf("AWDT1_RST = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_AWDT1_RST_MASK) != 0) );
	printf("SLC_RST   = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_SLC_RST_MASK) != 0) );
	printf("DBG_RST   = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_DBG_RST_MASK) != 0) );;
	printf("SRST_B    = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_SRST_B_MASK) != 0) );
	printf("POR       = %x\r\n", ( (slcr_reboot_sts & SLCR_RS_POR_MASK) != 0) );
	printf("(Note: Power-cycle (POR) required to clear this register.)\r\n");
	printf("-----------------------------------------------------------\r\n\r\n");
}

/****** End functions *****/

/****** End of File **********************************************************/






