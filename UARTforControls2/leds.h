/*
 * leds.h
 *
 *  Created on: Feb 16, 2016
 *      Author: luis gonzalez
 */
#ifndef UARTFORCONTROLS2_LEDS_H_
#define UARTFORCONTROLS2_LEDS_H_

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

//#define TARGET_IS_BLIZZARD_RB1 defined in properties

//base frequency to control servo
#define PWM_FREQUENCY 50 //will need to check specs. the parallax needs a pulse about every 20ms
#define LED_FREQUENCY 100// a pulse is 10ms long
//from pin_map.h, that whole lib not being inclded need to fix
#define GPIO_PA0_U0RX           0x00000001

#define GPIO_PA1_U0TX           0x00000401

#define GPIO_PE0_U7RX           0x00040001

#define GPIO_PE1_U7TX           0x00040401

#define PWM_BASE PWM1_BASE

#define FAILURE 0
#define SUCCESS 1

#define OFF 1 // since counter goes to 0
//for testing using lesser units, to light is not as bright divide by half
#define MIN 125// 1563 //25% -- 20%
#define MIDDLE_MIN 250 //3125 //50% -- 40%
#define MIDDLE 375 //4688 //75% -- 60%
#define MAX 500 //6250 //100% -- 80%
#define SUPERMAX 625 //6250 //100% -- 100%
#define MAX_LIMIT 12499



volatile uint32_t ui32Load;
volatile uint32_t brightness; // to hold pwm pulse width count
volatile uint16_t lightlevel; // variable to hold counter for brightness level
volatile uint32_t ui32PWMClock;
volatile uint16_t ui8Adjust;




/*
 * Function Prototypes
 */
/*****************************************************
 * @Function ledsInit()
 * @param configre PWM module 1 PWM PD1
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initliz pin and configure to allow for servo control
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
uint8_t ledsInit();


/*****************************************************
 * @Function ledsBright()
 * @param increase pulse width to brighten LED
 *
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control PWM
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
uint8_t ledsBright();

/*****************************************************
 * @Function ledsDim()
 * @param set pulse width to max
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
//uint8_t ledsDim();


/*****************************************************
 * @Function ledsOff()
 * @param set pulse width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse width to Min,
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
uint8_t ledsOff();




// used in test harness
void UART_Init(void);
//void PutString(char* string);


#endif /* UARTFORCONTROLS2_LEDS_H_ */
