/******************************************************************************
 * @Title		:	System Configuration
 * @Filename	:	system_config.c
 * @Author		:	Derek Murray
 * @Origin Date	:	06/11/2019
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
 *
 *
 * Copyright (C) 2019 Derek Murray.  All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
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
 * @brief		Calls various functions to initialise the system.
 *
 * @detail		Initialises the following:
 * 				(1) Assertion handling.
 * 				(3) AXI GPIO.
 * 				(2) PS7 GPIO.
 *
 * @param[in]	None.
 *
 * @return		Returns result of configuration attempt:
 * 				XIL_SUCCESS or XIL_FAILURE (int)
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

	p_init_status->xgpio0 = axiGpio0Init(); 		// AXI GPIO
	p_init_status->xgpiops = psGpioInit(); 			// PS7 GPIO

#if SYS_CONFIG_DEBUG
	if (p_init_status->xgpio0 != XST_SUCCESS) 		{ printf("AXI GPIO initialisation failed.\n\r"); }
	else											{ printf("AXI GPIO initialisation succeeded.\n\r"); }

	if (p_init_status->xgpiops != XST_SUCCESS) 		{ printf("PS7 GPIO initialisation failed.\n\r\n\r"); }
	else											{ printf("PS7 GPIO initialisation succeeded.\n\r\n\r"); }
#endif



    /* ----- Check result of initialisation and return to calling function ----- */

	if 	( (p_init_status->xgpio0 == XST_SUCCESS)			// AXI GPIO
    	&& (p_init_status->xgpiops == XST_SUCCESS) )		// PS7 GPIO
    {
    	return XST_SUCCESS;
    }
    else {
    	return XST_FAILURE;
    }
}



/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


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




