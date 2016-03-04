
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
static volatile uint8_t data[64];
static volatile uint8_t count=0;
static volatile uint8_t upsideDown = 0;
volatile accelerometer accel;


//For the USB UART connection to get controls from the pc
#ifdef TIMER
#if 1

void Timer0IntHandler(){

	static double xtmp=0;
	static double ytmp=0;
	static double ztmp=0;
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	accelerometer_data_get(&accel);
		if(accel.x != xtmp|accel.y != ytmp|accel.z != ztmp){
			if(accel.x == -6912){
				upsideDown = 1; //if the accelerometer is past 90 degrees in any direction
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
				ztmp = -asin((-accel.z * 31.2)/1000)* (180/3.14);
				if((-accel.z * 31.2)/1000 >1)
					ztmp = -90.0;
			} else {
				ztmp = asin((accel.z * 31.2)/1000)* (180/3.14);
				if((accel.z * 31.2)/1000 >1)
					ztmp = 90.0;
			}
			printf("x = %f, y= %f",xtmp,ytmp);
			(upsideDown)? printf("  UpsideDown\n"): printf("  RightsideUp\n");
			upsideDown = 0;
		}

}
#endif
#endif
#ifdef CONTROLS
#if 1
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
			//UARTCharPut(UART7_BASE, d);
			//continue;
			//if (d == LMSU |d == LMSD |d == RMSU |d == RMSD |d == STOP |d == CU |d == CD |d == SURFACE |d == DIVE | d==LIGHTS){
				data[index]=d;
				index++;
				count++;
			//}
			if(index > 63){
				index = 0;
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
	//static uint8_t headerflag=0;
	uint8_t temp;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART7_BASE, true); //get interrupt status
	UARTIntClear(UART7_BASE, ui32Status); //clear the asserted interrupts

	if(ui32Status == UART_INT_RX || ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART7_BASE)) //loop while there are chars
		{
			temp = UARTCharGetNonBlocking(UART7_BASE);
			UARTCharPut(UART0_BASE, temp);
			continue;
			/*
			if (temp == HEADER){
				headerflag =1;
				continue;
			}
			if(headerflag){
				*/
				if (SetBitCount(temp)!=1){
					if(temp  == STOP_ALL){
						PutString("Stop\n\r");
						servoSetCenter();
						motorStop(MOTOR_1);
						motorStop(MOTOR_2);

					}else if((temp & RMOTOR_2BITS) == LIGHTS_TOGGLE){
						PutString("Lights\n\r");
						ledsBright();

					} else {
						PutString("Bad Command\n\r");

					}
				} else {
					if ((temp & CAMERA_2BITS) == CAMERA_UP){
						PutString("Camera up\n\r");
						servoSetPulseWidth(1);

					}else if((temp & CAMERA_2BITS) == CAMERA_DOWN){
						PutString("Camera Down\n\r");
						servoSetPulseWidth(0);

					}else if((temp & LMOTOR_2BITS) == LMOTOR_UP){
						PutString("Left motor up\n\r");
						motorsSetPulseWidth(MOTOR_1,1);

					}else if((temp & LMOTOR_2BITS) == LMOTOR_DOWN){
						PutString("Left motor down\n\r");
						motorsSetPulseWidth(MOTOR_1,-1);

					}else if((temp & RMOTOR_2BITS) == RMOTOR_UP){
						PutString("Right motor up\n\r");
						motorsSetPulseWidth(MOTOR_2,1);

					}else if((temp & RMOTOR_2BITS) == RMOTOR_DOWN){
						PutString("Right motor down\n\r");
						motorsSetPulseWidth(MOTOR_2,-1);

					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_UP){
//		    			accelerometer_data_get(&accel);
						motorsSetPulseWidth(MOTOR_Z,1);

		    			//printf ("x = %d\n", accel.xg0);
						PutString("Z motor up\n\r");
					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_DOWN){
						PutString("Z motor down\n\r");
						motorsSetPulseWidth(MOTOR_Z,-1);

					}
					//headerflag = 0;
				}
			//}
		}
	}

}
#endif

#if 1
int main(void) {
	uartInit();
	servoInit();
	//ledsInit();
	//motorsInit();

	//initialize_i2c();
	//initialize_accelerometer();
	//timerInit();
	SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec

	ROM_IntMasterEnable(); //enable processor interrupts



	char* mes = "Enter Commands (w, s, e, d, r, f, x, i, k, or l): ";
    uint8_t i;
    uint8_t localdata;
    PutString(mes);

    //SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec


    //keeps an index of the data from uart
    i = 0;
    int c =0;
    while (1) //let interrupt handler do the UART echo function
    {
//		accelerometer_data_get(accel);

//		printf("x = %d\n", accel->x);

    	//takes data from UART0 (the computer) and puts them into UART7 (transfer uart)
    	if (count > 0){
    		localdata = data[i];
    		count--;

    		if(localdata == 0xAA){
    			c=1;
    			data[i]=0;
    			i++;

    		}
    		if(c){
    			count --;
    			localdata = data[i];
    			if ((localdata & 1) ==1){
    				servoSetPulseWidth(1);
    			}
    			if((localdata & 2)==2){
    				servoSetPulseWidth(0);
    			}
    			data[i]=0;
    			i++;
    			c=0;
    		} else {
    			i++;
    		}
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
    		if (i > 63){
    			i = 0;
    		}
    	}
    }

}

#endif
#endif



uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

