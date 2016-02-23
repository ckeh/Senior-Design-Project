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

#include "uart.h" //for use of PutStrin()

//#define TARGET_IS_BLIZZARD_RB1 //defined in properties

//base frequency to control servo
#define PWM_FREQUENCY 50 //will need to check specs. the parallax needs a pulse about every 2ms

//from pin_map.h, that whole lib not being inclded need to fix
#define GPIO_PA0_U0RX           0x00000001

#define GPIO_PA1_U0TX           0x00000401

#define GPIO_PE0_U7RX           0x00040001

#define GPIO_PE1_U7TX           0x00040401

#define PWM_BASE PWM1_BASE

#define FAILURE 0
#define SUCCESS 1

#define SERVO_MIN_PULSE 38// for the servo out min is .75 ms
#define SERVO_MAX_PULSE 112//90-> for esc, for servo the max is 2.25 ms


volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint16_t ui8Adjust;

/*
 * Function Prototypes
 */
/*****************************************************
 * @Function servoInit()
 * @param the pin want servo to be PWM to come from
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initliz pin and configure to allow for servo control
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoInit();


/*****************************************************
 * @Function servoSetPulseWidth()
 * @param pulse width want 1=increase width, 0 decrease width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/13
 *
 *****************************************************/
uint8_t servoSetPulseWidth(uint8_t var);

/*****************************************************
 * @Function servoSetMax()
 * @param set pulse width to max
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/13
 *
 *****************************************************/
uint8_t servoSetMax();

/*****************************************************
 * @Function servoSetMin()
 * @param set pulse width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse width to Min,
 * @note
 * @author Luis Gonzalez, 2/13
 *
 *****************************************************/
uint8_t servoSetMin();


/*****************************************************
 * @Function servoSetCenter()
 * @param pulse width want 0-100
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoSetCenter();


void UART_Init(void);
//void PutString(char* string);






