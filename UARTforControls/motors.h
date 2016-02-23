/*
 * motors.h
 *
 *  Created on: Feb 22, 2016
 *      Author: luis gonzalez
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"

#include "uart.h" // so can use putString

//the three motors will be using
#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_Y 3

#define SUCCESS 1
#define FAILURE 0


#define PWM_FREQUENCY 50

#define PULSE_MAX 100
#define PULSE_MIN 50

volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint16_t ui8Adjust;

volatile uint16_t ui8PulseAdjust_1;
volatile uint16_t ui8PulseAdjust_2;
volatile uint16_t ui8PulseAdjust_3;

static uint16_t motors_var; // for use in motors_var set pulse width

static volatile uint8_t data = 0; // used in UART code



/*
 * Function Prototypes
 */
/*****************************************************
 * @Function motorsInit()
 * @param configre PWM module 1 PWM 2 and pwm3 and PWM module 1 PWM 6
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initlize pin and configure to allow for motors to be controlled control
 * @note
 * @author Luis Gonzalez, 2/22
 *
 *****************************************************/
uint8_t motorsInit();


/*****************************************************
 * @Function motorsSetPulseWidth()
 * @param motor controlling, pulse want to change ti
 *			motor must one of the following:  MOTOR_1, MOTOR_2, MOTOR_Y

 * 			SpeedChange must be int (-5) - (5) which will be coresponding to jump in pulse width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief select motor want to change, then the speed you want to change
 * @note
 * @author Luis Gonzalez, 2/22
 *
 *****************************************************/
uint8_t motorsSetPulseWidth(uint8_t motor, int speedChange);



/*****************************************************
 * @Function motorStop()
 * @param motor that want to stop
 * motor must one of the following:  MOTOR_1, MOTOR_2, MOTOR_Y
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initlize pin and configure to allow for motors to be controlled control
 * @note
 * @author Luis Gonzalez, 2/22
 *
 *****************************************************/
uint8_t motorStop(uint8_t motor);


void UART_Init();
//void PutString(char* String);


#endif /* MOTORS_H_ */
