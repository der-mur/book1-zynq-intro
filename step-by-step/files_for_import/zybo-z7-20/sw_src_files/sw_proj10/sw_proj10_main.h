/******************************************************************************
 * @Title		:	Zynq Fundamentals Software Project 10
 * @Filename	:	sw_proj10_main.h
 * @Author		:	Derek Murray
 * @Origin Date	:	29/10/2020
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent Zybo-Z7-20
 *
 *------------------------------------------------------------------------
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

#ifndef SRC_SW_PROJ10_MAIN_H_
#define SRC_SW_PROJ10_MAIN_H_



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

/* Main system files */
#include <stdio.h>

/* Xilinx files */
#include "xil_printf.h"
#include "xparameters.h"

/* User files */
#include "system_config.h"
#include "intr_sys.h"
#include "tasks.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define MAIN_DEBUG		1


/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/



/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/


/****************************************************************************/
/***************** Macros (Inline Functions) Definitions ********************/
/****************************************************************************/

#define printf 		xil_printf 	/* Small foot-print printf function */


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/



#endif /* SRC_SW_PROJ10_MAIN_H_ */
