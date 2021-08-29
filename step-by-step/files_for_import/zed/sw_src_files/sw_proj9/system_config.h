/******************************************************************************
 * @Title		:	System Configuration (Header File)
 * @Filename	:	system_config.h
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


#ifndef SRC_SYSTEM_CONFIG_H_
#define SRC_SYSTEM_CONFIG_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

/* Xilinx files */
#include "xil_assert.h"



// Interface files
#include "gpio/ps7_gpio_if.h"
#include "gpio/axi_gpio0_if.h"
#include "wdt/scuwdt_if.h"
#include "timers/ttc0_if.h"
#include "uart/ps7_uart1_if.h"
#include "pmod/pmod_acl_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define SYS_CONFIG_DEBUG			1


/* ------------------------- RUN-TIME PARAMETERS ---------------------- */

/* Toggle rate for LED0 if initialisation fails */
#define INIT_FAIL_LOOP_DELAY		10000000U

/* Slow down LED9 toggle rate, when loop rate is very fast */
#define LED9_TOGGLE_COUNT			800U



/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/

typedef struct {
	volatile int xscu_wdt;
	volatile int xgpio0;
	volatile int xgpiops;
	volatile int xscu_gic;
	volatile int xttc0;
	volatile int uart1;
	volatile int pmod_acl;
}init_status_t;

typedef struct {
	volatile int xttc0;
	volatile int uart1;
	volatile int pmod_acl_intr1;
	volatile int pmod_acl_intr2;
}add_intr_status_t;


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

/* Assertion function */
void AssertPrint(const char8 *file, s32 line);


/* General functions */
int sys_init(void);


/* External functions from intr_sys.h */
extern int xScuGicInit(void);
extern void enableInterrupts(void);
extern void disableInterrupts(void);
extern int addTtc0ToInterruptSystem(uint32_t p_XScuTimerInst);
extern int addUart1ToInterruptSystem(u32 p_XUartPsInst);
extern int addPmodAcl_Intr1ToInterruptSystem(void);
extern int addPmodAcl_Intr2ToInterruptSystem(void);



#endif /* SRC_SYSTEM_CONFIG_H_ */
