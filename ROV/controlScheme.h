/*
 * controlScheme.h
 *
 *  Created on: Feb 18, 2016
 *      Author: ckeh
 */

#ifndef CONTROLSCHEME_H_
#define CONTROLSCHEME_H_

//control scheme
#define LMSU				 		'w'	//left motor speed up
#define RMSU 						'r' //right motor speed up
#define LMSD 						's' //left motor speed down
#define RMSD 						'f' //right motor speed down
#define STOP 						'x'
#define CU 							'i' //Camera up
#define CD			 				'k' //Camera down
#define DIVE 						'd'
#define SURFACE 					'e'
#define LIGHTS						'l'

//Bit Mask to filter controls
#define LMOTOR_2BITS				0xC0
#define RMOTOR_2BITS				0x30
#define ZMOTOR_2BITS				0x0C
#define CAMERA_2BITS				0x03

//Bit Mask to set individual controls
#define CAMERA_UP					0x01
#define CAMERA_DOWN					0x02
#define ZMOTOR_UP					0x04
#define ZMOTOR_DOWN					0x08
#define RMOTOR_UP					0x10
#define RMOTOR_DOWN					0x20
#define LMOTOR_UP					0x40
#define LMOTOR_DOWN					0x80
#define STOP_ALL					0xFF
#define LIGHTS_TOGGLE				0x30


typedef struct Command {
	bool stop;
	bool lights;
	bool badcom;
	bool cameraup;
	bool cameradown;
	bool leftup;
	bool leftdown;
	bool rightup;
	bool rightdown;
	bool zup;
	bool zdown;
} Command;


#endif /* CONTROLSCHEME_H_ */
