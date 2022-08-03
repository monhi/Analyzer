// Analyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Controller.h"
#include "general.h"
#include <conio.h>
#include "IniSingleton.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char logbuff[128];
	int i1, i2, i3, i4;
	GetProgramVersionInfo(i1, i2, i3, i4);
	DisableQuickEdit();
	CreateDirectoryEx(NULL, L"C:\\GSSINT\\SLS_EMBEDDED\\LOG\\", NULL);
	CreateDirectory(L"C:\\GSSINT\\SLS_EMBEDDED\\LOG\\", NULL);
	std::string stemp = "c:\\GSSINT\\SLS_EMBEDDED\\LOG\\LynxSLS.Log";
	plog::init(plog::debug, stemp.c_str(), (1024 * 1024), 5);
	CIniSingleton* m_instance = CIniSingleton::getInstance();

	sprintf(logbuff, "Preparing to start Analyzer server version %d.%d.%d.%d  ...", i1, i2, i3, i4);

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
	LOGIData("Analyzer server is started. to quit: press ESC button ...", GREEN);
	system("cls");
	int ch;
	while ((ch = _getch()) != 27)
	{

		Sleep(200);
	}
	delete m_pController;
	sprintf(logbuff, "Finishing Analyzer server version %d.%d.%d.%d  ...\n", i1, i2, i3, i4);
	LOGIData(logbuff, GREEN);

	//PLOG_INFO << "Program exit.";
	Sleep(1000);
	delete m_instance;
	return SUCCESS;
}

