#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "leds.h"
#include "servoControl.h"
#include "controlScheme.h"
#include "uart.h"
#include "i2c.h"
#include "accelerometer.h"
#include "motors.h"
#include "Timer.h"
#include "driverlib/i2c.h"
#include "pressure.h"

#define BUFF_SIZE 40

#define RADTD (180/3.14)
//Header for controls not needed since only one byte of data
//#define HEADER						0xAA

uint8_t SetBitCount(uint8_t);

//Store incoming data to be processed and packeted
static volatile uint8_t data[BUFF_SIZE];
static volatile uint8_t count = 0;
volatile accelerometer accel;
volatile pressure p;



//For the USB UART connection to get controls from the pc
#ifdef TIMER

void Timer0IntHandler() {
	static int16_t xtmp = 0;
	static int16_t ytmp = 0;
	static int16_t ztmp = 0;

	float depth = 0;
	static float max = 0;
	static float min = 35;

	pressure_read(&p);
	calculatePress(&p);

	//depth = ((p.pressure*10.0f)-101300)/(997*9.80665); // in meters
//	depth = ((p.pressure/10.0f)*0.0335); // in feet
//	if (depth > max){
//		max = depth;
//	}
//	if (depth < min){
//		min = depth;
//	}
//	diff = depth - min;

	accelerometer_data_get(&accel);
	if (accel.x != xtmp | accel.y != ytmp | accel.z != ztmp) {

		xtmp = to_degrees(XAXIS, xtmp, &accel);
		ytmp = to_degrees(YAXIS, ytmp, &accel);
		ztmp = to_degrees(ZAXIS, ztmp, &accel);


//		//7
//		if(ytmp < 0 && ztmp < 0 && xtmp < 0){
//			xtmp = -180 - xtmp;
//			ytmp = -180 - ytmp;
//		}
//		//8
//		else if(ytmp < 0 && ztmp < 0 && xtmp >= 0){
//			xtmp = xtmp;
//			ytmp = 180 - ytmp;
//		}
//		//3
//		else if(ytmp < 0 && ztmp >= 0 && xtmp < 0){
//			xtmp = -180 - xtmp;
//			ytmp = -180 - ytmp;
//		}
//		//4
//		else if(ytmp < 0 && ztmp >= 0 && xtmp >= 0){
//			xtmp = xtmp;
//			ytmp = 180 - ytmp;
//		}
//		//6
//		else if(ytmp >= 0 && ztmp < 0 && xtmp < 0){
//			xtmp = 180 - xtmp;
//			ytmp = ytmp;
//		}
//		//5
//		else if(ytmp >= 0 && ztmp < 0 && xtmp >= 0){
//			xtmp = xtmp;
//			ytmp = ytmp;
//		}
//		//2
//		else if(ytmp >= 0 && ztmp >= 0 && xtmp < 0){
//			xtmp = 180 - xtmp;
//			ytmp = ytmp;
//		}
//		//1
//		else if(ytmp >= 0 && ztmp >= 0 && xtmp >= 0){
//			xtmp = xtmp;
//			ytmp = ytmp;
//		}

		//lets x and y be full +-180
//		if(ytmp < 0 && ztmp < 0){
//			if(xtmp > 0){
//				xtmp = 180-xtmp;
//			} else {
//				xtmp = -180-xtmp;
//			}
//		}

	}
		UARTCharPut(UART7_BASE, 'a');

//		UARTCharPut(UART7_BASE, p.digital_pressure>>24);
//		UARTCharPut(UART7_BASE, p.digital_pressure>>16);
//		UARTCharPut(UART7_BASE, p.digital_pressure>>8);
//		UARTCharPut(UART7_BASE, p.digital_pressure);
//
//		UARTCharPut(UART7_BASE, p.digital_temperature>>24);
//		UARTCharPut(UART7_BASE, p.digital_temperature>>16);
//		UARTCharPut(UART7_BASE, p.digital_temperature>>8);
//		UARTCharPut(UART7_BASE, p.digital_temperature);



		UARTCharPut(UART7_BASE, p.pressure>>24);
		UARTCharPut(UART7_BASE, p.pressure>>16);
		UARTCharPut(UART7_BASE, p.pressure>>8);
		UARTCharPut(UART7_BASE, p.pressure);

		UARTCharPut(UART7_BASE, (xtmp>>8) & 0xFF);
		UARTCharPut(UART7_BASE, xtmp & 0xFF);

		UARTCharPut(UART7_BASE, (ytmp>>8) & 0xFF);
		UARTCharPut(UART7_BASE, ytmp & 0xFF);

		UARTCharPut(UART7_BASE, (ztmp>>8) & 0xFF);
		UARTCharPut(UART7_BASE, ztmp);

		printf("x%d y%d z%d\n\rp%d",xtmp,ytmp,ztmp,p.pressure);
		// Clear the timer interrupt
		TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

}
#endif
#ifdef CONTROLS
void UART0IntHandler(void) {
	uint8_t d;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	//If interrupt is from RX or receive timeout
	if (ui32Status == UART_INT_RX | ui32Status == UART_INT_RT) {
		while (UARTCharsAvail(UART0_BASE)) //loop while there are chars
		{
			d = UARTCharGetNonBlocking(UART0_BASE);
			UARTCharPut(UART7_BASE, d);
		}
	}
}

//For UART 7 connecting to the other device
void UART7IntHandler(void) {
	static uint8_t index = 0;
	uint8_t temp;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART7_BASE, true); //get interrupt status
	UARTIntClear(UART7_BASE, ui32Status); //clear the asserted interrupts
	if (ui32Status == UART_INT_RX | ui32Status == UART_INT_RT) {
		while (UARTCharsAvail(UART7_BASE)) //loop while there are chars
		{
			temp = UARTCharGet(UART7_BASE);
#ifdef UART_TEST
			UARTCharPut(UART0_BASE, temp);
			//continue;
#endif
			data[index] = temp;
			index++;
			count++;
			if (index > BUFF_SIZE-1) {
				index = 0;
			}
		}
	}
}

int main(void) {
	unsigned char check;

	uartInit();
	motorsInit();
	servoInit();
	ledsInit();

	initialize_i2c();
	pressure_init(&p);
	check = p.c[0] >> 12;
	if(check != crc4(p.c)) exit(1);
	initialize_accelerometer();
	timerInit();
	SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec

	ROM_IntMasterEnable(); //enable processor interrupts

	uint8_t i;
	uint8_t localdata;

	//keeps an index of the data from uart
	i = 0;
	uint8_t headerFlag = 0;
	uint8_t lightsFlag = 0;
	uint8_t first;
	uint8_t second;
	uint8_t third;
	uint8_t fourth;
	uint8_t tempmain;
	uint8_t rBumper;

	while (1) //let interrupt handler do the UART echo function
	{
		//UARTCharPut(UART7_BASE, 'a');

		//takes data from UART0 (the computer) and puts them into UART7 (transfer uart)
		if (count > 4) {
			count--;
			localdata = data[i];

			//HEADER
			if (localdata == 0xAA) {
				headerFlag = 1;
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
				count--;
				first = data[i];
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
				count--;
				second = data[i];
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
				count--;
				third = data[i];
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
				count--;
				fourth = data[i];
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
			}
			//if header is detected that means we start looking for packet
			if (headerFlag) {

				/*-------------------------------------------------------------------
				 * first byte of packet
				 *
				 * ---------------------------------------------------------------- */

				// D pad UP
				if ((first & 1) == 1) {
//					ledsBright();
//					motorsSetPulseWidth(MOTOR_1,1);
					servoSetPulseWidth(1);
				}

				// Left bumper
				if ((first & 2) == 2) {
//					motorsSetPulseWidth(MOTOR_1,-1);
//					servoSetPulseWidth(0);
				}

				// Dpad down
				if ((first & 4) == 4) {
//					motorsSetPulseWidth(MOTOR_1,-1);
//					servoSetPulseWidth(0);
					servoSetPulseWidth(0);
				}

				// D pad Right (Right Bumper Now)
				if ((first & 8) == 8) {
//					motorsSetPulseWidth(MOTOR_1,-1);
//					servoSetPulseWidth(1);
					rBumper = 1;
				} else {
					rBumper = 0;
				}

				// Right Trigger = 16-255
				if ((first & 0xF0) >= 0) {
					tempmain = (first & 0xf0);
					tempmain = (tempmain >> 4);
					motorsVariable(MOTOR_Z, tempmain);
				}

				/*-------------------------------------------------------------------
				 * second byte of packet
				 *
				 * ---------------------------------------------------------------- */
				// A
				if ((second & 1) == 1) {

				}

				// B
				if ((second & 2) == 2) {
					servoSetCenter();
//					motorStop(MOTOR_1);
//					motorStop(MOTOR_2);
//					motorStop(MOTOR_Z);
				}

				//user has to press and can't hold lights button
				// X
				if ((second & 4) == 4 && lightsFlag == 0) {
					ledsBright();
					lightsFlag = 1;
				} else if ((second & 4) != 4) {
					lightsFlag = 0;
				}

				// Y
				if ((second & 8) == 8) {

				}

				//L Trigger
				if ((second & 0xF0) > 0) {
					tempmain = (second & 0xf0);
					tempmain = (tempmain >> 4);
					motorsVariable(MOTOR_Z, -tempmain);

				}

				/*-------------------------------------------------------------------
				 * third byte of packet
				 *
				 * ---------------------------------------------------------------- */
				//Forward
				if ((third & 0x0F) >= 0) {
//    				motorsSetPulseWidth(MOTOR_1,1);
//    				motorsSetPulseWidth(MOTOR_2,1);
					tempmain = (third & 0x0f);
					motorsVariable(MOTOR_1, tempmain);
					motorsVariable(MOTOR_2, tempmain);
				}
				//Backward
				if ((third & 0xF0) > 0) {
//    				motorsSetPulseWidth(MOTOR_1,-1);
//    				motorsSetPulseWidth(MOTOR_2,-1);
					tempmain = (third & 0xf0);
					tempmain = (tempmain >> 4);
					motorsVariable(MOTOR_1, -tempmain);
					motorsVariable(MOTOR_2, -tempmain);
				}

				/*-------------------------------------------------------------------
				 * fourth byte of packet
				 *
				 * ---------------------------------------------------------------- */
				// Turn Left
				if ((fourth & 0x0F) > 0) {
					tempmain = (fourth & 0x0f);
					if (rBumper == 1) {
//    					motorsSetPulseWidth(MOTOR_2,-1);
//    					motorsSetPulseWidth(MOTOR_1,1);
						motorsVariable(MOTOR_1, -tempmain);
						motorsVariable(MOTOR_2, tempmain);
					} else {
						motorsVariable(MOTOR_1, -tempmain);
						motorsVariable(MOTOR_2, 0);
					}

				}
				// Turn Right
				if ((fourth & 0xF0) > 0) {
//    				motorsSetPulseWidth(MOTOR_2,1);
//    				motorsSetPulseWidth(MOTOR_1,-1);
					tempmain = (fourth & 0xf0);
					tempmain = (tempmain >> 4);
					if (rBumper == 1) {
						motorsVariable(MOTOR_1, tempmain);
						motorsVariable(MOTOR_2, -tempmain);
					} else {
						motorsVariable(MOTOR_2, -tempmain);
						motorsVariable(MOTOR_1, 0);
					}
				}

				headerFlag = 0;
			} else {
				i++;
				if (i > BUFF_SIZE-1) {
					i = 0;
				}
			}

			if (i > BUFF_SIZE-1) {
				i = 0;
			}
		}
	}

}

#endif

uint8_t SetBitCount(uint8_t i) {
	uint8_t count;
	for (count = 0; i; count++) {
		i &= i - 1;
	}
	return count;
}


