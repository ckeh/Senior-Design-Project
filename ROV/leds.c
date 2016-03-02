/*
 * leds.c
 *
 *  Created on: Feb 16, 2016
 *      Author: luis gonzalez
 */

#include "leds.h"
#include "uart.h" //so can use putString()

/*****************************************************
 * @Function ledsInit()
 * @param the pin want servo to be PWM to come from
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief Initliz pin and configure to allow for servo control
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
uint8_t ledsInit(){

	/*PWM period divded by 1000, it is programmed at 55Hz, then get 18.2 mS periods.Dividing by 1000 again gives 18.2 uS times
	 * 83 give 1.5ms
	 *
	 * */
	//ui8Adjust = 75; // so that starts in center
	lightlevel = 1;
	brightness = OFF;

	//all the proper pin configurations
	//ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //sets System clock to 40 MHz
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // sets pwm clock to 40Mhz / 64 = 625KHz

	//sets the PWM module
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	//for pwn pins
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	//pin config as PWM output
	ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);

	// PWM generator 1 pwm0 set
	ROM_GPIOPinConfigure(GPIO_PD1_M1PWM1 );


	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / LED_FREQUENCY) - 1; //sets pwm period to 12500 ticks of PWM clock (which is at 625kHz)
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

	//initial pulse width operations
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, brightness); // calc this 938 in notebook
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
	//do we need to enable everytime? -- dont think so
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return 1;
}

/*****************************************************
 * @Function ledsBright()
 * @param pulse width want 1=increase width, 0 decrease width
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/11
 *
 *****************************************************/
uint8_t ledsBright(){

	uint8_t returnVal = SUCCESS;

	//will dim livel
	if(lightlevel>=4){
		lightlevel=0;
	}else lightlevel+=1;


	//OFF
	if(lightlevel==0){
		brightness=OFF;
	}
	//25%brightness
	else if(lightlevel==1){
		brightness = MIN;
	}
	//50%brightness
	else if (lightlevel==2){
		brightness = MIDDLE_MIN;
	}
	//75%brightness
	else if(lightlevel==3){
		brightness = MIDDLE;
	}
	//100%brightness
	else if(lightlevel==4){
		brightness = MAX;
	}
	else returnVal = FAILURE;

	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, brightness); //set percent of pulse width
	//set PWM to output, might not need every chagne of pulse width, (need to verfiy)
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return returnVal;
}

/*****************************************************
 * @Function ledsOff()
 * @param set pulse width to 0 turning off LED
 * @return 1 on SUCCESS 0 on FAILURE
 *
 * @brief This function is a wrapper function that will in desired pulse widht and do necceasyr sys calls
 * 		  to control servo
 * @note
 * @author Luis Gonzalez, 2/16
 *
 *****************************************************/
uint8_t ledsOff(){

	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, OFF); // create #define for this
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	return SUCCESS;
}


uint8_t ledsMax(){
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, MAX_LIMIT); // create #define for this
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);

		return SUCCESS;
}


/*******************************************************************************************************
 *
 * Test Harness
 *
 * ******************************************************************************************************/
#ifdef LEDS_TEST


#include "leds.h"

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

	//brightness = OFF; //set default brightness to OFF

	ledsInit();

	//lightlevel = 1;// start it at 25% pulse width

	PutString("\nWelcome! LED test harness\n");
	uint8_t localdata;

	while(1){
		localdata = data;
		if(localdata =='w'){
			PutString("bright");
			ledsBright();
			//resets data to empty
			localdata=' ';
			data = ' ';
		}
		else if(data=='s'){
			PutString("Dim");
			//ledsDim();
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}
		else if(data=='z'){
			PutString("turn Off");
			ledsOff();
			//resets data to empty
			localdata = ' ';
			data = ' ';
		}

	}

}


void UART_Init(){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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

//void PutString(char* string){
//	uint64_t i;
//	for (i=0; i < strlen(string); i++){
//		UARTCharPut(UART0_BASE, string[i]);
//	}
//}

#endif








