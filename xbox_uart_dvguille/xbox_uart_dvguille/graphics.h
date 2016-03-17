#pragma once
#include <Windows.h>
#include <d2d1.h>

//#include <dwrite_2.h>
#include <dwrite.h>

class Graphics {
private:
	ID2D1Factory *factory;
	ID2D1HwndRenderTarget *renderTarget;
	IDWriteFactory *writeFactory;
	IDWriteTextFormat *textFormat;
	const wchar_t *text;
	UINT32 textLength;
	

public:
	Graphics();
	~Graphics();

	bool Init(HWND WindowHandle);
	bool CreateDirectWriteFactory();
	void BeginDraw() { renderTarget->BeginDraw(); }
	void EndDraw() { renderTarget->EndDraw(); }
	void ClearScreen(float r, float g, float b);
	void FillCircle(float x, float y, float rad, float r, float g, float b, float alpha);
	void DrawText();
};