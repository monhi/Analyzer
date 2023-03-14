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

	bool					m_report;
	bool					m_empty;
	u32						m_size;
	double					m_mean;
	double					m_sd;

	double					m_min;
	double					m_max;
	double					m_maxPrice;
	double					m_minPrice;
	bool					m_BTCReported;
	/////////////////////////////////////
	int						m_max_hit;
	int						m_max_total;
	/////////////////////////////////////
	double					m_BTCRate;
	double					m_BTCMax;
	double					m_BTCMin;
	double					m_btcMinRatio;
	double					m_btcMaxRatio;
	/////////////////////////////////////
	int						m_total;
	int						m_hit;
	void					Invalidate();
public:
							CCoinNode(CController*,std::string);
							~CCoinNode();
	void					Reset();
	double					GetRate() { return m_last; }
	void					AddRate(double r);	
	int						ProcessBTC(std::string& name, double& rate,double & max_rate, double & percent ,bool& report);
	void					SetBTCRate(double rate);


};