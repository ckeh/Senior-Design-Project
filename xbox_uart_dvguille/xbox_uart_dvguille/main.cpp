#include "includes.h"
#include <iostream>
#include <fstream>

#define PACKET_SIZE 48 //packet size in bits

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
unsigned long prevControlsPacket = 0;
XboxController gpad; //make it global
unsigned char header = 0xAA; //to make global


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


//the timer set on line 75, want to sent controls in this isr
void CALLBACK f(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	WriteFile((gpad._port), &header, sizeof(unsigned char), &gpad.bytes_written, NULL);
	WriteFile((gpad._port), &(gpad.total_packet), 4, &gpad.bytes_written, NULL);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pCmdLine, int nCmdShow) {
	int count = 0;
	wchar_t* wcstring;
	int i;

	auto win = make_shared<window>(hInstance, WindowProc);
	auto graph = make_shared<Graphics>();
	auto graph3d = make_shared <Graphics3D>();

	SetTimer(NULL, 0, 50, (TIMERPROC)&f); //sets timer to N ms

	gpad.Connect("COM5", 115200);

	win->CreateHandle(hInstance, 800, 600);
	bool res = graph->Init(win->GetHandle());
	if (!res) {
		return -1;
	}
	auto bmp3 = make_shared <Bitmap>(L"Flashlight_clear.JPG", graph);
	float opacity = 0.0;
	ShowWindow(win->GetHandle(), nCmdShow);
	uint8_t lightflag = 0;
	MSG msg;
	msg.message = WM_NULL;

	graph3d->Init3D(win->GetHandle());
	ID2D1LinearGradientBrush *lBrush = graph->CreateLinearGradientBrush(Point2F(700, 300), Point2F(700, 100));
	ID2D1LinearGradientBrush *lBrush2 = graph->CreateLinearGradientBrush2(Point2F(100, 240), Point2F(100, 59));
	ID2D1RadialGradientBrush *rBrush = graph->CreateRadialGradientBrush(Point2F(725, 300), Point2F(0, 0));
	ID2D1SolidColorBrush *sBrush = graph->CreateSolidColorBrush(ColorF(ColorF::DodgerBlue));
	graph->CreateSink();
	graph->CreateSink2();

	wchar_t oldbuf[100] = L"";
	//KENNY CHANGES
	//LG--commented out for now, while debugging
	//ofstream myfile;
	//myfile.open("test.txt");


	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			uint32_t basePressure;
			int8_t xdata;
			int xtmp;
			int8_t ydata;
			float sydata;
			int8_t zdata;
			uint8_t battery;
			uint16_t pressure;
			char dataprint[256];

			uint8_t buf[PACKET_SIZE / 8] = { '\0' };
			char headerr[1] = { '\0' };

			DWORD bytes = 0;
			int temp = gpad.DataAvailable();

			if (temp >= (PACKET_SIZE / 8) + 1) {
				//xdata = 0;
				//ydata = 0;
				//sydata = 0;
				//zdata = 0;
				//pressure = 0;
				//battery = 0;
				//dataprint[256] = { '\0' };

				ReadFile(gpad._port, headerr, 1, &bytes, NULL);
				if (headerr[0] == 'f') {
					xdata = 0;
					ydata = 0;
					sydata = 0;
					zdata = 0;
					pressure = 0;
					battery = 0;
					dataprint[256] = { '\0' };
					ReadFile(gpad._port, &buf, PACKET_SIZE / 8, &bytes, NULL);

					for (int i = 0; i < PACKET_SIZE / 8; i++) {
						if (i < 2) {
							pressure |= buf[i];
							if (i != 1)
								pressure = pressure << 8;
						}
						else if (i < 3) {
							xdata |= buf[i];
							//if (i != 5)
							//	xdata = xdata << 8;
						}
						else if (i < 4) {
							ydata |= buf[i];
							//if (i != 7)
							//	ydata = ydata << 8;
						}
						else if (i < 5) {
							zdata |= buf[i];
							//if (i != 9)
							//	zdata = zdata << 8;
						}
						else {//if (i == 10) {
							battery |= buf[i];
						}
					}


				}

				sydata = (((ydata + 90.0) / (90 + 90)) * (180 + 180)) - 180;
				//((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow;
				xtmp = xdata;
				if (zdata < 0) {
					if (xdata > 0) {
						xtmp = 180 - xtmp;
					}
					else {
						xtmp = -180 - xtmp;
					}
				}
				//pressure = pressure / 10;
				if (pressure <= 1000 && pressure >= 980) basePressure = pressure;
				float depth = 0;
				depth = (pressure-basePressure) * 0.0334552565551f;
				sprintf(dataprint, "press = %d mbar, depth = %f ft\nx = %d, y = %d\nBattery = %d %", pressure, depth, xtmp, ydata, battery);

				headerr[0] = '\0';

				//myfile << battery << "\n";
			}


			wcstring = convert(dataprint);

			gpad.Update();
			graph->BeginDraw();
			graph->Clear(ColorF(ColorF::WhiteSmoke, 1.f));

			graph->DrawMotorBars(rBrush);
			graph->FillMotorBars(lBrush, gpad);

			if ((gpad.buttons & 0x40)) {
				if (lightflag) {
					opacity += .2;
					lightflag = 0;
				}
			} else {
				lightflag = 1;
			}

			sBrush->SetColor(ColorF(ColorF::Black));
			//graph->CircleGraphics();
			//graph->CircleGraphics2(sBrush);

			graph->DrawCircle(Point2F(100.0f, 150.0f), 90.0f, sBrush);
			graph->DrawCircle(Point2F(300.0f, 150.0f), 90.0f, sBrush);

			bmp3->Draw(415.0f, 85.0f, opacity);
			if (opacity == 1.2f) opacity = 0.0f;
			graph->SetTransform(Matrix3x2F::Rotation(-xtmp, D2D1::Point2F(100, 150)));
			//bmp->Draw(30.0f, 50.0f, 1);
			sBrush->SetColor(ColorF(ColorF::DodgerBlue));
			graph->FillTriangle(sBrush);
			graph->SetTransform(Matrix3x2F::Identity());
			//graph->SetTransform(Matrix3x2F::Rotation(-ydata, D2D1::Point2F(305, 125)));
			//bmp2->Draw(205.0f, 50.0f, 0);
			//graph->SetTransform(Matrix3x2F::Rotation(-xdata, D2D1::Point2F(300, 150)));
			graph->FillCircle(Point2F(300.0f, 150.0f), 90.0f, lBrush2);
			lBrush2->SetStartPoint(Point2F(100, 240 +sydata)); //225 +-90
			//graph->SetTransform(Matrix3x2F::Identity());

			sBrush->SetColor(ColorF(ColorF::Black));
			graph->CircleGraphics();
			graph->CircleGraphics2(sBrush);

			graph->DrawText(L"Right Motor", L"Times New Roman", 14.f, RectF(700, 25, 800, 50), ColorF(ColorF::Black));
			graph->DrawText(L"Left Motor", L"Times New Roman", 14.f, RectF(620, 25, 700, 50), ColorF(ColorF::Black));
			graph->DrawText(L"Z Motor", L"Times New Roman", 14.f, RectF(550, 25, 600, 50), ColorF(ColorF::Black));
			graph->DrawText(L"Lights", L"Times New Roman", 14.f, RectF(460, 25, 600, 50), ColorF(ColorF::Black));
			//graph->DrawText(L"0/5", L"Times New Roman", 14.f, RectF(460, 75, 600, 50), ColorF(ColorF::Black));

			graph->DrawText(L"Pitch", L"Times New Roman", 14.f, RectF(280, 25, 600, 50), ColorF(ColorF::Black));
			graph->DrawText(L"Roll", L"Times New Roman", 14.f, RectF(85, 25, 600, 50), ColorF(ColorF::Black));
			graph->DrawText(L"0", L"Times New Roman", 14.f, RectF(755, 290, 800, 550), ColorF(ColorF::Black));
			graph->DrawText(L"100", L"Times New Roman", 14.f, RectF(755, 60, 800, 550), ColorF(ColorF::Black));
			graph->DrawText(L"-100", L"Times New Roman", 14.f, RectF(755, 530, 800, 800), ColorF(ColorF::Black));
			if (zdata < 0)
				graph->DrawText(L"ROV IS PAST 90 DEGREES!", L"Times New Roman", 14.f, RectF(75, 250, 600, 500), ColorF(ColorF::Red));
			graph->DrawText(wcstring, L"Times New Roman", 14.f, RectF(100, 300, 600, 500), ColorF(ColorF::Blue));
			////DPAD UP
			if (gpad.buttons & 0x1) {
				graph->DrawText(L"Camera Up", L"Times New Roman", 14.f, RectF(125, 250, 600, 500), ColorF(ColorF::Black));

			}
			else
				////DPAD DOWN
				if (gpad.buttons & 0x4) {
					graph->DrawText(L"Camera Down", L"Times New Roman", 14.f, RectF(125, 250, 600, 500), ColorF(ColorF::Black));

				}

			graph->EndDraw();
			delete[] wcstring;
		}
	}

	graph->BrushRelease(lBrush);
	graph->BrushRelease(rBrush);
	//myfile.close(); //--LG commented out while debugging 5/11
	return 0;
}

