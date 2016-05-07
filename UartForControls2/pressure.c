/*
 * pressure.c
 *
 *  Created on: Mar 29, 2016
 *      Author: dvguille
 */

#include "i2c.h"
#include "pressure.h"

#include <stdbool.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include <math.h>
/*
 * Param - void
 * Description - Initializes the ms5837 to be able to read data from the device
 * Notes - Because the device uses I2C, I2C must be intialized before calling this function
 * 		   resets the device per datasheet and read from the prom
 * Return - none
 */
void pressure_init(volatile pressure *p) {

	//I2CSend(MS5837_ADDR, MS5837_RESET);
	set_slave_address(MS5837_ADDR);
	write_byte(MS5837_RESET, I2C_MASTER_CMD_SINGLE_SEND);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM0, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[0] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM1, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[1] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM2, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[2] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM3, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[3] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM4, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[4] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM5, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[5] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM6, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[6] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

	//set_slave_address(MS5837_ADDR);
	write_byte(MS5837_PROM6, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->c[7] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<8 | (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);


}

void pressure_read(volatile pressure *p) {
	p->digital_pressure = 0;
	p->digital_temperature = 0;

	set_slave_address(MS5837_ADDR);

	write_byte(MS5837_D1_8192, I2C_MASTER_CMD_SINGLE_SEND);
	SysCtlDelay(900000);
	write_byte (MS5837_ADC, I2C_MASTER_CMD_SINGLE_SEND);
	p->digital_pressure = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START);
	p->digital_pressure = (p->digital_pressure<<8)|(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT);
	p->digital_pressure = (p->digital_pressure<<8)|(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);



	write_byte(MS5837_D2_8192, I2C_MASTER_CMD_SINGLE_SEND);
	SysCtlDelay(900000);

	write_byte (MS5837_ADC, I2C_MASTER_CMD_SINGLE_SEND);

	p->digital_temperature = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START);
	p->digital_temperature = (p->digital_temperature<<8)|(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT);
	p->digital_temperature = (p->digital_temperature<<8)|(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);


}


void calculatePress(volatile pressure *p) {
//	int64_t dT;
//	int temp;
//
//	int64_t off;
//	int64_t sens;
//	int press;
//
//	int32_t sensi;
//	int32_t offi;
//	int32_t ti;
//	int64_t off2;
//	int64_t sens2;
//
//	dT = p->digital_temperature - (p->c[5] * pow(2,8));
//	temp = 2000 + dT * p->c[6] / pow(2,23);
//	//p->temperature = temp;
//
//	off = p->c[2] * pow(2,16) + (p->c[4] * dT) / pow(2,7);
//	sens = p->c[1] * pow(2,15) + (p->c[3] * dT)/ pow(2,8);
//
//	press = (((p->digital_pressure * sens) / pow(2,21)) - off) / pow(2,13);
//	//p->pressure = press;
//
//	if((temp/100)<20){         //Low temp
//		ti = (3*(dT)*(dT))/(8589934592ll);
//		offi = (3*(temp-2000)*(temp-2000))/2;
//		sensi = (5*(temp-2000)*(temp-2000))/8;
//		if((temp/100)<-15){    //Very low temp
//			offi = offi+7*(temp+1500)*(temp+1500);
//			sensi = sensi+4*(temp+1500)*(temp+1500);
//		}
//	}
//	else if((temp/100)>=20){    //High temp
//		ti = 2*(dT*dT)/(137438953472);
//		offi = (1*(temp-2000)*(temp-2000))/16;
//		sensi = 0;
//	}
//	off2 = off-offi;
//	sens2 = sens = sensi;
//
//	temp = temp - ti;
//	press = (((p->digital_pressure * sens2) / pow(2,21)) - off2) / pow(2,13);
//
//	p->temperature = temp;
//	p->pressure = press;
//
	uint32_t D1 = p->digital_pressure;
	uint32_t D2 = p->digital_temperature;
	int32_t dT;
	int64_t SENS;
	int64_t OFF;
	int32_t SENSi;
	int32_t OFFi;
	int32_t Ti;
	int64_t OFF2;
	int64_t SENS2;
	int32_t TEMP;
	int32_t P;

	// Terms called
	dT = D2 - ((uint32_t) p->c[5] * 256l);

	SENS = (int64_t) (p->c[1]) * 32768l + ((int64_t) (p->c[3]) * dT) / 256l;
	OFF = (int64_t) (p->c[2]) * 65536l + ((int64_t) (p->c[4]) * dT) / 128l;

	//Temp and P conversion
	TEMP = 2000l + ((int64_t) dT * p->c[6]) / 8388608LL;
	P = (D1 * SENS / (2097152l) - OFF) / (8192l);

	//Second order compensation
	if ((TEMP / 100) < 20) {         //Low temp
		Ti = (3 * (int64_t) (dT) * (int64_t) (dT)) / (8589934592LL);
		OFFi = (3 * (TEMP - 2000) * (TEMP - 2000)) / 2;
		SENSi = (5 * (TEMP - 2000) * (TEMP - 2000)) / 8;
		if ((TEMP / 100) < -15) {    //Very low temp
			OFFi = OFFi + 7 * (TEMP + 1500l) * (TEMP + 1500l);
			SENSi = SENSi + 4 * (TEMP + 1500l) * (TEMP + 1500l);
		}
	} else if ((TEMP / 100) >= 20) {    //High temp
		Ti = 2 * (dT * dT) / (137438953472LL);
		OFFi = (1 * (TEMP - 2000) * (TEMP - 2000)) / 16;
		SENSi = 0;
	}

	OFF2 = OFF - OFFi;           //Calculate pressure and temp second order
	SENS2 = SENS - SENSi;

	TEMP = (TEMP - Ti);
	P = (((D1 * SENS2) / 2097152l - OFF2) / 8192l);

	p->temperature = TEMP;
	p->pressure = P;
}

unsigned char crc4(volatile uint16_t n_prom[8])	// n_prom defined as 8x unsigned int (n_prom[8])
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



