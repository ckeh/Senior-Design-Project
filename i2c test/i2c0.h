/*
 * i2c0.h
 *
 *  Created on: Apr 7, 2016
 *      Author: dvguille
 */

#ifndef I2C0_H_
#define I2C0_H_

void InitI2C0(void);
void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);



#endif /* I2C0_H_ */
