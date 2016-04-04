#include "UIWidgets.h"



void UIWidgets::init(HWND parentWindow){
	for (int i=0; i<axes.size(); i++)
		axes[i].init(parentWindow);
	for (int i=0; i<buttons.size(); i++)
		buttons[i].init(parentWindow);
}

void UIWidgets::update(const Joystick& joy, const int pollResolution){
	// process joystick and simulate key/mouse events
	for (int i=0; i<axes.size(); i++)
		axes[i].update(joy.axis(i), pollResolution);
	for (int i=0; i<buttons.size(); i++)
		buttons[i].update(joy.buttonHeld(i), joy.buttonPressed(i));
}

void UIWidgets::resize(int windowWidth, int windowHeight){
	// spacing between and around the modules
	int xPadding = 10,
		yAxisPadding = 12,
		yButtonPadding = 16;

	int axisColumns = 3, axisRows = 2,
		buttonColumns = 4, buttonRows = 4;

	int axisGroupHeight = (axes[0].height()*axisRows) + (yAxisPadding*axisRows);
	int buttonGroupHeight = (buttons[0].height()*buttonRows) + (yButtonPadding*buttonRows);
	int yButtonsPos = windowHeight - buttonGroupHeight; //39; //old value: 125
	if (yButtonsPos < axisGroupHeight){
		yButtonsPos = axisGroupHeight;
	}

	// resize axis modules
	const int axisWidth = (windowWidth/axisColumns) - ((xPadding*4)/axisColumns);
	int axis = 0;
	for (int i=0; i<axisColumns; i++){
		for (int j=0; j<axisRows; j++){
			int xPos = (i*axisWidth) + ((i+1)*xPadding);
			int yPos = (j*axes[j].height()) + ((j+1)*yAxisPadding);
			axes[axis].move(xPos, yPos, axisWidth);
			axis++;
		}
	}

	// resize button modules
	const int buttonWidth = (windowWidth/buttonColumns) - ((xPadding*5)/buttonColumns);
	int button = 0;
	for (int i=0; i<buttonColumns; i++){
		for (int j=0; j<buttonRows; j++){
			int xPos = (i*buttonWidth) + ((i+1)*xPadding);
			int yPos = j*(buttons[j].height()+yButtonPadding) + (yButtonsPos);
			buttons[button].move(xPos, yPos, buttonWidth);
			button++;
		}
	}
}