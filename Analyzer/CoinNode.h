	#pragma once		
#include <string>
#include "general.h"

#define Q_SIZE 128



typedef struct{
	bool active;
	double rate;
}	RateNode;

class CCryptoDB;
class CController;
class CCoinNode
{
private:
	CCryptoDB			   *m_db;
	double					m_last;
	CController*			m_pParent;
	std::string				m_CoinName;
	RateNode				m_buffer[Q_SIZE];
	int						m_front;
	bool					m_max_report;
	bool					m_empty;
	u32						m_size;
	double					m_mean;
	double					m_sd;
	double					m_startRatio;
	double					m_hitRatio;
	double					m_min;
	double					m_max;
	double					m_maxBTCPrice;
	bool					m_BTCReported;
	/////////////////////////////////////
	int						m_min_hit;
	int						m_min_total;
	bool					m_min_total_reported;
	bool					m_min_hit_reported;
	/////////////////////////////////////
	int						m_max_hit;
	int						m_max_total;
	bool					m_max_total_reported;
	bool					m_max_hit_reported;
	/////////////////////////////////////
	double					m_BTCRate;
	double					m_BTCMax;
	double					m_btcRatio;
	/////////////////////////////////////


	//void					CalcMean();
	//void					CalcStandardDeviation();
	void					Invalidate();
//	double					GetPrivElem();	
public:
							CCoinNode(CController*,std::string);
							~CCoinNode();
	void					Reset();
	double					GetRate() { return m_last; }
	void					AddRate(double r);	
	//int					ProcessMax(std::string& name, double& rate,double& percent,double& hitRate,bool& report);
	//int					ProcessMin(std::string& name, double& rate, double& percent, double& hitrate, bool& report);
	int						ProcessBTC(std::string& name, double& rate,double & max_rate, double & percent ,bool& report);
	void					SetBTCRate(double rate);
};