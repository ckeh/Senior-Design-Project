
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

#define RADTD (180/3.14)
//Header for controls not needed since only one byte of data
//#define HEADER						0xAA

uint8_t SetBitCount(uint8_t);

//Store incoming data to be processed and packeted
static volatile uint8_t data[20];
static volatile uint8_t count=0;
static volatile uint8_t upsideDown = 0;
volatile accelerometer accel;


//For the USB UART connection to get controls from the pc
#ifdef TIMER

void Timer0IntHandler(){

	static double xtmp=0;
	static double ytmp=0;
	static double ztmp=0;
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	accelerometer_data_get(&accel);
		if(accel.x != xtmp|accel.y != ytmp|accel.z != ztmp){
			if(accel.x == -6912){
				//upsideDown = 1; //if the accelerometer is past 90 degrees in any direction
				accelerometer_data_get(&accel); //gets data from accelerometer and places it in accel
			}
			if(accel.x <0){
				xtmp = -asin((-accel.x * 31.2)/1000) * (180/3.14);
				if((-accel.x * 31.2)/1000 >.9984)
					xtmp = -90.0;
			} else {
				xtmp = asin((accel.x * 31.2)/1000)* (180/3.14);
				if((accel.x * 31.2)/1000 >1)
					xtmp = 90.0;
			}
			if(accel.y <0){
				ytmp = -asin((-accel.y * 31.2)/1000)* (180/3.14);
				if((-accel.y * 31.2)/1000 >1)
					ytmp = -90.0;
			} else {
				ytmp = asin((accel.y * 31.2)/1000)* (180/3.14);
				if((accel.y * 31.2)/1000 >1)
					ytmp = 90.0;
			}
			if(accel.z <0){
				upsideDown = 1;
				ztmp = -asin((-accel.z * 31.2)/1000)* (180/3.14);
				if((-accel.z * 31.2)/1000 >1)
					ztmp = -90.0;
			} else {
				upsideDown = 0;
				ztmp = asin((accel.z * 31.2)/1000)* (180/3.14);
				if((accel.z * 31.2)/1000 >1)
					ztmp = 90.0;
			}
			/*
			if(xtmp >15 && xtmp<=30){
				PutString(UART7_BASE, "x 30\n\r");
			} else if (xtmp > 30 && xtmp <=60){
				PutString(UART7_BASE, "x 60\n\r");
			} else if (xtmp >60 && xtmp <=90){
				PutString(UART7_BASE, "x 90\n\r");
			} else if (xtmp <0 && xtmp>=-30){
				PutString(UART7_BASE, "x -30\n\r");
			} else if (xtmp <-30 && xtmp >=-60){
				PutString(UART7_BASE, "x -60\n\r");
			} else if (xtmp <-60 && xtmp >=-90){
				PutString(UART7_BASE, "x -90\n\r");
			} else {
				PutString(UART7_BASE, "x 0\n\r");
			}
			if(ytmp >15 && ytmp<=30){
				PutString(UART7_BASE, "y 30\n\r");
			} else if (ytmp > 30 && ytmp <=60){
				PutString(UART7_BASE, "y 60\n\r");
			} else if (ytmp >60 && ytmp <=90){
				PutString(UART7_BASE, "y 90\n\r");
			} else if (ytmp <0 && ytmp>=-30){
				PutString(UART7_BASE, "y -30\n\r");
			} else if (ytmp <-30 && ytmp >=-60){
				PutString(UART7_BASE, "y -60\n\r");
			} else if (ytmp <-60 && ytmp >=-90){
				PutString(UART7_BASE, "y -90\n\r");
			} else {
				PutString(UART7_BASE, "y 0\n\r");
			}
			*/
			printf("x = %f, y= %f",xtmp,ytmp);
			(upsideDown)? printf("  UpsideDown\n"): printf("  RightsideUp\n");
			upsideDown = 0;
		}

}
#endif
#ifdef CONTROLS
void UART0IntHandler(void)
{
	static uint8_t index = 0;

	uint8_t d;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	//If interrupt is from RX or receive timeout
	if(ui32Status == UART_INT_RX| ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
		{
			d = UARTCharGetNonBlocking(UART0_BASE);
#ifdef UART_TEST
			UARTCharPut(UART7_BASE, d);
			continue;
#endif
			//if (d == LMSU |d == LMSD |d == RMSU |d == RMSD |d == STOP |d == CU |d == CD |d == SURFACE |d == DIVE | d==LIGHTS){

				//data[index]=d;
				//index++;
				//count++;
			//}
			if(index > 19){
				//index = 0;
			}
		}
	}
	/*
	if(ui32Status ==UART_INT_TX){
		PutString("tx interrupt");
	}
	*/
}

//For UART 7 connecting to the other device
void UART7IntHandler(void)
{
	static uint8_t index = 0;
	uint8_t temp;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART7_BASE, true); //get interrupt status
	UARTIntClear(UART7_BASE, ui32Status); //clear the asserted interrupts
	if(ui32Status == UART_INT_RX| ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART7_BASE)) //loop while there are chars
		{
			temp = UARTCharGetNonBlocking(UART7_BASE);
			UARTCharPut(UART7_BASE, temp);
#ifdef UART_TEST
			UARTCharPut(UART0_BASE, temp);
			continue;
#endif
			//if (d == LMSU |d == LMSD |d == RMSU |d == RMSD |d == STOP |d == CU |d == CD |d == SURFACE |d == DIVE | d==LIGHTS){

				data[index]=temp;
				index++;
				count++;
			//}
			if(index > 19){
				index = 0;
			}
		}
	}


}


int main(void) {
	uartInit();
	servoInit();
	ledsInit();
	motorsInit();

	initialize_i2c();
	initialize_accelerometer();
	timerInit();
	SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec

	ROM_IntMasterEnable(); //enable processor interrupts



	char* mes = "Enter Commands (w, s, e, d, r, f, x, i, k, or l): ";
    uint8_t i;
    uint8_t localdata;
    PutString(UART7_BASE, mes);
    //UARTPutChar(UART7_BASE, 'huh');
    //SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec


    //keeps an index of the data from uart
    i = 0;
    uint8_t headerFlag =0;
    uint8_t lightsFlag = 0;
    uint8_t first;
    uint8_t second;
    uint8_t third;
    uint8_t fourth;
    uint8_t tempmain;
    uint8_t rBumper;

    while (1) //let interrupt handler do the UART echo function
    {

    	//takes data from UART0 (the computer) and puts them into UART7 (transfer uart)
    	if (count > 4){
    		count--;
    		localdata = data[i];

    		//HEADER
    		if(localdata == 0xAA){
    			headerFlag=1;
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    			count --;
    			first = data[i];
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    			count --;
    			second = data[i];
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    			count --;
    			third = data[i];
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    			count --;
    			fourth = data[i];
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    		}


    		//if header is detected that means we start looking for packet
    		if(headerFlag){
//    			count --;
//    			localdata = data[i];
//    			i++;
//        		if (i > 19){
//        			i = 0;
//        		}

    			/*-------------------------------------------------------------------
    			 * first byte of packet
    			 *
    			 * ---------------------------------------------------------------- */

    			// D pad UP
    			if ((first & 1) ==1){
    				//ledsBright();
    				//motorsSetPulseWidth(MOTOR_1,1);
    				servoSetPulseWidth(1);
    			}

    			// Left bumper
    			if((first & 2)==2){
    				//motorsSetPulseWidth(MOTOR_1,-1);
    				//servoSetPulseWidth(0);
    			}

    			// Dpad down
    			if((first & 4)==4){
    				//motorsSetPulseWidth(MOTOR_1,-1);
    				//servoSetPulseWidth(0);
    				servoSetPulseWidth(0);
    			}

    			// D pad Right (Right Bumper Now)
    			if((first & 8)==8){
    				//motorsSetPulseWidth(MOTOR_1,-1);
    				//servoSetPulseWidth(1);
    				rBumper = 1;
    			} else {
    				rBumper = 0;
    			}

    			// Right Trigger = 16-255
    			if((first & 0xF0) >= 0){
    				tempmain = (first & 0xf0);
    				tempmain = (tempmain>>4);
    				motorsVariable (MOTOR_Z, tempmain);
    			}


    			/*-------------------------------------------------------------------
    			 * second byte of packet
    			 *
    			 * ---------------------------------------------------------------- */
    			// A
				if((second & 1)==1){

				}

				// B
				if((second & 2) == 2){
					servoSetCenter();
					motorStop(MOTOR_1);
					motorStop(MOTOR_2);
					motorStop(MOTOR_Z);
				}

				//user has to press and can't hold lights button
				// X
				if((second & 4) == 4 && lightsFlag ==0){
					ledsBright();
					lightsFlag =1;
				} else if((second & 4) != 4){
					lightsFlag = 0;
				}

				// Y
				if((second & 8)== 8){

				}

				//L Trigger
    			if((second & 0xF0) >0){
    				tempmain = (second&0xf0);
    				tempmain = (tempmain>>4);
					motorsVariable (MOTOR_Z, -tempmain);

    			}

    			/*-------------------------------------------------------------------
    			 * third byte of packet
    			 *
    			 * ---------------------------------------------------------------- */
    			//Forward
    			if((third & 0x0F) >= 0){
//    				motorsSetPulseWidth(MOTOR_1,1);
//    				motorsSetPulseWidth(MOTOR_2,1);
    				tempmain = (third&0x0f);
    				motorsVariable (MOTOR_1, tempmain);
    				motorsVariable (MOTOR_2, tempmain);
    			}
    			//Backward
    			if((third & 0xF0) > 0){

//    				motorsSetPulseWidth(MOTOR_1,-1);
//    				motorsSetPulseWidth(MOTOR_2,-1);
    				tempmain = (third&0xf0);
					tempmain = (tempmain>>4);
					motorsVariable (MOTOR_1, -tempmain);
					motorsVariable (MOTOR_2, -tempmain);
    			}

    			/*-------------------------------------------------------------------
    			 * fourth byte of packet
    			 *
    			 * ---------------------------------------------------------------- */
    			// Turn Left
    			if((fourth & 0x0F)>0){
    				tempmain = (fourth & 0x0f);
    				if(rBumper == 1){
	//    				motorsSetPulseWidth(MOTOR_2,-1);
	//    				motorsSetPulseWidth(MOTOR_1,1);
						motorsVariable (MOTOR_1, -tempmain);
						motorsVariable (MOTOR_2, tempmain);
    				} else {
    					motorsVariable (MOTOR_1, 0);
    					motorsVariable (MOTOR_2, tempmain);
    				}

    			}
    			// Turn Right
    			if((fourth & 0xF0)>0){
//    				motorsSetPulseWidth(MOTOR_2,1);
//    				motorsSetPulseWidth(MOTOR_1,-1);
    				tempmain = (fourth & 0xf0);
					tempmain = (tempmain >> 4);
					if(rBumper == 1){
						motorsVariable (MOTOR_1, tempmain);
						motorsVariable (MOTOR_2, -tempmain);
					} else {
						motorsVariable (MOTOR_2, 0);
						motorsVariable (MOTOR_1, tempmain);
					}
				}


    			headerFlag=0;
    		} else {
    			i++;
        		if (i > 19){
        			i = 0;
        		}
    		}

    		//for keyboard controls
/*
    		if(localdata == LMSU){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, LMOTOR_UP);
    			i++;
    		} else if (localdata == LMSD){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, LMOTOR_DOWN);
    			i++;
    		}else if (localdata == RMSU){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, RMOTOR_UP);
    			i++;
    		}else if (localdata == RMSD){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, RMOTOR_DOWN);
    			i++;
    		}else if (localdata == CU){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, CAMERA_UP);
    			i++;
    		}else if (localdata == CD){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, CAMERA_DOWN);
    			i++;
    		}else if (localdata == DIVE){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, ZMOTOR_DOWN);
//    			accelerometer_data_get(&accel);
//    			printf ("x = %d\n", accel.xg0);
    			i++;
    		}else if (localdata == SURFACE){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, ZMOTOR_UP);
    			i++;
    		}else if (localdata == STOP){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, STOP_ALL);
    			i++;
    		}else if (localdata == LIGHTS){
    			//UARTCharPut(UART7_BASE, HEADER);
    			UARTCharPut(UART7_BASE, LIGHTS_TOGGLE);
    			i++;
    		}
    		*/
    		if (i > 19){
    			i = 0;
    		}
    	}
    }

}

#endif



uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

