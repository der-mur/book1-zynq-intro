/******************************************************************************
 * @Title		:	AXI SPI Interface
 * @Filename	:	axi_spi_if.c
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





/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "axi_spi_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/

/* Declare instance and associated pointer for XIicInst */
static XSpi			XSpiInst;
static XSpi 		*p_XSpiInst = &XSpiInst;


/* SPI Receive Buffer */
static uint8_t spi_rx_buffer [RX_NBYTES];




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



/*---------------------------------------------------------------------------*/
/*------------------------------- FUNCTIONS ---------------------------------*/
/*---------------------------------------------------------------------------*/


/*****************************************************************************
 * Function: axi_spiInit()
 *//**
 *
 * @brief		Configures AXI SPI block for use.
 *
 *
 * @details		Starts by doing device look-up, configuration and self-test.
 * 				Then configures the	TTC0 Timer for this project.
 *
 * 				The initialisation steps are:
 * 				(1) DEVICE LOOK-UP => Calls function "XSpi_LookupConfig"
 * 				(2) DRIVER INIT => Calls function "XSpi_CfgInitialize"
 * 				(3) SELF TEST => Calls function "XSpi_SelfTest"
 * 				(4) SPECIFIC CONFIG => Configures AXI SPI for this project
 *
 * 				If any of the first three states results in XST_FAILURE, the
 * 				initialisation will stop and the XST_FAILURE code will be
 * 				returned to the calling code. If initialisation completes with
 * 				no failures, then XST_SUCCESS is returned.
 *
 * @return		Integer indicating result of configuration attempt.
 * 				0 = SUCCESS, 1 = FAILURE
 *
 * @note		None
 *
******************************************************************************/

int axi_spiInit(uint32_t options){

		int status;


		/* Pointer to XTtcPs_Config is required for later functions. */
		XSpi_Config *p_XSpiCfg = NULL;




	/* === START CONFIGURATION SEQUENCE ===  */

	/* ---------------------------------------------------------------------
	 * ------------ STEP 1: DEVICE LOOK-UP ------------
	 * -------------------------------------------------------------------- */
	p_XSpiCfg = XSpi_LookupConfig(AXI_SPI_DEVICE_ID);
	if (p_XSpiCfg == NULL)
	{
		status = XST_FAILURE;
		return status;
	}


 	/* ---------------------------------------------------------------------
	 * ------------ STEP 2: DRIVER INITIALISATION ------------
	 * -------------------------------------------------------------------- */
	status = XSpi_CfgInitialize(p_XSpiInst, p_XSpiCfg, p_XSpiCfg->BaseAddress);
	if (status != XST_SUCCESS)
	{
 		return status;
	}


	/* ---------------------------------------------------------------------
	* ------------ STEP 3: SELF TEST ------------
	* -------------------------------------------------------------------- */
	status = XSpi_SelfTest(p_XSpiInst);
	Xil_AssertNonvoid(status == XST_SUCCESS);

	/* If the assertion test fails, we won't get here, but
	* leave the code in anyway, for possible future changes. */
	if (status != XST_SUCCESS)
	{
		return status;
	}


	/* ---------------------------------------------------------------------
	* ------------ STEP 4: PROJECT-SPECIFIC CONFIGURATION ------------
	* -------------------------------------------------------------------- */
	/* Configuration steps are:
	 * (1) Set AXI SPI options.
	 * (2) Set the slave select mask to SS1.
	 * (3) Enable (start) the SPI block.
	 * (4) Disable global interrupts as we are using polled mode. */
	XSpi_SetOptions(p_XSpiInst, options);
	XSpi_SetSlaveSelect(p_XSpiInst, 0x01);
	XSpi_Start(p_XSpiInst);
	XSpi_IntrGlobalDisable(p_XSpiInst);


	/* === END CONFIGURATION SEQUENCE ===  */
	/* ==================================  */



	/* Return initialisation result to calling code */
	return status;

}



/*****************************************************************************
 * Function: spiReadBytes()
 *//**
 *
 * @brief		Executes SPI Transfer and returns the data in the rx buffer.
 *
 * @return		Pointer to the Register read data (*uint8_t).
 *
 * @param[in]	*tx_data: Pointer to data to put on the SPI bus
 * @param[in]	nbytes: Number of bytes to put on the bus.
 *
 * @note
 *
****************************************************************************/

uint8_t* spiReadBytes(uint8_t* tx_data, uint8_t nbytes)
{

	XSpi_Transfer(p_XSpiInst, tx_data, spi_rx_buffer, nbytes);

	return spi_rx_buffer;
}



/*****************************************************************************
 * Function: spiWriteBytes()
 *//**
 *
 * @brief		Executes SPI Transfer, with no return data collected.
 *
 * @return		None
 *
 * @param[in]	*tx_data: Pointer to data to put on the SPI bus
 * @param[in]	nbytes: Number of bytes to put on the bus.
 *
 * @note
 *
****************************************************************************/

void spiWriteBytes(uint8_t* tx_data, uint8_t nbytes)
{

	XSpi_Transfer(p_XSpiInst, tx_data, NULL, nbytes);

}


/****** End functions *****/

/****** End of File **********************************************************/

