/******************************************************************************
 * @Title		:	System Configuration
 * @Filename	:	system_config.c
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

#include "system_config.h"




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


/*****************************************************************************
 * Function: sys_init()
 *//**
 *
 * @brief		Calls various functions to initialize the system.
 *
 * @detail		Initialises the following:
 * 				(1) Assertion handling.
 * 				(2) SCU WDT
 * 				(3) AXI GPIO.
 * 				(4) PS7 GPIO.
 * 				(5) SCU Timer
 * 				Function runs all the way to the end (unless an assertion is
 * 				triggered in one of the device init routines), and then checks if
 * 				all device were initialised correctly. That is, it does not
 * 				automatically return if any device fails to initialise.
 *
 * @param[in]	None.
 *
 * @return		Returns result of configuration attempt:
 * 				XIL_SUCCESS or XIL_FAILURE
 *
 * @note		None
 *
******************************************************************************/

int sys_init(void){


	/*
	 * Create init_status instance and associated pointer
	 * to track initialisation progress
	 */
	init_status_t init_status;
	init_status_t  *p_init_status = &init_status;



	/*---------------------------------------------------*/
    /* ------- Configure Assertion Functionality ------- */
	/*---------------------------------------------------*/

	/* Set up our callback function (AssertPrint) to be invoked when an assert occurs */
	Xil_AssertSetCallback((Xil_AssertCallback) AssertPrint);



	/*---------------------------------------------------*/
    /* ------------- Driver Initialization ------------- */
	/*---------------------------------------------------*/

#if SYS_CONFIG_DEBUG
	printf("\n\r===== Initialising Drivers =====\n\r");
#endif

	p_init_status->xscu_wdt = xScuWdtInit();		// SCU WDT
	p_init_status->xgpio0 = axiGpio0Init(); 		// AXI GPIO
	p_init_status->xgpiops = psGpioInit(); 			// PS7 GPIO
	p_init_status->xscu_timer = xScuTimerInit();	// SCU TIMER


#if SYS_CONFIG_DEBUG
	if (p_init_status->xscu_wdt != XST_SUCCESS) 	{ printf("SCUWDT initialisation failed.\n\r"); }
	else											{ printf("SCUWDT initialisation succeeded.\n\r"); }

	if (p_init_status->xgpio0 != XST_SUCCESS) 		{ printf("AXI GPIO initialisation failed.\n\r"); }
	else											{ printf("AXI GPIO initialisation succeeded.\n\r"); }

	if (p_init_status->xgpiops != XST_SUCCESS) 		{ printf("PS7 GPIO initialisation failed.\n\r"); }
	else											{ printf("PS7 GPIO initialisation succeeded.\n\r"); }

	if (p_init_status->xscu_timer != XST_SUCCESS) 	{ printf("SCU Timer initialisation failed.\n\r"); }
	else											{ printf("SCU Timer initialisation succeeded.\n\r"); }
#endif


    /* ----- Check result of initialisation and return to calling function ----- */

	if 	( (p_init_status->xscu_wdt == XST_SUCCESS)			// SCUWDT
    	&& (p_init_status->xgpio0 == XST_SUCCESS)			// AXI GPIO
    	&& (p_init_status->xgpiops == XST_SUCCESS)			// PS7 GPIO
		&& (p_init_status->xscu_timer == XST_SUCCESS) )		// SCU TIMER
    {
    	return XST_SUCCESS;
    }
    else {
    	return XST_FAILURE;
    }
}




/*****************************************************************************
 * Function: AssertPrint()
 *//**
 *
 * @brief		This is the callback function to be invoked when an assert
 * 				occurs.
 *
 * @details		The function prints out the file and line number where the
 * 				assertion has occurred, assuming a terminal/console is
 * 				connected to the test platform. It is used with the function
 * 				Xil_AssertSetCallback()  (see xil_assert.c).
 *
 * @param[in]	char8 *file:	Pointer to file where assert occurred.
 * @param[in] 	s32 line:		Line in file where assert occurred.
 *
 *
 * @return		None.
 *
 * @note		A stdout terminal or other UART host program must be connected
 * 				to see the output!
 *
******************************************************************************/

void AssertPrint(const char8 *file, s32 line) {
	xil_printf("\r\nAssertion in file %s on line %d\r\n", file, line);
}





/****** End functions *****/

/****** End of File **********************************************************/




