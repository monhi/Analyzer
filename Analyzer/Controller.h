#pragma once

#include "AudioPlayer.h"
#include <thread>
#include <string>
#include <map>
#include <vector>

class CCoinNode;
class CController
{
private:
	std::vector<std::string>			m_CoinNames;
	std::map<std::string, CCoinNode*>	m_Coins;
	std::string							m_address;
	std::string							m_key;
	std::string							m_coins;
	CAudioPlayer*						m_pAudioPlayer;
	std::thread							m_thread;
	bool								Process_REST();
	int									ProcessCoins();
public:
										 CController();
										~CController();
};