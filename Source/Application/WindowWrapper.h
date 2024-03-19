#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <iostream>

class WindowWrapper
{
	friend class SwapchainWrapper;
	friend class Application;
private:
	UINT width;
	UINT height;

	bool pendingResize;

	HWND hWnd;
	WNDCLASSEX wc;

	BOOL cursorLocked;
	BOOL cursorVisible;

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	WindowWrapper()
	{
		LPCWSTR applicationName = L"DirectX11 Project";
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = s_WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = applicationName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		::RegisterClassExW(&wc);

		width = 1280u;
		height = 720u;

		RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

		auto posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		auto posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
			dwStyle,
			posX, posY,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr, nullptr, hInstance, this);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	}

	void Update();

	void UpdateCursor();

	void SetCursorLocked(BOOL value);

	void SetCursorVisible(BOOL value);

	void Show();

	bool ShouldClose();

	UINT Width() const;

	UINT Height() const;

	HWND GetHWND() const;
};