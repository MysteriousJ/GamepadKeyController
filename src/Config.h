#pragma once

struct ButtonConfig {
	// the order of these must match the order in the combo box
	enum Action {None, LeftClick, MiddleClick, RightClick, KeyPress};
	enum PressType {Hold, Single, Rapid};

	Action		action;
	PressType	pressType;
	int			key,
				keyModifiers;

	ButtonConfig():
		action(None), pressType(Hold), key(0), keyModifiers(0)
	{}
};

struct AxisConfig {
	// the order of these must match the order in the combo box
	enum Action {None, MouseMove, KeyPress};
	enum Direction {Left, Right, Up, Down};

	Action		action;
	Direction	negMouseDirection,
				posMouseDirection;
	double		speed,
				sensitivity,
				threshold;
	int			negKey,
				posKey,
				negKeyModifiers,
				posKeyModifiers;

	AxisConfig():
		negMouseDirection(Left), posMouseDirection(Right),
		speed(0), sensitivity(0), threshold(0),
		negKey(0), negKeyModifiers(0), posKey(0), posKeyModifiers(0),
		action(None)
	{}
};