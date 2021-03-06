#include "XboxController.h"
#include <math.h>
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
	LPDCB port = new DCB();
	bool result = FALSE;

	_port = CreateFile(
		portname, 
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_FLAG_WRITE_THROUGH, 
		NULL
		);
	if (&_port == INVALID_HANDLE_VALUE) {
		CloseHandle(_port);
		_port = NULL;
		return FALSE;
	}
	SetCommState(_port, port);
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
	port->Parity = NOPARITY;
	port->StopBits = ONESTOPBIT;

	if (!SetCommState(_port, port)) {
		CloseHandle(this->_port);
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
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) >> 6;//2;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) >> 7;//3;
	buttontemp |= (_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) << 1;

	buttons = buttontemp;

	// This block sets the trigger data into a single byte.
	float trigtemp{ 0 }, rotationTempR{ 0 }, rotationTempL{ 0 };
	trigtemp = rotationTempR = _state.Gamepad.bRightTrigger;
	rtemp = (trigtemp / 255) * 15;
	rz = (trigtemp / 255) * 250;
	trigtemp = rotationTempL = _state.Gamepad.bLeftTrigger;
	ltemp = (trigtemp / 255) * 15;
	lz = (trigtemp / 255) * 250;
	triggers = rtemp;
	triggers |= (ltemp << 4);

	rotation = 0;
	if (rotationTempR) rotation = (rotationTempR / 255) * 180;
	else if (rotationTempL) rotation = -(rotationTempL / 255) * 180;

	//This block sets the thumbstick data and check for deadzones in the controller
	float LY = _state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LY*LY);

	//determine the direction the controller is pushed
	float normalizedLY = LY / magnitude;


	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	}
	else {		//if the controller is in the deadzone zero out the magnitude
		magnitude = 0.0;
		lstick = 0;
	}
	unsigned char x = (magnitude / 24827) * 15;
	if (normalizedLY == 1) { 
		lstick = x; 
		leftDirection = UP;
	}
	else { 
		lstick = (x << 4); 
		leftDirection = DOWN;
		
	}
	lt = (magnitude / 24827) * 250;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

	float RX = _state.Gamepad.sThumbRX;

	//determine how far the controller is pushed
	float magnituder = sqrt(RX*RX);

	//determine the direction the controller is pushed
	float normalizedRX = RX / magnituder;


	//check if the controller is outside a circular dead zone
	if (magnituder > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		//clip the magnitude at its expected maximum value
		if (magnituder > 32767) magnituder = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnituder -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	}
	else {		//if the controller is in the deadzone zero out the magnitude
		magnituder = 0.0;
		rstick = 0;
	}
	unsigned char y = (magnituder / (32767- XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) * 15;
	if (normalizedRX == 1)
	{
		rstick = y;
		rightDirection = RIGHT;
	}
	else
	{
		rstick = (y << 4);
		rightDirection = LEFT;
	}
	rt = (magnituder / 24078) * 250;
	//std::cout << (unsigned short)rstick << std::endl;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//CKEH EDIT
	unsigned char first, second;



	first = (this->buttons & 0x0F); //dpad
	first |= (this->triggers & 0x0F) << 4; //R trigger

	second = (this->triggers & 0xF0);
	second |= (this->buttons & 0xF0) >> 4; //xyab

	this->total_packet |= (this->buttons);
	this->total_packet |= (this->triggers << 8);
	this->total_packet = (unsigned long)(first);//|= (this->buttons);
	this->total_packet |= second << 8;//(this->triggers << 8);
	this->total_packet |= (this->lstick << 16);
	this->total_packet |= (this->rstick << 24);
	//this->total_packet |= (this->header);


}

int XboxController::DataAvailable() {
	if (_port == nullptr) return -1;

	DWORD errorflags = 0;
	COMSTAT comstat;

	ClearCommError(_port, &errorflags, &comstat);

	return (int)comstat.cbInQue;
}

