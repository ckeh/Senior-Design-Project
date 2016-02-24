/*
 * i2c.c
 *
 *  Created on: Feb 8, 2016
 *      Author: dvguille
 */
#include "tm4c123gh6pm.h"
#include <inttypes.h>



#define CHECK_BIT(var, pos) ((var)&(1<<(pos)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																														   ///
///											PRIVATE FUNCTIONS															   ///
///																														   ///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *! Function set_rw
 *! \Param read/write set 1 to read or 0 to write
 *!
 *! This function sets the master to read or write mode.
 *!
 *! \Return no return
 *!
 */
void set_rw (uint8_t rw){

	if (rw == 0) I2C1_MSA_R &= ~(1<<0);
	else I2C1_MSA_R |= (1<<0);
}

void initialize_i2c (void) {        // Will add parameter to set the speed of the scl line.
	// 1. Enable the I2C clock using the RCGCI2C register in the System Control module (see page 348)
	SYSCTL_RCGCI2C_R |= 1<<1;
	// 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register in the System
	//	  module (see page 340). To find out which GPIO port to enable, refer to Table
	//	  23-5 on page 1351.
	// i2c1scl = pa6 = pin 23;
	SYSCTL_RCGCGPIO_R |= 1<<0;
	// 3. In the GPIO module, enable the appropriate pins for their alternate function using the
	//	  GPIOAFSEL register (see page 671). To determine which GPIOs to configure, see Table
	//    23-4 on page 1344.
	// GPIO Port A (APB) base: 0x4000.4000
	//    Must also set to digital
	/* 0x4000.400 |= (1<<7)|(1<<6);? */
	GPIO_PORTA_AFSEL_R |= (1<<7)|(1<<6);
	GPIO_PORTA_DEN_R |= (1<<7)|(1<<6);
	// 4. Enable the I2CSDA pin for open-drain operation. See page 676
	// Must be port A
	GPIO_PORTA_ODR_R |= (1<<7);
	// 5. Configure the PMCn fields in the GPIOPCTL register to assign the I2C signals to the appropriate
	//    pins. See page 688 and Table 23-5 on page 1351.
	GPIO_PORTA_PCTL_R |= (3<<28)|(3<<24); // Put a value of 3 for the bits based of table on 1351, 31:28 = PA7, 27:24 = PA6
	// 6. Initialize the I2C Master by writing the I2CMCR register with a value of 0x0000.0010
	I2C1_MCR_R |= 1<<4;
	// 7. Set the desired SCL clock speed of 100 Kbps by writing the I2CMTPR register with the correct
	//    value. The value written to the I2CMTPR register represents the number of system clock periods
	//	  in one SCL clock period. The TPR value is determined by the following equation:
	//	  TPR = (System Clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1;
	//	  TPR = (80MHz/(2*(6+4)*100000))-1; Assume 80MHz for now.
	//	  TPR = 9
	//	  Write the I2CMTPR register with the value of 0x0000.0009.
	I2C1_MTPR_R |= 6<<0;  //TPR bits are 6:0 // set to 6 for 100kbs


}

void set_slave_address (uint8_t address) {
	I2C1_MSA_R |= (address<<1);
}


// Conditions will be STOP, START, RUN 2:0 respectively
int read_byte (uint8_t conditions) {

	set_rw(1);
	// Set Conditions for i2c
	I2C1_MCS_R |= conditions;
	// Check to make sure line isnt busy.

	// Wait until i2c is idle
	while ((I2C1_MCS_R & (1<<0)) != 0); // 0 bit in I2C1_MCS_R is the BUSY bit;

	// Error checking
	if ((I2C1_MCS_R & (1<<1)) != 0) {
		if ((I2C1_MCS_R & (1<<4)) == 0) {
			//Arbitration Lost
		}
		else {
			I2C1_MCR_R = (1<<2); // STOP BIT
			while ((I2C1_MCS_R & (1<<0)) != 0);
		}
	}
	// Read byte of data


	return I2C1_MDR_R&0xFF;
}

void write_byte (uint8_t data, uint8_t conditions) {
	set_rw(0);
	// Conditions from 0-7 in decimal/hex or 3 bits in binary
	// 9. Place data (byte) to be transmitted in the data register by writing the I2CMDR register with the
	// desired data.
	I2C1_MDR_R |= data;
	// 10. Initiate a single byte transmit of the data from Master to Slave by writing the I2CMCS register
	// with a value of 0x0000.0007 (STOP, START, RUN).
	I2C1_MCS_R |= 0<<4;
	I2C1_MCS_R |= conditions; // STOP, START, RUN

	// 11. Wait until the transmission completes by polling the I2CMCS register's BUSBSY bit until it has
	// been cleared.
	while ((I2C1_MCS_R & (1<<0)) != 0);

	//12. Check the ERROR bit in the I2CMCS register to confirm the transmit was acknowledged.
	if ((I2C1_MCS_R & (1<<1)) != 0) {
		if  ((I2C1_MCS_R & (1<<4)) == 1){
			//Arbitration lost
		}
		else {
			I2C1_MCR_R = (1<<2); // STOP BIT
			while ((I2C1_MCS_R & (1<<0)) != 0);
		}
	}
}
