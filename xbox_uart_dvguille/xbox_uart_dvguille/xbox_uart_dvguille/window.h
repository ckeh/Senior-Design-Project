#pragma once
#include <Windows.h>
#include "includes.h"
class window {
	private:
		WNDCLASSEX wnd;
		HWND windowHandle;
		RECT rect;

	public:
		window() {};
		window(HINSTANCE hInstance, WNDPROC WindowProc);
		HWND CreateHandle(HINSTANCE hInstance, int height, int width);
		HWND GetHandle() { return windowHandle; };

};

