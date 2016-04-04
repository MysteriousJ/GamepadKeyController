#include "Joystick.h"
#include <MMSystem.h>

Joystick::Joystick()
{
	for (int i=0; i<buttonsSupported_; i++)
		buttonHeldStates_[i] = buttonPressedStates_[i] = 0;
	for (int i=0; i<axesSupported_; i++)
		axisStates_[i] = 0;
}

bool Joystick::init(int joystickIndex)
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (SDL_NumJoysticks() > joystickIndex){
		joy_ = SDL_JoystickOpen(joystickIndex);
		return true;
	}
	return false;
}

void Joystick::pollUpdate()
{
	SDL_JoystickUpdate();
	// update the state of every button
	for (int i=0; i<buttonsSupported(); i++){
		bool buttonHeld = SDL_JoystickGetButton(joy_, i);

		// map hats to buttons 12 to 15
		bool hatHeld = false;
		if(i==12 && (SDL_JoystickGetHat(joy_, 0) & SDL_HAT_UP)
		|| i==13 && (SDL_JoystickGetHat(joy_, 0) & SDL_HAT_RIGHT)
		|| i==14 && (SDL_JoystickGetHat(joy_, 0) & SDL_HAT_DOWN)
		|| i==15 && (SDL_JoystickGetHat(joy_, 0) & SDL_HAT_LEFT))
			hatHeld = true;
		
		// buttonHeld will be true if either the button or a hat is held
		buttonHeld = buttonHeld|hatHeld;

		// set button pressed and held states
		if (buttonHeld){
			buttonPressedStates_[i] = !buttonHeldStates_[i];
			buttonHeldStates_[i] = true;
		}
		else
			buttonPressedStates_[i] = buttonHeldStates_[i] = false;
	}

	// axes
	for (int i=0; i<axesSupported(); i++)
		// map axis value to -1 to 1
		axisStates_[i] = float(SDL_JoystickGetAxis(joy_, i)) / 32768.0f; // max size of signed short int
}

float Joystick::axis(int axisNumber) const
{
	return axisStates_[axisNumber];
}

bool Joystick::buttonHeld(int buttonNumber) const
{
	return buttonHeldStates_[buttonNumber];
}

bool Joystick::buttonPressed(int buttonNumber) const
{
	return buttonPressedStates_[buttonNumber];
}