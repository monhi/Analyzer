// Analyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Controller.h"
#include "general.h"
#include <conio.h>
#include "IniSingleton.h"
#include "general.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	std::string coinStr;
	char logbuff[128];
	int i1, i2, i3, i4;
	std::string stemp;
	GetProgramVersionInfo(i1, i2, i3, i4);
	DisableQuickEdit();
	stemp = GetRunningPath();
	stemp += "LOG\\";
	CreateDirectoryA(stemp.c_str(), NULL);
	stemp += "Analyze.Log";
	plog::init(plog::debug, stemp.c_str(), (1024 * 1024), 5);
	CIniSingleton* m_instance = CIniSingleton::getInstance();

	// Disable Console Close button.
	// In this way program can be closed by menu item of software.
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, false);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);


	

	//ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, NAMED_MUTEX_CONSTANT_TEXT);

	if (!hMutex)
	{
		hMutex = CreateMutex(0, 0, NAMED_MUTEX_CONSTANT_TEXT);
	}
	else
	{
		LOGIData(" Another Instant of program is running.", GREEN);
		LOGIData(" Quit in 3 seconds...", GREEN);
		Sleep(3000);
		return FALSE;
	}

	CController* m_pController = new CController();
	system("cls");

	sprintf(logbuff, "Preparing to start Analyzer server version %d.%d.%d.%d  ...", i1, i2, i3, i4);
	LOGIData(logbuff,GREEN);
	//LOGIData("Analyzer server %d.%d.%d.%d is started. to quit: press ESC button ...", GREEN);
	
	int ch;
	while ((ch = _getch()) != 27)
	{
		switch (ch)
		{
			case 'h':
			case 'H':
			{
				color_printf(GREEN, "H,h: Show help \n");
				color_printf(GREEN, "R,r: Reset a coin \n");
				color_printf(GREEN, "S,s: Show previous data\n");
				color_printf(GREEN, "ESC: Exit Program\n");

			}
			break;
			case 's':
			case 'S':
			{
				m_pController->ShowResult();
			}
			break;
			case 'R':
			case 'r':
			{
				color_printf(GREEN, "Inuput coin name to reset:\n");
				std::cin >> coinStr;
				m_pController->Reset(coinStr);
			}
			break;
		}
		Sleep(200);
	}
	delete m_pController;
	sprintf(logbuff, "Finish Analyzer server version %d.%d.%d.%d  ...\n", i1, i2, i3, i4);
	LOGIData(logbuff, GREEN);

	Sleep(1000);
	delete m_instance;
	return SUCCESS;
}

