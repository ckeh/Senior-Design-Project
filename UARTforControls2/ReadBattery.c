#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "ReadBattery.h"

#define DWT_O_CYCCNT 0x00000004
#define SAMPLE 5
#define AMP_OVER_BITS 0.0109
#define START_CAPACITY 12000

	static uint32_t ui32ADC0Value[4];
	static volatile uint32_t ui32TempAvg;
	static volatile uint32_t ui32OldAvg;
	static volatile uint32_t ui32CurrVal;
	static volatile double currentConsumption;
	static volatile double batteryCapacity;


void ADC0IntHandler(void){
	ROM_ADCIntClear(ADC0_BASE,1);

	ui32TempAvg = 0;

	ROM_ADCSequenceDataGet(ADC0_BASE,1,ui32ADC0Value);
	ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

}

void GPIOInit(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//GPIO port
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	//Pushbutton
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);	//PD6 is GPIO

	//LED Test
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

void ADCInit(void){
//	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH4); //ADC_CTL_CH0 is PE3
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH4); //ADC_CTL_CH4 is PD3
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH4);

	ROM_ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH4|ADC_CTL_IE|ADC_CTL_END);

	ROM_ADCSequenceEnable(ADC0_BASE, 1);

	ROM_ADCIntClear(ADC0_BASE,1);
	ROM_ADCIntEnable(ADC0_BASE,1);
	ROM_IntEnable(INT_ADC0SS1);

	ROM_ADCIntClear(ADC0_BASE,1);
}

uint32_t ReadADC(void)
{
		//ADCIntClear(ADC0_BASE, 1);

		ROM_ADCProcessorTrigger(ADC0_BASE, 1);
		volatile int32_t status = status = ADCIntStatus(ADC0_BASE,1,false);

		if(ui32TempAvg){
			ui32OldAvg = ui32TempAvg;
			return ui32OldAvg;
		}
		else return ui32OldAvg;
}

void FlashWrite(uint32_t data){
	uint32_t pui32Data[2];
	pui32Data[0] = data;
	pui32Data[1] = 0x56789abc;

	FlashErase(0x10000);
	FlashProgram(pui32Data,0x10000,sizeof(pui32Data));
	//SysCtlDelay(20);
}

uint32_t FlashRead(int flag){
	uint32_t pui32Read;

	if(flag) return 0;

	pui32Read = *(uint32_t*)0x10000;

	return pui32Read;
}

void FlashReset(void){
	FlashErase(0x10000);
}

void openGate(void){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);
}

void closeGate(){
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00);
}

uint8_t convertBat(int bat){
	uint8_t temp = 0;
	temp = (100 - (((float)bat/3276) * 100));	//3276 is 80% of 4096, which is 80% of the battery capacity
	return temp;
}

int buttonCheck(){
	static uint8_t ledVal = 2;

	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0x00){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ledVal);
		return 1;
	}
	else {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
		return 0;
	}
}
