/*
 * pressure.h
 *
 *  Created on: Mar 29, 2016
 *      Author: dvguille
 */

#ifndef PRESSURE_H_
#define PRESSURE_H_

#define MS5837_ADDR               0x76
#define MS5837_RESET              0x1E
#define MS5837_ADC	              0x00
#define MS5837_PROM0 	          0xA0
#define MS5837_PROM1 	          0xA2
#define MS5837_PROM2	          0xA4
#define MS5837_PROM3 	          0xA6
#define MS5837_PROM4 	          0xA8
#define MS5837_PROM5 	          0xAA
#define MS5837_PROM6 	          0xAC
#define MS5837_PROM7 	          0xAE
#define MS5837_D1_256			  0x40
#define MS5837_D1_512			  0x42
#define MS5837_D1_2048   		  0x46
#define MS5837_D1_8192   		  0x4A
#define MS5837_D2_256			  0x50
#define MS5837_D2_512			  0x52
#define MS5837_D2_2048   		  0x56
#define MS5837_D2_8192    		  0x5A


typedef struct pressure pressure;

struct pressure {
	uint16_t c[8];			// C1 pressure sensitivity
							// C2 pressure offset
							// C3 temperature coefficient of pressure sensitivity
							// C4 temperature coefficient of pressure offeset
							// C5 reference temperature
							// C6 temperature coefficient of temp
	uint32_t digital_pressure,
			 digital_temperature;

	int32_t pressure, temperature;

};

/*
 * Param - void
 * Description - Initializes the ms5837 to be able to read data from the device
 * Notes - Because the device uses I2C, I2C must be intialized before calling this function
 * 		   resets the device per datasheet and read from the prom
 * Return - none
 */
void pressure_init(volatile pressure *p);



#endif /* PRESSURE_H_ */
