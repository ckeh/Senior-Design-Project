#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <d2d1.h>

#include "XboxController.h"
#include "serial.h"
#include "window.h"
#include "graphics.h"

#using <System.dll>

using namespace std;
using namespace System;
using namespace System::IO::Ports;

using xbox = XboxController;

#pragma comment (lib,"XInput9_1_0.lib")
#pragma comment (lib, "d2d1.lib")

Graphics *graph;

LRESULT _stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }
	
	if (uMsg == WM_PAINT) {
		graph->BeginDraw();
		graph->ClearScreen(0.1f, 0.1f, 0.1f);
		graph->FillCircle(400, 300, 100, 1.0f, 0.0f, 0.0f, 1);
		graph->EndDraw();
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pCmdLine, int nCmdShow) {

	window win(hInstance, WindowProc);
	xbox gpad;
	serial::open();
	unsigned char header = 0xAA;
	
	if (gpad.On()) cout << "Controller " << gpad._id + 1 << " is connected." << endl;
	if (gpad.Connect("COM9", 115200)) cout << "Serial line opened via COM9" << endl;
	else cout << "Could not open a serial line" << endl;
	
	win.CreateHandle(hInstance);

	graph = new Graphics();
	bool res = graph->Init(win.WindowHandle);
	if (!res) {
		delete graph;
		return -1;
	}

	ShowWindow(win.WindowHandle, nCmdShow);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
		//gpad.Send();
		//gpad.Update();
		WriteFile((gpad._port), &header, sizeof (unsigned char), &gpad.bytes_written, NULL);
		WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);	
	}
	system("pause");

	return 0;
}