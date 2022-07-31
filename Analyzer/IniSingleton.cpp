#include "IniSingleton.h"
#include <Shlwapi.h>

CIniSingleton* CIniSingleton::m_pInstance = NULL;

CIniSingleton::CIniSingleton(void)
{
	std::string stemp;
	GetModuleFileNameA(NULL,m_Currentpath,MAX_PATH);
	PathRemoveFileSpecA(m_Currentpath);
	m_iniFileName  = m_Currentpath;
	m_iniFileName += "\\Config.ini";
	m_IniReader.setINIFileName(m_iniFileName);
	if( PathFileExistsA(m_iniFileName.c_str()) == false ) // Create File.
	{
		m_IniReader.setKey("c:\\","SOURCEDIR"	,"GENERAL");
		m_IniReader.setKey("D:\\", "DESTDIR"	,"GENERAL");
		m_IniReader.setKey("asdfasdfasdfasdf"	,"LICENSE" , "GENERAL");
	}
	///////////////////////////////////////////////////////////
	stemp = m_IniReader.getKeyValue("SOURCEDIR","GENERAL");
	if ( stemp[stemp.length() - 1] != '\\' )
	{
		stemp += "\\";
	}
	SetSrcDir(stemp);
	stemp = m_IniReader.getKeyValue("DESTDIR", "GENERAL");
	if ( stemp[stemp.length() - 1] != '\\' )
	{
		stemp += "\\";
	}
	SetDstDir(stemp);
	stemp = m_IniReader.getKeyValue("LICENSE", "GENERAL");
	SetLicense(stemp);
	///////////////////////////////////////////////////////////
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

