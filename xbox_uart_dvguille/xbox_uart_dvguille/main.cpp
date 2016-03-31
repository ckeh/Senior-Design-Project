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

	window win(hInstance, WindowProc);
	XboxController gpad;
	//serial::open();
	unsigned char header = 0xAA;
	
	//if (gpad.On()) cout << "Controller " << gpad._id + 1 << " is connected." << endl;
	//if (gpad.Connect("COM9", 115200)) cout << "Serial line opened via COM9" << endl;
	//else cout << "Could not open a serial line" << endl;
	
	win.CreateHandle(hInstance, 800, 600);

	auto graph = make_shared<Graphics>();
	auto graph3d = make_shared <Graphics3D>();
	bool res = graph->Init(win.GetHandle());
	if (!res) {
		return -1;
	}

	ShowWindow(win.GetHandle(), nCmdShow);

	MSG msg;
	msg.message = WM_NULL;
	float initY = 125.f;
	float speedy = .5f;
	
	graph3d->Init3D(win.GetHandle());
	ID2D1SolidColorBrush *brush = graph->CreateBrush(ColorF(0.9f, 0.1f, 0.2f, 1.f));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			gpad.Update();
			graph->BeginDraw();
			graph->Clear(ColorF(1.f, 1.f, 1.f));
			graph->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(200, 200)));
			graph->FillRect(RectF(100.f, 100.f, 300.f, 300.f), ColorF(0.f, 0.f, 0.f, 1.f));
			graph->SetTransform(D2D1::Matrix3x2F::Identity());
			//graph->DrawText(L"Hello", L"Gabriola", 18.0f, RectF(x, y, x+50, y+50), ColorF(0.0f, 0.6f, 0.0f, 1.0f));
			//graph->SetTransform(D2D1::Matrix3x2F::Rotation(-angle, D2D1::Point2F(200, 200)));
			graph->FillCircle(Point2F(200.f, 200.f), 25.f, ColorF(0.9f, 0.9f, 0.9f, 0.75f));

			if (gpad.Direction() == UP)
				graph->FillRect(RectF(600.f, 300.f-gpad.lt, 700.f, 301.f), *brush);
			else
				graph->FillRect(RectF(600.f, 300.f, 700.f, 301.f+gpad.lt), *brush);
			
			speedy += 0.5f;
			initY += speedy;
			if (initY > 275) {
				initY = 275;
				speedy = -speedy;
			}
			//graph->SetTransform(D2D1::Matrix3x2F::Identity());
			
			graph->EndDraw();
			angle = angle + gpad.rstick;
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