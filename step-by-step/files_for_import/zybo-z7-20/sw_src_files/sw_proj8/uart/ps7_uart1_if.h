/******************************************************************************
 * @Title		:	UART1 Interface  (Header File)
 * @Filename	:	ps7_uart1_if.h
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

#ifndef SRC_UART_PS7_UART1_IF_H_
#define SRC_UART_PS7_UART1_IF_H_


/***************************** Include Files ********************************/


#include "xuartps.h"

#include "../gpio/ps7_gpio_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define UART1_DEBUG						0

/* Device ID */
#define PS7_UART1_DEVICE_ID			XPAR_PS7_UART_1_DEVICE_ID


/* Uart1 Settings */
#define UART_RX_BUFFER_SIZE			10U		// 10 byte command frame from host
#define UART_TX_BUFFER_SIZE			4U		// 4 byte response frame to host


/****************************************************************************/
/************************** Function Prototypes *****************************/
/****************************************************************************/

/* Device Initialization */
/* NOTE: *p_inst is being returned, not passed to the function! */
int xUart1PsInit(uint32_t *p_inst);

/* Interrupt handler */
void UartIntrHandler(void *CallBackRef, uint32_t event, uint32_t event_data);


/* Defined in cmd_handler code */
extern void handleCommand(uint8_t *rx_buffer, uint8_t *tx_buffer);



#endif /* SRC_UART_PS7_UART1_IF_H_ */
