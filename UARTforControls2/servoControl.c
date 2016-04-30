/*
 * servoControl.c
 *
 *  Created on: Feb 11, 2016
 *      Author: luis gonzalez
 */

#include "servoControl.h"




/*****************************************************
 * @Function servoInit()
 * @param the pin want servo to be PWM to come from
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initliz pin and configure to allow for servo control
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoInit(){

	/*PWM period divded by 1000, it is programmed at 55Hz, then get 18.2 mS periods.Dividing by 1000 again gives 18.2 uS times
	 * 83 give 1.5ms
	 *
	 * */
	ui8Adjust = 75; // so that starts in center

	//all the proper pin configurations
	//ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //sets System clock to 40 MHz
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // sets pwm clock to 40Mhz / 64 = 625KHz

	//sets the PWM module
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	//for pwn pins
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	//pin config as PWM output
	ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);

	// PWM generator 1 pwm0 set
	ROM_GPIOPinConfigure(GPIO_PD0_M1PWM0);


	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1; //sets pwm period to 12500 ticks of PWM clock (which is at 625kHz)
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

	//initial pulse width operations
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000); // calc this 938 in notebook
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	//do we need to enable everytime? -- dont think so
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return 1;
}

/*****************************************************
 * @Function servoSetPulseWidth()
 * @param pulse width want 1=increase width, 0 decrease width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoSetPulseWidth(uint8_t var){

	uint8_t returnVal = SUCCESS;

	//our resolution is between 1 and 83, can make that many jumps, which is pulses of 11.3mS to 931ms, with jumps of 11.362ms
	if(var==0){
		//this will not let servo go below 1ms pulse
		if(ui8Adjust <= SERVO_MIN_PULSE) ui8Adjust = SERVO_MIN_PULSE;
		else ui8Adjust-=1;
	}
	else if(var>=1){
		//this will not let servo pulse go above 2ms pulse
		if(ui8Adjust>=SERVO_MAX_PULSE){
			ui8Adjust=SERVO_MAX_PULSE;
		}else{
			ui8Adjust+=1;
		}

	}
	// if somehow get invalud parameter
	else{

		returnVal = FAILURE; //if dont enter valid param, return Failer
	}

	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000); //set percent of pulse width
	//set PWM to output, might not need every chagne of pulse width, (need to verfiy)
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return returnVal;
}

/*****************************************************
 * @Function servoSetMax()
 * @param pulse width want 0-100
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoSetMax(){
	//our resolution is between 1 and 83, can make that many jumps, which is pulses of 11.3mS to 931ms, with jumps of 11.362ms
	ui8Adjust = SERVO_MAX_PULSE;

	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return SUCCESS;
}
/*****************************************************
 * @Function servoSetMin()
 * @param pulse width want 0-100
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoSetMin(){

	ui8Adjust = SERVO_MIN_PULSE;
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return SUCCESS;
}

/*****************************************************
 * @Function servoSetCenter()
 * @param pulse width want 0-100
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t servoSetCenter(){

	ui8Adjust = 74; //
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000); // width is defined as # of PWM clock ticks
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return SUCCESS;
}



/*******************************************************************************************************
 *
 * Test Harness
 *
 * ******************************************************************************************************/
#ifdef SERVO_TEST


#include "servoControl.h"

#define PULSE_WIDTH_CCW_JUMP 50;
#define PULSE WIDTH_CW_JUMP 50;

static volatile uint8_t data = 0;

//For the USB UART connection to get controls from the pc
void UART0IntHandler(void)
{
	uint8_t d;
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
	UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

	//If interrupt is from RX or receive timeout
	while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
	{
		d = UARTCharGetNonBlocking(UART0_BASE);
		data = d; // store in global in main will adjust servo accordingly
		UARTCharPut(UART0_BASE,data);
	}

}


int main(void){


	UART_Init();
	servoInit(); //sets PD0 for pwm output

	//PutString("Welcome! possible inputs w,s,z,m,c");
	uint8_t localdata;

	while(1){
		localdata = data;
		if(localdata =='w'){
			PutString("up");
			servoSetPulseWidth(1);
			//resets data to empty
			localdata=' ';
			data = ' ';
		}
		else if(data=='s'){
			PutString("down");
			servoSetPulseWidth(0);
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='z'){
			PutString("reset MIN");
			servoSetMax();
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='m'){
			PutString("reset to MAX");
			servoSetMin();
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='c'){
			PutString("re-center");
			servoSetCenter();
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
	}

}


void UART_Init(){
	//ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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

void PutString(char* string){
	uint64_t i;
	for (i=0; i < strlen(string); i++){
		UARTCharPut(UART0_BASE, string[i]);
	}
}

#endif






