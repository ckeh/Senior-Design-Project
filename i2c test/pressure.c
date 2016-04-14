/*
 * pressure.c
 *
 *  Created on: Mar 29, 2016
 *      Author: dvguille
 */

#include "i2c.h"
#include "i2c0.h"
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

}

void pressure_read(volatile pressure *p) {
//	I2CSend(MS5837_ADDR, MS5837_D1_8192);
//	SysCtlDelay(400000);
//	p->digital_pressure = I2CReceive(MS5837_ADDR, MS5837_ADC);
//	p->digital_pressure = (p->digital_pressure<<8) | I2CReceive(MS5837_ADDR, MS5837_ADC);
//	p->digital_pressure = (p->digital_pressure<<8) | I2CReceive(MS5837_ADDR, MS5837_ADC);
//
//	I2CSend(MS5837_ADDR, MS5837_D2_8192);
//	SysCtlDelay(400000);
//	p->digital_pressure = I2CReceive(MS5837_ADDR, MS5837_ADC);
//	p->digital_pressure = (p->digital_pressure<<8) | I2CReceive(MS5837_ADDR, MS5837_ADC);
//	p->digital_pressure = (p->digital_pressure<<8) | I2CReceive(MS5837_ADDR, MS5837_ADC);


	set_slave_address(MS5837_ADDR);
	write_byte(MS5837_D2_8192, I2C_MASTER_CMD_SINGLE_SEND);
	SysCtlDelay(900000);

//	p->p[0] = (uint8_t)read_byte (I2C_MASTER_CMD_SINGLE_RECEIVE);
//	p->p[1] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT);
//	p->p[2] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

//	set_slave_address(MS5837_ADDR);
//	p->p[0] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START);
//
//	//set_slave_address(MS5837_ADDR);
//	//write_byte (MS5837_ADC, I2C_MASTER_CMD_BURST_SEND_START);
//	p->p[1] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT);
//
//	//set_slave_address(MS5837_ADDR);
//	//write_byte (MS5837_ADC, I2C_MASTER_CMD_BURST_SEND_START);
//	p->p[2] = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

//	write_byte(MS5837_D2_8192, I2C_MASTER_CMD_SINGLE_SEND);
//	SysCtlDelay(400000);
	//set_slave_address(MS5837_ADDR);
	write_byte (MS5837_ADC, I2C_MASTER_CMD_SINGLE_SEND);
	//write_byte (MS5837_ADC, I2C_MASTER_CMD_SINGLE_SEND);
	SysCtlDelay(900000);

	//set_slave_address(MS5837_ADDR);
	p->digital_temperature = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<16 |
			(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT)<<8 |
			(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);


	write_byte(MS5837_D1_8192, I2C_MASTER_CMD_SINGLE_SEND);
	SysCtlDelay(900000);
	//set_slave_address(MS5837_ADDR);
	write_byte (MS5837_ADC, I2C_MASTER_CMD_SINGLE_SEND);
	//set_slave_address(MS5837_ADDR);
	p->digital_pressure = (uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_START)<<16 |
			(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_CONT)<<8 |
			(uint8_t)read_byte (I2C_MASTER_CMD_BURST_RECEIVE_FINISH);


}


int calculatePress(volatile pressure *p){
	int dT;
	int temp;
	int64_t off;
	int64_t sens;
	int press;


	dT = p->digital_temperature - (p->c[5] * pow(2,8));
	temp = 2000 + dT * p->c[6] / pow(2,23);
	p->temperature = temp;

	off = p->c[2] * pow(2,16) + (p->c[4] * dT) / pow(2,7);
	sens = p->c[1] * pow(2,15) + (p->c[3] * dT)/ pow(2,8);

	press = (((p->digital_pressure * sens) / pow(2,21)) - off) / pow(2,13);
	p->pressure = press;

}



