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
	//ui32Period = 2000000; //2000000/40000000 = .05s
	//ui32Period = 20000000; //2000000/40000000 = .5s
	ui32Period = 8000000; //2000000/40000000 = .2s
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	ROM_IntEnable(INT_TIMER0A); // enables timer 0a interrupt
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //configures when interrupts will be generated
	//IntMasterEnable(); //enable interrupts

	ROM_IntPrioritySet(INT_TIMER0A, 0xE0);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	return 1;

}

uint8_t timerInit2(){
	uint32_t ui32Period;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	//Timer at 10Hz and 50% duty cycle
	//ui32Period = 2000000; //2000000/40000000 = .05s
	//ui32Period = 20000000; //2000000/40000000 = .5s
	ui32Period = 40000000; //2000000/40000000 = .2s
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);

	ROM_IntEnable(INT_TIMER1A); // enables timer 0a interrupt
	ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); //configures when interrupts will be generated
	//IntMasterEnable(); //enable interrupts

	//ROM_IntPrioritySet(INT_TIMER1B, 0xE0);
	ROM_TimerEnable(TIMER1_BASE, TIMER_A);
	return 1;




}

