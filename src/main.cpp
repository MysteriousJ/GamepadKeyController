#include "ButtonWidget.h"
#include "AxisWidget.h"
#include "resource.h"
#include "FileManager.h"
#include "Win32Window.h"
#include "MainWindow.h"
#include <array>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	// initialize a window (this function is in win32win.h)
	if(!CreateWin32Window(hInstance, 870, 460, L"Gamepad Key Controller", WndProc, NULL, NULL, L"GAMEPADKEYCONTROLLER", L"MainMenu")){
		// if initialization somehow failed
		MessageBox (NULL, TEXT ("Unable to initialize Win32Window"), TEXT (""), MB_ICONERROR);
		return 0;
	}
	// message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// The MainWindow class handles this program's messages, as well as everything else.
	static MainWindow mainWindow;

	if (mainWindow.processWndMsg(hwnd, message, wParam, lParam))
		return 0;

	return DefWindowProc (hwnd, message, wParam, lParam);
}