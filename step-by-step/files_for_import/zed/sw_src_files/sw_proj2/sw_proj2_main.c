/******************************************************************************
 * @Title		:	Zynq Fundamentals Software Project 2
 * @Filename	:	sw_proj2_main.c
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
 *
 *
 *------------------------------------------------------------------------
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

#include <stdio.h>
#include "xparameters.h"
#include "xgpiops.h"
#include "xgpio.h"
#include "xil_assert.h"
#include "xil_printf.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define DEBUG 						0


/*----------------------------- DEVICE IDS -----------------------------*/
#define PS7_GPIO_DEVICE_ID			XPAR_PS7_GPIO_0_DEVICE_ID		// PS7 GPIO
#define AXI_GPIO0_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID		// AXI GPIO



/*------------------------- DESIGN DETAILS ---------------------------- */

/* ----- AXI GPIO 0 -----
 *//**
 * AXI GPIO Channel 1 = Output
 * AXI GPIO Channel 2 = Input channel.
 * MASK = 0x0FFF means lower 12 bits of channel 2 are set to input. */
#define AXI_GPIO0_OP_CHANNEL 		1U
#define AXI_GPIO0_IP_CHANNEL 		2U
#define AXI_GPIO0_IP_MASK			(0x00000FFF)



/* ------------------------- RUN-TIME PARAMETERS ---------------------- */

/* Main loop time delay */
#define MAIN_LOOP_DELAY					10000000U



/*****************************************************************************/
/**************************** Type Definitions *******************************/
/*****************************************************************************/

/* ----------------------------------------------------------------------------
 * ----- PS7 GPIO -----
 *//**
 * For this project, LED9 and all PMOD JF pins are outputs.
 * BTN8 and BTN9 are inputs.
 * ---------------------------------------------------------------------------*/

typedef enum
{
	LED9 = 7,			/** MIO 7 */
	PS_GP_OUT0 = 13,	/** MIO13, PMOD JF PIN 1 */
	PS_GP_OUT1 = 10,	/** MIO10, PMOD JF PIN 2 */
	PS_GP_OUT2 = 11,	/** MIO11, PMOD JF PIN 3 */
	PS_GP_OUT3 = 12, 	/** MIO12, PMOD JF PIN 4 */
	PS_GP_OUT4 = 0, 	/** MIO0, PMOD JF PIN 7 */
	PS_GP_OUT5 = 9, 	/** MIO9, PMOD JF PIN 8 */
	PS_GP_OUT6 = 14, 	/** MIO14, PMOD JF PIN 9 */
	PS_GP_OUT7 = 15 	/** MIO15, PMOD JF PIN 10 */
}PsGpio_OutPin_t;

typedef enum
{
	BTN8 = 50,			/** MIO 50 */
	BTN9 = 51			/** MIO 51 */
}PsGpio_InPin_t;



/* ----------------------------------------------------------------------------
 * ----- AXI GPIO 0 -----
 *//**
 * For this project, channel 1 is the output channel.
 * 8 outputs are used: 4 x Board LEDS 0-3; 4 x GP_OUT 0-3 (PMOD JE Pins 1-4)
 * Channel 2 is the input channel. 12 inputs are used:
 * 4 x Board BTNS 0-3; 4 x Board SW 0-3; 4 x GP_IN 0-3 (PMOD JE pins 7-10)
 * --------------------------------------------------------------------------*/

typedef enum
{
	LED0, LED1, LED2, LED3,
	GP_OUT0, GP_OUT1, GP_OUT2,
	GP_OUT3
}AxiGpio0_OutPin_t;


typedef enum
{
	BTNU, BTNR, BTND, BTNL,
	SW0, SW1, SW2, SW3,
	GP_IN0, GP_IN1, GP_IN2, GP_IN3
}AxiGpio0_InPin_t;



/*****************************************************************************/
/************************** Variable Definitions *****************************/
/*****************************************************************************/

/* Declare instance and associated pointer for AXI GPIO */
static XGpio 		XGpio0Inst;
static XGpio 		*p_XGpio0Inst = &XGpio0Inst;


/* Declare instance and associated pointer for PS7 GPIO */
static XGpioPs		XGpioPsInst;
static XGpioPs 		*p_XGpioPsInst = &XGpioPsInst;


/*****************************************************************************/
/***************** Macros (Inline Functions) Definitions *********************/
/*****************************************************************************/

#define printf 		xil_printf 	/* Small foot-print printf function */



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Assertion functions */
void AssertPrint(const char8 *file, s32 line);

/* Device Initialization */
int axiGpio0Init(void);
int psGpioInit(void);


/* PS7 GPIO functions */
void psGpOutSet(PsGpio_OutPin_t pin);
void psGpOutClear(PsGpio_OutPin_t pin);
void psGpOutToggle(PsGpio_OutPin_t pin);
uint32_t psGpInRead(PsGpio_InPin_t pin);


/* AXI GPIO functions */
void axiGpOutSet(AxiGpio0_OutPin_t pin);
void axiGpOutClear(AxiGpio0_OutPin_t pin);
void axiGpOutToggle(AxiGpio0_OutPin_t pin);
uint32_t axiGpInRead(AxiGpio0_InPin_t pin);



/*****************************/

int main(void)
{


	// ********************************************************************************* //
	// *****  INITIALIZATION PHASE *****
	// ********************************************************************************* //

	/* Xilinx function to init cache */
	// init_platform();

	/* Set up our callback function (AssertPrint) to be invoked when an assert occurs */
	Xil_AssertSetCallback((Xil_AssertCallback) AssertPrint);


	printf("\n\r---------------------------------------------------------------\n\r");
	printf("---------- Zynq Fundamentals Software Project 2 ----------\n\r");
	printf("---------------------------------------------------------------\n\r");
	printf("Title: Zynq GPIO.\n\r");
	printf("Architecture: Infinite polling loop with for-loop timing delay.\n\r\n\r");


	printf("\n\r===== Initializing Drivers =====\n\r");

    /* AXI GPIO */
	int status;

	status = axiGpio0Init();
    if (status != XST_SUCCESS){
    	printf("AXI GPIO initialization failed.\n\r");
    	return 0;
    }
    else{
    	printf("AXI GPIO initialization succeeded.\n\r");
    }


    /* PS7 GPIO */
    status = psGpioInit();
    if (status != XST_SUCCESS){
    	printf("PS7 GPIO initialization failed.\n\r");
    	return 0;
    }
    else{
    	printf("PS7 GPIO initialization succeeded.\n\r\r\n");
    }

	printf("Press BTNU or set SW0 to ON to toggle LED0.\r\n");
	printf("Press BTN8 or BTN9 to toggle LED9.\r\n\r\n");


	// ********************************************************************************* //
	// *****   MAIN PROGRAM [SIMPLE POLLING 'ARCHITECTURE'] *****
	// ********************************************************************************* //
	/* AXI GPIO */
	volatile uint32_t btnU_state;
	volatile uint32_t sw0_state;
	
	/* PS GPIO */
	volatile uint32_t btn8_state;
	volatile uint32_t btn9_state;

	/* Program delay */
	volatile uint32_t delay;



    for(;;) // Infinite loop
	{

		/* --------------- AXI GPIO ------------------
		 *//**
		 * Read BTNU and SW0 states.
		 * If BTNU pressed or SW0 On, then toggle LED0
		 * Default state is led on.
		 */

		btnU_state = axiGpInRead(BTNU);
		sw0_state = axiGpInRead(SW0);


		if ( (btnU_state == 1) || (sw0_state == 1) ){
			axiGpOutToggle(LED0);
		}
		else{
			axiGpOutSet(LED0);
		}



		/* --------------- PS7 GPIO ------------------------
		 *//**
		 * If BTN8 pressed or BTN9 pressed, then toggle LED9
		 * Default state is led on.
		 */

		btn8_state = psGpInRead(BTN8);
		btn9_state = psGpInRead(BTN9);


		if ( (btn8_state == 1) || (btn9_state == 1) ){
			psGpOutToggle(LED9);
		}
		else{
			psGpOutSet(LED9);
		}




		/* === Delay before restart polling. === */
		// Crude timed-loop mechanism:
		for (delay = MAIN_LOOP_DELAY; delay != 0; delay--)
			{}

	}

	// Should never get here.

	return 0;

} /* End main() */



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



/*****************************************************************************
 * Function: axiGpio0Init()
 *//**
 *
 * @brief		Configures the PL AXI GPIO for use. Starts by doing device
 * 				look-up, configuration and self-test. Then configures the
 * 				channels. In this project, channel 1 is the output channel,
 * 				and channel 2 is the input channel,
 *
 *
 * @return		Integer indicating result of configuration attempt.
 * 				0 = SUCCESS, 1 = FAILURE
 *
 * @note		None.
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



	/* If we get this far, then assume init is successful */
	return XST_SUCCESS;

} /* End axiGpio0Init() */


/*****************************************************************************
 * Function: psGpioInit()
 *//**
 *
 * @brief		Configures the PS7 GPIO for use. Starts by doing device
 * 				look-up, configuration and self-test. Then carries out 
				project-specific settings.
 *
 *
 * @return		Integer indicating result of configuration attempt.
 * 				0 = SUCCESS, 1 = FAILURE
 *
 * @note		See PsGpio_OutPin_t definition for output pins, and
 * 				PsGpio_InPin_t definition for input pins.
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
	/* Set LED9 direction to output, enable it, and turn off */
	XGpioPs_SetDirectionPin(p_XGpioPsInst, LED9, 1);
	XGpioPs_SetOutputEnablePin(p_XGpioPsInst, LED9, 1);
	XGpioPs_WritePin(p_XGpioPsInst, LED9, 0);

	/* Set BTN8/5 to be inputs */
	XGpioPs_SetDirectionPin(p_XGpioPsInst, BTN8, 0);
	XGpioPs_SetDirectionPin(p_XGpioPsInst, BTN9, 0);

	/* === END CONFIGURATION SEQUENCE ===  */


	
	/* If we get this far, then assume init is successful */
	return XST_SUCCESS;

} /* End psGpioInit() */




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

	/* Read input channel and check if pin is set */
	pin_state = (XGpio_DiscreteRead(p_XGpio0Inst, AXI_GPIO0_IP_CHANNEL)
					& (1 << pin)) != 0;

	return pin_state;

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
 *				Assert functionality: Only accept pins LED9 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutSet(PsGpio_OutPin_t pin){

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED9));


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
 *				Assert functionality: Only accept pins LED9 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutClear(PsGpio_OutPin_t pin){

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED9));


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
 * 				does not seem to work for MIO7 (LED9). The offset used is
 * 				XGPIOPS_DATA_LSW_OFFSET, as we are only dealing with the PS GPIO
 * 				bits in bank 0 (i.e. the MIO's used in the project are 0, 7, 9,
 * 				10, 11, 12, 13, 14, 15).
 *
 * 				Also, to be absolutely precise, this read does not return the
 * 				current value of the pin, but the last value written to the
 * 				register. In terms of toggle functionality, this is all we need.
 *
 *				Assert functionality: Only accept pins LED9 and PS_GP_OUT[7:0];
 * 				Assert otherwise.
 *
******************************************************************************/

void psGpOutToggle(PsGpio_OutPin_t pin){

	uint32_t pin_state;

	/* Extract GPIO base address from instance pointer */
	uint32_t base_addr = p_XGpioPsInst->GpioConfig.BaseAddr;

	/* Function should only be passed these MIO outputs: */
	Xil_AssertVoid( (pin == PS_GP_OUT0) || (pin == PS_GP_OUT1)
					|| (pin == PS_GP_OUT2) || (pin == PS_GP_OUT3)
					|| (pin == PS_GP_OUT4) || (pin == PS_GP_OUT5)
					|| (pin == PS_GP_OUT6) || (pin == PS_GP_OUT7)
					|| (pin == LED9));



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
 * 				(BTN8 and BTN9); Assert otherwise.
 *
******************************************************************************/

uint32_t psGpInRead(PsGpio_InPin_t pin){

	uint32_t pin_state;

	/* Function should only handle BTN8 (MIO50) or BTN9 (MIO51) */
	Xil_AssertNonvoid( (pin == BTN8) || (pin == BTN9) );


	pin_state = XGpioPs_ReadPin(p_XGpioPsInst, pin);

	return pin_state;
}


/****** End functions *****/

/****** End of File **********************************************************/
