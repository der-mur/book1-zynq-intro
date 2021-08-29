/******************************************************************************
 * @Title		:	PS7 GPIO Interface
 * @Filename	:	ps7_gpio_if.c
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

#include "ps7_gpio_if.h"



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/

/* Declare instance and associated pointer for PS7 GPIO */
static XGpioPs		XGpioPsInst;
static XGpioPs 		*p_XGpioPsInst = &XGpioPsInst;




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/



/*****************************************************************************
 * Function: psGpioInit()
 *//**
 *
 * @brief		Configures the PS7 GPIO for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	pins as required for this project.
 *
 * 				The initialisation steps are:
 * 				(1) DEVICE LOOK-UP => Calls function "XGpio_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XGpio_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XGpio_SelfTest"
 * 				(4) SPECIFIC CONFIG => Configures GPIO channels and pins
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

int psGpioInit(void)
{

	int status;

	/* Pointer to XGpioPs_Config is required for later functions. */
	XGpioPs_Config *p_XGpioPsCfg = NULL;


	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XGpioPsCfg = XGpioPs_LookupConfig(PS7_GPIO_DEVICE_ID);
	 if (p_XGpioPsCfg == NULL)
	 {
		 status = XST_FAILURE;
		 return status;
	 }


	 /* ---------------------------------------------------------------------
	  * ------------ STEP 2: DRIVER INITIALISATION ------------
	  * -------------------------------------------------------------------- */
	 status = XGpioPs_CfgInitialize(p_XGpioPsInst, p_XGpioPsCfg, p_XGpioPsCfg->BaseAddr);
	 if (status != XST_SUCCESS)
	 {
		 return status;
	 }


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
	status = XGpioPs_SelfTest(p_XGpioPsInst);
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
	/* Set LED4 direction to output, enable it, and turn off */
	XGpioPs_SetDirectionPin(p_XGpioPsInst, LED4, 1);
	XGpioPs_SetOutputEnablePin(p_XGpioPsInst, LED4, 1);
	XGpioPs_WritePin(p_XGpioPsInst, LED4, 0);

	/* Set BTN4/5 to be inputs */
	XGpioPs_SetDirectionPin(p_XGpioPsInst, BTN4, 0);
	XGpioPs_SetDirectionPin(p_XGpioPsInst, BTN5, 0);

	/* === END CONFIGURATION SEQUENCE ===  */



	/* Return initialisation result to calling code */
	return status;

}




/*****************************************************************************
 * Function: psGpOutSet()
 *//**
 *
 * @brief		Sets a PS GPIO output pin.
 *
 * @param[in]	PsGpio_OutPin_t pin		The PS MIO pin to set.
 *
 * @return		None
 *
 *				Assert functionality: Only accept pins LED4 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutSet(PsGpio_OutPin_t pin){

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED4));


	XGpioPs_WritePin( p_XGpioPsInst, pin, 0x01 );

}



/*****************************************************************************
 * Function: psGpOutClear()
 *//**
 *
 * @brief		Clears a PS GPIO output pin.
 *
 * @param[in]	PsGpio_OutPin_t pin		The PS MIO pin to clear.
 *
 * @return		None
 *
 *				Assert functionality: Only accept pins LED4 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutClear(PsGpio_OutPin_t pin){

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED4));


	XGpioPs_WritePin( p_XGpioPsInst, pin, 0x00 );

}


/*****************************************************************************
 * Function: psGpOutToggle()
 *//**
 *
 * @brief		Toggles a PS GPIO output pin.
 *
 * @param[in]	PsGpio_OutPin_t pin		The PS MIO pin to toggle.
 *
 * @return		None
 *
 * @note		To get the value of PS GPIO pins, we use XGpioPs_ReadReg().
 * 				The reason we need to use this command is because XGpioPs_ReadPin
 * 				does not seem to work for MIO7 (LED4). The offset used is
 * 				XGPIOPS_DATA_LSW_OFFSET, as are only dealing with the PS GPIO
 * 				bits in bank 0 (i.e. the MIO's used in the project are 0, 7, 9,
 * 				10, 11, 12, 13, 14, 15).
 *
 * 				Also, to be absolutely precise, this read does not return the
 * 				current value of the pin, but the last value written to the
 * 				register. In terms of toggle functionality, this is all we need.
 *
 *				Assert functionality: Only accept pins LED4 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutToggle(PsGpio_OutPin_t pin){

	uint32_t pin_state;

	/* Extract UART1 base address from instance pointer */
	uint32_t base_addr = p_XGpioPsInst->GpioConfig.BaseAddr;

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED4));



	/* Check the GPIO state and toggle the value */
	pin_state = ( XGpioPs_ReadReg(base_addr, XGPIOPS_DATA_LSW_OFFSET)
	 				& (1 << pin) ) != 0;

	if (pin_state == 0x00000001)
	{
		psGpOutClear(pin);
	}
	else
	{
		psGpOutSet(pin);
	}
}



/*****************************************************************************
 * Function: psGpInRead()
 *//**
 *
 * @brief		Reads a PS GPIO input pin.
 *
 * @param[in]	PsGpio_InPin_t pin		The PS MIO pin to read.
 *
 * @return		1U if pin is set, else 0U.
 *
 * @note		Assert functionality: Only accept MIO GPIO inputs 50 and 51
 * 				(BTN4 and BTN5); Assert otherwise.
 *
******************************************************************************/

uint32_t psGpInRead(PsGpio_InPin_t pin){

	uint32_t pin_state;

	/* Function should only handle BTN4 or BTN5 */
	Xil_AssertNonvoid( (pin == BTN4) || (pin == BTN5) );


	pin_state = XGpioPs_ReadPin(p_XGpioPsInst, pin);

	return pin_state;
}


/****** End functions *****/

/****** End of File **********************************************************/


