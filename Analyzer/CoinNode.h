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
	double					m_startRatio;
	double					m_hitRatio;
	//double				m_SDRatio;
	double					m_min;
	double					m_max;
	int						m_total;
	int						m_hit;
	bool					m_total_reported;
	bool					m_hit_reported;

	void					CalcMean();
	void					CalcStandardDeviation();
	void					Invalidate();
//	double					GetPrivElem();	
public:
							CCoinNode(CController*,std::string);
							~CCoinNode();
	void					Reset();
	double					GetRate() { return m_last; }
	void					AddRate(double r);	
	int						Process(std::string& name,double& rate,double& percent,double& hitRate,bool& report);
};