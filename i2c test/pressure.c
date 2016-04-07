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
#include "driverlib/sysctl.h"

/*
 * Param - void
 * Description - Initializes the ms5837 to be able to read data from the device
 * Notes - Because the device uses I2C, I2C must be intialized before calling this function
 * 		   resets the device per datasheet and read from the prom
 * Return - none
 */
void pressure_init(volatile pressure *p) {

	I2CSend(MS5837_ADDR, MS5837_RESET);
	SysCtlDelay(400000);

	p->c[0] = (I2CReceive(MS5837_ADDR, MS5837_PROM0)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM0);
	p->c[1] = (I2CReceive(MS5837_ADDR, MS5837_PROM1)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM1);
	p->c[2] = (I2CReceive(MS5837_ADDR, MS5837_PROM2)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM2);
	p->c[3] = (I2CReceive(MS5837_ADDR, MS5837_PROM3)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM3);
	p->c[4] = (I2CReceive(MS5837_ADDR, MS5837_PROM4)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM4);
	p->c[5] = (I2CReceive(MS5837_ADDR, MS5837_PROM5)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM5);
	p->c[6] = (I2CReceive(MS5837_ADDR, MS5837_PROM6)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM6);
	p->c[7] = (I2CReceive(MS5837_ADDR, MS5837_PROM7)<<8)|I2CReceive(MS5837_ADDR, MS5837_PROM7);



//	set_slave_address(MS5837_ADDR);

//	write_byte(MS5837_PROM0, START|RUN);
//	//SysCtlDelay(400);
//	p->c[0] = (read_byte (M_ACK|START|RUN) << 8) | read_byte ((~M_ACK)&RUN|STOP);
//
//
//
//	write_byte(MS5837_PROM1, START|RUN|STOP);
//	//SysCtlDelay(400);
//	p->c[1] = (read_byte (M_ACK|START|RUN) << 8) | read_byte ((~M_ACK)&RUN|STOP);
//
//	write_byte(MS5837_PROM2, START|RUN);
//////	SysCtlDelay(40000);
//	p->c[2] = (read_byte (M_ACK|START|RUN) << 8) | read_byte ((~M_ACK)&RUN|STOP);
////	p->c[2] = read_byte (M_ACK|START|RUN) << 8;
////	p->c[2] |= read_byte ((~M_ACK) &RUN|STOP);
////
//	write_byte(MS5837_PROM3, START|RUN);
//	p->c[3] = (read_byte (M_ACK|START|RUN) << 8) | read_byte ((~M_ACK)&RUN|STOP);
//	write_byte(MS5837_PROM+6, START|RUN);
////	SysCtlDelay(40000);
//	p->c[3] = read_byte (M_ACK|START|RUN) << 8;
//	p->c[3] |= read_byte ((~M_ACK) &RUN|STOP);
//
//	write_byte(MS5837_PROM+8, START|RUN);
////	SysCtlDelay(40000);
//	p->c[4] = read_byte (M_ACK|START|RUN) << 8;
//	p->c[4] |= read_byte ((~M_ACK) &RUN|STOP);
//
//	write_byte(MS5837_PROM+10, START|RUN);
////	SysCtlDelay(40000);
//	p->c[5] = read_byte (M_ACK|START|RUN) << 8;
//	p->c[5] |= read_byte ((~M_ACK) &RUN|STOP);
//
//	write_byte(MS5837_PROM+12, START|RUN);
////	SysCtlDelay(40000);
//	p->c[6] = read_byte (M_ACK|START|RUN) << 8;
//	p->c[6] |= read_byte ((~M_ACK) &RUN|STOP);
//
//	write_byte(MS5837_PROM+14, START|RUN);
////	SysCtlDelay(40000);
//	p->c[7] = read_byte (M_ACK|START|RUN) << 8;
//	p->c[7] |= read_byte ((~M_ACK) &RUN|STOP);



}



