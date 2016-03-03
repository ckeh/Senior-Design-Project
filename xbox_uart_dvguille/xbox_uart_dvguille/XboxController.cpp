#include "XboxController.h"
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <string.h>
#include <iostream>

XboxController::XboxController() {
	_port = NULL;
	_id = -1;

	for (int i = 0; i < 4; ++i) {
		DWORD id = XInputGetState(i, &_state);
		if (id == ERROR_SUCCESS) _id = i;
	}
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
		
}
XboxController::~XboxController() {}

bool XboxController::On(void) {
	DWORD id = XInputGetState(_id, &_state);
	if (id == ERROR_SUCCESS) return true;
	else return false;
}

bool XboxController::Connect(LPCSTR portname, DWORD baud) {
	LPDCB port;
	bool result = FALSE;

	_port = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
	std::cout << &_port << std::endl;
	if (&_port == INVALID_HANDLE_VALUE) {
		CloseHandle(_port);
		_port = NULL;
		return FALSE;
	}

	port->DCBlength = sizeof(DCB);
	GetCommState(_port, port);
	port->BaudRate = baud;
	port->fBinary = TRUE;
	port->fParity = TRUE;
	port->fOutxCtsFlow = FALSE;
	port->fOutxDsrFlow = FALSE;
	port->fDtrControl = DTR_CONTROL_DISABLE;
	port->fDsrSensitivity = FALSE;
	port->fOutX = FALSE;
	port->fInX = FALSE;
	port->fErrorChar = FALSE;
	port->fNull = FALSE;
	port->fRtsControl = RTS_CONTROL_DISABLE;
	port->fAbortOnError = FALSE;
	port->ByteSize = 8;
	port->Parity = EVENPARITY;
	port->StopBits = ONESTOPBIT;

	if (!SetCommState(_port, port)) {
		CloseHandle(_port);
		_port = NULL;
		return FALSE;
	}
	
	return TRUE;
}

void XboxController::Update() {
	total_packet = 0;
	DWORD id = XInputGetState(this->_id, &(this->_state));
	// Variables to be used by multiple blocks to set data;
	unsigned char rtemp, ltemp; 
	// This block sets the dpad and buttons on the controller into the a single byte of data.
	unsigned char buttontemp{ 0 };
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)>>8;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)>>8;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_X)>>8;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)>>8;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	this->buttons = buttontemp;

	// This block sets the trigger data into a single byte.
	float trigtemp{ 0 };
	trigtemp = _state.Gamepad.bRightTrigger;
	rtemp = (trigtemp / 255) * 15;
	trigtemp = _state.Gamepad.bLeftTrigger;
	ltemp = (trigtemp / 255) * 15;
	this->triggers = rtemp;
	this->triggers |= (ltemp << 4);

	//This block sets the thumbstick data
	float thumbtemp{ 0 };
	thumbtemp = _state.Gamepad.sThumbLY;
	ltemp = (thumbtemp / 32767) * 15;
	if (ltemp < 16)
		this->lstick = ltemp;
	else
		this->lstick = (ltemp << 4);
	thumbtemp = _state.Gamepad.sThumbRX;
	rtemp = (thumbtemp / 32676) * 15;
	if (rtemp < 16)
		this->rstick = rtemp;
	else
		this->rstick = (rtemp << 4);

	this->total_packet |= (this->buttons);
	this->total_packet |= (this->triggers << 8);
	this->total_packet |= (this->lstick << 16);
	this->total_packet |= (this->rstick << 24);
}

void XboxController::Send() {
	this->Update();
	WriteFile(&_port, &total_packet, 4, &bytes_written, NULL);	

}




