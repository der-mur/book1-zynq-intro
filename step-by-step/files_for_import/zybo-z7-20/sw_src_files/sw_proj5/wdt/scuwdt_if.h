/******************************************************************************
 * @Title		:	SCUWDT Interface (Header File)
 * @Filename	:	scuwdt_if.h
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

#ifndef SRC_WDT_SCUWDT_IF_H_
#define SRC_WDT_SCUWDT_IF_H_



/***************************** Include Files ********************************/

#include "xscuwdt.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define SCUWDT_DEBUG						1

/* Device ID */
#define SCUWDT_DEVICE_ID			XPAR_PS7_SCUWDT_0_DEVICE_ID

/* SCU Watchdog Timeout = 6.44 seconds @ 111 MHz */
#define SCUWDT_LOAD_VALUE			(0x7FFFFFFF)




/* System Level Control Register Reboot Status values
 * Used to indicate reason for most recent reboot. */

#define SLCR_REBOOT_STATUS_REG 				0xF8000258U

#define SLCR_RS_BOOTROM_ERR_CODE_MASK	 	0x0000FFFFU
#define SLCR_RS_SWDT_RST_MASK				0x00010000U
#define SLCR_RS_AWDT0_RST_MASK				0x00020000U
#define SLCR_RS_AWDT1_RST_MASK				0x00040000U
#define SLCR_RS_SLC_RST_MASK				0x00080000U
#define SLCR_RS_DBG_RST_MASK				0x00100000U
#define SLCR_RS_SRST_B_MASK					0x00200000U
#define SLCR_RS_POR_MASK					0x00400000U
#define SLCR_RS_RESV_MASK					0x00800000U
#define SLCR_RS_REBOOT_STATE_MASK			0xFF000000U


/*****************************************************************************/
/************************ Macros (Inline Functions) **************************/
/*****************************************************************************/

#define printf 		xil_printf 	/* Small foot-print printf function */



/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialization */
int xScuWdtInit(void);

/* Interface functions */
void restartScuWdt(void);

/* Debug function */
void checkRebootStatus(void);


#endif /* SRC_WDT_SCUWDT_IF_H_ */
