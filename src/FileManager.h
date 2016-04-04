#include "FileDialog.h"
#include "UIWidgets.h"
#include <array>
#include <fstream>
#pragma once

// Hanldes loading and saving for this program's ".gkcc" files.
//  File format: text numbers separated by spaces in this order: poll resolution; axis configs; button configs
class FileManager {
	FileDialog fileDialog_;

	OPENFILENAME	constructOpenFileName();
	void			writeAxisConfig(AxisConfig c, std::ofstream& os);
	AxisConfig		readAxisConfig(std::ifstream& is);
	void			writeButtonConfig(ButtonConfig c, std::ofstream& os);
	ButtonConfig	readButtonConfig(std::ifstream& is);

public:
	FileManager(){}

	void	saveFile(const std::wstring& fileName, const UIWidgets& settings, const int pollResolution); // allows a file to be saved without opening a dialog
	void	loadFile(const std::wstring& fileName, UIWidgets& settings, int& pollResolution); // allows a file to be read without opening a dialog
	void	openBrowserAndSave(const UIWidgets& settings, const int pollResolution); // opens a dialog for the user to enter a file name, then saves a .gkcc file
	void	openBrowserAndLoad(UIWidgets& settings, int& pollResolution); // opens a dialog for the user to select a file, then reads it
};