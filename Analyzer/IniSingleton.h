#pragma once
#include "IniLinux.h"
#include "general.h"

class CIniSingleton
{
public:
								~CIniSingleton(void);
	static						CIniSingleton* getInstance();

	void						SetKey(std::string param){ m_key = param; }
	std::string					GetKey(){ return m_key; }


	void						SetCSCoins(std::string param){ m_csCoins = param; }
	std::string					GetCSCoins(){ return m_csCoins; }



	void						SetCoinNumber(int no) { m_coinNumber = no; }
	int							GetCoinNumber() { return m_coinNumber; }


	void						SetVIPCoins(std::string param) { m_csVipCoins = param; }
	std::string					GetVIPCoins() { return m_csVipCoins; }

private:
	CIniSingleton(void);
	static CIniSingleton*		m_pInstance;
	CIniLinux					m_IniReader;
	char						m_Currentpath[MAX_PATH];
	std::string					m_iniFileName;
	std::string					m_key;
	std::string					m_csCoins;// Comma separated coins.


	int							m_coinNumber;

	std::string					m_csVipCoins;
};