#include <windows.h>
#include "ButtonWidget.h"
#include "AxisWidget.h"
#include "resource.h"
#include "FileManager.h"
#include "UIWidgets.h"
#pragma once

// The entire program is in here.
class MainWindow {
	UIWidgets		widgets_; // button and axis settings GUI
	int				pollResolution_; // joystick polls per second setting
	int				joystickIndex_; // gamepad # being used
	bool			active_; // whether to process gamepad input and generate key/mouse events
	Joystick		joy_;
	FileManager		fileManager_; // for saving and loading files

	void setUpdateTimer(int updatesPerSecond, HWND window);
	void menuFunctions(HWND, WPARAM); // proccesses messages from the top menu bar
	void showMessage(std::wstring message, UINT typeFlag); // handles boiler-plate for winAPI's MessageBox()

	// the dialog procedure for Menu>Settings>Resolution
	static BOOL CALLBACK resolutionDialogProc(HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow():
		pollResolution_(60),
		joystickIndex_(0),
		active_(true)
	{}

	// call from a windows procedure to process messages
	bool processWndMsg(HWND, UINT, WPARAM, LPARAM);
};