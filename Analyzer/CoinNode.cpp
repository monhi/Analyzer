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
	m_btcMinRatio		= 0.99;
	m_btcMaxRatio		= 1.01;
	m_maxPrice			= 0;
	m_minPrice			= 0;
	m_BTCMin			= 1000000000;
	m_BTCMax			= 0;
	m_BTCReported		= false;

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
	m_last = r;
	m_empty  = false;
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
}


void CCoinNode::Reset()
{
	double last = m_last;
	Invalidate();
	AddRate(last);
}


int CCoinNode::ProcessBTC(std::string & name, double & rate,double& min_max_rate,double& percent ,bool & report)
{

	if ( m_CoinName == "BTC" )
	{
		if (m_BTCMin > m_BTCRate)
		{
			m_BTCMin = m_BTCRate;
		}
		
		if (m_BTCMax < m_BTCRate)
		{
			m_BTCMax = m_BTCRate;
		}

		if ( (m_BTCRate/m_BTCMin) > m_btcMaxRatio)
		{
			name = m_CoinName;
			rate = m_last;
			min_max_rate = m_BTCMin;
			percent = m_BTCRate / m_BTCMin;
			if (m_BTCReported == false)
			{
				report = true;
				m_BTCReported = true;
			}
			return SIGNAL;
		}

		if ( (m_BTCRate/m_BTCMax) < m_btcMinRatio)
		{
			name = m_CoinName;
			rate = m_BTCRate;
			min_max_rate = m_BTCMax;
			percent = m_BTCRate / m_BTCMax;
			if (m_BTCReported == false)
			{
				report = true;
				m_BTCReported = true;
			}
			return SIGNAL;
		}
		return SILENCE;
	}
	else
	{
		report = false;
		if (m_BTCRate == 0)
		{
			return SILENCE;
		}
		double l_rate = m_last / m_BTCRate;
		if (l_rate < m_BTCMin)
		{
			m_BTCMin = l_rate;
			m_minPrice = m_last;
		}
		if (l_rate > m_BTCMin * m_btcMaxRatio)
		{
			name = m_CoinName;
			rate = m_last;
			min_max_rate = m_minPrice;
			percent = l_rate / m_BTCMin;
			if (m_BTCReported == false)
			{
				report = true;
				m_BTCReported = true;
			}
			return SIGNAL;
		}

		if (l_rate > m_BTCMax)
		{
			m_BTCMax = l_rate;
			m_maxPrice = m_last;
		}

		if (l_rate < m_BTCMax * m_btcMinRatio)
		{
			name = m_CoinName;
			rate = m_last;
			min_max_rate = m_maxPrice;
			percent = l_rate / m_BTCMax;
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
}


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
