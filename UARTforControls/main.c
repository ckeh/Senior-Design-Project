
//#include <stdint.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <string.h>
//#include "inc/hw_ints.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "driverlib/gpio.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/uart.h"
//#define TARGET_IS_BLIZZARD_RB1
//#include "driverlib/rom.h"
#include "leds.h"
#include "servoControl.h"
#include "controlScheme.h"
#include "uart.h"


//Header for controls not needed since only one byte of data
//#define HEADER						0xAA

uint8_t SetBitCount(uint8_t);

//Store incoming data to be processed and packeted
static volatile uint8_t data[64];
static volatile uint8_t count=0;
static volatile uint8_t index=0;

//For the USB UART connection to get controls from the pc
void UART0IntHandler(void)
{
	uint8_t d;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	//If interrupt is from RX or receive timeout
	if(ui32Status == UART_INT_RX| ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
		{
			d = UARTCharGetNonBlocking(UART0_BASE);
			if (d == LMSU |d == LMSD |d == RMSU |d == RMSD |d == STOP |d == CU |d == CD |d == SURFACE |d == DIVE | d==LIGHTS){
				data[index]=d;
				index++;
				count++;
			}
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
					}else if((temp & LMOTOR_2BITS) == LMOTOR_DOWN){
						PutString("Left motor down\n\r");
					}else if((temp & RMOTOR_2BITS) == RMOTOR_UP){
						PutString("Right motor up\n\r");
					}else if((temp & RMOTOR_2BITS) == RMOTOR_DOWN){
						PutString("Right motor down\n\r");
					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_UP){
						PutString("Z motor up\n\r");
					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_DOWN){
						PutString("Z motor down\n\r");
					}
					//headerflag = 0;
				}
			//}
		}
	}

}

int main(void) {
	uartInit();
	servoInit();
	ledsInit();

	char* mes = "Enter Commands (w, s, e, d, r, f, x, i, k, or l): ";
    uint8_t i;
    uint8_t localdata;
    PutString(mes);

    SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
    i = 0;
    while (1) //let interrupt handler do the UART echo function
    {
    	if (count > 0){
    		localdata = data[i];
    		count--;
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
    		if (i > 63){
    			i = 0;
    		}
    	}
    }

}


uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

