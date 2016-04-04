#include "MainWindow.h"
#include "commons.h"
#include "Windowsx.h"

bool MainWindow::processWndMsg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CREATE){
		// innitialize the axis and button widgets
		widgets_.init(hwnd);

		// init the gamepad and display a message if it isn't working
		if (!joy_.init(0)){
			showMessage(L"No gamepad detected. \nPlug in a gamepad and \nresart the program.", MB_ICONERROR);
		}

		// start update "loop"
		setUpdateTimer(pollResolution_, hwnd);
		
		// if the user chose to open a .GKCC file with this program, load the file
		int numOfArgs;
		LPWSTR* texts = CommandLineToArgvW(GetCommandLine(), &numOfArgs);
		if(numOfArgs >= 2){
			// the file address is the second string in the command line arguments
			fileManager_.loadFile(texts[1], widgets_, pollResolution_);
		}
		return 0;
	}

	if(message == WM_TIMER){ // this is the update loop
		if(active_){
			joy_.pollUpdate();
			widgets_.update(joy_, pollResolution_);
		}
		return 0;
	}

	if(message == WM_SIZE){
		int windowWidth = LOWORD(lParam);
		int windowHeight = HIWORD(lParam);
		widgets_.resize(windowWidth, windowHeight);		
		return 0;
	}

	if(message == WM_COMMAND){
		menuFunctions(hwnd, wParam);
		return 0;
	}

	if(message == WM_DESTROY){
		PostQuitMessage(0);
		return 0;
	}

	return false;
}

void MainWindow::showMessage(std::wstring message, UINT typeFlag){
	MessageBox (NULL, message.c_str(), TEXT (""), typeFlag);
}

void MainWindow::setUpdateTimer(int updatesPerSecond, HWND window){
	static const int milisecondsPerSecond = 1000;
	SetTimer(window, 1, milisecondsPerSecond / pollResolution_, NULL);
}

void MainWindow::menuFunctions(HWND hwnd, WPARAM wParam){
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);
	HMENU hmenu = GetMenu(hwnd);

	switch(LOWORD(wParam)){
	case IDM_SAVE:
		fileManager_.openBrowserAndSave(widgets_, pollResolution_);
		break;
	case IDM_LOAD:
		fileManager_.openBrowserAndLoad(widgets_, pollResolution_);
		break;
	case IDM_ACTIVE:
		// Togle 'Active' check box
		if (CheckMenuItem(hmenu, IDM_ACTIVE, MF_CHECKED) == MF_UNCHECKED)
			active_ = true;
		else{
			CheckMenuItem(hmenu, IDM_ACTIVE, MF_UNCHECKED);
			active_ = false;
		}
		break;
	case IDM_RESOLUTION:
		if(DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_RESOLUTION), hwnd, resolutionDialogProc, (LPARAM) &pollResolution_))
			// set update loop calls per second
			setUpdateTimer(pollResolution_, hwnd);
		break;
	case IDM_SWITCHGAMEPAD:
		/* if it isn't able to switch to the next gamepad, switch to gamepad 0. if it
		isn't able to switch to gamepad 0, show an error. Othewise, show a message
		saying what game pad is now being used */
		joystickIndex_++;
		if(!joy_.init(joystickIndex_)){
			joystickIndex_ = 0;
			if(!joy_.init(0))
				showMessage(L"No gamepad detected", MB_ICONERROR);
			else
				showMessage(L"Now using gamepad 0", 0);
		}
		else
			showMessage(L"Now using gamepad " + commons::numberToWString(joystickIndex_), 0);
		break;
	case IDM_ABOUT:
		showMessage(L"Program written by Jacob Bell", 0);
		break;
	}
}

BOOL CALLBACK MainWindow::resolutionDialogProc(HWND hdlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	static int* pResolution;
	static std::wstring	resString;

	if (message == WM_INITDIALOG){
		pResolution = (int*) lParam;
		// set resolution field to previous resolution
		Edit_SetText(GetDlgItem(hdlg, IDC_POLLSPERSECOND), commons::numberToWString(*pResolution).c_str());
		SetFocus(GetDlgItem(hdlg, IDC_POLLSPERSECOND));
	}
	if (message == WM_COMMAND){
		switch(LOWORD(wParam)){
		case IDOK:
			*pResolution = int(commons::getNumberFromEditBox(GetDlgItem(hdlg, IDC_POLLSPERSECOND)));
			EndDialog(hdlg, 1);
			return true;
		case IDCANCEL:
			EndDialog(hdlg, 0);
			return false;
		case IDC_POLLSPERSECOND:
			commons::restrictEditBoxToNumbers(GetDlgItem(hdlg, IDC_POLLSPERSECOND), resString);
		}
	}
	return false;
}