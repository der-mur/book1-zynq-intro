/******************************************************************************
 * @Title		:	UART1 Interface
 * @Filename	:	ps7_uart1_if.c
 * @Author		:	Derek Murray
 * @Origin Date	:	15/05/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
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

#include "ps7_uart1_if.h"


/************************** Variable Definitions ****************************/

/* Declare instance and associated pointer for UART1 */
static XUartPs		XUart1PsInst;
static XUartPs		*p_XUart1PsInst = &XUart1PsInst;



/* === Buffers === */
/* Uart Buffer for receiving data from host */
static uint8_t RxBuffer [UART_RX_BUFFER_SIZE] = {0};
/* Uart Buffer for sending data to host */
static uint8_t TxBuffer [UART_TX_BUFFER_SIZE] = {0};




/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


/*****************************************************************************
 * Function: xUart1PsInit()
 *//**
 *
 * @brief		Configures UART1 for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	TTC0 Timer for this project.
 *
 * 				The initialisation steps are:
 * 				(1) DEVICE LOOK-UP => Calls function "XUartPs_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XUartPs_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XUartPs_SelfTest"
 * 				(4) SPECIFIC CONFIG => Configures UART1 for this project
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

int xUart1PsInit(uint32_t *p_inst) {

	int status;

	/* Pointer to XUartPs_Config is required some functions below. */
	XUartPs_Config *p_XUart1PsCfg = NULL;



	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XUart1PsCfg = XUartPs_LookupConfig(PS7_UART1_DEVICE_ID);
 	if (p_XUart1PsCfg == NULL)
	{
 		status = XST_FAILURE;
 		return status;
	}


 	/* ---------------------------------------------------------------------
	 * ------------ STEP 2: DRIVER INITIALISATION ------------
	 * -------------------------------------------------------------------- */
	status = XUartPs_CfgInitialize(p_XUart1PsInst, p_XUart1PsCfg, p_XUart1PsCfg->BaseAddress);
	if (status != XST_SUCCESS)
	{
 		return status;
	}


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
 	status = XUartPs_SelfTest(p_XUart1PsInst);
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
	 * (1) Set the interrupt handler.
	 * (2) Enable desired interrupts.
	 * (3) Set FIFO threshold.
	 * (4) Configure the UART in Normal Mode.*/

	XUartPs_SetHandler(p_XUart1PsInst, (XUartPs_Handler)UartIntrHandler, p_XUart1PsInst);
	XUartPs_SetInterruptMask(p_XUart1PsInst, XUARTPS_IXR_RXOVR);
	XUartPs_SetFifoThreshold(p_XUart1PsInst, UART_RX_BUFFER_SIZE);
	XUartPs_SetOperMode(p_XUart1PsInst, XUARTPS_OPER_MODE_NORMAL);


	/* === END CONFIGURATION SEQUENCE ===  */
	/* ==================================  */



	/* Update the pointer in the calling code */
	*p_inst = (uint32_t) p_XUart1PsInst;

	/* Return initialisation result to calling code */
	return status;

}



/*****************************************************************************
 * Function: UartIntrHandler()
 *//**
 *
 * @brief		Interrupt handler for PS7 UART1.
 *
 * @details		Interrupt handler for PS7 Uart1, which is used in this project
 * 				 as the communications block in conjunction with the command
 * 				 handler code. The ISR is called when a defined event occurs,
 * 				 such as data being received or transmitted, or an error being
 * 				 detected. The Rx and Tx events occur in pairs as follows:
 *
 * 				 1. RECV EVENT:
 * 				 a. A RECV EVENT occurs when the 10-byte command frame is
 * 				 received from the host PC.
 * 				 b. The event and event data are stored to memory for test purposes.
 * 				 c. XUartPs_Recv() is called to transfer the received bytes from
 * 				 the UART HW buffer to the software RxBuffer.
 * 				 d. The function handleCommand() is called to execute the command.
 * 				 e. XUartPs_Send() is called to send the response back to the host PC.
 * 				 f. For debug purposes, an assertion is triggered if the number of
 * 				 bytes sent back is not equal to 4.
 * 				 g. Otherwise the ISR exits.
 *
 * 				 2. SEND EVENT:
 * 				 a. When the response is sent back to the host PC, the ISR is called
 * 				 again.
 * 				 b. The event and event data are stored to memory for test purposes.
 * 				 c. The ISR exits.
 *
 * 				 The ISR can also be called for an unexpected event such as an error
 * 				 or buffer overflow. However, in this simple program the UART has not
 * 				 been specifically configured to allow such events to occur, so if
 * 				 such an event occurs, the code asserts.
 *
 *
 * @note		Modifications for sw_proj10:
 * 				1. Nested interrupts supported to allow higher-priority task
 * 				to interrupt this handler.
 * 				2. Shared variables in system_config.c are modified for shared
 * 				variable test.
 *
****************************************************************************/

void UartIntrHandler(void *CallBackRef, uint32_t event, uint32_t event_data)
 {

	/* === ENABLE NESTED INTERRUPTS. === */
	/* Normally, we should clear any active interrupts for this handler before
	 * enabling nested interrupts. However, clearing interrupts is handled by
	 * the lower-level Xilinx UART code, so we do not have any control over that
	 * process. The code appears to operate with no issues, even though technically
	 * the Rx interrupt is still active when nested interrupts are enabled. */

	Xil_EnableNestedInterrupts();



	// --------------------------------------------------------------------------------- //
	// event == XUARTPS_EVENT_RECV_DATA
	// 10 bytes should now have been received from the host.
	// --------------------------------------------------------------------------------- //
	if (event == XUARTPS_EVENT_RECV_DATA)
	{

		psGpOutSet(PS_GP_OUT6);	/// TEST SIGNAL: SET UART RX INTR


#if UART1_DEBUG			
		/* Store the event type and data to memory */
		Xil_Out32( 0x02000000, event);
		Xil_Out32( 0x02000004, event_data);
#endif

		/* === RX FROM HOST === */
		/* Get the data received from the host */
		XUartPs_Recv(p_XUart1PsInst, RxBuffer, UART_RX_BUFFER_SIZE);

		/* Call function to handle the data */
		handleCommand(RxBuffer, TxBuffer);

		/* === TX TO HOST === */
		/* Send the response data to the host.
		 * Note that XUartPs_Send() will enable some TX interrupts. */
		uint32_t n_bytes_sent = 0;
		n_bytes_sent = XUartPs_Send(p_XUart1PsInst, TxBuffer, UART_TX_BUFFER_SIZE);

		/* Assert if number of sent bytes is incorrect. */
		Xil_AssertVoid(n_bytes_sent == UART_TX_BUFFER_SIZE);



		/* Added in sw_proj10 to 'trample on' the shared variables in
		 * the system_config tasks. Used for the shared variable test. */
	    setTask1SharedVariable(0x12345678);
	    setTask2SharedVariable(0x12345678);



		psGpOutClear(PS_GP_OUT6); /// TEST SIGNAL: CLEAR UART RX INTR

	}

	// --------------------------------------------------------------------------------- //
	// event == XUARTPS_EVENT_SENT_DATA
	// This event should occur when data is sent back to the host.
	// --------------------------------------------------------------------------------- //
	else if (event == XUARTPS_EVENT_SENT_DATA)
	{

		psGpOutSet(PS_GP_OUT7);		/// TEST SIGNAL: SET UART TX INTR

#if UART1_DEBUG	
		/* Store the event type and data to memory */
		Xil_Out32( 0x02000008, event);
		Xil_Out32( 0x0200000C, event_data);
#endif

		psGpOutClear(PS_GP_OUT7);	/// TEST SIGNAL: CLEAR UART TX INTR

	}



	// --------------------------------------------------------------------------------- //
	// Assert for any other event.
	// --------------------------------------------------------------------------------- //
	else if ( (event == XUARTPS_EVENT_RECV_TOUT)
			| (event == XUARTPS_EVENT_RECV_ERROR)
			| (event == XUARTPS_EVENT_MODEM)
			| (event == XUARTPS_EVENT_PARE_FRAME_BRKE)
			| (event == XUARTPS_EVENT_RECV_ORERR) )
	{
		Xil_AssertVoid(0U);
	}



	/* === DISABLE NESTED INTERRUPTS! === */
	/* Disable nested interrupts when leaving the handler. */

	Xil_DisableNestedInterrupts();

}


/****** End functions *****/

/****** End of File **********************************************************/

