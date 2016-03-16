/*
 * uart.c
 *
 *  Created on: Feb 18, 2016
 *      Author: ckeh
 */

#include "uart.h"
#include "servoControl.h"


void uartInit(){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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

	//ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //enable pin for LED PF2

	ROM_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));
	ROM_UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));

    //UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full
    //UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_FIFO);
    //UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full

	ROM_UARTFIFOEnable(UART0_BASE);

    //UARTTxIntModeSet(UART7_BASE, UART_TXINT_MODE_FIFO);

	ROM_UARTFIFOEnable(UART7_BASE);

//	ROM_IntMasterEnable(); //enable processor interrupts

	ROM_IntEnable(INT_UART0); //enable the UART interrupt
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	ROM_UARTEnable(UART0_BASE);
	ROM_IntEnable(INT_UART7); //enable the UART interrupt
	ROM_UARTIntEnable(UART7_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	ROM_UARTEnable(UART7_BASE);

	ROM_IntPrioritySet(INT_UART0, 0x00);
	ROM_IntPrioritySet(INT_UART7, 0x00);
}

void PutString(uint32_t uartbase, char* string){
	uint32_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPut(uartbase, string[i]);
	}
}
void PutStringNonBlocking(char* string){
	uint32_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPutNonBlocking(UART0_BASE, string[i]);
	}
}
