#include "stdafx.h"
#include "Controller.h"
#include "general.h"
#include <string>
#include <json.h>
#include <writer.h>
#include <reader.h>
#include <value.h>
#include <json_batchallocator.h>
#include <sstream>
#include <iostream>
#include "CoinNode.h"
#include <thread>
#include <algorithm>
#include "IniSingleton.h"
#include <sstream>
#include <iostream>
#include <vector>

using namespace Json;

bool compare(signal_t a, signal_t b)
{
	if (a.percent >= b.percent)
		return 1;
	else
		return 0;
}

CController::CController()
{
	SDL_Init(SDL_INIT_AUDIO);
	//std::string stemp = GetRunningPath();
	//stemp += "Audio\\Warn.wav";
	m_pAudioPlayer = new CAudioPlayer();

	CIniSingleton* m_instance = CIniSingleton::getInstance();
	m_report	 = false;
	m_address	 = m_instance->GetAddress();
	m_key		 = m_instance->GetKey();
	m_coins		 = m_instance->GetCSCoins();
	m_coinNumber = m_instance->GetCoinNumber();
	m_vipCoins   = m_instance->GetVIPCoins();
	ProcessCoins();

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_t1);
	m_time_idx	= 1;
	m_Running	= true;
	m_thread	= std::thread(std::bind(&CController::MainLoop, this));
}


int CController::MainLoop()
{
	while (m_Running)
	{

		if (Process_REST())
		{
			/*
			system("cls");
			PrintTime();
			*/
			Process_Analyze();
		}		
		waitForSeconds(60);// process every one minute.
	}
	return SUCCESS;
}

int CController::ProcessCoins()
{
	std::stringstream s_stream(m_coins); //create string stream from the string
	while (s_stream.good()) 
	{
		std::string substr;
		getline(s_stream, substr, ','); //get first string delimited by comma
		if (substr.length())
		{
			substr.erase(substr.find_last_not_of(" \n\r\t") + 1);
			//substr.erase(substr.find_first_not_of(" \n\r\t") + 1);
			m_CoinNames.push_back(substr);
		}
	}

	for (u16 i = 0; i<m_CoinNames.size(); i++)
	{
		std::string stemp	= m_CoinNames.at(i);
		CCoinNode* ptr		= new CCoinNode(this, stemp);
		// Print all splitted strings.
		//std::cout << m_CoinNames.at(i) << std::endl;
		m_Coins[stemp]		= nullptr;
		m_Coins[stemp]		= ptr;

	}
	//////////////////////////////////////////////
	std::stringstream m_vip(m_vipCoins); //create string stream from the string
	while (m_vip.good())
	{
		std::string substr;
		getline(m_vip, substr, ','); //get first string delimited by comma
		if (substr.length())
		{
			substr.erase(substr.find_last_not_of(" \n\r\t") + 1);
			//substr.erase(substr.find_first_not_of(" \n\r\t") + 1);
			m_VIP.insert(substr);
		}
	}




	return SUCCESS;
}


void CController::Process_Analyze()
{
	int				res;
	std::string		name;
	double			rate;
	double			percent;
	double			hitRate;
	bool			report;

	m_report		= false;
	m_idx			= 0;

	for (auto const& node : m_Coins)
	{
		res = node.second->Process(name,rate,percent,hitRate,report);
		if (res == SIGNAL)
		{
			// Add node to array.
			m_signals[m_idx].name		= name;
			m_signals[m_idx].percent	= percent;
			m_signals[m_idx].rate		= rate;
			m_signals[m_idx].hitrate	= hitRate;
			m_idx++;
			if (report)
			{
				m_report = true;
			}
		}

	}
	// Show the result.
	ShowResult();
}

void CController::ShowResult()
{
	char			tempBuffer[256];
	system("cls");
	PrintTime();

	if (m_idx)
	{


		std::sort(m_signals, m_signals + m_idx, compare);
		color_printf(GREEN, "%-32s%-16s%-16s%-16s\n", "NAME", "PERCENT", "RATE", "HIT RATE");
		sprintf(tempBuffer, "%-32s%-16.4f", "BITCOIN", m_Coins["BTC"]->GetRate());
		LOGIData(tempBuffer, CYAN);
		
		for (int i = 0; i < m_idx; i++)
		{
			sprintf(tempBuffer, "%-32s%-16.4f%-16.6f%-16.4f", m_signals[i].name.c_str(), m_signals[i].percent, m_signals[i].rate,m_signals[i].hitrate);
			if (m_VIP.find(m_signals[i].name) != m_VIP.end())
			{
				LOGIData(tempBuffer, YELLOW);
			}
			else
			{
				LOGIData(tempBuffer, GREEN);
			}
		}

		if (m_report)
		{
			std::string stemp = GetRunningPath();
			stemp += "Audio\\Signal.wav";
			m_pAudioPlayer->PlayWave(stemp.c_str());
			m_report = false;
		}
	}
}

void CController::Reset(std::string param)
{
	if (m_Coins.find(param) != m_Coins.end())
	{
		m_Coins[param]->Reset();
	}
}




bool CController::Process_RESTChunk(std::string coins)
{
	int cntr = 0;
	std::string		command;
	char			path[8192];
	std::string		stemp = GetRunningPath();

	stemp += "service_result.dat";
	memset(path, 0, 8192);

	//sprintf(path, "curl.exe -X POST -H \"Content-Type: application/json\"  -H \"x-api-key:%s \"   -d \"{\\\"currency\\\":\\\"USD\\\",\\\"sort\\\":\\\"rank\\\",\\\"order\\\":\\\"ascending\\\",\\\"offset\\\":0,\\\"limit\\\":%d,\\\"meta\\\":false}\"    %s ", m_key.c_str(),m_coinNumber, m_address.c_str());	
	//sprintf(path, "curl.exe -X GET -H \"Content-Type: application/json\"  -d \"{\\\"currency\\\":\\\"USD\\\",\\\"sort\\\":\\\"rank\\\",\\\"order\\\":\\\"ascending\\\",\\\"offset\\\":0,\\\"limit\\\":%d,\\\"meta\\\":false}\"    %s ",  , m_address.c_str());
	//sprintf(path,   "curl.exe -X GET \"https://api.coingecko.com/api/v3/simple/price?ids=%s&vs_currencies=usd\" -H \"accept: application/json\"", m_coins.c_str());
	//sprintf(path, "curl.exe -X GET \"https://min-api.cryptocompare.com/data/pricemulti?fsyms=%s&tsyms=USD&api_key={%s}\"", coins.c_str(), m_key.c_str());
	sprintf(path, "https://min-api.cryptocompare.com/data/pricemulti?fsyms=%s&tsyms=USD&api_key={%s}", coins.c_str(), m_key.c_str());

	auto json = get_response(path);

/*
	command = path;
	command += " --output ";
	command += stemp;
	system(command.c_str());
*/


	//FILE* f = fopen(stemp.c_str(), "rb");
	if (json.str().length())
	{
/*
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		char* buffer = new char[size + 1];
		memset(buffer, 0, size + 1);
		fread(buffer, 1, size, f);
		fclose(f);
		// Remove the file.
		command = "del ";
		command += stemp;
		system(command.c_str());
*/
		/*
		if (size<1000)
		{
		return false;
		}
		*/


		////////////////////////////////////////////////////
		Json::Reader						reader;
		Json::Value							root;
		Json::Value							val;
		////////////////////////////////////////////////////
		Json::Value							st0;
		Json::Value							st1;
		Json::Value							st2;
		Json::Value							st3;

		int									a;
		int								    b;
		////////////////////////////////////////////////////
		std::string							key;
		std::string							usd;
		std::string							data;
		std::string							code;
		double								rate;
		////////////////////////////////////////////////////
		std::string							str = json.str();
		try
		{
			if (!reader.parse(str, root))
			{
			}
			else
			{
				int idxx = 0;
				for (Json::Value::iterator itr0 = root.begin(); itr0 != root.end(); itr0++)
				{
					code = "";
					rate = -1;
					st0 = itr0.key();
					a = itr0.key().type();

					switch (a)
					{
					case stringValue:
						code = st0.asString();
						st1 = root[st0.asString().c_str()];
						b = st1.type();
						switch (b)
						{
						case objectValue:
						{
							val = root[st0.asString().c_str()];
							if (val.type() == objectValue)
							{
								for (Json::Value::iterator itr = val.begin(); itr != val.end(); itr++)
								{
									std::string key = itr.key().asString().c_str();
									if (key == "usd" || key == "USD")
									{
										rate = val[key].asDouble();
										if (m_Coins.find(code) != m_Coins.end())
										{
											m_Coins[code]->AddRate(rate);
											cntr++;
										}
									}
								}
							}
						}
						break;
						}
						break;
					case intValue:
					case uintValue:
					{

						int idx = st0.asInt();
						val = root[idx];
						if (val.type() == objectValue)
						{
							for (Json::Value::iterator itr = val.begin(); itr != val.end(); itr++)
							{
								std::string key = itr.key().asString().c_str();
								if (key == "code")
								{
									code = val[key.c_str()].asString();
								}
								else if (key == "rate")
								{
									rate = val[key.c_str()].asDouble();
									//color_printf(YELLOW,"%d)%s:%f\n",idxx++, code.c_str(), rate);
									if (m_Coins.find(code) != m_Coins.end())
									{
										m_Coins[code]->AddRate(rate);
										cntr++;
									}
								}
							}
						}

					}
					break;
					case arrayValue:
						break;
					}
				}
			}
		}
		catch (...)
		{

		}
		///////////////////////////////////////////////

		//delete[] buffer;
		return true;
	}
	return false;
}

bool CController::Process_REST()
{
	bool alpha;
	m_coins;
	std::istringstream f(m_coins.c_str());
	std::string s;
	std::string coins;
	while (std::getline(f, s, ',')) 
	{
		coins += s;		
		coins += ",";

		if (coins.length() >= 290)
		{			
			alpha = Process_RESTChunk(coins);
			if (alpha == false)
			{
				return false;
			}
			coins = "";
		}
	}
	if (coins.length())
	{
		if (coins[coins.length() - 1] == ',')
		{
			coins[coins.length() - 1] = 0;
		}
		alpha = Process_RESTChunk(coins);
		if (alpha == false)
		{
			return false;
		}
		coins = "";
	}
	return true;
}

 
CController::~CController()
{
	m_Running = false;
	if (m_thread.joinable())
	{
		m_thread.join();
	}

	for (auto const& x : m_Coins)
	{
		delete x.second;
	}
	m_Coins.clear();
	delete m_pAudioPlayer;
	SDL_Quit();
}

void CController::waitForSeconds(int sec)
{
	QueryPerformanceCounter(&m_t2);
	m_elapsedTime_in_ms = (m_t2.QuadPart - m_t1.QuadPart) * 1000.0 / m_frequency.QuadPart;

	while (m_elapsedTime_in_ms < sec * 1000 * m_time_idx)
	{
		Sleep(125);
		QueryPerformanceCounter(&m_t2);
		m_elapsedTime_in_ms = (m_t2.QuadPart - m_t1.QuadPart) * 1000.0 / m_frequency.QuadPart;
		if (m_Running == false)
		{
			break;
		}
	}
	m_time_idx++;
}