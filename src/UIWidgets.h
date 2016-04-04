#include "ButtonWidget.h"
#include "AxisWidget.h"
#pragma once

struct UIWidgets {
	static const int supportedAxes = 6;
	static const int supportedButtons = 16;

	std::array<AxisWidget, supportedAxes> axes;
	std::array<ButtonWidget, supportedButtons> buttons;

	UIWidgets(){}

	void init(HWND parentWindow);
	void update(const Joystick& joy, const int pollResolution);
	void resize(int windowWidth, int windowHeight);
};