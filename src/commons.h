#include <windows.h>
#include <string>
#pragma once


// Functions that may be useful for any windows application
namespace commons
{
	int				getUniqueInt();

	bool			isNumber(const std::wstring& text);
	double			wStringToNumber(const std::wstring& text);
	std::wstring	numberToWString(double);
	void			restrictEditBoxToNumbers(HWND editTextControl, std::wstring& previousStringInBox);
	double			getNumberFromEditBox(HWND editTextControl);

	HWND createChildWindow(HWND parentWindow, std::basic_string<wchar_t> className, WNDPROC windowProcedure, int cbWndExtra, LPVOID lpParam);
	HWND createComboBox(HWND parentWindow, LPCWSTR items[], int numberOfItems);

	void simulateKeyPress(int key, int modifierKeyFlags);
	void simulateKeyRelease(int key, int modifierKeyFlags);
}