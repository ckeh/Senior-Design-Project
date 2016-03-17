#pragma once
#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
ref class serial {
	public:
		serial();
		SerialPort^ _serial;
		static void open();
		void close();

	private:
		static void DataReceivedHandler( Object^ sender, SerialDataReceivedEventArgs^ e);
};

