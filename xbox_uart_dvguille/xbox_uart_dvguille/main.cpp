#include "includes.h"

#define PACKET_SIZE 80

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

	int16_t xtmp = 0;
	int16_t ytmp = 0;
	int16_t ztmp = 0;

	wchar_t* wcstring;
	int i;

	auto win = make_shared<window>(hInstance, WindowProc);
	auto graph = make_shared<Graphics>();
	auto graph3d = make_shared <Graphics3D>();
	XboxController gpad;
	unsigned char header = 0xAA;
	
	serial::open();
	gpad.Connect("COM5", 115200);
	
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
	wchar_t oldbuf[100] = L"";



	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			int16_t xdata;
			int16_t ydata;
			int16_t zdata;
			uint32_t pressure;
			uint32_t temperature;
			int result;

			char dataprint[100];
			char dataprintx[20];
			char dataprinty[20];
			char dataprintz[20];

			uint8_t buf[100] = { '\0' };
			uint8_t buff[6] = { '\0' };
			char headerr[1] = { '\0' };
			
			DWORD bytes = 0;
			int temp = gpad.DataAvailable();
			if (temp >= PACKET_SIZE+1) {
				xdata = 0;
				ydata = 0;
				zdata = 0;
				pressure = 0;
				temperature = 0;
				result = 0;
				dataprint[100] = { '\0' };
				dataprintx[100] = { '\0' };
				dataprinty[100] = { '\0' };
				dataprintz[100] = { '\0' };
				ReadFile(gpad._port, headerr, 1, &bytes, NULL);
				if (headerr[0] == 'a') {
					ReadFile(gpad._port, &buf, PACKET_SIZE/8, &bytes, NULL);
				}


					for (int i = 0; i < 2; i++) {
						xdata |= buf[i];
						if (i == 0)
							xdata = xdata << 8;
					}
					sprintf(dataprintx, "x = %d", xdata);

					for (int i = 2; i < 6; i++) {
						pressure |= buf[i];
						if (i != 5)
							pressure = pressure << 8;
					}

					for (int i = 6; i < 10; i++) {
						temperature |= buf[i];
						if (i != 9)
							temperature = temperature << 8;
					}

					result = calcPressure(pressure, temperature) / 10;
					sprintf(dataprint, "press = %d mbar", result);
#if 0
					for (int i = 3; i > 1; i--) {
						ydata |= buff[i];
						if (i == 3)
							ydata = ydata << 8;
					}

					for (int i = 5; i > 3; i--) {
						zdata |= buff[i];
						if (i == 5)
							zdata = zdata << 8;
					}

					sprintf(dataprintx, "x = %d", xdata);
					sprintf(dataprinty, "y = %d", ydata);
					sprintf(dataprintz, "z = %d", zdata);
#endif	
					headerr[0] = '\0';
				}
			
		
			wcstring = convert(dataprint);

			gpad.Update();
			graph->BeginDraw();
			graph->Clear(ColorF(ColorF::WhiteSmoke, 1.f));
			graph->SetTransform(Matrix3x2F::Rotation(xdata, D2D1::Point2F(100, 150)));
			sBrush->SetColor(ColorF(ColorF::DodgerBlue));
			graph->FillTriangle(sBrush);
			graph->SetTransform(Matrix3x2F::Identity());
			sBrush->SetColor(ColorF(ColorF::Black));
			graph->DrawCircle(Point2F(100.0f, 150.0f), 75.0f, sBrush);
			graph->FillRect(RectF(700, 50, 750, 550), rBrush);
			graph->FillRect(RectF(700, 550 - gpad.lt, 750, 550), lBrush);
			graph->DrawText(wcstring, L"Times New Roman", 14.0f, RectF(400, 200, 600, 250), ColorF(ColorF::Black));

			//if (wcscmp(oldbuf, wcstring) != 0) {
				//graph->DrawText(wcstring, L"Times New Roman", 14.0f, RectF(400, 200, 600, 250), ColorF(ColorF::Black));
#if 0
				for (int i = 0; i < sizeof(buff); i++) {
					buff[i] = '\0';
				}

				
				for (int i = 0; i < sizeof(wcstring); i++) {
					wcstring[i] = '\0';
				}
				
#endif
			//}
			//for (int i = 0; i < sizeof(wcstring); i++) {
			//	oldbuf[i] = wcstring[i];
			//}

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
			//gpad.Send();
			
			WriteFile((gpad._port), &header, sizeof (unsigned char), &gpad.bytes_written, NULL);
			WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);

			delete[] wcstring;
		}
	}

	graph->BrushRelease(lBrush);
	graph->BrushRelease(rBrush);
	return 0;
}