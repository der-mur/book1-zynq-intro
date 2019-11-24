/******************************************************************************
 * @Title		:	Command Handler (Header File)
 * @Filename	:	cmd_handler.h
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


#ifndef SRC_CMD_HANDLER_H_
#define SRC_CMD_HANDLER_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

/* Xilinx files */
#include "xil_types.h"
#include "xil_io.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define RESPONSE_NBYTES		4U
#define WRITE_OKAY			(0x01010101U)
#define CMD_ERROR			(0xEEAA5577U)



/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/

/* -------- Command frame structure -------*/
/*	-----------------------------------------
*	| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
*	-----------------------------------------
*	|  CMD  |    FIELD 1    |    FIELD 2    |
*	-----------------------------------------*/

typedef struct {
	volatile uint16_t cmd;
	volatile uint32_t field1;
	volatile uint32_t field2;
} cmd_frame;



/* -------- Commands -------- */
typedef enum
{
	WRITE_WORD = 0x00D3,
	READ_WORD = 0x00D4
}commands;



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/


/****************************************************************************/
/***************** Macros (Inline Functions) Definitions ********************/
/****************************************************************************/


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Main function to be used by comms block ISR */
void handleCommand(uint8_t *rx_buffer, uint8_t *tx_buffer);


#endif /* SRC_CMD_HANDLER_H_ */
