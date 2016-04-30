/*
 * accelerometer.h
 *
 *  Created on: Feb 18, 2016
 *      Author: dvguille
 */

#ifndef UARTFORCONTROLS2_ACCELEROMETER_H_
#define UARTFORCONTROLS2_ACCELEROMETER_H_

#include <inttypes.h>

#define XAXIS 1
#define YAXIS 2
#define ZAXIS 3

typedef struct accelerometer accelerometer;

struct accelerometer {
	int8_t xg0, xg1;
	int8_t yg0, yg1;
	int8_t zg0, zg1;

	// Combined data from each of the fields above.
	int16_t x, y, z;
};

/*
 * Param - accelerometer*
 * Description - gets acceleromter data on all axis and puts them in the accel struct fields
 * Return - none
 */
void accelerometer_data_get (volatile accelerometer *accel);

/*
 * Param - void
 * Description - Initializes the adxl345 to be able to read data from the device
 * Notes - Because the device uses I2C, I2C must be intialized before calling this function
 * 		   DATA_FORMAT and POWER_CTL register get setup. Will set the slave addr.
 * Return - none
 */
void initialize_accelerometer (void);

/*
 * Param - uint8_t register, uint8_t data
 * Description - write data to the register address of the slave
 * Notes - Does not set the slave address, must be done before calling this function
 * Return - none
 */
void write_accelerometer (uint8_t reg, uint8_t data);

/*
 * Param -
 * Description -
 * Return - none
 */
int16_t to_degrees(uint8_t axis, int16_t value, volatile accelerometer* accel);

#endif /* UARTFORCONTROLS2_ACCELEROMETER_H_ */
