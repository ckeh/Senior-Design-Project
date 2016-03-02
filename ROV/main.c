
#include "leds.h"
#include "servoControl.h"
#include "controlScheme.h"
#include "uart.h"
#include "i2c.h"
#include "accelerometer.h"
#include "motors.h"
#include "Timer.h"
#include <stdbool.h>



//Header for controls not needed since only one byte of data
//#define HEADER						0xAA

uint8_t SetBitCount(uint8_t);

//Store incoming data to be processed and packeted
static volatile uint8_t data[64];
static volatile uint8_t count=0;
static volatile Command command;

volatile accelerometer accel;

void Timer0IntHandler(){
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//accelerometer_data_get(&accel);
//	printf ("x0 = %d\n", accel.xg0);
//	printf ("x1 = %d\n", accel.xg1);
//	printf ("y0 = %d\n", accel.yg0);
//	printf ("y1 = %d\n", accel.yg1);
//	printf ("z0 = %d\n", accel.zg0);
//	printf ("z1 = %d\n", accel.zg1);


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
			temp = UARTCharGetNonBlocking(UART7_BASE);
				if (SetBitCount(temp)!=1){
					if(temp  == STOP_ALL){
//						PutString("Stop\n\r");
						command.stop = true;
//						servoSetCenter();
//						motorStop(MOTOR_1);
//						motorStop(MOTOR_2);
					}else if((temp & RMOTOR_2BITS) == LIGHTS_TOGGLE){
//						PutString("Lights\n\r");
						command.lights = true;
//						ledsBright();
					} else {
						command.badcom = true;
//						PutString("Bad Command\n\r");
					}
				} else {
					if ((temp & CAMERA_2BITS) == CAMERA_UP){
						command.cameraup = true;
//						PutString("Camera up\n\r");
//						servoSetPulseWidth(1);
					}else if((temp & CAMERA_2BITS) == CAMERA_DOWN){
						command.cameradown = true;
//						PutString("Camera Down\n\r");
//						servoSetPulseWidth(0);
					}else if((temp & LMOTOR_2BITS) == LMOTOR_UP){
						command.leftup = true;
//						PutString("Left motor up\n\r");
//						motorsSetPulseWidth(MOTOR_1,1);
					}else if((temp & LMOTOR_2BITS) == LMOTOR_DOWN){
						command.leftdown = true;
//						PutString("Left motor down\n\r");
//						motorsSetPulseWidth(MOTOR_1,-1);
					}else if((temp & RMOTOR_2BITS) == RMOTOR_UP){
						command.rightup = true;
//						PutString("Right motor up\n\r");
//						motorsSetPulseWidth(MOTOR_2,1);
					}else if((temp & RMOTOR_2BITS) == RMOTOR_DOWN){
						command.rightdown = true;
//						PutString("Right motor down\n\r");
//						motorsSetPulseWidth(MOTOR_2,-1);
					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_UP){
						command.zup = true;
//						PutString("Z motor up\n\r");
					}else if((temp & ZMOTOR_2BITS) == ZMOTOR_DOWN){
						command.zdown = true;
//						PutString("Z motor down\n\r");
					}
				}
		}
	}

}

int main(void) {
	uartInit();
	servoInit();
	ledsInit();
	motorsInit();
	//timerInit();

//	initialize_i2c();
//	initialize_accelerometer();

    uint8_t i;

    SysCtlDelay(SysCtlClockGet() / (1000 * 3)); //delay ~1 msec


    while (1) //let interrupt handler do the UART echo function
    {
    	if(command.badcom){
    		//THIS SHOULDN'T HAPPEN
    		command.badcom = false;
    	}
    	if(command.cameradown==true){
    		servoSetPulseWidth(0);
    		command.cameradown = false;
    	}
    	if(command.cameraup==true){
    		servoSetPulseWidth(1);
    		command.cameraup = false;
    	}
    	if(command.leftdown){
    		motorsSetPulseWidth(MOTOR_1,-1);
    		command.leftdown = false;
    	}
    	if(command.leftup){
    		motorsSetPulseWidth(MOTOR_1,1);
    		command.leftup = false;
    	}
    	if(command.rightdown){
    		motorsSetPulseWidth(MOTOR_2,-1);
    		command.rightdown = false;
    	}
    	if(command.rightup){
    		motorsSetPulseWidth(MOTOR_2,1);
    		command.rightup = false;
    	}
    	if(command.zdown){
    		motorsSetPulseWidth(MOTOR_Z,-1);
    		command.zdown = false;
    	}
    	if(command.zup){
    		motorsSetPulseWidth(MOTOR_Z,1);
    		command.zup = false;
    	}
    	if(command.stop){
    		servoSetCenter();
    		motorStop(MOTOR_1);
    		motorStop(MOTOR_2);
    		command.stop = false;
    	}
    	if(command.lights){
    		ledsBright();
    		command.lights = false;
    	}
    }

}


uint8_t SetBitCount(uint8_t i){
    uint8_t count;
    for (count = 0; i; count++){
    	i &= i-1;
    }
    return count;
}

