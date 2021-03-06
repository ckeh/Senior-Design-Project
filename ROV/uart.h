/*
 * uart.h
 *
 *  Created on: Feb 18, 2016
 *      Author: ckeh
 */

#ifndef UART_H_
#define UART_H_

/*
 * Function Prototypes
 */
/*****************************************************
 * @Function uartInit()
 * @param none
 * @return none
 *
 * @brief Initlize uart0 for usb-uart and uart7 on pe0 and 1.
 * @note
 * @author Christopher Keh
 *
 *****************************************************/
void uartInit(void);

/*****************************************************
 * @Function PutStringNonBlocking()
 * @param string that you want to put to uart
 * @return
 *
 * @brief Puts string to the uart but doesn't wait for fifo to be ready.
 * @note
 * @author Christopher Keh
 *
 *****************************************************/
void PutStringNonBlocking(char* string);

/*****************************************************
 * @Function PutString()
 * @param configre PWM module 1 PWM PD1
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Put string to the uart and waits for fifo to be ready.
 * @note
 * @author Christopher Keh
 *
 *****************************************************/
void PutString(char* string);


#endif /* UART_H_ */
