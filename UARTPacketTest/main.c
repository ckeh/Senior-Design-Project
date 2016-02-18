
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


void Init(void);
void PutString(char* string);
uint8_t SetBitCount(uint8_t);

static volatile uint64_t count=0; // keeps count of the number of packets


//not needed for this test
void UART0IntHandler(void)
{

	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts


}

//For UART 7 connecting to the other device
void UART7IntHandler(void)
{
	uint8_t temp;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART7_BASE, true); //get interrupt status
	UARTIntClear(UART7_BASE, ui32Status); //clear the asserted interrupts

	if(ui32Status == UART_INT_RX || ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART7_BASE)) //loop while there are chars
		{
			temp = UARTCharGet(UART7_BASE);
			if(temp == 'F'){
				count++;
			}

		}
	}

}

int main(void) {
	Init();

	//char str[1000000];
	char* mes = "Enter Words: ";
    uint64_t i;
    PutString(mes);

    //sends 2 minutes worth of packets
	for (i=0; i<10473*60*2; i++){
    		UARTCharPut(UART7_BASE,'F');

    }
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14);

	SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec
    i = 0;
    while (1)
    {
    	//checks to see if the final packet was found (if we print count everytime it messes up
    	//You can also check count by using debug.
    	if(count >=10473*60*2)
    		printf("%lld\n", count);

    }

}

void Init(){
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED


    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);

    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //enable pin for LED PF2

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));

    //UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full
    //UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_FIFO);
    //UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full

    UARTFIFOEnable(UART0_BASE);

    //UARTTxIntModeSet(UART7_BASE, UART_TXINT_MODE_FIFO);

    UARTFIFOEnable(UART7_BASE);

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
    UARTEnable(UART0_BASE);
    IntEnable(INT_UART7); //enable the UART interrupt
    UARTIntEnable(UART7_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
    UARTEnable(UART7_BASE);
}

void PutString(char* string){
	uint64_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPutNonBlocking(UART0_BASE, string[i]);
	}
}

uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

