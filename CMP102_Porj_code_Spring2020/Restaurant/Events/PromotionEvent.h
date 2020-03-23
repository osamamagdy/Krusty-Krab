#pragma once
#include "Event.h"

class PromotionEvent : public Event  //class for the promotion event
{	
private :

	double ExMoney;

public:
	PromotionEvent(int eTime, int oID,double extra);

	virtual void Execute(Restaurant* pRest);	//override execute function




};