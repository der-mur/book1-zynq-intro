/******************************************************************************
 * @Title		:	Command Handler
 * @Filename	:	cmd_handler.c
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

#include "cmd_handler.h"



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/

/* Store cmd and data fields of received data in this structure */
static cmd_frame		CmdFrameInst;
static cmd_frame 		*p_cmd_frame = &CmdFrameInst;




/****************************************************************************/
/************************** Function Prototypes *****************************/
/****************************************************************************/

/* Functions internal to the command handler */
static void decodeRxData(uint8_t *rx_buffer);
static void executeCommand(uint8_t *tx_buffer);
static void setResponseBytes(uint8_t *tx_buffer, uint32_t tx_data);



/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/



/******************************************************************************
*
* Function:		handleCommand()
*
* Description:	Main function for command handling. Effectively a wrapper around
* 				the local functions decodeRxData() and executeCommand().
*
* param[in]		*rx_buffer: Pointer to the receive buffer in the comms block.
* param[in]		*tx_buffer: Pointer to the transmit buffer in the comms block.
*
* Returns:		None.
*
* Notes:		This is the interface function that is called by external code
* 				so that command handling is carried out. In this program, it is
* 				called by the ISR in 'ps7_uart1_if.c'
*
****************************************************************************/

void handleCommand(uint8_t *rx_buffer, uint8_t *tx_buffer)
{

	/* Decode the receive data and execute the command */
	decodeRxData(rx_buffer);
	executeCommand(tx_buffer);

}



/******************************************************************************
*
* Function:		decodeRxData
*
* Description:	Decodes the received data and converts it to a 'cmd_frame'
* 				structure. Updates 'CmdFrameInst' using the pointer *p_cmd_frame.
* 				The received data is formatted as follows:
*				Part 1 = bytes 0-1 = 2-byte command code
*				Part 2 = bytes 2-5 = 4-byte data field1. Example: memory address.
*				Part 3 = bytes 6-9 = 4-byte data field2. Example: memory write data.
*
*				-----------------------------------------
*				| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
*				-----------------------------------------
*				|  CMD  |    FIELD 1    |    FIELD 2    |
*				-----------------------------------------
*
*
* param[in]		*rx_buffer: Pointer to the receive buffer in the comms block.
*
* Returns:		None.
*
* Notes:
*
****************************************************************************/

void decodeRxData(uint8_t *rx_buffer){

	uint32_t idx = 0;

	/* ------ Extract command, bytes 0-1 ------- */
	for (idx = 0; idx < 2; idx++){
		p_cmd_frame->cmd = (p_cmd_frame->cmd << 8) | (rx_buffer[idx]);
	}


	/* ------- Extract field 1, bytes 2-5 ------ */
	for (idx = 2; idx < 6; idx++){
		p_cmd_frame->field1 = (p_cmd_frame->field1 << 8) | (rx_buffer[idx]);
	}


	/* -------  Extract field 2, bytes 6-9 ----- */
	for (idx = 6; idx < 10; idx++){
		p_cmd_frame->field2 = (p_cmd_frame->field2 << 8) | (rx_buffer[idx]);
	}

}



/******************************************************************************
*
* Function:		executeCommand()
*
* Description:	Executes the received command and directly updates the comms
* 				block transmit buffer with the response data.
*
* param[in]		*tx_buffer: Pointer to the transmit buffer in the comms block.
*
* Returns:		None.
*
* Notes:		None.
*
****************************************************************************/


void executeCommand(uint8_t *tx_buffer) {

	/* Extract fields from the cmd_frame structure */
	uint16_t cmd = p_cmd_frame->cmd;
	uint32_t field1 = p_cmd_frame->field1;
	uint32_t field2 = p_cmd_frame->field2;

	/* Variable to store memory read data */
	uint32_t mem_read_data;


	/* ----- Switch-Case to handle the packet ----- */

	switch(cmd) {

	// --------------------------------------------------------------------------------- //
	// WRITE_WORD: 32-bit write to memory location
	// Field 1 = address ; Field 2 = Data
	// --------------------------------------------------------------------------------- //
	case WRITE_WORD:
		/* Write the data and update the response buffer */
		Xil_Out32(field1, field2);
		setResponseBytes(tx_buffer, WRITE_OKAY);
		break;


	// --------------------------------------------------------------------------------- //
	// CMD = 0x00D4: 32-bit read from memory location
	// Field 1 = address
	// --------------------------------------------------------------------------------- //
	case READ_WORD:
		/* Read the data and update the response buffer */
		mem_read_data = Xil_In32(field1);
		setResponseBytes(tx_buffer, mem_read_data);
		break;


	// --------------------------------------------------------------------------------- //
	// CMD = 0x00F0: Used in shared variable test to clear LED1 and LED2.
	// Field 1 and Field 2 are empty
	// --------------------------------------------------------------------------------- //
	case CLEAR_LEDS:
		/* Clear the LEDs */
		axiGpOutClear(LED1);
		axiGpOutClear(LED2);
		/* Update the response buffer */
		setResponseBytes(tx_buffer, CLEAR_LEDS_RESP);
		break;


	// --------------------------------------------------------------------------------- //
	// Handle unknown commands
	// --------------------------------------------------------------------------------- //
	default:
		/* Update the response buffer with error code */
		setResponseBytes(tx_buffer, CMD_ERROR);
		break;
	}

}




/******************************************************************************
*
* Function:		setResponseBytes
*
* Description:	Updates the 4-byte Tx Buffer of the communications block with
* 				the data to be transmitted back to the host.
*
* Returns:		None.
*
* Notes:		None.
*
****************************************************************************/

void setResponseBytes(uint8_t* tx_buffer, uint32_t tx_data)
{
	*tx_buffer = (tx_data >> 24) & 0xFF;
	*(tx_buffer + 1) = (tx_data >> 16) & 0xFF;
	*(tx_buffer + 2) = (tx_data >> 8) & 0xFF;
	*(tx_buffer + 3) = tx_data & 0xFF;
}



/****** End functions *****/

/****** End of File **********************************************************/
