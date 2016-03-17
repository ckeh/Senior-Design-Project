#pragma once
#include <Windows.h>
class window {
	private:
		WNDCLASSEX wnd;
		

	public:
		window(HINSTANCE hInstance, WNDPROC WindowProc);
		HWND CreateHandle(HINSTANCE hInstance);
		HWND WindowHandle;
};

