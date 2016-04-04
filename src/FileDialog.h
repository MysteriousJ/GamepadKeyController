#include <windows.h>
#include "Windowsx.h"
#include <string>
#pragma once


class FileDialog {
	OPENFILENAME ofn_;

public:
	FileDialog();

	std::wstring getOpen(std::wstring fileExtension, std::wstring filter);
	std::wstring getSave(std::wstring fileExtension, std::wstring filter);
};