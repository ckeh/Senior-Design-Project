#include "graphics.h"

using namespace D2D1;

Graphics::Graphics() {
	factory = nullptr;
	renderTarget = nullptr;
}

Graphics::~Graphics() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
}

bool Graphics::Init(HWND windowHandle) {
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(windowHandle, SizeU(rect.right, rect.bottom)),
		&renderTarget
		);

	if (res != S_OK) return false;

	return true;
}

bool Graphics::CreateDirectWriteFactory() {


	return false;
}

void Graphics::ClearScreen(float r, float g, float b) {
	renderTarget->Clear(ColorF(r, g, b));
}
void Graphics::FillCircle(float x, float y, float rad, float r, float g, float b, float alpha) {
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(ColorF(r, g, b, alpha), &brush);
	renderTarget->FillEllipse(Ellipse(Point2F(x, y), rad, rad), brush);
	brush->Release();
}



void Graphics::DrawText() {
	
}
