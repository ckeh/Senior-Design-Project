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

HWND window::CreateHandle(HINSTANCE hInstance) {
	WindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "Main", "ROV GUI", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
	if (!WindowHandle) { return nullptr; }

}