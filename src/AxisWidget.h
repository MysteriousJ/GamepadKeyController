#include <windows.h>
#include "Joystick.h"
#include "AxisControl.h"
#pragma once

class AxisWidget {
	static const int	height_ = 23;

	static LRESULT CALLBACK axisSettingWndProc(HWND, UINT, WPARAM, LPARAM);
	static BOOL CALLBACK moveMouseDialogProc(HWND, UINT, WPARAM, LPARAM);
	static BOOL CALLBACK keyPressDialogProc(HWND, UINT, WPARAM, LPARAM);

	AxisControl axis_;
	int			width_,
				barWidth_; // bar visualizing the value of the axis
	HWND		hwnd_,
				comboBox_;

public:
	AxisWidget(){}

	void		init(HWND parentWindow);
	void		update(float axisValue, int updatesPerSecond);
	void		move(int x, int y, int width);
	bool		processWndMsg(HWND, UINT, WPARAM, LPARAM);
	void		setConfig(AxisConfig config);

	int			width() const;
	int			height() const;
	AxisConfig	config() const;
};