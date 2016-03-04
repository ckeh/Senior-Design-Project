#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

#include "XboxController.h"
#define CLASS
using namespace std;
using xbox = XboxController;
#pragma comment (lib,"XInput9_1_0.lib")


int main(int argc, char *argv[]) {
#ifdef CLASS
	xbox gpad;
	unsigned char header = 0xAA;
	if (gpad.On()) cout << "Controller " << gpad._id + 1 << " is connected." << endl;
	if (gpad.Connect("COM9", 115200)) cout << "Serial line opened via COM9" << endl;
	else cout << "Could not open a serial line" << endl;
	while (1) {
		//gpad.Send();
		gpad.Update();
		WriteFile((gpad._port), &header, sizeof (unsigned char), &gpad.bytes_written, NULL);
		WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);
		cout << gpad.total_packet << endl;
		Sleep(200);
	}
#endif
#ifndef CLASS
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
	DCB cport;
	HANDLE port{ 0 };
	bool result = FALSE;
	COMMTIMEOUTS tout;
	//LPCSTR commString = "COM3";
	port = CreateFile("COM9", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
	cport.DCBlength = sizeof(DCB);				// Setup config length
	GetCommState(&port, &cport);				// Get default port state
	cport.BaudRate = 115200;					// Set baud rate
	cport.fBinary = TRUE;						// Enable Binary mode
	cport.fParity = TRUE;						// Disable parity 
	cport.fOutxCtsFlow = FALSE;					// No CTS 
	cport.fOutxDsrFlow = FALSE;					// No DSR
	cport.fDtrControl = DTR_CONTROL_DISABLE;	// No DTR
	cport.fDsrSensitivity = FALSE;				// No DSR sensitivity 
	cport.fTXContinueOnXoff = TRUE;				// TX on XOFF
	cport.fOutX = FALSE;						// No XON/XOFF
	cport.fInX = FALSE;							//
	cport.fErrorChar = FALSE;					// No error correction
	cport.fNull = FALSE;						// Keep NULL values
	cport.fRtsControl = RTS_CONTROL_DISABLE;	// Disable RTS
	cport.fAbortOnError = FALSE;				// Disable abort-on-error
	cport.ByteSize = 8;							// 8-bit frames
	cport.Parity = EVENPARITY;					// Parity: none
	cport.StopBits = ONESTOPBIT;				// StopBits: 1
	SetCommState(port, &cport);
	DWORD bytesTrans;
	cout << &port << endl;
	char buf[64];
	int temp = 0;
	while (1) {
		//XInputGetState(0, &pad);
		//cout << (bool)(pad.Gamepad.wButtons&XINPUT_GAMEPAD_A) << endl;
		//WriteFile(gpad._port, "1", strlen("1"), NULL, NULL);
		//ReadFile(gpad._port, buf, (DWORD)strlen("1"), NULL, NULL);
		XInputGetState(0, &state);
		int a = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
		cout << a << endl;
		char b = a >> 12;
			WriteFile(port, &b, 1, &bytesTrans, NULL);
		temp = a;
		Sleep(100);
	}
#endif
	system("pause");

	return 0;
}