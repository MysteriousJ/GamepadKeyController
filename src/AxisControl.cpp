#include "AxisControl.h"
#include "commons.h"

AxisConfig AxisControl::config() const {
	return config_;
}

float AxisControl::value()const {
	return value_;
}

void AxisControl::setConfig(AxisConfig config){
	config_ = config;
}

void AxisControl::update(float newValue, int updatesPerSecond)
{
	// set direction
	int newDirection = 0;
	if (newValue < -config_.threshold)	newDirection = -1;
	else if (newValue > config_.threshold) newDirection = 1;

	// move mouse
	if (config_.action == AxisConfig::MouseMove && newDirection != 0){
		int h, v;
		AxisConfig::Direction* p;
		if (newDirection == -1)
			p = &config_.negMouseDirection;
		if (newDirection == 1)
			p = &config_.posMouseDirection;

		if (*p == AxisConfig::Left)	{h = -1;	v = 0;}
		if (*p == AxisConfig::Right){h = 1;		v = 0;}
		if (*p == AxisConfig::Up)	{h = 0;		v = -1;}
		if (*p == AxisConfig::Down)	{h = 0;		v = 1;}

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		SetCursorPos(cursorPos.x + ((h * pow(double(abs(newValue)),config_.sensitivity) * config_.speed) / updatesPerSecond),
					 cursorPos.y + ((v * pow(double(abs(newValue)),config_.sensitivity) * config_.speed) / updatesPerSecond));
	}

	if (config_.action == AxisConfig::KeyPress){
		// key presses
		if (newDirection == -1 && direction_ != -1)
			commons::simulateKeyPress(config_.negKey, config_.negKeyModifiers);
		if (newDirection == 1 && direction_ != 1)
			commons::simulateKeyPress(config_.posKey, config_.posKeyModifiers);
		// key releases
		if (direction_ == -1 && newDirection != -1)
			commons::simulateKeyRelease(config_.negKey, config_.negKeyModifiers);
		if (direction_ == 1 && newDirection != 1)
			commons::simulateKeyRelease(config_.posKey, config_.posKeyModifiers);
	}

	value_ = newValue;
	direction_ = newDirection;
}