/*
 * Timer.c
 *
 *  Created on: Feb 25, 2016
 *      Author: ckeh
 */
#include "Timer.h"
//#include "servoControl.h"

uint8_t timerInit(){
	uint32_t ui32Period;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

	//Timer at 10Hz and 50% duty cycle
	ui32Period = 2000000;//(SysCtlClockGet() /2) / 2;
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	ROM_IntEnable(INT_TIMER0A); // enables timer 0a interrupt
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //configures when interrupts will be generated
	//IntMasterEnable(); //enable interrupts

	ROM_IntPrioritySet(INT_TIMER0A, 0x20);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	return 1;

}

