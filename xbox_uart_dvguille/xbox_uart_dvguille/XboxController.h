#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

#define UP		(1<<0)
#define DOWN	(1<<1)
#define RIGHT	(1<<2)
#define LEFT	(1<<3)

class XboxController {
	public:
		XboxController();
		~XboxController();
		int _id;
		HANDLE _port;
		XINPUT_STATE _state;
		unsigned long bytes_written;
		// Button, trigger and analog variables.
		unsigned char buttons{ 0 };						// Buttons on 360 also include dpad pushes.
		unsigned char lstick{ 0 }, rstick{ 0 };			// Analog sticks
		unsigned char triggers{ 0 };					// Left and right triggers of the controller
		char leftDirection;
		char rightDirection;
		
		uint8_t lt;
		uint8_t rt;
		uint8_t rstickn;

		unsigned long total_packet;
		unsigned char header {0xAA};


		/* XboxController methods */
		bool On(void);
		bool Connect(LPCSTR portname, DWORD baud);
		void Update();
		void Send();
		char Direction() { return leftDirection; }
		

	private:

};

