/*
 * accelerometer.c
 *
 *  Created on: Feb 18, 2016
 *      Author: dvguille
 */

#include "accelerometer.h"
#include "i2c.h"
#include "tm4c123gh6pm.h"
#include <math.h>

/*
 * Param - accelerometer*
 * Description - gets acceleromter data on all value and puts them in the accel struct fields
 * Return - none
 */
void accelerometer_data_get (volatile accelerometer *accel) {

	set_slave_address (0xA6>>1);//(0x1D);
	write_byte(DATAX0, RUN|START);
	// Get X, Y, Z data from accelerometer
//	I2C1_MCS_R &= 0xFFFFFFE0;
	accel->xg0 = read_byte(M_ACK|START|RUN);

//	I2C1_MCS_R &= 0xFFFFFFE0;
	accel->xg1 = read_byte(M_ACK|RUN);

	accel->yg0 = read_byte(M_ACK|RUN);
	accel->yg1 = read_byte(M_ACK|RUN);

	accel->zg0 = read_byte(M_ACK|RUN);
//	I2C1_MCS_R &= 0xFFFFFFE0;
	accel->zg1 = read_byte(RUN|STOPI2C);

	accel->x = (accel->xg1<<8)|(accel->xg0);
	accel->y = (accel->yg1<<8)|(accel->yg0);
	accel->z = (accel->zg1<<8)|(accel->zg0);

}

/*
 * Param - void
 * Description - Initializes the adxl345 to be able to read data from the device
 * Notes - Because the device uses I2C, I2C must be intialized before calling this function
 * 		   DATA_FORMAT and POWER_CTL register get setup.
 * Return - none
 */
void initialize_accelerometer (void) {
	set_slave_address (0xA6>>1);//(0x1D);

	write_accelerometer(POWER_CTL, 0x08);
//	write_byte(POWER_CTL, START|RUN);
//	write_byte(0x08, RUN|STOP);

	write_accelerometer(DATA_FORMAT, 0x03);
//	write_byte(DATA_FORMAT, START|RUN);
//	write_byte(0x03, RUN|STOP);

	write_accelerometer(TAP_AXES, 0x08);
//	write_byte(TAP_AXES, RUN|START);
//	write_byte(0x08, RUN|STOP);
	write_accelerometer(OFSX, 0x01);

	write_accelerometer(OFSY, 0xFF);

	write_accelerometer(OFSZ, 0x02);

}

/*
 * Param - uint8_t register, uint8_t data
 * Description - write data to the register address of the slave
 * Notes - Does not set the slave address, must be done before calling this function
 * Return - none
 */
void write_accelerometer (uint8_t reg, uint8_t data) {
	write_byte(reg, RUN|START);
	write_byte(data, RUN|STOPI2C);
}

int16_t to_degrees(uint8_t axis, int16_t value, volatile accelerometer* accel){

	switch(axis){
		case XAXIS:
			if (accel->x < 0) {
				value = -asin((-accel->x * 31.2) / 1000) * (180 / 3.14);
				if ((-accel->x * 31.2) / 1000 > .9984)
					value = -90.0;
			} else {
				value = asin((accel->x * 31.2) / 1000) * (180 / 3.14);
				if ((accel->x * 31.2) / 1000 > 1)
					value = 90.0;
			}
			break;
		case YAXIS:
			if (accel->y < 0) {
				value = -asin((-accel->y * 31.2) / 1000) * (180 / 3.14);
				if ((-accel->y * 31.2) / 1000 > .9984)
					value = -90.0;
			} else {
				value = asin((accel->y * 31.2) / 1000) * (180 / 3.14);
				if ((accel->y * 31.2) / 1000 > 1)
					value = 90.0;
			}
			break;
		case ZAXIS:
			if (accel->z < 0) {
				value = -asin((-accel->z * 31.2) / 1000) * (180 / 3.14);
				if ((-accel->z * 31.2) / 1000 > .9984)
					value = -90.0;
			} else {
				value = asin((accel->z * 31.2) / 1000) * (180 / 3.14);
				if ((accel->z * 31.2) / 1000 > 1)
					value = 90.0;
			}
			break;
	}
		return value;
}
