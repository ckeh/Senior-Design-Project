/*
 * motors.c
 *
 *  Created on: Feb 22, 2016
 *      Author: luis gonzalez
 */
#include "motors.h"



uint8_t motorsInit(){
	/*PWM period divded by 1000, it is programmed at 55Hz, then get 18.2 mS periods.Dividing by 1000 again gives 18.2 uS times
	 * 83 give 1.5ms
	 *
	 * */
	ui8Adjust = PULSE_CENTER;
	//init these for calls in setpulsewidth
	ui8PulseAdjust_1 = PULSE_CENTER;
	ui8PulseAdjust_2 = PULSE_CENTER;
	ui8PulseAdjust_3 = PULSE_CENTER;

	//all the proper pin configurations
	//ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //sets System clock to 40 MHz
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // sets pwm clock to 40Mhz / 64 = 625KHz

	//sets the PWM module
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	//for pwn pins
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // motors 1,2
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // motor z

	//pin config as PWM output
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

	// PWM generator 1 pwm0 set
	ROM_GPIOPinConfigure(GPIO_PE4_M0PWM4);
	ROM_GPIOPinConfigure(GPIO_PE5_M0PWM5);
	ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);

	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1; //sets pwm period to 12500 ticks of PWM clock (which is at 625kHz)
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ui32Load);

	//for motor "Z"
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

	//initial pulse width operations
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,ui8Adjust * ui32Load / 1000); // calc this 938 in notebook
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5,ui8Adjust * ui32Load / 1000); // calc this 938 in notebook
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,ui8Adjust * ui32Load / 1000); // calc this 938 in notebook


	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

	//do we need to enable everytime? -- dont think so
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	// motor 'Z'
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	return SUCCESS;
}



uint8_t motorsSetPulseWidth(uint8_t motor, int speedChange){


	uint8_t returnVal = SUCCESS; // will SUCCED unless in wrong bounds

	//will check that speedchange is within bounds and does not exceed MAX and MIN
	if(speedChange < 0){

	}
	else if(speedChange >= 0){

	}

	if(speedChange >= -5 && speedChange <= 5){
		//set var's for pulse width adjustment
		if(motor == MOTOR_1 ){
			if(ui8PulseAdjust_1 <= PULSE_MIN && speedChange < 0) ui8PulseAdjust_1 = PULSE_MIN;
			else if(ui8PulseAdjust_1 <= PULSE_MIN && speedChange >= 0) ui8PulseAdjust_1 += speedChange;

			else if(ui8PulseAdjust_1 >= PULSE_MAX && speedChange >= 0) ui8PulseAdjust_1 = PULSE_MAX;
			else if(ui8PulseAdjust_1 >= PULSE_MAX && speedChange < 0) ui8PulseAdjust_1 +=speedChange;
			//deafulat case to catch a change
			else ui8PulseAdjust_1 += speedChange;
			// set pulse width adjustments here
			ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, ui8PulseAdjust_1 * ui32Load / 1000); //set percent to a pulse width
			ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);

		}

		else if(motor == MOTOR_2 ){
			if(ui8PulseAdjust_2 <= PULSE_MIN && speedChange < 0) ui8PulseAdjust_2 = PULSE_MIN;
			else if(ui8PulseAdjust_2 <= PULSE_MIN && speedChange >= 0) ui8PulseAdjust_2 += speedChange;

			else if(ui8PulseAdjust_2 >= PULSE_MAX && speedChange >= 0) ui8PulseAdjust_2 = PULSE_MAX;
			else if(ui8PulseAdjust_2 >= PULSE_MAX && speedChange < 0) ui8PulseAdjust_2 +=speedChange;
			//deafulat case to catch a change
			else ui8PulseAdjust_2 += speedChange;
			ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ui8PulseAdjust_2 * ui32Load / 1000); //set percent to a pulse width
			ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
		}

		else if(motor == MOTOR_Z  ){
			if(ui8PulseAdjust_3 <= PULSE_MIN && speedChange < 0) ui8PulseAdjust_3 = PULSE_MIN;
			else if(ui8PulseAdjust_3 <= PULSE_MIN && speedChange >= 0) ui8PulseAdjust_3 += speedChange;

			else if(ui8PulseAdjust_3 >= PULSE_MAX && speedChange >= 0) ui8PulseAdjust_3 = PULSE_MAX;
			else if(ui8PulseAdjust_3 >= PULSE_MAX && speedChange < 0) ui8PulseAdjust_3 +=speedChange;
			//deafulat case to catch a change
			else ui8PulseAdjust_3 += speedChange;

			ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8PulseAdjust_3 * ui32Load / 1000); //set percent to a pulse width
			ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
		}

	}

	else{
		returnVal = FAILURE;
	}

	return returnVal;

}



uint8_t motorStop(uint8_t motor){

	if(motor==MOTOR_1){
		ui8PulseAdjust_1 = PULSE_CENTER;
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, ui8PulseAdjust_1 * ui32Load / 1000); //set percent to a pulse width
		//set PWM to output, might not need every chagne of pulse width, (need to verfiy)
		ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	}
	else if(MOTOR_2 == motor){
		ui8PulseAdjust_2 = PULSE_CENTER;
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ui8PulseAdjust_2 * ui32Load / 1000); //set percent to a pulse width
		//set PWM to output, might not need every chagne of pulse width, (need to verfiy)
		ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	}
	else if(MOTOR_Z == motor){
		ui8PulseAdjust_3 = PULSE_CENTER;
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8PulseAdjust_3 * ui32Load / 1000); //set percent to a pulse width
		//set PWM to output, might not need every chagne of pulse width, (need to verfiy)
		ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	}


	return 1; // might want to add failur when not valid call
}

uint8_t motorsVariable (uint8_t motor, int speedChange) {
	uint8_t returnVal = SUCCESS; // will SUCCED unless in wrong bounds

	if(motor == MOTOR_1 ){
		ui8PulseAdjust_1 = (((float)speedChange/15)*18) + 75;
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, ui8PulseAdjust_1 * ui32Load / 1000); //set percent to a pulse width
		ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);

	}

	else if(motor == MOTOR_2 ){
		ui8PulseAdjust_2 = (((float)speedChange/15)*18) + 75;
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, ui8PulseAdjust_2 * ui32Load / 1000); //set percent to a pulse width
		ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	}

	else if(motor == MOTOR_Z  ){
		ui8PulseAdjust_3 = (((float)speedChange/15)*18) + 75;
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8PulseAdjust_3 * ui32Load / 1000); //set percent to a pulse width
		ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	}


		return returnVal;
}

/*******************************************************************************************************
 *
 * Test Harness
 *
 * ******************************************************************************************************/
#ifdef MOTOR_TEST


#include "motors.h"
#include "uart.h" // so can use PutString

static volatile uint8_t data = 0;


//For the USB UART connection to get controls from the pc
void UART0IntHandler(void)
{
	uint8_t d;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	//If interrupt is from RX or receive timeout
	if(ui32Status == UART_INT_RX || ui32Status == UART_INT_RT){
		while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
		{
			d = UARTCharGetNonBlocking(UART0_BASE);
			data = d; // store in global in main will adjust servo accordingly
			UARTCharPut(UART0_BASE,data);
		}
	}

}


int main(void){


	UART_Init();
	motorsInit();

	PutString("\nWelcome! Motors Test Harness\n");
	uint8_t localdata;

	while(1){
		localdata = data;
		if(localdata =='w'){
			PutString("1 up\n");
			motorsSetPulseWidth(MOTOR_1,1);
			//resets data to empty
			localdata=' ';
			data = ' ';
		}
		else if(data=='s'){
			PutString("1 down\n");
			motorsSetPulseWidth(MOTOR_1,-1);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='i'){
			PutString("2 up\n");
			motorsSetPulseWidth(MOTOR_2,1);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='k'){
			PutString("2 down\n");
			motorsSetPulseWidth(MOTOR_2,-1);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='t'){
			PutString("3 up\n");
			motorsSetPulseWidth(MOTOR_Z,1);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='g'){
			PutString("3 down\n");
			motorsSetPulseWidth(MOTOR_Z,-1);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='c'){
			PutString("re-center motor 1");
			motorStop(MOTOR_1);
			motorStop(MOTOR_2);
			motorStop(MOTOR_Z);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
	}

}


void UART_Init(){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);//to 40 MHz

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);

	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	ROM_UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));
	//ROM_UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_EVEN));

    //UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full
    //UARTTxIntModeSet(UART0_BASE, UART_TXINT_MODE_FIFO);
    //UARTFIFOLevelSet(UART7_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8); //set up FIFO to trigger interrupt on 14/16 full

	ROM_UARTFIFOEnable(UART0_BASE);

	ROM_IntMasterEnable(); //enable processor interrupts
	ROM_IntEnable(INT_UART0); //enable the UART interrupt
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_RT ); //only enable RX and TX interrupts
	ROM_UARTEnable(UART0_BASE);
}

//defined in uart.c
//void PutString(char* string){
//	uint64_t i;
//	for (i=0; i < strlen(string); i++){
//		UARTCharPut(UART0_BASE, string[i]);
//	}
//}

#endif
