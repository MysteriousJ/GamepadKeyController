#include <windows.h>
#include "Joystick.h"
#include "ButtonControl.h"
#pragma once

// Draws the UI and controls the logic for one button configuration
class ButtonWidget {
	static const int	height_ = 54; // each module's height is static, but its width is flexable.

	int					width_;
	ButtonControl		button_;	// this handles generating the key or mouse press when a button is pressed on a joystick
	HWND				hwnd_,		// the region of the screen this module occupies
						comboBox1_, // the list containing which event to simulate (keypress, left mouse button, etc.)
						comboBox2_; // the list containing how the event should be simulated (a single press, held down, or pressed rappidly)

	static LRESULT CALLBACK buttonSettingWndProc(HWND, UINT, WPARAM, LPARAM); // windows procedure for this module's region of the screen
	static BOOL CALLBACK	keyPressDialogProc(HWND, UINT, WPARAM, LPARAM); // the dialog procedure when "keypress" is chosen in comboBox1_

	void processCommandMsg(HWND, UINT, WPARAM, LPARAM); // to separate the combo box events from the rest of the messages

public:
	ButtonWidget(){}

	void			init(HWND parentWindow);
	void			update(bool buttonHeld, bool buttonPressed);
	void			move(int x, int y, int width); // call to initialize position and when the window is resized
	bool			processWndMsg(HWND, UINT, WPARAM, LPARAM); // called from buttonSettingWndProc to enable OOP
	void			setConfig(ButtonConfig config); // used when loading a file to set this module's settings

	int				width() const;
	int				height() const;
	ButtonConfig	config() const; // used when saving a file to get this module's current settings
};