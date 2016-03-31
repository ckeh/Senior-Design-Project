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
	//if (gpad.Connect("COM9", 115200)) cout << "Serial line opened via COM9" << endl;

	
	win->CreateHandle(hInstance, 800, 600);
	bool res = graph->Init(win->GetHandle());
	if (!res) {
		return -1;
	}

	ShowWindow(win->GetHandle(), nCmdShow);

	MSG msg;
	msg.message = WM_NULL;
	
	graph3d->Init3D(win->GetHandle());
	ID2D1LinearGradientBrush *lBrush = graph->CreateLinearGradientBrush(Point2F(700, 300), Point2F(700, 100));

	
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			gpad.Update();
			graph->BeginDraw();
			
			graph->Clear(ColorF(ColorF::WhiteSmoke, 1.f));
			graph->FillRect(RectF(700, 550 - gpad.lt, 750, 550), lBrush);
			graph->ShowPercentage();
			if (gpad.leftDirection == UP) {
				graph->DrawText(
					L"Forward",
					L"Times New Roman",
					14.f,
					RectF(700, 25, 750, 50),
					ColorF(ColorF::Black)
					);
			}
			else {
				graph->DrawText(
					L"Reverse",
					L"Times New Roman",
					14.f,
					RectF(700, 25, 750, 50),
					ColorF(ColorF::Black)
					);
			}
			graph->EndDraw();
			/*gpad.Send();
			WriteFile((gpad._port), &header, sizeof (unsigned char), &gpad.bytes_written, NULL);
			WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);	*/
		}
	}

	graph->BrushRelease(lBrush);
	return 0;
}