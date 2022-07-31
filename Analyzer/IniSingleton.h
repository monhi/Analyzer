#pragma once
#include "IniLinux.h"
#include "general.h"

class CIniSingleton
{
public:
					~CIniSingleton(void);
	static			CIniSingleton* getInstance();

	void						SetSrcDir(std::string param){ m_src_dir = param; }
	std::string					GetSrcDir(){ return m_src_dir; }
	void						SetDstDir(std::string param){ m_dst_dir = param; }
	std::string					GetDstDir(){ return m_dst_dir; }
	void						SetLicense(std::string param){ m_license = param; }
	std::string					GetLicense(){ return m_license; }


private:
	CIniSingleton(void);
	static CIniSingleton*		m_pInstance;
	CIniLinux					m_IniReader;
	char						m_Currentpath[MAX_PATH];
	std::string					m_iniFileName;
	std::string					m_src_dir;
	std::string					m_dst_dir;
	std::string					m_license;
};