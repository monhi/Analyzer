#include "stdafx.h"
#include "CoinNode.h"
#include "Controller.h"
#include <cmath>

CCoinNode::CCoinNode(CController* ptr,std::string name)
{
	m_pParent	= ptr ;
	m_CoinName	= name;
	m_ratio		= 0.02;
	m_SDRatio   = 0.02;
	Invalidate();
}

void CCoinNode::AddRate(double r)
{
	m_buffer[m_front].rate	 = r;
	m_buffer[m_front].active = true;
	m_front++;
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
	CalcMean();
	CalcStandardDeviation();
}

void CCoinNode::Invalidate()
{
	for (int i = 0; i < Q_SIZE; i++)
	{
		m_buffer[i].active = false;
		m_buffer[i].rate = 0;
	}
	m_front =  0;
	m_mean	= -1;
	m_sd	= -1;
	m_min	= 100000000000;
	m_max   = -1;
}

double CCoinNode::GetPrivElem()
{
	int idx = m_front-1;
	if (idx == -1)
	{
		idx = Q_SIZE - 1;
	}
	return m_buffer[idx].rate;
}

void CCoinNode::Process()
{
	double ftemp;
	if ( m_last < m_mean )
	{
		
		if (m_last == m_min)
		{
			// Signal to sell the element.
			Invalidate();
			AddRate(m_last);
		}
	}

	if (m_last > m_mean)
	{
		
		if (m_last > m_min*(1+m_ratio))
		{
			// Signal that everything
		}
		else
		{
			// do nothing.
		}
	}
}

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

CCoinNode::~CCoinNode()
{

}
