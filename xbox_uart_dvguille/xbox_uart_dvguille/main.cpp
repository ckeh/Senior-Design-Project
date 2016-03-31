#include "includes.h"

using namespace std;
using namespace System;
using namespace System::IO::Ports;
using namespace D2D1;


#pragma comment (lib,"XInput9_1_0.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "DWrite.lib")
#pragma comment (lib, "d3d11.lib")

float y = 0.0f;
float previousX = 0.f;
float previousY = 0.f;
float x = 0.0f;
float angle = 0.0f;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (uMsg) {
		case WM_DESTROY:  
			PostQuitMessage(0); 
			return 0; 

		case WM_MOUSEMOVE:
			y = (float)GET_Y_LPARAM(lParam);
			x = (float)GET_X_LPARAM(lParam);
			if (x > previousX)
				++angle;
			else
				--angle;
			previousX = x;
			break;

		case WM_RBUTTONDOWN:
			angle++;
			break;

		default:
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pCmdLine, int nCmdShow) {

	auto win = make_shared<window>(hInstance, WindowProc);
	auto graph = make_shared<Graphics>();
	auto graph3d = make_shared <Graphics3D>();
	XboxController gpad;
	unsigned char header = 0xAA;
	
	//serial::open();
	//if (gpad.On()) cout << "Controller " << gpad._id + 1 << " is connected." << endl;
	//if (gpad.Connect("COM9", 115200)) cout << "Serial line opened via COM9" << endl;
	//else cout << "Could not open a serial line" << endl;
	
	win->CreateHandle(hInstance, 800, 600);
	bool res = graph->Init(win->GetHandle());
	if (!res) {
		return -1;
	}

	ShowWindow(win->GetHandle(), nCmdShow);

	MSG msg;
	msg.message = WM_NULL;
	
	graph3d->Init3D(win->GetHandle());
	ID2D1SolidColorBrush *brush = graph->CreateBrush(ColorF(0.9f, 0.1f, 0.2f, 1.f));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			gpad.Update();
			graph->BeginDraw();
			graph->Clear(ColorF(ColorF::AliceBlue, 1.f));
			graph->DisplayGradient(Point2F(200, 300), Point2F(200, 100));
			graph->EndDraw();
			/*gpad.Send();
			gpad.Update();
			WriteFile((gpad._port), &header, sizeof (unsigned char), &gpad.bytes_written, NULL);
			WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);	*/
		}
	}
	//system("pause");
	graph->BrushRelease(brush);
	return 0;
}