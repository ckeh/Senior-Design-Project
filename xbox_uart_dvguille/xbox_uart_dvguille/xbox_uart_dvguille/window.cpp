#include "window.h"


window::window(HINSTANCE hInstance, WNDPROC WindowProc) {
	ZeroMemory(&wnd, sizeof(WNDCLASSEX));
	
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WindowProc;
	wnd.lpszClassName = "Main";
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wnd);

}

HWND window::CreateHandle(HINSTANCE hInstance, int height, int width) {
	RECT rect = { 0, 0, height, width };
	AdjustWindowRectEx(&rect, WS_OVERLAPPED, false, WS_EX_OVERLAPPEDWINDOW);

	windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "Main", "ROV GUI", WS_OVERLAPPEDWINDOW, 200, 200, 
		rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);
	if (!windowHandle) { return nullptr; }

}