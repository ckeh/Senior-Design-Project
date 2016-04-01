#include "serial.h"
#using <System.dll>
#include <vcclr.h>

using namespace System;
using namespace System::IO::Ports;

serial::serial() {}

void serial::open() {
	SerialPort^ _serial = gcnew SerialPort("COM4");

	_serial->BaudRate = 115200;
	_serial->Parity = Parity::Even;
	_serial->StopBits = StopBits::One;
	_serial->DataBits = 8;
	_serial->Handshake = Handshake::None;
	_serial->RtsEnable = false;

	_serial->DataReceived += gcnew SerialDataReceivedEventHandler(DataReceivedHandler);
	_serial->Open();
	//Console::WriteLine("Press any key to continue...");
	//Console::WriteLine();
	//Console::Read();
	
	_serial->Close();

}

void serial::close() {
	_serial->Close();
}

void serial::DataReceivedHandler(Object ^ sender, SerialDataReceivedEventArgs ^ e) {
	SerialPort^ sp = (SerialPort^)sender;
	String^ indata = sp->ReadExisting();
	pin_ptr<const wchar_t> data = PtrToStringChars(indata);

}
