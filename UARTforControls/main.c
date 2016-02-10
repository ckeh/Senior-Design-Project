
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
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"


//control scheme
#define LMSU				 		'w'	//left motor speed up
#define RMSU 						'r' //right motor speed up
#define LMSD 						's' //left motor speed down
#define RMSD 						'f' //right motor speed down
#define STOP 						'x'
#define CU 							'i' //Camera up
#define CD			 				'k' //Camera down
#define DIVE 						'd'
#define SURFACE 					'e'
#define LIGHTS						'l'

//Bit Mask to filter controls
#define LMOTOR_2BITS				0xC0
#define RMOTOR_2BITS				0x30
#define ZMOTOR_2BITS				0x0C
#define CAMERA_2BITS				0x03

//Bit Mask to set individual controls
#define CAMERA_UP					0x01
#define CAMERA_DOWN					0x02
#define ZMOTOR_UP					0x04
#define ZMOTOR_DOWN					0x08
#define RMOTOR_UP					0x10
#define RMOTOR_DOWN					0x20
#define LMOTOR_UP					0x40
#define LMOTOR_DOWN					0x80
#define STOP_ALL					0xFF
#define LIGHTS_TOGGLE				0x30


//Header for controls
#define HEADER						0xAA


void Init(void);
void PutString(char* string);
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
					} else {
						PutString("Bad Command\n\r");
					}
				} else {
					if ((temp & CAMERA_2BITS) == CAMERA_UP){
						PutString("Camera up\n\r");
					}else if((temp & CAMERA_2BITS) == CAMERA_DOWN){
						PutString("Camera Down\n\r");
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
	Init();


	char* mes = "Enter Words: ";
    uint8_t i;
    uint8_t localdata;
    PutString(mes);
    /*
    for (i = 0; i<13; i++){
    	UARTCharPut(UART0_BASE, mes[i]);
    }
*/
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

void Init(){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED


	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);

	ROM_GPIOPinConfigure(GPIO_PE0_U7RX);
	ROM_GPIOPinConfigure(GPIO_PE1_U7TX);

	ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //enable pin for LED PF2

	ROM_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	ROM_UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full
    //UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_FIFO);
    //UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full

	ROM_UARTFIFOEnable(UART0_BASE);

    //UARTTxIntModeSet(UART7_BASE, UART_TXINT_MODE_FIFO);

	ROM_UARTFIFOEnable(UART7_BASE);

	ROM_IntMasterEnable(); //enable processor interrupts
	ROM_IntEnable(INT_UART0); //enable the UART interrupt
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	ROM_UARTEnable(UART0_BASE);
	ROM_IntEnable(INT_UART7); //enable the UART interrupt
	ROM_UARTIntEnable(UART7_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	ROM_UARTEnable(UART7_BASE);
}

void PutString(char* string){
	uint8_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPutNonBlocking(UART0_BASE, string[i]);
	}
}

uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    //printf("%d", count);
    return count;
}

