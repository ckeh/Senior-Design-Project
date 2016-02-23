#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "ReadBattery.h"

	static uint32_t ui32ADC0Value[4];
	static volatile uint32_t ui32VoltAve;
	static volatile uint32_t ui32TempAvg;
	static volatile uint32_t ui32TempValueC;
	static volatile uint32_t ui32TempValueF;

void SetADC(void){
//	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0); //ADC_CTL_CH0 is PE3
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);

	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 1);
}

uint32_t ReadADC(void)
{
		ADCIntClear(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC0_BASE, 1);

		//Replace with interrupt
		while(!ADCIntStatus(ADC0_BASE, 1, false)){
		}


		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
		ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;

		return ui32TempAvg;
}
