#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ReadBattery.h"
#include "servoControl.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "driverlib/gpio.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/uart.h"

void Init(void);
uint8_t SetBitCount(uint8_t i);
void PutString(char* string);
void UART0IntHandler(void);

static volatile uint8_t data[64];
static volatile uint8_t count=0;
static volatile uint8_t index=0;

int main(void){
	Init();
	SetADC();
	servoInit();

	int BatVolt, oldBatVolt = 0;
	double j;

	while(1){
		BatVolt = ReadADC();
//		if(oldBatVolt >= BatVolt && oldBatVolt < BatVolt + 100){
//
//		}else if(oldBatVolt <= BatVolt && oldBatVolt > BatVolt - 100){
//
//		}else if(oldBatVolt >= BatVolt + 100){
//			oldBatVolt = BatVolt;
//			servoSetPulseWidth(1);
//		}else if(oldBatVolt <= BatVolt - 100){
//			oldBatVolt = BatVolt;
//			servoSetPulseWidth(0);
//		}
		if(BatVolt > 1900 && BatVolt < 2100){
			servoSetCenter();
		}
		else if(oldBatVolt >= BatVolt - 100 && oldBatVolt <= BatVolt + 100){
		}
		else{
			oldBatVolt = BatVolt;
			servoSetWidthValue(oldBatVolt);
		}
	}

//	while(1){
//		BatVolt = ReadBat();
//		j = BatVolt/4096.0;
//		if(j >= .99) PutString("90%\r\n");
//		else if(j < .99 && j>=.98) PutString("80%\r\n");
//		else if(j < .98 && j>=.97) PutString("70%\r\n");
//		else if(j < .97 && j>=.96) PutString("60%\r\n");
//		else if(j < .96 && j>=.95) PutString("50%\r\n");
//		else if(j < .95 && j>=.94) PutString("40%\r\n");
//		else if(j < .94 && j>=.93) PutString("30%\r\n");
//		else if(j < .93 && j>=.92) PutString("20% Warning\r\n");
//		else if(j < .92 && j>=.91) PutString("10% Warning\r\n");
//		else if(j < .91 && j>=.9) PutString("00% Warning\r\n");
//	}
}


void Init(void){
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //enable pin for LED PF2

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));

	UARTFIFOEnable(UART0_BASE);

	IntMasterEnable(); //enable processor interrupts
	IntEnable(INT_UART0); //enable the UART interrupt
	UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	UARTEnable(UART0_BASE);
}

void UART0IntHandler(void)
{
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
	 {
	 UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //echo character
	 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //blink LED
	 SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
	 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0); //turn off LED
	 }
}


uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

void PutString(char* string){
	uint8_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPut(UART0_BASE, string[i]);
	}
}

