/*
 * main.c
 */
#include <stdbool.h>
#include <stdlib.h>
#include "i2c.h"
#include "accelerometer.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"


#include <stdio.h>

volatile uint8_t data;
volatile accelerometer accel;
  int main(void) {

	// Init Code here
	accelerometer accel;


	initialize_i2c();

	// Step 1: Write Slave Address
	set_slave_address(0x1D);

	initialize_accelerometer();

    int i;
	while(1){

		accelerometer_data_get(&accel);
		printf ("x = %d\n", accel.xg0);
		for (i=0;i<100000;i++);
	}
}

