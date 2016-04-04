#include "ButtonControl.h"
#include "commons.h"
#include <windows.h>


bool ButtonControl::held() const {
	return held_;
}

ButtonConfig ButtonControl::config() const {
	return config_;
}

void ButtonControl::setConfig(const ButtonConfig& config){
	config_ = config;
}

void ButtonControl::mouseButtonPresses(){
	POINT p;
	GetCursorPos(&p);
	switch(config_.action){
	case ButtonConfig::LeftClick:
		mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, NULL, NULL);
		break;
	case ButtonConfig::RightClick:
		mouse_event(MOUSEEVENTF_RIGHTDOWN, p.x, p.y, NULL, NULL);
		break;
	case ButtonConfig::MiddleClick:
		mouse_event(MOUSEEVENTF_MIDDLEDOWN, p.x, p.y, NULL, NULL);
		break;
	}
}

void ButtonControl::mouseButtonReleases(){
	POINT p;
	GetCursorPos(&p);
	switch(config_.action){
	case ButtonConfig::LeftClick:
		mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, NULL, NULL);
		break;
	case ButtonConfig::RightClick:
		mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, NULL, NULL);
		break;
	case ButtonConfig::MiddleClick:
		mouse_event(MOUSEEVENTF_MIDDLEUP, p.x, p.y, NULL, NULL);
		break;
	}
}

bool ButtonControl::actionIsMouseEvent() const
{
	if (config_.action == ButtonConfig::LeftClick
		|| config_.action == ButtonConfig::MiddleClick
		|| config_.action == ButtonConfig::RightClick)
	{
		return true;
	}
	return false;
}

// simulate each type of mouse/key event for each type of press (hold, single, or rapid)
void ButtonControl::update(bool nowHeld, bool nowPressed)
{
	// Hold presses
	if (config_.pressType == ButtonConfig::Hold && nowPressed){
		if (actionIsMouseEvent()){
			mouseButtonPresses();
		}
		if (config_.action == ButtonConfig::KeyPress){
			commons::simulateKeyPress(config_.key, config_.keyModifiers);
		}
	}

	// Hold releases
	if (config_.pressType == ButtonConfig::Hold && !nowHeld && held_){
		if (actionIsMouseEvent()){
			mouseButtonReleases();
		}
		if (config_.action == ButtonConfig::KeyPress){
			commons::simulateKeyRelease(config_.key, config_.keyModifiers);
		}
	}

	// Single presses
	if (config_.pressType == ButtonConfig::Single && nowPressed){
		if (actionIsMouseEvent()){
			mouseButtonPresses();
			mouseButtonReleases();
		}
		if (config_.action == ButtonConfig::KeyPress){
			commons::simulateKeyPress(config_.key, config_.keyModifiers);
			commons::simulateKeyRelease(config_.key, config_.keyModifiers);
		}
	}

	// rapid fire presses
	if (config_.pressType == ButtonConfig::Rapid){
		static bool previouslyPressed = false;
		// Alternate pressing and releasing so that a game has time to read its state
		// and finishes on a release
		if (previouslyPressed) // release
		{
			if (actionIsMouseEvent()){
				mouseButtonReleases();
			}
			if (config_.action == ButtonConfig::KeyPress){
				commons::simulateKeyRelease(config_.key, config_.keyModifiers);
				
			}
			previouslyPressed = false;
		}
		else if (nowHeld) // press
		{
			if (actionIsMouseEvent()){
				mouseButtonPresses();
			}
			if (config_.action == ButtonConfig::KeyPress){
				commons::simulateKeyPress(config_.key, config_.keyModifiers);
			}
			previouslyPressed = true;
		}
	}

	held_ = nowHeld;
}