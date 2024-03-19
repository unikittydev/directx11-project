#include "WindowWrapper.h"

#include "Application.h"

LRESULT WINAPI WindowWrapper::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			pendingResize = true;
			width = (UINT)LOWORD(lParam);
			height = (UINT)HIWORD(lParam);
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		SetCursorLocked(FALSE);
		SetCursorVisible(TRUE);
		return 0;
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			InputDevice::KeyboardInputEventArgs args{};
			args.MakeCode = raw->data.keyboard.MakeCode;
			args.Flags = raw->data.keyboard.Flags;
			args.Message = raw->data.keyboard.Message;
			args.VKey = raw->data.keyboard.VKey;

			Application::GetInput().OnKeyDown(args);

		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			Application::GetInput().OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
	}
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowWrapper::s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowWrapper* pThis; // our "this" pointer will go here
	if (uMsg == WM_NCCREATE)
	{
		// Recover the "this" pointer which was passed as a parameter
		// to CreateWindow(Ex).
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<WindowWrapper*>(lpcs->lpCreateParams);
		// Put the value in a safe place for future use
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		// Recover the "this" pointer from where our WM_NCCREATE handler
		// stashed it.
		pThis = reinterpret_cast<WindowWrapper*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	if (pThis)
	{
		// Now that we have recovered our "this" pointer, let the
		// member function finish the job.
		return pThis->WndProc(hwnd, uMsg, wParam, lParam);
	}
	// We don't know what our "this" pointer is, so just do the default
	// thing. Hopefully, we didn't need to customize the behavior yet.
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowWrapper::Update()
{
	if (pendingResize)
	{
		Application::GetSwapchain().Resize(Application::GetDevice(), width, height);
		pendingResize = false;
	}
	UpdateCursor();
}

void WindowWrapper::UpdateCursor()
{
	if (cursorLocked)
	{
		POINT point{ (LONG)width / 2, (LONG)height / 2 };
		ClientToScreen(hWnd, &point);
		SetCursorPos(point.x, point.y);
	}

	ShowCursor(cursorVisible);
}

void WindowWrapper::SetCursorLocked(BOOL value)
{
	cursorLocked = value;
}

void WindowWrapper::SetCursorVisible(BOOL value)
{
	cursorVisible = value;
}

void WindowWrapper::Show()
{
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
}

bool WindowWrapper::ShouldClose()
{
	bool result = false;
	MSG msg;

	while (::PeekMessage(&msg, hWnd, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			result = true;
	}
	return result;
}

UINT WindowWrapper::Width() const
{
	return width;
}

UINT WindowWrapper::Height() const
{
	return height;
}

HWND WindowWrapper::GetHWND() const
{
	return hWnd;
}