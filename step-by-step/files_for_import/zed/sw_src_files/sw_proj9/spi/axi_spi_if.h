/******************************************************************************
 * @Title		:	AXI SPI Interface  (Header File)
 * @Filename	:	axi_spi_if.h
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

#ifndef SRC_SPI_AXI_SPI_IF_H_
#define SRC_SPI_AXI_SPI_IF_H_



/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "xspi.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

#define AXI_SPI_DEBUG					0

/* Device ID */
#define AXI_SPI_DEVICE_ID				XPAR_AXI_QUAD_SPI_0_DEVICE_ID


/* Receive buffer size */
#define RX_NBYTES					16U



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


/* Device Initialisation */
int axi_spiInit(uint32_t options);



/* Interface functions */
uint8_t* spiReadBytes(uint8_t* tx_data, uint8_t nbytes);
void spiWriteBytes(uint8_t* tx_data, uint8_t nbytes);



/****** End functions *****/

/****** End of File **********************************************************/


#endif /* SRC_SPI_AXI_SPI_IF_H_ */
