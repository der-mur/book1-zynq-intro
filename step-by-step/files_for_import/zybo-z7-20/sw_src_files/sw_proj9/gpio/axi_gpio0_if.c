 /******************************************************************************
 * @Title		:	AXI GPIO Interface (Header File)
 * @Filename	:	axi_gpio0_if.h
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



/******************************************************************************
* AXI GPIO0 Channel and Pin Mapping
*
* Channel 1 = outputs: Board LEDS[3:0], PMOD GP_OUT[3:0]
*	Mapping is:
*		Bit 0:LED0; Bit1:LED1; Bit2:LED2 Bit3:LED3.
*		Bit 4:GP_IN0; Bit 5:GP_IN1; Bit 6:GP_IN2; Bit 7: GP_IN3;
*
* Channel 2 = inputs: Board BTN[3:0], SW[3:0], PMOD GP_IN[3:0].
*	Mapping is:
*		Bit 0:BTN0; Bit1:BTN1; Bit2:BTN2; Bit3:BTN3;
* 		Bit 4:SW0; Bit 5:SW1; Bit 6:SW2; Bit 7:SW3.
* 		Bit 8:GP_IN0; Bit 9:GP_IN1; Bit 10:GP_IN2; Bit 11:GP_IN3;
*
******************************************************************************/


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "axi_gpio0_if.h"



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/

/* Declare instance and associated pointer for AXI GPIO */
static XGpio 		XGpio0Inst;
static XGpio 		*p_XGpio0Inst = &XGpio0Inst;




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


/*****************************************************************************
 * Function: axiGpio0Init()
 *//**
 *
 * @brief		Configures the PL AXI GPIO for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	channels. In this project, channel 1 is the
 * 				output channel,	and channel 2 is the input channel.
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

int axiGpio0Init(void)
{
	int status;

	/* Pointer to XGpio_Config is required for later functions. */
	XGpio_Config *p_XGpio0Cfg = NULL;



	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XGpio0Cfg = XGpio_LookupConfig(AXI_GPIO0_DEVICE_ID);
	 if (p_XGpio0Cfg == NULL)
	 {
		 status = XST_FAILURE;
		 return status;
	 }


	 /* ---------------------------------------------------------------------
	  * ------------ STEP 2: DRIVER INITIALISATION ------------
	  * -------------------------------------------------------------------- */
	 status = XGpio_CfgInitialize(p_XGpio0Inst, p_XGpio0Cfg, p_XGpio0Cfg->BaseAddress);
	 if (status != XST_SUCCESS)
	 {
		 return status;
	 }


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
	status = XGpio_SelfTest(p_XGpio0Inst);
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
	/* Configure channel 1 to be all outputs */
	XGpio_SetDataDirection(p_XGpio0Inst, AXI_GPIO0_OP_CHANNEL, 0);

	/* Configure channel 2 to be inputs, depending on AXI_GPIO0_IP_MASK */
	XGpio_SetDataDirection(p_XGpio0Inst, AXI_GPIO0_IP_CHANNEL, AXI_GPIO0_IP_MASK);

	/* All LEDs initially off */
	axiGpOutClear(LED0);
	axiGpOutClear(LED1);
	axiGpOutClear(LED2);
	axiGpOutClear(LED3);

	/* === END CONFIGURATION SEQUENCE ===  */



	/* Return initialisation result to calling code */
	return status;

}




/*****************************************************************************
 * Function: axiGpOutSet()
 *//**
 *
 * @brief		Sets an AXI GPIO output pin.
 *
 * @param[in]	AxiGpio0_OutPin_t pin: The AXI channel(1) pin to set.
 *
 * @return		None
 *
 * @note		Assert functionality: Only accept output channel bits [7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void axiGpOutSet(AxiGpio0_OutPin_t pin){

	/* Function should only be passed GPIO bits 0-7 */
	Xil_AssertVoid(pin < 8);

	XGpio_DiscreteSet(p_XGpio0Inst, AXI_GPIO0_OP_CHANNEL, (1 << pin));

}



/*****************************************************************************
 * Function: axiGpOutClear()
 *//**
 *
 * @brief		Clears an AXI GPIO output pin.
 *
 * @param[in]	AxiGpio0_OutPin_t pin		The AXI channel(1) pin to clear.
 *
 * @return		None
 *
 * @note		Assert functionality: Only accept output channel bits [7:0];
 * 				Assert otherwise..
 *
******************************************************************************/

void axiGpOutClear(AxiGpio0_OutPin_t pin){

	/* Function should only be passed GPIO bits 0-7 */
	Xil_AssertVoid(pin < 8);

	XGpio_DiscreteClear(p_XGpio0Inst, AXI_GPIO0_OP_CHANNEL, (1 << pin));

}



/*****************************************************************************
 * Function: axiGpOutToggle()
*//**
 *
 * @brief		Toggles an AXI GPIO output pin.
 *
 * @param[in]	AxiGpio0_OutPin_t pin		The AXI channel(1) pin to toggle.
 *
 * @return		None
 *
 * @note		Assert functionality: Only accept output channel bits [7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void axiGpOutToggle(AxiGpio0_OutPin_t pin){

	uint32_t pin_state;

	/* Function should only be passed GPIO bits 0-7 */
	Xil_AssertVoid(pin < 8);


	/* Read the current pin state, then use it to change the pin accordingly */
	pin_state = (XGpio_DiscreteRead(p_XGpio0Inst, AXI_GPIO0_OP_CHANNEL)
	 				& (1 << pin)) != 0;

	if (pin_state == 1)
	{
		axiGpOutClear(pin);
	}
	else
	{
		axiGpOutSet(pin);
	}
}



/*****************************************************************************
 * Function: axiGpInRead()
 *//**
 *
 * @brief		Reads an AXI GPIO input pin.
 *
 * @param[in]	AxiGpio0_OutPin_t pin		The AXI channel(2) pin to read.
 *
 * @return		1U if pin is set, else 0U.
 *
 * @note		Assert functionality: Only accept input channel bits [11:0];
 * 				Assert otherwise.
 *
******************************************************************************/

uint32_t axiGpInRead(AxiGpio0_InPin_t pin){

	uint32_t pin_state;

	/* Function should only be passed GPIO bits 0-11 */
	Xil_AssertNonvoid(pin < 12);

	/* Read input channel and check if bit is set */
	pin_state = (XGpio_DiscreteRead(p_XGpio0Inst, AXI_GPIO0_IP_CHANNEL)
					& (1 << pin)) != 0;

	return pin_state;

}


/****** End functions *****/

/****** End of File **********************************************************/






