/*
 * main.c
 */
#include <stdbool.h>
#include <stdlib.h>
#include "i2c.h"
#include "i2c0.h"
#include "accelerometer.h"
#include "pressure.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"


#include <stdio.h>

volatile uint8_t data;
volatile accelerometer accel;
volatile pressure p;
  int main(void) {

	// Init Code here


	//initialize_i2c();
	//set_slave_address(MS5837_ADDR);
	//write_byte(MS5837_RESET, START|RUN|STOP);
	//SysCtlDelay(400000);
	//pressure_init(&p);
	//initialize_accelerometer ();

	InitI2C0();
	pressure_init(&p);
  	while(1){

		//SysCtlDelay(50);
  		//accelerometer_data_get (&accel);
  		//for (i=0;i<10000;++i);
  	}

}

