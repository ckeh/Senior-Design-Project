#include "graphics.h"

#define THETA 22.5
#define RADIUS 90
#define PI 3.14159265
#define X 100
#define Y 60

using namespace D2D1;

D2D1_POINT_2F * SegmentLength();

Graphics::Graphics() {
	factory = nullptr;
	renderTarget = nullptr;
	writeFactory = nullptr;
	points = SegmentLength();
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

	factory->CreatePathGeometry(&pathGeometry);
	factory->CreatePathGeometry(&pathGeometry2);
	

	if (res != S_OK) return false;

	return true;
}

ID2D1SolidColorBrush * Graphics::CreateSolidColorBrush(D2D1_COLOR_F &color) {
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
	gradientStopsArray[2].color = D2D1::ColorF(ColorF::DarkRed, 1);
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

ID2D1LinearGradientBrush * Graphics::CreateLinearGradientBrush2(const D2D1_POINT_2F &start, const D2D1_POINT_2F &end) 
{
	ID2D1LinearGradientBrush *linearGradientBrush;
	ID2D1GradientStopCollection *gradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStopsArray[2];
	gradientStopsArray[0].color = D2D1::ColorF(ColorF::DodgerBlue, 1);
	gradientStopsArray[0].position = 0.5f;
	gradientStopsArray[1].color = D2D1::ColorF(ColorF::WhiteSmoke, 1);
	gradientStopsArray[1].position = .50f;

	renderTarget->CreateGradientStopCollection(
		gradientStopsArray,
		2,
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

ID2D1RadialGradientBrush * Graphics::CreateRadialGradientBrush(const D2D1_POINT_2F &center, const D2D1_POINT_2F &offset) {
	ID2D1RadialGradientBrush *radialGradientBrush;
	ID2D1GradientStopCollection *gradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStopsArray[2];
	gradientStopsArray[0].color = D2D1::ColorF(ColorF::Black, 1);
	gradientStopsArray[0].position = 0.33f;
	gradientStopsArray[1].color = D2D1::ColorF(ColorF::WhiteSmoke, 1);
	gradientStopsArray[1].position = .75f;


	renderTarget->CreateGradientStopCollection(
		gradientStopsArray,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&gradientStops
		);
	renderTarget->CreateRadialGradientBrush(
		RadialGradientBrushProperties(
			center,
			offset,
			45,
			1300),
		gradientStops,
		&radialGradientBrush
		);
	return radialGradientBrush;
}

void Graphics::CircleGraphics()
{
	//FillRect(RectF(265, 220, 285, 221), ColorF(ColorF::Black));
	FillRect(RectF(260, 82, 340, 83), ColorF(ColorF::Black));

	FillRect(RectF(285, 105, 315, 106), ColorF(ColorF::Black));

	FillRect(RectF(260, 127, 340, 128), ColorF(ColorF::Black));

	FillRect(RectF(220, 150, 380, 151), ColorF(ColorF::Black)); // Middle bar

	FillRect(RectF(260, 172, 340, 173), ColorF(ColorF::Black));

	FillRect(RectF(285, 195, 315, 196), ColorF(ColorF::Black));

	FillRect(RectF(260, 217, 340, 218), ColorF(ColorF::Black));
	//FillRect(RectF(265, 80, 285, 81), ColorF(ColorF::Black));
}
void Graphics::CircleGraphics2(ID2D1Brush *brush)
{
	FillTriangle2(brush);

	for (int i = 0; i < 8; ++i)
	{
		if (!i) continue;
		SetTransform(Matrix3x2F::Rotation(THETA*i - 90, points[i]));
		if (i % 2)
			FillRect(RectF(points[i].x, points[i].y, points[i].x + 5, points[i].y + 1), brush);
		else
			FillRect(RectF(points[i].x, points[i].y, points[i].x + 15, points[i].y + 1), brush);
		SetTransform(Matrix3x2F::Identity());
	}


	for (int i = 8; i < 16; ++i)
	{
		if (i == 8) continue;
		SetTransform(Matrix3x2F::Rotation(-THETA*(i - 8) - 90, points[i]));
		if (i % 2)
			FillRect(RectF(points[i].x, points[i].y, points[i].x + 5, points[i].y + 1), brush);
		else
			FillRect(RectF(points[i].x, points[i].y, points[i].x + 15, points[i].y + 1), brush);
		SetTransform(Matrix3x2F::Identity());
	}

}

void Graphics::FillCircle(D2D1_POINT_2F &center, float rad, D2D1_COLOR_F &color) {
	ID2D1SolidColorBrush *brush;
	renderTarget->CreateSolidColorBrush(color, &brush);
	renderTarget->FillEllipse(Ellipse(center, rad, rad), brush);
	brush->Release();
}

void Graphics::FillCircle(D2D1_POINT_2F &center, float rad, ID2D1Brush *lBrush) {
	renderTarget->FillEllipse(Ellipse(center, rad, rad), lBrush);
}

void Graphics::DrawCircle(D2D1_POINT_2F &center, float rad, ID2D1Brush *brush) {
	renderTarget->DrawEllipse(Ellipse(center, rad, rad), brush);
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

void Graphics::CreateSink()
{

	ID2D1GeometrySink *sink = nullptr;
	HRESULT res = pathGeometry->Open(&sink);

	if (SUCCEEDED(res)) {
		sink->SetFillMode(D2D1_FILL_MODE_WINDING);

		sink->BeginFigure(
			Point2F(190.0f, 150.0f),
			D2D1_FIGURE_BEGIN_FILLED
			);

		sink->AddArc(
			ArcSegment(
			Point2F(10, 150),
			SizeF(90, 90),
			0.0f, 
			D2D1_SWEEP_DIRECTION_CLOCKWISE,
			D2D1_ARC_SIZE_SMALL
			));
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	res = sink->Close();
	sink->Release();
}

void Graphics::CreateSink2()
{
	ID2D1GeometrySink *sink = nullptr;
	HRESULT res = pathGeometry2->Open(&sink);

	if (SUCCEEDED(res)) {
		sink->SetFillMode(D2D1_FILL_MODE_WINDING);
		// Begin PFD graphics
		sink->BeginFigure(
			Point2F(100, 75),
			D2D1_FIGURE_BEGIN_FILLED
			);
		D2D1_POINT_2F points[3] = {
			Point2F(95,70),
			Point2F(105,70),
			Point2F(100,75),
		};
		sink->AddLines(points, ARRAYSIZE(points));
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	res = sink->Close();
	sink->Release();
}

void Graphics::FillTriangle(ID2D1Brush *brush)
{
	renderTarget->FillGeometry(pathGeometry, brush);
}

void Graphics::FillTriangle2(ID2D1Brush *brush)
{
	renderTarget->FillGeometry(pathGeometry2, brush);
}

void Graphics::DrawText(const wchar_t *text, const wchar_t *font, float size, D2D1_RECT_F &rect, D2D1_COLOR_F &color) 
{
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

void Graphics::DrawMotorBars(ID2D1RadialGradientBrush *rBrush)
{
	rBrush->SetCenter(Point2F(725, 300));
	FillRect(RectF(700, 50, 750, 550), rBrush);
	rBrush->SetCenter(Point2F(650, 300));
	FillRect(RectF(625, 50, 675, 550), rBrush);
	rBrush->SetCenter(Point2F(575, 300));
	FillRect(RectF(550, 50, 600, 550), rBrush);
}

void Graphics::FillMotorBars(ID2D1LinearGradientBrush *lBrush, XboxController gpad)
{
	////////////// Third Motor Bar ////////////////////
	lBrush->SetStartPoint(Point2F(700, 300));
	lBrush->SetEndPoint(Point2F(700, 550));
	if (gpad.leftDirection == DOWN )
	{
		int diff = int(gpad.lt - gpad.rt);
		if (gpad.lt)
		{
			if ((diff >= 0) && (gpad.rightDirection == LEFT))
				FillRect(RectF(700, 300, 750, (300 + gpad.lt)-gpad.rt), lBrush);
			else
				FillRect(RectF(700, 300, 750, 300 + (gpad.lt)), lBrush);	// Alter this line with gpad direction (second argument of RectF)
		}
		else
		{
			FillRect(RectF(700, 300, 750, 300 + (gpad.lt)), lBrush);	// Alter this line with gpad direction (second argument of RectF)
		}
		
	}
		
	else if (gpad.rightDirection == RIGHT && !gpad.lt)
	{
		FillRect(RectF(700, 300, 750, 300+gpad.rt), lBrush);
	}
	else if (gpad.rightDirection == RIGHT && !gpad.lt && (gpad.buttons & 0x8)) {
		FillRect(RectF(700, 300, 750, 300 + gpad.rt), lBrush);
	}
	else if (gpad.rightDirection == LEFT && !gpad.lt && (gpad.buttons & 0x8)) {
		FillRect(RectF(700, 300, 750, 300 - gpad.rt), lBrush);
	}
	lBrush->SetStartPoint(Point2F(700, 300));
	lBrush->SetEndPoint(Point2F(700, 50));
	if (gpad.leftDirection == UP)
	{
		if (gpad.rightDirection == RIGHT && gpad.lt)
		{
			FillRect(RectF(700, (300 - gpad.lt) + gpad.rt, 750, 300), lBrush);
		}
		else
			FillRect(RectF(700, 300 - gpad.lt, 750, 300), lBrush);		// Alter this line with gpad direction (second argument of RectF)
		/*if (gpad.rightDirection == RIGHT)
			FillRect(RectF(700, (300 - gpad.lt) + gpad.rt, 750, 300), lBrush);*/
	}
	///////////////////////////////////////////////////

	////////////// Second Motor Bar ////////////////////
	lBrush->SetStartPoint(Point2F(700, 300));
	lBrush->SetEndPoint(Point2F(700, 550));
	if (gpad.leftDirection == DOWN)
	{
		int diff = int(gpad.lt - gpad.rt);
		if ((gpad.lt))
		{
			if ((diff >= 0) && (gpad.rightDirection == RIGHT))
				FillRect(RectF(625, 300, 675, (300+gpad.lt)-gpad.rt), lBrush);
			else
				FillRect(RectF(625, 300, 675, (300 + gpad.lt)), lBrush);

		}
		else
		{
			FillRect(RectF(625, 300, 675, 300 + (gpad.lt)), lBrush);	// Alter this line with gpad direction (second argument of RectF)
		}
	}
	else if (gpad.rightDirection == LEFT && !gpad.lt)
	{
		FillRect(RectF(625, 300, 675, 300 + gpad.rt), lBrush);
	}
	else if (gpad.rightDirection == RIGHT && !gpad.lt && (gpad.buttons & 0x8))
	{
		FillRect(RectF(625, 300, 675, 300 - gpad.rt), lBrush);
	}
	lBrush->SetStartPoint(Point2F(700, 300));
	lBrush->SetEndPoint(Point2F(700, 50));
	if (gpad.leftDirection == UP)
	{
		if (gpad.rightDirection == LEFT && gpad.lt)
		{
			FillRect(RectF(625, (300 - gpad.lt) + gpad.rt, 675, 300), lBrush);
		}
		else
			FillRect(RectF(625, 300 - gpad.lt, 675, 300), lBrush);
	}
	// Alter this line with gpad direction (second argument of RectF)
	///////////////////////////////////////////////////

	////////////// First Motor Bar ////////////////////
	

	//FillRect(RectF(550, 300, 600, 550), lBrush);
	int diff = gpad.rz - gpad.lz;
	if (diff > 0) 
	{
		lBrush->SetStartPoint(Point2F(700, 300));
		lBrush->SetEndPoint(Point2F(700, 50));
		FillRect(RectF(550, 300-diff, 600, 300), lBrush);
	}
	else
	{
		lBrush->SetStartPoint(Point2F(700, 300));
		lBrush->SetEndPoint(Point2F(700, 550));
		FillRect(RectF(550, 300, 600, 300-diff), lBrush);
	}
	//if (gpad.lz)
	//{
	//	//if (gpad.rz)
	//	//	FillRect(RectF(550, 300, 600, 300 + gpad.lz), lBrush);	// Alter this line with gpad direction (second argument of RectF)

	//	if ((gpad.lz - gpad.rz) > 0)
	//	{
	//		FillRect(RectF(550, 300, 600, (300 + gpad.lz)-gpad.rz), lBrush);	// Alter this line with gpad direction (second argument of RectF)
	//	}
	//	else
	//	{
	//		lBrush->SetStartPoint(Point2F(700, 300));
	//		lBrush->SetEndPoint(Point2F(700, 50));
	//		FillRect(RectF(550, 300, 600, 300), lBrush);	// Alter this line with gpad direction (second argument of RectF)
	//	}
	//}

	//lBrush->SetStartPoint(Point2F(700, 300));
	//lBrush->SetEndPoint(Point2F(700, 50));
	////FillRect(RectF(550, 50, 600, 300), lBrush);
	//FillRect(RectF(550, (300), 600, 300), lBrush);		// Alter this line with gpad direction (second argument of RectF)
	/////////////////////////////////////////////////////
}

Bitmap::Bitmap(const wchar_t *filename, std::shared_ptr<Graphics> gfx)
{
	graph = gfx;
	bmp = nullptr;
	HRESULT hr;

	IWICImagingFactory *wicFactory = nullptr;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory
		);

	IWICBitmapDecoder *wicDecoder = nullptr;
	hr = wicFactory->CreateDecoderFromFilename(
		filename,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&wicDecoder
		);

	IWICBitmapFrameDecode *wicFrame = nullptr;
	hr = wicDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter *wicConverter = nullptr;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	hr = wicConverter->Initialize(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0,
		WICBitmapPaletteTypeCustom
		);

	graph->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter,
		&bmp
		);

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicFrame) wicFrame->Release();
	if (wicConverter) wicConverter->Release();
}

Bitmap::~Bitmap()
{
	if (bmp != nullptr) bmp->Release();
}

void Bitmap::Draw(float x, float y, int flag)
{
	if (flag) {
		graph->GetRenderTarget()->DrawBitmap(
			bmp,
			RectF(x, y, x + 150, y + 150),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
			);
	}
	else {
		graph->GetRenderTarget()->DrawBitmap(
			bmp,
			RectF(x, y, x + 200, y + 150),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
			);
	}
}

void Bitmap::Draw(float x, float y, float opacity)
{
	graph->GetRenderTarget()->DrawBitmap(
		bmp,
		RectF(x, y, x + 125, y + 125),
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
		);

}

D2D1_POINT_2F * SegmentLength()
{
	float segments[8];
	static D2D1_POINT_2F points[16];

	for (int i = 0; i < 8; ++i)
	{
		float theta = (THETA*i) * PI / 180;
		segments[i] = RADIUS*sqrt(2 - 2 * cosf(theta));
		float psi = theta / 2;
		float x = segments[i] * cosf(psi);
		float y = segments[i] * sinf(psi);
		points[i] = Point2F(X + x, Y + y);
	}
	for (int i = 0; i < 8; ++i)
	{
		float theta = (THETA*i) * PI / 180;
		float psi = theta / 2;
		float x = segments[i] * cosf(psi);
		float y = segments[i] * sinf(psi);
		points[i + 8] = Point2F(X - x, Y + y);
	}
	return points;
}