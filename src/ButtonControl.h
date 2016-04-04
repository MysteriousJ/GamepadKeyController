#include "Config.h"
#pragma once

// ButtonControl and AxisControl are pretty much the only things in this program that aren't UI code.
// This class handles checking if a button is pressed, and generating the key and mouse events if it is.
class ButtonControl {
	ButtonConfig	config_;
	bool			held_;

	// generates mouse left/right/middle button events
	void			mouseButtonPresses();
	void			mouseButtonReleases();
	// checks if the current config_.action is a mouse event
	bool			actionIsMouseEvent() const;

public:
	ButtonControl():
		held_(false)
	{}

	void			update(bool buttonHeld, bool buttonPressed);
	void			setConfig(const ButtonConfig& config); // set by the ButtonWidget owning this

	bool			held() const; // whether the assigned button is being pressed
	ButtonConfig	config() const;
};