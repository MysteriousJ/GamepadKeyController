#include <windows.h>
#pragma once

// Creates a basic window. Returns true if successful.
// Remember to start string parameters with 'L' to make them wide strings (i.e. L"Hello World!")
bool CreateWin32Window(HINSTANCE hInstance,
						int width,
						int height,
						std::basic_string<wchar_t> className, // also used for the window's title bar
						WNDPROC windowProcedure,
						// these two parameters are how you can make winAPI object oriented
						int cbWndExtra,
						LPVOID lpParam,
						// these two refer to items in a resource script (.rc file)
						std::basic_string<wchar_t> icon,
						std::basic_string<wchar_t> menuName)
{
	WNDCLASS wcs;

	wcs.style			= CS_HREDRAW | CS_VREDRAW;
	wcs.lpfnWndProc		= windowProcedure;
	wcs.cbClsExtra		= 0;
	wcs.cbWndExtra		= cbWndExtra;
	wcs.hInstance		= hInstance;
	wcs.hIcon			= LoadIcon(hInstance, icon.c_str());
	wcs.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcs.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcs.lpszMenuName	= menuName.c_str();
	wcs.lpszClassName	= className.c_str();

	if(!RegisterClass(&wcs))
	{
		return false;
	}

	HWND hwnd;
	hwnd = CreateWindow (className.c_str(), className.c_str(),
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, width, height,
							NULL, NULL, hInstance, lpParam);

	ShowWindow(hwnd, 1);
	UpdateWindow(hwnd);
	return true;
}