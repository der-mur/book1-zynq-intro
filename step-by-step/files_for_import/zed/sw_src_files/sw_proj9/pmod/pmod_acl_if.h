/******************************************************************************
 * @Title		:	PmodACL Interface (Header File)
 * @Filename	:	pmod_acl_if.h
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

#ifndef SRC_PMOD_PMOD_ACL_IF_H_
#define SRC_PMOD_PMOD_ACL_IF_H_



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "../spi/axi_spi_if.h"

// Access to LED3 for interrupt handling:
#include "../gpio/axi_gpio0_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define PMOD_ACL_DEBUG					0



/* Pmod ACL registers */
#define DEVID_REG						0x00
#define THRESH_TAP_REG					0x1D
#define OFSX_REG						0x1E
#define OFSY_REG						0x1F
#define OFSZ_REG						0x20
#define DUR_REG							0x21
#define LATENT_REG						0x22
#define WINDOW_REG						0x23
#define THRESH_ACT_REG					0x24
#define THRESH_INACT_REG				0x25
#define TIME_INACT_REG					0x26
#define ACT_INACT_CTL_REG				0x27
#define THRESH_FF_REG					0x28
#define TIME_FF_REG						0x29
#define TAP_AXES_REG					0x2A
#define ACT_TAP_STATUS_REG				0x2B
#define BW_RATE_REG						0x2C
#define POWER_CTL_REG					0x2D
#define INT_ENABLE_REG 					0x2E
#define INT_MAP_REG						0x2F
#define INT_SOURCE_REG					0x30
#define DATA_FORMAT_REG					0x31
#define DATAX0_REG 						0x32
#define DATAX1_REG 						0x33
#define DATAY0_REG 						0x34
#define DATAY1_REG 						0x35
#define DATAZ0_REG 						0x36
#define DATAZ1_REG 						0x37
#define FIFO_CTL_REG					0x38
#define FIFO_STATUS_REG					0x39

/* Values for this project */
#define THRESH_TAP_VAL					0x0F /* 62.5mg/LSB = 937.5mg */
#define DUR_VAL							0x0F /* 625us/LSB = 9.375ms*/
#define THRESH_INACT_VAL				0x0F /* 62.5mg/LSB = 937.5mg */
#define TIME_INACT_VAL					0x0A /* 10 SECONDS */
#define ACT_INACT_CTL_VAL				0x04 /* INACT_X enable */
#define TAP_AXES_VAL					0x04 /* TAP_X enable */
#define BW_RATE_VAL						0x0F /* 3200 Hz Rate */
#define POWER_CTL_VAL					0x08 /* Enable MEASURE mode */
#define INT_ENABLE_VAL					0x48 /* SINGLE_TAP, Inactivity */
#define INT_MAP_VAL						0x08 /* Inactivity mapped to INT2 */
#define DATA_FORMAT_VAL					0x0B /* Full Resolution, 16g mode */


/*****************************************************************************/
/******************************* Typedefs ************************************/
/*****************************************************************************/


/*****************************************************************************/
/************************** Variable Declarations ****************************/
/*****************************************************************************/


/****************************************************************************/
/***************** Macros (Inline Functions) Definitions ********************/
/****************************************************************************/


/*****************************************************************************/
/************************** Function Prototypes ******************************/
/*****************************************************************************/

/* Device Initialization */
int pmodAcl_Init(void);


/* Interface functions */
uint8_t pmodAcl_ReadByte(uint8_t reg_addr);
void pmodAcl_WriteByte(uint8_t reg_addr, uint8_t write_data);

uint32_t pmodAcl_ReadXYData(void);
uint8_t pmodAcl_ReadIntrStatus(void);



/* Interrupt Handlers */
void pmodAcl_Intr1Handler(void);
void pmodAcl_Intr2Handler(void);


/****** End functions *****/

/****** End of File **********************************************************/


#endif /* SRC_PMOD_PMOD_ACL_IF_H_ */
