/*
 * i2c0.h
 *
 *  Created on: Apr 7, 2016
 *      Author: dvguille
 */

#ifndef I2C0_H_
#define I2C0_H_

void InitI2C1(void);
void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);
uint32_t I2CReceive1(uint32_t slave_addr, uint8_t reg, uint8_t command1, uint8_t command2);
uint32_t I2CReadStart(uint32_t slave_addr, uint8_t reg );
uint32_t I2CRead(uint32_t slave_addr, uint32_t cmd);
uint32_t I2CReadEnd(uint32_t slave_addr);


#endif /* I2C0_H_ */
