#include "AxisWidget.h"
#include "commons.h"
#include "resource.h"
#include "Windowsx.h"
#include "Commctrl.h"

int AxisWidget::width() const{
	return width_;
}
int AxisWidget::height() const{
	return height_;
}
AxisConfig AxisWidget::config() const{
	return axis_.config();
}

void AxisWidget::setConfig(AxisConfig config){
	axis_.setConfig(config);
	// set combo boxe to the correct selection
	SendMessage(comboBox_, CB_SETCURSEL, (WPARAM)config.action, 0);
}

void AxisWidget::init(HWND parentWindow)
{
	hwnd_ = commons::createChildWindow(parentWindow, L"axisSeetingClass", axisSettingWndProc, sizeof(AxisWidget*), this);

	LPCWSTR comboBoxItems[] = {L"none", L"Move Mouse", L"Key Press"};
	comboBox_ = commons::createComboBox(hwnd_, comboBoxItems, 3);
}

void AxisWidget::move(int x, int y, int width)
{
	width_ = width;
	barWidth_ = width/2;
	MoveWindow(hwnd_, x, y, width_, height_, TRUE);
}

void AxisWidget::update(float value, int updatesPerSecond)
{
	if(axis_.value() != value){
		RECT region = {0, 0, barWidth_, height_};
		InvalidateRect(hwnd_, &region, true);
	}
	
	axis_.update(value, updatesPerSecond);
}

LRESULT CALLBACK AxisWidget::axisSettingWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE){
		LPCREATESTRUCT cs = (LPCREATESTRUCT) lParam;
		SetWindowLongPtr(hwnd, GWL_USERDATA, (long)cs->lpCreateParams);
	}
	
	// have the AxisWidget owning this window process its messages
	AxisWidget* as = (AxisWidget *) GetWindowLongPtr(hwnd, GWL_USERDATA);

	if (as)
		if(as->processWndMsg(hwnd, message, wParam, lParam)) // if as processes the message, return 0
			return 0;

	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool AxisWidget::processWndMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE){
		int comboBoxX = barWidth_ + 10;
		MoveWindow(comboBox_, comboBoxX, 0, width()-comboBoxX, 500, true);
		return true;
	}
	if (message == WM_COMMAND){
		HINSTANCE hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);

		if (LOWORD(wParam) == GetWindowLong(comboBox_, GWL_ID) // if the combo box has been changed
			&& HIWORD(wParam) == CBN_SELCHANGE)
		{
			const char	none = 0, moveMouse = 1, keyPress = 2;

			AxisConfig c = axis_.config();
			int result = SendMessage(comboBox_, CB_GETCURSEL, 0, 0);
			switch(result){
			case none:
				c.action = AxisConfig::None;
				break;
			case moveMouse:
				c.action = AxisConfig::MouseMove;
				DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_AXISMOVEMOUSE), hwnd_, moveMouseDialogProc, (LPARAM) &c);
				break;
			case keyPress:
				c.action = AxisConfig::KeyPress;
				DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_AXISKEYPRESS), hwnd_, keyPressDialogProc, (LPARAM) &c);
			}
			axis_.setConfig(c);
		}
		return true;
	}
	if (message == WM_PAINT){
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		int axisCenter = barWidth_/2;

		// this bar starts at zero and ends at the value of the axis
		SelectObject(hdc, CreateSolidBrush(RGB(255,0,255))); // purple
		Rectangle(hdc, 0, 0, axisCenter+(axis_.value()*axisCenter), height_);

		// this bar starts at the value of the axis and ends at the bar's width
		DeleteObject(SelectObject(hdc, CreateSolidBrush(RGB(0,0,255)))); // blue
		Rectangle(hdc, axisCenter+(axis_.value()*axisCenter), 0, barWidth_, height_);

		DeleteObject(SelectObject(hdc, GetStockObject(BLACK_BRUSH)));
		EndPaint(hwnd, &ps);
		return true;
	}

	return false;
}

BOOL CALLBACK AxisWidget::moveMouseDialogProc(HWND hdlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	static const int	stringSize = 8;
	static AxisConfig*	pConfig;
	static std::wstring	speed,
						sensitivity,
						threshold;

	if (message == WM_INITDIALOG){
		pConfig = (AxisConfig*) lParam;
		// If the values have not previously been set (they're defalut initialized to 0), give them a default example value
		Edit_SetText(GetDlgItem(hdlg, IDC_PIXELSPERSECOND), pConfig->speed ? commons::numberToWString(pConfig->speed).c_str()  :  L"2048");
		Edit_SetText(GetDlgItem(hdlg, IDC_SENSITIVITY), pConfig->sensitivity ? commons::numberToWString(pConfig->sensitivity).c_str()  :  L"2");
		Edit_SetText(GetDlgItem(hdlg, IDC_THRESHOLD), pConfig->threshold ? commons::numberToWString(pConfig->threshold).c_str()  :  L"0.25");
		/*  use the values of the radio button IDs plus the values of the enums
			to decide which radio buttons to tick...*/
		CheckRadioButton(hdlg, IDC_NLEFT, IDC_NDOWN, IDC_NLEFT + pConfig->negMouseDirection);
		CheckRadioButton(hdlg, IDC_PLEFT, IDC_PDOWN, IDC_PLEFT + pConfig->posMouseDirection);
	}
	if (message == WM_COMMAND){
		int item = LOWORD(wParam);
		switch(item){
		case IDOK :
			pConfig->speed = commons::getNumberFromEditBox(GetDlgItem(hdlg, IDC_PIXELSPERSECOND));
			pConfig->sensitivity = commons::getNumberFromEditBox(GetDlgItem(hdlg, IDC_SENSITIVITY));
			pConfig->threshold = commons::getNumberFromEditBox(GetDlgItem(hdlg, IDC_THRESHOLD));
			// negative radio buttons
			if (IsDlgButtonChecked(hdlg, IDC_NLEFT)) pConfig->negMouseDirection = AxisConfig::Left;
			if (IsDlgButtonChecked(hdlg, IDC_NRIGHT)) pConfig->negMouseDirection = AxisConfig::Right;
			if (IsDlgButtonChecked(hdlg, IDC_NUP)) pConfig->negMouseDirection = AxisConfig::Up;
			if (IsDlgButtonChecked(hdlg, IDC_NDOWN)) pConfig->negMouseDirection = AxisConfig::Down;
			// positive radio buttons
			if (IsDlgButtonChecked(hdlg, IDC_PLEFT)) pConfig->posMouseDirection = AxisConfig::Left;
			if (IsDlgButtonChecked(hdlg, IDC_PRIGHT)) pConfig->posMouseDirection = AxisConfig::Right;
			if (IsDlgButtonChecked(hdlg, IDC_PUP)) pConfig->posMouseDirection = AxisConfig::Up;
			if (IsDlgButtonChecked(hdlg, IDC_PDOWN)) pConfig->posMouseDirection = AxisConfig::Down;
			// fall through
		case IDCANCEL:
			EndDialog(hdlg, 0);
			return true;
		case IDC_PIXELSPERSECOND:
			commons::restrictEditBoxToNumbers(GetDlgItem(hdlg, item), speed);
		case IDC_SENSITIVITY:
			commons::restrictEditBoxToNumbers(GetDlgItem(hdlg, item), sensitivity);
		case IDC_THRESHOLD:
			commons::restrictEditBoxToNumbers(GetDlgItem(hdlg, item), threshold);
		}
	}
	return false;
}

BOOL CALLBACK AxisWidget::keyPressDialogProc(HWND hdlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	static AxisConfig*	pConfig;
	static std::wstring	threshold;

	if (message == WM_INITDIALOG){
		pConfig = (AxisConfig*) lParam;
		Edit_SetText(GetDlgItem(hdlg, IDC_THRESHOLD), pConfig->threshold ? commons::numberToWString(pConfig->threshold).c_str()  :  L"0.25");
		// fill in hotkey field with the previously selected hotkey
		SendMessage(GetDlgItem(hdlg, IDC_NHOTKEY), HKM_SETHOTKEY, MAKEWORD(pConfig->negKey, pConfig->negKeyModifiers), 0);
		SendMessage(GetDlgItem(hdlg, IDC_PHOTKEY), HKM_SETHOTKEY, MAKEWORD(pConfig->posKey, pConfig->posKeyModifiers), 0);
		SetFocus(GetDlgItem(hdlg, IDC_NHOTKEY));
	}

	if(message == WM_COMMAND)
	{
		switch(LOWORD(wParam)){
			case IDOK:
				// hotkeys
				pConfig->negKey = LOBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_NHOTKEY), HKM_GETHOTKEY, 0, 0)));
				pConfig->negKeyModifiers = HIBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_NHOTKEY), HKM_GETHOTKEY, 0, 0)));
				pConfig->posKey = LOBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_PHOTKEY), HKM_GETHOTKEY, 0, 0)));
				pConfig->posKeyModifiers = HIBYTE(LOWORD(SendMessage(GetDlgItem(hdlg, IDC_PHOTKEY), HKM_GETHOTKEY, 0, 0)));
				// threshold box
				pConfig->threshold = commons::getNumberFromEditBox(GetDlgItem(hdlg, IDC_THRESHOLD));
				// fall through
			case IDCANCEL:
				EndDialog(hdlg, 0);
				return true;

			case IDC_THRESHOLD:
				commons::restrictEditBoxToNumbers(GetDlgItem(hdlg, IDC_THRESHOLD), threshold);
		}
	}
	return false;
}