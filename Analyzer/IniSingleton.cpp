#include "IniSingleton.h"
#include "general.h"
#include <Shlwapi.h>

CIniSingleton* CIniSingleton::m_pInstance = NULL;

CIniSingleton::CIniSingleton(void)
{
	std::string stemp;
	m_iniFileName  = GetRunningPath();
	m_iniFileName += "Config.ini";
	m_IniReader.setINIFileName(m_iniFileName);
	if( PathFileExistsA(m_iniFileName.c_str()) == false ) // Create File.
	{
		m_IniReader.setKey("type_serial_number_here", "KEY", "GENERAL");
		m_IniReader.setKey("type_address_here", "ADDRESS", "GENERAL");
		m_IniReader.setKey("type_comma_separated_coins_here", "COINS", "GENERAL");
	}
	////////////////////////////////////////////////////////
	stemp = m_IniReader.getKeyValue("KEY",		"GENERAL");
	SetKey(stemp);
	stemp = m_IniReader.getKeyValue("ADDRESS",	"GENERAL");
	SetAddress(stemp);
	stemp = m_IniReader.getKeyValue("COINS",	"GENERAL");
	SetCSCoins(stemp);
}

CIniSingleton::~CIniSingleton(void)
{

}

CIniSingleton* CIniSingleton::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CIniSingleton();
	}
	return m_pInstance;
}

