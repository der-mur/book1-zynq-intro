/******************************************************************************
 * @Title		:	Interrupt System (Header File)
 * @Filename	:	intr_sys.h
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

#ifndef SRC_INTR_SYS_H_
#define SRC_INTR_SYS_H_



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

/* Xilinx low-level */
#include "xscugic.h"
#include "xil_exception.h"

/* Must also include any files for drivers which will be added to intr sys: */
#include "uart/ps7_uart1_if.h"
#include "timers/ttc0_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define INTR_SYS_DEBUG				1

/* Device ID */
#define PS7_SCUGIC_DEVICE_ID		XPAR_PS7_SCUGIC_0_DEVICE_ID


/* Interrupt IDs (from "xparameters_ps.h") */
#define UART1_INTR_ID				XPS_UART1_INT_ID 	// PS7 UART1, 82U
#define TTC0_INT_IRQ_ID				XPS_TTC0_0_INT_ID	// TTC0, 42U



/* === Interrupt priorities/triggers === */
/* In this project, timing is more important than UART comms,
 * so TTC0 has higher priority (In ARM, lower number = higher priority) */

/* PS7 UART1 */
#define UART1_INTR_PRI				(0xB0)
#define UART1_INTR_TRIG				(0x01) // Active-high Level Sensitive

/* TTC0 */
#define TTC0_INTR_PRI				(0xA0) // Higher priority than UART
#define TTC0_INTR_TRIG				(0x01) // Active-high Level Sensitive




/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/




/*****************************************************************************/
/************************ Macros (Inline Functions) **************************/
/*****************************************************************************/

/* Ensure xil_printf.h is included: */
#ifndef SRC_XIL_PRINTF_H
#define SRC_XIL_PRINTF_H
#endif

#define printf 		xil_printf 	/* Small foot-print printf function */



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialization */
int xScuGicInit(void);


/* Interrupt configuration */
int addTtc0ToInterruptSystem(uint32_t p_Xttc0Inst);
int addUart1ToInterruptSystem(uint32_t p_XUartPsInst);


/* Interface functions */
void enableInterrupts(void);
void disableInterrupts(void);


#endif /* SRC_INTR_SYS_H_ */
