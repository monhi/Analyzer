#pragma once
#include "IniLinux.h"
#include "general.h"

class CIniSingleton
{
public:
					~CIniSingleton(void);
	static			CIniSingleton* getInstance();

	void						SetKey(std::string param){ m_key = param; }
	std::string					GetKey(){ return m_key; }

	void						SetAddress(std::string param){ m_address = param; }
	std::string					GetAddress(){ return m_address; }


	void						SetCSCoins(std::string param){ m_csCoins = param; }
	std::string					GetCSCoins(){ return m_csCoins; }

private:
	CIniSingleton(void);
	static CIniSingleton*		m_pInstance;
	CIniLinux					m_IniReader;
	char						m_Currentpath[MAX_PATH];
	std::string					m_iniFileName;
	std::string					m_key;
	std::string					m_address;
	std::string					m_csCoins;// Comma separated coins.
};