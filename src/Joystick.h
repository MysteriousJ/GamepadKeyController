#include <windows.h>
#include <array>
#include <SDL.h>
#include <SDL_joystick.h>
#pragma once

class Joystick {
	const static int buttonsSupported_ = 16,
					 axesSupported_ = 6;

	float x_, y_, z_, u_, v_, r_, threshhold_;
	std::array<bool, buttonsSupported_>	buttonHeldStates_,		// starts at 0
										buttonPressedStates_;
	std::array<float, axesSupported_>	axisStates_;
	SDL_Joystick* joy_;

public:
	Joystick();

	bool	init(int joystickIndex); // starts at 0
	void	pollUpdate();

	float	axis(int axisNumber) const;
	bool	buttonPressed(int buttonNumber) const;
	bool	buttonHeld(int buttonNumber) const;
	int		buttonsSupported() const {return buttonsSupported_;}
	int		axesSupported() const {return axesSupported_;}
};