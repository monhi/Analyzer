#include "stdafx.h"
#include "CoinNode.h"
#include "Controller.h"
#include <cmath>
#include "general.h"
#include "CryptoDB.h"

CCoinNode::CCoinNode(CController* ptr,std::string name)
{
	m_pParent			= ptr;
	m_CoinName			= name;
	m_btcRatio			= 0.97;
	m_startRatio		= 1.05;
	m_hitRatio			= 1.1;	
	m_maxBTCPrice		= 0;
	m_BTCReported		= false;
	//m_SDRatio			= 0.02;
	m_max_total_reported	= false;
	m_max_hit_reported		= false;
	m_db		= new CCryptoDB();
	m_db->OpenDB();
	m_db->GetInfo(m_CoinName, m_max_total, m_max_hit);
	Invalidate();
}

void CCoinNode::AddRate(double r)
{
	m_buffer[m_front].rate	 = r;
	m_buffer[m_front].active = true;
	m_front++;
	m_empty  = false;

	m_size++;
	m_last   = r;

	if (r < m_min)
	{
		m_min = r;
	}

	if (r > m_max)
	{
		m_max = r;
	}
	m_front	%= Q_SIZE;
	//CalcMean();
	//CalcStandardDeviation();
}

void CCoinNode::SetBTCRate(double rate)
{
	m_BTCRate = rate;
}

void CCoinNode::Invalidate()
{
	for (int i = 0; i < Q_SIZE; i++)
	{
		m_buffer[i].active = false;
		m_buffer[i].rate = 0;
	}

	m_empty					= true;
	m_size					=  0;
	m_front					=  0;
	m_mean					= -1;
	m_sd					= -1;
	m_min					=  100000000000;
	m_max					= -1;

	m_last					= -1;
	m_max_total_reported	= false;
	m_max_hit_reported		= false;
	m_max_report			= false;
}

/*
double CCoinNode::GetPrivElem()
{
int idx = m_front-1;
if (idx == -1)
{
idx = Q_SIZE - 1;
}
return m_buffer[idx].rate;
}
*/

void CCoinNode::Reset()
{
	double last = m_last;
	Invalidate();
	AddRate(last);
}

/*
int CCoinNode::ProcessMin(std::string& name, double& rate, double& percent, double& hitrate, bool& report)
{
	report = false;
	hitrate = 0;
	percent = 0;
	rate = 0;
	double last = m_last;

	if (m_empty)
	{
		return SILENCE;
	}

	if (m_last > m_min)
	{
		return SILENCE;
	}
	return SILENCE;
}
*/
int CCoinNode::ProcessBTC(std::string & name, double & rate,double& max_rate,double& percent ,bool & report)
{
	report = false;
	if (m_BTCRate == 0)
	{
		return SILENCE;
	}
	double l_rate = m_last / m_BTCRate;
	if (l_rate > m_BTCMax)
	{
		m_BTCMax		= l_rate;
		m_maxBTCPrice	= m_last;
	}
	if (l_rate < m_BTCMax * m_btcRatio)
	{
		name		= m_CoinName;
		rate		= m_last;
		max_rate	= m_maxBTCPrice;
		percent		= l_rate / m_BTCMax;
		if (m_BTCReported == false)
		{
			report = true;
			m_BTCReported = true;
		}
		return SIGNAL;
	}
	m_BTCReported = false;
	return SILENCE;
}

/*
int CCoinNode::ProcessMax(std::string& name, double& rate, double& percent,double& hitrate,bool& report)
{
	report  = false;
	hitrate = 0;
	percent = 0;
	rate    = 0;
	double last = m_last;

	if (m_empty)
	{
		return SILENCE;
	}
		
	if ((m_last == m_min) && (m_size > 1))
	{
		// Signal to sell the element.
		Invalidate();
		AddRate(last);
		return SILENCE;
	}


	if (m_last > m_min*(m_startRatio))
	{
		// Signal that everything
		name	=  m_CoinName;
		rate	= m_last;
		percent = m_last / m_min;
		if (m_max_report == false)
		{
			m_max_report = true;
			report = true;
		}
		else
		{
			report = false;
		}

		if (m_max_total_reported == false)
		{
			m_max_total_reported = true;
			m_max_total++;
			m_db->UpdateInfo(m_CoinName, m_max_total, m_max_hit);
		}

		if (percent >= m_hitRatio)
		{
			if ( m_max_hit_reported == false )
			{
				m_max_hit_reported = true;
				m_max_hit++;
				m_db->UpdateInfo(m_CoinName, m_max_total, m_max_hit);
			}
		}

		if (m_max_total != 0)
		{
			hitrate  = m_max_hit;
			hitrate /= m_max_total;
		}
		else
		{
			hitrate = 0;
		}	
		return SIGNAL;
	}
	return SILENCE;
}
*/
/*
void CCoinNode::CalcMean()
{
	double mean		   = 0;
	int	   activeCntr  = 0;
	for (int i = 0; i < Q_SIZE; i++)
	{
		if (m_buffer[i].active)
		{
			mean += m_buffer[i].rate;
			activeCntr++;
		}		
	}
	if (activeCntr)
	{
		m_mean = mean / activeCntr;
	}
	else
	{
		m_mean = -1; // invalid mean value.
	}		
}
*/
/*
void CCoinNode::CalcStandardDeviation()
{
	double standardDeviation = 0;
	int	   activeCntr		 = 0;	

	for (int i = 0; i < Q_SIZE; i++) 
	{
		if (m_buffer[i].active)
		{
			standardDeviation += pow(m_buffer[i].rate - m_mean, 2);
			activeCntr++;
		}		
	}
	if (activeCntr)
	{
		m_sd = sqrt(standardDeviation / activeCntr);
	}
	else
	{
		m_sd = -1;
	}
}
*/
CCoinNode::~CCoinNode()
{
	m_db->CloseDB();
	delete m_db;
}
