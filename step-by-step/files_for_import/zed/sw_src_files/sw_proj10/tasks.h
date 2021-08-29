/******************************************************************************
 * @Title		:	Tasks
 * @Filename	:	tasks.h
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

#ifndef SRC_TASKS_H_
#define SRC_TASKS_H_

/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/
// Xilinx Files
#include "intr_sys.h"

// Interface files
#include "gpio/ps7_gpio_if.h"
#include "gpio/axi_gpio0_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/
/* Slow down LED toggle rates, when loop rate is very fast */
// #define LED1_TOGGLE_COUNT			4000U
// #define LED2_TOGGLE_COUNT			2000U


#define TASK1_SHARED_VAR_TEST 		1
#define TASK2_SHARED_VAR_TEST 		0


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/
/*=== Task logic === */
/* Tasks */
void task1(void);
void task2(void);



/* Added for sw_proj10: */
// Setter/Getter for Task 1 Shared Variable
void setTask1SharedVariable(uint32_t value);
uint32_t getTask1SharedVariable(void);

// Setter/Getter for Task 2 Shared Variable
void setTask2SharedVariable(uint32_t value);
uint32_t getTask2SharedVariable(void);


#endif /* SRC_TASKS_H_ */
