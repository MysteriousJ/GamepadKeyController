#include "commons.h"
#include <vector>
#include <algorithm>
#include <sstream>
#include "Windowsx.h"
#include "Commctrl.h" // needed for hotkey macros

int commons::getUniqueInt()
{
	static int i = 0;
	i++;
	return i;
}

bool commons::isNumber(const std::wstring& s){
	for(unsigned int i=0; i<s.size(); i++){
		if (s[i]!=L'1' && s[i]!=L'2' && s[i]!=L'3' && s[i]!=L'4' && s[i]!=L'5' && s[i]!=L'6' &&
			s[i]!=L'7' && s[i]!=L'8' && s[i]!=L'9' && s[i]!=L'0' && s[i]!=L'.' && s[i]!=L'-')
			return false;
	}
	return true;
}

double commons::wStringToNumber(const std::wstring& text){
	std::wstringstream ss(text);
	double result;
	ss >> result;
    return result;
}

std::wstring commons::numberToWString(double d)
{
	std::wstringstream ss;
	ss << d;
	std::wstring s;
	ss>>s;

	return s;
}

void commons::restrictEditBoxToNumbers(HWND editTextControl, std::wstring& previousStringInBox)
{
	static const int maxChars = 8;
	wchar_t text[maxChars];
	Edit_GetText(editTextControl, (LPWSTR)text, maxChars);

	// limit input to numbers or a decimal point
	if (commons::isNumber(text) && Edit_GetTextLength(editTextControl) < maxChars-1)
		previousStringInBox = text;
	else
		// if the new character is not a number, revert to the previous string
		Edit_SetText(editTextControl, (LPCWSTR)previousStringInBox.c_str());
}

double commons::getNumberFromEditBox(HWND editTextControl)
{
	wchar_t text[8]; // arbitrarily sized buffer
	Edit_GetText(editTextControl, (LPWSTR)text, 8);
	return commons::wStringToNumber(text);
}

HWND commons::createChildWindow(HWND parentWindow, std::basic_string<wchar_t> className, WNDPROC windowProcedure, int cbWndExtra, LPVOID lpParam)
{
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong (parentWindow, GWL_HINSTANCE);

	static std::vector<std::basic_string<wchar_t>> registeredClasses;
	if (std::find(registeredClasses.begin(), registeredClasses.end(), className) == registeredClasses.end())
	{
		WNDCLASS wc;

		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= windowProcedure;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= cbWndExtra; // We're passing a pointer to this object so it can be used in an OOP style in wndProc
		wc.hInstance		= hInstance;
		wc.hIcon			= NULL;
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= className.c_str();

		RegisterClass(&wc);
		registeredClasses.push_back(className);
	}

	return CreateWindow(className.c_str(), NULL, WS_CHILDWINDOW | WS_VISIBLE, 0,0,0,0,
						parentWindow, (HMENU) getUniqueInt(), hInstance, lpParam);
}

HWND commons::createComboBox(HWND parentWindow, LPCWSTR items[], int numberOfItems)
{
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong (parentWindow, GWL_HINSTANCE);

	HWND wnd = CreateWindow(TEXT("COMBOBOX"),TEXT("THING"),
							WS_CHILD| WS_VISIBLE | CBS_DROPDOWNLIST,
							0, 0, 0, 0, parentWindow,
							(HMENU) getUniqueInt(),hInstance,NULL);

	for (int i=0; i<numberOfItems; i++)
		SendMessage(wnd, CB_ADDSTRING, 0, (LPARAM)items[i]);
	//set to first item
	SendMessage(wnd, CB_SETCURSEL, (WPARAM)0, 0);

	return wnd;
}

void commons::simulateKeyPress(int key, int modifierKeyFlags)
{
	// Accelerator keys
	if(HOTKEYF_SHIFT & modifierKeyFlags)
		keybd_event(VK_SHIFT, 0, 0, NULL);
	if(HOTKEYF_CONTROL & modifierKeyFlags)
		keybd_event(VK_CONTROL, 0, 0, NULL);
	if(HOTKEYF_ALT & modifierKeyFlags)
		keybd_event(VK_MENU, 0, 0, NULL); // VK_MENU is Alt

	keybd_event(key, 0, 0, NULL);
}

void commons::simulateKeyRelease(int key, int modifierKeyFlags)
{
	keybd_event(key, 0, KEYEVENTF_KEYUP, NULL);

	// Accelerator keys
	if(HOTKEYF_SHIFT & modifierKeyFlags)
		keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, NULL);
	if(HOTKEYF_CONTROL & modifierKeyFlags)
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, NULL);
	if(HOTKEYF_ALT & modifierKeyFlags)
		keybd_event(VK_MENU, 0, 0, NULL); // VK_MENU is Alt
}