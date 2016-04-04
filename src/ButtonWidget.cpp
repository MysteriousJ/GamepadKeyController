#include "ButtonWidget.h"
#include "commons.h"
#include "resource.h"
#include "Commctrl.h" // needed for HKM_GETHOTKEY


int ButtonWidget::width() const {
	return width_;
}
int ButtonWidget::height() const {
	return height_;
}
ButtonConfig ButtonWidget::config() const{
	return button_.config();
}

void ButtonWidget::setConfig(ButtonConfig config){
	button_.setConfig(config);

	// set combo boxes to their correct selections
	SendMessage(comboBox1_, CB_SETCURSEL, (WPARAM)config.action, 0);
	SendMessage(comboBox2_, CB_SETCURSEL, (WPARAM)config.pressType, 0);
	EnableWindow(comboBox2_, (config.action != ButtonConfig::None));

	// special case for space bar
	if (config.action == ButtonConfig::KeyPress && config.key == VK_SPACE)
		SendMessage(comboBox1_, CB_SETCURSEL, (WPARAM)config.action+1, 0);
}

void ButtonWidget::init(HWND parentWindow)
{
	// create a child window for this module inside the main window
	hwnd_ = commons::createChildWindow(parentWindow, L"buttonSetting", buttonSettingWndProc, sizeof(ButtonWidget*), this);

	// create the combo boxes
	LPCWSTR child1Items[] = {L"none", L"Left Click", L"Middle Click", L"Right Click", L"Key Press", L"Space Bar", L"Enter"};
	comboBox1_ = commons::createComboBox(hwnd_, child1Items, 7);

	LPCWSTR child2Items[] = {L"Hold", L"Single", L"Rapid fire"};
	comboBox2_ = commons::createComboBox(hwnd_, child2Items, 3);
	EnableWindow(comboBox2_, false); // initialy disable the press-type selection box
}

void ButtonWidget::move(int x, int y, int width)
{
	width_ = width;
	MoveWindow(hwnd_, x, y, width_, height_, TRUE);
}

void ButtonWidget::update(bool held, bool pressed)
{
	if (button_.held() != held){
		// only the region with the circle representing the state of the button press needs to be refreshed
		RECT region = {0, 0, height_, height_};
		InvalidateRect(hwnd_, &region, true);
	}

	button_.update(held, pressed); // simulate key/mouse events based on button state
}


LRESULT CALLBACK ButtonWidget::buttonSettingWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE){
		LPCREATESTRUCT cs = (LPCREATESTRUCT) lParam;
		SetWindowLongPtr(hwnd, GWL_USERDATA, (long)cs->lpCreateParams);
	}
	
	// have the ButtonWidget owning this window process its messages for OOP
	ButtonWidget* bs = (ButtonWidget *) GetWindowLongPtr(hwnd, GWL_USERDATA);

	if (bs)
		if(bs->processWndMsg(hwnd, message, wParam, lParam)) // if bs processes the message, return 0
			return 0;

	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool ButtonWidget::processWndMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE){
		int comboBoxX = 66;
		MoveWindow(comboBox1_, comboBoxX, 0, width()-comboBoxX, 500, true);
		MoveWindow(comboBox2_, comboBoxX, 28, width()-comboBoxX, 500, true);
		return true;
	}
	if (message == WM_COMMAND){
		processCommandMsg(hwnd, message, wParam, lParam); // code for the combo boxes separated to keep it cleaner
		return true;
	}
	if (message == WM_PAINT){
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hwnd, &ps);
		// if the button is pressed, draw a red circle, if it is not, draw a white one (which looks transparent)
		if (button_.held())
			SelectObject(hdc, CreateSolidBrush(RGB(255,0,0))); // red
		else
			SelectObject(hdc, CreateSolidBrush(RGB(255,255,255))); // white
		Ellipse(hdc, 0, 0, height_, height_);
		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_BRUSH))); // clean up the brush
		EndPaint(hwnd, &ps);
		return true;
	}

	return false;
}

void ButtonWidget::processCommandMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);

	if (LOWORD(wParam) == GetWindowLong(comboBox1_, GWL_ID) // if comboBox1_ has been changed
		&& HIWORD(wParam) == CBN_SELCHANGE)
	{
		const char	none = 0, leftClick = 1, middleClick = 2,
					rightClick = 3, keyPress = 4, spaceBar = 5,
					enter = 6;

		ButtonConfig c = button_.config();
		int result = SendMessage(comboBox1_, CB_GETCURSEL, 0, 0);
		switch(result){
		case none:
			c.action = ButtonConfig::None;
			break;
		case leftClick:
			c.action = ButtonConfig::LeftClick;
			break;
		case middleClick:
			c.action = ButtonConfig::MiddleClick;
			break;
		case rightClick:
			c.action = ButtonConfig::RightClick;
			break;
		case keyPress:
			c.action = ButtonConfig::KeyPress;
			DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_BUTTONKEYPRESS), hwnd_, keyPressDialogProc, (LPARAM) &c);
			break;
		case spaceBar:
			c.action = ButtonConfig::KeyPress;
			c.key = VK_SPACE;
			c.keyModifiers = 0;
			break;
		case enter:
			c.action = ButtonConfig::KeyPress;
			c.key = VK_RETURN;
			c.keyModifiers = 0;
			break;
		}
		button_.setConfig(c);

		// Enable or dissable comboBox2_, based on comboBox1_
		switch(result)
		{
		case none:
			EnableWindow(comboBox2_, false);
			break;
		case leftClick: case middleClick: case rightClick: case keyPress: case spaceBar:
			EnableWindow(comboBox2_, true);
			break;
		}
	}

	if (LOWORD(wParam) == GetWindowLong(comboBox2_, GWL_ID) // if combBox2_ has been changed
		 && HIWORD(wParam) == CBN_SELCHANGE)
	{
		const char	hold = 0, single = 1, rapid = 2;

		ButtonConfig c = button_.config();
		int result = SendMessage(comboBox2_, CB_GETCURSEL, 0, 0);
		switch(result){
		case hold:
			c.pressType = ButtonConfig::Hold;
			break;
		case single:
			c.pressType = ButtonConfig::Single;
			break;
		case rapid:
			c.pressType = ButtonConfig::Rapid;
			break;
		}
		button_.setConfig(c);
	}
}

BOOL CALLBACK ButtonWidget::keyPressDialogProc(HWND hdlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	static ButtonConfig* pConfig;

	if (message == WM_INITDIALOG){
		pConfig = (ButtonConfig*) lParam; // get a pointer to the ButtonWidget's Config structure
		SetFocus(GetDlgItem(hdlg, IDC_HOTKEY));
		// fill in hotkey field with the previously selected hotkey
		SendMessage(GetDlgItem(hdlg, IDC_HOTKEY), HKM_SETHOTKEY, MAKEWORD(pConfig->key, pConfig->keyModifiers), 0);
	}
	if (message == WM_COMMAND){
		switch(LOWORD(wParam)){
		case IDOK:
			pConfig->key = LOBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_HOTKEY), HKM_GETHOTKEY, 0, 0)));;
			pConfig->keyModifiers = HIBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_HOTKEY), HKM_GETHOTKEY, 0, 0)));;
			EndDialog(hdlg, 0);
			return true;
		case IDCANCEL:
			EndDialog(hdlg, 0);
			return false;
		}
	}
	return false;
}