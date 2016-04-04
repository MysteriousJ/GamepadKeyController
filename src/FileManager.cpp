#include "FileManager.h"
#include "resource.h"
#include "commons.h"
#include "Windowsx.h"
#include <array>


OPENFILENAME FileManager::constructOpenFileName()
{
	OPENFILENAME ofn;
	TCHAR fileExtention[] = TEXT("gkcc");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize =		sizeof(OPENFILENAME);
	ofn.hwndOwner =			NULL;
	ofn.hInstance =			NULL;
	ofn.lpstrFilter =		TEXT("Gamepad Key Controller Configuration(*.GKCC)\0*.gkcc\0");
	ofn.lpstrCustomFilter =	NULL;
	ofn.nMaxCustFilter =	0;
	ofn.nFilterIndex =		0;
	ofn.lpstrFile =			NULL;
	ofn.nMaxFile =			MAX_PATH;
	ofn.lpstrFileTitle =	NULL;
	ofn.nMaxFileTitle =		MAX_PATH;
	ofn.lpstrInitialDir =	NULL;
	ofn.lpstrTitle =		NULL;
	ofn.Flags =				0;
	ofn.nFileOffset =		0;
	ofn.nFileExtension =	0;
	ofn.lpstrDefExt =		fileExtention;
	ofn.lCustData =			0L;
	ofn.lpfnHook =			NULL;
	ofn.lpTemplateName =	NULL;

	return ofn;
}

void FileManager::writeAxisConfig(AxisConfig c, std::ofstream& os)
{
	os	<< c.action		<< ' ' << c.negMouseDirection	<< ' ' << c.posMouseDirection	<< ' '
		<< c.speed		<< ' ' << c.sensitivity			<< ' ' << c.threshold			<< ' '
		<< c.negKey		<< ' ' << c.posKey				<< ' ' << c.negKeyModifiers		<< ' '
		<< c.posKeyModifiers << ' ';
}

void FileManager::writeButtonConfig(ButtonConfig c, std::ofstream& os){
	os	<< c.action << ' ' << c.pressType		<< ' '
		<< c.key	<< ' ' << c.keyModifiers	<< ' ';
}

AxisConfig FileManager::readAxisConfig(std::ifstream& is)
{
	AxisConfig c;
	int action, negMouseDirection, posMouseDirection;

	is	>> action	>> negMouseDirection >> posMouseDirection
		>> c.speed	>> c.sensitivity >> c.threshold
		>> c.negKey >> c.posKey >> c.negKeyModifiers >> c.posKeyModifiers;

	c.action = AxisConfig::Action(action);
	c.negMouseDirection = AxisConfig::Direction(negMouseDirection);
	c.posMouseDirection = AxisConfig::Direction(posMouseDirection);

	return c;
}

ButtonConfig FileManager::readButtonConfig(std::ifstream& is)
{
	ButtonConfig c;
	int action, pressType;

	is	>> action >> pressType >> c.key >> c.keyModifiers;

	c.action = (ButtonConfig::Action) action;
	c.pressType = (ButtonConfig::PressType) pressType;

	return c;
}


void FileManager::saveFile(const std::wstring& fileName, const UIWidgets& settings, const int pollResolution)
{
	//  File format: poll resolution; axis configs; button configs

	std::ofstream os(fileName.c_str());
	if (!os)
		MessageBox (NULL, TEXT ("Could not create file stream"), TEXT (""), MB_ICONERROR);
	else
	{
		os << pollResolution << ' ';
		for (int i=0; i<settings.supportedAxes; i++)
			writeAxisConfig(settings.axes.at(i).config(), os);
		for (int i=0; i<settings.supportedButtons; i++)
			writeButtonConfig(settings.buttons.at(i).config(), os);
	}
}

void FileManager::loadFile(const std::wstring& fileName, UIWidgets& settings, int& pollResolution)
{
	std::ifstream is(fileName.c_str());
	if(!is)
		MessageBox (NULL, TEXT ("Could not read file"), TEXT (""), MB_ICONERROR);
	else
	{
		is >> pollResolution;
		for (int i=0; i<settings.supportedAxes; i++)
			settings.axes[i].setConfig(readAxisConfig(is));
		for (int i=0; i<settings.supportedButtons; i++)
			settings.buttons[i].setConfig(readButtonConfig(is));
	}
}


void FileManager::openBrowserAndSave(const UIWidgets& settings, const int pollResolution)
{
	std::wstring file = fileDialog_.getSave(L"gkcc", L"Gamepad Key Controller Configuration(*.GKCC)\0*.gkcc\0");
	if(file != L"")
		saveFile(file, settings, pollResolution);
}

void FileManager::openBrowserAndLoad(UIWidgets& settings, int& pollResolution)
{
	std::wstring file = fileDialog_.getOpen(L"gkcc", L"Gamepad Key Controller Configuration(*.GKCC)\0*.gkcc\0");
	if(file != L"")
		loadFile(file, settings, pollResolution);
}