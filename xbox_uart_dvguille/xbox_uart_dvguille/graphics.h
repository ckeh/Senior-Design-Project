#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <memory>
#include <wincodec.h>
#include "XboxController.h"

//#include <dwrite_2.h>
#include <dwrite.h>

class Graphics {
private:
	ID2D1Factory *factory;
	ID2D1HwndRenderTarget *renderTarget;
	
	ID2D1PathGeometry *pathGeometry;
	IDWriteFactory *writeFactory;
	IDWriteTextFormat *textFormat;
	RECT rect;
	

public:
	Graphics();
	~Graphics();
	
	bool Init(HWND WindowHandle);	// Creates factory and writefactory objects to render text and object.
	
	inline void BeginDraw() { renderTarget->BeginDraw(); }
	void Clear(D2D_COLOR_F &color) { renderTarget->Clear(color); } 	// Clears the Screen to the Selected color

	inline void SetTransform(D2D1_MATRIX_3X2_F &matrix) { renderTarget->SetTransform(matrix); }
	inline void EndDraw() { renderTarget->EndDraw(); }
	inline ID2D1RenderTarget * GetRenderTarget() { return renderTarget; }

	ID2D1SolidColorBrush * CreateSolidColorBrush(D2D1_COLOR_F &color);
	ID2D1LinearGradientBrush * CreateLinearGradientBrush(const D2D1_POINT_2F &start, const D2D1_POINT_2F &end);
	ID2D1RadialGradientBrush * CreateRadialGradientBrush(const D2D1_POINT_2F &center, const D2D1_POINT_2F &offset);
	
	void BrushRelease(ID2D1Brush *brush) { brush->Release(); }
	
	void FillCircle(D2D1_POINT_2F &center, float rad, D2D1_COLOR_F &color);
	void DrawCircle(D2D1_POINT_2F &center, float rad, ID2D1Brush *brush);
	
	void FillRect(D2D1_RECT_F &rect, D2D1_COLOR_F &color);
	void FillRect(D2D1_RECT_F &rect, ID2D1Brush *brush);

	void FillTriangle(ID2D1Brush *brush);

	void CreateSink();
	
	void DrawMotorBars(ID2D1RadialGradientBrush *rBrush);
	void FillMotorBars(ID2D1LinearGradientBrush *lBrush, XboxController gpad);
	void DrawText(const wchar_t *text, const wchar_t *font, float size, D2D1_RECT_F &rect, D2D1_COLOR_F &color);
	void ShowPercentage();
};

class Bitmap
{
private:
	 std::shared_ptr<Graphics> graph;
	 ID2D1Bitmap *bmp;

public:
	Bitmap(const wchar_t *filename, std::shared_ptr<Graphics> graph);
	~Bitmap();

	void Draw(float x, float y, int flag);
	void Draw(float x, float y, float opacity);

};