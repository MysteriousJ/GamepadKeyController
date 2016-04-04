#include "Config.h"
#pragma once

class AxisControl {
	AxisConfig	config_;
	float		value_;
	int			direction_; // -1 for left, 0 for center, 1 for right

public:
	AxisControl(){}

	void		update(float newValue, int updatesPerSecond);
	void		setConfig(AxisConfig config);

	float		value() const;
	AxisConfig	config() const;
};