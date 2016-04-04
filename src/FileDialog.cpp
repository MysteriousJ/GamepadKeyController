#include "FileDialog.h"

FileDialog::FileDialog()
{
	ZeroMemory(&ofn_, sizeof(ofn_));
	ofn_.lStructSize =			sizeof(OPENFILENAME);
	ofn_.hwndOwner =			NULL;
	ofn_.hInstance =			NULL;
	ofn_.lpstrFilter =			TEXT("Gamepad Key Controller Configuration(*.GKCC)\0*.gkcc\0");
	ofn_.lpstrCustomFilter =	NULL;
	ofn_.nMaxCustFilter =		0;
	ofn_.nFilterIndex =			0;
	ofn_.lpstrFile =			NULL;
	ofn_.nMaxFile =				MAX_PATH;
	ofn_.lpstrFileTitle =		NULL;
	ofn_.nMaxFileTitle =		MAX_PATH;
	ofn_.lpstrInitialDir =		NULL;
	ofn_.lpstrTitle =			NULL;
	ofn_.Flags =				0;
	ofn_.nFileOffset =			0;
	ofn_.nFileExtension =		0;
	ofn_.lpstrDefExt =			NULL;
	ofn_.lCustData =			0L;
	ofn_.lpfnHook =				NULL;
	ofn_.lpTemplateName =		NULL;
}

std::wstring FileDialog::getOpen(std::wstring fileExtension, std::wstring filter)
{
	TCHAR fileName[MAX_PATH];
	TCHAR fileTitle[MAX_PATH];

	ofn_.lpstrFile = fileName;
	ofn_.lpstrFile[0] = L'\0';
	ofn_.lpstrFileTitle = fileTitle;
	ofn_.lpstrDefExt = fileExtension.c_str();

	if(GetOpenFileName(&ofn_))
		return ofn_.lpstrFile;
	return std::wstring();
}


std::wstring FileDialog::getSave(std::wstring fileExtension, std::wstring filter)
{
	TCHAR fileName[MAX_PATH];
	TCHAR fileTitle[MAX_PATH];

	ofn_.lpstrFile = fileName;
	ofn_.lpstrFile[0] = L'\0';
	ofn_.lpstrFileTitle = fileTitle;
	ofn_.Flags = OFN_OVERWRITEPROMPT;
	ofn_.lpstrDefExt = fileExtension.c_str();

	if(GetSaveFileName(&ofn_))
		return ofn_.lpstrFile;
	return std::wstring();
}