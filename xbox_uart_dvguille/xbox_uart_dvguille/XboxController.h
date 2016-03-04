#pragma once
#include <Windows.h>
#include <Xinput.h>


class XboxController
{
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
	unsigned long total_packet;
	unsigned char header {0xAA};


	/* XboxController methods */
	bool On(void);
	bool Connect(LPCSTR portname, DWORD baud);
	void Update();
	void Send();

private:

};

