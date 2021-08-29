/******************************************************************************
 * @Title		:	PmodACL Interface
 * @Filename	:	pmod_acl_if.c
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





/*****************************************************************************/
/***************************** Include Files *********************************/
/*****************************************************************************/

#include "pmod_acl_if.h"


/*****************************************************************************/
/************************** Constant Definitions *****************************/
/*****************************************************************************/



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
 * Function: pmodAcl_Init()
 *//**
 *
 * @brief		Initialises the PmodACL accelerometer.
 *
 *
 * @details		Calls the axi_spiInit() function to initialise the
 * 				AXI SPI	block. If the AXI SPI initialisation is successful, then
 * 				configures the PmodACL with the desired values for this project.
 *
 * @return		Integer indicating result of configuration attempt.
 * 				0 = SUCCESS, 1 = FAILURE
 * @note
 *
****************************************************************************/

int pmodAcl_Init()
{

	/* Set SPI options for this PmodACL.
	 * Parameters can be found in xspi.h. */
	uint32_t options = XSP_CLK_ACTIVE_LOW_OPTION
						| XSP_MANUAL_SSELECT_OPTION
						| XSP_CLK_PHASE_1_OPTION
						| XSP_MASTER_OPTION;



	/* Initialise the AXI SPI block */
	int status;
	status = axi_spiInit(options);


	/* If AXI SPI initialisation is successful, configure
	 * the PmodACL with desired values for this project. */

	if (status == XST_SUCCESS)
	{
		pmodAcl_WriteByte(THRESH_TAP_REG, THRESH_TAP_VAL);
		pmodAcl_WriteByte(DUR_REG, DUR_VAL);
		pmodAcl_WriteByte(THRESH_INACT_REG, THRESH_INACT_VAL);
		pmodAcl_WriteByte(TIME_INACT_REG, TIME_INACT_VAL);
		pmodAcl_WriteByte(ACT_INACT_CTL_REG, ACT_INACT_CTL_VAL);
		pmodAcl_WriteByte(TAP_AXES_REG, TAP_AXES_VAL);
		pmodAcl_WriteByte(BW_RATE_REG, BW_RATE_VAL);
		pmodAcl_WriteByte(POWER_CTL_REG, POWER_CTL_VAL);
		pmodAcl_WriteByte(INT_ENABLE_REG, INT_ENABLE_VAL);
		pmodAcl_WriteByte(INT_MAP_REG, INT_MAP_VAL);
		pmodAcl_WriteByte(DATA_FORMAT_REG, DATA_FORMAT_VAL);
	}
	// else just end

	return status;

}



/*****************************************************************************
 * Function: pmodAcl_ReadByte()
 *//**
 *
 * @brief		Reads a single register of the PmodACL.
 *
 * @return		Register read data, single byte (uint8_t).
 *
 * @param[in]	PmodACL register address to read from.
 *
 * @note
 *
****************************************************************************/

uint8_t pmodAcl_ReadByte(uint8_t reg_addr)
{

	uint8_t nbytes = 2U;
	uint8_t *read_data;

	/* Data to put on SPI bus */
	uint8_t tx_data[2] = {(reg_addr |= 0x80), 0U};

	/* Set nbytes = 2U; one byte for addr, and one for return data */
	read_data =  spiReadBytes(tx_data, nbytes);

	/* Byte 1 is the read data */
	return read_data[1];
}



/*****************************************************************************
 * Function: pmodAcl_WriteByte()
 *//**
 *
 * @brief		Writes to a single register of the PmodACL.
 *
 *
 * @return		Register read data, single byte (uint8_t).
 *
 * @note
 *
****************************************************************************/

void pmodAcl_WriteByte(uint8_t reg_addr, uint8_t write_data)
{

	uint8_t nbytes = 2U;

	/* Data to put on SPI bus */
	uint8_t tx_data[2] = {reg_addr, write_data};

	/* Call AXI SPI I/F function */
	spiWriteBytes(tx_data, nbytes);

}



/*****************************************************************************
 * Function: pmodAcl_ReadXYData()
 *//**
 *
 * @brief		Reads the Pmod ACL X-Y data registers.
 *
 *
 * @return		32-bit data: Lower 16-bits = X-data = [X1] [X0]
 * 				Upper 16-bits = Y-data = [Y1] [Y0], i.e.
 * 				Y1 = [31:24], Y0 = [23:16], X1 = [15:8], X0 = [7:0]
 *
 * @note
 *
****************************************************************************/

uint32_t pmodAcl_ReadXYData(void)
{

	uint8_t start_addr;

	/* Set the SPI 'Read' and 'multi-byte' bits with the register value. */
	start_addr = DATAX0_REG | 0xC0;

	/* Data to put on SPI bus */
	uint8_t tx_data[5] = {start_addr, 0U, 0U, 0U, 0U};

	/* Pointer to the return data from SPI block */
	uint8_t *xy_data_bytes;

	/* Set nbytes = 5U; byte 0 = start_addr, then 4 bytes to read back. */
	xy_data_bytes =  spiReadBytes(tx_data, 5U);




	/* Format the return data into a 32-bit word:
	 * X0 = byte [1] = [7:0];
	 * X1 = byte [2] = [15:8];
	 * Y0 = byte [3] = [23:16];
	 * Y1 = byte [4] = [31:24]; 	 */
	uint32_t xy_data;
	xy_data = (uint32_t) xy_data_bytes[1]			/* X0 */
			| (uint32_t) (xy_data_bytes[2] << 8)	/* X1 */
			| (uint32_t) (xy_data_bytes[3] << 16)	/* Y0 */
			| (uint32_t) (xy_data_bytes[4] << 24);	/* Y1 */

	return xy_data;

}



/*****************************************************************************
 * Function: pmodAcl_ReadIntrStatus()
 *//**
 *
 * @brief		Reads the Pmod ACL X-Y Interrupt Status register.
 *
 * @details		Reads the Pmod ACL X-Y Interrupt Status register 0x30. Reading
 * 				this register clears any triggered interrupt(s). In this
 * 				project, LED3 is always set when INT1 or INT2 fires, and so
 * 				we also clear LED3 after reading (clearing) the interrupt
 * 				register.
 *
 *
 * @return 		Interrupts status register value, single byte (uint8_t).
 *
 * @note
 *
****************************************************************************/

uint8_t pmodAcl_ReadIntrStatus(void)
{
	uint8_t intr_status;

	intr_status = pmodAcl_ReadByte(INT_SOURCE_REG);
	axiGpOutClear(LED3);

	return intr_status;

}



/*****************************************************************************
 * Function: pmodAcl_Intr1Handler()
 *//**
 *
 * @brief		Interrupt handler for PmodACL INT1.
 *
 * @details		Handles the PmodACL interrupt 1 event. All we do is set LED3
 * 				on the board. When the interrupt status is read using function
 * 				pmodAcl_ReadIntrStatus(), the LED will be cleared.
 *
 * @return 		None
 *
 * @note		None
 *
****************************************************************************/

void pmodAcl_Intr1Handler(void)
{
	axiGpOutSet(LED3);
}



/*****************************************************************************
 * Function: pmodAcl_Intr2Handler()
 *//**
 *
 * @brief		Interrupt handler for PmodACL INT2.
 *
 * @details		Handles the PmodACL interrupt 2 event. All we do is set LED3
 * 				on the board. When the interrupt status is read using function
 * 				pmodAcl_ReadIntrStatus(), the LED will be cleared.
 *
 * @return 		None
 *
 * @note		None
 *
****************************************************************************/

void pmodAcl_Intr2Handler(void)
{
	axiGpOutSet(LED3);
}




/****** End functions *****/

/****** End of File **********************************************************/

