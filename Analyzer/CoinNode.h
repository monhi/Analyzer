#pragma once
#include <string>

#define Q_SIZE 128

typedef struct{
	bool active;
	bool rate;
}	RateNode;

class CController;
class CCoinNode
{
private:
	double		 m_last;
	CController* m_pParent;
	std::string	 m_CoinName;
	RateNode	 m_buffer[Q_SIZE];
	int			 m_front;
	double		 m_mean;
	double		 m_sd;
	double		 m_ratio;
	double		 m_SDRatio;
	double		 m_min;
	double		 m_max;
	void		 CalcMean();
	void		 CalcStandardDeviation();
	void		 Process();
	void		 Invalidate();
	double		 GetPrivElem();	
public:
	CCoinNode(CController*,std::string);
	~CCoinNode();
	void AddRate(double r);	
};

