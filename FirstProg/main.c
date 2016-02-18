/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"


int main(void) {
	uint32_t ui32Period;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	//Timer at 10Hz and 50% duty cycle
	ui32Period = (SysCtlClockGet() / 2) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_TIMER0A); // enables timer 0a interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //configures when interrupts will be generated
	IntMasterEnable(); //enable interrupts

	TimerEnable(TIMER0_BASE, TIMER_A);
	/* turns on and off an lED with different colors
	uint8_t ui8PinData = 2;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	while(1){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
		SysCtlDelay(2000000);
		if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
	}
	 */
	return 0;
}

void Timer0IntHandler(void){
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	// Read the current state of the GPIO pin and
	// write back the opposite state
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
}
