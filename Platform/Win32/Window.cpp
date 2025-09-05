#include "Window.h"

Window::Window() : hwnd_(nullptr) {}
Window::~Window() {}

bool Window::Create(const char* title, int width, int height)
{
	std::wstring wtitle(title, title + strlen(title));

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"RancageWindowClass";

	RegisterClass(&wc);

	hwnd_ = CreateWindowEx(
		0, L"RancageWindowClass", wtitle.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

	return hwnd_ != nullptr;
}

void Window::Show()
{
	ShowWindow(hwnd_, SW_SHOW);
}

void Window::PollEvents()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

