#pragma once

#include "AudioPlayer.h"
#include <thread>
#include <string>
#include <map>
#include <vector>
#include <atomic>
#include <set>
#include "general.h"


#define MAX_COINS   512

typedef struct
{
	std::string name;
	double      rate;

	double		maxRate;
	double		percent;
	//double	percent;
	//double	hitrate;
}	signal_t;


class CCoinNode;
class CController
{
private:
	///////////////////////////////////////////////
	LARGE_INTEGER						m_frequency;
	LARGE_INTEGER						m_t1, m_t2;
	double								m_elapsedTime_in_ms;
	///////////////////////////////////////////////
	std::set<std::string>				m_VIP;
	std::atomic<bool>					m_Running;
	std::vector<std::string>			m_CoinNames;
	std::map<std::string, CCoinNode*>	m_Coins;
	//std::string							m_address;
	std::string							m_key;
	std::string							m_coins;
	std::string							m_vipCoins;
	CAudioPlayer*						m_pAudioPlayer;
	std::thread							m_thread;
	int									m_time_idx;
	int									m_idx;
	signal_t						    m_signals[MAX_COINS];
	bool								m_reportMax;
	bool								m_reportMin;

private:
	void								waitForSeconds(int sec);
	bool								Process_REST();
	int									ProcessCoins();
	void								Process_Analyze();
public:
										CController();
										~CController();
	int									MainLoop();
	void								ShowResult();
	void								Reset(std::string param);
	bool								Process_RESTChunk(std::string coins);
};