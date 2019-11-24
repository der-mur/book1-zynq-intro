/******************************************************************************
 * @Title		:	Zynq Fundamentals Software Project 3
 * @Filename	:	sw_proj3_main.c
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
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

#include "sw_proj3_main.h"


/*****************************/

int main(void)
{

	// ********************************************************************************* //
	// *****  INITIALIZATION PHASE *****
	// ********************************************************************************* //

#if MAIN_DEBUG
	printf("\n\r---------------------------------------------------------------\n\r");
	printf("------------ Zynq Fundamentals Software Project 3 -------------\n\r");
	printf("---------------------------------------------------------------\n\r");
	printf("Title: Structured Programming in C.\r\n");
	printf("Architecture: Infinite polling loop with for-loop timing delay.\n\r\n\r");
#endif

	/* Variable for initialisation status */
	int init_status;

	/* Run initialisation */
	init_status = sys_init();

#if MAIN_DEBUG
	if (init_status != XST_SUCCESS) {
		printf("Device initialization failed.\n\r");
	}
	else{
		printf("Device initialization succeeded.\n\r");
	}
#endif

	/* End program if initialisation was not successful */
	if (init_status != XST_SUCCESS) {
		return 0;
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

/****** End main()  *****/

/****** End of File **********************************************************/



