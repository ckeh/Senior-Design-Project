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

unsigned char crc4(volatile unsigned int n_prom[8])	// n_prom defined as 8x unsigned int (n_prom[8])
{
	int cnt; 								// simple counter
	unsigned int n_rem = 0; 				// crc remainder
	unsigned char n_bit;

	n_prom[0] = ((n_prom[0]) & 0x0FFF);		// CRC byte is replaced by 0
	n_prom[7] = 0; 							// Subsidiary value, set to 0
	for (cnt = 0; cnt < 16; cnt++) 			// operation is performed on bytes
			{ 								// choose LSB or MSB
		if (cnt % 2 == 1)
			n_rem ^= (unsigned short) ((n_prom[cnt >> 1]) & 0x00FF);
		else
			n_rem ^= (unsigned short) (n_prom[cnt >> 1] >> 8);
		for (n_bit = 8; n_bit > 0; n_bit--) {
			if (n_rem & (0x8000))
				n_rem = (n_rem << 1) ^ 0x3000;
			else
				n_rem = (n_rem << 1);
		}
	}
	n_rem = ((n_rem >> 12) & 0x000F); // final 4-bit remainder is CRC code
	return (n_rem ^ 0x00);
}


int main(void) {
	unsigned char check;
	unsigned int prom[8];
	// Init Code here
	int i;


	initialize_i2c();
	//initialize_accelerometer ();
	pressure_init(&p);

	check = p.c[0] >> 12;

	if(check != crc4(p.c)) exit(1);

  	while(1){
  		pressure_read(&p);
  		calculatePress(&p);

  		//pressure_read(&p);
		//SysCtlDelay(40000);
  		//accelerometer_data_get (&accel);
  		//for (i=0;i<10000;++i);
  	}

}

