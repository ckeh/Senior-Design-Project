/*
 * Timer.h
 *
 *  Created on: Feb 25, 2016
 *      Author: ckeh
 */

#ifndef UARTFORCONTROLS2_TIMER_H_
#define UARTFORCONTROLS2_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/timer.h"
#include "servoControl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"


/*
 * Function Prototypes
 */
/*****************************************************
 * @Function timerInit()
 * @param configre PWM module 1 PWM 2 and pwm3 and PWM module 1 PWM 6
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initlize pin and configure to allow for motors to be controlled control
 * @note
 * @author Christopher Keh
 *
 *****************************************************/
uint8_t timerInit();
uint8_t timerInit2();



#endif /* UARTFORCONTROLS2_TIMER_H_ */
