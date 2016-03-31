#include "graphics.h"

using namespace D2D1;

Graphics::Graphics() {
	factory = nullptr;
	renderTarget = nullptr;
	writeFactory = nullptr;
}

Graphics::~Graphics() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (writeFactory) writeFactory->Release();
}

bool Graphics::Init(HWND windowHandle) {
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;
	
	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&writeFactory)
		);

	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(windowHandle, SizeU(rect.right, rect.bottom)),
		&renderTarget
		);
	if (res != S_OK) return false;

	return true;
}

ID2D1SolidColorBrush * Graphics::CreateBrush(D2D1_COLOR_F &color) {
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(color, &brush);
	return brush;
}

ID2D1LinearGradientBrush * Graphics::CreateLinearGradientBrush(const D2D1_POINT_2F &start, const D2D1_POINT_2F &end) {
	ID2D1LinearGradientBrush *linearGradientBrush;
	ID2D1GradientStopCollection *gradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStopsArray[3];
	gradientStopsArray[0].color = D2D1::ColorF(ColorF::ForestGreen, 1);
	gradientStopsArray[0].position = 0.f;
	gradientStopsArray[1].color = D2D1::ColorF(ColorF::Yellow, 1);
	gradientStopsArray[1].position = 0.66f;
	gradientStopsArray[2].color = D2D1::ColorF(ColorF::Red, 1);
	gradientStopsArray[2].position = 1.f;

	renderTarget->CreateGradientStopCollection(
		gradientStopsArray,
		3,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&gradientStops
		);
	renderTarget->CreateLinearGradientBrush(
		LinearGradientBrushProperties(
			start,
			end),
		gradientStops,
		&linearGradientBrush
		);
	return linearGradientBrush;
}

void Graphics::FillCircle(D2D1_POINT_2F &center, float rad, D2D1_COLOR_F &color) {
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->FillEllipse(Ellipse(center, rad, rad), brush);
	brush->Release();
}

void Graphics::FillRect(D2D1_RECT_F &rect, D2D1_COLOR_F &color) {
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->FillRectangle(rect, brush);
	brush->Release();
}

void Graphics::FillRect(D2D1_RECT_F &rect, ID2D1Brush *brush) {
	renderTarget->FillRectangle(rect, brush);
}

void Graphics::DrawText(const wchar_t *text, const wchar_t *font, float size, D2D1_RECT_F &rect, D2D1_COLOR_F &color) {
	UINT32 textLength = (UINT32)wcslen(text);
	HRESULT res = writeFactory->CreateTextFormat(
		font,
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"en-us",
		&textFormat);

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->DrawText(text, textLength, textFormat, rect, brush);
	brush->Release();
}

void Graphics::ShowPercentage()
{
	DrawText(
		L"100",
		L"Times New Roman",
		14.f,
		RectF(670, 50, 700, 150),
		ColorF(ColorF::Black)
		);
	DrawText(
		L"75",
		L"Times New Roman",
		14.f,
		RectF(670, 175, 700, 200),
		ColorF(ColorF::Black)
		);
	DrawText(
		L"50",
		L"Times New Roman",
		14.f,
		RectF(670, 300, 700, 325),
		ColorF(ColorF::Black)
		);
	DrawText(
		L"25",
		L"Times New Roman",
		14.f,
		RectF(670, 425, 700, 450),
		ColorF(ColorF::Black)
		);


}
