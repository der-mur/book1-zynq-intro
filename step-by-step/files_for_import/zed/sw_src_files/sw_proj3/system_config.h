/******************************************************************************
 * @Title		:	Software Project 3: System Configuration Header
 * @Filename	:	system_config.h
 * @Author		:	Derek Murray
 * @Origin Date	:	06/11/2019
 * @Version		:	1.0.0
 * @Compiler	:	arm-none-eabi-gcc
 * @Target		: 	Xilinx Zynq-7000
 * @Platform	: 	Digilent/Avnet ZedBoard
 *
 *
 * Copyright (C) 2019 Derek Murray.  All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
******************************************************************************/


#ifndef SRC_SYSTEM_CONFIG_H_
#define SRC_SYSTEM_CONFIG_H_


/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

/* Xilinx files */
#include "xil_assert.h"

/* Interface files */
#include "gpio/ps7_gpio_if.h"
#include "gpio/axi_gpio0_if.h"





/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define SYS_CONFIG_DEBUG				1


/*------------------------- DESIGN DETAILS ---------------------------- */


/* ------------------------- RUN-TIME PARAMETERS ---------------------- */

/* Main loop time delay */
#define MAIN_LOOP_DELAY					10000000U



/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/

typedef struct {
	volatile int xgpio0;
	volatile int xgpiops;
}init_status_t;



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

/* Assertion functions */
void AssertPrint(const char8 *file, s32 line);

/* General functions */
int sys_init(void);



#endif /* SRC_SYSTEM_CONFIG_H_ */
