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
	
	serial::open();
	gpad.Connect("COM4", 115200);
	
	win->CreateHandle(hInstance, 800, 600);
	bool res = graph->Init(win->GetHandle());
	if (!res) {
		return -1;
	}

	ShowWindow(win->GetHandle(), nCmdShow);

	MSG msg;
	msg.message = WM_NULL;
	
	graph3d->Init3D(win->GetHandle());
	ID2D1Brush *lBrush = graph->CreateLinearGradientBrush(Point2F(700, 300), Point2F(700, 100));
	ID2D1Brush *rBrush = graph->CreateRadialGradientBrush(Point2F(725, 300), Point2F(0, 0));
	ID2D1SolidColorBrush *sBrush = graph->CreateSolidColorBrush(ColorF(ColorF::DodgerBlue));
	graph->CreateSink();
	//wchar_t oldbuf[64] = L"";
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			char buf[64];
			DWORD bytes = 0;
			ReadFile(gpad._port, buf, sizeof(buf), &bytes, NULL);
			//size_t length = strlen(buf);
			//std::wstring text_wchar(length, L'#');
			//mbstowcs(&text_wchar[0], buf, length);
			//const wchar_t *wbuf = text_wchar.c_str();
			gpad.Update();
			graph->BeginDraw();
			graph->Clear(ColorF(ColorF::WhiteSmoke, 1.f));
			graph->SetTransform(Matrix3x2F::Rotation(gpad.rotation, D2D1::Point2F(100, 150)));
			sBrush->SetColor(ColorF(ColorF::DodgerBlue));
			graph->FillTriangle(sBrush);
			graph->SetTransform(Matrix3x2F::Identity());
			sBrush->SetColor(ColorF(ColorF::Black));
			graph->DrawCircle(Point2F(100.0f, 150.0f), 75.0f, sBrush);
			graph->FillRect(RectF(700, 50, 750, 550), rBrush);
			graph->FillRect(RectF(700, 550 - gpad.lt, 750, 550), lBrush);

			//if (wcscmp(oldbuf, wbuf) == 0)
				//graph->DrawText((wchar_t*)buf, L"Times New Roman", 14.0f, RectF(400, 200, 600, 250), ColorF(ColorF::Black));
			//wcscpy(oldbuf, wbuf);

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
	graph->BrushRelease(rBrush);
	return 0;
}