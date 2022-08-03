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

#include "IniSingleton.h"

using namespace Json;

CController::CController()
{
	SDL_Init(SDL_INIT_AUDIO);
	//std::string stemp = GetRunningPath();
	//stemp += "Audio\\Warn.wav";
	//m_pAudioPlayer = new CAudioPlayer();
	//m_pAudioPlayer->PlayWave(stemp.c_str());

	CIniSingleton* m_instance = CIniSingleton::getInstance();

	m_address	= m_instance->GetAddress();
	m_key		= m_instance->GetKey();
	m_coins		= m_instance->GetCSCoins();
	ProcessCoins();

	//Process_REST();
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
			m_CoinNames.push_back(substr);
		}
	}

	for (u16 i = 0; i<m_CoinNames.size(); i++)
	{
		// Print all splitted strings.
		std::cout << m_CoinNames.at(i) << std::endl;
		m_Coins[m_CoinNames.at(i)] = new CCoinNode(this, m_CoinNames.at(i));
	}
	return SUCCESS;
}

bool CController::Process_REST()
{
	std::string		command;
	char			path[MAX_PATH*4];
	std::string		stemp = GetRunningPath();

	stemp += "service_result.dat";
	memset(path, 0, MAX_PATH );

	sprintf(path, "curl.exe -X POST -H \"Content-Type: application/json\"  -H \"x-api-key:%s \"   -d \"{\\\"currency\\\":\\\"USD\\\",\\\"sort\\\":\\\"rank\\\",\\\"order\\\":\\\"ascending\\\",\\\"offset\\\":0,\\\"limit\\\":500,\\\"meta\\\":false}\"    %s ", m_key.c_str(), m_address.c_str());

	command = path;
	command += "--output ";
	command += stemp;
	system(command.c_str());

	FILE* f = fopen(stemp.c_str(), "rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		char* buffer = new char[size + 1];
		memset(buffer, 0, size + 1);
		fread(buffer, 1, size, f);
		fclose(f);
		// Remove the file.
		command = "rm ";
		command += stemp;
		system(command.c_str());
		if (size)
		{

		}
		////////////////////////////////////////////////////
		Json::Reader						reader;
		Json::Value							root;
		Json::Value							val;
		////////////////////////////////////////////////////
		Json::Value							st0;

		int									a;

		////////////////////////////////////////////////////
		std::string							key;
		std::string							data;

		std::string							code;
		double								rate;
		////////////////////////////////////////////////////
		std::string							str = buffer;
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
					code	= "";
					rate	= -1;
					st0		= itr0.key();
					a		= itr0.key().type();

					switch (a)
					{
					case stringValue:
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
									color_printf(YELLOW,"%d)%s:%f\n",idxx++, code.c_str(), rate);
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
		delete[] buffer;
		return true;
	}
	return false;
}

 
CController::~CController()
{
	for (auto const& x : m_Coins)
	{
		delete x.second;
	}
	m_Coins.clear();
	delete m_pAudioPlayer;
	SDL_Quit();
}
